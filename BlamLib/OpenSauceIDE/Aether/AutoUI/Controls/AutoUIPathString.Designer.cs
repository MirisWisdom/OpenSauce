namespace OpenSauceIDE.Aether.AutoUI.Controls
{
	partial class AutoUIPathString
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
			this.BrowseButton = new System.Windows.Forms.Button();
			this.PathBrowser = new System.Windows.Forms.FolderBrowserDialog();
			this.PathStringTextBox = new OpenSauceIDE.Aether.AutoUI.Controls.AutoUIString();
			this.SuspendLayout();
			// 
			// BrowseButton
			// 
			this.BrowseButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.BrowseButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.BrowseButton.Location = new System.Drawing.Point(374, 0);
			this.BrowseButton.Margin = new System.Windows.Forms.Padding(0);
			this.BrowseButton.Name = "BrowseButton";
			this.BrowseButton.Size = new System.Drawing.Size(26, 22);
			this.BrowseButton.TabIndex = 1;
			this.BrowseButton.Text = "...";
			this.BrowseButton.UseVisualStyleBackColor = true;
			this.BrowseButton.Click += new System.EventHandler(this.BrowseForPath);
			// 
			// PathStringTextBox
			// 
			this.PathStringTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.PathStringTextBox.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.PathStringTextBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.PathStringTextBox.ForeColor = System.Drawing.Color.LightGreen;
			this.PathStringTextBox.Location = new System.Drawing.Point(0, 0);
			this.PathStringTextBox.Margin = new System.Windows.Forms.Padding(0);
			this.PathStringTextBox.MaxLength = 0;
			this.PathStringTextBox.Name = "PathStringTextBox";
			this.PathStringTextBox.Size = new System.Drawing.Size(374, 20);
			this.PathStringTextBox.TabIndex = 0;
			this.PathStringTextBox.TextChanged += new System.EventHandler(this.PathChanged);
			// 
			// AutoUIPathString
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.Controls.Add(this.BrowseButton);
			this.Controls.Add(this.PathStringTextBox);
			this.ForeColor = System.Drawing.Color.LightGreen;
			this.Margin = new System.Windows.Forms.Padding(0);
			this.MinimumSize = new System.Drawing.Size(200, 22);
			this.Name = "AutoUIPathString";
			this.Size = new System.Drawing.Size(400, 22);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private AutoUIString PathStringTextBox;
		private System.Windows.Forms.Button BrowseButton;
		private System.Windows.Forms.FolderBrowserDialog PathBrowser;
	}
}
