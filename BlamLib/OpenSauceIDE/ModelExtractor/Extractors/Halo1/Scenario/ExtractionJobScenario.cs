/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.IO;
using BlamLib.Managers;
using BlamLib.Render.COLLADA;
using BlamLib.Render.COLLADA.Halo1;
using OpenSauceIDE.ModelExtractor.TagIO;

namespace OpenSauceIDE.ModelExtractor.Extractors.Halo1.Scenario
{
	/// <summary>   An structure bsp extraction jobs data. </summary>
	public class ExtractionJobScenarioData
		: ICloneable
	{
		public bool ExportScenery;
		public bool ExportDeviceMachines;
		public bool ExportControls;
		public bool ExportLightFixtures;
		public bool ExportSoundScenery;

		public bool ExportObjectMeshes;

		public object Clone()
		{
			return MemberwiseClone();
		}
	}

	/// <summary>   An scenario extraction job. </summary>
	public class ExtractionJobScenario
		: Halo1ExtractorJob
		, IExtractionJob
	{
		private IExtractionData mExtractionData;
		private ColladaSettings mColladaSettings;
		private ModelExtractionData mModelExtractionData;
		private ExtractionJobScenarioData mScenarioData;

		public string JobName
		{
			get { return Path.GetFileName(mModelExtractionData.TagFile.AbsoluteTagPath); }
		}

		public ExtractionStateEnum JobState
		{
			get { return ExtractionStateEnum.Queued; }
		}

		public string JobResultMessage
		{
			get { return "Success"; }
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Constructor. </summary>
		///
		/// <param name="extractionData">   Data for the extraction. </param>
		public ExtractionJobScenario(IExtractionData extractionData)
		{
			mExtractionData = extractionData;
			mColladaSettings = extractionData.Get<ColladaSettings>();
			mModelExtractionData = extractionData.Get<ModelExtractionData>();
			mScenarioData = extractionData.Get<ExtractionJobScenarioData>();
		}

		#region Data Collection
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Gets the scenario data from the tag. </summary>
		///
		/// <param name="tagIndex">     Zero-based index of the tag. </param>
		/// <param name="tagManager">   The tag's manager. </param>
		///
		/// <returns>   The scenario data. </returns>
		private ScenarioData GetScenarioData(TagIndexBase tagIndex, BlamLib.Managers.TagManager tagManager)
		{
			var data = new ScenarioData();

			data.CollectData(tagIndex, tagManager);

			return data;
		}
		#endregion

		#region Child Jobs
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds the scenarios objects as child jobs. </summary>
		///
		/// <param name="scenarioData">	Information describing the scenario. </param>
		private void AddChildJobs(ScenarioData scenarioData)
		{
			foreach(var objectTypeList in scenarioData.ScenarioObjectLists)
			{
				foreach(var objectType in objectTypeList.Value)
				{
					// Get the model tag
					if(!BlamLib.Managers.TagIndex.IsValid(objectType.ObjectTagDatum))
					{
						mMessageHandler.SendMessage("Failed to open {0}, child job not added", Path.GetFileName(objectType.ObjectTagPath));
						continue;
					}

					var objectData = new ObjectData();
					objectData.CollectData(TagIndex, TagIndex[objectType.ObjectTagDatum]);

					if (!BlamLib.Managers.TagIndex.IsValid(objectData.Model))
					{
						mMessageHandler.SendMessage("Failed to open model tag for {0}, child job not added", Path.GetFileName(objectType.ObjectTagPath));
						continue;
					}

					// Add an extraction job for the model tag
					var modelTagManager = TagIndex[objectData.Model];
					var modelFile = Path.Combine(mModelExtractionData.TagsDirectory.Root
						, mModelExtractionData.TagsDirectory.EndFolder
						, modelTagManager.Name);
					modelFile = Path.ChangeExtension(modelFile, "gbxmodel");

					mModelExtractionData.JobInterface.AddJob(modelFile, mExtractionData);
				}
			}
		}
		#endregion

		public void Export(ScenarioData scenarioData
			, BlamLib.Managers.TagManager tagManager
			, string outputFile)
		{
			// Create the COLLADA exporter
			var extractor = new ColladaScenarioExporter(mColladaSettings,
				TagIndex,
				tagManager);

			extractor.MessageSent +=
				(sender, args) =>
				{
					mMessageHandler.SendMessage("Exporter Error: {0}", args.Message);
				};

			extractor.AddDataProvider(scenarioData);

			if (extractor.BuildColladaInstance())
			{
				extractor.SaveDAE(outputFile);
				mMessageHandler.SendMessage("Extraction complete! Tag {0}", Path.GetFileName(outputFile));
			}
			else
			{
				mMessageHandler.SendMessage("Failed to build COLLADA file for tag {0}", Path.GetFileName(outputFile));
			}
		}

		/// <summary>   Executes the extraction job. </summary>
		public void Execute()
		{
			// Create the tag index
			if (!CreateTagIndex(mModelExtractionData.TagsDirectory.Root, mModelExtractionData.TagsDirectory.AbsoluteFolder))
			{
				mMessageHandler.SendMessage("Failed to create a tag index for {0}\\{1}", mModelExtractionData.TagsDirectory.Root, mModelExtractionData.TagsDirectory.AbsoluteFolder);
				return;
			}

			var tagPath = mModelExtractionData.TagFile.TagPath;

			// Open the tag
			var tagManager = OpenTag(tagPath, BlamLib.Blam.Halo1.TagGroups.scnr);
			if (tagManager == null)
			{
				mMessageHandler.SendMessage("Failed to open tag {0}", tagPath);
				DestroyTagIndex();
				return;
			}

			try
			{
				// Add the tags data sources
				var scenarioData = GetScenarioData(TagIndex, tagManager);

				if (mScenarioData.ExportObjectMeshes)
				{
					AddChildJobs(scenarioData);
				}

				string filename = Path.GetFileNameWithoutExtension(mModelExtractionData.OutputFile);
				string outputFile = Path.GetDirectoryName(mModelExtractionData.OutputFile);
				outputFile = Path.Combine(outputFile, filename + "-{0}");
				outputFile = Path.ChangeExtension(outputFile, Path.GetExtension(mModelExtractionData.OutputFile));

				if (mScenarioData.ExportScenery)
				{
					scenarioData.IncludeScenery = true;
					Export(scenarioData, tagManager, String.Format(outputFile, "scenery"));
					scenarioData.IncludeScenery = false;
				}

				if (mScenarioData.ExportDeviceMachines)
				{
					scenarioData.IncludeDeviceMachines = true;
					Export(scenarioData, tagManager, String.Format(outputFile, "device_machines"));
					scenarioData.IncludeDeviceMachines = false;
				}

				if (mScenarioData.ExportControls)
				{
					scenarioData.IncludeDeviceControls = true;
					Export(scenarioData, tagManager, String.Format(outputFile, "device_controls"));
					scenarioData.IncludeDeviceControls = false;
				}

				if (mScenarioData.ExportLightFixtures)
				{
					scenarioData.IncludeDeviceLightFixtures = true;
					Export(scenarioData, tagManager, String.Format(outputFile, "device_light_fixtures"));
					scenarioData.IncludeDeviceLightFixtures = false;
				}

				if (mScenarioData.ExportSoundScenery)
				{
					scenarioData.IncludeSoundScenery = true;
					Export(scenarioData, tagManager, String.Format(outputFile, "sound_scenery"));
					scenarioData.IncludeSoundScenery = false;
				}
			}
			catch (ColladaException e)
			{
				mMessageHandler.SendMessage("Exporter Error: {0}", e.Message);
			}

			DestroyTagIndex();
		}
	}
}
