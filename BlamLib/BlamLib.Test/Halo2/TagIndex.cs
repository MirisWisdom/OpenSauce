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
		const string kTestTagIndexTagsPath = @"C:\Program Files\Microsoft Games\Halo 2 Map Editor\";
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
			var datum_tagindex = bd.OpenTagIndex(BlamVersion.Halo2_PC, kTestTagIndexTagsPath);
			var tagindex = bd.GetTagIndex(datum_tagindex) as Managers.TagIndex;
			Blam.DatumIndex datum_scnr_index, datum_matg_index;

			DateTime dt = DateTime.Now;
			{
				DateTime dt_s = DateTime.Now;
				datum_scnr_index = tagindex.Open(
					@"scenarios\multi\example\example", Blam.Halo2.TagGroups.scnr, 
					IO.ITagStreamFlags.LoadDependents);

				Assert.IsTrue(datum_scnr_index != Blam.DatumIndex.Null);

				Debug.LogFile.WriteLine("SCENARIO LOAD: Time taken: {0}", (DateTime.Now - dt_s));
			}
			{
				DateTime dt_g = DateTime.Now;
				datum_matg_index = tagindex.Open(
					@"globals\globals", Blam.Halo2.TagGroups.matg, 
					IO.ITagStreamFlags.LoadDependents);

				Assert.IsTrue(datum_matg_index != Blam.DatumIndex.Null);

				Debug.LogFile.WriteLine("GLOBALS LOAD: Time taken: {0}", (DateTime.Now - dt_g));
			}
			Debug.LogFile.WriteLine("TAG INDEX: Time taken: {0}", (DateTime.Now - dt));

			using (var sw = new StreamWriter(Path.Combine(kTestResultsPath, "dump_tag_index.txt")))
 			{ tagindex.Dump(sw); }
// 			tagindex.ErrorDatabaseSave();
			Assert.IsTrue( tagindex.Unload(datum_scnr_index) );
			Assert.IsTrue( tagindex.Unload(datum_matg_index) );

			tagindex = null;
			bd.CloseTagIndex(datum_tagindex);
		}
	};
}