/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using System.IO;
using System.Collections.Generic;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using BlamLib.Bitmaps;

namespace BlamLib.Test
{
	[TestClass]
	public partial class Halo1 : BaseTestClass
	{
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

		[TestMethod]
		public void Halo1TestTagIndex()
		{
			using (var handler = new TagIndexHandler<Managers.TagIndex>(BlamVersion.Halo1_CE, kTestInstallationRootPath, kTestTagsDir))
			{
				var tagindex = handler.IndexInterface;
				Blam.DatumIndex datum_scnr_index, datum_matg_index;

				StartStopwatch();
				{
					StartSubStopwatch();
					datum_scnr_index = tagindex.Open(@"levels\test\tutorial\tutorial", Blam.Halo1.TagGroups.scnr, IO.ITagStreamFlags.LoadDependents);
					Console.WriteLine("SCENARIO LOAD: Time taken: {0}", StopSubStopwatch());
				}
				{
					StartSubStopwatch();
					datum_matg_index = tagindex.Open(@"globals\globals", Blam.Halo1.TagGroups.matg, IO.ITagStreamFlags.LoadDependents);
					Console.WriteLine("GLOBALS LOAD: Time taken: {0}", StopSubStopwatch());
				}
				Console.WriteLine("TAG INDEX: Time taken: {0}", StopStopwatch());

				var tag_manager_scnr = tagindex[datum_scnr_index];
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
			using (var handler = new TagIndexHandler<Managers.TagIndex>(BlamVersion.Halo1_CE, kTestInstallationRootPath, kTestTagsDir))
			{
				var tagindex = handler.IndexInterface;
				Blam.DatumIndex datum_test_index;

				{
					StartStopwatch();
					datum_test_index = tagindex.Open(
						@"characters\cyborg\cyborg", Blam.Halo1.TagGroups.bipd,
						IO.ITagStreamFlags.LoadDependents);
					Console.WriteLine("TEST LOAD: Time taken: {0}", StopStopwatch());
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
				StartStopwatch();
				using (var handler = new TagIndexHandler<Managers.TagIndex>(BlamVersion.Halo1_CE, kTestInstallationRootPath, kTestTagsDir))
				{
					var tagindex_matg = handler.IndexInterface;

					var datum_test_index = tagindex_matg.Open(
						@"globals\globals", Blam.Halo1.TagGroups.matg,
						IO.ITagStreamFlags.LoadDependents);
					Console.WriteLine("GLOBALS LOAD: Time taken: {0}", StopStopwatch());

				}
			});
			var thread_scnr = new System.Threading.Thread(delegate(/*object param*/)
			{
				StartStopwatch();
				using (var handler = new TagIndexHandler<Managers.TagIndex>(BlamVersion.Halo1_CE, kTestInstallationRootPath, kTestTagsDir))
				{
					var tagindex_scnr = handler.IndexInterface;

					var datum_test_index2 = tagindex_scnr.Open(
						@"levels\test\tutorial\tutorial", Blam.Halo1.TagGroups.scnr,
						IO.ITagStreamFlags.LoadDependents);
					Console.WriteLine("SCENARIO LOAD: Time taken: {0}", StopStopwatch());
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
			using (var handler_matg = new TagIndexHandler<Managers.TagIndex>(BlamVersion.Halo1_CE, kTestInstallationRootPath, kTestTagsDir))
			using (var handler_scnr = new TagIndexHandler<Managers.TagIndex>(BlamVersion.Halo1_CE, kTestInstallationRootPath, kTestTagsDir))
			{
				var tagindex_matg = handler_matg.IndexInterface;
				var tagindex_scnr = handler_scnr.IndexInterface;

				Blam.DatumIndex datum_test_index, datum_test_index2;

				{
					StartStopwatch();
					datum_test_index = tagindex_matg.Open(
						@"globals\globals", Blam.Halo1.TagGroups.matg,
						IO.ITagStreamFlags.LoadDependents);
					Console.WriteLine("TEST LOAD: Time taken: {0}", StopStopwatch());
				}
				{
					StartStopwatch();
					datum_test_index2 = tagindex_scnr.Open(
						@"levels\test\tutorial\tutorial", Blam.Halo1.TagGroups.scnr,
						IO.ITagStreamFlags.LoadDependents);
					Console.WriteLine("TEST LOAD 2: Time taken: {0}", StopStopwatch());
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
			StartStopwatch();

			var bd = BlamLib.Program.Halo1.Manager;

			TagIndexHandler<Managers.TagIndex> handler_matg = null;
			var thread_matg = new System.Threading.Thread(delegate (/*object param*/)
			{
				StartSubStopwatch();
				handler_matg = new TagIndexHandler<Managers.TagIndex>(BlamVersion.Halo1_CE, kTestInstallationRootPath, kTestTagsDir);
				{
					var tagindex_matg = handler_matg.IndexInterface;

					var datum_test_index = tagindex_matg.Open(
						@"globals\globals", Blam.Halo1.TagGroups.matg,
						IO.ITagStreamFlags.LoadDependents);

					tagindex_matg.Open(@"ui\ui_tags_loaded_all_scenario_types", Blam.Halo1.TagGroups.tagc,
						IO.ITagStreamFlags.LoadDependents);

					Console.WriteLine("GLOBALS LOAD: Time taken: {0}", StopSubStopwatch());
				}
			});

			TagIndexHandler<Managers.TagIndex> handler_scnr = null;
			var thread_scnr = new System.Threading.Thread(delegate(/*object param*/)
			{
				StartSubStopwatch();
				handler_scnr = new TagIndexHandler<Managers.TagIndex>(BlamVersion.Halo1_CE, kTestInstallationRootPath, kTestTagsDir);
				{
					var tagindex_scnr = handler_scnr.IndexInterface;

					var datum_test_index2 = tagindex_scnr.Open(
						@"levels\test\tutorial\tutorial", Blam.Halo1.TagGroups.scnr,
						IO.ITagStreamFlags.LoadDependents);

					tagindex_scnr.Open(@"ui\ui_tags_loaded_multiplayer_scenario_type", Blam.Halo1.TagGroups.tagc,
						IO.ITagStreamFlags.LoadDependents);

					Console.WriteLine("SCENARIO LOAD: Time taken: {0}", StopSubStopwatch());
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

			Console.WriteLine("TOTAL: Time taken: {0}", StopStopwatch());
		}

		[TestMethod]
		public void Halo1TestTagIndexNonTags()
		{
			using (var handler = new TagIndexHandler<Managers.TagIndex>(BlamVersion.Halo1_CE, kTestInstallationRootPath, kTestTagsDir))
			{
				var tagindex = handler.IndexInterface;

				using (var sw = new StreamWriter(Path.Combine(kTestResultsTagsPath, @"non_tag_files.txt")))
				{ tagindex.DumpNonTagFiles(sw); }
			}
		}

		#region COLLADA tests
		static readonly ModelTestDefinition[] ModelTestDefinitions = new ModelTestDefinition[]
		{
			new ModelTestDefinition("BIPED", @"characters\elite\elite",
				Blam.Halo1.TagGroups.bipd),
			new ModelTestDefinition("BIPED", @"characters\cyborg\cyborg",
				Blam.Halo1.TagGroups.bipd),
			new ModelTestDefinition("VEHICLE", @"vehicles\pelican\pelican",
				Blam.Halo1.TagGroups.vehi),
			new ModelTestDefinition("VEHICLE", @"vehicles\warthog\warthog",
				Blam.Halo1.TagGroups.vehi),
			new ModelTestDefinition("DEVICE MACHINE", @"levels\b30\devices\doors\door small\door small",
				Blam.Halo1.TagGroups.mach),
			new ModelTestDefinition("SCENERY", @"scenery\tubewire\tubewire",
				Blam.Halo1.TagGroups.scen),
			new ModelTestDefinition("DEVICE CONTROL", @"levels\a50\devices\interior tech objects\holo command control\holo command control",
				Blam.Halo1.TagGroups.ctrl),
			new ModelTestDefinition("DEVICE LIGHT FIXTURE", @"levels\c10\devices\ground_bright\ground_bright",
				Blam.Halo1.TagGroups.lifi),
			new ModelTestDefinition("GARBAGE", @"characters\sentinel\sentinel arm bit\sentinel arm bit",
				Blam.Halo1.TagGroups.garb),
			new ModelTestDefinition("WEAPON", @"weapons\sniper rifle\sniper rifle",
				Blam.Halo1.TagGroups.weap),
		};
		static readonly ModelTestDefinition[] BSPTestDefinitions = new ModelTestDefinition[]
		{
			new ModelTestDefinition("BSP", @"levels\test\beavercreek\beavercreek",
				Blam.Halo1.TagGroups.sbsp),
			new ModelTestDefinition("BSP", @"levels\test\longest\longest",
				Blam.Halo1.TagGroups.sbsp),
			new ModelTestDefinition("BSP", @"levels\test\ratrace\ratrace",
				Blam.Halo1.TagGroups.sbsp),
			new ModelTestDefinition("BSP", @"levels\test\deathisland\deathisland",
				Blam.Halo1.TagGroups.sbsp),
			new ModelTestDefinition("BSP", @"levels\a50\a50_exterior",
				Blam.Halo1.TagGroups.sbsp),
			new ModelTestDefinition("BSP", @"levels\b40\b40_b4",
				Blam.Halo1.TagGroups.sbsp),
		};
		static readonly ModelTestDefinition[] ScenarioTestDefinitions = new ModelTestDefinition[]
		{
			new ModelTestDefinition("SCENARIO", @"levels\test\beavercreek\beavercreek",
				Blam.Halo1.TagGroups.scnr),
			new ModelTestDefinition("SCENARIO", @"levels\test\longest\longest",
				Blam.Halo1.TagGroups.scnr),
			new ModelTestDefinition("SCENARIO", @"levels\test\ratrace\ratrace",
				Blam.Halo1.TagGroups.scnr),
			new ModelTestDefinition("SCENARIO", @"levels\test\deathisland\deathisland",
				Blam.Halo1.TagGroups.scnr),
			new ModelTestDefinition("SCENARIO", @"levels\a50\a50",
				Blam.Halo1.TagGroups.scnr),
			new ModelTestDefinition("SCENARIO", @"levels\b40\b40",
				Blam.Halo1.TagGroups.scnr),
		};

		public class TestColladaSettings : BlamLib.Render.COLLADA.IColladaSettings
		{
			public bool Overwrite { get; private set; }
			public string RootDirectory { get; private set; }
			public AssetFormat BitmapFormat { get; private set; }

			public TestColladaSettings(bool overwrite, string root, AssetFormat format)
			{
				Overwrite = overwrite;
				RootDirectory = root;
				BitmapFormat = format;
			}
		}

		[TestMethod]
		public void Halo1TestCOLLADAModelExport()
		{
			var settings = new TestColladaSettings(
				true,
				Path.Combine(kTestInstallationRootPath, kTestDataDir),
				AssetFormat.bmp);

			using (var handler = new TagIndexHandler<Managers.TagIndex>(BlamVersion.Halo1_CE, kTestInstallationRootPath, kTestTagsDir))
			{
				var tagindex = handler.IndexInterface;
				foreach (var model_def in ModelTestDefinitions)
				{
					// load the tag with dependents
					StartStopwatch();
					{
						model_def.Open(tagindex);
						Console.WriteLine("{0} LOAD: Time taken: {1}", model_def.TypeString, m_testStopwatch.Elapsed);
					}
					Console.WriteLine("TAG INDEX: Time taken: {0}", StopStopwatch());
					
					// create a halo1 collada interface with the gbxmodel datum
					var object_tag = tagindex[model_def.TagIndex].TagDefinition as Blam.Halo1.Tags.object_group;
					var tagManager = tagindex[object_tag.Model.Datum];
					string name = Path.GetFileNameWithoutExtension(model_def.Name);

					var modelData = new BlamLib.Render.COLLADA.Halo1.ModelData();
					var modelShaderData = new BlamLib.Render.COLLADA.Halo1.ModelShaderData();
					
					modelData.CollectData(tagindex, tagManager);
					modelShaderData.CollectData(tagindex, tagManager);

					var exporter = new BlamLib.Render.COLLADA.Halo1.ColladaModelExporter(settings,
						tagindex,
						tagManager);

					exporter.MessageSent +=
						(object sender, BlamLib.Messaging.MessageArgs args) =>
						{
							Console.WriteLine("COLLADA_ERROR: {0}", args.Message);
						};

					exporter.AddDataProvider(modelData);
					exporter.AddDataProvider(modelShaderData);

					StartStopwatch();

					Assert.IsTrue(exporter.BuildColladaInstance(), "Failed to build collada instance for {0}", model_def.Name);
					exporter.SaveDAE(Path.Combine(kTestResultsDataPath, modelData.GetRelativeURL()) + ".dae");

					Console.WriteLine("EXPORT {0} TIME: Time taken: {1}", name, StopStopwatch());

					model_def.Close(tagindex);
				}
			}
		}

		[TestMethod]
		public void Halo1TestCOLLADABSPExport()
		{
			var settings = new TestColladaSettings(
				true,
				Path.Combine(kTestInstallationRootPath, kTestDataDir),
				AssetFormat.bmp);

			using (var handler = new TagIndexHandler<Managers.TagIndex>(BlamVersion.Halo1_CE, kTestInstallationRootPath, kTestTagsDir))
			{
				var tagindex = handler.IndexInterface;
				foreach (var model_def in BSPTestDefinitions)
				{
					StartStopwatch();
					{
						model_def.Open(tagindex);
						Console.WriteLine(model_def.TypeString + " LOAD: Time taken: {0}", m_testStopwatch.Elapsed);
					}
					Console.WriteLine("TAG INDEX: Time taken: {0}", StopStopwatch());

					var tagManager = tagindex[model_def.TagIndex];

					var bspData = new BlamLib.Render.COLLADA.Halo1.StructureBSPData();
					var bspShaderData = new BlamLib.Render.COLLADA.Halo1.StructureBSPShaderData();

					bspData.CollectData(tagindex, tagManager);
					bspShaderData.CollectData(tagindex, tagManager);

					var exporter = new BlamLib.Render.COLLADA.Halo1.ColladaBSPExporter(settings,
						tagindex,
						tagManager);

					exporter.MessageSent +=
						(object sender, BlamLib.Messaging.MessageArgs args) =>
						{
							Console.WriteLine("COLLADA_ERROR: {0}", args.Message);
						};

					exporter.AddDataProvider(bspData);
					exporter.AddDataProvider(bspShaderData);

					StartStopwatch();

					Assert.IsTrue(exporter.BuildColladaInstance(), "Failed to build collada instance for {0}", model_def.Name);
					exporter.SaveDAE(Path.Combine(kTestResultsDataPath, tagManager.Name) + ".dae");

					Console.WriteLine("EXPORT {0} TIME: Time taken: {1}", model_def.Name, StopStopwatch());

					model_def.Close(tagindex);
				}
			}
		}

		[TestMethod]
		public void Halo1TestCOLLADAScenarioExport()
		{
			var settings = new TestColladaSettings(
				true,
				Path.Combine(kTestInstallationRootPath, kTestDataDir),
				AssetFormat.bmp);

			using (var handler = new TagIndexHandler<Managers.TagIndex>(BlamVersion.Halo1_CE, kTestInstallationRootPath, kTestDataDir))
			{
				var tagindex = handler.IndexInterface;
				foreach (var scenario_def in ScenarioTestDefinitions)
				{
					StartStopwatch();
					{
						scenario_def.Open(tagindex);
						Console.WriteLine(scenario_def.TypeString + " LOAD: Time taken: {0}", m_testStopwatch.Elapsed);
					}
					Console.WriteLine("TAG INDEX: Time taken: {0}", StopStopwatch());

					var tagManager = tagindex[scenario_def.TagIndex];

					var scenarioData = new BlamLib.Render.COLLADA.Halo1.ScenarioData();

					scenarioData.CollectData(tagindex, tagManager);

					var exporter = new BlamLib.Render.COLLADA.Halo1.ColladaScenarioExporter(settings,
						tagindex,
						tagManager);

					exporter.MessageSent +=
						(object sender, BlamLib.Messaging.MessageArgs args) =>
						{
							Console.WriteLine("COLLADA_ERROR: {0}", args.Message);
						};

					exporter.AddDataProvider(scenarioData);

					StartStopwatch();

					Assert.IsTrue(exporter.BuildColladaInstance(), "Failed to build collada instance for {0}", scenario_def.Name);
					exporter.SaveDAE(Path.Combine(kTestResultsDataPath, tagManager.Name) + "-objects.dae");

					Console.WriteLine("EXPORT {0} TIME: Time taken: {1}", scenario_def.Name, StopStopwatch());

					scenario_def.Close(tagindex);
				}
			}
		}

		[TestMethod]
		public void Halo1TestCOLLADAModelImport()
		{
			// this isn't importing collada files to gbxmodels or anything, just loading the collada file and validating it
			// so that we know the exported file can be imported without error

			var serializer = new System.Xml.Serialization.XmlSerializer(typeof(Render.COLLADA.ColladaFile));

			foreach (var model_def in ModelTestDefinitions)
			{
				var file_path = Path.Combine(Path.Combine(kTestInstallationRootPath, kTestDataDir));
				file_path = Path.Combine(file_path, model_def.Name);
				file_path = Path.ChangeExtension(file_path, "dae");
				
				using (var reader = new System.Xml.XmlTextReader(file_path))
				{
					var collada_file = serializer.Deserialize(reader) as Render.COLLADA.ColladaFile;

					var validator = new BlamLib.Render.COLLADA.Validation.ColladaFileValidator();

					validator.ErrorOccured += new EventHandler<BlamLib.Render.COLLADA.ColladaErrorEventArgs>(ValidatorErrorOccured);
					bool success = validator.ValidateFile(collada_file);
					validator.ErrorOccured -= new EventHandler<BlamLib.Render.COLLADA.ColladaErrorEventArgs>(ValidatorErrorOccured);

					if (!success)
						Assert.Fail("COLLADA file failed validation");
				}
			}
		}

		void ValidatorErrorOccured(object sender, BlamLib.Render.COLLADA.ColladaErrorEventArgs e)
		{
			Console.WriteLine(e.ErrorMessage);
		}
		#endregion
	};
}