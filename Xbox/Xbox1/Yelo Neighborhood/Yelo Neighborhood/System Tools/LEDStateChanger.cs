using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Reflection;
using System.Windows.Forms;
using YeloDebug;

namespace Yelo_Neighborhood
{
    partial class LEDStateChanger : Form
    {
        public LEDStateChanger()
        {
            InitializeComponent();
            cboState1.DataSource = Enum.GetValues(typeof(LEDState));
            cboState2.DataSource = Enum.GetValues(typeof(LEDState));
            cboState3.DataSource = Enum.GetValues(typeof(LEDState));
            cboState4.DataSource = Enum.GetValues(typeof(LEDState));
        }

        private void cmdApply_Click(object sender, EventArgs e)
        {
            if (!Program.XBox.Ping()) new Settings().ShowDialog();
            Program.XBox.SetLEDState((LEDState)cboState1.SelectedItem, (LEDState)cboState2.SelectedItem, (LEDState)cboState3.SelectedItem, (LEDState)cboState4.SelectedItem); 
        }
    }
}
