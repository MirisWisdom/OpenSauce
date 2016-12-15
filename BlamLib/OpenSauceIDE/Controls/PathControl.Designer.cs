namespace OpenSauceIDE.Controls
{
    partial class PathControl
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
			this.mBrowseButton = new System.Windows.Forms.Button();
			this.mTitleLabel = new System.Windows.Forms.Label();
			this.mPathTextBox = new System.Windows.Forms.TextBox();
			this.SuspendLayout();
			// 
			// mBrowseButton
			// 
			this.mBrowseButton.Dock = System.Windows.Forms.DockStyle.Right;
			this.mBrowseButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.mBrowseButton.Location = new System.Drawing.Point(471, 0);
			this.mBrowseButton.Name = "mBrowseButton";
			this.mBrowseButton.Size = new System.Drawing.Size(29, 21);
			this.mBrowseButton.TabIndex = 0;
			this.mBrowseButton.Text = "...";
			this.mBrowseButton.UseVisualStyleBackColor = true;
			this.mBrowseButton.Click += new System.EventHandler(this.Browse);
			// 
			// mTitleLabel
			// 
			this.mTitleLabel.Dock = System.Windows.Forms.DockStyle.Left;
			this.mTitleLabel.Location = new System.Drawing.Point(0, 0);
			this.mTitleLabel.Name = "mTitleLabel";
			this.mTitleLabel.Size = new System.Drawing.Size(120, 21);
			this.mTitleLabel.TabIndex = 1;
			this.mTitleLabel.Text = "Title";
			this.mTitleLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// mPathTextBox
			// 
			this.mPathTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
			this.mPathTextBox.Location = new System.Drawing.Point(120, 0);
			this.mPathTextBox.Name = "mPathTextBox";
			this.mPathTextBox.Size = new System.Drawing.Size(351, 20);
			this.mPathTextBox.TabIndex = 2;
			// 
			// PathControl
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.mPathTextBox);
			this.Controls.Add(this.mTitleLabel);
			this.Controls.Add(this.mBrowseButton);
			this.Name = "PathControl";
			this.Size = new System.Drawing.Size(500, 21);
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button mBrowseButton;
        private System.Windows.Forms.Label mTitleLabel;
        private System.Windows.Forms.TextBox mPathTextBox;
    }
}
