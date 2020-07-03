/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace BlamLib.Test
{
	[TestClass]
	public partial class Stubbs : BaseTestClass
	{
		[ClassInitialize]
		public static void Initialize(TestContext context)
		{
		}
		[ClassCleanup]
		public static void Dispose()
		{
		}

		static readonly string[] kMapNames = {
			"ui.map",
			"a10_plaza.map",
			"a30_greenhouse.map",
			"a40_police_station.map",
			"a45_dance.map",
			"a50_maul.map",
			"a60_maulfight.map",
			"b10_farm_house.map",
			"b30_dam.map",
			"c10_offender.map",
			"c30_lab.map",
			"c40_cityhall.map",
			"c50_end.map",
		};

		#region TestMapOutput
		static void CacheOutputInformation(object param)
		{
			var args = param as CacheFileOutputInfoArgs;

			using (var handler = new CacheHandler<Blam.Stubbs.CacheFile>(args.Game, args.MapPath))
			{
				handler.Read();
				var cache = handler.CacheInterface;

				BlamLib.Blam.CacheFile.OutputTags(cache,
					BuildResultPath(kTestResultsPath, args.Game, cache.Header.Name, null, "txt"));
			}

			args.SignalFinished();
		}
		void CacheOutputInformation(BlamVersion game)
		{
			string dir = null;
			if (game == BlamVersion.Stubbs_PC)
				dir = kMapsDirectoryPc;
			else if (game == BlamVersion.Stubbs_Xbox)
				dir = kMapsDirectoryXbox;

			if(!string.IsNullOrEmpty(dir))
			{
				CacheFileOutputInfoArgs.TestThreadedMethod(TestContext,
					CacheOutputInformation,
					game, dir, kMapNames);
			}
		}
		[TestMethod]
		public void StubbsTestCacheOutputPc()
		{
			CacheOutputInformation(BlamVersion.Stubbs_PC);
		}
		[TestMethod]
		public void StubbsTestCacheOutputXbox()
		{
			CacheOutputInformation(BlamVersion.Stubbs_Xbox);
		}
		#endregion
	};
}