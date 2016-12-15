/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.ComponentModel;
using System.IO;
using BlamLib.Managers;
using BlamLib.Render.COLLADA;
using BlamLib.Render.COLLADA.Halo1;

namespace OpenSauceIDE.ModelExtractor.Extractors.Halo1.Gbxmodel
{
	/// <summary>   A gbxmodel extraction job. </summary>
	public class ExtractionJobGbxmodel
		: Halo1ExtractorJob
		, IExtractionJob
	{
		private IColladaSettings mColladaSettings;
		private ModelExtractionData mModelExtractionData;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Constructor. </summary>
		///
		/// <param name="extractionData">   Data for the extraction. </param>
		public ExtractionJobGbxmodel(IExtractionData extractionData)
		{
			mColladaSettings = extractionData.Get<IColladaSettings>();
			mModelExtractionData = extractionData.Get<ModelExtractionData>();

			JobID = mModelExtractionData.TagFile.AbsoluteTagPath;
			JobName = mModelExtractionData.TagFile.TagNameWithExtension;
		}

		#region Data Collection
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Gets the model data from the tag. </summary>
		///
		/// <param name="tagIndex">     The parent tag index. </param>
		/// <param name="tagManager">   The tags' manager. </param>
		///
		/// <returns>   The model data. </returns>
		private ModelData GetModelData(TagIndexBase tagIndex, BlamLib.Managers.TagManager tagManager)
		{
			var data = new ModelData();

			data.CollectData(tagIndex, tagManager);

			return data;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Gets the shader data from the tag. </summary>
		///
		/// <param name="tagIndex">     The parent tag index. </param>
		/// <param name="tagManager">   The tags' manager. </param>
		///
		/// <returns>   The shader data. </returns>
		private ModelShaderData GetShaderData(TagIndexBase tagIndex, BlamLib.Managers.TagManager tagManager)
		{
			var data = new ModelShaderData();

			data.CollectData(tagIndex, tagManager);

			return data;
		}
		#endregion

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Executes the extraction job. </summary>
		///
		/// <returns>   true if it succeeds, false if it fails. </returns>
		public void Execute()
		{
			JobState = ExtractionStateEnum.InProgress;

			// Create the tag index
			if (!CreateTagIndex(mModelExtractionData.TagsDirectory.Root, mModelExtractionData.TagsDirectory.EndFolder))
			{
				SetJobCompleted(ExtractionStateEnum.Failed, "Failed to create a tag index for {0}\\{1}", mModelExtractionData.TagsDirectory.Root, mModelExtractionData.TagsDirectory.EndFolder);
				return;
			}

			var tagPath = mModelExtractionData.TagFile.TagPath;

			// Open the tag
			var tagManager = OpenTag(tagPath, BlamLib.Blam.Halo1.TagGroups.mod2);
			if (tagManager == null)
			{
				SetJobCompleted(ExtractionStateEnum.Failed, "Failed to open tag {0}", tagPath);
				DestroyTagIndex();
				return;
			}

			// Create the COLLADA exporter
			var extractor = new ColladaModelExporter(mColladaSettings,
				TagIndex,
				tagManager);

			extractor.MessageSent +=
				(sender, args) =>
				{
					mMessageHandler.SendMessage("Exporter Error: {0}", args.Message);
				};

			bool result = false;
			try
			{
				// Add the tags data sources
				extractor.AddDataProvider(GetModelData(TagIndex, tagManager));
				extractor.AddDataProvider(GetShaderData(TagIndex, tagManager));

				result = extractor.BuildColladaInstance();
			}
			catch (ColladaException e)
			{
				mMessageHandler.SendMessage("Exporter Error: {0}", e.Message);
				SetJobCompleted(ExtractionStateEnum.Failed, "Failed to build COLLADA file for tag {0} due to: {1}", JobName, e.Message);
			}

			// Export the tag
			if (result)
			{
				extractor.SaveDAE(mModelExtractionData.OutputFile);
				SetJobCompleted(ExtractionStateEnum.Succeeded, "Extraction complete! Tag {0}", tagPath);
			}
			else
			{
				SetJobCompleted(ExtractionStateEnum.Failed, "Failed to build COLLADA file for tag {0}", tagPath);
			}

			DestroyTagIndex();
		}
	}
}
