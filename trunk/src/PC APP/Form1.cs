﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Timers;
using System.Threading;
using System.Xml;
using ZedGraph;
using System.Diagnostics;

namespace PTO_PC_APP
{
    public partial class Form1 : Form
    {

        Serial_comm sc = new Serial_comm();
        System.Timers.Timer GUITimer = new System.Timers.Timer(50);
        Thread comm;
        Thread scope_th;
        Thread gen_th;
        int scopeWatchDog = 0;

        Scope_thread scope;
        Generator_thread generator;

        Paint_mode.Mode mode;

        Stopwatch stopWatch;
        private int watchdog = 0;
       
        public Form1()
        {
            InitializeComponent();
            zedGraphControl_scope.MasterPane[0].IsFontsScaled = false;
            zedGraphControl_scope.MasterPane[0].Title.IsVisible = false;
            zedGraphControl_scope.MasterPane[0].XAxis.MajorGrid.IsVisible = true;
            zedGraphControl_scope.MasterPane[0].XAxis.Title.IsVisible = false;

            zedGraphControl_scope.MasterPane[0].YAxis.MajorGrid.IsVisible = true;
            zedGraphControl_scope.MasterPane[0].YAxis.Title.IsVisible = false;
            scope = new Scope_thread();

            zedGraphControl_generator.MasterPane[0].IsFontsScaled = false;
            zedGraphControl_generator.MasterPane[0].Title.IsVisible = false;
            zedGraphControl_generator.MasterPane[0].XAxis.MajorGrid.IsVisible = true;
            zedGraphControl_generator.MasterPane[0].XAxis.Title.IsVisible = false;

            zedGraphControl_generator.MasterPane[0].YAxis.MajorGrid.IsVisible = true;
            zedGraphControl_generator.MasterPane[0].YAxis.Title.IsVisible = false;
            generator = new Generator_thread();

            this.panel5.Enabled = false;

            GUITimer.Elapsed += new ElapsedEventHandler(Update_GUI);
            GUITimer.Interval = 50;

            comm = new Thread(new ThreadStart(sc.comm_run));
            scope_th = new Thread(new ThreadStart(scope.run));
            gen_th = new Thread(new ThreadStart(generator.run));
            mode = Paint_mode.Mode.IDLE;
            scope.scopePane = zedGraphControl_scope.GraphPane;
            scope.trig_level = this.trackBar_trig_level.Value;
            scope.pretrig = this.trackBar_pretrig.Value;

            generator.generatorPane = zedGraphControl_generator.GraphPane;
 
            comm.Start();

        }


        private void Update_GUI(object sender, ElapsedEventArgs e)
        {
            if (scope.mode==Paint_mode.Mode.SCOPE && sc.is_connected())
            {
                if (sc.is_new_scope_data())
                {
                    scope.new_data(sc.get_scope_data());
                    if (scope.trig == Scope_thread.TriggerType.NORMAL)
                    {
                        sc.scope_start();
                        scope.trigShow = true;
                        scopeWatchDog = 0;
                    }
                }
                else
                {
                    if (scopeWatchDog > 25 && scope.trig==Scope_thread.TriggerType.NORMAL) {
                        sc.scope_start();
                        Console.WriteLine("Scope watch dog occurs");
                        scopeWatchDog = 0;
                    }
                    scopeWatchDog++;
                }
            }
            if (mode == Paint_mode.Mode.SCOPE)
            {
                if (scope.is_invalid())
                {
                    zedGraphControl_scope.Invalidate();
                }
            }
            this.Invalidate();
        }


