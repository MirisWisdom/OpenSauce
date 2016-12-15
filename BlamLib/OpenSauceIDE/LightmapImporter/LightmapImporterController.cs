/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.IO;
using BlamLib;
using BlamLib.Messaging;
using BlamLib.TagInterface;
using OpenSauceIDE.LightmapImporter.Settings;

using String = System.String;

namespace OpenSauceIDE.LightmapImporter
{
	/// <summary>	A controller for handling lightmap importing. </summary>
	public class LightmapImporterController
		: ILightmapImporterController
	{
		#region Constructor
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="gameVersion">	The game version. </param>
		public LightmapImporterController(BlamVersion gameVersion)
		{
			SettingsHandler.GetSettings();
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
		/// <param name="e">        The message arguments. </param>
		private void MessageRedirect(object sender, MessageArgs e)
		{
			mMessageHandler.SendMessage(e.Message);
		}
		#endregion
		
		#region State
		private LightmapImporterStateEnum mState;

		/// <summary>   Event queue for all listeners interested in StateChanged events. </summary>
		public event EventHandler<LightmapImporterStateChangeArgs> StateChanged;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the controllers state. </summary>
		///
		/// <param name="state">	The state to change to. </param>
		private void SetState(LightmapImporterStateEnum state)
		{
			mState = state;

			var handler = StateChanged;
			if (handler != null)
			{
				handler(this, new LightmapImporterStateChangeArgs(state));
			}
		}
		#endregion

		#region Settings
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets importer settings. </summary>
		///
		/// <returns>	The importer settings. </returns>
		public Settings.ILightmapImporterSettings GetImporterSettings()
		{
			return SettingsHandler.LightmapImporter;
		}
		#endregion

		#region File Selection
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets a file dialog with default settings set. </summary>
		///
		/// <typeparam name="T">	Generic type parameter. </typeparam>
		///
		/// <returns>	The default file dialog. </returns>
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
		/// <summary>	Gets an input file from the user. </summary>
		///
		/// <param name="initialDirectory">	Pathname of the initial directory. </param>
		/// <param name="description">	   	The dialog description. </param>
		/// <param name="fileFilter">	   	A filter specifying the file type. </param>
		///
		/// <returns>	The input file path. </returns>
		private string GetInputFile(string initialDirectory, string description, string fileFilter)
		{
			var fileDialog = GetFileDefaultDialog<System.Windows.Forms.OpenFileDialog>();

			// Set the dialog settings
			fileDialog.Title = description;
			fileDialog.Multiselect = false;
			fileDialog.InitialDirectory = initialDirectory;

			// Build the extension filter
			fileDialog.Filter = fileFilter;

			// Get the files to extract
			if (fileDialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
			{
				return fileDialog.FileName;
			}

			return null;
		}
		#endregion

		/// <summary>	Imports lightmap coordinates into a bsp. </summary>
		public void ImportLightmap()
		{
			// Verify the directory settings are correct
			var importerSettings = GetImporterSettings();

			if (!Directory.Exists(importerSettings.DataFolder))
			{
				mMessageHandler.SendMessage("The selected data directory does not exist");
				return;
			}

			if (!Directory.Exists(importerSettings.TagsFolder))
			{
				mMessageHandler.SendMessage("The selected tags directory does not exist");
				return;
			}

			var tagsDir = new BlamPath(importerSettings.TagsFolder);
			var dataDir = new BlamPath(importerSettings.DataFolder);

			// Get the input files
			string structureBSPPath = GetInputFile(GetImporterSettings().TagsFolder, "Select the BSP tag", "Structure BSP (*.scenario_structure_bsp)|*.scenario_structure_bsp");
			if (String.IsNullOrEmpty(structureBSPPath))
			{
				mMessageHandler.SendMessage("No BSP tag was selected");
				return;
			}

			string colladaPath = GetInputFile(GetImporterSettings().DataFolder, "Select the COLLADA file", "COLLADA (*.dae)|*.dae");
			if (String.IsNullOrEmpty(colladaPath))
			{
				mMessageHandler.SendMessage("No COLLADA file was selected");
				return;
			}

			// Verify the selected files are under the right directories
			var absoluteBSPFile = Path.GetFullPath(structureBSPPath);
			if (System.String.Compare(absoluteBSPFile, 0, tagsDir.AbsoluteFolder, 0, tagsDir.AbsoluteFolder.Length, true) != 0)
			{
				System.Windows.Forms.MessageBox.Show("The selected BSP file is not under the tags directory", "Invalid File Path", System.Windows.Forms.MessageBoxButtons.OK);
				return;
			}

			var absoluteCOLLADAFile = Path.GetFullPath(colladaPath);
			if (System.String.Compare(absoluteCOLLADAFile, 0, dataDir.AbsoluteFolder, 0, dataDir.AbsoluteFolder.Length, true) != 0)
			{
				System.Windows.Forms.MessageBox.Show("The selected COLLADA file is not under the data directory", "Invalid File Path", System.Windows.Forms.MessageBoxButtons.OK);
				return;
			}

			// Run the import process
			var lightmapImporter = new Importer.LightmapImporter();

			SetState(LightmapImporterStateEnum.ImporterImporting);
			lightmapImporter.MessageSent += MessageRedirect;

			var tagPath = new BlamTagPath(tagsDir.AbsoluteFolder);
			tagPath.SetPath(absoluteBSPFile);

			bool result = lightmapImporter.ImportTexcoords(tagsDir.AbsoluteFolder, tagPath.TagPath, absoluteCOLLADAFile);

			lightmapImporter.MessageSent -= MessageRedirect;
			SetState(LightmapImporterStateEnum.ImporterReady);

			if(!result)
			{
				mMessageHandler.SendMessage("Lightmap UV import failed");
				return;
			}

			mMessageHandler.SendMessage("Lightmap UV import completed");
		}
	}
}
