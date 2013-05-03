using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Ports;
using System.Threading;
using PTO_PC_APP;


namespace PTO_PC_APP
{
    


    class Serial_comm
    {
        //promenne pro pripojeni a spravu devices
        private bool find_request = false;
        private bool connected = false;
        private bool connectingInProgress = false;
        public int progress = 0;
       
        private int error = 0;
        
        private int numberOfPorts = 0;
        private List<Device> devices = new List<Device>();
        private Device connectedDevice;
        private bool newDevices = false;
        private bool run = true;



        public void comm_run() {
            while (run) {
                Thread.Sleep(10);

                if (find_request) {
                    find_request = false;
                    find_devices();
                }

                if (connectedDevice != null)
                {
                    connected = connectedDevice.is_connected();
                }          
            }
        }

        public void comm_stop()
        {
            this.run = false;
            if (this.is_connected()) {
                disconnect_device();
            }
        }


        public void connect_device(string port) {

            foreach (Device d in devices)
            {
                if (port.Equals(d.get_port())) {
                    this.connectedDevice=d;
                    d.open_port();
                    break;
                }
            }
            
        }

        public void disconnect_device()
        {
            this.connectedDevice.close_port();
            this.connectedDevice = null;

            connected = false;
        }


        public string device_string() {
            string result;
            if (connectedDevice != null)
            {
                result = connectedDevice.get_processor() + " " + connectedDevice.get_version() + " at " + connectedDevice.get_port();
            }
            else {
                result = "error";
            }
            return result;
        }

        public string device_port() {
            if (connectedDevice != null)
            {
                return connectedDevice.get_port();
            }
            else {
                return "error";
            }
        }


        // nalezne vsechna pripojena zarizeni a da je do listu
        public void find_devices()
        {
            devices.Clear();
            
            numberOfPorts = 0;

            if (!connected)
            {   
                SerialPort serialPort = new SerialPort();
                serialPort.ReadBufferSize = 1048576;
                serialPort.BaudRate = 115200;
                this.connectingInProgress = true;
                this.error = 0;

                foreach (string s in SerialPort.GetPortNames())
                {
                    numberOfPorts++;
                }

                int counter = 0;
                foreach (string s in SerialPort.GetPortNames())
                {
                    counter++;
                    progress = (counter * 100) / numberOfPorts;
                    try
                    {
                        Thread.Yield();
                        serialPort.PortName = s;
                        
                        serialPort.Open();
                        
                        serialPort.Write(Defines.IDNRequest);
                        Thread.Sleep(250);

                        char[] msg = new char[1024];
                        int toRead = serialPort.BytesToRead;

                        serialPort.Read(msg, 0, toRead);
                        string msgInput = "";
                        string procesor = "";
                        string deviceName = "";
                        string version = "";
                        
                        int i=0;

                        if (toRead > 4) { 
                            for (i = 0; i < 4; i++)
                            {
                                msgInput = msgInput + msg[i];
                            }
                        }

                        
                        Thread.Yield();
                        string msgCompare = Defines.IDN;
                        if (msgInput.Contains(msgCompare))
                        {
                            if (toRead > 10)
                            {
                                while (msg[i] != ' ')
                                {
                                    procesor = procesor + msg[i];
                                    i++;
                                }
                                i++;
                                while (msg[i] != 'V')
                                {
                                    deviceName = deviceName + msg[i];
                                    i++;
                                }

                                for (int j = 0; j < 4; j++)
                                {
                                    version = version + msg[i];
                                    i++;

                                }
                            }
                            serialPort.Close();
                            Device tmp = new Device(serialPort, deviceName, procesor, version);
                            devices.Add(tmp);
                            
                        }
                        else
                        {
                            serialPort.Close();
                        }

                    }
                    catch (Exception ex)
                    {
                        if (serialPort.IsOpen) {
                            serialPort.Close();
                        }
                        Console.WriteLine(ex);
                    }

                }
                newDevices = true;
                this.connectingInProgress = false;
            }
        }

        //request na nalezeni zarizeni
        public void find_dev_req() {
            this.find_request = true;
        }

        public bool is_connecting_in_progress() {
            return this.connectingInProgress;
        }

        public bool is_connected() {
            return this.connected;
        }

        public int get_connecting_progress() {
            return this.progress;
        }

        public int get_error() {
            return this.error;
        }

        public int get_num_of_devices() {
            return this.devices.Count;
        }

        public bool is_new_devices() {
            return this.newDevices;
        }

        public Device.config get_dev_configuration() {
            return connectedDevice.get_configuration();
        }

        public string[] get_dev_names() {
            string[] result = new string[devices.Count()];
            newDevices = false;
            int i=0;
            foreach (Device d in devices) { 
                result[i]=d.get_port()+": "+ d.get_processor() + " - " + d.get_version();
                i++;
            }
            return result;
        }

        /*Metody obsluhujici oscilop*/
        public void set_scope_sampling_freq(string smp) { 
            connectedDevice.send(Defines.SCOPE+":"+Defines.SCOPE_FREQUENCY+" "+smp);
        }

        public void set_scope_start()
        {
            connectedDevice.send(Defines.SCOPE + ":" + Defines.START);
        }

        public void set_scope_stop()
        {
            connectedDevice.send(Defines.SCOPE + ":" + Defines.STOP);
        }

        public void set_scope_trigger_edge(string edge)
        {
            connectedDevice.send(Defines.SCOPE + ":" + Defines.SCOPE_EDGE + " " + edge);
        }

        public void set_scope_trigger_type(string trig)
        {
            connectedDevice.send(Defines.SCOPE + ":" + Defines.SCOPE_TRIG + " " + trig);
        }
        public void set_scope_trigger_level(int levl)
        {
            connectedDevice.send(Defines.SCOPE + ":" + Defines.SCOPE_TRIG_LEVEL + " ");
            connectedDevice.send_short(levl);
        }
        public void set_scope_pretrig(int pre) {
            
            connectedDevice.send(Defines.SCOPE + ":" + Defines.SCOPE_PRETRIGGER + " ");
            connectedDevice.send_short(pre);
        }

        public bool is_new_scope_data() {
            return connectedDevice.is_new_scope_data();
        }

        public byte[] get_scope_data() {
            return connectedDevice.get_scope_data();
        }



    }
}