        protected override void OnPaint(PaintEventArgs e)
        {
            switch (mode)
            {
                    
                case Paint_mode.Mode.IDLE:
                    break;


                case Paint_mode.Mode.SEARCHING:
                    if (sc.is_connecting_in_progress())
                    {
                        this.button_connect.Enabled = false;
                        this.toolStripStatusLabel_status.Text = "Serching for available devices";
                        this.toolStripProgressBar1.Value = sc.get_connecting_progress();
                    }
                    else
                    {
                        if (sc.is_new_devices())
                        {
                            this.mode = Paint_mode.Mode.DEV_FOUND;
                        }
                    }
                    break;


                case Paint_mode.Mode.DEV_FOUND:
                    if (sc.is_new_devices())
                    {
                        if (sc.get_num_of_devices() > 0)
                        {
                            this.toolStripStatusLabel_status.Text = "Searching done";
                            this.button_connect.Enabled = true;

                            for (int i = 0; i < sc.get_num_of_devices(); i++)
                            {
                                this.listBox_devices.Items.Add(sc.get_dev_names()[i]);
                            }
                            this.listBox_devices.SelectedIndex = 0;
                        }
                        else
                        {
                            this.toolStripStatusLabel_status.Text = "No device found";
                        }
                    }
                    break;


                case Paint_mode.Mode.CONNECTING:
                    if (sc.is_connected())
                    {
                        this.mode = Paint_mode.Mode.CONNECTED;
                    }
                    else {
                        watchdog++;
                        if (watchdog > 20) {
                            watchdog = 0;
                            this.mode = Paint_mode.Mode.COMM_ERR;
                            comm_error();
                            sc.disconnect_device();
                        }
                    }
                    break;


                case Paint_mode.Mode.CONNECTED:
                    connected();
                    validate_general();
                    validate_scope();
                    generator.set_gen_buff_lenght(sc.get_dev_configuration().genBuffLenght);
                    validate_generator();
                    this.mode = Paint_mode.Mode.IDLE;
                    break;
                case Paint_mode.Mode.DISCONECTED:
                    break;

                case Paint_mode.Mode.COMM_ERR:
                    break;

                case Paint_mode.Mode.SCOPE:
                    paint_scope();
                    zedGraphControl_scope.Refresh();
                    break;

                case Paint_mode.Mode.GENERATOR:
                   // paint_generator();
                    zedGraphControl_generator.Refresh();
                    break;
                default:
                    break;
            }

        }


        private void button_scan_Click(object sender, EventArgs e)
        {
            sc.find_dev_req();
            this.listBox_devices.Items.Clear();
            mode = Paint_mode.Mode.SEARCHING;
            if (!GUITimer.Enabled)
            {
                GUITimer.Enabled = true;
            }
            Thread.Sleep(10);

        }
        
