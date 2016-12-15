/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿namespace OpenSauceIDE
{
	partial class CheApeApplier
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
			System.Windows.Forms.Label label1;
			System.Windows.Forms.Label label2;
			System.Windows.Forms.Label label3;
			System.Windows.Forms.Label label4;
			System.Windows.Forms.Label label5;
			this.cbEngineVersion = new System.Windows.Forms.ComboBox();
			this.txtPathGuerilla = new System.Windows.Forms.TextBox();
			this.btPathGuerilla = new System.Windows.Forms.Button();
			this.btPathSapien = new System.Windows.Forms.Button();
			this.txtPathSapien = new System.Windows.Forms.TextBox();
			this.btPathTool = new System.Windows.Forms.Button();
			this.txtPathTool = new System.Windows.Forms.TextBox();
			this.btPathOutput = new System.Windows.Forms.Button();
			this.txtPathOutput = new System.Windows.Forms.TextBox();
			this.dlgOpenFile = new System.Windows.Forms.OpenFileDialog();
			this.dlgFolderBrowser = new System.Windows.Forms.FolderBrowserDialog();
			this.btApply = new System.Windows.Forms.Button();
			label1 = new System.Windows.Forms.Label();
			label2 = new System.Windows.Forms.Label();
			label3 = new System.Windows.Forms.Label();
			label4 = new System.Windows.Forms.Label();
			label5 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// label1
			// 
			label1.AutoSize = true;
			label1.ForeColor = System.Drawing.Color.LightGreen;
			label1.Location = new System.Drawing.Point(12, 10);
			label1.Name = "label1";
			label1.Size = new System.Drawing.Size(78, 13);
			label1.TabIndex = 0;
			label1.Text = "Engine Version";
			// 
			// label2
			// 
			label2.AutoSize = true;
			label2.ForeColor = System.Drawing.Color.LightGreen;
			label2.Location = new System.Drawing.Point(12, 39);
			label2.Name = "label2";
			label2.Size = new System.Drawing.Size(67, 13);
			label2.TabIndex = 2;
			label2.Text = "Guerilla Path";
			// 
			// label3
			// 
			label3.AutoSize = true;
			label3.ForeColor = System.Drawing.Color.LightGreen;
			label3.Location = new System.Drawing.Point(12, 68);
			label3.Name = "label3";
			label3.Size = new System.Drawing.Size(65, 13);
			label3.TabIndex = 5;
			label3.Text = "Sapien Path";
			// 
			// label4
			// 
			label4.AutoSize = true;
			label4.ForeColor = System.Drawing.Color.LightGreen;
			label4.Location = new System.Drawing.Point(12, 97);
			label4.Name = "label4";
			label4.Size = new System.Drawing.Size(53, 13);
			label4.TabIndex = 8;
			label4.Text = "Tool Path";
			// 
			// label5
			// 
			label5.AutoSize = true;
			label5.ForeColor = System.Drawing.Color.LightGreen;
			label5.Location = new System.Drawing.Point(12, 128);
			label5.Name = "label5";
			label5.Size = new System.Drawing.Size(64, 13);
			label5.TabIndex = 11;
			label5.Text = "Output Path";
			// 
			// cbEngineVersion
			// 
			this.cbEngineVersion.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.cbEngineVersion.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cbEngineVersion.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.cbEngineVersion.ForeColor = System.Drawing.Color.LightGreen;
			this.cbEngineVersion.FormattingEnabled = true;
			this.cbEngineVersion.Location = new System.Drawing.Point(96, 6);
			this.cbEngineVersion.Name = "cbEngineVersion";
			this.cbEngineVersion.Size = new System.Drawing.Size(121, 21);
			this.cbEngineVersion.TabIndex = 1;
			// 
			// txtPathGuerilla
			// 
			this.txtPathGuerilla.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.txtPathGuerilla.BackColor = System.Drawing.Color.Gray;
			this.txtPathGuerilla.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.txtPathGuerilla.ForeColor = System.Drawing.Color.LightGreen;
			this.txtPathGuerilla.Location = new System.Drawing.Point(96, 36);
			this.txtPathGuerilla.Name = "txtPathGuerilla";
			this.txtPathGuerilla.ReadOnly = true;
			this.txtPathGuerilla.Size = new System.Drawing.Size(561, 20);
			this.txtPathGuerilla.TabIndex = 3;
			// 
			// btPathGuerilla
			// 
			this.btPathGuerilla.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btPathGuerilla.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btPathGuerilla.ForeColor = System.Drawing.Color.LightGreen;
			this.btPathGuerilla.Location = new System.Drawing.Point(662, 34);
			this.btPathGuerilla.Name = "btPathGuerilla";
			this.btPathGuerilla.Size = new System.Drawing.Size(32, 23);
			this.btPathGuerilla.TabIndex = 4;
			this.btPathGuerilla.Text = "...";
			this.btPathGuerilla.UseVisualStyleBackColor = true;
			this.btPathGuerilla.Click += new System.EventHandler(this.OnPathGuerilla);
			// 
			// btPathSapien
			// 
			this.btPathSapien.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btPathSapien.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btPathSapien.ForeColor = System.Drawing.Color.LightGreen;
			this.btPathSapien.Location = new System.Drawing.Point(662, 63);
			this.btPathSapien.Name = "btPathSapien";
			this.btPathSapien.Size = new System.Drawing.Size(32, 23);
			this.btPathSapien.TabIndex = 7;
			this.btPathSapien.Text = "...";
			this.btPathSapien.UseVisualStyleBackColor = true;
			this.btPathSapien.Click += new System.EventHandler(this.OnPathSapien);
			// 
			// txtPathSapien
			// 
			this.txtPathSapien.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.txtPathSapien.BackColor = System.Drawing.Color.Gray;
			this.txtPathSapien.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.txtPathSapien.ForeColor = System.Drawing.Color.LightGreen;
			this.txtPathSapien.Location = new System.Drawing.Point(96, 65);
			this.txtPathSapien.Name = "txtPathSapien";
			this.txtPathSapien.ReadOnly = true;
			this.txtPathSapien.Size = new System.Drawing.Size(561, 20);
			this.txtPathSapien.TabIndex = 6;
			// 
			// btPathTool
			// 
			this.btPathTool.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btPathTool.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btPathTool.ForeColor = System.Drawing.Color.LightGreen;
			this.btPathTool.Location = new System.Drawing.Point(662, 92);
			this.btPathTool.Name = "btPathTool";
			this.btPathTool.Size = new System.Drawing.Size(32, 23);
			this.btPathTool.TabIndex = 10;
			this.btPathTool.Text = "...";
			this.btPathTool.UseVisualStyleBackColor = true;
			this.btPathTool.Click += new System.EventHandler(this.OnPathTool);
			// 
			// txtPathTool
			// 
			this.txtPathTool.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.txtPathTool.BackColor = System.Drawing.Color.Gray;
			this.txtPathTool.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.txtPathTool.ForeColor = System.Drawing.Color.LightGreen;
			this.txtPathTool.Location = new System.Drawing.Point(96, 94);
			this.txtPathTool.Name = "txtPathTool";
			this.txtPathTool.ReadOnly = true;
			this.txtPathTool.Size = new System.Drawing.Size(561, 20);
			this.txtPathTool.TabIndex = 9;
			// 
			// btPathOutput
			// 
			this.btPathOutput.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btPathOutput.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btPathOutput.ForeColor = System.Drawing.Color.LightGreen;
			this.btPathOutput.Location = new System.Drawing.Point(662, 123);
			this.btPathOutput.Name = "btPathOutput";
			this.btPathOutput.Size = new System.Drawing.Size(32, 23);
			this.btPathOutput.TabIndex = 13;
			this.btPathOutput.Text = "...";
			this.btPathOutput.UseVisualStyleBackColor = true;
			this.btPathOutput.Click += new System.EventHandler(this.OnPathOutput);
			// 
			// txtPathOutput
			// 
			this.txtPathOutput.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.txtPathOutput.BackColor = System.Drawing.Color.Gray;
			this.txtPathOutput.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.txtPathOutput.ForeColor = System.Drawing.Color.LightGreen;
			this.txtPathOutput.Location = new System.Drawing.Point(96, 125);
			this.txtPathOutput.Name = "txtPathOutput";
			this.txtPathOutput.ReadOnly = true;
			this.txtPathOutput.Size = new System.Drawing.Size(561, 20);
			this.txtPathOutput.TabIndex = 12;
			// 
			// dlgOpenFile
			// 
			this.dlgOpenFile.Filter = "EXE files|*.exe";
			this.dlgOpenFile.Title = "Select Exe...";
			// 
			// dlgFolderBrowser
			// 
			this.dlgFolderBrowser.Description = "Select output path...";
			this.dlgFolderBrowser.RootFolder = System.Environment.SpecialFolder.MyComputer;
			// 
			// btApply
			// 
			this.btApply.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btApply.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btApply.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.btApply.ForeColor = System.Drawing.Color.LightGreen;
			this.btApply.Location = new System.Drawing.Point(565, 5);
			this.btApply.Name = "btApply";
			this.btApply.Size = new System.Drawing.Size(129, 23);
			this.btApply.TabIndex = 14;
			this.btApply.Text = "Apply";
			this.btApply.UseVisualStyleBackColor = true;
			this.btApply.Click += new System.EventHandler(this.OnApply);
			// 
			// CheApeApplier
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.ClientSize = new System.Drawing.Size(706, 163);
			this.Controls.Add(this.btApply);
			this.Controls.Add(this.btPathOutput);
			this.Controls.Add(this.txtPathOutput);
			this.Controls.Add(label5);
			this.Controls.Add(this.btPathTool);
			this.Controls.Add(this.txtPathTool);
			this.Controls.Add(label4);
			this.Controls.Add(this.btPathSapien);
			this.Controls.Add(this.txtPathSapien);
			this.Controls.Add(label3);
			this.Controls.Add(this.btPathGuerilla);
			this.Controls.Add(this.txtPathGuerilla);
			this.Controls.Add(label2);
			this.Controls.Add(this.cbEngineVersion);
			this.Controls.Add(label1);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.MinimumSize = new System.Drawing.Size(380, 190);
			this.Name = "CheApeApplier";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "CheApeApplier";
			this.TopMost = true;
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.ComboBox cbEngineVersion;
		private System.Windows.Forms.TextBox txtPathGuerilla;
		private System.Windows.Forms.Button btPathGuerilla;
		private System.Windows.Forms.Button btPathSapien;
		private System.Windows.Forms.TextBox txtPathSapien;
		private System.Windows.Forms.Button btPathTool;
		private System.Windows.Forms.TextBox txtPathTool;
		private System.Windows.Forms.Button btPathOutput;
		private System.Windows.Forms.TextBox txtPathOutput;
		private System.Windows.Forms.OpenFileDialog dlgOpenFile;
		private System.Windows.Forms.FolderBrowserDialog dlgFolderBrowser;
		private System.Windows.Forms.Button btApply;
	}
}