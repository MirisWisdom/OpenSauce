using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using BlamLib;
using BlamLib.Render.COLLADA;
using OpenSauceIDE.ModelExtractor.TagIO;

namespace OpenSauceIDE.ModelExtractor.Extractors.Halo1.Scenario
{
    [ModelExtractor(BlamVersion.Halo1_CE, "Scenario", "scenario")]
    public class ExtractorScenario
        : IExtractor
    {
        public string TagTypeName
        {
            get { return GetType().GetCustomAttribute<ModelExtractorAttribute>().TagName; }
        }

        public string TagExtension
        {
            get { return GetType().GetCustomAttribute<ModelExtractorAttribute>().TagExtension; }
        }

        #region Initialise/Dispose
        /// <summary>   Initialises this object. </summary>
        public void Initialise()
        {
            mExtractorUI = new ExtractorScenarioUI();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>
        ///     Performs application-defined tasks associated with freeing, releasing, or resetting
        ///     unmanaged resources.
        /// </summary>
        public void Dispose()
        {
            mExtractorUI = null;
        }
        #endregion

        #region UI
        private ExtractorScenarioUI mExtractorUI;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Gets form control. </summary>
        ///
        /// <returns>   The form control. </returns>
        public System.Windows.Forms.Control GetFormControl()
        {
            return mExtractorUI;
        }
        #endregion

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Create an extraction job. </summary>
        ///
        /// <param name="colladaSettings">  The collada settings to use. </param>
        /// <param name="tagsDirectory">    Pathname of the tags directory. </param>
        /// <param name="tagPath">          Full pathname of the tag file. </param>
        /// <param name="outputFile">       The output file. </param>
        ///
        /// <returns>   The new job. </returns>
        public IExtractionJob CreateJob(IColladaSettings colladaSettings
            , string tagsDirectory
            , string tagPath
            , string outputFile)
        {
            var tagsFolder = new BlamPath(tagsDirectory);

            var outputFileRoot = Path.GetDirectoryName(outputFile);
            var outputFileName = Path.GetFileNameWithoutExtension(outputFile) + "-objects";
            var outputFileFinal = Path.Combine(outputFileRoot, outputFileName);
            outputFileFinal = Path.ChangeExtension(outputFileFinal, Path.GetExtension(outputFile));
            
            // Set up the extraction data
            var extractionData = new ExtractionData();

            extractionData.Add("COLLADA", new ColladaSettings()
            {
                Overwrite = colladaSettings.Overwrite,
                RootDirectory = colladaSettings.RootDirectory,
                BitmapExtension = colladaSettings.BitmapExtension
            });

            extractionData.Add("SCENARIO", new ExtractionJobScenarioData()
            {
                TagsRoot = tagsFolder.Root,
                TagsFolder = tagsFolder.EndFolder,
                TagPath = tagPath,
                
                ExportScenery = mExtractorUI.ExportScenery,
                ExportDeviceMachines = mExtractorUI.ExportDeviceMachines,
                ExportControls = mExtractorUI.ExportControls,
                ExportLightFixtures = mExtractorUI.ExportLightFixtures,
                ExportSoundScenery = mExtractorUI.ExportSoundScenery,

                OutputFile = outputFileFinal
            });

            // Create the extraction job
            return new ExtractionJobScenario(extractionData);
        }
    }
}
