/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿using System;
using System.IO;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System.Collections.Generic;

namespace BlamLib.Test
{
	[TestClass]
	public partial class Halo2 : BaseTestClass
	{
		static string EngineGetTestResultsPath(BlamVersion engine)
		{
			switch (engine)
			{
				case BlamVersion.Halo2_Xbox:	return kTestResultsPathXbox;
				case BlamVersion.Halo2_Alpha:	return kTestResultsPathXboxAlpha;
				case BlamVersion.Halo2_PC:		return kTestResultsPathPc;

				default: return kTestResultsPath;
			}
		}

		[ClassInitialize]
		public static void Initialize(TestContext context)
		{
			Directory.CreateDirectory(kTestResultsPath);
			Directory.CreateDirectory(kTestResultsPathXbox);
			Directory.CreateDirectory(kTestResultsPathXboxAlpha);
			Directory.CreateDirectory(kTestResultsPathPc);
		}
		[ClassCleanup]
		public static void Dispose()
		{
		}

		static readonly string[] kMapNames = {
			"mainmenu.map",
			"shared.map",
			"single_player_shared.map",
			"00a_introduction.map",
			"01a_tutorial.map",
			"01b_spacestation.map",
			"03a_oldmombasa.map",
			"03b_newmombasa.map",
			"04a_gasgiant.map",
			"04b_floodlab.map",
			"05a_deltaapproach.map",
			"05b_deltatowers.map",
			"06a_sentinelwalls.map",
			"06b_floodzone.map",
			"07a_highcharity.map",
			"07b_forerunnership.map",
			"08a_deltacliffs.map",
			"08b_deltacontrol.map",
			"ascension.map",
			"backwash.map",
			"burial_mounds.map",
			"coagulation.map",
			"colossus.map",
			"containment.map",
			"cyclotron.map",
			"deltatap.map",
			"dune.map",
			"elongation.map",
			"foundation.map",
			"gemini.map",
			"headlong.map",
			"lockout.map",
			"midship.map",
			"triplicate.map",
			"turf.map",
			"warlock.map",
			"waterworks.map",
			"zanzibar.map",
		};
		static readonly string[] kMapNamesXbox = {
			"derelict.map",
			"highplains.map",
		};
		static readonly string[] kMapNamesPc = {
			"beavercreek.map", // I thought Xbox had this too?
			"example.map",
			"example_sans_sharing.map",
			"forerunner_sample.map",
			"human_sample.map",
			"needle.map",
			"street_sweeper.map",
		};
		static readonly string[] kMapNamesXboxAlpha = {
			"mainmenu.map",
			"shared.map",
			"cyclotron.map",
			"midship.map",
			"waterworks.map",
			"zanzibar.map",
			//C:\Mount\B\Bungie\Halo2\Xbox\Alpha\test\4d53008200000000\lockout.map

			// Epsilon
			//C:\Mount\B\Bungie\Halo2\Xbox\Alpha\test\4d53008200000778\box_multiplayer.map
		};

		#region CacheOutputInformation
		static void CacheOutputInformationMethod(object param)
		{
			var args = param as CacheFileOutputInfoArgs;

			using (var handler = new CacheHandler<Blam.Halo2.CacheFile>(args.Game, args.MapPath))
			{
				handler.Read();
				var cache = handler.CacheInterface;

				string test_results_path = EngineGetTestResultsPath(args.Game);

				Blam.CacheFile.OutputStringIds(cache,
					BuildResultPath(test_results_path, args.Game, cache.Header.Name, "string_ids", "txt"), true);
				Blam.CacheFile.OutputTags(cache,
					BuildResultPath(test_results_path, args.Game, cache.Header.Name, null, "txt"));
			}

			args.SignalFinished();
		}
		[TestMethod]
		public void Halo2TestCacheOutputXbox()
		{
			(Program.GetManager(BlamVersion.Halo2_Xbox) as Managers.IStringIdController)
				.StringIdCacheOpen(BlamVersion.Halo2_Xbox);

			CacheFileOutputInfoArgs.TestThreadedMethod(TestContext,
				CacheOutputInformationMethod,
				BlamVersion.Halo2_Xbox, kMapsDirectoryXbox, kMapNames);
			CacheFileOutputInfoArgs.TestThreadedMethod(TestContext,
				CacheOutputInformationMethod,
				BlamVersion.Halo2_Xbox, kMapsDirectoryXbox, kMapNamesXbox);

			(Program.GetManager(BlamVersion.Halo2_Xbox) as Managers.IStringIdController)
				.StringIdCacheClose(BlamVersion.Halo2_Xbox);
		}
		[TestMethod]
		public void Halo2TestCacheOutputPc()
		{
			(Program.GetManager(BlamVersion.Halo2_PC) as Managers.IStringIdController)
				.StringIdCacheOpen(BlamVersion.Halo2_PC);

			CacheFileOutputInfoArgs.TestThreadedMethod(TestContext,
				CacheOutputInformationMethod,
				BlamVersion.Halo2_PC, kMapsDirectoryPc, kMapNames);
			CacheFileOutputInfoArgs.TestThreadedMethod(TestContext,
				CacheOutputInformationMethod,
				BlamVersion.Halo2_PC, kMapsDirectoryPc, kMapNamesPc);

			(Program.GetManager(BlamVersion.Halo2_PC) as Managers.IStringIdController)
				.StringIdCacheClose(BlamVersion.Halo2_PC);
		}
		[TestMethod]
		public void Halo2TestCacheOutputXboxAlpha()
		{
			(Program.GetManager(BlamVersion.Halo2_Alpha) as Managers.IStringIdController)
				.StringIdCacheOpen(BlamVersion.Halo2_Alpha);

			CacheFileOutputInfoArgs.TestThreadedMethod(TestContext,
				CacheOutputInformationMethod,
				BlamVersion.Halo2_Alpha, kMapsDirectoryXboxAlpha, kMapNamesXboxAlpha);

			(Program.GetManager(BlamVersion.Halo2_Alpha) as Managers.IStringIdController)
				.StringIdCacheClose(BlamVersion.Halo2_Alpha);
		}
		#endregion

