using System;
using System.Collections.Generic;
using System.Windows.Forms;
using YeloDebug;
using System.Threading;
using System.Xml;
using System.IO;
using YeloDebug.Exceptions;

namespace Yelo_Neighborhood
{
    static class Program
    {
        public static Xbox XBox { get { return _xbox; } }
        static Xbox _xbox;

        public static Main MainWindow { get { return _mainWindow; } }
        static Main _mainWindow;

        public static ScreenshotTool ScreenshotTool { get { return _screenshotTool; } }
        static ScreenshotTool _screenshotTool;

        public static XBoxLocator XBoxLocator { get { return _xboxLocator; } }
        static XBoxLocator _xboxLocator;

        public static LEDStateChanger LEDStateChanger { get { return _LEDStateChanger; } }
        static LEDStateChanger _LEDStateChanger;

        public static ModuleManager ModuleManager { get { return _moduleManager; } }
        static ModuleManager _moduleManager;

        public static NewModule NewModule { get { return _newModule; } }
        static NewModule _newModule;

        public static List<Executable> Executables { get { return _executables; } }
        static List<Executable> _executables = new List<Executable>();

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Control.CheckForIllegalCrossThreadCalls = false;
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            LoadExecutables();

            _xboxLocator = new XBoxLocator();
            _screenshotTool = new ScreenshotTool();
            _LEDStateChanger = new LEDStateChanger();
            _moduleManager = new ModuleManager();
			_newModule = new NewModule();

            if (Properties.Settings.Default.AutoDiscover) FindXBox();
            else FindXBox(Properties.Settings.Default.XBoxIP);
        }

        public static void LoadExecutables()
        {
            if (!File.Exists("Executables.xml"))
            {
				using (var sw = File.CreateText("Executables.xml"))
				{
					sw.WriteLine("<Executables></Executables>");
				}
            }
			using (var xr = XmlReader.Create(File.OpenRead("Executables.xml")))
			{
				Executable workingExe = null;
				while (!xr.EOF)
				{
					xr.Read();
					switch (xr.Name)
					{
						case "Executable":
							if (xr.NodeType == XmlNodeType.EndElement) continue;
							workingExe = new Executable()
                            { 
                                Name = xr.GetAttribute("Name"),
                                Filename = xr.GetAttribute("Filename")
                            };
							_executables.Add(workingExe);
							break;
						case "Script":
							if (xr.NodeType == XmlNodeType.EndElement) continue;
							Executable.Script script = new Executable.Script()
                            {
							    Name = xr.GetAttribute("Name"),
							    FileType = xr.GetAttribute("FileType"),
							    Code = xr.ReadInnerXml(),
                            };
							workingExe.Scripts.Add(script);
							break;
                        case "Module":
                            if (xr.NodeType == XmlNodeType.EndElement) continue;

                            uint baseAddress = 0;
                            try { baseAddress = Convert.ToUInt32(xr.GetAttribute("BaseAddress"), 16); }
                            catch (Exception ex)
                            {
                                MessageBox.Show(ex.Message, "Error Reading Module For: " + workingExe.Filename, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                                break;
                            }

                            Executable.Module module = new Executable.Module(xr.GetAttribute("Filename"), baseAddress)
                            { Name = xr.GetAttribute("Name") };
                            workingExe.Modules.Add(module);
                            break;
					}
				}
			}
        }

        static void AsyncConnect(string xbox)
        { new Thread(new ParameterizedThreadStart(Connect)).Start(xbox); }

        static void AsyncConnect()
        { new Thread(new ThreadStart(Connect)).Start(); }

        static void Connect(object xbox)
        {
            try
            { _xbox.Connect((string)xbox); }
            catch (Exception e)
            { MessageBox.Show(e.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation); }
            finally
            { _xboxLocator.Hide(); }
        }

        static void Connect()
        {
            try
            { _xbox.Connect(); }
            catch (Exception e)
            { MessageBox.Show(e.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Exclamation); }
            finally
            { _xboxLocator.Hide(); }
        }

        public static void FindXBox()
        {
             _xbox = new Xbox();
             AsyncConnect();
            _xboxLocator.ShowDialog();

            if (XBox.Ping())
            {
                if (_mainWindow == null)
                {
                    _mainWindow = new Main();
                    _mainWindow.ShowDialog();
                    XBox.Disconnect();
                }
            }
            else new Settings().ShowDialog();
        }

        public static void FindXBox(string xbox)
        {
            _xbox = new Xbox();
            AsyncConnect(xbox);
            _xboxLocator.ShowDialog();

            if (XBox.Ping())
            {
                if (_mainWindow == null)
                {
                    _mainWindow = new Main();
                    _mainWindow.ShowDialog();
                    XBox.Disconnect();
                }
            }
            else new Settings().ShowDialog();
        }
    };
}