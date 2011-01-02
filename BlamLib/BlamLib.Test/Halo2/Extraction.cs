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
	partial class Halo2
	{
		static readonly TagInterface.TagGroupCollection kExtractionDontUseTags = new TagInterface.TagGroupCollection(true,
			Blam.Halo2.TagGroups.ugh_,
		#region These will never appear in a cache file anyway
// 			Blam.Halo2.TagGroups.srscen,
// 			Blam.Halo2.TagGroups.srbipd,
// 			Blam.Halo2.TagGroups.srvehi,
// 			Blam.Halo2.TagGroups.sreqip,
// 			Blam.Halo2.TagGroups.srweap,
// 			Blam.Halo2.TagGroups.srssce,
// 			Blam.Halo2.TagGroups.srligh,
// 			Blam.Halo2.TagGroups.srdgrp,
// 			Blam.Halo2.TagGroups.srdeca,
// 			Blam.Halo2.TagGroups.srcine,
// 			Blam.Halo2.TagGroups.srtrgr,
// 			Blam.Halo2.TagGroups.srclut,
// 			Blam.Halo2.TagGroups.srcrea,
// 			Blam.Halo2.TagGroups.srdcrs,
// 			Blam.Halo2.TagGroups.srsslt,
// 			Blam.Halo2.TagGroups.srhscf,
// 			Blam.Halo2.TagGroups.srai,
// 			Blam.Halo2.TagGroups.srcmmt,
		#endregion

// 			Blam.Halo2.TagGroups.ltmp,
// 			Blam.Halo2.TagGroups.sbsp,
			Blam.Halo2.TagGroups.spas,

			Blam.Halo2.TagGroups.snd_  // TODO: reconstruction
		);

		static void CacheExtractionMethod(object param)
		{
			var args = param as CacheFileOutputInfoArgs;

			using (var handler = new CacheHandler<Blam.Halo2.CacheFile>(args.Game, args.MapPath))
			{
				handler.Read();
				var cache = handler.CacheInterface;

				var ti = cache.TagIndexManager as Blam.Halo2.InternalCacheTagIndex;
				ti.ExtractionInitialize();
				Assert.IsNotNull(ti.kVertexBuffers);
				{
					string test_results_tags_path = EngineGetTestResultsPath(args.Game) + @"tags\";

					// extract with dependents, database and overwrite existing tag files
					var ex_args = new Blam.CacheExtractionArguments(test_results_tags_path,
						true, true, true, kExtractionDontUseTags);
					Blam.CacheIndex.Item tag_item;
					//Assert.IsTrue(cache.TryAndFind(@"objects\characters\masterchief\masterchief", Blam.Halo2.TagGroups.hlmt, out tag_item));
					//Assert.IsTrue(cache.TryAndFind(@"scenarios\solo\07a_highcharity\07a_highcharity_high_0_lightmap", Blam.Halo2.TagGroups.ltmp, out tag_item));
					//Assert.IsTrue(cache.TryAndFind(@"scenarios\multi\example\example_example_lightmap", Blam.Halo2.TagGroups.ltmp, out tag_item));
					//Assert.IsTrue(cache.TryAndFind(@"scenarios\objects\covenant\military\scarab\scarab", Blam.Halo2.TagGroups.mode, out tag_item));
					Assert.IsTrue(cache.TryAndFind(@"scenarios\solo\03b_newmombasa\earthcity_4", Blam.Halo2.TagGroups.sbsp, out tag_item));
					//Assert.IsTrue(cache.TryAndFind(@"ui\hud\hud_messages", Blam.Halo2.TagGroups.unic, out tag_item));
					{
						var cei = ti.ExtractionBegin(tag_item.Datum, ex_args);
						Assert.IsTrue(ti.Extract(cei));
						ti.ExtractionEnd();
					}
				}
				ti.ExtractionDispose();
			}
		}

		void Halo2TestCacheExtraction(BlamVersion game, string dir, params string[] map_names)
		{
			(Program.GetManager(game) as Managers.IStringIdController)
				.StringIdCacheOpen(game);
			(Program.GetManager(game) as Managers.IVertexBufferController)
				.VertexBufferCacheOpen(game);

			CacheFileOutputInfoArgs.TestThreadedMethod(TestContext,
				CacheExtractionMethod,
				game, dir, map_names);

			(Program.GetManager(game) as Managers.IVertexBufferController)
				.VertexBufferCacheClose(game);
			(Program.GetManager(game) as Managers.IStringIdController)
				.StringIdCacheClose(game);
		}

		[TestMethod]
		public void Halo2TestCacheExtractionPc()
		{
			BlamLib.Program.Halo2.LoadPc(
				kMapsDirectoryPc + @"mainmenu.map",
				kMapsDirectoryPc + @"shared.map",
				kMapsDirectoryPc + @"single_player_shared.map");
			Assert.IsNotNull(Program.Halo2.PcMainmenu);
			Assert.IsNotNull(Program.Halo2.PcShared);
			Assert.IsNotNull(Program.Halo2.PcCampaign);

			Halo2TestCacheExtraction(BlamVersion.Halo2_PC, kMapsDirectoryPc, 
				//"00a_introduction.map"
				//"example.map"
				"03b_newmombasa.map"
				);
		}

		[TestMethod]
		public void Halo2TestCacheExtractionXbox()
		{
			Program.Halo2.LoadXbox(
				kMapsDirectoryXbox + @"mainmenu.map",
				kMapsDirectoryXbox + @"shared.map",
				kMapsDirectoryXbox + @"single_player_shared.map");
			Assert.IsNotNull(Program.Halo2.XboxMainmenu);
			Assert.IsNotNull(Program.Halo2.XboxShared);
			Assert.IsNotNull(Program.Halo2.XboxCampaign);

			Halo2TestCacheExtraction(BlamVersion.Halo2_Xbox, kMapsDirectoryXbox, "00a_introduction.map");
		}
	};
}