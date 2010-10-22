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
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace BlamLib.Test
{
	partial class Halo3
	{
		internal partial class ScenarioScriptInterop : ScenarioScriptInteropGen3
		{
			TagInterface.Block<Blam.Halo3.Tags.hs_scripts_block> hs_scripts;

			public ScenarioScriptInterop(Blam.Halo3.CacheFile cf) : base(cf,
				new TagInterface.Block<Blam.Halo3.Tags.hs_scripts_block>(null, 0),
				0x3E0,
				0x3F4, 0x400,
				0x4A4)
			{
				hs_scripts = base.sncr_hs_scripts as TagInterface.Block<Blam.Halo3.Tags.hs_scripts_block>;
			}
		};

		#region ScanForScriptFunctions
		static void ScanForScriptFunctionsImpl(string[] script_functions, Blam.Halo3.CacheFile cf)
		{
			var interop = new ScenarioScriptInterop(cf);

			interop.FindFunctionNames(script_functions);
		}
		static void ScanForScriptFunctions(BlamVersion engine, string path, string[] script_functions)
		{
			using (var handler = new CacheHandler<Blam.Halo3.CacheFile>(engine, path))
			{
				var cf = handler.CacheInterface;
				cf.Read();

				ScanForScriptFunctionsImpl(script_functions, handler.CacheInterface);
			}
		}
		
		[TestMethod]
		public void Halo3TestScanForScriptFunctionsXbox()
		{
			string[] script_functions;
			var engine = BlamVersion.Halo3_Xbox;

			Scripts.InitializeScriptFunctionsList(engine, out script_functions);
			foreach (var s in kMapNames_Retail)
				ScanForScriptFunctions(engine, kDirectoryXbox + s, script_functions);
			Scripts.OutputFunctionNames(false, kTestResultsPath, "halo3.functions.xml", script_functions);
		}
		#endregion
	};
}