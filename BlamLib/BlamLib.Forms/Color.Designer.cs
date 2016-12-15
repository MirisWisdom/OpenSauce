/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
namespace BlamLib.Forms
{
	partial class Color
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
			this.label1 = new System.Windows.Forms.Label();
			this.field_A = new BlamLib.Forms.IntegerTextBox();
			this.field_R = new BlamLib.Forms.IntegerTextBox();
			this.label2 = new System.Windows.Forms.Label();
			this.field_G = new BlamLib.Forms.IntegerTextBox();
			this.label3 = new System.Windows.Forms.Label();
			this.field_B = new BlamLib.Forms.IntegerTextBox();
			this.label4 = new System.Windows.Forms.Label();
			this.colorButton = new System.Windows.Forms.Button();
			this.colorDialog = new System.Windows.Forms.ColorDialog();
			this.SuspendLayout();
			// 
			// name
			// 
			this.name.Location = new System.Drawing.Point(16, 4);
			this.name.Name = "name";
			this.name.Size = new System.Drawing.Size(162, 16);
			this.name.TabIndex = 1;
			this.name.Text = "color";
			this.name.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(168, 0);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(16, 24);
			this.label1.TabIndex = 2;
			this.label1.Text = "a";
			this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// field_A
			// 
			this.field_A.AllowNegative = true;
			this.field_A.DecimalPoint = '.';
			this.field_A.DigitsInGroup = 0;
			this.field_A.Double = 0;
			this.field_A.Flags = 0;
			this.field_A.GroupSeparator = ',';
			this.field_A.Int = 0;
			this.field_A.Location = new System.Drawing.Point(184, 2);
			this.field_A.Long = ((long)(0));
			this.field_A.MaxDecimalPlaces = 0;
			this.field_A.MaxWholeDigits = 9;
			this.field_A.Name = "field_A";
			this.field_A.NegativeSign = '-';
			this.field_A.Prefix = "";
			this.field_A.RangeMax = 2147483647;
			this.field_A.RangeMin = -2147483648;
			this.field_A.Size = new System.Drawing.Size(64, 20);
			this.field_A.TabIndex = 3;
			this.field_A.Text = "0";
			this.field_A.TextChanged += new System.EventHandler(this.OnChanged);
			// 
			// field_R
			// 
			this.field_R.AllowNegative = true;
			this.field_R.DecimalPoint = '.';
			this.field_R.DigitsInGroup = 0;
			this.field_R.Double = 0;
			this.field_R.Flags = 0;
			this.field_R.GroupSeparator = ',';
			this.field_R.Int = 0;
			this.field_R.Location = new System.Drawing.Point(272, 2);
			this.field_R.Long = ((long)(0));
			this.field_R.MaxDecimalPlaces = 0;
			this.field_R.MaxWholeDigits = 9;
			this.field_R.Name = "field_R";
			this.field_R.NegativeSign = '-';
			this.field_R.Prefix = "";
			this.field_R.RangeMax = 2147483647;
			this.field_R.RangeMin = -2147483648;
			this.field_R.Size = new System.Drawing.Size(64, 20);
			this.field_R.TabIndex = 5;
			this.field_R.Text = "0";
			this.field_R.TextChanged += new System.EventHandler(this.OnChanged);
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(256, 0);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(16, 24);
			this.label2.TabIndex = 4;
			this.label2.Text = "r";
			this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// field_G
			// 
			this.field_G.AllowNegative = true;
			this.field_G.DecimalPoint = '.';
			this.field_G.DigitsInGroup = 0;
			this.field_G.Double = 0;
			this.field_G.Flags = 0;
			this.field_G.GroupSeparator = ',';
			this.field_G.Int = 0;
			this.field_G.Location = new System.Drawing.Point(360, 2);
			this.field_G.Long = ((long)(0));
			this.field_G.MaxDecimalPlaces = 0;
			this.field_G.MaxWholeDigits = 9;
			this.field_G.Name = "field_G";
			this.field_G.NegativeSign = '-';
			this.field_G.Prefix = "";
			this.field_G.RangeMax = 2147483647;
			this.field_G.RangeMin = -2147483648;
			this.field_G.Size = new System.Drawing.Size(64, 20);
			this.field_G.TabIndex = 7;
			this.field_G.Text = "0";
			this.field_G.TextChanged += new System.EventHandler(this.OnChanged);
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(344, 0);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(16, 24);
			this.label3.TabIndex = 6;
			this.label3.Text = "g";
			this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// field_B
			// 
			this.field_B.AllowNegative = true;
			this.field_B.DecimalPoint = '.';
			this.field_B.DigitsInGroup = 0;
			this.field_B.Double = 0;
			this.field_B.Flags = 0;
			this.field_B.GroupSeparator = ',';
			this.field_B.Int = 0;
			this.field_B.Location = new System.Drawing.Point(448, 2);
			this.field_B.Long = ((long)(0));
			this.field_B.MaxDecimalPlaces = 0;
			this.field_B.MaxWholeDigits = 9;
			this.field_B.Name = "field_B";
			this.field_B.NegativeSign = '-';
			this.field_B.Prefix = "";
			this.field_B.RangeMax = 2147483647;
			this.field_B.RangeMin = -2147483648;
			this.field_B.Size = new System.Drawing.Size(64, 20);
			this.field_B.TabIndex = 9;
			this.field_B.Text = "0";
			this.field_B.TextChanged += new System.EventHandler(this.OnChanged);
			// 
			// label4
			// 
			this.label4.Location = new System.Drawing.Point(432, 0);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(16, 24);
			this.label4.TabIndex = 8;
			this.label4.Text = "b";
			this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// colorButton
			// 
			this.colorButton.Location = new System.Drawing.Point(520, 0);
			this.colorButton.Name = "colorButton";
			this.colorButton.Size = new System.Drawing.Size(24, 23);
			this.colorButton.TabIndex = 10;
			this.colorButton.UseVisualStyleBackColor = true;
			this.colorButton.Click += new System.EventHandler(this.OnChangeColor);
			// 
			// colorDialog
			// 
			this.colorDialog.AllowFullOpen = false;
			this.colorDialog.AnyColor = true;
			// 
			// Color
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.Controls.Add(this.colorButton);
			this.Controls.Add(this.field_B);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.field_G);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.field_R);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.field_A);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.name);
			this.Name = "Color";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label name;
		private System.Windows.Forms.Label label1;
		private IntegerTextBox field_A;
		private IntegerTextBox field_R;
		private System.Windows.Forms.Label label2;
		private IntegerTextBox field_G;
		private System.Windows.Forms.Label label3;
		private IntegerTextBox field_B;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Button colorButton;
		private System.Windows.Forms.ColorDialog colorDialog;
	}
}
