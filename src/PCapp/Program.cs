using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace MCUmechanic
{
    static class Program
    {
     /*global variables*/
        static bool openPort = false;
        
        
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}