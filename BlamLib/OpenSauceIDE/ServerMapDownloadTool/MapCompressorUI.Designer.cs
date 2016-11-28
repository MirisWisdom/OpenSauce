namespace OpenSauceIDE.ServerMapDownloadTool
{
	partial class MapCompressorUI
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
			this.components = new System.ComponentModel.Container();
			this.MapCompressorGroupbox = new System.Windows.Forms.GroupBox();
			this.DefinitionsFolderBrowseButton = new System.Windows.Forms.Button();
			this.DefinitionsFolderTextBox = new System.Windows.Forms.TextBox();
			this.DefinitionsFolderLabel = new System.Windows.Forms.Label();
			this.CompressMapButton = new System.Windows.Forms.Button();
			this.ServerPasswordTextBox = new System.Windows.Forms.TextBox();
			this.ServerPasswordLabel = new System.Windows.Forms.Label();
			this.EncryptArchiveCheckbox = new System.Windows.Forms.CheckBox();
			this.PartsFolderBrowseButton = new System.Windows.Forms.Button();
			this.PartsFolderTextBox = new System.Windows.Forms.TextBox();
			this.PartsFolderLabel = new System.Windows.Forms.Label();
			this.MapCompressorToolTip = new System.Windows.Forms.ToolTip(this.components);
			this.MapCompressorGroupbox.SuspendLayout();
			this.SuspendLayout();
			// 
			// MapCompressorGroupbox
			// 
			this.MapCompressorGroupbox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.MapCompressorGroupbox.Controls.Add(this.DefinitionsFolderBrowseButton);
			this.MapCompressorGroupbox.Controls.Add(this.DefinitionsFolderTextBox);
			this.MapCompressorGroupbox.Controls.Add(this.DefinitionsFolderLabel);
			this.MapCompressorGroupbox.Controls.Add(this.CompressMapButton);
			this.MapCompressorGroupbox.Controls.Add(this.ServerPasswordTextBox);
			this.MapCompressorGroupbox.Controls.Add(this.ServerPasswordLabel);
			this.MapCompressorGroupbox.Controls.Add(this.EncryptArchiveCheckbox);
			this.MapCompressorGroupbox.Controls.Add(this.PartsFolderBrowseButton);
			this.MapCompressorGroupbox.Controls.Add(this.PartsFolderTextBox);
			this.MapCompressorGroupbox.Controls.Add(this.PartsFolderLabel);
			this.MapCompressorGroupbox.ForeColor = System.Drawing.Color.LightGreen;
			this.MapCompressorGroupbox.Location = new System.Drawing.Point(0, 3);
			this.MapCompressorGroupbox.Name = "MapCompressorGroupbox";
			this.MapCompressorGroupbox.Size = new System.Drawing.Size(624, 100);
			this.MapCompressorGroupbox.TabIndex = 0;
			this.MapCompressorGroupbox.TabStop = false;
			this.MapCompressorGroupbox.Text = "Map Compressor";
			// 
			// DefinitionsFolderBrowseButton
			// 
			this.DefinitionsFolderBrowseButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.DefinitionsFolderBrowseButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.DefinitionsFolderBrowseButton.Location = new System.Drawing.Point(590, 45);
			this.DefinitionsFolderBrowseButton.Name = "DefinitionsFolderBrowseButton";
			this.DefinitionsFolderBrowseButton.Size = new System.Drawing.Size(28, 23);
			this.DefinitionsFolderBrowseButton.TabIndex = 13;
			this.DefinitionsFolderBrowseButton.Text = "...";
			this.DefinitionsFolderBrowseButton.UseVisualStyleBackColor = true;
			this.DefinitionsFolderBrowseButton.ClientSizeChanged += new System.EventHandler(this.DefinitionsFolderBrowseButton_Click);
			this.DefinitionsFolderBrowseButton.Click += new System.EventHandler(this.DefinitionsFolderBrowseButton_Click);
			// 
			// DefinitionsFolderTextBox
			// 
			this.DefinitionsFolderTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.DefinitionsFolderTextBox.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.DefinitionsFolderTextBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.DefinitionsFolderTextBox.Cursor = System.Windows.Forms.Cursors.IBeam;
			this.DefinitionsFolderTextBox.ForeColor = System.Drawing.Color.LightGreen;
			this.DefinitionsFolderTextBox.Location = new System.Drawing.Point(112, 47);
			this.DefinitionsFolderTextBox.Name = "DefinitionsFolderTextBox";
			this.DefinitionsFolderTextBox.Size = new System.Drawing.Size(472, 20);
			this.DefinitionsFolderTextBox.TabIndex = 12;
			// 
			// DefinitionsFolderLabel
			// 
			this.DefinitionsFolderLabel.Location = new System.Drawing.Point(6, 45);
			this.DefinitionsFolderLabel.Name = "DefinitionsFolderLabel";
			this.DefinitionsFolderLabel.Size = new System.Drawing.Size(100, 23);
			this.DefinitionsFolderLabel.TabIndex = 11;
			this.DefinitionsFolderLabel.Text = "Definitions Folder:";
			this.DefinitionsFolderLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// CompressMapButton
			// 
			this.CompressMapButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.CompressMapButton.Enabled = false;
			this.CompressMapButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.CompressMapButton.ForeColor = System.Drawing.SystemColors.ControlDark;
			this.CompressMapButton.Location = new System.Drawing.Point(508, 71);
			this.CompressMapButton.Name = "CompressMapButton";
			this.CompressMapButton.Size = new System.Drawing.Size(110, 23);
			this.CompressMapButton.TabIndex = 8;
			this.CompressMapButton.Text = "Compress Map";
			this.CompressMapButton.UseVisualStyleBackColor = true;
			this.CompressMapButton.Click += new System.EventHandler(this.CompressMapButton_Click);
			// 
			// ServerPasswordTextBox
			// 
			this.ServerPasswordTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.ServerPasswordTextBox.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.ServerPasswordTextBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.ServerPasswordTextBox.Enabled = false;
			this.ServerPasswordTextBox.ForeColor = System.Drawing.Color.LightGreen;
			this.ServerPasswordTextBox.Location = new System.Drawing.Point(426, 73);
			this.ServerPasswordTextBox.MaxLength = 8;
			this.ServerPasswordTextBox.Name = "ServerPasswordTextBox";
			this.ServerPasswordTextBox.Size = new System.Drawing.Size(76, 20);
			this.ServerPasswordTextBox.TabIndex = 7;
			// 
			// ServerPasswordLabel
			// 
			this.ServerPasswordLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.ServerPasswordLabel.AutoSize = true;
			this.ServerPasswordLabel.Location = new System.Drawing.Point(330, 76);
			this.ServerPasswordLabel.Name = "ServerPasswordLabel";
			this.ServerPasswordLabel.Size = new System.Drawing.Size(90, 13);
			this.ServerPasswordLabel.TabIndex = 6;
			this.ServerPasswordLabel.Text = "Server Password:";
			// 
			// EncryptArchiveCheckbox
			// 
			this.EncryptArchiveCheckbox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.EncryptArchiveCheckbox.AutoSize = true;
			this.EncryptArchiveCheckbox.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
			this.EncryptArchiveCheckbox.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.EncryptArchiveCheckbox.Location = new System.Drawing.Point(220, 74);
			this.EncryptArchiveCheckbox.Name = "EncryptArchiveCheckbox";
			this.EncryptArchiveCheckbox.Size = new System.Drawing.Size(101, 17);
			this.EncryptArchiveCheckbox.TabIndex = 5;
			this.EncryptArchiveCheckbox.Text = "Encrypt Archive:";
			this.EncryptArchiveCheckbox.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			this.EncryptArchiveCheckbox.UseVisualStyleBackColor = true;
			// 
			// PartsFolderBrowseButton
			// 
			this.PartsFolderBrowseButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.PartsFolderBrowseButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.PartsFolderBrowseButton.Location = new System.Drawing.Point(590, 16);
			this.PartsFolderBrowseButton.Name = "PartsFolderBrowseButton";
			this.PartsFolderBrowseButton.Size = new System.Drawing.Size(28, 23);
			this.PartsFolderBrowseButton.TabIndex = 3;
			this.PartsFolderBrowseButton.Text = "...";
			this.PartsFolderBrowseButton.UseVisualStyleBackColor = true;
			this.PartsFolderBrowseButton.Click += new System.EventHandler(this.PartsFolderBrowseButton_Click);
			// 
			// PartsFolderTextBox
			// 
			this.PartsFolderTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.PartsFolderTextBox.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.PartsFolderTextBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.PartsFolderTextBox.Cursor = System.Windows.Forms.Cursors.IBeam;
			this.PartsFolderTextBox.ForeColor = System.Drawing.Color.LightGreen;
			this.PartsFolderTextBox.Location = new System.Drawing.Point(112, 18);
			this.PartsFolderTextBox.Name = "PartsFolderTextBox";
			this.PartsFolderTextBox.Size = new System.Drawing.Size(472, 20);
			this.PartsFolderTextBox.TabIndex = 1;
			// 
			// PartsFolderLabel
			// 
			this.PartsFolderLabel.Location = new System.Drawing.Point(6, 16);
			this.PartsFolderLabel.Name = "PartsFolderLabel";
			this.PartsFolderLabel.Size = new System.Drawing.Size(100, 23);
			this.PartsFolderLabel.TabIndex = 0;
			this.PartsFolderLabel.Text = "Parts Folder:";
			this.PartsFolderLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// MapCompressorToolTip
			// 
			this.MapCompressorToolTip.Active = false;
			this.MapCompressorToolTip.AutomaticDelay = 50;
			// 
			// MapCompressorUI
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.AutoValidate = System.Windows.Forms.AutoValidate.EnableAllowFocusChange;
			this.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.Controls.Add(this.MapCompressorGroupbox);
			this.ForeColor = System.Drawing.Color.LightGreen;
			this.Margin = new System.Windows.Forms.Padding(0);
			this.MinimumSize = new System.Drawing.Size(624, 103);
			this.Name = "MapCompressorUI";
			this.Size = new System.Drawing.Size(624, 103);
			this.MapCompressorGroupbox.ResumeLayout(false);
			this.MapCompressorGroupbox.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.GroupBox MapCompressorGroupbox;
		private System.Windows.Forms.Button PartsFolderBrowseButton;
		private System.Windows.Forms.TextBox PartsFolderTextBox;
		private System.Windows.Forms.Label PartsFolderLabel;
		private System.Windows.Forms.CheckBox EncryptArchiveCheckbox;
		private System.Windows.Forms.TextBox ServerPasswordTextBox;
		private System.Windows.Forms.Label ServerPasswordLabel;
		private System.Windows.Forms.Button CompressMapButton;
		private System.Windows.Forms.ToolTip MapCompressorToolTip;
		private System.Windows.Forms.Button DefinitionsFolderBrowseButton;
		private System.Windows.Forms.TextBox DefinitionsFolderTextBox;
		private System.Windows.Forms.Label DefinitionsFolderLabel;
	}
}
