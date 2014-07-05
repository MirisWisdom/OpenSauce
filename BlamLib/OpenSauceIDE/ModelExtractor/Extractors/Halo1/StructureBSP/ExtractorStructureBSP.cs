using BlamLib.IO;
using BlamLib.Managers;
using BlamLib.Render.COLLADA;
using BlamLib.Render.COLLADA.Halo1;
using OpenSauceIDE.ModelExtractor.TagIO;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using BlamLib;

namespace OpenSauceIDE.ModelExtractor.Extractors.Halo1.StructureBSP
{
    [ModelExtractor(BlamVersion.Halo1_CE, "Structure BSP", "scenario_structure_bsp")]
    public class ExtractorStructureBSP
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
            mExtractorUI = new ExtractorStructureBSPUI();
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
        private ExtractorStructureBSPUI mExtractorUI;

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
            var tagsFolder = new BlamTagPath(tagsDirectory);

            // Set up the extraction data
            var extractionData = new ExtractionData();

            extractionData.Add("COLLADA", new ColladaSettings()
            {
                Overwrite = colladaSettings.Overwrite,
                RootDirectory = colladaSettings.RootDirectory,
                BitmapExtension = colladaSettings.BitmapExtension
            });

            extractionData.Add("STRUCTUREBSP", new ExtractionJobStructureBSPData()
            {
                TagsRoot = tagsFolder.Root,
                TagsFolder = tagsFolder.EndFolder,
                TagPath = tagPath,

                ExportRenderMesh = mExtractorUI.ExportRenderMesh,
                ExportPortals = mExtractorUI.ExportPortals,
                ExportFogPlanes = mExtractorUI.ExportFogPlanes,
                ExportBSPMarkers = mExtractorUI.ExportBSPMarkers,
                ExportLightmapSizes = mExtractorUI.ExportLightmapSizes,
                LightmapSizeMultiplier = mExtractorUI.LightmapSizeMultiplier,

                OutputFile = outputFile
            });

            // Create the extraction job
            return new ExtractionJobStructureBSP(extractionData);
        }
    }
}
