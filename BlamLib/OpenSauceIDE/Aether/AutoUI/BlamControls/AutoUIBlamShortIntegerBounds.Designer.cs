namespace OpenSauceIDE.Aether.AutoUI.BlamControls
{
	partial class AutoUIBlamShortIntegerBounds
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

		#region Component Designer generated code

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.ValueLower = new OpenSauceIDE.Aether.AutoUI.BlamControls.AutoUIBlamShortInteger();
			this.ValueUpper = new OpenSauceIDE.Aether.AutoUI.BlamControls.AutoUIBlamShortInteger();
			this.LabelTo = new System.Windows.Forms.Label();
			this.LabelUnits = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// ValueLower
			// 
			this.ValueLower.Location = new System.Drawing.Point(0, 0);
			this.ValueLower.Margin = new System.Windows.Forms.Padding(0);
			this.ValueLower.Name = "ValueLower";
			this.ValueLower.Size = new System.Drawing.Size(100, 20);
			this.ValueLower.TabIndex = 0;
			// 
			// ValueUpper
			// 
			this.ValueUpper.Location = new System.Drawing.Point(120, -1);
			this.ValueUpper.Margin = new System.Windows.Forms.Padding(0);
			this.ValueUpper.Name = "ValueUpper";
			this.ValueUpper.Size = new System.Drawing.Size(100, 20);
			this.ValueUpper.TabIndex = 1;
			// 
			// LabelTo
			// 
			this.LabelTo.Location = new System.Drawing.Point(100, -1);
			this.LabelTo.Margin = new System.Windows.Forms.Padding(0);
			this.LabelTo.Name = "LabelTo";
			this.LabelTo.Size = new System.Drawing.Size(20, 20);
			this.LabelTo.TabIndex = 2;
			this.LabelTo.Text = "to";
			this.LabelTo.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// LabelUnits
			// 
			this.LabelUnits.AutoSize = true;
			this.LabelUnits.Location = new System.Drawing.Point(220, 0);
			this.LabelUnits.Margin = new System.Windows.Forms.Padding(0);
			this.LabelUnits.MinimumSize = new System.Drawing.Size(0, 20);
			this.LabelUnits.Name = "LabelUnits";
			this.LabelUnits.Size = new System.Drawing.Size(0, 20);
			this.LabelUnits.TabIndex = 3;
			this.LabelUnits.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// AutoUIBlamRealBounds
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.AutoSize = true;
			this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.Controls.Add(this.LabelUnits);
			this.Controls.Add(this.LabelTo);
			this.Controls.Add(this.ValueUpper);
			this.Controls.Add(this.ValueLower);
			this.Margin = new System.Windows.Forms.Padding(0);
			this.Name = "AutoUIBlamRealBounds";
			this.Size = new System.Drawing.Size(220, 20);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private AutoUIBlamShortInteger ValueLower;
		private AutoUIBlamShortInteger ValueUpper;
		private System.Windows.Forms.Label LabelTo;
		private System.Windows.Forms.Label LabelUnits;
	}
}
