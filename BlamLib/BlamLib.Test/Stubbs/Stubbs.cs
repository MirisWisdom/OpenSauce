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
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace BlamLib.Test
{
	[TestClass]
	public partial class Stubbs : BaseTestClass
	{
		internal const string kTestResultsPath = TestLibrary.kTestResultsPath + @"Stubbs\";

		[ClassInitialize]
		public static void Initialize(TestContext context)
		{
		}
		[ClassCleanup]
		public static void Dispose()
		{
		}

		const string kMapsDirectoryPc = @"C:\Mount\A\Bungie\GamesRelated\Stubbs\PC\Maps\";
		const string kMapsDirectoryXbox = @"";
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