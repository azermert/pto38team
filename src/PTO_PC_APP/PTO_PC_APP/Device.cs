using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Ports;
using System.Xml;
using System.Windows.Forms;
using System.Threading;
using System.Diagnostics;
using System.IO;

namespace PTO_PC_APP
{
    class Device
    {
        //public enum processor_type { STM32F051,STM32F100,STM32F303,STM32F407,ADuC843}
        public struct config {
          public string procType;
          public string procCore;

          public string comm;
          public string connection;
          public string clock;
  
          public int scopeMaxf;
          public int scopeDept;
          public int scopeBuffLenght;
          public string scopePin;
          public int ScopeVref_mv;
        }

        config procConf;
        private string name;
        private string version;
        private SerialPort port;
        private string portName="";
        private int Baudrate=0;
        private bool opened=false;
        private string msg;
        XmlDocument xmlDoc = new XmlDocument();

        private bool newScopeData=false;
        private byte[] scopeBuffer;

        int count = 0;
        Stopwatch st;

        private StreamWriter logWriter;

        public Device(string portn, string name, string processor, string version, int speed)
        {

            this.portName = portn;
            this.Baudrate = speed;
            this.name = name;

            this.port = new SerialPort();
            this.port.PortName = portName;
            this.port.ReadBufferSize = 1024*1024;
            this.port.BaudRate = Baudrate;
            
            procConf = new config();
            switch (processor) { 
                case Defines.STM32F051:
                    procConf.procType = Defines.STM32F051;
                    break;
                case Defines.STM32F100:
                    procConf.procType = Defines.STM32F100;
                    break;
                case Defines.STM32F303:
                    procConf.procType = Defines.STM32F303;
                    break;
                case Defines.STM32F407:
                    procConf.procType = Defines.STM32F407;
                    break;
                case Defines.ADuC843:
                    procConf.procType = Defines.ADuC843;
                    break;
                default:
                    break;
            }
            tic();

            this.version = version;
            this.port.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort_DataReceived);
            this.port.ErrorReceived += new System.IO.Ports.SerialErrorReceivedEventHandler(this.serialPort_ErrorReceived);

        }

        

