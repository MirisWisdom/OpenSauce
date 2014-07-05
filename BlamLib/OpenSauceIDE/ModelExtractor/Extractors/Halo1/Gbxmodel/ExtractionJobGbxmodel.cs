using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using BlamLib.Managers;
using BlamLib.Render.COLLADA;
using BlamLib.Render.COLLADA.Halo1;

namespace OpenSauceIDE.ModelExtractor.Extractors.Halo1.Gbxmodel
{
    /// <summary>   A gbxmodel extraction jobs data. </summary>
    public class ExtractionJobGbxmodelData
    {
        public string TagsRoot;
        public string TagsFolder;
        public string TagPath;

        public string OutputFile;
    }

    /// <summary>   A gbxmodel extraction job. </summary>
    public class ExtractionJobGbxmodel
        : Halo1ExtractorJob
        , IExtractionJob
    {
        private IColladaSettings mColladaSettings;
        private ExtractionJobGbxmodelData mGbxmodelData;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Constructor. </summary>
        ///
        /// <param name="extractionData">   Data for the extraction. </param>
        public ExtractionJobGbxmodel(ExtractionData extractionData)
        {
            mColladaSettings = extractionData["COLLADA"] as IColladaSettings;
            mGbxmodelData = extractionData["GBXMODEL"] as ExtractionJobGbxmodelData;
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
            // Create the tag index
            if (!CreateTagIndex(mGbxmodelData.TagsRoot, mGbxmodelData.TagsFolder))
            {
                mMessageHandler.SendMessage("Failed to create a tag index for {0}\\{1}", mGbxmodelData.TagsRoot, mGbxmodelData.TagsFolder);
                return;
            }

            // Open the tag
            var tagManager = OpenTag(mGbxmodelData.TagPath, BlamLib.Blam.Halo1.TagGroups.mod2);
            if (tagManager == null)
            {
                mMessageHandler.SendMessage("Failed to open tag {0}", mGbxmodelData.TagPath);
                DestroyTagIndex();
                return;
            }

            // Create the COLLADA exporter
            var extractor = new ColladaModelExporter(mColladaSettings,
                TagIndex,
                tagManager);

            extractor.ErrorOccured +=
                (sender, args) =>
                {
                    mMessageHandler.SendMessage("Exporter Error: {0}", args.ErrorMessage);
                };

            // Add the tags data sources
            extractor.AddDataProvider(GetModelData(TagIndex, tagManager));
            extractor.AddDataProvider(GetShaderData(TagIndex, tagManager));

            // Export the tag
            if (extractor.BuildColladaInstance())
            {
                extractor.SaveDAE(mGbxmodelData.OutputFile);
                mMessageHandler.SendMessage("Extraction complete! Tag {0}", mGbxmodelData.TagPath);
            }
            else
            {
                mMessageHandler.SendMessage("Failed to build COLLADA file for tag {0}", mGbxmodelData.TagPath);
            }

            DestroyTagIndex();
        }
    }
}
