using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace PTO_PC_APP
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Form f = new Form1();
            Application.Run(f);

        }
    }
}
