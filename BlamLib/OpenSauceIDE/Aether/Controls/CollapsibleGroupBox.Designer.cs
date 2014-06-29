namespace OpenSauceIDE.Aether.Controls
{
    partial class CollapsibleGroupBox
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
			this.ContainerGroupBox = new System.Windows.Forms.GroupBox();
			this.VisibleToggle = new System.Windows.Forms.Button();
			this.ContentsPanel = new System.Windows.Forms.FlowLayoutPanel();
			this.ContainerGroupBox.SuspendLayout();
			this.SuspendLayout();
			// 
			// ContainerGroupBox
			// 
			this.ContainerGroupBox.AutoSize = true;
			this.ContainerGroupBox.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.ContainerGroupBox.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.ContainerGroupBox.Controls.Add(this.VisibleToggle);
			this.ContainerGroupBox.Controls.Add(this.ContentsPanel);
			this.ContainerGroupBox.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.ContainerGroupBox.ForeColor = System.Drawing.Color.LightGreen;
			this.ContainerGroupBox.Location = new System.Drawing.Point(0, 0);
			this.ContainerGroupBox.Margin = new System.Windows.Forms.Padding(0);
			this.ContainerGroupBox.MinimumSize = new System.Drawing.Size(600, 0);
			this.ContainerGroupBox.Name = "ContainerGroupBox";
			this.ContainerGroupBox.Padding = new System.Windows.Forms.Padding(3, 10, 3, 0);
			this.ContainerGroupBox.Size = new System.Drawing.Size(609, 36);
			this.ContainerGroupBox.TabIndex = 0;
			this.ContainerGroupBox.TabStop = false;
			// 
			// VisibleToggle
			// 
			this.VisibleToggle.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.VisibleToggle.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.VisibleToggle.Location = new System.Drawing.Point(583, 0);
			this.VisibleToggle.Margin = new System.Windows.Forms.Padding(0);
			this.VisibleToggle.Name = "VisibleToggle";
			this.VisibleToggle.Size = new System.Drawing.Size(23, 23);
			this.VisibleToggle.TabIndex = 2;
			this.VisibleToggle.Text = "-";
			this.VisibleToggle.TextAlign = System.Drawing.ContentAlignment.TopCenter;
			this.VisibleToggle.UseVisualStyleBackColor = true;
			this.VisibleToggle.Click += new System.EventHandler(this.VisibleToggle_Click);
			// 
			// ContentsPanel
			// 
			this.ContentsPanel.AutoSize = true;
			this.ContentsPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.ContentsPanel.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
			this.ContentsPanel.Location = new System.Drawing.Point(3, 23);
			this.ContentsPanel.Margin = new System.Windows.Forms.Padding(0);
			this.ContentsPanel.MinimumSize = new System.Drawing.Size(600, 0);
			this.ContentsPanel.Name = "ContentsPanel";
			this.ContentsPanel.Size = new System.Drawing.Size(600, 0);
			this.ContentsPanel.TabIndex = 1;
			this.ContentsPanel.WrapContents = false;
			// 
			// CollapsibleGroupBox
			// 
			this.AutoSize = true;
			this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.Controls.Add(this.ContainerGroupBox);
			this.ForeColor = System.Drawing.Color.LightGreen;
			this.MinimumSize = new System.Drawing.Size(600, 0);
			this.Name = "CollapsibleGroupBox";
			this.Size = new System.Drawing.Size(609, 36);
			this.ContainerGroupBox.ResumeLayout(false);
			this.ContainerGroupBox.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion

		protected System.Windows.Forms.Button VisibleToggle;
		protected System.Windows.Forms.GroupBox ContainerGroupBox;
		protected System.Windows.Forms.FlowLayoutPanel ContentsPanel;
    }
}
