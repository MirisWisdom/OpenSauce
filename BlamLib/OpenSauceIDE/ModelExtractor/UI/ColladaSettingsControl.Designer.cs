namespace OpenSauceIDE.ModelExtractor.UI
{
    partial class ColladaSettingsControl
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
            this.mOverwriteCheckbox = new System.Windows.Forms.CheckBox();
            this.mBitmapExtensionComboBox = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // mOverwriteCheckbox
            // 
            this.mOverwriteCheckbox.AutoSize = true;
            this.mOverwriteCheckbox.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.mOverwriteCheckbox.Location = new System.Drawing.Point(14, 27);
            this.mOverwriteCheckbox.Name = "mOverwriteCheckbox";
            this.mOverwriteCheckbox.Size = new System.Drawing.Size(98, 17);
            this.mOverwriteCheckbox.TabIndex = 0;
            this.mOverwriteCheckbox.Text = "Overwrite Files:";
            this.mOverwriteCheckbox.UseVisualStyleBackColor = true;
            // 
            // mBitmapExtensionComboBox
            // 
            this.mBitmapExtensionComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.mBitmapExtensionComboBox.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.mBitmapExtensionComboBox.FormattingEnabled = true;
            this.mBitmapExtensionComboBox.Location = new System.Drawing.Point(100, 0);
            this.mBitmapExtensionComboBox.Name = "mBitmapExtensionComboBox";
            this.mBitmapExtensionComboBox.Size = new System.Drawing.Size(121, 21);
            this.mBitmapExtensionComboBox.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 3);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(91, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Bitmap Extension:";
            // 
            // COLLADASettingsUI
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.label1);
            this.Controls.Add(this.mBitmapExtensionComboBox);
            this.Controls.Add(this.mOverwriteCheckbox);
            this.Name = "COLLADASettingsUI";
            this.Size = new System.Drawing.Size(222, 44);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox mOverwriteCheckbox;
        private System.Windows.Forms.ComboBox mBitmapExtensionComboBox;
        private System.Windows.Forms.Label label1;
    }
}
