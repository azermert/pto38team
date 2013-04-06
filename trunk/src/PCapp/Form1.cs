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



namespace MCUmechanic
{

    public partial class Form1 : Form
    {

        static bool openPort = false;
        SerialError spErr;
        public Form1()
        {
            InitializeComponent();
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

        private void button2_Click(object sender, EventArgs e)
        {
            if (openPort == false)
            {
                try
                {
                    serialPort1.PortName = listBox2.SelectedValue.ToString();
                    serialPort1.BaudRate = Convert.ToInt32(listBox3.SelectedValue);
                    serialPort1.Open();
                    button2.Text = "Odpojit";
                }
                catch (IOException ioex)
                {
                    MessageBox.Show("Error opening device:" + ioex.Message, "Warning");
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Error initializing device:" + ex.Message, "Warning");
                }
                finally
                {
                    //Bind the events on the following event handler
                    serialPort1.DataReceived += new SerialDataReceivedEventHandler(sp_DataReceived);
                    serialPort1.ErrorReceived += new SerialErrorReceivedEventHandler(sp_ErrorReceived);
                }

            }
            else
            {
                if (serialPort1.IsOpen) serialPort1.Close();
                button2.Text = "Pripojit";
            }

            //hint: http://www.fryan0911.com/2009/04/c-serial-port-communication.html
        }

        private void button1_Click(object sender, EventArgs e)
        {
           if (serialPort1.IsOpen)
            {
                serialPort1.WriteLine(textBox1.Text);
                listBox1.Items.Add(textBox1.Text);
                textBox1.Clear();
            }
        }
        private void sp_DataReceived(object sender, SerialDataReceivedEventArgs e)
        { 
            listBox1.Items.Add(serialPort1.ReadLine());
            //TO DO zpracovani prijatych dat, DOPLNIT TIME-outem
            
           // hint: http://www.fryan0911.com/2009/04/c-serial-port-communication.html
        }
        private void sp_ErrorReceived(object sender, SerialErrorReceivedEventArgs e)
        {
            spErr = e.EventType;
        }
    }
}

