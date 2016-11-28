using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace Yelo_Neighborhood
{
    public partial class NewModule : Form
    {
        public string ModuleName { get; set; }
        public string Filename { get; set; }
        public string BaseAddressString { get; set; }
        public uint BaseAddress { get; set; }
        public int ExecutableIndex { get; set; }

        public NewModule()
        {
            InitializeComponent();
        }

        private void cmdOK_Click(object sender, EventArgs e)
        {
            if (!cboName.Items.Contains(cboName.Text))
                cboName.Items.Add(cboName.Text);
            ModuleName = cboName.Text;

            if (!cboFilename.Items.Contains(cboFilename.Text))
                cboFilename.Items.Add(cboFilename.Text);
            Filename = cboFilename.Text;

            if (!cboAddress.Items.Contains(cboAddress.Text))
                cboAddress.Items.Add(cboAddress.Text);
            BaseAddressString = cboAddress.Text;

            try { BaseAddress = Convert.ToUInt32(BaseAddressString, 16); }
            catch(Exception ex)
            { 
                MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            ExecutableIndex = cboExecutable.SelectedIndex - 1;

            DialogResult = DialogResult.OK;
        }

        private void cmdBrowse_Click(object sender, EventArgs e)
        {
            if (OFD.ShowDialog() == DialogResult.OK)
            {
                cboFilename.Text = OFD.FileName;
                cboName.Text = Path.GetFileNameWithoutExtension(OFD.FileName);
            }
        }

        private void NewModule_Shown(object sender, EventArgs e)
        {
            cboExecutable.Items.Clear();
            cboExecutable.Items.Add("Any");
            cboExecutable.Items.AddRange(Program.Executables.ToArray());
            cboExecutable.SelectedIndex = 0;
        }
    }
}
