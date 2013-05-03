using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Ports;
using System.Xml;
using System.Windows.Forms;
using System.Threading;

namespace PTO_PC_APP
{
    class Device
    {
        public enum processor_type { STM32F051,STM32F100,STM32F303,STM32F407,ADuC843}
        public struct config {
          public  int scopeMaxf;
          public  int scopeDept;
          public  int scopeBuffLenght;
          public  int vref_mv;
        }

        config procConf;
        private string name;
        private processor_type processor;
        private string version;
        private SerialPort port;
        private bool opened=false;
        private string msg;
        XmlDocument xmlDoc = new XmlDocument();

        private bool newScopeData=false;
        private byte[] scopeBuffer;


        public Device(SerialPort port, string name, string processor, string version)
        {
            this.port = port;
            this.name = name;
            switch (processor) { 
                case Defines.STM32F051:
                    this.processor = processor_type.STM32F051;
                    break;
                case Defines.STM32F100:
                    this.processor = processor_type.STM32F100;
                    break;
                case Defines.STM32F303:
                    this.processor = processor_type.STM32F303;
                    break;
                case Defines.STM32F407:
                    this.processor = processor_type.STM32F407;
                    break;
                case Defines.ADuC843:
                    this.processor = processor_type.ADuC843;
                    break;
                default:
                    break;
            }

            this.version = version;
            this.port.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort_DataReceived);
            this.port.ErrorReceived += new System.IO.Ports.SerialErrorReceivedEventHandler(this.serialPort_ErrorReceived);

        }

        public void open_port() {
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
            port.Close();
            port.Dispose();
            this.opened = false;
        }

        public bool is_connected() {
            return this.opened;
        }

        public void send(string s) {
            port.Write(s);
            Console.WriteLine(s);
        
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

        public processor_type get_processor() {
            return this.processor;
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

            XmlTextReader reader = new XmlTextReader(Defines.confPath + Defines.STM32F100 + ".xml");
            procConf = new config();
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
                                procConf.vref_mv = int.Parse(reader.Value);
                                break;
                            case "sc_buffsize":
                                procConf.scopeBuffLenght = int.Parse(reader.Value);
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
            while (port.IsOpen && port.BytesToRead > 0)
            {
                while (port.BytesToRead > 0)
                {
                    input = (char)port.ReadChar();
                    msg += input;
                    if (msg.Length == 4)
                    {
                        break;
                    }
                }
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
                           // Console.WriteLine("new");

                            for(;input>0;input--){
                                leng+=""+(port.ReadChar() - 48);
                            }
                           // Console.WriteLine(leng);
                            int delka = int.Parse(leng);


                            int wd = 0;
                            while (port.IsOpen && port.BytesToRead < delka) {
                                Thread.Sleep(1);
                                wd++;
                                if (wd > 1000) {
                                    break;
                                }
                            }
                            if (!port.IsOpen) {
                                break;
                            }
                            port.Read(scopeBuffer, 0, delka);
                            newScopeData = true;
                        
                            break;
                        case Defines.ERROR:
                            break;
                        case Defines.ACKNOWLEDGE:
                            int k = 0;
                            break;

                        default:
                            break;

                    }
                }



                if (msg.Length > 3)
                {
                    msg = msg.Substring(1, 3);
                }
            }
        }

        private void serialPort_ErrorReceived(object sender, System.IO.Ports.SerialErrorReceivedEventArgs e)
        {
            int i = 0;
        }



        





    }
}
