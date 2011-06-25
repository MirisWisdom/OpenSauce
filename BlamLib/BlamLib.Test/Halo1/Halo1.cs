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
using System.Collections.Generic;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace BlamLib.Test
{
	[TestClass]
	public partial class Halo1 : BaseTestClass
	{
		private struct ModelTestDefinition
		{
			public string TypeString;
			public string Tag;
			public TagInterface.TagGroup Group;

			public ModelTestDefinition(string type, string tag, TagInterface.TagGroup group)
			{
				TypeString = type;
				Tag = tag;
				Group = group;
			}
		}

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

		static readonly ModelTestDefinition[] ModelTestDefinitions = new ModelTestDefinition[]
		{
			new ModelTestDefinition("BIPED", @"tags\characters\elite\elite",
			    Blam.Halo1.TagGroups.bipd),
			new ModelTestDefinition("BIPED", @"tags\characters\cyborg\cyborg",
			    Blam.Halo1.TagGroups.bipd),
			new ModelTestDefinition("VEHICLE", @"tags\vehicles\pelican\pelican",
			    Blam.Halo1.TagGroups.vehi),
			new ModelTestDefinition("VEHICLE", @"tags\vehicles\warthog\warthog",
			    Blam.Halo1.TagGroups.vehi),
			new ModelTestDefinition("DEVICE MACHINE", @"tags\levels\b30\devices\doors\door small\door small",
			    Blam.Halo1.TagGroups.mach),
			new ModelTestDefinition("SCENERY", @"tags\scenery\tubewire\tubewire",
			    Blam.Halo1.TagGroups.scen),
			new ModelTestDefinition("DEVICE CONTROL", @"tags\levels\a50\devices\interior tech objects\holo command control\holo command control",
			    Blam.Halo1.TagGroups.ctrl),
			new ModelTestDefinition("DEVICE LIGHT FIXTURE", @"tags\levels\c10\devices\ground_bright\ground_bright",
			    Blam.Halo1.TagGroups.lifi),
			new ModelTestDefinition("GARBAGE", @"tags\characters\sentinel\sentinel arm bit\sentinel arm bit",
			    Blam.Halo1.TagGroups.garb),
			new ModelTestDefinition("WEAPON", @"tags\weapons\sniper rifle\sniper rifle",
			    Blam.Halo1.TagGroups.weap),
		};
		static readonly ModelTestDefinition[] BSPTestDefinitions = new ModelTestDefinition[]
		{
			new ModelTestDefinition("BSP", @"tags\levels\test\ratrace\ratrace",
			    Blam.Halo1.TagGroups.sbsp),
			new ModelTestDefinition("BSP", @"tags\levels\test\deathisland\deathisland",
			    Blam.Halo1.TagGroups.sbsp),
			new ModelTestDefinition("BSP", @"tags\levels\a50\a50_exterior",
				Blam.Halo1.TagGroups.sbsp),
			new ModelTestDefinition("BSP", @"tags\levels\b40\b40_b4",
				Blam.Halo1.TagGroups.sbsp),
		};

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

		[TestMethod]
		public void Halo1TestCOLLADAModelExport()
		{
			using (var handler = new TagIndexHandler<Managers.TagIndex>(BlamVersion.Halo1_CE, kTestTagIndexTagsPath))
			{
				string data_path = kTestTagIndexTagsPath;
				if (!data_path.EndsWith("\\"))
					data_path += "\\";

				var tagindex = handler.IndexInterface;
				foreach (var model_def in ModelTestDefinitions)
				{
					Blam.DatumIndex datum_index;

					// load the tag with dependents
					DateTime dt = DateTime.Now;
					{
						DateTime dt_s = DateTime.Now;
						datum_index = tagindex.Open(kTestTagIndexTagsPath + model_def.Tag, model_def.Group, IO.ITagStreamFlags.LoadDependents);
						Console.WriteLine(model_def.TypeString + " LOAD: Time taken: {0}", (DateTime.Now - dt_s));
					}
					Console.WriteLine("TAG INDEX: Time taken: {0}", (DateTime.Now - dt));

					// create a halo1 collada interface with the gbxmodel datum
					Blam.Halo1.Tags.object_group object_tag = tagindex[datum_index].TagDefinition as Blam.Halo1.Tags.object_group;

					Render.COLLADA.Halo1.ColladaHalo1 halo1 = new BlamLib.Render.COLLADA.Halo1.ColladaHalo1(tagindex, object_tag.Model.Datum);

					// set the interfaces settings (want to look into a better way of doing this)
					halo1.Overwrite = true;
					halo1.RelativeFilePath = data_path + @"data\";
					halo1.BitmapFormat = "tif";

					string report = null;
					// export each permutation and lod seperately
					foreach (var info in halo1)
					{
						// clear the export register
						halo1.ClearRegister();
						// add the info object to export
						halo1.RegisterForExport(info);

						Render.COLLADA.Halo1.ColladaHalo1ModelInfo model_info = info as Render.COLLADA.Halo1.ColladaHalo1ModelInfo;

						// export to file
						halo1.Export(model_info.Name + "_perm" + model_info.Permutation.ToString() + "_lod" + model_info.LevelOfDetail.ToString());

						// print any errors
						while ((report = halo1.GetReport()) != null)
							Console.WriteLine(report);
					}
					// export all the models into a single collada file
					halo1.ClearRegister();
					foreach (var info in halo1)
						halo1.RegisterForExport(info);

					halo1.Export(Path.GetFileNameWithoutExtension(model_def.Tag) +"_all");

					while ((report = halo1.GetReport()) != null)
						Console.WriteLine(report);
					
					Assert.IsTrue(tagindex.Unload(datum_index));
				}
			}
		}

		[TestMethod]
		public void Halo1TestCOLLADABSPExport()
		{
			using (var handler = new TagIndexHandler<Managers.TagIndex>(BlamVersion.Halo1_CE, kTestTagIndexTagsPath))
			{
				string data_path = kTestTagIndexTagsPath;
				if (!data_path.EndsWith("\\"))
					data_path += "\\";

				var tagindex = handler.IndexInterface;
				foreach (var model_def in BSPTestDefinitions)
				{
					Blam.DatumIndex datum_index;

					DateTime dt = DateTime.Now;
					{
						DateTime dt_s = DateTime.Now;
						datum_index = tagindex.Open(kTestTagIndexTagsPath + model_def.Tag, model_def.Group, IO.ITagStreamFlags.LoadDependents);
						Console.WriteLine(model_def.TypeString + " LOAD: Time taken: {0}", (DateTime.Now - dt_s));
					}
					Console.WriteLine("TAG INDEX: Time taken: {0}", (DateTime.Now - dt));

					Render.COLLADA.Halo1.ColladaHalo1 halo1 = new BlamLib.Render.COLLADA.Halo1.ColladaHalo1(tagindex, datum_index);

					halo1.Overwrite = true;
					halo1.RelativeFilePath = data_path + @"data\";
					halo1.BitmapFormat = "tif";

					halo1.ClearRegister();

					foreach (var info in halo1)
						halo1.RegisterForExport(info);

					Render.COLLADA.Halo1.ColladaHalo1BSPInfo bsp_info = halo1[0] as Render.COLLADA.Halo1.ColladaHalo1BSPInfo;

					halo1.Export(bsp_info.Name);

					string report = null;
					while ((report = halo1.GetReport()) != null)
						Console.WriteLine(report);

					Assert.IsTrue(tagindex.Unload(datum_index));
				}
			}
		}

		[TestMethod]
		public void Halo1TestCOLLADAModelImport()
		{
			// this isn't importing collada files to gbxmodels or anything, just loading the collada file and validating it
			// so that we know the exported file can be imported without error

			string data_path = kTestTagIndexTagsPath;
			if (!data_path.EndsWith("\\"))
				data_path += "\\";

			System.Xml.Serialization.XmlSerializer serializer = new System.Xml.Serialization.XmlSerializer(typeof(Render.COLLADA.ColladaFile));

			foreach (var model_def in ModelTestDefinitions)
			{
				System.Xml.XmlTextReader reader = new System.Xml.XmlTextReader(data_path + @"data\" + Path.GetFileNameWithoutExtension(model_def.Tag) + "_all.dae");
				Render.COLLADA.ColladaFile collada_file = (Render.COLLADA.ColladaFile)serializer.Deserialize(reader);

				try
				{
					collada_file.Validate();
				}
				catch (Exception exception)
				{
					Console.WriteLine("COLLADA: validation failed");
					Exception exception_temp = exception;
					while (exception != null)
					{
						Console.WriteLine(exception.Message);

						Render.COLLADA.ColladaValidationException validation_exception = (exception as Render.COLLADA.ColladaValidationException);
						if ((validation_exception != null) && (validation_exception.ElementDetails != null))
						{
							foreach (string detail in validation_exception.ElementDetails)
								Console.WriteLine(detail);
						}

						exception = exception.InnerException;
					}
					Assert.Fail("COLLADA file failed validation");
				}

				collada_file = null;
				reader.Close();
			}
		}
	};
}