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
/*
 TODO - detekce pri odpojenem seriaku v za behu programu
 * po stisku ENTER v terminalu odeslat cmd
 */


namespace MCUmechanic
{

    public partial class Form1 : Form
    {
        
        
        byte[] buf2;
        static bool openPort = false;
        static bool bufsaved = false;

        static string pamet;
        SerialError spErr;
        private Thread trd;
        delegate void serialDelegate(object sglton);
        private delegate void SetTextDeleg(string data);
        SerialPort mySerialPort;
        Generator myGenerator;
        string lastIDN = "Pøipojte kit s MCU";   

        
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
            SettingGenerator();
        }
        public void SettingGenerator() {
            myGenerator = new Generator();
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
                mySerialPort.ReadTimeout = 500;
                mySerialPort.WriteTimeout = 200;

                mySerialPort.DtrEnable = true;
                mySerialPort.RtsEnable = true;

                
                //mySerialPort.DataReceived += new SerialDataReceivedEventHandler(DataReceivedHandler);
                

                //listBox1.Items.Add("Serial Port pøipraven.");

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
            System.Threading.Thread.Sleep(200);
            string indata = sp.ReadExisting();
            this.BeginInvoke(new SetTextDeleg(DisplayToUI), new object[] { indata });
            //textBox1.Text += indata;
        }
        public static byte[] StrToByteArray(string str)
        {
            System.Text.UTF8Encoding encoding = new System.Text.UTF8Encoding();
            return encoding.GetBytes(str);
        }
        private void DisplayToUI(object displayData)
        {
            switch (tabControl1.SelectedIndex) {
                // uvodni panel
                case 0: object pokus1 = displayData;
                    protocolParser(StrToByteArray(pokus1.ToString())); 
                    label6.Text = lastIDN; return;
                //panel Scope
                case 1:
                    protocolParser(StrToByteArray(displayData.ToString())); return;
                // panel Log
                case 2: return;
                // panel terminal
                case 3: listBox1.Items.Add((displayData.ToString()).Trim()); return; 
                
                default: return;
            }
               
            //textBox1.Text += displayData.Trim();
            // textBox1.Text += displayData;

        }

        public byte[] ObjectToByteArray(object _Object)
        {
            try
            {

                System.IO.MemoryStream _MemoryStream = new System.IO.MemoryStream();
                System.Runtime.Serialization.Formatters.Binary.BinaryFormatter _BinaryFormatter = new System.Runtime.Serialization.Formatters.Binary.BinaryFormatter();
                _BinaryFormatter.Serialize(_MemoryStream, _Object);
                return _MemoryStream.ToArray();

            }
            catch (Exception _Exception)
            {
                warnmessage();
            }
            return null;
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
            
            

        }

       

       
        public string byteArrToStr(byte[] arr) {
            string outstr = "";
            for (int i = 0; i < arr.Length; i++) {
                outstr += (char)arr[i];
            }
            return outstr;


        }

        public byte[] remCmd(byte[] pole) {
            byte[] outArr = new byte[60];
            for (int i = 0; i < pole.Length-4; i++) {
                outArr[i] = pole[i+3];
            }
            return outArr;
        }