		#region COLLADA tests
		static readonly ModelTestDefinition[] LightmapTestDefinitions = new ModelTestDefinition[]
		{
			new ModelTestDefinition("LTMP", @"scenarios\multi\example\example_example_lightmap",
			    Blam.Halo2.TagGroups.ltmp)
		};
		static readonly ModelTestDefinition[] BSPTestDefinitions = new ModelTestDefinition[]
		{
			new ModelTestDefinition("SBSP", @"scenarios\multi\example\example",
			    Blam.Halo2.TagGroups.sbsp),
			new ModelTestDefinition("SBSP", @"scenarios\solo\03a_oldmombasa\earthcity_3b",
			    Blam.Halo2.TagGroups.sbsp),
			new ModelTestDefinition("SBSP", @"scenarios\solo\03b_newmombasa\earthcity_4",
			    Blam.Halo2.TagGroups.sbsp),
			new ModelTestDefinition("SBSP", @"scenarios\solo\07a_highcharity\high_0",
			    Blam.Halo2.TagGroups.sbsp),
			new ModelTestDefinition("SBSP", @"scenarios\multi\backwash\backwash",
			    Blam.Halo2.TagGroups.sbsp)
		};
		static readonly ModelTestDefinition[] RenderModelTestDefinitions = new ModelTestDefinition[]
		{
			new ModelTestDefinition("MODE", @"objects\characters\elite\elite",
			    Blam.Halo2.TagGroups.mode),
			new ModelTestDefinition("MODE", @"objects\characters\masterchief\masterchief",
			    Blam.Halo2.TagGroups.mode),
			new ModelTestDefinition("MODE", @"objects\vehicles\wraith\wraith",
			    Blam.Halo2.TagGroups.mode),
			new ModelTestDefinition("MODE", @"objects\characters\elite\fp_body\fp_body",
			    Blam.Halo2.TagGroups.mode),
			new ModelTestDefinition("MODE", @"scenarios\skies\multi\halo\coagulation\coagulation",
			    Blam.Halo2.TagGroups.mode),
			new ModelTestDefinition("MODE", @"objects\vehicles\wraith\turrets\minigun\minigun",
			    Blam.Halo2.TagGroups.mode),
			new ModelTestDefinition("MODE", @"objects\vehicles\warthog\warthog",
			    Blam.Halo2.TagGroups.mode),
			new ModelTestDefinition("MODE", @"objects\vehicles\warthog\turrets\gauss\gauss",
			    Blam.Halo2.TagGroups.mode),
			new ModelTestDefinition("MODE", @"objects\characters\masterchief\fp\fp",
			    Blam.Halo2.TagGroups.mode),
			new ModelTestDefinition("MODE", @"objects\characters\elite\fp_arms\fp_arms",
			    Blam.Halo2.TagGroups.mode),
		};

		//[TestMethod]
		//public void Halo2TestCOLLADALightmapExport()
		//{
		//	using (var handler = new TagIndexHandler<Managers.TagIndex>(BlamVersion.Halo2_PC, kTestInstallationRootPath, kTestTagsDir))
		//	{
		//		var tagindex = handler.IndexInterface;
		//		foreach (var model_def in LightmapTestDefinitions)
		//		{
		//			StartStopwatch();
		//			{
		//				model_def.Open(tagindex);
		//				Console.WriteLine("{0} LOAD: Time taken: {1}", model_def.TypeString, m_testStopwatch.Elapsed);
		//			}
		//			Console.WriteLine("TAG INDEX: Time taken: {0}", StopStopwatch());

