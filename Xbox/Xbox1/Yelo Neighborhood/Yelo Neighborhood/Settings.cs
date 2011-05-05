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
    public partial class Settings : Form
    {
        public Settings()
        {
            InitializeComponent();

            checkAutoDiscover.Checked = Properties.Settings.Default.AutoDiscover;
            txtIP.Text = Properties.Settings.Default.XBoxIP;
        }

        private void checkAutoDiscover_CheckedChanged(object sender, EventArgs e)
        { txtIP.Enabled = !checkAutoDiscover.Checked; }

        private void cmdConnect_Click(object sender, EventArgs e)
        {
            Properties.Settings.Default.AutoDiscover = checkAutoDiscover.Checked;
            Properties.Settings.Default.XBoxIP = txtIP.Text;
            Properties.Settings.Default.Save();

            Hide();
            DialogResult = DialogResult.OK;

            if (checkAutoDiscover.Checked) Program.FindXBox();
            else Program.FindXBox(txtIP.Text);
        }
    }
}
