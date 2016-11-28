namespace OpenSauceIDE.LightmapImporter
{
	partial class LightmapImporter
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
			this.mImportUVsButton = new System.Windows.Forms.Button();
			this.mMessageListControl = new OpenSauceIDE.Controls.MessageListControl();
			this.mDataPathControl = new OpenSauceIDE.Controls.PathControl();
			this.mTagsPathControl = new OpenSauceIDE.Controls.PathControl();
			this.SuspendLayout();
			// 
			// mImportUVsButton
			// 
			this.mImportUVsButton.Dock = System.Windows.Forms.DockStyle.Right;
			this.mImportUVsButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.mImportUVsButton.Location = new System.Drawing.Point(703, 42);
			this.mImportUVsButton.Name = "mImportUVsButton";
			this.mImportUVsButton.Size = new System.Drawing.Size(121, 122);
			this.mImportUVsButton.TabIndex = 3;
			this.mImportUVsButton.Text = "Import Lightmap UV\'s";
			this.mImportUVsButton.UseVisualStyleBackColor = true;
			this.mImportUVsButton.Click += new System.EventHandler(this.ImportUVsClick);
			// 
			// mMessageListControl
			// 
			this.mMessageListControl.Dock = System.Windows.Forms.DockStyle.Left;
			this.mMessageListControl.Location = new System.Drawing.Point(0, 42);
			this.mMessageListControl.MaxMessageCount = 256;
			this.mMessageListControl.MinimumSize = new System.Drawing.Size(600, 72);
			this.mMessageListControl.Name = "mMessageListControl";
			this.mMessageListControl.Size = new System.Drawing.Size(697, 122);
			this.mMessageListControl.TabIndex = 2;
			// 
			// mDataPathControl
			// 
			this.mDataPathControl.BackColorExists = System.Drawing.Color.White;
			this.mDataPathControl.BackColorMissing = System.Drawing.Color.White;
			this.mDataPathControl.Description = null;
			this.mDataPathControl.Dock = System.Windows.Forms.DockStyle.Top;
			this.mDataPathControl.ForeColorExists = System.Drawing.Color.Black;
			this.mDataPathControl.ForeColorMissing = System.Drawing.Color.Red;
			this.mDataPathControl.Location = new System.Drawing.Point(0, 21);
			this.mDataPathControl.Name = "mDataPathControl";
			this.mDataPathControl.SelectedPath = null;
			this.mDataPathControl.Size = new System.Drawing.Size(824, 21);
			this.mDataPathControl.TabIndex = 1;
			this.mDataPathControl.Title = "Data Directory";
			// 
			// mTagsPathControl
			// 
			this.mTagsPathControl.BackColorExists = System.Drawing.Color.White;
			this.mTagsPathControl.BackColorMissing = System.Drawing.Color.White;
			this.mTagsPathControl.Description = null;
			this.mTagsPathControl.Dock = System.Windows.Forms.DockStyle.Top;
			this.mTagsPathControl.ForeColorExists = System.Drawing.Color.Black;
			this.mTagsPathControl.ForeColorMissing = System.Drawing.Color.Red;
			this.mTagsPathControl.Location = new System.Drawing.Point(0, 0);
			this.mTagsPathControl.Name = "mTagsPathControl";
			this.mTagsPathControl.SelectedPath = null;
			this.mTagsPathControl.Size = new System.Drawing.Size(824, 21);
			this.mTagsPathControl.TabIndex = 0;
			this.mTagsPathControl.Title = "Tags Directory";
			// 
			// LightmapImporter
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.ClientSize = new System.Drawing.Size(824, 164);
			this.Controls.Add(this.mImportUVsButton);
			this.Controls.Add(this.mMessageListControl);
			this.Controls.Add(this.mDataPathControl);
			this.Controls.Add(this.mTagsPathControl);
			this.ForeColor = System.Drawing.Color.LightGreen;
			this.MaximumSize = new System.Drawing.Size(840, 202);
			this.Name = "LightmapImporter";
			this.Text = "Lightmap Importer";
			this.ResumeLayout(false);

		}

		#endregion

		private Controls.PathControl mTagsPathControl;
		private Controls.PathControl mDataPathControl;
		private Controls.MessageListControl mMessageListControl;
		private System.Windows.Forms.Button mImportUVsButton;
	}
}