		//			var halo2 = new BlamLib.Render.COLLADA.Halo2.ColladaHalo2(tagindex, model_def.TagIndex);

		//			halo2.Overwrite = true;
		//			halo2.RelativeFilePath = Path.Combine(kTestInstallationRootPath, kTestDataDir);

		//			halo2.ClearRegister();

		//			foreach (var info in halo2)
		//				halo2.RegisterForExport(info);

		//			var lightmap_info = halo2[0] as Render.COLLADA.Halo2.ColladaHalo2LightmapInfo;

		//			StartStopwatch();
		//			halo2.Export(lightmap_info.Name);
		//			Console.WriteLine("EXPORT {0} TIME: Time taken: {1}", lightmap_info.Name, StopStopwatch());

		//			foreach (string report in halo2.Reports())
		//				Console.WriteLine(report);

		//			model_def.Close(tagindex);
		//		}
		//	}
		//}
		//[TestMethod]
		//public void Halo2TestCOLLADABSPExport()
		//{
		//	using (var handler = new TagIndexHandler<Managers.TagIndex>(BlamVersion.Halo2_PC, kTestInstallationRootPath, kTestTagsDir))
		//	{
		//		var tagindex = handler.IndexInterface;
		//		foreach (var model_def in BSPTestDefinitions)
		//		{
		//			StartStopwatch();
		//			{
		//				model_def.Open(tagindex);
		//				Console.WriteLine("{0} LOAD: Time taken: {1}", model_def.TypeString, m_testStopwatch.Elapsed);
		//			}
		//			Console.WriteLine("TAG INDEX: Time taken: {0}", StopStopwatch());

		//			var halo2 = new BlamLib.Render.COLLADA.Halo2.ColladaHalo2(tagindex, model_def.TagIndex);

		//			halo2.Overwrite = true;
		//			halo2.RelativeFilePath = Path.Combine(kTestInstallationRootPath, kTestDataDir);

		//			halo2.ClearRegister();

		//			foreach (var info in halo2)
		//				halo2.RegisterForExport(info);

		//			var bsp_info = halo2[0] as Render.COLLADA.Halo2.ColladaHalo2BSPInfo;

		//			StartStopwatch();
		//			halo2.Export(bsp_info.Name);
		//			Console.WriteLine("EXPORT {0} TIME: Time taken: {1}", bsp_info.Name, StopStopwatch());

		//			foreach (string report in halo2.Reports())
		//				Console.WriteLine(report);

		//			model_def.Close(tagindex);
		//		}
		//	}
		//}
		//[TestMethod]
		//public void Halo2TestCOLLADARenderModelExport()
		//{
		//	using (var handler = new TagIndexHandler<Managers.TagIndex>(BlamVersion.Halo2_PC, kTestInstallationRootPath, kTestTagsDir))
		//	{
		//		var tagindex = handler.IndexInterface;
		//		foreach (var model_def in RenderModelTestDefinitions)
		//		{
		//			StartStopwatch();
		//			{
		//				model_def.Open(tagindex);
		//				Console.WriteLine("{0} LOAD: Time taken: {1}", model_def.TypeString, m_testStopwatch.Elapsed);
		//			}
		//			Console.WriteLine("TAG INDEX: Time taken: {0}", StopStopwatch());

		//			var halo2 = new BlamLib.Render.COLLADA.Halo2.ColladaHalo2(tagindex, model_def.TagIndex);

		//			halo2.Overwrite = true;
		//			halo2.RelativeFilePath = Path.Combine(kTestInstallationRootPath, kTestDataDir);

		//			foreach (Render.COLLADA.Halo2.ColladaHalo2RenderModelInfo info in halo2)
		//			{
		//				halo2.ClearRegister();
		//				halo2.RegisterForExport(info);

		//				string name = string.Format("{0}-perm{1}-lod{2}", info.Name, info.Permutation, info.LevelOfDetail);

		//				StartStopwatch();
		//				halo2.Export(name);
		//				Console.WriteLine("EXPORT {0} TIME: Time taken: {1}", name, StopStopwatch());

		//				foreach (string report in halo2.Reports())
		//					Console.WriteLine(report);
		//			}

		//			halo2.ClearRegister();
					
		//			foreach (var info in halo2)
		//				halo2.RegisterForExport(info);

		//			var model_info = halo2[0] as Render.COLLADA.Halo2.ColladaHalo2RenderModelInfo;

		//			string name_all = model_info.Name + "_all";

		//			StartStopwatch();
		//			halo2.Export(name_all);
		//			Console.WriteLine("EXPORT {0} TIME: Time taken: {1}", name_all, StopStopwatch());

		//			foreach (string report in halo2.Reports())
		//				Console.WriteLine(report);

		//			model_def.Close(tagindex);
		//		}
		//	}
		//}
		#endregion
	};
}