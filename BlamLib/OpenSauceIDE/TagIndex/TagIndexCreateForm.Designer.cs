/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿namespace OpenSauceIDE
{
	partial class TagIndexCreateForm
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
			System.Windows.Forms.Button ButtonCancel;
			System.Windows.Forms.Label label4;
			this.TagIndexName = new System.Windows.Forms.TextBox();
			this.TagIndexDir = new System.Windows.Forms.TextBox();
			this.TagIndexDirBrowse = new System.Windows.Forms.Button();
			this.TagIndexRoot = new System.Windows.Forms.TextBox();
			this.TagIndexRootBrowse = new System.Windows.Forms.Button();
			this.ButtonOK = new System.Windows.Forms.Button();
			this.TagIndexDirBrowser = new System.Windows.Forms.FolderBrowserDialog();
			this.TagIndexRootBrowser = new System.Windows.Forms.OpenFileDialog();
			this.ModifyFilter = new System.Windows.Forms.Button();
			label1 = new System.Windows.Forms.Label();
			label2 = new System.Windows.Forms.Label();
			label3 = new System.Windows.Forms.Label();
			ButtonCancel = new System.Windows.Forms.Button();
			label4 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// label1
			// 
			label1.AutoSize = true;
			label1.Location = new System.Drawing.Point(12, 9);
			label1.Name = "label1";
			label1.Size = new System.Drawing.Size(38, 13);
			label1.TabIndex = 0;
			label1.Text = "Name:";
			// 
			// label2
			// 
			label2.AutoSize = true;
			label2.Location = new System.Drawing.Point(12, 64);
			label2.Name = "label2";
			label2.Size = new System.Drawing.Size(52, 13);
			label2.TabIndex = 2;
			label2.Text = "Directory:";
			// 
			// label3
			// 
			label3.AutoSize = true;
			label3.Location = new System.Drawing.Point(12, 91);
			label3.Name = "label3";
			label3.Size = new System.Drawing.Size(55, 13);
			label3.TabIndex = 5;
			label3.Text = "Root Tag:";
			// 
			// ButtonCancel
			// 
			ButtonCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			ButtonCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			ButtonCancel.Location = new System.Drawing.Point(293, 115);
			ButtonCancel.Name = "ButtonCancel";
			ButtonCancel.Size = new System.Drawing.Size(75, 23);
			ButtonCancel.TabIndex = 9;
			ButtonCancel.Text = "Cancel";
			ButtonCancel.UseVisualStyleBackColor = true;
			ButtonCancel.Click += new System.EventHandler(this.OnCancel);
			// 
			// label4
			// 
			label4.AutoSize = true;
			label4.Location = new System.Drawing.Point(12, 37);
			label4.Name = "label4";
			label4.Size = new System.Drawing.Size(29, 13);
			label4.TabIndex = 10;
			label4.Text = "Filter";
			// 
			// TagIndexName
			// 
			this.TagIndexName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TagIndexName.Location = new System.Drawing.Point(73, 6);
			this.TagIndexName.MaxLength = 32;
			this.TagIndexName.Name = "TagIndexName";
			this.TagIndexName.Size = new System.Drawing.Size(262, 20);
			this.TagIndexName.TabIndex = 1;
			// 
			// TagIndexDir
			// 
			this.TagIndexDir.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TagIndexDir.Location = new System.Drawing.Point(73, 61);
			this.TagIndexDir.Name = "TagIndexDir";
			this.TagIndexDir.ReadOnly = true;
			this.TagIndexDir.Size = new System.Drawing.Size(262, 20);
			this.TagIndexDir.TabIndex = 3;
			// 
			// TagIndexDirBrowse
			// 
			this.TagIndexDirBrowse.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.TagIndexDirBrowse.Location = new System.Drawing.Point(341, 56);
			this.TagIndexDirBrowse.Name = "TagIndexDirBrowse";
			this.TagIndexDirBrowse.Size = new System.Drawing.Size(27, 23);
			this.TagIndexDirBrowse.TabIndex = 4;
			this.TagIndexDirBrowse.Text = "...";
			this.TagIndexDirBrowse.UseVisualStyleBackColor = true;
			this.TagIndexDirBrowse.Click += new System.EventHandler(this.OnIndexBrowse);
			// 
			// TagIndexRoot
			// 
			this.TagIndexRoot.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TagIndexRoot.Location = new System.Drawing.Point(73, 88);
			this.TagIndexRoot.Name = "TagIndexRoot";
			this.TagIndexRoot.ReadOnly = true;
			this.TagIndexRoot.Size = new System.Drawing.Size(262, 20);
			this.TagIndexRoot.TabIndex = 6;
			// 
			// TagIndexRootBrowse
			// 
			this.TagIndexRootBrowse.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.TagIndexRootBrowse.Location = new System.Drawing.Point(341, 85);
			this.TagIndexRootBrowse.Name = "TagIndexRootBrowse";
			this.TagIndexRootBrowse.Size = new System.Drawing.Size(27, 23);
			this.TagIndexRootBrowse.TabIndex = 7;
			this.TagIndexRootBrowse.Text = "...";
			this.TagIndexRootBrowse.UseVisualStyleBackColor = true;
			this.TagIndexRootBrowse.Click += new System.EventHandler(this.OnRootBrowse);
			// 
			// ButtonOK
			// 
			this.ButtonOK.Location = new System.Drawing.Point(15, 115);
			this.ButtonOK.Name = "ButtonOK";
			this.ButtonOK.Size = new System.Drawing.Size(75, 23);
			this.ButtonOK.TabIndex = 8;
			this.ButtonOK.Text = "OK";
			this.ButtonOK.UseVisualStyleBackColor = true;
			this.ButtonOK.Click += new System.EventHandler(this.OnOK);
			// 
			// TagIndexDirBrowser
			// 
			this.TagIndexDirBrowser.Description = "Browse For Game Directory...";
			this.TagIndexDirBrowser.RootFolder = System.Environment.SpecialFolder.MyComputer;
			this.TagIndexDirBrowser.ShowNewFolderButton = false;
			// 
			// TagIndexRootBrowser
			// 
			this.TagIndexRootBrowser.Title = "Browse for root tag...";
			this.TagIndexRootBrowser.FileOk += new System.ComponentModel.CancelEventHandler(this.OnTagIndexRootBrowserFileOk);
			// 
			// ModifyFilter
			// 
			this.ModifyFilter.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.ModifyFilter.Enabled = false;
			this.ModifyFilter.Location = new System.Drawing.Point(73, 32);
			this.ModifyFilter.Name = "ModifyFilter";
			this.ModifyFilter.Size = new System.Drawing.Size(262, 23);
			this.ModifyFilter.TabIndex = 11;
			this.ModifyFilter.Text = "Modify Filter";
			this.ModifyFilter.UseVisualStyleBackColor = true;
			this.ModifyFilter.Click += new System.EventHandler(this.OnModifyFilter);
			// 
			// TagIndexCreateForm
			// 
			this.AcceptButton = this.ButtonOK;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = ButtonCancel;
			this.ClientSize = new System.Drawing.Size(388, 150);
			this.Controls.Add(this.ModifyFilter);
			this.Controls.Add(label4);
			this.Controls.Add(ButtonCancel);
			this.Controls.Add(this.ButtonOK);
			this.Controls.Add(this.TagIndexRootBrowse);
			this.Controls.Add(this.TagIndexRoot);
			this.Controls.Add(label3);
			this.Controls.Add(this.TagIndexDirBrowse);
			this.Controls.Add(this.TagIndexDir);
			this.Controls.Add(label2);
			this.Controls.Add(this.TagIndexName);
			this.Controls.Add(label1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "TagIndexCreateForm";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Create Tag Index";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.TextBox TagIndexName;
		private System.Windows.Forms.TextBox TagIndexDir;
		private System.Windows.Forms.Button TagIndexDirBrowse;
		private System.Windows.Forms.TextBox TagIndexRoot;
		private System.Windows.Forms.Button TagIndexRootBrowse;
		private System.Windows.Forms.Button ButtonOK;
		private System.Windows.Forms.FolderBrowserDialog TagIndexDirBrowser;
		private System.Windows.Forms.OpenFileDialog TagIndexRootBrowser;
		private System.Windows.Forms.Button ModifyFilter;
	}
}