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
        static bool bufsaved = false;

        static string pamet;
        SerialError spErr;
        private Thread trd;
        delegate void serialDelegate(object sglton);
        private delegate void SetTextDeleg(string data);
        SerialPort mySerialPort;
        
        
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


            /*konfigurace serial portu*/
            SettingRS232();
        }
        public void SettingRS232()
        {
            try
            {
                mySerialPort = new SerialPort("COM11");

                mySerialPort.BaudRate = 115200;
                mySerialPort.Parity = Parity.None;
                mySerialPort.StopBits = StopBits.One;
                mySerialPort.DataBits = 8;
                mySerialPort.Handshake = Handshake.None;
                mySerialPort.ReadTimeout = 8000;
                mySerialPort.WriteTimeout = 500;

                mySerialPort.DtrEnable = true;
                mySerialPort.RtsEnable = true;

                
                //mySerialPort.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);
                

                //listBox1.Items.Add("Serial Port p�ipraven.");

            }
            catch (Exception ex)
            {
                listBox1.Items.Add(ex.Message);
            }

        }
        // obsluha serioveho portu
        public void DataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            SerialPort sp = (SerialPort)sender;
            System.Threading.Thread.Sleep(5000);
            string indata = sp.ReadExisting();
            this.BeginInvoke(new SetTextDeleg(DisplayToUI), new object[] { indata });
            //textBox1.Text += indata;
        }
        
        private void DisplayToUI(string displayData)
        {
             listBox1.Items.Add(displayData.Trim());
            //textBox1.Text += displayData.Trim();
            // textBox1.Text += displayData;

        }



        static object zamek = new object();
        static string buffer;
        static void updateListBox(string buffer) { 
            lock(zamek) {
                // textBox1.Text = buffer;
                    
            }
        
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
            byte[] buffer = new byte[5];
            if (mode == 0)
            {// ASCII rezim
                read(serialClient1, buffer, 1, interval);
                
                buf2 = buffer; //prepis do globalni promenne
                
               
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

        
        
        private string protocolParser(byte[] inputBuffer) {

            string lastcmd, buff ="", retbuff; //lastcmd pozdeji bude static
            
            for (int i = 0; i < 4; i++) {
                buff += inputBuffer[i];
            }

            switch(buff){
                case "IDN": return "neimplemenotvano";
                case "GPIO": return "neimplemenotvano";
                case "OSC8": return "neimplemenotvano";
                case "OSCF": return "neimplemenotvano";
                case "LOG_": return "neimplemenotvano";
                case "CNTF": return "neimplemenotvano";
                case "CNTP": return "neimplemenotvano";
                case "MEAS": return "neimplemenotvano";
                case "VOLT": return "neimplemenotvano";


                default: retbuff = "nezn�m� vstup"; break;
            }
            return "nezn�m� vstup";
        }

        private string separeVaue(byte[] inputbuffer, uint type){ //!!! predelat na object
            
            string retbuff = "";
            string dataLenbuff = "";

            switch (type) {
                /*char[] to string*/
                case 0: for (int i = 0; i < int.MaxValue; i++) { 
                            if (inputbuffer[i] == 13 || inputbuffer[i] == 10 ){return retbuff;}
                            retbuff = retbuff+inputbuffer[i];
                        }   return "prilis dlouhy retezec";
                /*uint8 to string*/
                case 1:
                    int dataLen = 0;
                    int cntCData = Convert.ToInt16((char)inputbuffer[0]);
                    if (cntCData > 0){
                        
                        for (int i = 0; i < cntCData; i++ )
                        {
                            dataLenbuff = dataLenbuff+inputbuffer[1 + i];
                        }
                        dataLen = Convert.ToInt32(dataLenbuff);

                        for (int i=0; i< int.MaxValue; i++){
                            if (i == dataLen) {return retbuff;}
                            retbuff = retbuff+Convert.ToString(Convert.ToUInt16(inputbuffer[cntCData + 1 + i]));
                        }
                    } return "Neplatn� form�t dat";
                /*uint8bininput to intArray16*/
                case 2:
                    
                    dataLen = 0;
                    cntCData = Convert.ToInt16((char)(inputbuffer[0]));
                    if (cntCData > 0)
                    {
                        
                        for (int i = 0; i < cntCData; i++)
                        {
                            dataLenbuff = dataLenbuff+inputbuffer[1 + i];
                        }
                        dataLen = Convert.ToInt32(dataLenbuff);

                        for (int i = 0; i < int.MaxValue; i++)
                        {
                            if (i == dataLen || inputbuffer[i] == 13 || inputbuffer[i] == 10) { return retbuff; }
                            retbuff = retbuff + (Convert.ToUInt16(inputbuffer[cntCData + 1 + i])); // vraci pole uint16
                        }
                    } return "Neplatn� form�t dat";
                /*uint^16*/

                default: return "nezn�m� hodnota"; break;
            }
        
        
        }



        private void button2_Click(object sender, EventArgs e)
        {

            if (button2.Text == "P�ipojit")
            {
                try
                {
                    mySerialPort.PortName = listBox2.SelectedItem.ToString();
                    mySerialPort.BaudRate = Convert.ToInt32(listBox3.SelectedItem.ToString());
                    mySerialPort.Open();
                 }
                catch (IOException ioex)
                {
                  listBox1.Items.Add(ioex.Message);
                }
                
                if (mySerialPort.IsOpen)
                {
                    mySerialPort.DataReceived += DataReceivedHandler;
                    listBox1.Items.Add("Port otev�en");
                    button2.Text = "Odpojit";
                }
                
                //catch (Exception ex)
                //{
                //    MessageBox.Show("Error initializing device:" + ex.Message, "Warning");
                //}
            }
            else {
                mySerialPort.Close();
                mySerialPort.Dispose();
                button2.Text = "P�ipojit";
            }
            
           

            //hint: http://www.fryan0911.com/2009/04/c-serial-port-communication.html
        }

        private void button1_Click(object sender, EventArgs e)
        {
         //  if (serialPort1.IsOpen)
            string scmd = textBox1.Text.ToString().ToUpper();
            //byte[] cmd = Encoding.ASCII.GetBytes(scmd);
            try
            {
                mySerialPort.WriteLine(scmd);
            }
            catch (IOException ioex)
                {
                  listBox1.Items.Add(ioex.Message);
                }
            listBox1.Items.Add(scmd);
            textBox1.Clear();
            
            
        }
        private void sp_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            updateListBox(serialPort1.ReadLine());
            //listBox1.Items.Add(serialPort1.ReadLine());
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

            timer1.Start();
            //listBox1.Items.Add(System.Text.Encoding.ASCII.GetString(buf2));
        }

        private void button29_Click(object sender, EventArgs e)
        {

        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            lock (zamek)
            {
                listBox1.Items.Add((buf2.ToString()));
                timer1.Stop();
            }
        }

        


        /* Vse prislusici k osciloskopu */
        bool scopeRun = false;
        char[] scopeBuffer;
        int maxscopeBuffer = Int32.MaxValue;
        static int cntAddSamples = 0; // !!! static sdilene s rs232 vlaknem
        object scopeBufferLock;
        object cntSamplesLock;
        int lastPosinBuf = 0;


        private void button16_Click(object sender, EventArgs e)
        {
            if (button16.Text == "RUN")
            {
                scopeRun = true;
                button16.Text = "STOP";
            }
            else { scopeRun = false; button16.Text = "RUN"; }
        }


        private string getVolt() { 
        // posli VOLT? vrat hodnotu
            return "neimplementov�no";
        }

        private string getIDN()
        {
            // posli VOLT? vrat hodnotu
            return "neimplementov�no";
        }

        private void refreshPrehled() {
            
            scopeRun = false;
            //nastav mode panel = prehled
            getIDN();
            getVolt();
            
        
        }

        private void scopeTimer_Tick(object sender, EventArgs e)
        {
            int readToBuf = 0;
            // pohyb v bufferu
            lock (cntSamplesLock)
            {
            readToBuf = lastPosinBuf+cntAddSamples;
            
            }

            lock (cntSamplesLock)
            {
                for (int i = lastPosinBuf; i < readToBuf; i++) { 
                
                }

            }

        }

        private void label15_Click(object sender, EventArgs e)
        {

        }

        private void label17_Click(object sender, EventArgs e)
        {
            Generator myGener = new Generator();
            myGener.isEnabled = true;

        }

        
    }

    public class Generator
    {

        static bool _isEnabled;
        int _type ;
         uint _amplituda ;
         uint _offset ;
        uint _freq;

        public Generator()
        {
            _type = 0;
            _amplituda= 0;
            _isEnabled = false;
            _offset= 0;
            _freq = 1000;
         }

        public bool isEnabled {
            get { return _isEnabled; }
            set { _isEnabled = value; }
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

