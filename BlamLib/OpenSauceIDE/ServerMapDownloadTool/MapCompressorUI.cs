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

			PartsFolderTextBox.DataBindings.Add(new Binding("Text", m_map_compressor_globals, "PartsFolder", true, DataSourceUpdateMode.OnPropertyChanged));
			DefinitionsFolderTextBox.DataBindings.Add(new Binding("Text", m_map_compressor_globals, "DefinitionsFolder", true, DataSourceUpdateMode.OnPropertyChanged));
			EncryptArchiveCheckbox.DataBindings.Add(new Binding("Checked", m_map_compressor_globals, "EncryptArchive", true, DataSourceUpdateMode.OnPropertyChanged));
			ServerPasswordTextBox.DataBindings.Add(new Binding("Text", m_map_compressor_globals, "ServerPassword", true, DataSourceUpdateMode.OnPropertyChanged));

			RecentPartsFolderComboBox.DisplayMember = "Value";
			RecentPartsFolderComboBox.ValueMember = "Value";
			RecentPartsFolderComboBox.DataSource = SettingsHandler.MapDownloadToolSettings.MapCompressor.RecentPartsFolders;
			RecentPartsFolderComboBox.DataBindings.Add(new Binding("SelectedValue", m_map_compressor_globals, "PartsFolder", true, DataSourceUpdateMode.OnPropertyChanged));

			RecentDefinitionsFolderComboBox.DisplayMember = "Value";
			RecentDefinitionsFolderComboBox.ValueMember = "Value";
			RecentDefinitionsFolderComboBox.DataSource = SettingsHandler.MapDownloadToolSettings.MapCompressor.RecentDefinitionsFolders;
			RecentDefinitionsFolderComboBox.DataBindings.Add(new Binding("SelectedValue", m_map_compressor_globals, "DefinitionsFolder", true, DataSourceUpdateMode.OnPropertyChanged));

			MapCompressorToolTip.SetToolTip(CompressMapButton, "");

			m_map_compressor_globals.PropertyChanged += new PropertyChangedEventHandler(MapCompressorGlobals_PropertyChanged);
		}
	}
}