        /*  Button Connect */
        private void button_connect_Click(object sender, EventArgs e)
        {
            if (this.button_connect.Text.Equals("Connect"))
            {
                string dev = (string)this.listBox_devices.SelectedItem;

                if (dev == null)
                {
                    MessageBox.Show("You have to select device first", "No device selected", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                {
                    if (dev[4] == ':')
                    {
                        dev = dev.Substring(0, 4);
                    }
                    else
                    {
                        dev = dev.Substring(0, 5);
                    }
                    sc.connect_device(dev);
                    this.toolStripStatusLabel_status.Text = "Connecting to " + dev;
                    this.mode = Paint_mode.Mode.CONNECTING;

                }
            }
            else
            {
                this.mode = Paint_mode.Mode.DISCONECTED;
                //vypnuti osciloskopu
                scope.update_mode(mode);
                if (scope_th.ThreadState!=System.Threading.ThreadState.Unstarted)
                {
                    scope_th.Join();
                }
                sc.scope_stop();
                this.checkBox_scope_enable.Checked = false;
                invalidate_scope();


                generator.update_mode(mode);
                if (gen_th.IsAlive)
                {
                    gen_th.Join();
                }
                sc.gen_stop();
                this.checkBox_gen_en.Checked = false;
                invalidate_generator();

                sc.disconnect_device();
                disconnect();
                
                
            }
        }



        /* zavirani okna */
        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (this.mode == Paint_mode.Mode.SCOPE) {
                scope.update_mode(Paint_mode.Mode.IDLE);
                scope_th.Join();
                sc.scope_stop();
            }


            if (this.mode == Paint_mode.Mode.GENERATOR)
            {
                generator.update_mode(Paint_mode.Mode.IDLE);
                gen_th.Join();
                sc.gen_stop();
            }
            this.mode = Paint_mode.Mode.IDLE;
            sc.comm_stop();
        }


        /*  Scope enable */
        private void checkBox_scope_enable_CheckedChanged(object sender, EventArgs e)
        {
            if (this.checkBox_scope_enable.Checked)
            {
                this.mode = Paint_mode.Mode.SCOPE;
                scope.update_mode(mode);

                scope_th = new Thread(new ThreadStart(scope.run));

                scope_th.Start();

                if (scope.firstRun)
                {
                    scope.firstRun = false;
                    sc.set_scope_trigger_edge(Defines.RISE);
                    sc.set_scope_trigger_type(Defines.SCOPE_TRIG_SINGLE);
                    sc.set_scope_sampling_freq(Defines.FREQ_10K);
                }

                scope.buffLenght = sc.get_dev_configuration().scopeBuffLenght;
                scope.accuracy = (double)(sc.get_dev_configuration().ScopeVref_mv) / 255;
                scope.time = new double[scope.buffLenght];
                scope.update_time_base();
                scope.signal = new double[scope.buffLenght];

                sc.set_scope_pretrig(sc.get_dev_configuration().scopeBuffLenght * scope.pretrig / 100);
                sc.set_scope_trigger_level(scope.trig_level);
                sc.scope_start();
            }
            else {
                this.mode = Paint_mode.Mode.IDLE;
                scope.update_mode(mode);
                scope_th.Join();
                sc.scope_stop();
            }
        }



        /* Generator enable*/
        private void checkBox_gen_en_CheckedChanged(object sender, EventArgs e)
        {
            if (this.checkBox_gen_en.Checked)
            {
                this.mode = Paint_mode.Mode.GENERATOR;
                generator.update_mode(mode);

                gen_th = new Thread(new ThreadStart(generator.run));

                gen_th.Start();
            }
            else {
                this.mode = Paint_mode.Mode.IDLE;
                generator.update_mode(mode);
                gen_th.Join();
                sc.gen_stop();
            }
        }



        //svazani posivniku s text boxem a odesilani hgodnot trigger
        private void trackBar_trig_level_MouseUp(object sender, MouseEventArgs e)
        {
            if (scope.trig_level != this.trackBar_trig_level.Value)
            {
                this.maskedTextBox_trig_level.Text = this.trackBar_trig_level.Value.ToString();
            }
        }

        private void maskedTextBox_trig_level_TextChanged(object sender, EventArgs e)
        {
            try {
                int val = int.Parse(this.maskedTextBox_trig_level.Text);
                if (val > 4096) {
                    throw new System.ArgumentException("Parameter cannot be greather then 4096", "original");
                }
                this.trackBar_trig_level.Value = val;
                scope.trig_level = val;
                sc.set_scope_trigger_level(scope.trig_level);
            }catch(Exception ex){
                this.maskedTextBox_trig_level.Text = this.trackBar_trig_level.Value.ToString();
            }
        }


        //svazani posivniku s text boxem a odesilani hodnot pretrigger
        private void maskedTextBox_pretrig_TextChanged(object sender, EventArgs e)
        {
            try
            {
                int val = int.Parse(this.maskedTextBox_pretrig.Text);
                if (val > 100)
                {
                    throw new System.ArgumentException("Parameter cannot be greather then 100", "original");
                }
                this.trackBar_pretrig.Value = val;
                scope.pretrig = val;
                int buffl = sc.get_dev_configuration().scopeBuffLenght * val / 100;
                sc.set_scope_pretrig(buffl);

            }
            catch (Exception ex)
            {
                this.maskedTextBox_pretrig.Text = this.trackBar_pretrig.Value.ToString();
            }
        }

        private void trackBar_pretrig_MouseUp(object sender, MouseEventArgs e)
        {
            if (scope.pretrig != this.trackBar_pretrig.Value)
            {
                this.maskedTextBox_pretrig.Text = this.trackBar_pretrig.Value.ToString();
            }
        }



        /* funkce pro mereni casu - pro debug*/
        private void tic() {
            stopWatch = new Stopwatch();
            stopWatch.Start();
        }

        private void toc(string s) {
            stopWatch.Stop();

            Console.WriteLine(s+" " +stopWatch.ElapsedMilliseconds.ToString() + "ms");
            stopWatch.Start();
        }



        /*
         Metody zdlouhave a nudne pro vykreslovani prvku
         */
        private void comm_error() {
            this.button_connect.Text = "Connect";
            this.label_device_connected.Text = "No device connected";
            this.toolStripStatusLabel_device.Text = "None";
            this.toolStripStatusLabel_status.Text = "Comunication error";
            this.toolStripStatusLabel_status_color.BackColor = Color.Red;
            this.button_scan.Enabled = true;
        }

        private void connected() {
            this.toolStripStatusLabel_status_color.BackColor = Color.LawnGreen;
            this.toolStripStatusLabel_device.Text = sc.device_string();
            this.toolStripStatusLabel_status.Text = "Sucsesfully connected to " + sc.device_port();
            this.label_device_connected.Text = sc.device_string();
            this.button_connect.Text = "Disconnect";
            this.button_scan.Enabled = false;
        }


        private void disconnect()
        {
            this.toolStripStatusLabel_status_color.BackColor = Color.Red;
            this.toolStripStatusLabel_status.Text = "Disconnected";
            this.label_device_connected.Text = "No Device connected";
            this.toolStripStatusLabel_device.Text = "None";
            this.button_connect.Text = "Connect";
            this.button_scan.Enabled = true;
        }

        private void validate_general()
        {
            Device.config c = sc.get_dev_configuration();
            this.label18.Text = (c.scopeMaxf / 1000) + " kHz";
            this.label21.Text = c.scopeDept + " bits";
            this.label23.Text = c.scopeBuffLenght + " samples";
            this.label12.Text = c.ScopeVref_mv + " mV";
            this.label41.Text = c.procCore;
            this.label37.Text = c.clock;
            this.label26.Text = c.comm;
            this.label53.Text = c.connection;
            this.label11.Text = c.scopePin;
        }
        private void validate_generator() {
            this.tableLayoutPanel14.Enabled = true;
        }


        private void paint_scope()
        {
            if (scope.verCurEN)
            {
                this.label_cur_time_a.Text = scope.timeA;
                this.label_cur_time_b.Text = scope.timeB;
                this.label_time_diff.Text = scope.timeDif;
                this.label_cur_freq.Text = scope.frequency;
                this.label_cur_ua.Text = scope.UA;
                this.label_cur_ub.Text = scope.UB;
                this.label_cur_du.Text = scope.DiffU;
            }
            if (scope.horCurEN)
            {
                this.label_volt_diff.Text = scope.voltDif;
                this.label_cur_u_a.Text = scope.voltA;
                this.label_cur_u_b.Text = scope.voltB;
            }
            if (this.checkBox_RMS.Checked)
            {
                this.checkBox_RMS.Text = "RMS " + (Math.Round(scope.RMS, 3)).ToString() + " V";
            }

            if (this.checkBox_mean.Checked)
            {
                this.checkBox_mean.Text = "Mean " + (Math.Round(scope.mean, 3)).ToString() + " V";
            }
            if (this.checkBox_max.Checked)
            {
                this.checkBox_max.Text = "Max " + (Math.Round(scope.Max, 3)).ToString() + " V";
            }
            if (this.checkBox_min.Checked)
            {
                this.checkBox_min.Text = "Min " + (Math.Round(scope.Min, 3)).ToString() + " V";
            }
            if (this.checkBox_PkPk.Checked)
            {
                this.checkBox_PkPk.Text = "Pk-Pk " + (Math.Round(scope.PkPk, 3)).ToString() + " V";
            }
            if (this.checkBox_duty.Checked)
            {
                this.checkBox_duty.Text = "Duty " + (Math.Round(scope.duty * 100, 1)).ToString() + " %";
            }
            if (this.checkBox_high.Checked)
            {
                this.checkBox_high.Text = "High " + (Math.Round(scope.High * 100, 1)).ToString() + " %";
            }
            if (this.checkBox_low.Checked)
            {
                this.checkBox_low.Text = "Low " + (Math.Round(scope.Low * 100, 1)).ToString() + " %";
            }
            if (this.checkBox_freq.Checked)
            {
                if (Double.IsInfinity(scope.Freq))
                {
                    this.checkBox_freq.Text = "Freq ??? Hz";
                }
                else if (scope.Freq >= 1000)
                {
                    this.checkBox_freq.Text = "Freq " + (Math.Round(scope.Freq / 1000, 2)).ToString() + " kHz";
                }
                else {
                    this.checkBox_freq.Text = "Freq " + (Math.Round(scope.Freq, 1)).ToString() + " Hz";
                }
            }
            if (this.checkBox_period.Checked)
            {
                if (Double.IsInfinity(scope.period))
                {
                    this.checkBox_period.Text = "Period ??? ms";
                }
                else
                {
                    this.checkBox_period.Text = "Per. " + (Math.Round(scope.period*1000, 2)).ToString() + " ms";
                }
            }

            if (scope.trigShow)
            {
                this.toolStripStatusLabel_status.Text = "Trig";
                scope.trigShow = false;
            }
            else
            {
                this.toolStripStatusLabel_status.Text = "";
            }
        }

        private void invalidate_scope()
        {
            this.panel5.Enabled = false;
        }

        private void invalidate_generator()
        {
            this.tableLayoutPanel14.Enabled = false;
        }

        private void validate_scope(){ 
            this.radioButton_5m.Enabled = true;
            this.radioButton_2m.Enabled = true;
            this.radioButton_1m.Enabled = true;
            this.radioButton_500k.Enabled = true;
            this.radioButton_200k.Enabled = true;
            this.radioButton_100k.Enabled = true;
            this.radioButton_50k.Enabled = true;
            this.radioButton_10k.Enabled = true;
            this.radioButton_20k.Enabled = true;

            this.radioButton_10k.Checked = true;
            this.radioButton_trig_normal.Checked = true;
            this.checkBox_trig_rise.Checked = true;
            this.checkBox_trig_fall.Checked = false;
            this.panel5.Enabled = true;
            Device.config c = sc.get_dev_configuration();
            if (c.scopeMaxf < 5000000) {
                this.radioButton_5m.Enabled = false;
            }
            if (c.scopeMaxf < 2000000)
            {
                this.radioButton_2m.Enabled = false;
            }
            if (c.scopeMaxf < 1000000)
            {
                this.radioButton_1m.Enabled = false;
            }
            if (c.scopeMaxf < 500000)
            {
              this.radioButton_500k.Enabled = false;
            }
            if (c.scopeMaxf < 200000)
            {
              this.radioButton_200k.Enabled = false;
            }
            if (c.scopeMaxf < 100000)
            {
              this.radioButton_100k.Enabled = false;
            }
            if (c.scopeMaxf < 50000)
            {
               this.radioButton_50k.Enabled = false;
            }
            if (c.scopeMaxf < 20000)
            {
                this.radioButton_20k.Enabled = false;
            }
            if (c.scopeMaxf < 10000)
            {
                this.radioButton_10k.Enabled = false;
            }
            if (c.scopeMaxf < 5000)
            {
                this.radioButton_5k.Enabled = false;
            }
            if (c.scopeMaxf < 2000)
            {
                this.radioButton_2k.Enabled = false;
            }  
        }


        /* button sampling calbacks */
        private void radioButton_5m_CheckedChanged(object sender, EventArgs e)
        {
            if (this.radioButton_5m.Checked) {
                scope.samplingfreq = 5000000;
                sc.set_scope_sampling_freq(Defines.FREQ_5M);
            }
        }

        private void radioButton_2m_CheckedChanged(object sender, EventArgs e)
        {
            if (this.radioButton_2m.Checked)
            {
                scope.samplingfreq = 2000000;
                sc.set_scope_sampling_freq(Defines.FREQ_2M);
            }
        }

        private void radioButton_1m_CheckedChanged(object sender, EventArgs e)
        {
            if (this.radioButton_1m.Checked)
            {
                scope.samplingfreq = 1000000;
                sc.set_scope_sampling_freq(Defines.FREQ_1M);
            }
        }

        private void radioButton_500k_CheckedChanged(object sender, EventArgs e)
        {
            if (this.radioButton_500k.Checked)
            {
                scope.samplingfreq = 500000;
                sc.set_scope_sampling_freq(Defines.FREQ_500K);
            }
        }

        private void radioButton_200k_CheckedChanged(object sender, EventArgs e)
        {
            if (this.radioButton_200k.Checked)
            {
                scope.samplingfreq = 200000;
                sc.set_scope_sampling_freq(Defines.FREQ_200K);
            }
        }

        private void radioButton_100k_CheckedChanged(object sender, EventArgs e)
        {
            if (this.radioButton_100k.Checked)
            {
                scope.samplingfreq = 100000;
                sc.set_scope_sampling_freq(Defines.FREQ_100K);
            }
        }

        private void radioButton_50k_CheckedChanged(object sender, EventArgs e)
        {
            if (this.radioButton_50k.Checked)
            {
                scope.samplingfreq = 50000;
                sc.set_scope_sampling_freq(Defines.FREQ_50K);
            }
        }

        private void radioButton_20k_CheckedChanged(object sender, EventArgs e)
        {
            if (this.radioButton_20k.Checked)
            {
                scope.samplingfreq = 20000;
                sc.set_scope_sampling_freq(Defines.FREQ_20K);
            }
        }

        private void radioButton_10k_CheckedChanged(object sender, EventArgs e)
        {
            if (this.radioButton_10k.Checked)
            {
                scope.samplingfreq = 10000;
                sc.set_scope_sampling_freq(Defines.FREQ_10K);
            }
        }

        private void radioButton_5k_CheckedChanged(object sender, EventArgs e)
        {
            if (this.radioButton_5k.Checked)
            {
                scope.samplingfreq = 5000;
                sc.set_scope_sampling_freq(Defines.FREQ_5K);
            }
        }

        private void radioButton_2k_CheckedChanged(object sender, EventArgs e)
        {
            if (this.radioButton_2k.Checked)
            {
                scope.samplingfreq = 2000;
                sc.set_scope_sampling_freq(Defines.FREQ_2K);
            }
        }

        private void radioButton_1k_CheckedChanged(object sender, EventArgs e)
        {
            if (this.radioButton_1k.Checked)
            {
                scope.samplingfreq = 1000;
                sc.set_scope_sampling_freq(Defines.FREQ_1K);
            }
        }

        private void checkBox_trig_rise_CheckedChanged(object sender, EventArgs e)
        {
            if (this.checkBox_trig_rise.Checked) {
                this.checkBox_trig_fall.Checked = false;
                sc.set_scope_trigger_edge(Defines.RISE);
            }
        }

        private void checkBox_trig_fall_CheckedChanged(object sender, EventArgs e)
        {
            if (this.checkBox_trig_fall.Checked)
            {
                this.checkBox_trig_rise.Checked = false;
                sc.set_scope_trigger_edge(Defines.FALL);
            }
        }

        private void radioButton_trig_normal_CheckedChanged(object sender, EventArgs e)
        {
            if (this.radioButton_trig_normal.Checked)
            {
                scope.set_trigger(Scope_thread.TriggerType.NORMAL);
                sc.set_scope_trigger_type(Defines.SCOPE_TRIG_SINGLE);
                sc.scope_start();
            }
        }

        private void radioButton_trig_auto_CheckedChanged(object sender, EventArgs e)
        {
            if (this.radioButton_trig_auto.Checked)
            {
                scope.set_trigger(Scope_thread.TriggerType.AUTO);
                sc.set_scope_trigger_type(Defines.SCOPE_TRIG_AUTO);
                sc.scope_start();
            }
        }

        private void radioButton_trig_single_Click(object sender, EventArgs e)
        {
            if (this.radioButton_trig_single.Checked)
            {
                scope.set_trigger(Scope_thread.TriggerType.SINGLE);
                sc.set_scope_trigger_type(Defines.SCOPE_TRIG_SINGLE);
                sc.scope_start();
            }
        }

        private void trackBar_zoom_ValueChanged(object sender, EventArgs e)
        {
            scope.scale = 1.0 - (double)(this.trackBar_zoom.Value) / (this.trackBar_zoom.Maximum - this.trackBar_zoom.Minimum + 10);
        }

        private void trackBar_position_ValueChanged(object sender, EventArgs e)
        {
            scope.horPosition = (double)(this.trackBar_position.Value) / (this.trackBar_position.Maximum - this.trackBar_position.Minimum);
        }

        private void trackBar_vol_level_ValueChanged(object sender, EventArgs e)
        {
            scope.verPosition = (double)(this.trackBar_vol_level.Value) / (this.trackBar_vol_level.Maximum - this.trackBar_vol_level.Minimum);
        }

        private void radioButton_1mv_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_1mv.Checked) {
                scope.range = 0.001;
            }
        }

