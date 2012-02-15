using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Yelo_Neighborhood
{
    public partial class NewModule : Form
    {
        public string Filename { get; set; }
        public string BaseAddressString { get; set; }
        public uint BaseAddress { get; set; }

        public NewModule()
        {
            InitializeComponent();
        }

        private void cmdOK_Click(object sender, EventArgs e)
        {
            if (!cboModule.Items.Contains(cboModule.Text))
                cboModule.Items.Add(cboModule.Text);
            Filename = cboModule.Text;

            if (!cboAddress.Items.Contains(cboAddress.Text))
                cboAddress.Items.Add(cboAddress.Text);
            BaseAddressString = cboAddress.Text;

            try { BaseAddress = Convert.ToUInt32(BaseAddressString, 16); }
            catch(Exception ex)
            { 
                MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            DialogResult = DialogResult.OK;
        }

        private void cmdBrowse_Click(object sender, EventArgs e)
        {
            if (OFD.ShowDialog() == DialogResult.OK)
				cboModule.Text = OFD.FileName;
        }
    }
}
