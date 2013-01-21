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
		private class MapCompressorGlobalsClass : INotifyPropertyChanged
		{
			#region Fields
			public MapCompressor.MapCompressorArguments MapCompressorArguments = new MapCompressor.MapCompressorArguments();

			public static readonly DictionarySizeWrapper[] DictionarySizeMap =
			{
				new DictionarySizeWrapper(65536, "64KB"),
				new DictionarySizeWrapper(1048576, "1MB"),
				new DictionarySizeWrapper(1048576 * 2, "2MB"),
				new DictionarySizeWrapper(1048576 * 3, "3MB"),
				new DictionarySizeWrapper(1048576 * 4, "4MB"),
				new DictionarySizeWrapper(1048576 * 6, "6MB"),
				new DictionarySizeWrapper(1048576 * 8, "8MB"),
				new DictionarySizeWrapper(1048576 * 12, "12MB"),
				new DictionarySizeWrapper(1048576 * 16, "16MB"),
				new DictionarySizeWrapper(1048576 * 24, "24MB"),
				new DictionarySizeWrapper(1048576 * 32, "32MB"),
				new DictionarySizeWrapper(1048576 * 48, "48MB"),
				new DictionarySizeWrapper(1048576 * 64, "64MB"),
			};
			#endregion

			#region Properties
			public string PartsFolder
			{
				get { return MapCompressorArguments.PartsFolder; }
				set
				{
					MapCompressorArguments.PartsFolder = value;
					OnPropertyChanged("PartsFolder");
				}
			}

			public string DefinitionsFolder
			{
				get { return MapCompressorArguments.DefinitionsFolder; }
				set
				{
					MapCompressorArguments.DefinitionsFolder = value;
					OnPropertyChanged("DefinitionsFolder");
				}
			}

			public bool EncryptArchive
			{
				get { return MapCompressorArguments.EncryptArchive; }
				set
				{
					MapCompressorArguments.EncryptArchive = value;
					OnPropertyChanged("EncryptArchive");
				}
			}

			public string ServerPassword
			{
				get { return MapCompressorArguments.ServerPassword; }
				set
				{
					MapCompressorArguments.ServerPassword = value;
					OnPropertyChanged("ServerPassword");
				}
			}
			#endregion

			#region Events
			public event PropertyChangedEventHandler PropertyChanged;

			private void OnPropertyChanged(string property)
			{
				if (PropertyChanged != null)
					PropertyChanged(this, new PropertyChangedEventArgs(property));
			}
			#endregion

			public MapCompressorGlobalsClass()
			{
				MapCompressorArguments = new MapCompressor.MapCompressorArguments();
			}
		}

		#region Fields
		MapCompressorGlobalsClass m_map_compressor_globals = new MapCompressorGlobalsClass();

		List<ProgressController> m_progress_controllers = new List<ProgressController>();
		#endregion

		bool ValidateVariables()
		{
			// determine if the required data has been supplied by the user
			if (!Directory.Exists(m_map_compressor_globals.PartsFolder)) return false;
			if (!Directory.Exists(m_map_compressor_globals.DefinitionsFolder)) return false;
			if (m_map_compressor_globals.EncryptArchive && (m_map_compressor_globals.ServerPassword == "")) return false;

			return true;
		}

		void PartsFolderBrowseButton_Click(object sender, EventArgs e)
		{
			// get the output directroy from the user
			string directory = Util.GetDirectoryDialog(m_map_compressor_globals.PartsFolder);

			if (directory.Length != 0)
				m_map_compressor_globals.PartsFolder = directory;
		}

		void DefinitionsFolderBrowseButton_Click(object sender, EventArgs e)
		{
			// get the output directroy from the user
			string directory = Util.GetDirectoryDialog(m_map_compressor_globals.DefinitionsFolder);

			if (directory.Length != 0)
				m_map_compressor_globals.DefinitionsFolder = directory;
		}

		void MapCompressorGlobals_PropertyChanged(object sender, PropertyChangedEventArgs e)
		{
			// update the UI according to the bound properties that have changed
			if (e.PropertyName == "PartsFolder")
			{
				if (!Directory.Exists(m_map_compressor_globals.PartsFolder))
					PartsFolderTextBox.ForeColor = Color.Salmon;
				else
					PartsFolderTextBox.ForeColor = Color.LightGreen;
			}
			else if (e.PropertyName == "DefinitionsFolder")
			{
				if (!Directory.Exists(m_map_compressor_globals.DefinitionsFolder))
					DefinitionsFolderTextBox.ForeColor = Color.Salmon;
				else
					DefinitionsFolderTextBox.ForeColor = Color.LightGreen;
			}
			else if (e.PropertyName == "EncryptArchive" || e.PropertyName == "ServerPassword")
			{
				ServerPasswordTextBox.Enabled = m_map_compressor_globals.EncryptArchive;

				if (m_map_compressor_globals.EncryptArchive)
				{
					if (m_map_compressor_globals.ServerPassword == "")
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
			if (!Util.BindingListFind<StringWrapperClass>(SettingsHandler.MapDownloadToolSettings.MapCompressor.RecentPartsFolders,
				delegate(StringWrapperClass element)
				{
					return element.Value.Equals(m_map_compressor_globals.PartsFolder);
				}))
			{
				SettingsHandler.MapDownloadToolSettings.MapCompressor.RecentPartsFolders.Insert(0, new StringWrapperClass(m_map_compressor_globals.PartsFolder));
				if (SettingsHandler.MapDownloadToolSettings.MapCompressor.RecentPartsFolders.Count > 10)
					SettingsHandler.MapDownloadToolSettings.MapCompressor.RecentPartsFolders.RemoveAt(SettingsHandler.MapDownloadToolSettings.MapCompressor.RecentPartsFolders.Count - 1);
				RecentPartsFolderComboBox.SelectedIndex = 0;
				RecentPartsFolderComboBox.UpdateDropDownWidth();
			}
			if (!Util.BindingListFind<StringWrapperClass>(SettingsHandler.MapDownloadToolSettings.MapCompressor.RecentDefinitionsFolders,
				delegate(StringWrapperClass element)
				{
					return element.Value.Equals(m_map_compressor_globals.DefinitionsFolder);
				}))
			{
				SettingsHandler.MapDownloadToolSettings.MapCompressor.RecentDefinitionsFolders.Insert(0, new StringWrapperClass(m_map_compressor_globals.DefinitionsFolder));
				if (SettingsHandler.MapDownloadToolSettings.MapCompressor.RecentDefinitionsFolders.Count > 10)
					SettingsHandler.MapDownloadToolSettings.MapCompressor.RecentDefinitionsFolders.RemoveAt(SettingsHandler.MapDownloadToolSettings.MapCompressor.RecentDefinitionsFolders.Count - 1);
				RecentDefinitionsFolderComboBox.SelectedIndex = 0;
				RecentDefinitionsFolderComboBox.UpdateDropDownWidth();
			}

			if (!Directory.Exists(SettingsHandler.MapDownloadToolSettings.MapCompressor.LastMapsFolder))
				SettingsHandler.MapDownloadToolSettings.MapCompressor.LastMapsFolder = m_map_compressor_globals.PartsFolder;

			// get the map file to compress from the user
			string map_file_path = Util.GetSingleFileOpenDialog(SettingsHandler.MapDownloadToolSettings.MapCompressor.LastMapsFolder,
				"(*.map,*.yelo) Halo CE Map Cache|*.yelo;*.map",
				"Select Map Cache");

			if (map_file_path.Length == 0)
				return;

			SettingsHandler.MapDownloadToolSettings.MapCompressor.LastMapsFolder = Path.GetDirectoryName(map_file_path);

			if (map_file_path != "")
			{
				m_map_compressor_globals.MapCompressorArguments.Map = map_file_path;

				// create a progress controller to run the command in the background
				ProgressController progress_controller = new ProgressController(MapCompressor.CompressMap, null);

				progress_controller.WorkStarted += new EventHandler<ProgressController.ProgressControllerEventArgs>(ProgressControllerWorkStarted);
				progress_controller.WorkEnded += new EventHandler<ProgressController.ProgressControllerEventArgs>(ProgressControllerWorkEnded);
				
				progress_controller.Start(m_map_compressor_globals.MapCompressorArguments);

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
