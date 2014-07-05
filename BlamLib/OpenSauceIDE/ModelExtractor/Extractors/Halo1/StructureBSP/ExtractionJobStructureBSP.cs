using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using BlamLib.Managers;
using BlamLib.Render.COLLADA;
using BlamLib.Render.COLLADA.Halo1;
using OpenSauceIDE.ModelExtractor.MessageHandler;

namespace OpenSauceIDE.ModelExtractor.Extractors.Halo1.StructureBSP
{
    /// <summary>   An structure bsp extraction jobs data. </summary>
    public class ExtractionJobStructureBSPData
    {
        public string TagsRoot;
        public string TagsFolder;
        public string TagPath;

        public bool ExportRenderMesh;
        public bool ExportPortals;
        public bool ExportFogPlanes;
        public bool ExportBSPMarkers;
        public bool ExportLightmapSizes;
        public int LightmapSizeMultiplier;

        public string OutputFile;
    }

    /// <summary>   An structure bsp extraction job. </summary>
    public class ExtractionJobStructureBSP
        : Halo1ExtractorJob
        , IExtractionJob
    {
        private IColladaSettings mColladaSettings;
        private ExtractionJobStructureBSPData mBspData;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Constructor. </summary>
        ///
        /// <param name="extractionData">   Data for the extraction. </param>
        public ExtractionJobStructureBSP(ExtractionData extractionData)
        {
            mColladaSettings = extractionData["COLLADA"] as IColladaSettings;
            mBspData = extractionData["STRUCTUREBSP"] as ExtractionJobStructureBSPData;
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
            // Create the tag index. 
            if (!CreateTagIndex(mBspData.TagsRoot, mBspData.TagsFolder))
            {
                mMessageHandler.SendMessage("Failed to create a tag index for {0}\\{1}", mBspData.TagsRoot, mBspData.TagsFolder);
                return;
            }

            // Open the tag
            var tagManager = OpenTag(mBspData.TagPath, BlamLib.Blam.Halo1.TagGroups.sbsp);
            if (tagManager == null)
            {
                mMessageHandler.SendMessage("Failed to open tag {0}", mBspData.TagPath);
                DestroyTagIndex();
                return;
            }

            // Export the lightmap sizes
            if(mBspData.ExportLightmapSizes)
            {
                var lightmapExporter = new LightmapSizeExporter();

                lightmapExporter.MessageSent += MessageRedirect;
                lightmapExporter.Export(TagIndex, tagManager, mBspData.LightmapSizeMultiplier, mBspData.OutputFile);
                lightmapExporter.MessageSent -= MessageRedirect;
            }

            if (mBspData.ExportRenderMesh
                || mBspData.ExportPortals
                || mBspData.ExportFogPlanes
                || mBspData.ExportBSPMarkers)
            {
                // Create the COLLADA exporter
                var extractor = new ColladaBSPExporter(mColladaSettings,
                   TagIndex,
                   tagManager);

                extractor.ErrorOccured +=
                    (sender, args) =>
                    {
                        mMessageHandler.SendMessage("Exporter Error: {0}", args.ErrorMessage);
                    };

                // Add the tags data sources
                extractor.AddDataProvider(GetBSPData(TagIndex, tagManager));
                extractor.AddDataProvider(GetShaderData(TagIndex, tagManager));

                // Export the tag
                if (extractor.BuildColladaInstance())
                {
                    extractor.SaveDAE(mBspData.OutputFile);
                    mMessageHandler.SendMessage("Extraction complete! Tag {0}", mBspData.TagPath);
                }
                else
                {
                    mMessageHandler.SendMessage("Failed to build COLLADA file for tag {0}", mBspData.TagPath);
                }
            }

            DestroyTagIndex();
        }
    }
}
