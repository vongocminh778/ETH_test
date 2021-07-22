using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace ReaderDemo
{
    static class Program
    {
        /// <summary>
        /// Điểm vào chính cho ứng dụng.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new MainForm());
        }
    }
}
