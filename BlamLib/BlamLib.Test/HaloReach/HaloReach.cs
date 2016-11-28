/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using System.Collections.Generic;
using System.IO;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace BlamLib.Test
{
	[TestClass]
	public partial class HaloReach : BaseTestClass
	{
		[ClassInitialize]
		public static void Initialize(TestContext context)
		{
			(Program.GetManager(BlamVersion.HaloReach_Beta) as Managers.IStringIdController)
				.StringIdCacheOpen(BlamVersion.HaloReach_Beta);

			Directory.CreateDirectory(kTestResultsPath);
		}
		[ClassCleanup]
		public static void Dispose()
		{
			(Program.GetManager(BlamVersion.HaloReach_Beta) as Managers.IStringIdController)
				.StringIdCacheClose(BlamVersion.HaloReach_Beta);
		}

		static bool MapNeedsUniqueName(string header_name)
		{
			switch (header_name)
			{
				case "20_sword_slayer":
				case "30_settlement":
				case "70_boneyard":
				case "ff10_prototype":
				case "mainmenu":
				// dlc_defiant
				case "condemned":
				case "ff_unearthed":
				case "trainingpreserve":
					return true;
				default: return false;
			}
		}
		static readonly string[] kMapNames_Retail = {
			// 11860.10.07.24.0147
			@"Retail\maps\20_sword_slayer.map",
			@"Retail\maps\30_settlement.map",
			@"Retail\maps\35_island.map",
			@"Retail\maps\45_aftship.map",
			@"Retail\maps\45_launch_station.map",
			@"Retail\maps\50_panopticon.map",
			@"Retail\maps\52_ivory_tower.map",
			@"Retail\maps\70_boneyard.map",

			@"Retail\maps\ff10_prototype.map",
			@"Retail\maps\ff20_courtyard.map",
			@"Retail\maps\ff30_waterfront.map",
			@"Retail\maps\ff45_corvette.map",
			@"Retail\maps\ff50_park.map",
			@"Retail\maps\ff60_airview.map",
			@"Retail\maps\ff60_icecave.map",
			@"Retail\maps\ff70_holdout.map",

			@"Retail\maps\forge_halo.map",

			@"Retail\maps\m05.map",
			@"Retail\maps\m10.map",
			@"Retail\maps\m20.map",
			@"Retail\maps\m30.map",
			@"Retail\maps\m35.map",
			@"Retail\maps\m45.map",
			@"Retail\maps\m50.map",
			@"Retail\maps\m52.map",
			@"Retail\maps\m60.map",
			@"Retail\maps\m70.map",
			@"Retail\maps\m70_a.map",
			@"Retail\maps\m70_bonus.map",

			@"Retail\maps\mainmenu.map",

			// dlc_noble
			@"Retail\dlc_noble\dlc_invasion.map",
			@"Retail\dlc_noble\dlc_medium.map",
			@"Retail\dlc_noble\dlc_slayer.map",

			// dlc_defiant
			@"Retail\dlc_defiant\p1\condemned.map",
			@"Retail\dlc_defiant\p1\ff_unearthed.map",
			@"Retail\dlc_defiant\p1\trainingpreserve.map",
			@"Retail\dlc_defiant\condemned.map",
			@"Retail\dlc_defiant\ff_unearthed.map",
			@"Retail\dlc_defiant\trainingpreserve.map",
		};
		static readonly string[] kMapNames_Beta = {
			// 09449.10.03.25.1545
			@"Beta\maps\20_sword_slayer.map",
			@"Beta\maps\30_settlement.map",
			@"Beta\maps\70_boneyard.map",
			@"Beta\maps\ff10_prototype.map",
			@"Beta\maps\mainmenu.map",
		};
		static readonly string[] kMapNames_Delta = {
			// 09730.10.04.09.1309
			@"Delta\maps\20_sword_slayer.map",
			@"Delta\maps\30_settlement.map",
			@"Delta\maps\70_boneyard.map",
			@"Delta\maps\ff10_prototype.map",
			@"Delta\maps\mainmenu.map",
		};

		#region CacheOutputInformation
		static void CacheOutputInformationMethod(object param)
		{
			var args = param as CacheFileOutputInfoArgs;

			using(var handler = new CacheHandler<Blam.HaloReach.CacheFile>(args.Game, args.MapPath))
			{
				handler.Read();
				var cache = handler.CacheInterface;

				string header_name = cache.Header.Name;
				if (MapNeedsUniqueName(header_name))
					header_name = cache.GetUniqueName();

				Blam.CacheFile.OutputStringIds(cache,
					BuildResultPath(kTestResultsPath, args.Game, header_name, "string_ids", "txt"), true);
				Blam.CacheFile.OutputTags(cache,
					BuildResultPath(kTestResultsPath, args.Game, header_name, null, "txt"));
			}

			args.SignalFinished();
		}
		[TestMethod]
		public void HaloReachTestCacheOutputXbox()
		{
			CacheFileOutputInfoArgs.TestThreadedMethod(TestContext,
				CacheOutputInformationMethod,
				BlamVersion.HaloReach_Xbox, kDirectoryXbox, kMapNames_Retail);
		}
		[TestMethod]
		public void HaloReachTestCacheOutputXboxBeta()
		{
			CacheFileOutputInfoArgs.TestThreadedMethod(TestContext,
				CacheOutputInformationMethod,
				BlamVersion.HaloReach_Beta, kDirectoryXbox, kMapNames_Delta);
		}
		#endregion

		class CacheSymbolInterface : IDisposable
		{
			class StringSymbols
			{
				public int Count;
				public byte[] ValueLengths;

				public void Build(IO.EndianReader stream, int offset,
					int count, int total_size)
				{
					stream.Seek(offset);
					int[] indicies = new int[count];
					for (int x = 0; x < indicies.Length; x++)
						indicies[x] = stream.ReadInt32();

					ValueLengths = new byte[count];
					for (int x = count-1, prev_offset = total_size; x >= 0; x--)
					{
						ValueLengths[x] = (byte)((prev_offset - indicies[x])-1);
						prev_offset = indicies[x];
					}

					Count = count;
				}

				void Output(StreamWriter sw, ref int index, int count)
				{
					sw.WriteLine(); sw.WriteLine();
					for(int x = 0; x < count && index < ValueLengths.Length; x++, index++)
						sw.Write("{0} ", ValueLengths[index].ToString());
				}
				public void Output(StreamWriter sw, int[] set_breakers)
				{
					bool use_set_breakers = set_breakers != null;

					int index = 0;
					if (use_set_breakers)
						foreach (int count in set_breakers)
							Output(sw, ref index, count);
					for (int x = index; x < ValueLengths.Length; x++)
						sw.Write("{0} ", ValueLengths[x].ToString());
				}
				public void OutputLines(StreamWriter sw)
				{
					for (int x = 0; x < ValueLengths.Length; x++)
						sw.WriteLine("{0}\t{1}", x.ToString("X4"), ValueLengths[x].ToString());
				}
			};

			CacheHandler<Blam.HaloReach.CacheFile> cacheHandler;
			public Blam.HaloReach.CacheFile Cache { get { return cacheHandler.CacheInterface; } }

			public CacheSymbolInterface(CacheFileOutputInfoArgs args)
			{
				cacheHandler = new CacheHandler<Blam.HaloReach.CacheFile>(args.Game, args.MapPath);
				cacheHandler.Read();
			}

			#region IDisposable Members
			public void Dispose()
			{
				if (cacheHandler != null)
				{
					cacheHandler.Dispose();
					cacheHandler = null;
				}
			}
			#endregion

			public int StringIdCacheSetStartIndex; // Raw index where the cache's string ids start
			public int[] StringIdSetBreakers;
			StringSymbols stringIdSymbols;
			public void BuildStringIdSymbols()
			{
				var c = cacheHandler.CacheInterface;
				var head = c.HeaderHaloReach;
				var stream = c.InputStream;

				stringIdSymbols = new StringSymbols();
				stringIdSymbols.Build(stream, head.StringIdIndicesOffset,
					head.StringIdsCount, head.StringIdsBufferSize);
			}

			public void OutputStringIdSymbols(string path)
			{
				using (var sw = new StreamWriter(path))
				{
					sw.WriteLine("Engine Count: {0}", StringIdCacheSetStartIndex.ToString());
					stringIdSymbols.Output(sw, StringIdSetBreakers);
				}
			}

			StringSymbols tagNameSymbols;
			public void BuildTagNameSymbols()
			{
				var c = cacheHandler.CacheInterface;
				var head = c.HeaderHaloReach;
				var stream = c.InputStream;

				tagNameSymbols = new StringSymbols();
				tagNameSymbols.Build(stream, head.TagNameIndicesOffset,
					head.TagNamesCount, head.TagNamesBufferSize);
			}

			public void OutputTagNameSymbols(string path)
			{
				using (var sw = new StreamWriter(path))
					tagNameSymbols.Output(sw, null);
			}
		};
		[TestMethod]
		public void HaloReachBuildCacheSymbols()
		{
			CacheSymbolInterface delta = new CacheSymbolInterface(new CacheFileOutputInfoArgs(TestContext,
				BlamVersion.HaloReach_Beta, kDirectoryXbox, kMapNames_Beta[0]));
			CacheSymbolInterface retail = new CacheSymbolInterface(new CacheFileOutputInfoArgs(TestContext,
				BlamVersion.HaloReach_Xbox, kDirectoryXbox, kMapNames_Retail[0]));

			string results_path;

			delta.BuildStringIdSymbols();
			{
				var c = delta.Cache;
				var sid_cache = c.StringIds;

				delta.StringIdSetBreakers = new int[15] {
					1124, // 0
					1476, // 1
					170,  // 2
					101,  // 3
					213,  // 4 
					37,   // 5
					5,    // 6
					1416, // 7
					324,  // 8
					15,   // 9
					92,   // A
					      // B - unused
					24,   // C
					13,   // D
					39,   // E
					64,   // F
				};
				delta.StringIdCacheSetStartIndex = sid_cache.StaticCount;
				results_path = BuildResultPath(kTestResultsPath, delta.Cache.EngineVersion, c.Header.Name, "symbols.string_ids", "txt");
				delta.OutputStringIdSymbols(results_path);

				results_path = BuildResultPath(kTestResultsPath, delta.Cache.EngineVersion, c.Header.Name, "symbols.tag_names", "txt");
			}
			retail.BuildStringIdSymbols();
			{
				var c = delta.Cache;

				retail.StringIdSetBreakers = new int[15] {
					1198, // 0 - 0x4AE
					1637, // 1 - 0x665
					216,  // 2 - 0xD8
					106,  // 3 - 0x6A
					217,  // 4 - 0xD9
					38,   // 5 - 0x26
					5,    // 6 - 0x5
					1725, // 7 - 0x6BD
					367,  // 8 - 0x16F
					20,   // 9 - 0x14
					98,   // A - 0x62
					      // B - unused
					24,   // C - 0x18
					13,   // D - 0xD
					41,   // E - 0x29
					97,   // F - 0x61
				};
				retail.StringIdCacheSetStartIndex = 5802;
				results_path = BuildResultPath(kTestResultsPath, retail.Cache.EngineVersion, c.Header.Name, "symbols.string_ids", "txt");
				retail.OutputStringIdSymbols(results_path);

				results_path = BuildResultPath(kTestResultsPath, retail.Cache.EngineVersion, c.Header.Name, "symbols.tag_names", "txt");
			}
		}


		class ScenarioScriptInterop : ScenarioScriptInteropGen3
		{
			TagInterface.Block<Blam.HaloReach.Tags.hs_scripts_block> hs_scripts;

			public ScenarioScriptInterop(Blam.HaloReach.CacheFile cf) : base(cf,
				new TagInterface.Block<Blam.HaloReach.Tags.hs_scripts_block>(null, 0),
				0x430,
				0x444, 0x450,
				0x518)
			{
				hs_scripts = base.sncr_hs_scripts as TagInterface.Block<Blam.HaloReach.Tags.hs_scripts_block>;
			}
		};

		// names: 104\8
		// bipd: 128\78
		// vehi: 140\D0
		// weap: 170\D0
		// eqip: 158\B4
		// term: 1AC\7C
		// scen: 110\DC
		// mach: 194\E4
		// ctrl: 1C4\DC
		// sscn: 1DC\8C
		// bloc: 614\D8
		// crea: 654\58
		// gint: 1F4\88
		// efsc: 20C\B0

		#region DumpScriptGraphs
		static void CacheDumpScriptGraphs(object param)
		{
			var args = param as CacheFileOutputInfoArgs;

			using (var handler = new CacheHandler<Blam.HaloReach.CacheFile>(args.Game, args.MapPath))
			{
				handler.Read();
				var cache = handler.CacheInterface;

				var filename = System.IO.Path.Combine(kTestResultsPath, cache.Header.Name + ".script_graph.txt");
				using(var sw = new StreamWriter(filename))
				{
					var interop = new ScenarioScriptInterop(cache);
					interop.DumpScriptGraphs(cache, sw);
				}
			}

			args.SignalFinished();
		}

		[TestMethod]
		public void HaloReachTestDumpScriptGraphs()
		{
			CacheFileOutputInfoArgs.TestThreadedMethod(TestContext,
				CacheDumpScriptGraphs,
				BlamVersion.HaloReach_Xbox, kDirectoryXbox, kMapNames_Retail);
		}
		#endregion

		#region ScanForScriptFunctions
		static void ScanForScriptFunctionsImpl(string[] script_functions, Blam.HaloReach.CacheFile cf)
		{
			var interop = new ScenarioScriptInterop(cf);

			interop.FindFunctionNames(script_functions);
		}
		static void ScanForScriptFunctions(BlamVersion engine, string path, string[] script_functions)
		{
			using (var handler = new CacheHandler<Blam.HaloReach.CacheFile>(engine, path))
			{
				var cf = handler.CacheInterface;
				cf.Read();

				ScanForScriptFunctionsImpl(script_functions, handler.CacheInterface);
			}
		}
		
		[TestMethod]
		public void HaloReachTestScanForScriptFunctions()
		{
			string[] script_functions;
			var engine = BlamVersion.HaloReach_Xbox;

			Scripts.InitializeScriptFunctionsList(engine, out script_functions);
			foreach (var s in kMapNames_Retail)
				ScanForScriptFunctions(engine, kDirectoryXbox + s, script_functions);
			Scripts.OutputFunctionNames(false, kTestResultsPath, "halo_reach.functions.xml", script_functions);
		}
		#endregion
	};
}