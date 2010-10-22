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
	partial class AngleBounds
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
			this.name = new System.Windows.Forms.Label();
			this.field_Lower = new BlamLib.Forms.NumericTextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.field_Upper = new BlamLib.Forms.NumericTextBox();
			this.units = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// name
			// 
			this.name.Location = new System.Drawing.Point(16, 4);
			this.name.Name = "name";
			this.name.Size = new System.Drawing.Size(162, 16);
			this.name.TabIndex = 1;
			this.name.Text = "angle bounds";
			this.name.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// field_Lower
			// 
			this.field_Lower.AllowNegative = true;
			this.field_Lower.DecimalPoint = '.';
			this.field_Lower.DigitsInGroup = 0;
			this.field_Lower.Double = 0;
			this.field_Lower.Flags = 0;
			this.field_Lower.GroupSeparator = ',';
			this.field_Lower.Int = 0;
			this.field_Lower.Location = new System.Drawing.Point(184, 2);
			this.field_Lower.Long = ((long)(0));
			this.field_Lower.MaxDecimalPlaces = 7;
			this.field_Lower.MaxWholeDigits = 9;
			this.field_Lower.Name = "field_Lower";
			this.field_Lower.NegativeSign = '-';
			this.field_Lower.Prefix = "";
			this.field_Lower.RangeMax = 3.4028234663852886E+38;
			this.field_Lower.RangeMin = -3.4028234663852886E+38;
			this.field_Lower.Size = new System.Drawing.Size(64, 20);
			this.field_Lower.TabIndex = 2;
			this.field_Lower.Text = "0";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(254, 6);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(16, 13);
			this.label1.TabIndex = 3;
			this.label1.Text = "to";
			this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// field_Upper
			// 
			this.field_Upper.AllowNegative = true;
			this.field_Upper.DecimalPoint = '.';
			this.field_Upper.DigitsInGroup = 0;
			this.field_Upper.Double = 0;
			this.field_Upper.Flags = 0;
			this.field_Upper.GroupSeparator = ',';
			this.field_Upper.Int = 0;
			this.field_Upper.Location = new System.Drawing.Point(272, 2);
			this.field_Upper.Long = ((long)(0));
			this.field_Upper.MaxDecimalPlaces = 7;
			this.field_Upper.MaxWholeDigits = 9;
			this.field_Upper.Name = "field_Upper";
			this.field_Upper.NegativeSign = '-';
			this.field_Upper.Prefix = "";
			this.field_Upper.RangeMax = 3.4028234663852886E+38;
			this.field_Upper.RangeMin = -3.4028234663852886E+38;
			this.field_Upper.Size = new System.Drawing.Size(64, 20);
			this.field_Upper.TabIndex = 4;
			this.field_Upper.Text = "0";
			// 
			// units
			// 
			this.units.Location = new System.Drawing.Point(344, 0);
			this.units.Name = "units";
			this.units.Size = new System.Drawing.Size(224, 23);
			this.units.TabIndex = 5;
			this.units.Text = "degrees per second";
			this.units.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// AngleBounds
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.Controls.Add(this.units);
			this.Controls.Add(this.field_Upper);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.field_Lower);
			this.Controls.Add(this.name);
			this.Name = "AngleBounds";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label name;
		private NumericTextBox field_Lower;
		private System.Windows.Forms.Label label1;
		private NumericTextBox field_Upper;
		private System.Windows.Forms.Label units;
	}
}
