/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace OpenSauceIDE
{
	static class Program
	{
		public static bool WorkInProgress(IWin32Window owner)
		{
			bool wip = true;
			if (wip)
				MessageBox.Show(owner, "This is a WIP", "Check back later...",
					MessageBoxButtons.OK, MessageBoxIcon.Information);
			return wip;
		}

		public const uint PeAddressMask = 0x400000;

		public static string ReadCString(uint offset, BlamLib.IO.EndianReader io)
		{
			if (offset == 0) return string.Empty;

			uint pos = io.PositionUnsigned;

			io.Seek(offset);

			byte btchar = 0;
			var cstring = new System.Text.StringBuilder();

			do
			{
				btchar = io.ReadByte();
				if (btchar != 0)
					cstring.Append((char)btchar);

			} while (btchar != 0);

			io.Seek(pos);

			return cstring.ToString();
		}

		[System.Runtime.InteropServices.DllImport("kernel32.dll")]
		static extern bool AllocConsole();
		[System.Runtime.InteropServices.DllImport("kernel32.dll")]
		static extern bool FreeConsole();

		static int MainTool(string[] args)
		{
			var command = args[1];

			var cmd_args = new List<string>();
			for (int x = 2; x < args.Length; x++)
				cmd_args.Add(args[x]);

			AllocConsole();

			int result_code = 0;
			try
			{
				if (!BlamLib.Tool.RunCommand(command, out result_code, cmd_args.ToArray()))
					BlamLib.Tool.PrintUsage(command);
			}
			catch (Exception ex)
			{
				Console.WriteLine("Unhandled tool exception: {0}", ex.Message);
				Console.WriteLine(ex.StackTrace);
			}

			System.Threading.Thread.Sleep(5000);
			FreeConsole();

			return result_code;
		}

		/// <summary>The main entry point for the application.</summary>
		[STAThread]
		static int Main(string[] args)
		{
			bool cmdline = false;
			if(args.Length > 0)
			{
				switch(args[0])
				{
					case "-cmdline":	cmdline = true;	break;
				}
			}

			if (cmdline && args.Length > 1)
				return MainTool(args);
			else if(!cmdline)
			{
				Settings.SettingsManager.LoadSettings();

				Application.EnableVisualStyles();
				Application.SetCompatibleTextRenderingDefault(false);
				Application.Run(new MainForm(args));

				Settings.SettingsManager.SaveSettings();
			}

			return 0;
		}
	};
}