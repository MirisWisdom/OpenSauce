/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using BlamLib.Bitmaps;
using BlamLib.Render.COLLADA;
using OpenSauceIDE.ModelExtractor.Settings;

namespace OpenSauceIDE.ModelExtractor.UI
{
	public partial class ColladaSettingsControl : UserControl
	{
		private readonly BindingList<Tuple<string, AssetFormat>> mBitmapFormatList = new BindingList<Tuple<string, AssetFormat>>()
		{
			new Tuple<string, AssetFormat>("Targa (*.tga)", AssetFormat.tga),
			new Tuple<string, AssetFormat>("TIFF (*.tif)", AssetFormat.tif),
			new Tuple<string, AssetFormat>("Bitmap (*.bmp)", AssetFormat.bmp),
			new Tuple<string, AssetFormat>("JPEG (*.jpg)", AssetFormat.jpg),
			new Tuple<string, AssetFormat>("PNG (*.png)", AssetFormat.png),
		};

		/// <summary>   Default constructor. </summary>
		public ColladaSettingsControl()
		{
			InitializeComponent();

			mBitmapExtensionComboBox.DataSource = mBitmapFormatList;
			mBitmapExtensionComboBox.DisplayMember = "Item1";
			mBitmapExtensionComboBox.ValueMember = "Item2";
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Attaches to the given collada settings. </summary>
		///
		/// <param name="colladaSettings">	The collada settings. </param>
		public void Attach(IExtractorColladaSettings colladaSettings)
		{
			mOverwriteCheckbox.DataBindings.Add(new Binding("Checked", colladaSettings, "Overwrite"));
			mBitmapExtensionComboBox.DataBindings.Add(new Binding("SelectedValue", colladaSettings, "BitmapFormat"));
		}
	}
}
