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
	[TestClass]
	public partial class Halo1 : BaseTestClass
	{
		const string kTestResultsPath = TestLibrary.kTestResultsPath + @"Halo1\";

		[ClassInitialize]
		public static void Initialize(TestContext context)
		{
			Directory.CreateDirectory(kTestResultsPath);
			Directory.CreateDirectory(kTestResultsTagsPath);
		}
		[ClassCleanup]
		public static void Dispose()
		{
		}

		const string kTestResultsTagsPath = kTestResultsPath + @"tags\";

		const string kTestTagIndexTagsPath = @"C:\Program Files (x86)\Microsoft Games\Halo Custom Edition\";

		[TestMethod]
		public void Halo1TestTagIndex()
		{
			using (var handler = new TagIndexHandler<Managers.TagIndex>(BlamVersion.Halo1_CE, kTestTagIndexTagsPath))
			{
				var tagindex = handler.IndexInterface;
				Blam.DatumIndex datum_scnr_index, datum_matg_index;

				DateTime dt = DateTime.Now;
				{
					DateTime dt_s = DateTime.Now;
					datum_scnr_index = tagindex.Open(@"levels\test\tutorial\tutorial", Blam.Halo1.TagGroups.scnr, IO.ITagStreamFlags.LoadDependents);
					Console.WriteLine("SCENARIO LOAD: Time taken: {0}", (DateTime.Now - dt_s));
				}
 				{
 					DateTime dt_g = DateTime.Now;
					datum_matg_index = tagindex.Open(@"globals\globals", Blam.Halo1.TagGroups.matg, IO.ITagStreamFlags.LoadDependents);
 					Console.WriteLine("GLOBALS LOAD: Time taken: {0}", (DateTime.Now - dt_g));
 				}
				Console.WriteLine("TAG INDEX: Time taken: {0}", (DateTime.Now - dt));

				Managers.TagManager tag_manager_scnr = tagindex[datum_scnr_index];
				var tag_scenario = tag_manager_scnr.TagDefinition as Blam.Halo1.Tags.scenario_group;
				tag_scenario = null;
				tag_manager_scnr = null;
// 				using (var sw = new StreamWriter(Path.Combine(kTestResultsTagsPath, @"dump_tag_index.txt")))
// 				{ tagindex.Dump(sw); }
				Assert.IsTrue(tagindex.Unload(datum_scnr_index));
				Assert.IsTrue(tagindex.Unload(datum_matg_index));
			}
		}

		[TestMethod]
		public void Halo1TestTagIndexRar()
		{
			using (var handler = new TagIndexHandler<Managers.TagIndex>(BlamVersion.Halo1_CE, kTestTagIndexTagsPath))
			{
				var tagindex = handler.IndexInterface;
				Blam.DatumIndex datum_test_index;

				DateTime dt = DateTime.Now;
				{
					DateTime dt_s = DateTime.Now;
					datum_test_index = tagindex.Open(
						@"characters\cyborg\cyborg", Blam.Halo1.TagGroups.bipd,
						IO.ITagStreamFlags.LoadDependents);
					Console.WriteLine("TEST LOAD: Time taken: {0}", (DateTime.Now - dt_s));
				}
				tagindex.WinRar(Path.Combine(kTestResultsTagsPath, @"test.rar"), true);
				Assert.IsTrue(tagindex.Unload(datum_test_index));
			}
		}

		[TestMethod]
		public void Halo1TestTagIndexThreaded()
		{
			var bd = BlamLib.Program.Halo1.Manager;
			
			var thread_matg = new System.Threading.Thread(delegate (/*object param*/)
			{
				DateTime dt_s = DateTime.Now;
				using (var handler = new TagIndexHandler<Managers.TagIndex>(BlamVersion.Halo1_CE, kTestTagIndexTagsPath))
				{
					var tagindex_matg = handler.IndexInterface;

					var datum_test_index = tagindex_matg.Open(
						@"globals\globals", Blam.Halo1.TagGroups.matg,
						IO.ITagStreamFlags.LoadDependents);
					Console.WriteLine("GLOBALS LOAD: Time taken: {0}", (DateTime.Now - dt_s));

				}
			});
			var thread_scnr = new System.Threading.Thread(delegate(/*object param*/)
			{
				DateTime dt_s = DateTime.Now;
				using (var handler = new TagIndexHandler<Managers.TagIndex>(BlamVersion.Halo1_CE, kTestTagIndexTagsPath))
				{
					var tagindex_scnr = handler.IndexInterface;

					var datum_test_index2 = tagindex_scnr.Open(
						@"levels\test\tutorial\tutorial", Blam.Halo1.TagGroups.scnr,
						IO.ITagStreamFlags.LoadDependents);
					Console.WriteLine("SCENARIO LOAD: Time taken: {0}", (DateTime.Now - dt_s));
				}
			});

			thread_matg.Start(/*TestContext*/);
			thread_scnr.Start(/*TestContext*/);
			thread_matg.Join();
			thread_scnr.Join();
		}

		[TestMethod]
		public void Halo1TestTagIndexSharing()
		{
			using (var handler_matg = new TagIndexHandler<Managers.TagIndex>(BlamVersion.Halo1_CE, kTestTagIndexTagsPath))
			using (var handler_scnr = new TagIndexHandler<Managers.TagIndex>(BlamVersion.Halo1_CE, kTestTagIndexTagsPath))
			{
				var tagindex_matg = handler_matg.IndexInterface;
				var tagindex_scnr = handler_scnr.IndexInterface;

				Blam.DatumIndex datum_test_index, datum_test_index2;

				DateTime dt = DateTime.Now;
				{
					DateTime dt_s = DateTime.Now;
					datum_test_index = tagindex_matg.Open(
						@"globals\globals", Blam.Halo1.TagGroups.matg,
						IO.ITagStreamFlags.LoadDependents);
					Console.WriteLine("TEST LOAD: Time taken: {0}", (DateTime.Now - dt_s));
				}
				{
					DateTime dt_s = DateTime.Now;
					datum_test_index2 = tagindex_scnr.Open(
						@"levels\test\tutorial\tutorial", Blam.Halo1.TagGroups.scnr,
						IO.ITagStreamFlags.LoadDependents);
					Console.WriteLine("TEST LOAD 2: Time taken: {0}", (DateTime.Now - dt_s));
				}

				//using (var sw = new StreamWriter(Path.Combine(kTestResultsTagsPath, @"dump_shared_tag_index.txt")))
				//{ tagindex_scnr.DumpSharedReferences(sw, tagindex_matg); }

				tagindex_matg.ToDatabase(Path.Combine(kTestResultsTagsPath, @"_matg.tag_database"));
				tagindex_scnr.ToDatabase(Path.Combine(kTestResultsTagsPath, @"_scnr.tag_database"));

				Assert.IsTrue(tagindex_scnr.Unload(datum_test_index2));
				Assert.IsTrue(tagindex_matg.Unload(datum_test_index));
			}
		}

		[TestMethod]
		public void Halo1TestTagIndexSharingThreaded()
		{
			DateTime g_dt_s = DateTime.Now;

			var bd = BlamLib.Program.Halo1.Manager;

			TagIndexHandler<Managers.TagIndex> handler_matg = null;
			var thread_matg = new System.Threading.Thread(delegate (/*object param*/)
			{
				DateTime dt_s = DateTime.Now;
				handler_matg = new TagIndexHandler<Managers.TagIndex>(BlamVersion.Halo1_CE, kTestTagIndexTagsPath);
				{
					var tagindex_matg = handler_matg.IndexInterface;

					var datum_test_index = tagindex_matg.Open(
						@"globals\globals", Blam.Halo1.TagGroups.matg,
						IO.ITagStreamFlags.LoadDependents);

					tagindex_matg.Open(@"ui\ui_tags_loaded_all_scenario_types", Blam.Halo1.TagGroups.tagc,
						IO.ITagStreamFlags.LoadDependents);

					Console.WriteLine("GLOBALS LOAD: Time taken: {0}", (DateTime.Now - dt_s));
				}
			});

			TagIndexHandler<Managers.TagIndex> handler_scnr = null;
			var thread_scnr = new System.Threading.Thread(delegate(/*object param*/)
			{
				DateTime dt_s = DateTime.Now;
				handler_scnr = new TagIndexHandler<Managers.TagIndex>(BlamVersion.Halo1_CE, kTestTagIndexTagsPath);
				{
					var tagindex_scnr = handler_scnr.IndexInterface;

					var datum_test_index2 = tagindex_scnr.Open(
						@"levels\test\tutorial\tutorial", Blam.Halo1.TagGroups.scnr,
						IO.ITagStreamFlags.LoadDependents);

					tagindex_scnr.Open(@"ui\ui_tags_loaded_multiplayer_scenario_type", Blam.Halo1.TagGroups.tagc,
						IO.ITagStreamFlags.LoadDependents);

					Console.WriteLine("SCENARIO LOAD: Time taken: {0}", (DateTime.Now - dt_s));
				}
			});

			thread_matg.Start(/*TestContext*/);
			thread_scnr.Start(/*TestContext*/);
			thread_matg.Join();
			thread_scnr.Join();

			using (var sw = new StreamWriter(Path.Combine(kTestResultsTagsPath, @"dump_tag_index_matg.txt")))
			{ handler_matg.IndexInterface.Dump(sw); }
			using (var sw = new StreamWriter(Path.Combine(kTestResultsTagsPath, @"dump_tag_index_sncr.txt")))
			{ handler_scnr.IndexInterface.Dump(sw); }
			using (var sw = new StreamWriter(Path.Combine(kTestResultsTagsPath, @"dump_shared_tag_index.txt")))
			{ handler_scnr.IndexInterface.DumpSharedReferences(sw, handler_matg.IndexInterface); }

			handler_matg.Dispose();
			handler_scnr.Dispose();

			Console.WriteLine("TOTAL: Time taken: {0}", (DateTime.Now - g_dt_s));
		}

		[TestMethod]
		public void Halo1TestTagIndexNonTags()
		{
			using (var handler = new TagIndexHandler<Managers.TagIndex>(BlamVersion.Halo1_CE, kTestTagIndexTagsPath))
			{
				var tagindex = handler.IndexInterface;

				using (var sw = new StreamWriter(Path.Combine(kTestResultsTagsPath, @"non_tag_files.txt")))
				{ tagindex.DumpNonTagFiles(sw); }
			}
		}
	};
}