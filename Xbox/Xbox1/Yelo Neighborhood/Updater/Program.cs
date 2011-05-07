using System;
using System.Windows.Forms;
using System.Net;
using System.IO;
using System.Diagnostics;

namespace Updater
{
    public static class Jobs
    {
        public static Uri DownloadDirectory { get; set; }
        public static string ProgramName { get; set; }
        public static string UserName { get; set; }
        public static string Password { get; set; }

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            if (args.Length == 0) return;
            else
            {
                DownloadDirectory = new Uri(args[0]);
                UserName = args[1];
                Password = args[2];
                ProgramName = args[3];
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                Application.Run(new Downloader(args[4]));
            }
        }

        public static void CheckForUpdates(int versionNumber)
        { 
            WebClient wc = new WebClient();
            wc.Credentials = new NetworkCredential(Jobs.UserName, Jobs.Password);
            wc.OpenReadCompleted += new OpenReadCompletedEventHandler(wc_OpenReadCompleted);
            wc.OpenReadAsync(new Uri(DownloadDirectory, "Version.txt"), versionNumber);
        }

        static void wc_OpenReadCompleted(object sender, OpenReadCompletedEventArgs e)
        {
			int latest;
			using (var sr = new StreamReader(e.Result))
			{
				latest = Convert.ToInt32(sr.ReadLine());
			}

            if (latest > (int)e.UserState)
            {
                if (MessageBox.Show("Update Is Available, Download Now?", "Update Available", MessageBoxButtons.YesNo, MessageBoxIcon.Question, MessageBoxDefaultButton.Button2) == DialogResult.Yes)
                {
                    ProcessStartInfo startInfo = new ProcessStartInfo(Application.StartupPath + "\\Updater.exe");
					startInfo.Arguments = string.Format("{0} {1} {2} {3} {4}",
						DownloadDirectory.OriginalString, UserName, Password, ProgramName, latest.ToString());

                    Process.Start(startInfo);
                    Application.Exit();
                }
            }
            else MessageBox.Show("You Already Have The Latest Version!", "No Update Available");
        }
    }
}