using System;
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
        Scope_thread scope;
        //enum paintMode { IDLE,SEARCHING,DEV_FOUND,CONNECTING,DISCONECTED,CONNECTED,COMM_ERR,SCOPE };
        //paintMode mode;

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

            this.panel5.Enabled = false;

            GUITimer.Elapsed += new ElapsedEventHandler(Update_GUI);
            GUITimer.Interval = 50;

            comm = new Thread(new ThreadStart(sc.comm_run));
            scope_th = new Thread(new ThreadStart(scope.run));
            mode = Paint_mode.Mode.IDLE;
            scope.scopePane = zedGraphControl_scope.GraphPane;
            scope.trig_level = this.trackBar_trig_level.Value;
            scope.pretrig = this.trackBar_pretrig.Value;
 
            comm.Start();

        }


        private void Update_GUI(object sender, ElapsedEventArgs e)
        {
            if (sc.is_connected() && sc.is_new_scope_data())
            {
                scope.new_data(sc.get_scope_data());
            }
            if (mode == Paint_mode.Mode.SCOPE)
            {
                if (scope.is_invalid())
                {
                    //zedGraphControl_scope.AxisChange();
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
                    this.mode = Paint_mode.Mode.IDLE;
                    break;
                case Paint_mode.Mode.DISCONECTED:
                    break;

                case Paint_mode.Mode.COMM_ERR:
                    break;

                case Paint_mode.Mode.SCOPE:
                    if (scope.verCurEN) {
                        this.label_cur_time_a.Text = scope.timeA;
                        this.label_cur_time_b.Text = scope.timeB;
                        this.label_time_diff.Text = scope.timeDif;
                        this.label_cur_freq.Text = scope.freq;
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
                    if (this.checkBox_RMS.Checked) { 
                        double RMS=0;
                        for (int i = 0; i < scope.buffLenght; i++)
                        {
                            RMS += scope.signal[i] * scope.signal[i];
                        }
                        RMS = Math.Sqrt(RMS / scope.buffLenght);

                        this.checkBox_RMS.Text = "RMS " + (Math.Round(RMS, 3)).ToString()+" V";
                    }
                    
                    zedGraphControl_scope.Refresh();
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
                sc.disconnect_device();
                disconnect();
                this.mode = Paint_mode.Mode.DISCONECTED;

            }
        }




        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (this.mode == Paint_mode.Mode.SCOPE) {
                scope.update_mode(Paint_mode.Mode.IDLE);
                scope_th.Join();
                sc.set_scope_stop();
            }
            this.mode = Paint_mode.Mode.IDLE;
            sc.comm_stop();
        }

        private void checkBox_scope_enable_CheckedChanged(object sender, EventArgs e)
        {
            if (this.checkBox_scope_enable.Checked)
            {
                this.mode = Paint_mode.Mode.SCOPE;
                scope.update_mode(mode);

                scope_th = new Thread(new ThreadStart(scope.run));

                scope_th.Start();
                
                    

                sc.set_scope_trigger_edge(Defines.RISE);
                sc.set_scope_trigger_type(Defines.SCOPE_TRIG_AUTO);
                sc.set_scope_sampling_freq(Defines.FREQ_10K);
                

                scope.buffLenght = sc.get_dev_configuration().scopeBuffLenght;
                scope.accuracy = (double)(sc.get_dev_configuration().vref_mv) / 255;
                scope.time = new double[scope.buffLenght];
                scope.update_time_base();
                scope.signal = new double[scope.buffLenght];

                //sc.set_scope_pretrig(sc.get_dev_configuration().scopeBuffLenght * scope.pretrig / 100);
                //sc.set_scope_trigger_level(scope.trig_level);
                sc.set_scope_start();

            }
            else {
                this.mode = Paint_mode.Mode.IDLE;
                scope.update_mode(mode);
                scope_th.Join();
                sc.set_scope_stop();
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
                    throw new System.ArgumentException("Parameter cannot be greatherthen 4096", "original");
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
                    throw new System.ArgumentException("Parameter cannot be greatherthen 4096", "original");
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


        private void tic() {
            stopWatch = new Stopwatch();
            stopWatch.Start();
        }

        private void toc(string s) {
            stopWatch.Stop();

            Console.WriteLine(s+" " +stopWatch.ElapsedMilliseconds.ToString() + "ms");
            stopWatch.Start();
        }

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


        private void disconnect() {
            this.toolStripStatusLabel_status_color.BackColor = Color.Red;
            this.toolStripStatusLabel_status.Text = "Disconnected";
            this.label_device_connected.Text = "No Device connected";
            this.toolStripStatusLabel_device.Text = "None";
            this.button_connect.Text = "Connect";
            this.button_scan.Enabled = true;
        }

        private void validate_general() {
            Device.config c = sc.get_dev_configuration();
            this.label18.Text = (c.scopeMaxf / 1000) + " kHz";
            this.label21.Text = c.scopeDept + " bits";
            this.label23.Text = c.scopeBuffLenght + " samples";
            this.label12.Text = c.vref_mv + " mV";
        }
        private void validate_scope(){
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
                sc.set_scope_trigger_type(Defines.SCOPE_TRIG_NORMAL);
                sc.set_scope_start();
            }
        }

        private void radioButton_trig_auto_CheckedChanged(object sender, EventArgs e)
        {
            if (this.radioButton_trig_auto.Checked)
            {
                sc.set_scope_trigger_type(Defines.SCOPE_TRIG_AUTO);
                sc.set_scope_start();
            }
        }

        private void radioButton_trig_single_Click(object sender, EventArgs e)
        {
            if (this.radioButton_trig_single.Checked)
            {
                sc.set_scope_trigger_type(Defines.SCOPE_TRIG_SINGLE);
                sc.set_scope_start();
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





    }
}
