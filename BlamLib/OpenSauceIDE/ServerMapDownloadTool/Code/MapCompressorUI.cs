using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;
using System.Windows.Forms;
using System.ComponentModel;
using System.IO;
using System.Drawing;

namespace OpenSauceIDE.ServerMapDownloadTool
{
	public partial class MapCompressorUI
	{
		#region Fields
		List<ProgressController> m_progress_controllers = new List<ProgressController>();
		#endregion

		bool ValidateVariables()
		{
			// determine if the required data has been supplied by the user
			if (!Directory.Exists(SettingsHandler.MapDownloadToolSettings.MapCompressor.PartsFolder)) return false;
			if (!Directory.Exists(SettingsHandler.MapDownloadToolSettings.MapCompressor.DefinitionsFolder)) return false;
			if (SettingsHandler.MapDownloadToolSettings.MapCompressor.EncryptArchive && (SettingsHandler.MapDownloadToolSettings.MapCompressor.ServerPassword == "")) return false;

			return true;
		}

		void PartsFolderBrowseButton_Click(object sender, EventArgs e)
		{
			// get the output directroy from the user
			string directory = Util.GetDirectoryDialog(SettingsHandler.MapDownloadToolSettings.MapCompressor.PartsFolder);

			if (directory.Length != 0)
				SettingsHandler.MapDownloadToolSettings.MapCompressor.PartsFolder = directory;
		}

		void DefinitionsFolderBrowseButton_Click(object sender, EventArgs e)
		{
			// get the output directroy from the user
			string directory = Util.GetDirectoryDialog(SettingsHandler.MapDownloadToolSettings.MapCompressor.DefinitionsFolder);

			if (directory.Length != 0)
				SettingsHandler.MapDownloadToolSettings.MapCompressor.DefinitionsFolder = directory;
		}

		void MapCompressorGlobals_PropertyChanged(object sender, PropertyChangedEventArgs e)
		{
			// update the UI according to the bound properties that have changed
			if (e.PropertyName == "PartsFolder")
			{
				if (!Directory.Exists(SettingsHandler.MapDownloadToolSettings.MapCompressor.PartsFolder))
					PartsFolderTextBox.ForeColor = Color.Salmon;
				else
					PartsFolderTextBox.ForeColor = Color.LightGreen;
			}
			else if (e.PropertyName == "DefinitionsFolder")
			{
				if (!Directory.Exists(SettingsHandler.MapDownloadToolSettings.MapCompressor.DefinitionsFolder))
					DefinitionsFolderTextBox.ForeColor = Color.Salmon;
				else
					DefinitionsFolderTextBox.ForeColor = Color.LightGreen;
			}
			else if (e.PropertyName == "EncryptArchive" || e.PropertyName == "ServerPassword")
			{
				ServerPasswordTextBox.Enabled = SettingsHandler.MapDownloadToolSettings.MapCompressor.EncryptArchive;

				if (SettingsHandler.MapDownloadToolSettings.MapCompressor.EncryptArchive)
				{
					if (SettingsHandler.MapDownloadToolSettings.MapCompressor.ServerPassword == "")
						ServerPasswordTextBox.ForeColor = Color.Salmon;
					else
						ServerPasswordTextBox.ForeColor = Color.LightGreen;
				}
			}

			if (!ValidateVariables())
			{
				CompressMapButton.ForeColor = Color.FromKnownColor(KnownColor.ControlDark);
				CompressMapButton.Enabled = false;
			}
			else
			{
				CompressMapButton.ForeColor = Color.LightGreen;
				CompressMapButton.Enabled = true;
			}
		}

		void CompressMapButton_Click(object sender, EventArgs e)
		{
			// add the output folders to the recent folders lists
			if (!Directory.Exists(SettingsHandler.MapDownloadToolSettings.MapCompressor.MapsFolder))
			{
				SettingsHandler.MapDownloadToolSettings.MapCompressor.MapsFolder = SettingsHandler.MapDownloadToolSettings.MapCompressor.PartsFolder;
			}

			// get the map file to compress from the user
			string map_file_path = Util.GetSingleFileOpenDialog(SettingsHandler.MapDownloadToolSettings.MapCompressor.MapsFolder,
				"(*.map,*.yelo) Halo CE Map Cache|*.yelo;*.map",
				"Select Map Cache");

			if (map_file_path.Length == 0)
				return;

			SettingsHandler.MapDownloadToolSettings.MapCompressor.MapsFolder = Path.GetDirectoryName(map_file_path);

			if (map_file_path != "")
			{
				MapCompressor.MapCompressorArgs compressorArgs = new MapCompressor.MapCompressorArgs
				{
					Map = map_file_path,
					PartsFolder = SettingsHandler.MapDownloadToolSettings.MapCompressor.PartsFolder,
					DefinitionsFolder = SettingsHandler.MapDownloadToolSettings.MapCompressor.DefinitionsFolder,
					EncryptArchive = SettingsHandler.MapDownloadToolSettings.MapCompressor.EncryptArchive,
					ServerPassword = SettingsHandler.MapDownloadToolSettings.MapCompressor.ServerPassword
				};

				// create a progress controller to run the command in the background
				ProgressController progress_controller = new ProgressController(MapCompressor.CompressMap, null);

				progress_controller.WorkStarted += new EventHandler<ProgressController.ProgressControllerEventArgs>(ProgressControllerWorkStarted);
				progress_controller.WorkEnded += new EventHandler<ProgressController.ProgressControllerEventArgs>(ProgressControllerWorkEnded);

				progress_controller.Start(compressorArgs);

				m_progress_controllers.Add(progress_controller);
			}
		}

		void ProgressControllerWorkStarted(object sender, ProgressController.ProgressControllerEventArgs e)
		{
			// disable the control as work is in progress
			MapCompressorToolTip.Active = true;
			this.Enabled = false;
		}

		void ProgressControllerWorkEnded(object sender, ProgressController.ProgressControllerEventArgs e)
		{
			ProgressController controller = sender as ProgressController;

			if (!m_progress_controllers.Contains(controller)) throw new Exception("A ProgressController was not added to the progress controller list");

			// work is completed, remove the event callbacks and reenable the control
			controller.WorkEnded -= new EventHandler<ProgressController.ProgressControllerEventArgs>(ProgressControllerWorkEnded); 
			controller.WorkStarted -= new EventHandler<ProgressController.ProgressControllerEventArgs>(ProgressControllerWorkStarted);

			m_progress_controllers.Remove(controller);

			// enable the control as the work has been completed
			MapCompressorToolTip.Active = false;
			this.Enabled = true;
		}
	}
}
