using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace Yelo_Neighborhood
{
    public partial class ExecutableTool : Form
    {
        public Executable Executable 
        { 
            get { return _executable; }
            set
            {
                _executable = value;
                txtExeName.Text = Executable.Name;
                txtFilename.Text = Executable.Filename;

                cboScripts.Items.Clear();
                cboScripts.Items.AddRange(value.Scripts.ToArray());
                SetScriptEnabled(cboScripts.Items.Count > 0);
                if (cboScripts.Enabled) cboScripts.SelectedIndex = 0;
            }
        }
        Executable _executable;

        public ExecutableTool()
        {
            InitializeComponent();
        }

        private void SetScriptEnabled(bool enabled)
        {
            cmdEditScript.Enabled = enabled;
            cmdDeleteScript.Enabled = enabled;
            cboScripts.Enabled = enabled;
        }

        private void cmdSaveClose_Click(object sender, EventArgs e)
        {
            Executable.Filename = txtFilename.Text;
            Executable.Name = txtExeName.Text;
            DialogResult = DialogResult.OK; 
        }

        private void cmdEditScript_Click(object sender, EventArgs e)
        {
            Executable.Script cmd = (Executable.Script)cboScripts.SelectedItem;
            ScriptTool ST = new ScriptTool();
            ST.Script = cmd;
            ST.ShowDialog();
        }

        private void cmdDeleteScript_Click(object sender, EventArgs e)
        {
            Executable.Script cmd = (Executable.Script)cboScripts.SelectedItem;
            _executable.Scripts.Remove(cmd);
            cboScripts.Items.Remove(cmd);
        }

        private void cmdNewScript_Click(object sender, EventArgs e)
        {
            Executable.Script cmd = new Executable.Script();
            ScriptTool ST = new ScriptTool();
            ST.Script = cmd;
            if (ST.ShowDialog() == DialogResult.OK)
            {
                _executable.Scripts.Add(cmd);
                cboScripts.Items.Add(cmd);
                cboScripts.SelectedItem = cmd;
                SetScriptEnabled(true);
            }
        }
    }
}
