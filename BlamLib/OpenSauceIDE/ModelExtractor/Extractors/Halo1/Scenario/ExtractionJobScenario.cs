/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.ComponentModel;
using System.IO;
using BlamLib.Blam.Halo1;
using BlamLib.Managers;
using BlamLib.Render.COLLADA;
using BlamLib.Render.COLLADA.Halo1;

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
		private IColladaSettings mColladaSettings;
		private ModelExtractionData mModelExtractionData;
		private ExtractionJobScenarioData mScenarioData;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Constructor. </summary>
		///
		/// <param name="extractionData">   Data for the extraction. </param>
		public ExtractionJobScenario(IExtractionData extractionData)
		{
			mExtractionData = extractionData;
			mColladaSettings = extractionData.Get<IColladaSettings>();
			mModelExtractionData = extractionData.Get<ModelExtractionData>();
			mScenarioData = extractionData.Get<ExtractionJobScenarioData>();

			JobID = mModelExtractionData.TagFile.AbsoluteTagPath;
			JobName = mModelExtractionData.TagFile.TagNameWithExtension;
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

			data.MessageSent += MessageRedirect;

			data.CollectData(tagIndex, tagManager);

			data.MessageSent -= MessageRedirect;

			return data;
		}
		#endregion

		#region Child Jobs
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds the scenarios objects as child jobs. </summary>
		///
		/// <param name="scenarioData">	Information describing the scenario. </param>
		/// <param name="objectType">  	Type of the object. </param>
		private void AddChildJobs(ScenarioData scenarioData, TypeEnums.ObjectType objectType)
		{
			foreach (var scenarioObjectType in scenarioData.ScenarioObjectLists[objectType])
			{
				// Get the model tag
				if (!BlamLib.Managers.TagIndex.IsValid(scenarioObjectType.ObjectTagDatum))
				{
					mMessageHandler.SendMessage("Failed to open {0}, child job not added", Path.GetFileName(scenarioObjectType.ObjectTagPath));
					continue;
				}

				var objectData = new ObjectData();
				objectData.CollectData(TagIndex, TagIndex[scenarioObjectType.ObjectTagDatum]);

				// If no model was set don't continue
				if (objectData.Model.IsNull)
				{
					continue;
				}

				if (!BlamLib.Managers.TagIndex.IsValid(objectData.Model))
				{
					mMessageHandler.SendMessage("Failed to open model tag for {0}, child job not added", Path.GetFileName(scenarioObjectType.ObjectTagPath));
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
		#endregion

		public bool Export(ScenarioData scenarioData
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
				return true;
			}
			return false;
		}

		/// <summary>   Executes the extraction job. </summary>
		public void Execute()
		{
			JobState = ExtractionStateEnum.InProgress;

			// Create the tag index
			if (!CreateTagIndex(mModelExtractionData.TagsDirectory.Root, mModelExtractionData.TagsDirectory.AbsoluteFolder))
			{
				SetJobCompleted(ExtractionStateEnum.Failed, "Failed to create a tag index for {0}\\{1}", mModelExtractionData.TagsDirectory.Root, mModelExtractionData.TagsDirectory.AbsoluteFolder);
				return;
			}

			var tagPath = mModelExtractionData.TagFile.TagPath;

			// Open the tag
			var tagManager = OpenTag(tagPath, BlamLib.Blam.Halo1.TagGroups.scnr);
			if (tagManager == null)
			{
				SetJobCompleted(ExtractionStateEnum.Failed, "Failed to open tag {0}", tagPath);
				DestroyTagIndex();
				return;
			}

			string filename = Path.GetFileNameWithoutExtension(mModelExtractionData.OutputFile);
			string outputFile = Path.GetDirectoryName(mModelExtractionData.OutputFile);
			outputFile = Path.Combine(outputFile, filename + "-{0}");
			outputFile = Path.ChangeExtension(outputFile, Path.GetExtension(mModelExtractionData.OutputFile));

			bool success = true;
			try
			{
				// Add the tags data sources
				var scenarioData = GetScenarioData(TagIndex, tagManager);

				if (mScenarioData.ExportScenery)
				{
					if (mScenarioData.ExportObjectMeshes)
					{
						AddChildJobs(scenarioData, TypeEnums.ObjectType.Scenery);
					}

					scenarioData.IncludeScenery = true;
					success &= Export(scenarioData, tagManager, String.Format(outputFile, "scenery"));
					scenarioData.IncludeScenery = false;
				}

				if (mScenarioData.ExportDeviceMachines)
				{
					if (mScenarioData.ExportObjectMeshes)
					{
						AddChildJobs(scenarioData, TypeEnums.ObjectType.DeviceMachine);
					}

					scenarioData.IncludeDeviceMachines = true;
					success &= Export(scenarioData, tagManager, String.Format(outputFile, "device_machines"));
					scenarioData.IncludeDeviceMachines = false;
				}

				if (mScenarioData.ExportControls)
				{
					if (mScenarioData.ExportObjectMeshes)
					{
						AddChildJobs(scenarioData, TypeEnums.ObjectType.DeviceControl);
					}

					scenarioData.IncludeDeviceControls = true;
					success &= Export(scenarioData, tagManager, String.Format(outputFile, "device_controls"));
					scenarioData.IncludeDeviceControls = false;
				}

				if (mScenarioData.ExportLightFixtures)
				{
					if (mScenarioData.ExportObjectMeshes)
					{
						AddChildJobs(scenarioData, TypeEnums.ObjectType.DeviceLightFixture);
					}

					scenarioData.IncludeDeviceLightFixtures = true;
					success &= Export(scenarioData, tagManager, String.Format(outputFile, "device_light_fixtures"));
					scenarioData.IncludeDeviceLightFixtures = false;
				}

				if (mScenarioData.ExportSoundScenery)
				{
					if (mScenarioData.ExportObjectMeshes)
					{
						AddChildJobs(scenarioData, TypeEnums.ObjectType.SoundScenery);
					}

					scenarioData.IncludeSoundScenery = true;
					success &= Export(scenarioData, tagManager, String.Format(outputFile, "sound_scenery"));
					scenarioData.IncludeSoundScenery = false;
				}
			}
			catch (ColladaException e)
			{
				mMessageHandler.SendMessage("Exporter Error: {0}", e.Message);
				SetJobCompleted(ExtractionStateEnum.Failed, "Failed to build COLLADA files for tag {0} due to: {1}", JobName, e.Message);

				success = false;
			}

			if (success)
			{
				SetJobCompleted(ExtractionStateEnum.Succeeded, "Extraction complete! Job: {0}", JobName);
			}
			else
			{
				SetJobCompleted(ExtractionStateEnum.Failed, "Failed to build one or more COLLADA files for tag {0}", Path.GetFileName(tagPath));
			}

			DestroyTagIndex();
		}
	}
}
