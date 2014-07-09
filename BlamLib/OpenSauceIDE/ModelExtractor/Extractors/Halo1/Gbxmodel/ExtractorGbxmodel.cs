using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using BlamLib;
using BlamLib.Render.COLLADA;
using OpenSauceIDE.ModelExtractor.TagIO;

namespace OpenSauceIDE.ModelExtractor.Extractors.Halo1.Gbxmodel
{
    [ModelExtractor(BlamVersion.Halo1_CE, "Gbxmodel", "gbxmodel")]
    public class ExtractorGbxmodel
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

        private ExtractorGbxmodelUI mExtractorUI;

        #region Initialise/Dispose
        /// <summary>   Initialises this object. </summary>
        public void Initialise()
        {
            mExtractorUI = new ExtractorGbxmodelUI();
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

            // Set up the extraction data
            var extractionData = new ExtractionData();

            extractionData.Add("COLLADA", new ColladaSettings()
            {
                Overwrite = colladaSettings.Overwrite,
                RootDirectory = colladaSettings.RootDirectory,
                BitmapExtension = colladaSettings.BitmapExtension
            });

            extractionData.Add("GBXMODEL", new ExtractionJobGbxmodelData()
            {
                TagsRoot = tagsFolder.Root,
                TagsFolder = tagsFolder.EndFolder,
                TagPath = tagPath,

                OutputFile = outputFile
            });

            // Create the extraction job
            return new ExtractionJobGbxmodel(extractionData);
        }
    }
}
