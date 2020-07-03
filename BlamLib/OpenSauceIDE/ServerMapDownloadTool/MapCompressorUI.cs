using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace OpenSauceIDE.ServerMapDownloadTool
{
	public partial class MapCompressorUI : UserControl
	{
		public MapCompressorUI()
		{
			InitializeComponent();

			PartsFolderTextBox.DataBindings.Add(new Binding("Text", SettingsHandler.MapDownloadToolSettings.MapCompressor, "PartsFolder", true, DataSourceUpdateMode.OnPropertyChanged));
			DefinitionsFolderTextBox.DataBindings.Add(new Binding("Text", SettingsHandler.MapDownloadToolSettings.MapCompressor, "DefinitionsFolder", true, DataSourceUpdateMode.OnPropertyChanged));
			EncryptArchiveCheckbox.DataBindings.Add(new Binding("Checked", SettingsHandler.MapDownloadToolSettings.MapCompressor, "EncryptArchive", true, DataSourceUpdateMode.OnPropertyChanged));
			ServerPasswordTextBox.DataBindings.Add(new Binding("Text", SettingsHandler.MapDownloadToolSettings.MapCompressor, "ServerPassword", true, DataSourceUpdateMode.OnPropertyChanged));

			MapCompressorToolTip.SetToolTip(CompressMapButton, "");

			SettingsHandler.MapDownloadToolSettings.MapCompressor.PropertyChanged += new PropertyChangedEventHandler(MapCompressorGlobals_PropertyChanged);
		}
	}
}
