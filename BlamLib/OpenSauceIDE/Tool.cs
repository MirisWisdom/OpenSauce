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

namespace BlamLib
{
	internal static partial class Tool
	{
		public delegate void CommandFunction(params string[] args);

		public static Dictionary<string, CommandFunction> kCommands = new Dictionary<string, CommandFunction>()
		{
			{"build-tag-database", BuildTagDatabase},
			{"apply-cheape", UnlockBlamTools}, //apply-cheape version g_path t_path s_path output
			{"apply-opensauce", UnlockBlamGames}, //apply-opensauce version c_path d_path output
		};

		public static bool RunCommand(string command, params string[] args)
		{
			BlamLib.Tool.CommandFunction func;
			if (kCommands.TryGetValue(command, out func))
			{
				func(args);
				return true;
			}

			return false;
		}

		public static void PrintUsage(string invalid_command)
		{
			if (invalid_command != null)
			{
				Console.WriteLine("This is not a command: {0}", invalid_command);
				Console.WriteLine("Use one of these commands:");
				foreach (var name in kCommands.Keys)
					Console.WriteLine(name);
			}
		}

		static void BuildTagDatabase(params string[] args)
		{
			if (args.Length < 2)
			{
				Console.WriteLine("");
				return;
			}

			string arg_source_tag = args[0];
			string arg_dest_db = args[1];
		}
	};
}