namespace OpenSauceIDE.Aether.Controls
{
    partial class ExportPane
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
			this.ExportGroupBox = new System.Windows.Forms.GroupBox();
			this.ExportButton = new System.Windows.Forms.Button();
			this.PathsPanel = new System.Windows.Forms.Panel();
			this.PathsColladaOptionsLabel = new System.Windows.Forms.Label();
			this.PathsOutputPathLabel = new System.Windows.Forms.Label();
			this.PathsLabel = new System.Windows.Forms.Label();
			this.PathsColladaFileLabel = new System.Windows.Forms.Label();
			this.PathsColladaOptionsCheckList = new System.Windows.Forms.CheckedListBox();
			this.PathsOutputFolderText = new System.Windows.Forms.TextBox();
			this.PathsColladaFileText = new System.Windows.Forms.TextBox();
			this.BitmapExportPanel = new System.Windows.Forms.Panel();
			this.BitmapExportFormatLabel = new System.Windows.Forms.Label();
			this.BitmapExportFormatCombo = new System.Windows.Forms.ComboBox();
			this.BitmapExportLightmapsCheck = new System.Windows.Forms.CheckBox();
			this.BitmapExportRadioDiffuse = new System.Windows.Forms.RadioButton();
			this.BitmapExportRadioAll = new System.Windows.Forms.RadioButton();
			this.BitmapExportRadioNone = new System.Windows.Forms.RadioButton();
			this.BitmapExportLabel = new System.Windows.Forms.Label();
			this.OtherPanel = new System.Windows.Forms.Panel();
			this.OtherLabel = new System.Windows.Forms.Label();
			this.OtherCheckList = new System.Windows.Forms.CheckedListBox();
			this.ExportGroupBox.SuspendLayout();
			this.PathsPanel.SuspendLayout();
			this.BitmapExportPanel.SuspendLayout();
			this.OtherPanel.SuspendLayout();
			this.SuspendLayout();
			// 
			// ExportGroupBox
			// 
			this.ExportGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.ExportGroupBox.Controls.Add(this.ExportButton);
			this.ExportGroupBox.Controls.Add(this.PathsPanel);
			this.ExportGroupBox.Controls.Add(this.BitmapExportPanel);
			this.ExportGroupBox.Controls.Add(this.OtherPanel);
			this.ExportGroupBox.ForeColor = System.Drawing.Color.LightGreen;
			this.ExportGroupBox.Location = new System.Drawing.Point(0, 0);
			this.ExportGroupBox.Name = "ExportGroupBox";
			this.ExportGroupBox.Size = new System.Drawing.Size(390, 381);
			this.ExportGroupBox.TabIndex = 0;
			this.ExportGroupBox.TabStop = false;
			this.ExportGroupBox.Text = "Export";
			// 
			// ExportButton
			// 
			this.ExportButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.ExportButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.ExportButton.Location = new System.Drawing.Point(265, 348);
			this.ExportButton.Name = "ExportButton";
			this.ExportButton.Size = new System.Drawing.Size(119, 26);
			this.ExportButton.TabIndex = 4;
			this.ExportButton.Text = "Export";
			this.ExportButton.UseVisualStyleBackColor = true;
			// 
			// PathsPanel
			// 
			this.PathsPanel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.PathsPanel.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(100)))), ((int)(((byte)(100)))));
			this.PathsPanel.Controls.Add(this.PathsColladaOptionsLabel);
			this.PathsPanel.Controls.Add(this.PathsOutputPathLabel);
			this.PathsPanel.Controls.Add(this.PathsLabel);
			this.PathsPanel.Controls.Add(this.PathsColladaFileLabel);
			this.PathsPanel.Controls.Add(this.PathsColladaOptionsCheckList);
			this.PathsPanel.Controls.Add(this.PathsOutputFolderText);
			this.PathsPanel.Controls.Add(this.PathsColladaFileText);
			this.PathsPanel.Location = new System.Drawing.Point(6, 19);
			this.PathsPanel.Name = "PathsPanel";
			this.PathsPanel.Size = new System.Drawing.Size(378, 179);
			this.PathsPanel.TabIndex = 3;
			// 
			// PathsColladaOptionsLabel
			// 
			this.PathsColladaOptionsLabel.AutoSize = true;
			this.PathsColladaOptionsLabel.Location = new System.Drawing.Point(4, 68);
			this.PathsColladaOptionsLabel.Name = "PathsColladaOptionsLabel";
			this.PathsColladaOptionsLabel.Size = new System.Drawing.Size(81, 13);
			this.PathsColladaOptionsLabel.TabIndex = 11;
			this.PathsColladaOptionsLabel.Text = "Output Options:";
			// 
			// PathsOutputPathLabel
			// 
			this.PathsOutputPathLabel.AutoSize = true;
			this.PathsOutputPathLabel.Location = new System.Drawing.Point(21, 18);
			this.PathsOutputPathLabel.Name = "PathsOutputPathLabel";
			this.PathsOutputPathLabel.Size = new System.Drawing.Size(64, 13);
			this.PathsOutputPathLabel.TabIndex = 10;
			this.PathsOutputPathLabel.Text = "Output Path";
			// 
			// PathsLabel
			// 
			this.PathsLabel.AutoSize = true;
			this.PathsLabel.Location = new System.Drawing.Point(3, 0);
			this.PathsLabel.Name = "PathsLabel";
			this.PathsLabel.Size = new System.Drawing.Size(34, 13);
			this.PathsLabel.TabIndex = 4;
			this.PathsLabel.Text = "Paths";
			// 
			// PathsColladaFileLabel
			// 
			this.PathsColladaFileLabel.AutoSize = true;
			this.PathsColladaFileLabel.Location = new System.Drawing.Point(10, 44);
			this.PathsColladaFileLabel.Name = "PathsColladaFileLabel";
			this.PathsColladaFileLabel.Size = new System.Drawing.Size(75, 13);
			this.PathsColladaFileLabel.TabIndex = 10;
			this.PathsColladaFileLabel.Text = "COLLADA file:";
			// 
			// PathsColladaOptionsCheckList
			// 
			this.PathsColladaOptionsCheckList.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.PathsColladaOptionsCheckList.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(100)))), ((int)(((byte)(100)))));
			this.PathsColladaOptionsCheckList.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.PathsColladaOptionsCheckList.ColumnWidth = 100;
			this.PathsColladaOptionsCheckList.ForeColor = System.Drawing.Color.LightGreen;
			this.PathsColladaOptionsCheckList.FormattingEnabled = true;
			this.PathsColladaOptionsCheckList.Items.AddRange(new object[] {
            "Portals",
            "Fog Planes"});
			this.PathsColladaOptionsCheckList.Location = new System.Drawing.Point(91, 68);
			this.PathsColladaOptionsCheckList.MultiColumn = true;
			this.PathsColladaOptionsCheckList.Name = "PathsColladaOptionsCheckList";
			this.PathsColladaOptionsCheckList.Size = new System.Drawing.Size(254, 75);
			this.PathsColladaOptionsCheckList.TabIndex = 9;
			// 
			// PathsOutputFolderText
			// 
			this.PathsOutputFolderText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.PathsOutputFolderText.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.PathsOutputFolderText.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.PathsOutputFolderText.ForeColor = System.Drawing.Color.LightGreen;
			this.PathsOutputFolderText.Location = new System.Drawing.Point(91, 16);
			this.PathsOutputFolderText.Name = "PathsOutputFolderText";
			this.PathsOutputFolderText.ReadOnly = true;
			this.PathsOutputFolderText.Size = new System.Drawing.Size(279, 20);
			this.PathsOutputFolderText.TabIndex = 5;
			// 
			// PathsColladaFileText
			// 
			this.PathsColladaFileText.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.PathsColladaFileText.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.PathsColladaFileText.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.PathsColladaFileText.ForeColor = System.Drawing.Color.LightGreen;
			this.PathsColladaFileText.Location = new System.Drawing.Point(91, 42);
			this.PathsColladaFileText.Name = "PathsColladaFileText";
			this.PathsColladaFileText.ReadOnly = true;
			this.PathsColladaFileText.Size = new System.Drawing.Size(255, 20);
			this.PathsColladaFileText.TabIndex = 8;
			// 
			// BitmapExportPanel
			// 
			this.BitmapExportPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.BitmapExportPanel.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(100)))), ((int)(((byte)(100)))));
			this.BitmapExportPanel.Controls.Add(this.BitmapExportFormatLabel);
			this.BitmapExportPanel.Controls.Add(this.BitmapExportFormatCombo);
			this.BitmapExportPanel.Controls.Add(this.BitmapExportLightmapsCheck);
			this.BitmapExportPanel.Controls.Add(this.BitmapExportRadioDiffuse);
			this.BitmapExportPanel.Controls.Add(this.BitmapExportRadioAll);
			this.BitmapExportPanel.Controls.Add(this.BitmapExportRadioNone);
			this.BitmapExportPanel.Controls.Add(this.BitmapExportLabel);
			this.BitmapExportPanel.Location = new System.Drawing.Point(265, 204);
			this.BitmapExportPanel.Name = "BitmapExportPanel";
			this.BitmapExportPanel.Size = new System.Drawing.Size(119, 138);
			this.BitmapExportPanel.TabIndex = 2;
			// 
			// BitmapExportFormatLabel
			// 
			this.BitmapExportFormatLabel.AutoSize = true;
			this.BitmapExportFormatLabel.Location = new System.Drawing.Point(3, 111);
			this.BitmapExportFormatLabel.Name = "BitmapExportFormatLabel";
			this.BitmapExportFormatLabel.Size = new System.Drawing.Size(39, 13);
			this.BitmapExportFormatLabel.TabIndex = 6;
			this.BitmapExportFormatLabel.Text = "Format";
			// 
			// BitmapExportFormatCombo
			// 
			this.BitmapExportFormatCombo.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.BitmapExportFormatCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.BitmapExportFormatCombo.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.BitmapExportFormatCombo.ForeColor = System.Drawing.Color.LightGreen;
			this.BitmapExportFormatCombo.FormattingEnabled = true;
			this.BitmapExportFormatCombo.Items.AddRange(new object[] {
            "DDS",
            "TGA",
            "TIF",
            "PNG",
            "BMP",
            "JPG"});
			this.BitmapExportFormatCombo.Location = new System.Drawing.Point(48, 108);
			this.BitmapExportFormatCombo.Name = "BitmapExportFormatCombo";
			this.BitmapExportFormatCombo.Size = new System.Drawing.Size(63, 21);
			this.BitmapExportFormatCombo.TabIndex = 5;
			// 
			// BitmapExportLightmapsCheck
			// 
			this.BitmapExportLightmapsCheck.AutoSize = true;
			this.BitmapExportLightmapsCheck.Checked = true;
			this.BitmapExportLightmapsCheck.CheckState = System.Windows.Forms.CheckState.Checked;
			this.BitmapExportLightmapsCheck.Location = new System.Drawing.Point(6, 85);
			this.BitmapExportLightmapsCheck.Name = "BitmapExportLightmapsCheck";
			this.BitmapExportLightmapsCheck.Size = new System.Drawing.Size(74, 17);
			this.BitmapExportLightmapsCheck.TabIndex = 4;
			this.BitmapExportLightmapsCheck.Text = "Lightmaps";
			this.BitmapExportLightmapsCheck.UseVisualStyleBackColor = true;
			// 
			// BitmapExportRadioDiffuse
			// 
			this.BitmapExportRadioDiffuse.AutoSize = true;
			this.BitmapExportRadioDiffuse.Location = new System.Drawing.Point(6, 62);
			this.BitmapExportRadioDiffuse.Name = "BitmapExportRadioDiffuse";
			this.BitmapExportRadioDiffuse.Size = new System.Drawing.Size(82, 17);
			this.BitmapExportRadioDiffuse.TabIndex = 3;
			this.BitmapExportRadioDiffuse.Text = "Diffuse Only";
			this.BitmapExportRadioDiffuse.UseVisualStyleBackColor = true;
			// 
			// BitmapExportRadioAll
			// 
			this.BitmapExportRadioAll.AutoSize = true;
			this.BitmapExportRadioAll.Checked = true;
			this.BitmapExportRadioAll.Location = new System.Drawing.Point(6, 39);
			this.BitmapExportRadioAll.Name = "BitmapExportRadioAll";
			this.BitmapExportRadioAll.Size = new System.Drawing.Size(36, 17);
			this.BitmapExportRadioAll.TabIndex = 2;
			this.BitmapExportRadioAll.TabStop = true;
			this.BitmapExportRadioAll.Text = "All";
			this.BitmapExportRadioAll.UseVisualStyleBackColor = true;
			// 
			// BitmapExportRadioNone
			// 
			this.BitmapExportRadioNone.AutoSize = true;
			this.BitmapExportRadioNone.Location = new System.Drawing.Point(6, 16);
			this.BitmapExportRadioNone.Name = "BitmapExportRadioNone";
			this.BitmapExportRadioNone.Size = new System.Drawing.Size(51, 17);
			this.BitmapExportRadioNone.TabIndex = 1;
			this.BitmapExportRadioNone.Text = "None";
			this.BitmapExportRadioNone.UseVisualStyleBackColor = true;
			// 
			// BitmapExportLabel
			// 
			this.BitmapExportLabel.AutoSize = true;
			this.BitmapExportLabel.Location = new System.Drawing.Point(3, 0);
			this.BitmapExportLabel.Name = "BitmapExportLabel";
			this.BitmapExportLabel.Size = new System.Drawing.Size(72, 13);
			this.BitmapExportLabel.TabIndex = 0;
			this.BitmapExportLabel.Text = "Bitmap Export";
			// 
			// OtherPanel
			// 
			this.OtherPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.OtherPanel.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(100)))), ((int)(((byte)(100)))));
			this.OtherPanel.Controls.Add(this.OtherLabel);
			this.OtherPanel.Controls.Add(this.OtherCheckList);
			this.OtherPanel.Location = new System.Drawing.Point(111, 322);
			this.OtherPanel.Name = "OtherPanel";
			this.OtherPanel.Size = new System.Drawing.Size(148, 52);
			this.OtherPanel.TabIndex = 1;
			// 
			// OtherLabel
			// 
			this.OtherLabel.AutoSize = true;
			this.OtherLabel.Location = new System.Drawing.Point(3, 0);
			this.OtherLabel.Name = "OtherLabel";
			this.OtherLabel.Size = new System.Drawing.Size(33, 13);
			this.OtherLabel.TabIndex = 2;
			this.OtherLabel.Text = "Other";
			// 
			// OtherCheckList
			// 
			this.OtherCheckList.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.OtherCheckList.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(100)))), ((int)(((byte)(100)))));
			this.OtherCheckList.BorderStyle = System.Windows.Forms.BorderStyle.None;
			this.OtherCheckList.ForeColor = System.Drawing.Color.LightGreen;
			this.OtherCheckList.FormattingEnabled = true;
			this.OtherCheckList.Items.AddRange(new object[] {
            "Overwrite",
            "Maya Export"});
			this.OtherCheckList.Location = new System.Drawing.Point(6, 16);
			this.OtherCheckList.Name = "OtherCheckList";
			this.OtherCheckList.Size = new System.Drawing.Size(139, 30);
			this.OtherCheckList.TabIndex = 0;
			// 
			// ExportPane
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.Controls.Add(this.ExportGroupBox);
			this.ForeColor = System.Drawing.Color.LightGreen;
			this.MinimumSize = new System.Drawing.Size(390, 381);
			this.Name = "ExportPane";
			this.Size = new System.Drawing.Size(390, 381);
			this.ExportGroupBox.ResumeLayout(false);
			this.PathsPanel.ResumeLayout(false);
			this.PathsPanel.PerformLayout();
			this.BitmapExportPanel.ResumeLayout(false);
			this.BitmapExportPanel.PerformLayout();
			this.OtherPanel.ResumeLayout(false);
			this.OtherPanel.PerformLayout();
			this.ResumeLayout(false);

        }

        #endregion

		private System.Windows.Forms.GroupBox ExportGroupBox;
        private System.Windows.Forms.Panel OtherPanel;
        private System.Windows.Forms.Label OtherLabel;
        private System.Windows.Forms.CheckedListBox OtherCheckList;
        private System.Windows.Forms.Panel BitmapExportPanel;
        private System.Windows.Forms.RadioButton BitmapExportRadioAll;
        private System.Windows.Forms.RadioButton BitmapExportRadioNone;
        private System.Windows.Forms.Label BitmapExportLabel;
        private System.Windows.Forms.ComboBox BitmapExportFormatCombo;
        private System.Windows.Forms.CheckBox BitmapExportLightmapsCheck;
        private System.Windows.Forms.RadioButton BitmapExportRadioDiffuse;
        private System.Windows.Forms.Label BitmapExportFormatLabel;
        private System.Windows.Forms.Panel PathsPanel;
        private System.Windows.Forms.Label PathsLabel;
        private System.Windows.Forms.TextBox PathsOutputFolderText;
        private System.Windows.Forms.Label PathsOutputPathLabel;
		private System.Windows.Forms.Button ExportButton;
        private System.Windows.Forms.CheckedListBox PathsColladaOptionsCheckList;
        private System.Windows.Forms.TextBox PathsColladaFileText;
        private System.Windows.Forms.Label PathsColladaOptionsLabel;
        private System.Windows.Forms.Label PathsColladaFileLabel;
    }
}
