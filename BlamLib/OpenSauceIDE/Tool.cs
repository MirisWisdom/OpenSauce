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
using System.Collections.Generic;
using System.Linq;
using System.Text;
using OpenSauceIDE.CheApeInterface;

namespace BlamLib
{
	internal static partial class Tool
	{
		public delegate void CommandFunction(List<string> args);

		public static Dictionary<string, CommandFunction> kCommands = new Dictionary<string, CommandFunction>()
		{
			{"build-tag-database", BuildTagDatabase},
			{"apply-cheape", UnlockBlamTools}, //apply-cheape version g_path t_path s_path output
		};

		static void BuildTagDatabase(List<string> args)
		{
			if (args.Count < 2)
			{
				Console.WriteLine("");
				return;
			}

			string arg_source_tag = args[0];
			string arg_dest_db = args[1];
		}

		static void UnlockBlamTools(List<string> args)
		{
			if (args.Count < 5)
			{
				Console.WriteLine("error: invalid command argument count");
				return;
			}

			BlamVersion version = BlamVersion.Unknown;
			switch (args[0])
			{
				case "Halo1_CE":
					version = BlamVersion.Halo1_CE;
					break;
                //case "Halo2_PC":
                //    version = BlamVersion.Halo2_PC;
                //    break;
				default:
					Console.WriteLine("error: unsupported engine version - {0}", version.ToString());
					break;
			};
			Console.WriteLine("Applying CheApe modifications...");
			bool no_errors = true;
			switch (version)
			{
				case BlamVersion.Halo1_CE:
					try
					{
						var h1 = new UnlockH1(args[4], args[1], args[2], args[3]);
						h1.Unlock(false);
						h1.Close();
					}
					catch (Exception ex)
					{
						Console.WriteLine("error: an exception occurred when attempting to apply the CheApe modifications");
						Console.WriteLine();
						Console.WriteLine("exception details:");
						Console.WriteLine(ex.Message);
						Console.WriteLine();
						no_errors = false;
					}
					break;
			}

			string msg = no_errors ?
				"CheApe successfully applied!" :
				"There was an error while trying to apply CheApe. Validate that you selected copies of the original tools and try again.";
			Console.WriteLine(msg);
		}
	};
}