using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.IO.Ports;
using MCUmechanic.Properties;
using System.Threading;



namespace MCUmechanic
{

    public partial class Form1 : Form
    {
         SerialClient serialClient1;
        byte[] buf2;
        static bool openPort = false;
        SerialError spErr;
        private Thread trd;
        delegate void serialDelegate(object sglton);
        
        
        
        public Form1()
        {
            InitializeComponent();
            /*
             Zjisteni moznosti vyplyvajicich z konfigurace pc
             */
            string[] ports = SerialPort.GetPortNames();
            foreach(string port in ports)
            {
            listBox2.Items.Add(port);
            }
            if (listBox2.Items.Count > 0){
            listBox2.SelectedIndex = 0;}
            listBox3.SelectedIndex = 0;

        }
        
        

        private void Form1_Load(object sender, EventArgs e)
        {

            

        }

       
        

        


        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void tableLayoutPanel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void zedGraphControl1_Load(object sender, EventArgs e)
        {

        }
        private void receiveHandler(object sender, DataStreamEventArgs e)
        {
            int mode = 0;
            TimeSpan interval = TimeSpan.Parse("1000");
            byte[] buffer = new byte[25];
            if (mode == 0)
            {// ASCII rezim
                read(serialClient1, buffer, 1, interval);
                
                buf2 = buffer; //prepis do globalni promenne
                timer1.Start();
            }
            else { 
            //binarni rezim
                //rozhodni o dat. typu prijimanych dat
                read(serialClient1, buffer, sizeof(Int32), interval);


            }
            

        }

        private int read(SerialClient p, byte[] buf, int count, TimeSpan timeout)
        {

            int len = 0;

            DateTime start = DateTime.Now;
            
            while (len < count && DateTime.Now < start + timeout)
            {

                try
                {

                    len += p.Receive(buf, len, count - len);

                }

                catch (TimeoutException)
                {

                    //ignore this, it is broken in the system libraries.

                }

            }

            return len;

        }

        /* read ASCII !!! TO DO*/
        private int readASC(SerialClient p, byte[] buf, int count, TimeSpan timeout)
        {

            int len = 0;
            DateTime start = DateTime.Now;
            while (len < count && DateTime.Now < start + timeout )
            {

                try
                {

                    len += p.Receive(buf, len, count - len);

                }

                catch (TimeoutException)
                {

                    //ignore this, it is broken in the system libraries.

                }

            }

            return len;

        }


        private void button2_Click(object sender, EventArgs e)
        {

            if (button2.Text == "Pøipojit")
            {
                serialClient1 = new SerialClient(listBox2.SelectedItem.ToString(), Convert.ToInt32(listBox3.SelectedItem.ToString()));
                serialClient1.OnReceiving += new EventHandler<DataStreamEventArgs>(receiveHandler);
                if (serialClient1.OpenConn())
                {
                    button2.Text = "Odpojit";
                }
               // try
             ///   {
                    
                    
              //  }
                //catch (IOException ioex)
                //{
                //    MessageBox.Show("Error opening device:" + ioex.Message, "Warning");
                //}
                //catch (Exception ex)
                //{
                //    MessageBox.Show("Error initializing device:" + ex.Message, "Warning");
                //}
            }
            else {
                serialClient1.CloseConn();
                serialClient1.OnReceiving -= new EventHandler<DataStreamEventArgs>(receiveHandler);
                serialClient1.Dispose();
                button2.Text = "Pøipojit";
            }
            
           

            //hint: http://www.fryan0911.com/2009/04/c-serial-port-communication.html
        }

        private void button1_Click(object sender, EventArgs e)
        {
         //  if (serialPort1.IsOpen)
            listBox1.Items.Add(textBox1.Text);
            byte[] cmd = Encoding.ASCII.GetBytes(textBox1.Text);
            
                serialClient1.Transmit(cmd); 
                textBox1.Clear();
            
            
        }
        private void sp_DataReceived(object sender, SerialDataReceivedEventArgs e)
        { 
            listBox1.Items.Add(serialPort1.ReadLine());
            //TO DO zpracovani prijatych dat, DOPLNIT TIME-outem
            
           // hint: http://www.fryan0911.com/2009/04/c-serial-port-communication.html
            //http://programujte.com/forum/vlakno/9302-chyba-mezi-podprocesy/
        }
        private void sp_ErrorReceived(object sender, SerialErrorReceivedEventArgs e)
        {
            spErr = e.EventType;
        }

        private void button42_Click(object sender, EventArgs e)
        {
            listBox1.Items.Add(System.Text.Encoding.ASCII.GetString(buf2));
        }

        private void button29_Click(object sender, EventArgs e)
        {

        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            
            this.listBox2.Items.Add(buf2.ToString());
            timer1.Stop();
        }

        
    }

    /*after form classes*/
    public class DataStreamEventArgs : EventArgs
    {
        #region Defines
        private byte[] _bytes;
        #endregion

