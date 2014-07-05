using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BlamLib;
using BlamLib.Render.COLLADA;
using OpenSauceIDE.ModelExtractor.Extractors;
using OpenSauceIDE.ModelExtractor.MessageHandler;
using OpenSauceIDE.ModelExtractor.TagIO;
using OpenSauceIDE.ModelExtractor.UI;

namespace OpenSauceIDE.ModelExtractor
{
    /// <summary>   Enum for the bitmap format to use when extracting models. </summary>
    public enum ModelExtractorBitmapFormat
    {
        Tga,
        Tiff,
        Bmp,
        Jpg,
        Png
    }
        
    /// <summary>   A controller for handling model extraction. </summary>
    public class ModelExtractorController
        : IModelExtractorController
    {
        #region Constructor
        /// <summary>   Default constructor. </summary>
        public ModelExtractorController(BlamVersion gameVersion)
        {
            // Start the extraction thread
            mExtractionManager.MessageSent += MessageRedirect;
            mExtractionManager.Start();

            // Add all extractor types
            mExtractorFactory.RegisterExtractors(gameVersion);
        }
        #endregion

        #region IDisposable
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>
        ///     Performs application-defined tasks associated with freeing, releasing, or resetting
        ///     resources.
        /// </summary>
        public void Dispose()
        {
            // Stop the extraction thread
            mExtractionManager.Stop();
        }
        #endregion

        #region Messaging
        private IMessageHandler mMessageHandler = new MessageHandler.MessageHandler();

        /// <summary>   Event queue for all listeners interested in MessageSent events. </summary>
        public event EventHandler<ModelExtractorMessageArgs> MessageSent
        {
            add { mMessageHandler.MessageSent += value; }
            remove { mMessageHandler.MessageSent -= value; }
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Passes a message through to the controllers message handler. </summary>
        ///
        /// <param name="sender">   Source of the event. </param>
        /// <param name="e">        The ModelExtractorMessageArgs to process. </param>
        private void MessageRedirect(object sender, ModelExtractorMessageArgs e)
        {
            mMessageHandler.SendMessage(e.Message);
        }
        #endregion

        #region State
        private ModelExtractorStateEnum mState;

        /// <summary>   Event queue for all listeners interested in StateChanged events. </summary>
        public event EventHandler<ModelExtractorStateChangeArgs> StateChanged;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Sets the controllers state. </summary>
        ///
        /// <param name="state">    The state to change to. </param>
        private void SetState(ModelExtractorStateEnum state)
        {
            mState = state;

            var handler = StateChanged;
            if(handler != null)
            {
                handler(this, new ModelExtractorStateChangeArgs(state));
            }
        }
        #endregion
        
        #region File Selection
        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Gets a file dialog with default settings set. </summary>
        ///
        /// <tparam name="T">   Generic type parameter. </tparam>
        ///
        /// <returns>   The default file dialog. </returns>
        private T GetFileDefaultDialog<T>()
            where T : System.Windows.Forms.FileDialog, new()
        {
            return new T()
            {
                AddExtension = true,
                CheckFileExists = true,
                ShowHelp = false
            };
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Gets the input files from the user. </summary>
        ///
        /// <param name="initialDirectory"> Pathname of the initial directory. </param>
        ///
        /// <returns>   The input file paths. </returns>
        private string[] GetInputFiles(string initialDirectory)
        {
            var fileDialog = GetFileDefaultDialog<System.Windows.Forms.OpenFileDialog>();

            // Set the dialog settings
            fileDialog.Title = "Select models to extract";
            fileDialog.Multiselect = true;
            fileDialog.InitialDirectory = initialDirectory;

            // Build the extension filter
            fileDialog.Filter = String.Format("{0} (*.{1})|*.{1}", mExtractor.TagTypeName, mExtractor.TagExtension);

            // Get the files to extract
            if (fileDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                return fileDialog.FileNames;
            }

            return null;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Gets the output file from the input file path. </summary>
        ///
        /// <param name="tagsDirectory">    Pathname of the tags directory. </param>
        /// <param name="dataDirectory">    Pathname of the data directory. </param>
        /// <param name="inputFile">        The input file path. </param>
        ///
        /// <returns>   The output file path. </returns>
        private string GetOutputFile(string tagsDirectory, string dataDirectory, string inputFile)
        {
            var input = new BlamTagPath(tagsDirectory);
            input.SetPath(inputFile);

            var outputFile = Path.Combine(dataDirectory, input.TagPath);
            outputFile = Path.ChangeExtension(outputFile, ".dae");

            return outputFile;
        }
        #endregion
        
        #region Extractor
        private IExtractorFactory mExtractorFactory = new ExtractorFactory();
        private ExtractionManager mExtractionManager = new ExtractionManager();
        private IExtractor mExtractor;

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Gets the extractor factory. </summary>
        ///
        /// <returns>   The extractor factory. </returns>
        public IExtractorFactory GetExtractorFactory()
        {
            return mExtractorFactory;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Opens an extractor type. </summary>
        ///
        /// <exception cref="NotSupportedException">
        ///     Thrown when the requested operation is not supported.
        /// </exception>
        ///
        /// <param name="extension">    The extension to open an exporter for. </param>
        public void OpenExtractor(string extension)
        {
            // Get the extractor
            mExtractor = mExtractorFactory.GetExtractor(extension);
            if(mExtractor == null)
            {
                throw new NotSupportedException("Attempted to extract an unsupported file type");
            }

            // Initialise the extractor
            SetState(ModelExtractorStateEnum.ExtractorInitialising);
            mExtractor.Initialise();
            SetState(ModelExtractorStateEnum.ExtractorReady);
        }

        /// <summary>   Closes the extractor. </summary>
        public void CloseExtractor()
        {
            if (mExtractor == null)
            {
                return;
            }

            // Destroy the extractor
            SetState(ModelExtractorStateEnum.ExtractorClosing);
            mExtractor.Dispose();
            SetState(ModelExtractorStateEnum.ExtractorClosed);
            mExtractor = null;
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Extracts a model. </summary>
        ///
        /// <param name="tagsDirectory">    Pathname of the tags directory. </param>
        /// <param name="dataDirectory">    Pathname of the data directory. </param>
        public void Extract(string tagsDirectory, string dataDirectory)
        {
            var tagsDir = new BlamPath(tagsDirectory);
            var dataDir = new BlamPath(dataDirectory);

            // Get the input files
            var modelFiles = GetInputFiles(tagsDir.AbsoluteFolder);
            if (modelFiles == null)
            {
                mMessageHandler.SendMessage("No models selected");
                return;
            }

            // Check that all of the selected models reside in the tags directory
            foreach (var modelFile in modelFiles)
            {
                var absoluteModelFile = Path.GetFullPath(modelFile);
                if (String.Compare(absoluteModelFile, 0, tagsDir.AbsoluteFolder, 0, tagsDir.AbsoluteFolder.Length, true) != 0)
                {
                    System.Windows.Forms.MessageBox.Show("A selected model file is not under the tags directory", "Invalid File Path", System.Windows.Forms.MessageBoxButtons.OK);
                    return;
                }
            }

            // Build the Collada settings
            var bitmapExtension = "";
            switch (mSettingsUI.BitmapFormat)
            {
                case ModelExtractorBitmapFormat.Tga:
                    bitmapExtension = "tga";
                    break;
                case ModelExtractorBitmapFormat.Bmp:
                    bitmapExtension = "bmp";
                    break;
                case ModelExtractorBitmapFormat.Jpg:
                    bitmapExtension = "jpg";
                    break;
                case ModelExtractorBitmapFormat.Png:
                    bitmapExtension = "png";
                    break;
                default:
                    bitmapExtension = "tif";
                    break;
            }

            var colladaSettings = new ColladaSettings()
            {
                Overwrite = mSettingsUI.Overwrite,
                BitmapExtension = bitmapExtension,
                RootDirectory = dataDir.AbsoluteFolder
            };

            // Create extraction jobs for the selected models
            foreach(var modelFile in modelFiles)
            {
                // Get the output file path
                var outputFile = GetOutputFile(tagsDir.AbsoluteFolder, dataDir.AbsoluteFolder, modelFile);

                // Build the tag path
                var tagPath = new BlamTagPath(tagsDir.AbsoluteFolder);
                tagPath.SetPath(modelFile);

                // Add an extraction job to the extraction manager
                var extractionJob = mExtractor.CreateJob(colladaSettings, tagsDir.AbsoluteFolder, tagPath.TagPath, outputFile);
                mExtractionManager.Add(extractionJob);
            }
        }
        #endregion

        #region UI
        private ColladaSettingsControl mSettingsUI = new ColladaSettingsControl();

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Gets the extractors interface control. </summary>
        ///
        /// <returns>   The extractors interface control. </returns>
        public System.Windows.Forms.Control GetExtractorControl()
        {
            if(mExtractor == null)
            {
                return null;
            }

            return mExtractor.GetFormControl();
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////////
        /// <summary>   Gets the collada settings control. </summary>
        ///
        /// <returns>   The collada settings control. </returns>
        public System.Windows.Forms.Control GetCOLLADAControl()
        {
            return mSettingsUI;
        }
        #endregion
    }
}