        private string protocolParser(byte[] inputBuffer)
        {

            string lastcmd, buff ="", retbuff; //lastcmd pozdeji bude static
            
            for (int i = 0; i < 4; i++) {
                buff += (char)inputBuffer[i];
            }

            switch(buff){
                case "IDN_":
                    string str = byteArrToStr(inputBuffer);
                    return lastIDN = str.Substring(str.IndexOf(" "));


                case "GPIO": return "neimplemenotvano";
                case "OSC8": separeVaue(remCmd(inputBuffer),2);
                    return "neimplemenotvano";  
                case "OSCF": return "neimplemenotvano";
                case "LOG_": return "neimplemenotvano";
                case "CNTF": return "neimplemenotvano";
                case "CNTP": return "neimplemenotvano";
                case "MEAS": return "neimplemenotvano";
                case "VOLT": return "neimplemenotvano";


                default: retbuff = "neznámý vstup"; break;
            }
            return "neznámý vstup";
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
                    } return "Neplatný formát dat";
                /*uint8bininput to intArray16*/
                case 2:
                    dataLen = 0;
                    cntCData = Convert.ToInt16(inputbuffer[0].ToString());
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
                    } return "Neplatný formát dat";
                /*uint^16*/

                default: return "neznámá hodnota"; break;
            }
        
        
        }

        //Thread readThread = new Thread(readRS232);
        List<byte> bBuffer = new List<byte>();

        private bool sendCmd(string cmd) {
            string sBuffer = "";
            if (mySerialPort.IsOpen) {
                mySerialPort.WriteLine(cmd);
                try
                {
                    while (mySerialPort.BytesToRead > 0) bBuffer.Add((byte)mySerialPort.ReadByte());
                    sBuffer += mySerialPort.ReadExisting();
                    
                    if (sBuffer.Contains("OK"))
                    {
                        return true;
                    }
                }
                catch (TimeoutException) { 
                
                }

            }
            return false;
        }

        




        private void button2_Click(object sender, EventArgs e)
        {

            if (button2.Text == "Pøipojit")
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
                    listBox1.Items.Add("Port otevøen");
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
                button2.Text = "Pøipojit";
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
                if (!(sendCmd("OSCP:STRT"))) warnmessage();
                else
                {
                    scopeRun = true;
                    scopeTimer.Start();
                    button16.Text = "STOP";
                }

            }
            else
            {
                if (!(sendCmd("OSCP:STOP"))) warnmessage();
                else
                {
                    scopeRun = false; button16.Text = "RUN";
                }
            }
        }


        private string getVolt() { 
        // posli VOLT? vrat hodnotu
            return "neimplementováno";
        }

        private string getIDN()
        {
            // posli VOLT? vrat hodnotu
            return "neimplementováno";
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
            
            
            if (scopeRun == false) { scopeTimer.Stop(); }

        }

        private void label15_Click(object sender, EventArgs e)
        {

        }

        private void label17_Click(object sender, EventArgs e)
        {
            Generator myGener = new Generator();
            myGener.isEnabled = true;

        }

        private void trackBar7_Scroll(object sender, EventArgs e)
        {
            textBox3.Text = Convert.ToString(trackBar7.Value*2000);
        }

        private void trackBar8_Scroll(object sender, EventArgs e)
        {
            textBox7.Text = Convert.ToString(trackBar8.Value * 2000);
        }

        private void trackBar5_Scroll(object sender, EventArgs e)
        {
            textBox4.Text = Convert.ToString(trackBar5.Value * 2000);
        }

        private void trackBar6_Scroll(object sender, EventArgs e)
        {
            textBox2.Text = Convert.ToString(Convert.ToDouble(trackBar6.Value) /5)+":1";
        }

        private void button4_Click(object sender, EventArgs e)
        {
            if (!(sendCmd("OSCP:FREQ 1K__"))) warnmessage(); 
        }

        private void warnmessage() { }

        private void button48_Click(object sender, EventArgs e)
        {
            if (!(sendCmd("OSCP:FREQ 1M__"))) warnmessage();
        }

        private void button49_Click(object sender, EventArgs e)
        {
            if (!(sendCmd("OSCP:FREQ 1M__"))) warnmessage();
        }

        private void button50_Click(object sender, EventArgs e)
        {
            if (!(sendCmd("OSCP:FREQ 1M__"))) warnmessage();
        }

        private void button15_Click(object sender, EventArgs e)
        {
            if (!(sendCmd("OSCP:FREQ 1M__"))) warnmessage();
        }

        private void button14_Click(object sender, EventArgs e)
        {
            if (!(sendCmd("OSCP:FREQ 1M__"))) warnmessage();
        }

        private void button13_Click(object sender, EventArgs e)
        {
            if (!(sendCmd("OSCP:FREQ 1M__"))) warnmessage();
        }

        private void button12_Click(object sender, EventArgs e)
        {
            if (!(sendCmd("OSCP:FREQ 100K"))) warnmessage();
        }

        private void button11_Click(object sender, EventArgs e)
        {
            if (!(sendCmd("OSCP:FREQ 100K"))) warnmessage();
        }

        private void button10_Click(object sender, EventArgs e)
        {
            if (!(sendCmd("OSCP:FREQ 100K"))) warnmessage();
        }

        private void button9_Click(object sender, EventArgs e)
        {
            if (!(sendCmd("OSCP:FREQ 10K_"))) warnmessage();
        }

        private void button8_Click(object sender, EventArgs e)
        {
            if (!(sendCmd("OSCP:FREQ 10K_"))) warnmessage();
        }

        private void button7_Click(object sender, EventArgs e)
        {
            if (!(sendCmd("OSCP:FREQ 10K_"))) warnmessage();
        }

        private void button6_Click(object sender, EventArgs e)
        {
            if (!(sendCmd("OSCP:FREQ 10K_"))) warnmessage();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            if (!(sendCmd("OSCP:FREQ 1K__"))) warnmessage();
        }

        private void tabPage3_Click(object sender, EventArgs e)
        {
            if (!(sendCmd("IDN?"))) warnmessage();
            else { 
            
            
            }
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


}

