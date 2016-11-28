/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿namespace OpenSauceIDE
{
	partial class TagIndexViewForm
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
			this.LabelName = new System.Windows.Forms.Label();
			this.LabelDir = new System.Windows.Forms.Label();
			this.LabelRoot = new System.Windows.Forms.Label();
			label1 = new System.Windows.Forms.Label();
			label2 = new System.Windows.Forms.Label();
			label3 = new System.Windows.Forms.Label();
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
			label2.Location = new System.Drawing.Point(12, 22);
			label2.Name = "label2";
			label2.Size = new System.Drawing.Size(55, 13);
			label2.TabIndex = 2;
			label2.Text = "Directory: ";
			// 
			// label3
			// 
			label3.AutoSize = true;
			label3.Location = new System.Drawing.Point(12, 35);
			label3.Name = "label3";
			label3.Size = new System.Drawing.Size(33, 13);
			label3.TabIndex = 4;
			label3.Text = "Root:";
			// 
			// LabelName
			// 
			this.LabelName.AutoSize = true;
			this.LabelName.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.LabelName.Location = new System.Drawing.Point(61, 9);
			this.LabelName.Name = "LabelName";
			this.LabelName.Size = new System.Drawing.Size(49, 15);
			this.LabelName.TabIndex = 1;
			this.LabelName.Text = "<Name>";
			// 
			// LabelDir
			// 
			this.LabelDir.AutoSize = true;
			this.LabelDir.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.LabelDir.Location = new System.Drawing.Point(61, 24);
			this.LabelDir.Name = "LabelDir";
			this.LabelDir.Size = new System.Drawing.Size(34, 15);
			this.LabelDir.TabIndex = 3;
			this.LabelDir.Text = "<Dir>";
			// 
			// LabelRoot
			// 
			this.LabelRoot.AutoSize = true;
			this.LabelRoot.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.LabelRoot.Location = new System.Drawing.Point(61, 39);
			this.LabelRoot.Name = "LabelRoot";
			this.LabelRoot.Size = new System.Drawing.Size(44, 15);
			this.LabelRoot.TabIndex = 5;
			this.LabelRoot.Text = "<Root>";
			// 
			// TagIndexViewForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(532, 273);
			this.Controls.Add(this.LabelRoot);
			this.Controls.Add(label3);
			this.Controls.Add(this.LabelDir);
			this.Controls.Add(label2);
			this.Controls.Add(this.LabelName);
			this.Controls.Add(label1);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "TagIndexViewForm";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Tag Index View";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label LabelName;
		private System.Windows.Forms.Label LabelDir;
		private System.Windows.Forms.Label LabelRoot;
	}
}