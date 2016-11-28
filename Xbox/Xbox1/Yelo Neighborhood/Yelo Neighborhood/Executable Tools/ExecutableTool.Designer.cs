namespace Yelo_Neighborhood
{
    partial class ExecutableTool
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ExecutableTool));
            this.label1 = new System.Windows.Forms.Label();
            this.txtExeName = new System.Windows.Forms.TextBox();
            this.cmdSaveClose = new System.Windows.Forms.Button();
            this.txtFilename = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.cmdEditScript = new System.Windows.Forms.Button();
            this.label5 = new System.Windows.Forms.Label();
            this.cmdDeleteScript = new System.Windows.Forms.Button();
            this.cmdNewScript = new System.Windows.Forms.Button();
            this.cboScripts = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(38, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Name:";
            // 
            // txtExeName
            // 
            this.txtExeName.Location = new System.Drawing.Point(56, 12);
            this.txtExeName.Name = "txtExeName";
            this.txtExeName.Size = new System.Drawing.Size(520, 20);
            this.txtExeName.TabIndex = 1;
            // 
            // cmdSaveClose
            // 
            this.cmdSaveClose.Location = new System.Drawing.Point(483, 91);
            this.cmdSaveClose.Name = "cmdSaveClose";
            this.cmdSaveClose.Size = new System.Drawing.Size(93, 23);
            this.cmdSaveClose.TabIndex = 9;
            this.cmdSaveClose.Text = "Save & Close";
            this.cmdSaveClose.UseMnemonic = false;
            this.cmdSaveClose.UseVisualStyleBackColor = true;
            this.cmdSaveClose.Click += new System.EventHandler(this.cmdSaveClose_Click);
            // 
            // txtFilename
            // 
            this.txtFilename.Location = new System.Drawing.Point(70, 38);
            this.txtFilename.Name = "txtFilename";
            this.txtFilename.Size = new System.Drawing.Size(506, 20);
            this.txtFilename.TabIndex = 15;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 41);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(52, 13);
            this.label4.TabIndex = 14;
            this.label4.Text = "Filename:";
            // 
            // cmdEditScript
            // 
            this.cmdEditScript.Enabled = false;
            this.cmdEditScript.Location = new System.Drawing.Point(381, 64);
            this.cmdEditScript.Name = "cmdEditScript";
            this.cmdEditScript.Size = new System.Drawing.Size(60, 21);
            this.cmdEditScript.TabIndex = 20;
            this.cmdEditScript.Tag = "Commands";
            this.cmdEditScript.Text = "Edit";
            this.cmdEditScript.UseVisualStyleBackColor = true;
            this.cmdEditScript.Click += new System.EventHandler(this.cmdEditScript_Click);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(13, 68);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(39, 13);
            this.label5.TabIndex = 19;
            this.label5.Text = "Scripts";
            // 
            // cmdDeleteScript
            // 
            this.cmdDeleteScript.Enabled = false;
            this.cmdDeleteScript.Location = new System.Drawing.Point(447, 64);
            this.cmdDeleteScript.Name = "cmdDeleteScript";
            this.cmdDeleteScript.Size = new System.Drawing.Size(60, 21);
            this.cmdDeleteScript.TabIndex = 18;
            this.cmdDeleteScript.Tag = "Commands";
            this.cmdDeleteScript.Text = "Delete";
            this.cmdDeleteScript.UseVisualStyleBackColor = true;
            this.cmdDeleteScript.Click += new System.EventHandler(this.cmdDeleteScript_Click);
            // 
            // cmdNewScript
            // 
            this.cmdNewScript.Location = new System.Drawing.Point(513, 64);
            this.cmdNewScript.Name = "cmdNewScript";
            this.cmdNewScript.Size = new System.Drawing.Size(60, 21);
            this.cmdNewScript.TabIndex = 17;
            this.cmdNewScript.Tag = "Commands";
            this.cmdNewScript.Text = "New";
            this.cmdNewScript.UseVisualStyleBackColor = true;
            this.cmdNewScript.Click += new System.EventHandler(this.cmdNewScript_Click);
            // 
            // cboScripts
            // 
            this.cboScripts.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboScripts.Enabled = false;
            this.cboScripts.FormattingEnabled = true;
            this.cboScripts.Location = new System.Drawing.Point(58, 64);
            this.cboScripts.Name = "cboScripts";
            this.cboScripts.Size = new System.Drawing.Size(317, 21);
            this.cboScripts.TabIndex = 16;
            // 
            // ExecutableTool
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(588, 127);
            this.Controls.Add(this.cmdEditScript);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.cmdDeleteScript);
            this.Controls.Add(this.cmdNewScript);
            this.Controls.Add(this.cboScripts);
            this.Controls.Add(this.txtFilename);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.cmdSaveClose);
            this.Controls.Add(this.txtExeName);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "ExecutableTool";
            this.Text = "Executable Tool";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtExeName;
        private System.Windows.Forms.Button cmdSaveClose;
        private System.Windows.Forms.TextBox txtFilename;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button cmdEditScript;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button cmdDeleteScript;
        private System.Windows.Forms.Button cmdNewScript;
        private System.Windows.Forms.ComboBox cboScripts;
    }
}