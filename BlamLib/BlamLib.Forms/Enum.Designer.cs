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
namespace BlamLib.Forms
{
	partial class Enum
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
			this.field = new System.Windows.Forms.ComboBox();
			this.name = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// field
			// 
			this.field.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.field.FormattingEnabled = true;
			this.field.Location = new System.Drawing.Point(184, 2);
			this.field.Name = "field";
			this.field.Size = new System.Drawing.Size(208, 21);
			this.field.TabIndex = 4;
			// 
			// name
			// 
			this.name.Location = new System.Drawing.Point(16, 4);
			this.name.Name = "name";
			this.name.Size = new System.Drawing.Size(162, 16);
			this.name.TabIndex = 3;
			this.name.Text = "enum";
			this.name.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// Enum
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.Controls.Add(this.field);
			this.Controls.Add(this.name);
			this.Name = "Enum";
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.ComboBox field;
		private System.Windows.Forms.Label name;
	}
}
