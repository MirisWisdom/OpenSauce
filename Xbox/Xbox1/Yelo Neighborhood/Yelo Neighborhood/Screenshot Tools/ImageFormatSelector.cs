using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Forms;
using System.Drawing.Imaging;

namespace Yelo_Neighborhood
{
    public partial class ImageFormatSelector : Form
    {
        public ImageFormat ImageFormat { get { return (ImageFormat)cboFormats.SelectedItem; } set { cboFormats.SelectedItem = value; } }

        public ImageFormatSelector()
        {
            InitializeComponent();
            cboFormats.Items.Add(ImageFormat.Png);
            cboFormats.Items.Add(ImageFormat.Jpeg);
            cboFormats.Items.Add(ImageFormat.Gif);
            cboFormats.Items.Add(ImageFormat.Bmp);
            cboFormats.SelectedIndex = 0;
        }

        void cmdOK_Click(object sender, EventArgs e)
        { DialogResult = DialogResult.OK; }
    };
}