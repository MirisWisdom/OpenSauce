namespace OpenSauceIDE.ServerMapDownloadTool
{
	partial class MainForm
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
			this.mapCompressorUI1 = new OpenSauceIDE.ServerMapDownloadTool.MapCompressorUI();
			this.SuspendLayout();
			// 
			// mapCompressorUI1
			// 
			this.mapCompressorUI1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.mapCompressorUI1.AutoValidate = System.Windows.Forms.AutoValidate.EnableAllowFocusChange;
			this.mapCompressorUI1.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.mapCompressorUI1.ForeColor = System.Drawing.Color.LightGreen;
			this.mapCompressorUI1.Location = new System.Drawing.Point(9, 9);
			this.mapCompressorUI1.Margin = new System.Windows.Forms.Padding(0);
			this.mapCompressorUI1.MinimumSize = new System.Drawing.Size(624, 130);
			this.mapCompressorUI1.Name = "mapCompressorUI1";
			this.mapCompressorUI1.Size = new System.Drawing.Size(686, 130);
			this.mapCompressorUI1.TabIndex = 0;
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.ClientSize = new System.Drawing.Size(704, 149);
			this.Controls.Add(this.mapCompressorUI1);
			this.Name = "MainForm";
			this.Text = "Server Map Download Tool";
			this.ResumeLayout(false);

		}

		#endregion

		private MapCompressorUI mapCompressorUI1;

	}
}