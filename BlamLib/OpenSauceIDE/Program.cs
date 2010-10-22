/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace OpenSauceIDE
{
	static class Program
	{
		public const uint PeAddressMask = 0x400000;

		public static string ReadCString(uint offset, BlamLib.IO.EndianReader io)
		{
			if (offset == 0) return string.Empty;

			uint pos = io.PositionUnsigned;

			io.Seek(offset);

			byte btchar = 0;
			string cstring = "";

			do
			{
				btchar = io.ReadByte();
				if (btchar != 0)
					cstring += (char)btchar;

			} while (btchar != 0);

			io.Seek(pos);

			return cstring;
		}

		[System.Runtime.InteropServices.DllImport("kernel32.dll")]
		static extern bool AllocConsole();
		[System.Runtime.InteropServices.DllImport("kernel32.dll")]
		static extern bool FreeConsole();

		

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string[] args)
		{
			bool cmdline = false;
			if(args.Length > 0)
			{
				switch(args[0])
				{
					case "-cmdline":
						cmdline = true;
						break;
				}
			}

			if (cmdline && args.Length > 1)
			{
				var commands = BlamLib.Tool.kCommands;

				List<string> cmd_args = new List<string>();
				for (int x = 2; x < args.Length; x++)
					cmd_args.Add(args[x]);

				AllocConsole();

				BlamLib.Tool.CommandFunction func;
				if (commands.TryGetValue(args[1], out func))
					func(cmd_args);
				else
				{
					Console.WriteLine("This is not a command: {0}", args[1]);
					Console.WriteLine("Use one of these commands:");
					foreach (string name in commands.Keys)
						Console.WriteLine(name);
				}

				System.Threading.Thread.Sleep(5000);
				FreeConsole();
			}
			else if(!cmdline)
			{
				Application.EnableVisualStyles();
				Application.SetCompatibleTextRenderingDefault(false);
				Application.Run(new MainForm(args));
			}
		}
	};
}