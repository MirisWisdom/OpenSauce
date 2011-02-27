/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
namespace OpenSauceIDE
{
	partial class SelectEngineDialog
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
			this.cbEngineVersion = new System.Windows.Forms.ComboBox();
			this.btOk = new System.Windows.Forms.Button();
			this.button1 = new System.Windows.Forms.Button();
			label1 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// label1
			// 
			label1.AutoSize = true;
			label1.ForeColor = System.Drawing.Color.LightGreen;
			label1.Location = new System.Drawing.Point(12, 9);
			label1.Name = "label1";
			label1.Size = new System.Drawing.Size(78, 13);
			label1.TabIndex = 2;
			label1.Text = "Engine Version";
			// 
			// cbEngineVersion
			// 
			this.cbEngineVersion.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.cbEngineVersion.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cbEngineVersion.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.cbEngineVersion.ForeColor = System.Drawing.Color.LightGreen;
			this.cbEngineVersion.FormattingEnabled = true;
			this.cbEngineVersion.Location = new System.Drawing.Point(96, 5);
			this.cbEngineVersion.Name = "cbEngineVersion";
			this.cbEngineVersion.Size = new System.Drawing.Size(121, 21);
			this.cbEngineVersion.TabIndex = 3;
			// 
			// btOk
			// 
			this.btOk.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btOk.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.btOk.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btOk.ForeColor = System.Drawing.Color.LightGreen;
			this.btOk.Location = new System.Drawing.Point(12, 31);
			this.btOk.Name = "btOk";
			this.btOk.Size = new System.Drawing.Size(110, 23);
			this.btOk.TabIndex = 5;
			this.btOk.Text = "OK";
			this.btOk.UseVisualStyleBackColor = true;
			this.btOk.Click += new System.EventHandler(this.OnOK);
			// 
			// button1
			// 
			this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.button1.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.button1.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.button1.ForeColor = System.Drawing.Color.LightGreen;
			this.button1.Location = new System.Drawing.Point(133, 31);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(110, 23);
			this.button1.TabIndex = 6;
			this.button1.Text = "Cancel";
			this.button1.UseVisualStyleBackColor = true;
			this.button1.Click += new System.EventHandler(this.OnCancel);
			// 
			// SelectEngineDialog
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.ClientSize = new System.Drawing.Size(255, 66);
			this.Controls.Add(this.button1);
			this.Controls.Add(this.btOk);
			this.Controls.Add(this.cbEngineVersion);
			this.Controls.Add(label1);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "SelectEngineDialog";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Select Engine";
			this.TopMost = true;
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.ComboBox cbEngineVersion;
		private System.Windows.Forms.Button btOk;
		private System.Windows.Forms.Button button1;
	}
}