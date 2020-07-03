/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Xml;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace BlamLib.Test
{
	[TestClass]
	public partial class Halo4 : BaseTestClass
	{
		[ClassInitialize]
		public static void Initialize(TestContext context)
		{
			(Program.GetManager(BlamVersion.Halo4_Xbox) as Managers.IStringIdController)
				.StringIdCacheOpen(BlamVersion.Halo4_Xbox);

			Directory.CreateDirectory(kTestResultsPath);
		}
		[ClassCleanup]
		public static void Dispose()
		{
			(Program.GetManager(BlamVersion.Halo4_Xbox) as Managers.IStringIdController)
				.StringIdCacheClose(BlamVersion.Halo4_Xbox);
		}

		static bool MapNeedsUniqueName(string header_name)
		{
			return false;
		}
		static readonly string[] kMapNames_Retail = {
			@"Retail\maps\mainmenu.map",

			@"Retail\maps\m05_prologue.map",
			@"Retail\maps\m10_crash.map",
			@"Retail\maps\m020.map",
			@"Retail\maps\m30_cryptum.map",
			@"Retail\maps\m40_invasion.map",
			@"Retail\maps\m60_rescue.map",
			@"Retail\maps\m70_liftoff.map",
			@"Retail\maps\m80_delta.map",
			@"Retail\maps\m90_sacrifice.map",
			@"Retail\maps\m95_epilogue.map",

			@"Retail\maps\ff81_courtyard.map",
			@"Retail\maps\ff82_scurve.map",
			@"Retail\maps\ff84_temple.map",
			@"Retail\maps\ff86_sniperalley.map",
			@"Retail\maps\ff87_chopperbowl.map",
			@"Retail\maps\ff90_fortsw.map",
			@"Retail\maps\ff91_complex.map",
			@"Retail\maps\ff92_valhalla.map",

			@"Retail\maps\ca_blood_cavern.map",
			@"Retail\maps\ca_blood_crash.map",
			@"Retail\maps\ca_canyon.map",
			@"Retail\maps\ca_forge_bonanza.map",
			@"Retail\maps\ca_forge_erosion.map",
			@"Retail\maps\ca_forge_ravine.map",
			@"Retail\maps\ca_gore_valley.map",
			@"Retail\maps\ca_redoubt.map",
			@"Retail\maps\ca_tower.map",
			@"Retail\maps\ca_warhouse.map",
			@"Retail\maps\wraparound.map",
			@"Retail\maps\z05_cliffside.map",
			@"Retail\maps\z11_valhalla.map",

			// dlc_crimson
			@"Retail\dlc_crimson\dlc_dejewel.map",
			@"Retail\dlc_crimson\dlc_dejunkyard.map",
			@"Retail\dlc_crimson\zd_02_grind.map",
		};

		// NOTE: Getting out of memory exceptions sometimes when running H4 cache output tests...
		// Need to verify if this is BlamLib's StringId system's fault or just due to the fact that
		// Halo4 maps have loads of strings and we process multiple maps at a time (ThreadPool queue)

		#region CacheOutputInformation
		static void CacheOutputInformationMethod(object param)
		{
			var args = param as CacheFileOutputInfoArgs;

			using (var handler = new CacheHandler<Blam.Halo4.CacheFile>(args.Game, args.MapPath))
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
		public void Halo4TestCacheOutputXbox()
		{
			CacheFileOutputInfoArgs.TestThreadedMethod(TestContext,
				CacheOutputInformationMethod,
				BlamVersion.Halo4_Xbox, kDirectoryXbox, kMapNames_Retail);
		}
		#endregion
	};
}