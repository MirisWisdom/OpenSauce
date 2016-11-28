/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
namespace BlamLib.Forms
{
	partial class Data
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
			this.fieldSize = new BlamLib.Forms.IntegerTextBox();
			this.units = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// name
			// 
			this.name.Location = new System.Drawing.Point(16, 4);
			this.name.Name = "name";
			this.name.Size = new System.Drawing.Size(162, 16);
			this.name.TabIndex = 1;
			this.name.Text = "data";
			this.name.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// fieldSize
			// 
			this.fieldSize.AllowNegative = true;
			this.fieldSize.DecimalPoint = '.';
			this.fieldSize.DigitsInGroup = 0;
			this.fieldSize.Double = 0;
			this.fieldSize.Flags = 0;
			this.fieldSize.GroupSeparator = ',';
			this.fieldSize.Int = 0;
			this.fieldSize.Location = new System.Drawing.Point(184, 2);
			this.fieldSize.Long = ((long)(0));
			this.fieldSize.MaxDecimalPlaces = 0;
			this.fieldSize.MaxWholeDigits = 9;
			this.fieldSize.Name = "fieldSize";
			this.fieldSize.NegativeSign = '-';
			this.fieldSize.Prefix = "";
			this.fieldSize.RangeMax = 2147483647;
			this.fieldSize.RangeMin = -2147483648;
			this.fieldSize.Size = new System.Drawing.Size(64, 20);
			this.fieldSize.TabIndex = 6;
			this.fieldSize.Text = "0";
			// 
			// units
			// 
			this.units.Location = new System.Drawing.Point(256, 0);
			this.units.Name = "units";
			this.units.Size = new System.Drawing.Size(110, 24);
			this.units.TabIndex = 7;
			this.units.Text = "units";
			this.units.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// Data
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.Controls.Add(this.units);
			this.Controls.Add(this.fieldSize);
			this.Controls.Add(this.name);
			this.Name = "Data";
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Label name;
		private IntegerTextBox fieldSize;
		private System.Windows.Forms.Label units;
	}
}
