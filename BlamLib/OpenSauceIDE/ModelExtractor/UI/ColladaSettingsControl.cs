using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace OpenSauceIDE.ModelExtractor.UI
{
    public partial class ColladaSettingsControl : UserControl
    {
        private readonly BindingList<Tuple<string, ModelExtractorBitmapFormat>> mBitmapFormatList = new BindingList<Tuple<string, ModelExtractorBitmapFormat>>()
        {
            new Tuple<string, ModelExtractorBitmapFormat>("Targa (*.tga)", ModelExtractorBitmapFormat.Tga),
            new Tuple<string, ModelExtractorBitmapFormat>("TIFF (*.tif)", ModelExtractorBitmapFormat.Tiff),
            new Tuple<string, ModelExtractorBitmapFormat>("Bitmap (*.bmp)", ModelExtractorBitmapFormat.Bmp),
            new Tuple<string, ModelExtractorBitmapFormat>("JPEG (*.jpg)", ModelExtractorBitmapFormat.Jpg),
            new Tuple<string, ModelExtractorBitmapFormat>("PNG (*.png)", ModelExtractorBitmapFormat.Png),
        };

        public ModelExtractorBitmapFormat BitmapFormat { get { return (ModelExtractorBitmapFormat)mBitmapExtensionComboBox.SelectedValue; } }
        public bool Overwrite { get { return mOverwriteCheckbox.Checked; } }

        /// <summary>   Default constructor. </summary>
        public ColladaSettingsControl()
        {
            InitializeComponent();

            mBitmapExtensionComboBox.DataSource = mBitmapFormatList;
            mBitmapExtensionComboBox.DisplayMember = "Item1";
            mBitmapExtensionComboBox.ValueMember = "Item2";
        }
    }
}
