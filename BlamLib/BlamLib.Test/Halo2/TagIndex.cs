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
		static TagInterface.TagGroupCollection kImportInfoTags = new TagInterface.TagGroupCollection(true,
			Blam.Halo2.TagGroups.coll,
			Blam.Halo2.TagGroups.phmo,
			Blam.Halo2.TagGroups.mode,
			Blam.Halo2.TagGroups.sbsp
		);

		/* 2010-02-06 report:
		 * tag		success	count	failed count	success rate		time to load (debug, no optimization)
		 * scenario		1109		256						77%			00:00:26.5022100
		 * globals		2232		356						84%			00:01:52.2828525
		 * 
		 * ALL			3341		612						82%			00:02:18.7850625
		 * 
		 * 18% of the tags can't be loaded due to versioning conflicts (namely, no upgrade code is written for a
		 * specific tag group\block)
		 * 
		 * Test computer
		 * CPU: Intel T2500 2GHz
		 * RAM:	1GB
		 * OS: Vista
		 * 
		 * 2010-02-12 report of my tags directory
		 * Total Tags: 5591
		 * Total Tags that need upgrading: 672
		 * Total: 88% of tags are 
		*/
		[TestMethod]
		public void Halo2TestTagIndex()
		{
			var bd = Program.Halo2.Manager;
            var datum_tagindex = bd.OpenTagIndex(BlamVersion.Halo2_PC, kTestInstallationRootPath, kTestTagsDir);
			var tagindex = bd.GetTagIndex(datum_tagindex) as Managers.TagIndex;
			Blam.DatumIndex datum_scnr_index, datum_matg_index;

			StartStopwatch();
			{
				StartSubStopwatch();
				datum_scnr_index = tagindex.Open(
					@"scenarios\multi\example\example", Blam.Halo2.TagGroups.scnr, 
					IO.ITagStreamFlags.LoadDependents);

				Assert.IsTrue(datum_scnr_index != Blam.DatumIndex.Null);

				Debug.LogFile.WriteLine("SCENARIO LOAD: Time taken: {0}", StopSubStopwatch());
			}
			{
				StartSubStopwatch();
				datum_matg_index = tagindex.Open(
					@"globals\globals", Blam.Halo2.TagGroups.matg, 
					IO.ITagStreamFlags.LoadDependents);

				Assert.IsTrue(datum_matg_index != Blam.DatumIndex.Null);

				Debug.LogFile.WriteLine("GLOBALS LOAD: Time taken: {0}", StopSubStopwatch());
			}
			Debug.LogFile.WriteLine("TAG INDEX: Time taken: {0}", StopStopwatch());

			using (var sw = new StreamWriter(Path.Combine(kTestResultsPath, "dump_tag_index.txt")))
 			{ tagindex.Dump(sw); }
// 			tagindex.ErrorDatabaseSave();
			Assert.IsTrue( tagindex.Unload(datum_scnr_index) );
			Assert.IsTrue( tagindex.Unload(datum_matg_index) );

			tagindex = null;
			bd.CloseTagIndex(datum_tagindex);
		}

		[TestMethod]
		public void Halo2TestTagIndexStringIds()
		{
			var bd = Program.Halo2.Manager;
            var datum_tagindex = bd.OpenTagIndex(BlamVersion.Halo2_PC, kTestInstallationRootPath, kTestTagsDir);
			var tagindex = bd.GetTagIndex(datum_tagindex) as Managers.TagIndex;

			var model_index = tagindex.Open(
					@"objects\characters\elite\elite", Blam.Halo2.TagGroups.mode);

			Assert.IsTrue(model_index != Blam.DatumIndex.Null);

			var model = tagindex[model_index].TagDefinition as Blam.Halo2.Tags.render_model_group;

			var sid = model.Name.ToString();

			Assert.IsTrue( tagindex.Unload(model_index) );

			tagindex = null;
			bd.CloseTagIndex(datum_tagindex);
		}
	};
}