        private void radioButton_2mv_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_2mv.Checked)
            {
                scope.range = 0.002;
            }
        }

        private void radioButton_5mv_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_5mv.Checked)
            {
                scope.range = 0.005;
            }
        }

        private void radioButton_10mv_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_10mv.Checked)
            {
                scope.range = 0.01;
            }
        }

        private void radioButton_20mv_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_20mv.Checked)
            {
                scope.range = 0.02;
            }
        }

        private void radioButton_50mv_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_50mv.Checked)
            {
                scope.range = 0.05;
            }
        }

        private void radioButton_100mv_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_100mv.Checked)
            {
                scope.range = 0.1;
            }
        }

        private void radioButton_200mv_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_200mv.Checked)
            {
                scope.range = 0.2;
            }
        }

        private void radioButton_500mv_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_500mv.Checked)
            {
                scope.range = 0.5;
            }
        }

        private void radioButton_1v_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_1v.Checked)
            {
                scope.range = 1;
            }
        }

        private void radioButton_2v_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_2v.Checked)
            {
                scope.range = 2;
            }
        }

        private void radioButton_5v_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_5v.Checked)
            {
                scope.range = 5;
            }
        }

        private void checkBox_ver_cur_en_CheckedChanged(object sender, EventArgs e)
        {
            scope.verCurEN = this.checkBox_ver_cur_en.Checked;
        }

        private void checkBox_hor_cur_en_CheckedChanged(object sender, EventArgs e)
        {
            scope.horCurEN = this.checkBox_hor_cur_en.Checked;
        }

        private void trackBar_ver_cur_a_ValueChanged(object sender, EventArgs e)
        {
            scope.verCursorA = (double)(this.trackBar_ver_cur_a.Value) / (this.trackBar_ver_cur_a.Maximum - this.trackBar_ver_cur_a.Minimum);
        }

        private void trackBar_ver_cur_b_ValueChanged(object sender, EventArgs e)
        {
            scope.verCursorB = (double)(this.trackBar_ver_cur_b.Value) / (this.trackBar_ver_cur_b.Maximum - this.trackBar_ver_cur_b.Minimum);
        }

        private void trackBar_hor_cur_a_ValueChanged(object sender, EventArgs e)
        {
            scope.horCursorA = (double)(this.trackBar_hor_cur_a.Value) / (this.trackBar_hor_cur_a.Maximum - this.trackBar_hor_cur_a.Minimum);
        }

        private void trackBar_hor_cur_b_ValueChanged(object sender, EventArgs e)
        {
            scope.horCursorB = (double)(this.trackBar_hor_cur_b.Value) / (this.trackBar_hor_cur_b.Maximum - this.trackBar_hor_cur_b.Minimum);
  
        }

        private void checkBox_RMS_CheckedChanged(object sender, EventArgs e)
        {
            if (!this.checkBox_RMS.Checked) {
                this.checkBox_RMS.Text = "RMS";
            }
        }

        private void checkBox_mean_CheckedChanged(object sender, EventArgs e)
        {
            if (!this.checkBox_mean.Checked)
            {
                this.checkBox_mean.Text = "Mean";
            }
        }

        private void checkBox_PkPk_CheckedChanged(object sender, EventArgs e)
        {
            if (!this.checkBox_PkPk.Checked)
            {
                this.checkBox_PkPk.Text = "Pk-Pk";
            }
        }

        private void checkBox_max_CheckedChanged(object sender, EventArgs e)
        {
            if (!this.checkBox_max.Checked)
            {
                this.checkBox_max.Text = "Max";
            }
        }

        private void checkBox_min_CheckedChanged(object sender, EventArgs e)
        {
            if (!this.checkBox_min.Checked)
            {
                this.checkBox_min.Text = "Min";
            }
        }

        private void checkBox_duty_CheckedChanged(object sender, EventArgs e)
        {
            if (!this.checkBox_duty.Checked)
            {
                this.checkBox_duty.Text = "Duty";
            }
        }

        private void checkBox_low_CheckedChanged(object sender, EventArgs e)
        {
            if (!this.checkBox_low.Checked)
            {
                this.checkBox_low.Text = "Low";
            }
        }

        private void checkBox_high_CheckedChanged(object sender, EventArgs e)
        {
            if (!this.checkBox_high.Checked)
            {
                this.checkBox_high.Text = "High";
            }
        }

        private void checkBox_freq_CheckedChanged(object sender, EventArgs e)
        {
            if (!this.checkBox_freq.Checked)
            {
                this.checkBox_freq.Text = "Freq";
            }
        }

        private void checkBox_period_CheckedChanged(object sender, EventArgs e)
        {
            if (!this.checkBox_period.Checked)
            {
                this.checkBox_period.Text = "Period";
            }
        }

        private void checkBox_points_CheckedChanged(object sender, EventArgs e)
        {
            scope.showPoints = this.checkBox_points.Checked;
        }

        private void radioButton_interp_CheckedChanged(object sender, EventArgs e)
        {
            scope.interpolation = this.radioButton_interp.Checked;
        }

        private void tabPage_device_Enter(object sender, EventArgs e)
        {
            Console.WriteLine("dev enter");
        }

        private void trackBar_freq_ValueChanged(object sender, EventArgs e)
        {
            generator.frequency = ((double)(this.trackBar_freq.Value) / 10); 
            this.textBox_freq.Text = generator.frequency.ToString();   
        }

        private void textBox_freq_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == Convert.ToChar(Keys.Enter))
            {
                try
                {
                    Double val = Double.Parse(this.textBox_freq.Text);
                    if (val > 1000)
                    {
                        throw new System.ArgumentException("Parameter cannot be greather then ", "original");
                    }
                    this.trackBar_freq.Value = (int)(val * 10);
                    generator.frequency = val;
                }
                catch (Exception ex)
                {
                    this.textBox_freq.Text = generator.frequency.ToString();
                }
            }
        }

        private void trackBar_ampl_ValueChanged(object sender, EventArgs e)
        {
            generator.amplitude= ((double)(this.trackBar_ampl.Value)/this.trackBar_ampl.Maximum*generator.v_ref);
            this.textBox_ampl.Text = generator.amplitude.ToString(); 
        }

        private void textBox_ampl_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == Convert.ToChar(Keys.Enter))
            {
                try
                {
                    Double val = Double.Parse(this.textBox_ampl.Text);
                    if (val > generator.v_ref)
                    {
                        throw new System.ArgumentException("Parameter cannot be greather then ", "original");
                    }
                    this.trackBar_ampl.Value = (int)(val/generator.v_ref*trackBar_ampl.Maximum);
                    generator.amplitude = val;
                }
                catch (Exception ex)
                {
                    this.textBox_ampl.Text = generator.amplitude.ToString();
                }
            }
        }

        private void trackBar_offset_ValueChanged(object sender, EventArgs e)
        {
            generator.offset = ((double)(this.trackBar_offset.Value) / this.trackBar_offset.Maximum * generator.v_ref);
            this.textBox_offset.Text = generator.offset.ToString(); 
        }

        private void textBox_offset_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == Convert.ToChar(Keys.Enter))
            {
                try
                {
                    Double val = Double.Parse(this.textBox_offset.Text);
                    if (val > generator.v_ref)
                    {
                        throw new System.ArgumentException("Parameter cannot be greather then ", "original");
                    }
                    this.trackBar_offset.Value = (int)(val / generator.v_ref * trackBar_offset.Maximum);
                    generator.offset = val;
                }
                catch (Exception ex)
                {
                    this.textBox_offset.Text = generator.offset.ToString();
                }
            }
        }

        private void trackBar_duty_ValueChanged(object sender, EventArgs e)
        {
            generator.duty = ((double)(this.trackBar_duty.Value) / this.trackBar_offset.Maximum * 100);
            this.textBox_duty.Text = generator.duty.ToString(); 
        }

        private void textBox_duty_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == Convert.ToChar(Keys.Enter))
            {
                try
                {
                    Double val = Double.Parse(this.textBox_duty.Text);
                    if (val > 100)
                    {
                        throw new System.ArgumentException("Parameter cannot be greather then ", "original");
                    }
                    this.trackBar_duty.Value = (int)(val / 100 * trackBar_duty.Maximum);
                    generator.duty = val;
                }
                catch (Exception ex)
                {
                    this.textBox_duty.Text = generator.duty.ToString();
                }
            }
        }



    }
}
