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

namespace OpenSauceIDE.ModelExtractor.Extractors.Halo1.StructureBSP
{
	/// <summary>   An structure bsp extraction jobs data. </summary>
	public class ExtractionJobStructureBSPData
		: ICloneable
	{
		public bool ExportRenderMesh;
		public bool ExportPortals;
		public bool ExportFogPlanes;
		public bool ExportBSPMarkers;
		public bool ExportLightmapSizes;
		public int LightmapSizeMultiplier;

		public object Clone()
		{
			return MemberwiseClone();
		}
	}

	/// <summary>   An structure bsp extraction job. </summary>
	public class ExtractionJobStructureBSP
		: Halo1ExtractorJob
		, IExtractionJob
	{
		public ExtractionData ExtractionData { get; private set; }

		private IColladaSettings mColladaSettings;
		private ModelExtractionData mModelExtractionData;
		private ExtractionJobStructureBSPData mBspData;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="extractionData">	Data for the extraction. </param>
		public ExtractionJobStructureBSP(IExtractionData extractionData)
		{
			mColladaSettings = extractionData.Get<IColladaSettings>();
			mModelExtractionData = extractionData.Get<ModelExtractionData>();
			mBspData = extractionData.Get<ExtractionJobStructureBSPData>();

			JobID = mModelExtractionData.TagFile.AbsoluteTagPath;
			JobName = mModelExtractionData.TagFile.TagNameWithExtension;
		}

		#region Data Collection
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Gets the BSP data from the tag. </summary>
		///
		/// <param name="tagIndex">     The parent tag index. </param>
		/// <param name="tagManager">   The tag's manager. </param>
		///
		/// <returns>   The bsp data. </returns>
		private StructureBSPData GetBSPData(TagIndexBase tagIndex, BlamLib.Managers.TagManager tagManager)
		{
			var data = new StructureBSPData()
			{
				IncludeRenderMesh = mBspData.ExportRenderMesh,
				IncludePortals = mBspData.ExportPortals,
				IncludeFogPlanes = mBspData.ExportFogPlanes,
				IncludeBSPMarkers = mBspData.ExportBSPMarkers
			};

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
		private StructureBSPShaderData GetShaderData(TagIndexBase tagIndex, BlamLib.Managers.TagManager tagManager)
		{
			var data = new StructureBSPShaderData();

			data.CollectData(tagIndex, tagManager);

			return data;
		}
		#endregion

		/// <summary>   Executes the extraction job. </summary>
		public void Execute()
		{
			JobState = ExtractionStateEnum.InProgress;

			// Create the tag index.
			if (!CreateTagIndex(mModelExtractionData.TagsDirectory.Root, mModelExtractionData.TagsDirectory.AbsoluteFolder))
			{
				SetJobCompleted(ExtractionStateEnum.Failed, "Failed to create a tag index for {0}\\{1}", mModelExtractionData.TagsDirectory.Root, mModelExtractionData.TagsDirectory.AbsoluteFolder);
				return;
			}

			var tagPath = mModelExtractionData.TagFile.TagPath;

			// Open the tag
			var tagManager = OpenTag(tagPath, BlamLib.Blam.Halo1.TagGroups.sbsp);
			if (tagManager == null)
			{
				SetJobCompleted(ExtractionStateEnum.Failed, "Failed to open tag {0}", tagPath);
				DestroyTagIndex();
				return;
			}

			// Export the lightmap sizes
			if(mBspData.ExportLightmapSizes)
			{
				var lightmapExporter = new LightmapSizeExporter();

				lightmapExporter.MessageSent += MessageRedirect;
				lightmapExporter.Export(TagIndex, tagManager, mBspData.LightmapSizeMultiplier, mModelExtractionData.OutputFile);
				lightmapExporter.MessageSent -= MessageRedirect;
			}

			// Export the BSP meshes
			if (mBspData.ExportRenderMesh
				|| mBspData.ExportPortals
				|| mBspData.ExportFogPlanes
				|| mBspData.ExportBSPMarkers)
			{
				// Create the COLLADA exporter
				var extractor = new ColladaBSPExporter(mColladaSettings,
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
					extractor.AddDataProvider(GetBSPData(TagIndex, tagManager));
					extractor.AddDataProvider(GetShaderData(TagIndex, tagManager));

					result = extractor.BuildColladaInstance();
				}
				catch (ColladaException e)
				{
					mMessageHandler.SendMessage("Exporter Error: {0}", e.Message);
					SetJobCompleted(ExtractionStateEnum.Failed, "Failed to build COLLADA file for tag {0} due to: {1}", JobName, e.Message);
				}

				if (result)
				{
					extractor.SaveDAE(mModelExtractionData.OutputFile);
					SetJobCompleted(ExtractionStateEnum.Succeeded, "Extraction complete! Tag {0}", tagPath);
				}
				else
				{
					SetJobCompleted(ExtractionStateEnum.Failed, "Failed to build COLLADA file for tag {0}", tagPath);
				}
			}

			DestroyTagIndex();
		}
	}
}
