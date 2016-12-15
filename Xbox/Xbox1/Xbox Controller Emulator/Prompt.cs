/*
	OpenSauceBox: SDK for Xbox User Modding

	See license\Xbox\Xbox for specific license information
*/
using System;
using System.Windows.Forms;

namespace Xbox_Controller_Emulator
{
    public partial class Prompt : Form
    {
        public string DebugName;

        public Prompt()
        {
            InitializeComponent();
        }

        void txtDebugName_KeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter && txtDebugName.Text.Length > 0)
            {
                DebugName = txtDebugName.Text;
                Dispose();
            }
        }
    };
}