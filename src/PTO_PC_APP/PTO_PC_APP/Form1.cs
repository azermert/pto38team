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
        Thread scope_zed;
        enum paintMode { IDLE,SEARCHING,DEV_FOUND,CONNECTING,DISCONECTED,CONNECTED,COMM_ERR,SCOPE };
        paintMode mode;

        //scope promenne
        GraphPane scopePane;

        int trig_level = 0;
        int pretrig = 100;
        int buffLenght = 0;
        double scale = 1;
        double horPosition = 0.5;

        double verPosition = 0;
        double accuracy = 0;  //prepocet z bitu na volty
        double range = 5;
        int maxRange = 5; //V;

        bool horCurEN = false;
        double horCursorA = 0;
        double horCursorB = 0;
        string timeA = "";
        string timeB = "";
        string timeDif = "";
        string freq = "";


        bool verCurEN = false;
        double verCursorA = 0;
        double verCursorB = 0;
        string voltA = "";
        string voltB = "";
        string voltDif = "";
        int samplingfreq = 10000;
        double[] signal;
        byte[] meas;
        double[] time;
        double maxTime = 0;

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

            scopePane = zedGraphControl_scope.GraphPane;

            this.panel5.Enabled = false;
            

            GUITimer.Elapsed += new ElapsedEventHandler(Update_GUI);
            GUITimer.Interval = 50;

            comm = new Thread(new ThreadStart(sc.comm_run));
            scope_zed = new Thread(new ThreadStart(zed_scope_thread));
            mode = paintMode.IDLE;

            this.trig_level = this.trackBar_trig_level.Value;
            this.pretrig = this.trackBar_pretrig.Value;

            
            comm.Start();
        }

        public void zed_scope_thread() {
            while(this.mode==paintMode.SCOPE){
                if (sc.is_connected() && sc.is_new_scope_data())
                {
                    generate_time_base(samplingfreq, buffLenght);
                    meas = sc.get_scope_data();
                    for (int i = 0; i < buffLenght; i++) {
                        signal[i] = accuracy * meas[i] / 1000;
                    }
                }

                //osy grafu
                scopePane.YAxis.Scale.MaxAuto = false;
                scopePane.YAxis.Scale.MinAuto = false;

                scopePane.XAxis.Scale.MaxAuto = false;
                scopePane.XAxis.Scale.MinAuto = false;

                double interval = scale * maxTime;
                double posmin = (interval / 2) ;
                double posScale = (maxTime - interval) / maxTime;

                double maxX = (maxTime) * horPosition * posScale + posmin + interval / 2;
                double minX = (maxTime) * horPosition * posScale + posmin - interval / 2;

                interval = range;
                posmin = (interval / 2) ;
                posScale = (maxRange - interval) / maxRange;

                double maxY = maxRange * verPosition * posScale + posmin + interval / 2;
                double minY = maxRange * verPosition * posScale + posmin - interval / 2;

                scopePane.XAxis.Scale.Max = maxX;
                scopePane.XAxis.Scale.Min = minX;

                scopePane.YAxis.Scale.Max = maxY;
                scopePane.YAxis.Scale.Min = minY;

                //vykresleni prubehu
                scopePane.CurveList.Clear();
                LineItem curve = scopePane.AddCurve("", time, signal, Color.Red, SymbolType.Diamond);
                curve.Symbol.Size = 2;
                curve.Line.IsSmooth = true;
                curve.Line.SmoothTension = 0.5F;

                //zoom position
                PointPairList list1 = new PointPairList();
                list1.Add( (maxTime) * horPosition,maxY);
                curve = scopePane.AddCurve("", list1, Color.Red, SymbolType.TriangleDown);
                curve.Symbol.Size = 15;
                curve.Symbol.Fill.Color = Color.Red;
                curve.Symbol.Fill.IsVisible = true;

                //trigger time
                list1 = new PointPairList();
                list1.Add((maxTime) * pretrig/100, maxY);
                curve = scopePane.AddCurve("", list1, Color.Blue, SymbolType.TriangleDown);
                curve.Symbol.Size = 20;
                curve.Symbol.Fill.Color = Color.Blue;
                curve.Symbol.Fill.IsVisible = true;

                //triggerlevel
                list1 = new PointPairList();
                list1.Add(minX, trig_level/16*accuracy/1000);
                curve=scopePane.AddCurve("", list1, Color.Green, SymbolType.Diamond);
                curve.Symbol.Size = 15;
                curve.Symbol.Fill.Color = Color.Green;
                curve.Symbol.Fill.IsVisible = true;

                //kurzory
                if (horCurEN) {
                    double uA = horCursorA * maxY + (1 - horCursorA) * minY;
                    double uB = horCursorB * maxY + (1 - horCursorB) * minY;
                    double ud = uA - uB;
                    this.voltDif = "dU " + (Math.Round(ud * 1000, 2)).ToString() + " mV";
                    this.voltA = "U " + (Math.Round(uA * 1000, 2)).ToString() + " mV";
                    this.voltB = "U " + (Math.Round(uB * 1000, 2)).ToString() + " mV";

                    list1 = new PointPairList();
                    list1.Add(minX, uA);
                    list1.Add(maxX, uA);
                    curve = scopePane.AddCurve("", list1, Color.DarkBlue, SymbolType.HDash);
                    curve.Symbol.Size = 0;
                    curve.Line.IsSmooth = true;
                    

                    list1 = new PointPairList();
                    list1.Add(minX, uB);
                    list1.Add(maxX, uB);
                    curve = scopePane.AddCurve("", list1, Color.DarkBlue, SymbolType.HDash);
                    curve.Symbol.Size = 0;
                    curve.Line.IsSmooth = true;
                }

                if (verCurEN)
                {
                    double tA = verCursorA * maxX + (1 - verCursorA) * minX;
                    double tB = verCursorB * maxX + (1 - verCursorB) * minX;
                    double td = tA - tB;
                    double f = 1 / td;
                    this.timeDif = "dt " + (Math.Round(td * 1000, 3)).ToString() + " ms";
                    this.timeA = "t " + (Math.Round(tA * 1000, 3)).ToString() + " ms";
                    this.timeB = "t " + (Math.Round(tB * 1000, 3)).ToString() + " ms";
                    this.freq = "f= " + Math.Abs((Math.Round(f / 1000, 3))).ToString() + " kHz";
                    
                    list1 = new PointPairList();
                    list1.Add(tA, minY);
                    list1.Add(tA, maxY);
                    curve = scopePane.AddCurve("", list1, Color.DarkBlue, SymbolType.HDash);
                    curve.Symbol.Size = 0;
                    curve.Line.IsSmooth = true;
                    

                    list1 = new PointPairList();
                    list1.Add(tB, minY);
                    list1.Add(tB, maxY);
                    curve = scopePane.AddCurve("", list1, Color.DarkBlue, SymbolType.HDash);
                    curve.Symbol.Size = 0;
                    curve.Line.IsSmooth = true;
                    
                }


                zedGraphControl_scope.AxisChange();
                Invalidate();
                zedGraphControl_scope.Invalidate();
                
                Thread.Yield();
                Thread.Sleep(10);
            }
        }

        private void Update_GUI(object sender, ElapsedEventArgs e) {
            this.Invalidate();
        }


        protected override void OnPaint(PaintEventArgs e)
        {
            switch (mode)
            {
                case paintMode.IDLE:
                    break;


                case paintMode.SEARCHING:
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
                            this.mode = paintMode.DEV_FOUND;
                        }
                    }
                    break;


                case paintMode.DEV_FOUND:
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


                case paintMode.CONNECTING:
                    if (sc.is_connected())
                    {
                        this.mode = paintMode.CONNECTED;
                    }
                    else {
                        watchdog++;
                        if (watchdog > 20) {
                            watchdog = 0;
                            this.mode = paintMode.COMM_ERR;
                            comm_error();
                            sc.disconnect_device();
                        }
                    }
                    break;


                case paintMode.CONNECTED:
                    connected();
                    validate_scope();
                    this.mode = paintMode.IDLE;
                    break;
                case paintMode.DISCONECTED:
                    break;

                case paintMode.COMM_ERR:
                    break;

                case paintMode.SCOPE:
                    if (verCurEN) {
                        this.label_cur_time_a.Text = timeA;
                        this.label_cur_time_b.Text = timeB;
                        this.label_time_diff.Text = timeDif;
                        this.label_cur_freq.Text = freq;
                    }

                    if (horCurEN) { 
                        this.label_volt_diff.Text = voltDif;
                        this.label_cur_u_a.Text = voltA;
                        this.label_cur_u_b.Text = voltB;
                    }

                    break;


                default:
                    break;
            }
        }

        //vypocet casove zakladny
        private void generate_time_base(int sampling, int lenght)
        {
            for (int i = 0; i < lenght; i++)
            {
                this.time[i] = (double)(i) / sampling;
            }

            maxTime = (double)(lenght) / sampling;
        }



        private void button_scan_Click(object sender, EventArgs e)
        {
            sc.find_dev_req();
            this.listBox_devices.Items.Clear();
            mode = paintMode.SEARCHING;
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
                    this.mode = paintMode.CONNECTING;

                }
            }
            else {
                sc.disconnect_device();
                disconnect();
                this.mode = paintMode.DISCONECTED;

            
            
            
            }
        }




        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (this.mode == paintMode.SCOPE) {
                sc.set_scope_stop();
            }
            this.mode = paintMode.IDLE;
            sc.comm_stop();
        }

        private void checkBox_scope_enable_CheckedChanged(object sender, EventArgs e)
        {
            if (this.checkBox_scope_enable.Checked)
            {
                this.mode = paintMode.SCOPE;

                scope_zed = new Thread(new ThreadStart(zed_scope_thread));

                scope_zed.Start();
                
                    

                sc.set_scope_trigger_edge(Defines.RISE);
                sc.set_scope_trigger_type(Defines.SCOPE_TRIG_NORMAL);
                sc.set_scope_sampling_freq(Defines.FREQ_100K);
                

                buffLenght = sc.get_dev_configuration().scopeBuffLenght;
                accuracy = (double)(sc.get_dev_configuration().vref_mv) / 255;
                this.time = new double[buffLenght];
                generate_time_base(100000, buffLenght);
                signal = new double[buffLenght];
                
                sc.set_scope_pretrig( sc.get_dev_configuration().scopeBuffLenght * this.pretrig / 100);
                sc.set_scope_trigger_level(this.trig_level);
                sc.set_scope_start();

            }
            else {
                this.mode = paintMode.IDLE;
                scope_zed.Join();
                scope_zed.Abort();
                sc.set_scope_stop();
            }
        }

        //svazani posivniku s text boxem a odesilani hgodnot trigger
        private void trackBar_trig_level_MouseUp(object sender, MouseEventArgs e)
        {
            if (this.trig_level != this.trackBar_trig_level.Value)
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
                this.trig_level = val;
                sc.set_scope_trigger_level(this.trig_level);

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
                this.pretrig = val;
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
            if (this.pretrig != this.trackBar_pretrig.Value)
            {
                this.maskedTextBox_pretrig.Text = this.trackBar_pretrig.Value.ToString();
            }
        }


        private void tic() {
            stopWatch = new Stopwatch();
            stopWatch.Start();
        }

        private void toc() {
            stopWatch.Stop();
            Console.WriteLine(stopWatch.ElapsedMilliseconds.ToString() + "ms");
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

        private void validate_scope(){
            Device.config c = sc.get_dev_configuration();
            this.label18.Text = (c.scopeMaxf / 1000) + " kHz";
            this.label21.Text = c.scopeDept + " bits";
            this.label23.Text = c.scopeBuffLenght+" samples";
            this.panel5.Enabled = true;

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
                samplingfreq = 5000000;
                sc.set_scope_sampling_freq(Defines.FREQ_5M);
            }
        }

        private void radioButton_2m_CheckedChanged(object sender, EventArgs e)
        {
            if (this.radioButton_2m.Checked)
            {
                samplingfreq = 2000000;
                sc.set_scope_sampling_freq(Defines.FREQ_2M);
            }
        }

        private void radioButton_1m_CheckedChanged(object sender, EventArgs e)
        {
            if (this.radioButton_1m.Checked)
            {
                samplingfreq = 1000000;
                sc.set_scope_sampling_freq(Defines.FREQ_1M);
            }
        }

        private void radioButton_500k_CheckedChanged(object sender, EventArgs e)
        {
            if (this.radioButton_500k.Checked)
            {
                samplingfreq = 500000;
                sc.set_scope_sampling_freq(Defines.FREQ_500K);
            }
        }

        private void radioButton_200k_CheckedChanged(object sender, EventArgs e)
        {
            if (this.radioButton_200k.Checked)
            {
                samplingfreq = 200000;
                sc.set_scope_sampling_freq(Defines.FREQ_200K);
            }
        }

        private void radioButton_100k_CheckedChanged(object sender, EventArgs e)
        {
            if (this.radioButton_100k.Checked)
            {
                samplingfreq = 100000;
                sc.set_scope_sampling_freq(Defines.FREQ_100K);
            }
        }

        private void radioButton_50k_CheckedChanged(object sender, EventArgs e)
        {
            if (this.radioButton_50k.Checked)
            {
                samplingfreq = 50000;
                sc.set_scope_sampling_freq(Defines.FREQ_50K);
            }
        }

        private void radioButton_20k_CheckedChanged(object sender, EventArgs e)
        {
            if (this.radioButton_20k.Checked)
            {
                samplingfreq = 20000;
                sc.set_scope_sampling_freq(Defines.FREQ_20K);
            }
        }

        private void radioButton_10k_CheckedChanged(object sender, EventArgs e)
        {
            if (this.radioButton_10k.Checked)
            {
                samplingfreq = 10000;
                sc.set_scope_sampling_freq(Defines.FREQ_10K);
            }
        }

        private void radioButton_5k_CheckedChanged(object sender, EventArgs e)
        {
            if (this.radioButton_5k.Checked)
            {
                samplingfreq = 5000;
                sc.set_scope_sampling_freq(Defines.FREQ_5K);
            }
        }

        private void radioButton_2k_CheckedChanged(object sender, EventArgs e)
        {
            if (this.radioButton_2k.Checked)
            {
                samplingfreq = 2000;
                sc.set_scope_sampling_freq(Defines.FREQ_2K);
            }
        }

        private void radioButton_1k_CheckedChanged(object sender, EventArgs e)
        {
            if (this.radioButton_1k.Checked)
            {
                samplingfreq = 1000;
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
            this.scale = 1.0 - (double)(this.trackBar_zoom.Value) / (this.trackBar_zoom.Maximum - this.trackBar_zoom.Minimum + 10);
        }

        private void trackBar_position_ValueChanged(object sender, EventArgs e)
        {
            this.horPosition = (double)(this.trackBar_position.Value) / (this.trackBar_position.Maximum - this.trackBar_position.Minimum);
        }

        private void trackBar_vol_level_ValueChanged(object sender, EventArgs e)
        {
            this.verPosition = (double)(this.trackBar_vol_level.Value) / (this.trackBar_vol_level.Maximum - this.trackBar_vol_level.Minimum);
        }

        private void radioButton_1mv_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_1mv.Checked) {
                this.range = 0.001;
            }
        }

        private void radioButton_2mv_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_2mv.Checked)
            {
                this.range = 0.002;
            }
        }

        private void radioButton_5mv_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_5mv.Checked)
            {
                this.range = 0.005;
            }
        }

        private void radioButton_10mv_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_10mv.Checked)
            {
                this.range = 0.01;
            }
        }

        private void radioButton_20mv_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_20mv.Checked)
            {
                this.range = 0.02;
            }
        }

        private void radioButton_50mv_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_50mv.Checked)
            {
                this.range = 0.05;
            }
        }

        private void radioButton_100mv_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_100mv.Checked)
            {
                this.range = 0.1;
            }
        }

        private void radioButton_200mv_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_200mv.Checked)
            {
                this.range = 0.2;
            }
        }

        private void radioButton_500mv_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_500mv.Checked)
            {
                this.range = 0.5;
            }
        }

        private void radioButton_1v_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_1v.Checked)
            {
                this.range = 1;
            }
        }

        private void radioButton_2v_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_2v.Checked)
            {
                this.range = 2;
            }
        }

        private void radioButton_5v_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButton_5v.Checked)
            {
                this.range = 5;
            }
        }

        private void checkBox_ver_cur_en_CheckedChanged(object sender, EventArgs e)
        {
           this.verCurEN=this.checkBox_ver_cur_en.Checked;
        }

        private void checkBox_hor_cur_en_CheckedChanged(object sender, EventArgs e)
        {
            this.horCurEN = this.checkBox_hor_cur_en.Checked;
        }

        private void trackBar_ver_cur_a_ValueChanged(object sender, EventArgs e)
        {
            this.verCursorA = (double)(this.trackBar_ver_cur_a.Value) / (this.trackBar_ver_cur_a.Maximum - this.trackBar_ver_cur_a.Minimum);
        }

        private void trackBar_ver_cur_b_ValueChanged(object sender, EventArgs e)
        {
            this.verCursorB = (double)(this.trackBar_ver_cur_b.Value) / (this.trackBar_ver_cur_b.Maximum - this.trackBar_ver_cur_b.Minimum);
        }

        private void trackBar_hor_cur_a_ValueChanged(object sender, EventArgs e)
        {
            this.horCursorA = (double)(this.trackBar_hor_cur_a.Value) / (this.trackBar_hor_cur_a.Maximum - this.trackBar_hor_cur_a.Minimum);
        }

        private void trackBar_hor_cur_b_ValueChanged(object sender, EventArgs e)
        {
            this.horCursorB = (double)(this.trackBar_hor_cur_b.Value) / (this.trackBar_hor_cur_b.Maximum - this.trackBar_hor_cur_b.Minimum);
  
        }












        




    }
}
