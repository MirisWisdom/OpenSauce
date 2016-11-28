/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using System.IO;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace BlamLib.Test
{
	partial class Halo2
	{
		#region ImportInfoExtraction
		static void ExtractImportInfo(Blam.Halo2.Tags.global_tag_import_info_block tii, string out_path)
		{
			if (tii != null) foreach (var b in tii.Files)
			{
				var path = Path.Combine(out_path, b.Path.Value);

				var dir = Path.GetDirectoryName(path);
				if (!Directory.Exists(dir)) Directory.CreateDirectory(dir);

				using (var fs = File.Create(path))
				{
					byte[] data = b.Decompress();
					fs.Write(data, 0, data.Length);
				}
			}
		}
		static void ExtractImportInfo(string test_results_path, Managers.TagIndex ti, TagInterface.TagGroup group, params string[] files)
		{
			int ti_dir_length = ti.Directory.Length;
			int group_length = group.Name.Length;

			foreach (string f in files)
			{
				var t = f.Substring(ti_dir_length); // remove tags dir
				t = t.Remove(t.Length - group_length - 1); // remove extension

				var tag_index = ti.Open(t, group);
				if (Managers.TagIndex.IsSentinel(tag_index))
					continue;

				Assert.IsTrue(tag_index != Blam.DatumIndex.Null);

				var tagman = ti[tag_index];
				var import_def = tagman.TagDefinition as Blam.Halo2.Tags.ITagImportInfo;
				ExtractImportInfo(import_def.GetImportInfo(), test_results_path);

				ti.Unload(tag_index);
			}
		}
		[TestMethod]
		public void Halo2TestImportInfoExtraction()
		{
			string test_results_path = EngineGetTestResultsPath(BlamVersion.Halo2_PC);

            string[] coll = Directory.GetFiles(kTestInstallationRootPath + kTestTagsDir, 
				Blam.Halo2.TagGroups.coll.ToFileSearchPattern(), SearchOption.AllDirectories);
			string[] phmo = Directory.GetFiles(kTestInstallationRootPath + kTestTagsDir,
				Blam.Halo2.TagGroups.phmo.ToFileSearchPattern(), SearchOption.AllDirectories);
			string[] mode = Directory.GetFiles(kTestInstallationRootPath + kTestTagsDir,
				Blam.Halo2.TagGroups.mode.ToFileSearchPattern(), SearchOption.AllDirectories);

			var bd = Program.Halo2.Manager;
            var datum_tagindex = bd.OpenTagIndex(BlamVersion.Halo2_PC, kTestInstallationRootPath, kTestTagsDir);
			var tagindex = bd.GetTagIndex(datum_tagindex) as Managers.TagIndex;

			ExtractImportInfo(test_results_path, tagindex, Blam.Halo2.TagGroups.coll, coll);
			ExtractImportInfo(test_results_path, tagindex, Blam.Halo2.TagGroups.phmo, phmo);
			ExtractImportInfo(test_results_path, tagindex, Blam.Halo2.TagGroups.mode, mode);

			tagindex = null;
			bd.CloseTagIndex(datum_tagindex);
		}
		#endregion

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

			args.SignalFinished();
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
			StartStopwatch();
			Program.Halo2.LoadPc(
				kMapsDirectoryPc + @"mainmenu.map",
				kMapsDirectoryPc + @"shared.map",
				kMapsDirectoryPc + @"single_player_shared.map");
			Assert.IsNotNull(Program.Halo2.PcMainmenu);
			Assert.IsNotNull(Program.Halo2.PcShared);
			Assert.IsNotNull(Program.Halo2.PcCampaign);

			StartSubStopwatch();
			Halo2TestCacheExtraction(BlamVersion.Halo2_PC, kMapsDirectoryPc, 
				//"00a_introduction.map"
				//"example.map"
				"03b_newmombasa.map"
				);
			Console.WriteLine("Halo2TestCacheExtractionPc: Extract time: {0}", StopSubStopwatch());

			Console.WriteLine("Halo2TestCacheExtractionPc: Overall time: {0}", StopStopwatch());
		}

		[TestMethod]
		public void Halo2TestCacheExtractionXbox()
		{
			StartStopwatch();
			Program.Halo2.LoadXbox(
				kMapsDirectoryXbox + @"mainmenu.map",
				kMapsDirectoryXbox + @"shared.map",
				kMapsDirectoryXbox + @"single_player_shared.map");
			Assert.IsNotNull(Program.Halo2.XboxMainmenu);
			Assert.IsNotNull(Program.Halo2.XboxShared);
			Assert.IsNotNull(Program.Halo2.XboxCampaign);

			StartSubStopwatch();
			Halo2TestCacheExtraction(BlamVersion.Halo2_Xbox, kMapsDirectoryXbox, "00a_introduction.map");
			Console.WriteLine("Halo2TestCacheExtractionXbox: Extract time: {0}", StopSubStopwatch());

			Console.WriteLine("Halo2TestCacheExtractionXbox: Overall time: {0}", StopStopwatch());
		}
	};
}