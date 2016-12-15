/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
namespace BlamLib.Forms
{
	partial class LongInteger
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
			this.field = new BlamLib.Forms.IntegerTextBox();
			this.units = new System.Windows.Forms.Label();
			this.name = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// field
			// 
			this.field.AllowNegative = true;
			this.field.DecimalPoint = '.';
			this.field.DigitsInGroup = 0;
			this.field.Double = 0;
			this.field.Flags = 0;
			this.field.GroupSeparator = ',';
			this.field.Int = 0;
			this.field.Location = new System.Drawing.Point(184, 2);
			this.field.Long = ((long)(0));
			this.field.MaxDecimalPlaces = 0;
			this.field.MaxLength = 11;
			this.field.MaxWholeDigits = 10;
			this.field.Name = "field";
			this.field.NegativeSign = '-';
			this.field.Prefix = "";
			this.field.RangeMax = 2147483647;
			this.field.RangeMin = -2147483646;
			this.field.Size = new System.Drawing.Size(64, 20);
			this.field.TabIndex = 8;
			this.field.Text = "0";
			// 
			// units
			// 
			this.units.Location = new System.Drawing.Point(256, 0);
			this.units.Name = "units";
			this.units.Size = new System.Drawing.Size(312, 24);
			this.units.TabIndex = 7;
			this.units.Text = "units";
			this.units.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// name
			// 
			this.name.Location = new System.Drawing.Point(16, 4);
			this.name.Name = "name";
			this.name.Size = new System.Drawing.Size(162, 16);
			this.name.TabIndex = 6;
			this.name.Text = "long integer";
			this.name.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// LongInteger
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.Controls.Add(this.field);
			this.Controls.Add(this.units);
			this.Controls.Add(this.name);
			this.Name = "LongInteger";
			this.ResumeLayout(false);

		}

		#endregion

		private IntegerTextBox field;
		private System.Windows.Forms.Label units;
		private System.Windows.Forms.Label name;
	}
}
