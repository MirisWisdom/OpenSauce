/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
namespace BlamLib.Forms
{
	partial class ByteInteger
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
			this.units = new System.Windows.Forms.Label();
			this.name = new System.Windows.Forms.Label();
			this.field = new BlamLib.Forms.IntegerTextBox();
			this.SuspendLayout();
			// 
			// units
			// 
			this.units.Location = new System.Drawing.Point(256, 0);
			this.units.Name = "units";
			this.units.Size = new System.Drawing.Size(312, 24);
			this.units.TabIndex = 4;
			this.units.Text = "units";
			this.units.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// name
			// 
			this.name.Location = new System.Drawing.Point(16, 4);
			this.name.Name = "name";
			this.name.Size = new System.Drawing.Size(162, 16);
			this.name.TabIndex = 3;
			this.name.Text = "char integer";
			this.name.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
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
			this.field.MaxLength = 4;
			this.field.MaxWholeDigits = 3;
			this.field.Name = "field";
			this.field.NegativeSign = '-';
			this.field.Prefix = "";
			this.field.RangeMax = 128;
			this.field.RangeMin = -127;
			this.field.Size = new System.Drawing.Size(64, 20);
			this.field.TabIndex = 5;
			this.field.Text = "0";
			// 
			// ByteInteger
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.Controls.Add(this.field);
			this.Controls.Add(this.units);
			this.Controls.Add(this.name);
			this.Name = "ByteInteger";
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Label units;
		private System.Windows.Forms.Label name;
		private IntegerTextBox field;
	}
}
