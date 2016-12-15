using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Windows.Forms;

namespace Yelo_Neighborhood
{
    public partial class ScriptTool : Form
    {
        public Executable.Script Script
        {
            get { return _script; }
            set
            {
                _script = value;
                txtScript.Lines = value.Code.Split(new char[] { '\n' }, StringSplitOptions.None);
                txtScript.SelectionLength = 0;
                txtScript.SelectionStart = 0;
                txtName.Text = value.Name;
                txtFileType.Text = value.FileType;
            }
        }
        Executable.Script _script;

        public ScriptTool()
        {
            InitializeComponent();
        }

        void cmdSaveClose_Click(object sender, EventArgs e)
        {
            _script.Code = txtScript.Text;
            _script.Name = txtName.Text;
            _script.FileType = txtFileType.Text;
            DialogResult = DialogResult.OK;
        }
    };
}