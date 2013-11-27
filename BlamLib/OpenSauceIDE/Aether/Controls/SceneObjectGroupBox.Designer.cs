namespace OpenSauceIDE.Aether.Controls
{
	partial class SceneObjectGroupBox
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
			this.MembersPanel = new System.Windows.Forms.FlowLayoutPanel();
			this.ChildrenPanel = new System.Windows.Forms.FlowLayoutPanel();
			this.ObjectSelectionCombo = new System.Windows.Forms.ComboBox();
			this.ExtractAllButton = new System.Windows.Forms.Button();
			this.ContainerGroupBox.SuspendLayout();
			this.SuspendLayout();
			// 
			// ExtractButton
			// 
			this.ExtractButton.Location = new System.Drawing.Point(513, 0);
			// 
			// VisibleToggle
			// 
			this.VisibleToggle.Location = new System.Drawing.Point(583, 0);
			// 
			// ContainerGroupBox
			// 
			this.ContainerGroupBox.Controls.Add(this.ExtractAllButton);
			this.ContainerGroupBox.Controls.Add(this.ObjectSelectionCombo);
			this.ContainerGroupBox.Size = new System.Drawing.Size(609, 39);
			this.ContainerGroupBox.Controls.SetChildIndex(this.ObjectSelectionCombo, 0);
			this.ContainerGroupBox.Controls.SetChildIndex(this.VisibleToggle, 0);
			this.ContainerGroupBox.Controls.SetChildIndex(this.ExtractButton, 0);
			this.ContainerGroupBox.Controls.SetChildIndex(this.ExtractAllButton, 0);
			// 
			// MembersPanel
			// 
			this.MembersPanel.AutoSize = true;
			this.MembersPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.MembersPanel.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
			this.MembersPanel.Location = new System.Drawing.Point(3, 9);
			this.MembersPanel.MinimumSize = new System.Drawing.Size(594, 0);
			this.MembersPanel.Name = "MembersPanel";
			this.MembersPanel.Size = new System.Drawing.Size(594, 0);
			this.MembersPanel.TabIndex = 0;
			this.MembersPanel.WrapContents = false;
			// 
			// ChildrenPanel
			// 
			this.ChildrenPanel.AutoSize = true;
			this.ChildrenPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.ChildrenPanel.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
			this.ChildrenPanel.Location = new System.Drawing.Point(3, 3);
			this.ChildrenPanel.MinimumSize = new System.Drawing.Size(594, 0);
			this.ChildrenPanel.Name = "ChildrenPanel";
			this.ChildrenPanel.Size = new System.Drawing.Size(594, 0);
			this.ChildrenPanel.TabIndex = 1;
			this.ChildrenPanel.WrapContents = false;
			// 
			// ObjectSelectionCombo
			// 
			this.ObjectSelectionCombo.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.ObjectSelectionCombo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.ObjectSelectionCombo.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.ObjectSelectionCombo.ForeColor = System.Drawing.Color.LightGreen;
			this.ObjectSelectionCombo.FormattingEnabled = true;
			this.ObjectSelectionCombo.Location = new System.Drawing.Point(6, 0);
			this.ObjectSelectionCombo.Margin = new System.Windows.Forms.Padding(3, 0, 3, 0);
			this.ObjectSelectionCombo.Name = "ObjectSelectionCombo";
			this.ObjectSelectionCombo.Size = new System.Drawing.Size(270, 21);
			this.ObjectSelectionCombo.TabIndex = 4;
			// 
			// ExtractAllButton
			// 
			this.ExtractAllButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.ExtractAllButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.ExtractAllButton.Location = new System.Drawing.Point(443, 0);
			this.ExtractAllButton.Margin = new System.Windows.Forms.Padding(0);
			this.ExtractAllButton.Name = "ExtractAllButton";
			this.ExtractAllButton.Size = new System.Drawing.Size(70, 23);
			this.ExtractAllButton.TabIndex = 4;
			this.ExtractAllButton.Text = "Extract All";
			this.ExtractAllButton.UseVisualStyleBackColor = true;
			this.ExtractAllButton.Click += new System.EventHandler(this.ExtractAllButton_Click);
			// 
			// SceneObjectGroupBox
			// 
			this.ContentsPanel.Controls.Add(this.MembersPanel);
			this.ContentsPanel.Controls.Add(this.ChildrenPanel);
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Name = "SceneObjectGroupBox";
			this.Size = new System.Drawing.Size(609, 39);
			this.ContainerGroupBox.ResumeLayout(false);
			this.ContainerGroupBox.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		private System.Windows.Forms.FlowLayoutPanel MembersPanel;
		private System.Windows.Forms.FlowLayoutPanel ChildrenPanel;

		private System.Windows.Forms.ComboBox ObjectSelectionCombo;
		private System.Windows.Forms.Button ExtractAllButton;
		#endregion
	}
}
