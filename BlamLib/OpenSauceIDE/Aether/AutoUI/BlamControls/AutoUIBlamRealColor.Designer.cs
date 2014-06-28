namespace OpenSauceIDE.Aether.AutoUI.BlamControls
{
	partial class AutoUIBlamRealColor
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
			this.ColorButton = new System.Windows.Forms.Button();
			this.ColorSplit = new System.Windows.Forms.SplitContainer();
			this.ColorFields = new OpenSauceIDE.Aether.AutoUI.BlamControls.AutoUIBlamMultiReal();
			((System.ComponentModel.ISupportInitialize)(this.ColorSplit)).BeginInit();
			this.ColorSplit.Panel1.SuspendLayout();
			this.ColorSplit.Panel2.SuspendLayout();
			this.ColorSplit.SuspendLayout();
			this.SuspendLayout();
			// 
			// ColorButton
			// 
			this.ColorButton.Dock = System.Windows.Forms.DockStyle.Fill;
			this.ColorButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.ColorButton.Location = new System.Drawing.Point(0, 0);
			this.ColorButton.Name = "ColorButton";
			this.ColorButton.Size = new System.Drawing.Size(70, 20);
			this.ColorButton.TabIndex = 1;
			this.ColorButton.UseVisualStyleBackColor = true;
			this.ColorButton.Click += new System.EventHandler(this.ColorButton_Click);
			// 
			// ColorSplit
			// 
			this.ColorSplit.Dock = System.Windows.Forms.DockStyle.Fill;
			this.ColorSplit.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
			this.ColorSplit.IsSplitterFixed = true;
			this.ColorSplit.Location = new System.Drawing.Point(0, 0);
			this.ColorSplit.Margin = new System.Windows.Forms.Padding(0);
			this.ColorSplit.Name = "ColorSplit";
			// 
			// ColorSplit.Panel1
			// 
			this.ColorSplit.Panel1.Controls.Add(this.ColorButton);
			this.ColorSplit.Panel1MinSize = 70;
			// 
			// ColorSplit.Panel2
			// 
			this.ColorSplit.Panel2.Controls.Add(this.ColorFields);
			this.ColorSplit.Panel2MinSize = 600;
			this.ColorSplit.Size = new System.Drawing.Size(670, 20);
			this.ColorSplit.SplitterDistance = 70;
			this.ColorSplit.SplitterWidth = 1;
			this.ColorSplit.TabIndex = 2;
			// 
			// ColorFields
			// 
			this.ColorFields.AutoSize = true;
			this.ColorFields.Dock = System.Windows.Forms.DockStyle.Fill;
			this.ColorFields.Location = new System.Drawing.Point(0, 0);
			this.ColorFields.Margin = new System.Windows.Forms.Padding(0);
			this.ColorFields.Name = "ColorFields";
			this.ColorFields.Size = new System.Drawing.Size(600, 20);
			this.ColorFields.TabIndex = 0;
			// 
			// AutoUIBlamRealColor
			// 
			this.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.Controls.Add(this.ColorSplit);
			this.ForeColor = System.Drawing.Color.LightGreen;
			this.Name = "AutoUIBlamRealColor";
			this.Size = new System.Drawing.Size(670, 20);
			this.ColorSplit.Panel1.ResumeLayout(false);
			this.ColorSplit.Panel2.ResumeLayout(false);
			this.ColorSplit.Panel2.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.ColorSplit)).EndInit();
			this.ColorSplit.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private AutoUIBlamMultiReal ColorFields;
		private System.Windows.Forms.Button ColorButton;
		private System.Windows.Forms.SplitContainer ColorSplit;
	}
}
