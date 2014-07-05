using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using BlamLib.Managers;
using BlamLib.Render.COLLADA;
using BlamLib.Render.COLLADA.Halo1;

namespace OpenSauceIDE.ModelExtractor.Extractors.Halo1.Scenario
{
    /// <summary>   An structure bsp extraction jobs data. </summary>
    public class ExtractionJobScenarioData
    {
        public string TagsRoot;
        public string TagsFolder;
        public string TagPath;

        public bool ExportScenery;
        public bool ExportDeviceMachines;
        public bool ExportControls;
        public bool ExportLightFixtures;
        public bool ExportSoundScenery;

        public string OutputFile;
    }

    /// <summary>   An scenario extraction job. </summary>
    public class ExtractionJobScenario
        : Halo1ExtractorJob
        , IExtractionJob
    {
        private IColladaSettings mColladaSettings;
        private ExtractionJobScenarioData mScenarioData;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Constructor. </summary>
        ///
        /// <param name="extractionData">   Data for the extraction. </param>
        public ExtractionJobScenario(ExtractionData extractionData)
        {
            mColladaSettings = extractionData["COLLADA"] as IColladaSettings;
            mScenarioData = extractionData["SCENARIO"] as ExtractionJobScenarioData;
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
            var data = new ScenarioData()
            {
                IncludeScenery = mScenarioData.ExportScenery,
                IncludeDeviceMachines = mScenarioData.ExportDeviceMachines,
                IncludeDeviceControls = mScenarioData.ExportControls,
                IncludeDeviceLightFixtures = mScenarioData.ExportLightFixtures,
                IncludeSoundScenery = mScenarioData.ExportSoundScenery
            };

            data.CollectData(tagIndex, tagManager);

            return data;
        }
        #endregion

        /// <summary>   Executes the extraction job. </summary>
        public void Execute()
        {
            // Create the tag index
            if (!CreateTagIndex(mScenarioData.TagsRoot, mScenarioData.TagsFolder))
            {
                mMessageHandler.SendMessage("Failed to create a tag index for {0}\\{1}", mScenarioData.TagsRoot, mScenarioData.TagsFolder);
                return;
            }

            // Open the tag
            var tagManager = OpenTag(mScenarioData.TagPath, BlamLib.Blam.Halo1.TagGroups.scnr);
            if (tagManager == null)
            {
                mMessageHandler.SendMessage("Failed to open tag {0}", mScenarioData.TagPath);
                DestroyTagIndex();
                return;
            }

            // Create the COLLADA exporter
            var extractor = new ColladaScenarioExporter(mColladaSettings,
                    TagIndex,
                    tagManager);

            extractor.ErrorOccured +=
                (sender, args) =>
                {
                    mMessageHandler.SendMessage("Exporter Error: {0}", args.ErrorMessage);
                };

            // Add the tags data sources
            extractor.AddDataProvider(GetScenarioData(TagIndex, tagManager));

            // Export the tag
            if (extractor.BuildColladaInstance())
            {
                extractor.SaveDAE(mScenarioData.OutputFile);
                mMessageHandler.SendMessage("Extraction complete! Tag {0}", mScenarioData.TagPath);
            }
            else
            {
                mMessageHandler.SendMessage("Failed to build COLLADA file for tag {0}", mScenarioData.TagPath);
            }

            DestroyTagIndex();
        }
    }
}
