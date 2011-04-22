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
﻿using System;
using System.IO;

namespace OpenSauceIDE.GameInterface
{
	partial class UnlockH1 : UnlockGameBase
	{
		abstract class PlatformInterface : PlatformInterfaceBase
		{
			public PlatformInterface(string output_dir, string path, Platform pv) : base(pv)
			{
				if (!ValidateExe(path))
					throw new BlamLib.Debug.ExceptionLog("{0} is not a supported {1} exe", path, pv.ToString());

				string dir = string.IsNullOrEmpty(output_dir) ? Path.GetDirectoryName(path) : output_dir;
				string name = Path.GetFileName(path);
				name = Path.Combine(dir, "OS_" + name);
				File.Copy(path, name, true);
				output = new BlamLib.IO.EndianWriter(name);
				output.BaseAddress = Program.PeAddressMask;
			}

			#region EnableOpenSauce
			void EnableOpenSauce()
			{
				output.Seek(AddressOf.OpenSauceDllString[PlatformVersion] - Program.PeAddressMask);
				output.Write(Constants.OpenSauceDllString[PlatformVersion]);

				output.Seek(AddressOf.OpenSauceFunction1[PlatformVersion] - Program.PeAddressMask);
				output.Write(Constants.OpenSauceFunction1[PlatformVersion]);

				output.Seek(AddressOf.OpenSauceFunction2[PlatformVersion] - Program.PeAddressMask);
				output.Write(Constants.OpenSauceFunction2[PlatformVersion]);
			}
			#endregion

			public override void Unlock(bool debug)
			{
				EnableOpenSauce();
			}
		};

		#region Client
		class Client : PlatformInterface
		{
			protected override bool ValidateExe(string path)
			{
				return UnlockExeBase.ValidateExe(path, 0x128, 0x4AC7FAF1);
			}

			public Client(string output_dir, string path) : base(output_dir, path, Platform.Client) { }

			public override void Unlock(bool debug)
			{
				base.Unlock(debug);
			}

		}; Client ClientInterface = null;
		#endregion

		#region Dedi
		class Dedi : PlatformInterface
		{
			protected override bool ValidateExe(string path)
			{
				return UnlockExeBase.ValidateExe(path, 0xF8, 0x4AC7FB12);
			}

			public Dedi(string output_dir, string path) : base(output_dir, path, Platform.Dedi) { }

			public override void Unlock(bool debug)
			{
				base.Unlock(debug);
			}

		}; Dedi DediInterface = null;
		#endregion

		public UnlockH1(string output_dir, string c_path, string d_path)
		{
#if false
			if (!string.IsNullOrEmpty(c_path))
				try { ClientInterface = new Client(output_dir, c_path); }
				catch (BlamLib.Debug.ExceptionLog) { EncounteredInvalidExe = true; }
#endif

			if (!string.IsNullOrEmpty(d_path))
				try { DediInterface = new Dedi(output_dir, d_path); }
				catch (BlamLib.Debug.ExceptionLog) { EncounteredInvalidExe = true; }
		}

		public override void Close()
		{
			if (ClientInterface != null) ClientInterface.Close();
			if (DediInterface != null) DediInterface.Close();
		}

		public override void Unlock(bool debug)
		{
			if (ClientInterface != null) ClientInterface.Unlock(debug);
			if (DediInterface != null) DediInterface.Unlock(debug);
		}
	};
}

namespace BlamLib
{
	partial class Tool
	{
		static void UnlockBlamGames(params string[] args)
		{
			const string kSupportedGameVersion = "1.09";

			if (args.Length < 4)
			{
				Console.WriteLine("error: invalid command argument count");
				return;
			}

			BlamVersion version = BlamVersion.Unknown;
			switch (args[0])
			{
				case "Halo1_CE": version = BlamVersion.Halo1_CE; break;
				//case "Halo2_PC": version = BlamVersion.Halo2_PC; break;
				default:
					Console.WriteLine("error: unsupported engine version - {0}", version.ToString());
					break;
			};

			Console.WriteLine("Applying {0} modifications...", "OpenSauce");
			Exception exception = null;
			OpenSauceIDE.GameInterface.UnlockGameBase unlocker = null;
			switch (version)
			{
				case BlamVersion.Halo1_CE:
					try					{ unlocker = new OpenSauceIDE.GameInterface.UnlockH1(args[3], args[1], args[2]); }
					catch (Exception ex){ exception = ex; }
					break;
// 				case BlamVersion.Halo2_PC:
// 					try					{ unlocker = new OpenSauceIDE.GameInterface.UnlockH2(args[3], args[1], args[2]); }
// 					catch (Exception ex){ exception = ex; }
// 					break;
			}

			if (unlocker != null) // If the unlocker api was initialized OK, run the unlocking operations
			{
				try
				{
					unlocker.Unlock(false);
					unlocker.Close();
				}
				catch (Exception ex)	{ exception = ex; }
			}

			if (exception != null)
			{
				Console.WriteLine("error: an exception occurred when attempting to apply the {0} modifications", "OpenSauce");
				Console.WriteLine();
				Console.WriteLine("exception details:");
				Console.WriteLine(exception.Message);
				Console.WriteLine();
			}

			string msg;
			if (exception == null)
				msg = "OpenSauce successfully applied!";
			else if (unlocker.EncounteredInvalidExe)
				msg = "OpenSauce couldn't be applied to some or all of the exes. Check the debug log for more details";
			else
				msg = "There was an error while trying to apply OpenSauce. Validate that you selected original copies of the " +
					"v" + kSupportedGameVersion + " game exes and try again.";

			Console.WriteLine(msg);
		}
	};
}