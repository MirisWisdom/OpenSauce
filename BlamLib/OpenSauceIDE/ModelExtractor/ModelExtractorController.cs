/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;
using BlamLib;
using BlamLib.Bitmaps;
using BlamLib.Messaging;
using BlamLib.Render.COLLADA;
using BlamLib.TagInterface;
using OpenSauceIDE.ModelExtractor.Extractors;
using OpenSauceIDE.ModelExtractor.Settings;
using OpenSauceIDE.ModelExtractor.UI;

namespace OpenSauceIDE.ModelExtractor
{
	public class ModelExtractionData
		: ICloneable
	{
		public IModelExtractorJobInterface JobInterface { get; set; }

		public BlamPath TagsDirectory { get; set; }
		public BlamPath DataDirectory { get; set; }
		public BlamTagPath TagFile { get; set; }

		public string OutputFile { get; set; }

		public object Clone()
		{
			return MemberwiseClone();
		}
	}

	/// <summary>   A controller for handling model extraction. </summary>
	public class ModelExtractorController
		: IModelExtractorController
		, IModelExtractorJobInterface
	{
		#region Constructor
		/// <summary>   Default constructor. </summary>
		public ModelExtractorController(BlamVersion gameVersion)
		{
			SettingsHandler.GetSettings();
			mSettingsUI.Attach(SettingsHandler.ModelExtractor.Collada);

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
		private IMessageHandler mMessageHandler = new MessageHandler();

		/// <summary>   Event queue for all listeners interested in MessageSent events. </summary>
		public event EventHandler<MessageArgs> MessageSent
		{
			add { mMessageHandler.MessageSent += value; }
			remove { mMessageHandler.MessageSent -= value; }
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Passes a message through to the controllers message handler. </summary>
		///
		/// <param name="sender">   Source of the event. </param>
		/// <param name="e">        The ModelExtractorMessageArgs to process. </param>
		private void MessageRedirect(object sender, MessageArgs e)
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

		#region Settings
		public IExtractorPathSettings GetExtractorSettings()
		{
			return SettingsHandler.ModelExtractor.Extractor;
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
			fileDialog.Filter = mCurrentFileType.TypeFilter;

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
		private class ColladaSettingsInstance
			: IColladaSettings
			, ICloneable
		{
			public bool Overwrite { get; set; }
			public string RootDirectory { get; set; }
			public AssetFormat BitmapFormat { get; set; }

			public object Clone()
			{
				return MemberwiseClone();
			}
		}

		private IExtractorFactory mExtractorFactory = new ExtractorFactory();
		private IExtractionManager mExtractionManager = new ExtractionManager();
		private List<IExtractor> mExtractors = new List<IExtractor>();
		private ExtractorFileType mCurrentFileType;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Gets the extractor factory. </summary>
		///
		/// <returns>   The extractor factory. </returns>
		public IExtractorFactory GetExtractorFactory()
		{
			return mExtractorFactory;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the extraction manager. </summary>
		///
		/// <returns>	The extraction manager. </returns>
		public IExtractionManager GetExtractionManager()
		{
			return mExtractionManager;
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
			// Get the extractor ui's
			mExtractors.AddRange(mExtractorFactory.GetExtractors(extension));
			if (mExtractors.Count == 0)
			{
				throw new NotSupportedException("Attempted to extract an unsupported file type");
			}

			mCurrentFileType = mExtractorFactory.GetFileType(extension);

			// Initialise the extractor
			SetState(ModelExtractorStateEnum.ExtractorInitialising);
			mExtractors.ForEach((extractor) => extractor.Initialise());
			SetState(ModelExtractorStateEnum.ExtractorReady);
		}

		/// <summary>   Closes the extractor. </summary>
		public void CloseExtractor()
		{
			if (mExtractors.Count == 0)
			{
				return;
			}

			// Destroy the extractor
			SetState(ModelExtractorStateEnum.ExtractorClosing);
			mExtractors.ForEach((extractor) => extractor.Dispose());
			SetState(ModelExtractorStateEnum.ExtractorClosed);
			mExtractors.Clear();
		}

		/// <summary>	Extracts a model. </summary>
		public void Extract()
		{
			var extractorSettings = SettingsHandler.ModelExtractor.Extractor;

			if (!Directory.Exists(extractorSettings.DataFolder))
			{
				mMessageHandler.SendMessage("The selected data directory does not exist");
				return;
			}

			if (!Directory.Exists(extractorSettings.TagsFolder))
			{
				mMessageHandler.SendMessage("The selected tags directory does not exist");
				return;
			}

			var tagsDir = new BlamPath(extractorSettings.TagsFolder);
			var dataDir = new BlamPath(extractorSettings.DataFolder);

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
				if (System.String.Compare(absoluteModelFile, 0, tagsDir.AbsoluteFolder, 0, tagsDir.AbsoluteFolder.Length, true) != 0)
				{
					System.Windows.Forms.MessageBox.Show("A selected model file is not under the tags directory", "Invalid File Path", System.Windows.Forms.MessageBoxButtons.OK);
					return;
				}
			}

			// Partially set up the model extraction data
			var modelExtractionData = new ModelExtractionData()
			{
				JobInterface = this,

				TagsDirectory = tagsDir,
				DataDirectory = dataDir
			};

			// Build the Collada settings
			var colladaSettings = SettingsHandler.ModelExtractor.Collada;
			var colladaSettingsInstance = new ColladaSettingsInstance()
			{
				Overwrite = colladaSettings.Overwrite,
				BitmapFormat = colladaSettings.BitmapFormat,
				RootDirectory = dataDir.AbsoluteFolder
			};

			// Create extraction jobs for the selected models
			foreach (var modelFile in modelFiles)
			{
				var extractionData = new ExtractionData();

				extractionData.Set(modelExtractionData);
				extractionData.Set(colladaSettingsInstance, typeof(IColladaSettings));

				foreach(var extractorUI in mExtractors)
				{
					var data = extractorUI.GetExtractionData();

					if (data != null)
					{
						extractionData.Set(data);
					}
				}

				AddJob(modelFile, extractionData);
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds a job to the extraction queue</summary>
		///
		/// <param name="modelFile">	 	The path of the model tag. </param>
		/// <param name="extractionData">	The extraction data to use. </param>
		public void AddJob(string modelFile, IExtractionData extractionData)
		{
			// Clone the extraction data, then override the model extraction data values
			var clonedData = extractionData.Clone() as IExtractionData;
			var modelExtractionData = clonedData.Get<ModelExtractionData>();

			var outputFile = GetOutputFile(modelExtractionData.TagsDirectory.AbsoluteFolder, modelExtractionData.DataDirectory.AbsoluteFolder, modelFile);

			var tagPath = new BlamTagPath(modelExtractionData.TagsDirectory.AbsoluteFolder);
			tagPath.SetPath(modelFile);

			modelExtractionData.TagFile = tagPath;
			modelExtractionData.OutputFile = outputFile;

			// Add an extraction job to the extraction manager
			var modelExtractor = mExtractorFactory.GetExtractionJob(Path.GetExtension(modelFile), clonedData);
			mExtractionManager.Add(modelExtractor);
		}
		#endregion

		#region UI
		private ColladaSettingsControl mSettingsUI = new ColladaSettingsControl();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>   Gets the extractors interface control. </summary>
		///
		/// <returns>   The extractors interface control. </returns>
		public IEnumerable<System.Windows.Forms.Control> GetControls()
		{
			yield return mSettingsUI;

			foreach(var extractor in mExtractors)
			{
				yield return extractor.GetFormControl();
			}
		}
		#endregion
	}
}
