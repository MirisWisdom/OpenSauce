namespace OpenSauceIDE.Aether.AutoUI.Controls
{
	partial class AutoUIFieldContainer
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
			this.TitleLabel = new System.Windows.Forms.Label();
			this.ControlPanel = new System.Windows.Forms.FlowLayoutPanel();
			this.FieldContainerSplit = new System.Windows.Forms.SplitContainer();
			((System.ComponentModel.ISupportInitialize)(this.FieldContainerSplit)).BeginInit();
			this.FieldContainerSplit.Panel1.SuspendLayout();
			this.FieldContainerSplit.Panel2.SuspendLayout();
			this.FieldContainerSplit.SuspendLayout();
			this.SuspendLayout();
			// 
			// TitleLabel
			// 
			this.TitleLabel.Location = new System.Drawing.Point(0, 0);
			this.TitleLabel.Margin = new System.Windows.Forms.Padding(0);
			this.TitleLabel.MinimumSize = new System.Drawing.Size(120, 20);
			this.TitleLabel.Name = "TitleLabel";
			this.TitleLabel.Size = new System.Drawing.Size(120, 20);
			this.TitleLabel.TabIndex = 0;
			this.TitleLabel.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			// 
			// ControlPanel
			// 
			this.ControlPanel.AutoSize = true;
			this.ControlPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.ControlPanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.ControlPanel.Dock = System.Windows.Forms.DockStyle.Fill;
			this.ControlPanel.Location = new System.Drawing.Point(0, 0);
			this.ControlPanel.Margin = new System.Windows.Forms.Padding(0);
			this.ControlPanel.MinimumSize = new System.Drawing.Size(2, 22);
			this.ControlPanel.Name = "ControlPanel";
			this.ControlPanel.Padding = new System.Windows.Forms.Padding(1);
			this.ControlPanel.Size = new System.Drawing.Size(479, 22);
			this.ControlPanel.TabIndex = 1;
			// 
			// FieldContainerSplit
			// 
			this.FieldContainerSplit.Dock = System.Windows.Forms.DockStyle.Fill;
			this.FieldContainerSplit.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
			this.FieldContainerSplit.IsSplitterFixed = true;
			this.FieldContainerSplit.Location = new System.Drawing.Point(0, 0);
			this.FieldContainerSplit.Margin = new System.Windows.Forms.Padding(0);
			this.FieldContainerSplit.Name = "FieldContainerSplit";
			// 
			// FieldContainerSplit.Panel1
			// 
			this.FieldContainerSplit.Panel1.Controls.Add(this.TitleLabel);
			// 
			// FieldContainerSplit.Panel2
			// 
			this.FieldContainerSplit.Panel2.Controls.Add(this.ControlPanel);
			this.FieldContainerSplit.Size = new System.Drawing.Size(600, 22);
			this.FieldContainerSplit.SplitterDistance = 120;
			this.FieldContainerSplit.SplitterWidth = 1;
			this.FieldContainerSplit.TabIndex = 2;
			// 
			// AutoUIFieldContainer
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.AutoSize = true;
			this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.Controls.Add(this.FieldContainerSplit);
			this.Margin = new System.Windows.Forms.Padding(0);
			this.MinimumSize = new System.Drawing.Size(600, 22);
			this.Name = "AutoUIFieldContainer";
			this.Size = new System.Drawing.Size(600, 22);
			this.FieldContainerSplit.Panel1.ResumeLayout(false);
			this.FieldContainerSplit.Panel2.ResumeLayout(false);
			this.FieldContainerSplit.Panel2.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.FieldContainerSplit)).EndInit();
			this.FieldContainerSplit.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Label TitleLabel;
		private System.Windows.Forms.FlowLayoutPanel ControlPanel;
		private System.Windows.Forms.SplitContainer FieldContainerSplit;
	}
}