        #region Constructors
        public DataStreamEventArgs(byte[] bytes)
        {
            _bytes = bytes;
        }
        #endregion

        #region Properties
        public byte[] Response
        {
            get { return _bytes; }
        }
        #endregion
    }


    public class SerialClient : IDisposable
    {
        #region Defines
        private string _port;
        private int _baudRate;
        private SerialPort _serialPort;
        private Thread serThread;
        private double _PacketsRate;
        private DateTime _lastReceive;
        /*The Critical Frequency of Communication to Avoid Any Lag*/
        private const int freqCriticalLimit = 20;
        #endregion

        #region Constructors
        public SerialClient(string Port)
        {
            _port = Port;
            _baudRate = 115200;
            _lastReceive = DateTime.MinValue;

            serThread = new Thread(new ThreadStart(SerialReceiving));
            serThread.Priority = ThreadPriority.Normal;
            serThread.Name = "SerialHandle" + serThread.ManagedThreadId;
        }
        public SerialClient(string Port, int baudRate)
            : this(Port)
        {
            _baudRate = baudRate;
        }
        #endregion

        


        #region Custom Events
        public event EventHandler<DataStreamEventArgs> OnReceiving;
        #endregion

        #region Properties
        public string Port
        {
            get { return _port; }
        }
        public int BaudRate
        {
            get { return _baudRate; }
        }
        public string ConnectionString
        {
            get
            {
                return String.Format("[Serial] Port: {0} | Baudrate: {1}",
                    _serialPort.PortName, _serialPort.BaudRate.ToString());
            }
        }
        #endregion

        #region Methods
        public void setPort(string name, int baudrate)
        {
            _port = name;
            _baudRate = baudrate;
            return;
        }

        public bool IsOpen()
        {
            if (_serialPort != null ){
            return _serialPort.IsOpen;}
            else return false;
            
        }

        
        #region Port Control
        public bool OpenConn()
        {
            try
            {
                if (_serialPort == null)
                    _serialPort = new SerialPort(_port, _baudRate, Parity.None);

                if (!_serialPort.IsOpen)
                {
                    _serialPort.ReadTimeout = -1;
                    _serialPort.WriteTimeout = -1;

                    _serialPort.Open();

                    if (_serialPort.IsOpen)
                        serThread.Start(); /*Start The Communication Thread*/
                }
            }
            catch (Exception ex)
            {
                return false;
            }

            return true;
        }
        public bool OpenConn(string port, int baudRate)
        {
            _port = port;
            _baudRate = baudRate;

            return OpenConn();
        }
        public void CloseConn()
        {
            if (_serialPort != null && _serialPort.IsOpen)
            {
                serThread.Abort();

                if (serThread.ThreadState == ThreadState.Aborted)
                    _serialPort.Close();
            }
        }
        public bool ResetConn()
        {
            CloseConn();
            return OpenConn();
        }
        #endregion
        #region Transmit/Receive
        public void Transmit(byte[] packet)
        {
            _serialPort.Write(packet, 0, packet.Length);
        }
        public int Receive(byte[] bytes, int offset, int count)
        {
            int readBytes = 0;

            if (count > 0)
            {
                readBytes = _serialPort.Read(bytes, offset, count);
            }

            return readBytes;
        }
        #endregion
        #region IDisposable Methods
        public void Dispose()
        {
            CloseConn();

            if (_serialPort != null)
            {
                _serialPort.Dispose();
                _serialPort = null;
            }
        }
        #endregion
        #endregion

        #region Threading Loops
        private void SerialReceiving()
        {
            while (true)
            {
                int count = _serialPort.BytesToRead;

                /*Get Sleep Inteval*/
                TimeSpan tmpInterval = (DateTime.Now - _lastReceive);

                /*Form The Packet in The Buffer*/
                byte[] buf = new byte[count];
                int readBytes = Receive(buf, 0, count);

                if (readBytes > 0)
                {
                    OnSerialReceiving(buf);
                }

                #region Frequency Control
                _PacketsRate = ((_PacketsRate + readBytes) / 2);

                _lastReceive = DateTime.Now;

                if ((double)(readBytes + _serialPort.BytesToRead) / 2 <= _PacketsRate)
                {
                    if (tmpInterval.Milliseconds > 0)
                        Thread.Sleep(tmpInterval.Milliseconds > freqCriticalLimit ? freqCriticalLimit : tmpInterval.Milliseconds);

                    /*Testing Threading Model*/
                    // ; Diagnostics.Debug.Write(tmpInterval.Milliseconds.ToString());
                    // ; Diagnostics.Debug.Write(" - ");
                    //  ; Diagnostics.Debug.Write(readBytes.ToString());
                    //  ; Diagnostics.Debug.Write("\r\n");
                }
                #endregion
            }

        }
        #endregion

        #region Custom Events Invoke Functions
        private void OnSerialReceiving(byte[] res)
        {
            if (OnReceiving != null)
            {
                OnReceiving(this, new DataStreamEventArgs(res));
            }
        }
        #endregion
    }

}

