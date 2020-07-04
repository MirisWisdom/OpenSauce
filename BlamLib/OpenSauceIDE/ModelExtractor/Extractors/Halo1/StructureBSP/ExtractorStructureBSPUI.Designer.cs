namespace OpenSauceIDE.ModelExtractor.Extractors.Halo1.StructureBSP
{
    partial class ExtractorStructureBSPUI
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
			this.mExtractionOptionFlags = new System.Windows.Forms.CheckedListBox();
			this.label1 = new System.Windows.Forms.Label();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.label2 = new System.Windows.Forms.Label();
			this.mSizeMultiplierNumeric = new System.Windows.Forms.NumericUpDown();
			this.groupBox1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.mSizeMultiplierNumeric)).BeginInit();
			this.SuspendLayout();
			// 
			// mExtractionOptionFlags
			// 
			this.mExtractionOptionFlags.FormattingEnabled = true;
			this.mExtractionOptionFlags.Items.AddRange(new object[] {
            "Render Mesh",
            "Portals",
            "Fog Planes",
            "BSP Markers",
            "Lightmap Sizes"});
			this.mExtractionOptionFlags.Location = new System.Drawing.Point(120, 16);
			this.mExtractionOptionFlags.Name = "mExtractionOptionFlags";
			this.mExtractionOptionFlags.Size = new System.Drawing.Size(150, 79);
			this.mExtractionOptionFlags.TabIndex = 0;
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(71, 16);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(43, 13);
			this.label1.TabIndex = 1;
			this.label1.Text = "Extract:";
			// 
			// groupBox1
			// 
			this.groupBox1.AutoSize = true;
			this.groupBox1.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.groupBox1.Controls.Add(this.label2);
			this.groupBox1.Controls.Add(this.mSizeMultiplierNumeric);
			this.groupBox1.Controls.Add(this.mExtractionOptionFlags);
			this.groupBox1.Controls.Add(this.label1);
			this.groupBox1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.groupBox1.ForeColor = System.Drawing.Color.LightGreen;
			this.groupBox1.Location = new System.Drawing.Point(0, 0);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(276, 135);
			this.groupBox1.TabIndex = 2;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Structure BSP";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(17, 98);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(97, 13);
			this.label2.TabIndex = 3;
			this.label2.Text = "Lightmap Multiplier:";
			// 
			// mSizeMultiplierNumeric
			// 
			this.mSizeMultiplierNumeric.Location = new System.Drawing.Point(120, 96);
			this.mSizeMultiplierNumeric.Maximum = new decimal(new int[] {
            8,
            0,
            0,
            0});
			this.mSizeMultiplierNumeric.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
			this.mSizeMultiplierNumeric.Name = "mSizeMultiplierNumeric";
			this.mSizeMultiplierNumeric.Size = new System.Drawing.Size(150, 20);
			this.mSizeMultiplierNumeric.TabIndex = 2;
			this.mSizeMultiplierNumeric.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
			// 
			// ExtractorStructureBSPUI
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.AutoSize = true;
			this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.Controls.Add(this.groupBox1);
			this.Name = "ExtractorStructureBSPUI";
			this.Size = new System.Drawing.Size(276, 135);
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.mSizeMultiplierNumeric)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckedListBox mExtractionOptionFlags;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown mSizeMultiplierNumeric;
    }
}
