/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using System.Threading;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace BlamLib.Test
{
	partial class Halo3
	{
		partial class ScenarioScriptInterop
		{
			public ScenarioScriptInterop(Blam.HaloOdst.CacheFile cf) : base(cf,
				new TagInterface.Block<Blam.Halo3.Tags.hs_scripts_block>(null, 0),
				0x3E0 + 0x4C,
				0x3F4 + 0x4C, 0x400 + 0x4C,
				0x4A4 + 0x4C)
			{
				hs_scripts = base.sncr_hs_scripts as TagInterface.Block<Blam.Halo3.Tags.hs_scripts_block>;
			}
		};
	};

	[TestClass]
	public partial class HaloOdst : BaseTestClass
	{
		[ClassInitialize]
		public static void Initialize(TestContext context)
		{
			(Program.GetManager(BlamVersion.HaloOdst_Xbox) as Managers.IStringIdController)
				.StringIdCacheOpen(BlamVersion.HaloOdst_Xbox);

			System.IO.Directory.CreateDirectory(kTestResultsPath);
		}
		[ClassCleanup]
		public static void Dispose()
		{
			(Program.GetManager(BlamVersion.HaloOdst_Xbox) as Managers.IStringIdController)
				.StringIdCacheClose(BlamVersion.HaloOdst_Xbox);
		}

		static readonly string[] kMapNames = {
			"mainmenu.map",
			"c100.map",
			"c200.map",
			"h100.map",
			"l200.map",
			"l300.map",
			"sc100.map",
			"sc110.map",
			"sc120.map",
			"sc130.map",
			"sc140.map",
			"sc150.map",
		};

		[TestMethod]
		public void HaloOdstTestCacheOutputXbox()
		{
			CacheFileOutputInfoArgs.TestThreadedMethod(TestContext,
				Halo3.CacheOutputInformationMethod,
				BlamVersion.HaloOdst_Xbox, kMapsDirectoryXbox, kMapNames);
		}

		[TestMethod]
		public void HaloOdstTestCacheLoadResourceXbox()
		{
			CacheFileOutputInfoArgs.TestThreadedMethod(TestContext,
				Halo3.CacheLoadResourceMethod,
				BlamVersion.HaloOdst_Xbox, kMapsDirectoryXbox, kMapNames);
		}

		#region DumpZoneData
		[TestMethod]
		public void HaloOdstTestDumpZoneDataXbox()
		{
			CacheFileOutputInfoArgs.TestThreadedMethod(TestContext, Halo3.DumpZoneDataMethod,
				BlamVersion.HaloOdst_Xbox, kMapsDirectoryXbox, kMapNames);
		}
		#endregion

		#region ScanForScriptFunctions
		static void ScanForScriptFunctionsImpl(string[] script_functions, Blam.HaloOdst.CacheFile cf)
		{
			var interop = new Halo3.ScenarioScriptInterop(cf);

			interop.FindFunctionNames(script_functions);
		}
		static void ScanForScriptFunctions(BlamVersion engine, string path, string[] script_functions)
		{
			using (var handler = new CacheHandler<Blam.HaloOdst.CacheFile>(engine, path))
			{
				var cf = handler.CacheInterface;
				cf.Read();

				ScanForScriptFunctionsImpl(script_functions, handler.CacheInterface);
			}
		}
		
		[TestMethod]
		public void HaloOdstTestScanForScriptFunctions()
		{
			string[] script_functions;
			var engine = BlamVersion.HaloOdst_Xbox;

			Scripts.InitializeScriptFunctionsList(engine, out script_functions);
			foreach (var s in kMapNames)
				ScanForScriptFunctions(engine, kMapsDirectoryXbox + s, script_functions);
			Scripts.OutputFunctionNames(false, kTestResultsPath, "halo_odst.functions.xml", script_functions);
		}
		#endregion
	};
}