        public void open_port() {
            logWriter = File.AppendText("logfile.txt");
            Log("PORT otevřen: "+this.portName+"  Baudrate:"+this.Baudrate+"  Zařízení:"+this.get_processor());
            try {
                parse_Xml();
                msg = "";
                port.Open();
                send_IDN();

            }catch(Exception ex){
                MessageBox.Show("Fatal error during connecting to device \r\n"+ex, "Fatal error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            
        }

        public void close_port() {
            Log("PORT zavřen: " + this.portName);
            logWriter.Close();
            port.Close();
            port.Dispose();
            this.opened = false;
        }

        public bool is_connected() {
            return this.opened;
        }

        public void send(string s) {
            try
            {
                port.Write(s);

                if (s.Length>5 && s[5] != 'S' && s[6] != 'R') {
                    logUart(s);
                }
                Console.WriteLine(s);
            }
            catch (Exception ex) {
                Log("Data se nepodařilo odeslat:\r\n"+ex);
                Console.WriteLine(ex);
            }
        }

        public void send_short(int l)
        {
            byte[] bt = BitConverter.GetBytes(l);
            byte[] se=new byte[4];
            se[0]=0;
            se[1]=0;
            se[2]=bt[0];
            se[3]=bt[1];
            port.Write(se, 0, 4);
            Console.WriteLine(l.ToString());
        }

        public string get_processor() {
            return this.procConf.procType;
        }

        public string get_version() {
            return this.version;
        }

        public string get_port() {
            return this.port.PortName;
        }

        public void send_IDN() {
            port.Write(Defines.IDNRequest);
        }

        public config get_configuration() {
            return this.procConf;
        }

        public bool is_new_scope_data() {
            return this.newScopeData;
        }

        public byte[] get_scope_data() {
            this.newScopeData = false;
            return this.scopeBuffer;
        }

        private void parse_Xml() {

            XmlTextReader reader = new XmlTextReader(Defines.confPath + procConf.procType + ".xml");
            //procConf = new config();
            string param_name="";

            while (reader.Read())
            {
                switch (reader.NodeType)
                {
                    case XmlNodeType.Element: // Tento uzel je prvek.
                        param_name=reader.Name;
                        break;
                    case XmlNodeType.Text:
                        switch (param_name) { 
                            case "sc_maxf":
                                procConf.scopeMaxf = int.Parse(reader.Value);
                                break;
                            case "sc_depth":
                                procConf.scopeDept = int.Parse(reader.Value);
                                break;
                            case "sc_v_ref":
                                procConf.ScopeVref_mv = int.Parse(reader.Value);
                                break;
                            case "sc_buffsize":
                                procConf.scopeBuffLenght = int.Parse(reader.Value);
                                break;
                            case "sc_pin":
                                procConf.scopePin = reader.Value;
                                break;
                            case "processor":
                                procConf.procType = reader.Value;
                                break;
                            case "core":
                                procConf.procCore = reader.Value;
                                break;
                            case "comm":
                                procConf.comm = reader.Value;
                                break;
                            case "clock":
                                procConf.clock = reader.Value;
                                break;
                            case "connect":
                                procConf.connection = reader.Value;
                                break;
                            default:
                                break;
                        }                       
                    break;
                }
            }
            this.scopeBuffer = new byte[procConf.scopeBuffLenght];
        }


        private void serialPort_DataReceived(object sender, SerialDataReceivedEventArgs e){
            char input;
         //  tic();
            while (port.IsOpen && port.BytesToRead > 0)
            {
                while (port.BytesToRead > 0)
                {
                    input = (char)port.ReadChar();
                    msg += input;
                    if (msg.Length == 4)
                    {
                    //    Console.WriteLine(msg);
                        break;
                    }
                //    Thread.Yield();
                }
             //   toc("cmd read");
                if (msg.Length > 4)
                {
                    msg = msg.Substring(0, 4);
                }

                if (msg.Length == 4)
                {
                    switch (msg)
                    {
                        case Defines.IDN:
                            opened = true;
                            break;
                        case Defines.SCOPE_8BIT:
                            input = (char)(port.ReadChar()-48);
                            string leng="";
                            count++;
                       //    toc("new OSCP "+count+" - ");

                            for(;input>0;input--){
                                leng+=""+(port.ReadChar() - 48);
                            }
                            Console.WriteLine("new OSCP "+count+" - "+leng);
                            int delka = int.Parse(leng);
                            

                            int wd = 0;
                            while (port.IsOpen && port.BytesToRead < delka) {
                                //Thread.Yield();
                                wd++;
                               //toc("sleep");
                                if (wd > 500000) {
                                    break;
                                }
                            }
                            if (!port.IsOpen) {
                                break;
                            }
                          //  toc("data ready");
                            port.Read(scopeBuffer, 0, delka);
                           // Array.Copy(actualScopeData, test, delka);
                          // toc("New data read");
                            newScopeData = true;
                        
                            break;
                        case Defines.ERROR:
                            break;
                        case Defines.ACKNOWLEDGE:
                            break;

                        default:
                            break;

                    }
                }



                if (msg.Length > 3)
                {
                    msg = msg.Substring(1, 3);
                }

                Thread.Yield();
            }
       //     toc("COmm pause");
        }

        private void serialPort_ErrorReceived(object sender, System.IO.Ports.SerialErrorReceivedEventArgs e)
        {
            Log("Chyba na komunikaci seriové linky:\r\n" + e);
            Console.WriteLine(e);
            //MessageBox.Show("Fatal error at serial comunication\r\n Didn't you unplag the device?\r\n\r\n Please restart the aplication!!!\r\n\r\n " + e, "Fatal error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            int i = 0;
        }



        private void tic()
        {
            st = new Stopwatch();
            st.Start();
        }

        private void toc(string s)
        {
            st.Stop();
            Console.WriteLine(s + " " + st.ElapsedMilliseconds.ToString() + "ms");
            st.Start();
        }


        public void Log(string logMessage)
        {
            logWriter.Write("\r\nLog Entry : ");
            logWriter.WriteLine("{0} {1}", DateTime.Now.ToLongTimeString(),
            DateTime.Now.ToLongDateString());
            logWriter.WriteLine("  :{0}", logMessage);
            logWriter.WriteLine("-------------------------------");
        }

        public void LogEnd() {
            logWriter.WriteLine("Konec logu");
            logWriter.WriteLine("-------------------------------");
            logWriter.WriteLine("\r\n\r\n\r\n");
        }

        public void logUart(string s){
            logWriter.Write("UART send (" + DateTime.Now.Minute + ":" + DateTime.Now.Second + "." + DateTime.Now.Millisecond + "): " +s+"\r\n");
       
        }

        public void logText(string s) {
            logWriter.Write(s);
        }



    }
}
