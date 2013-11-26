namespace OpenSauceIDE.Aether.Controls
{
	partial class SceneObjectExtractableGroupBox
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
			this.ExtractButton = new System.Windows.Forms.Button();
			this.ContainerGroupBox.SuspendLayout();
			this.SuspendLayout();
			// 
			// ContainerGroupBox
			// 
			this.ContainerGroupBox.Controls.Add(this.ExtractButton);
			this.ContainerGroupBox.Controls.SetChildIndex(this.VisibleToggle, 0);
			this.ContainerGroupBox.Controls.SetChildIndex(this.ExtractButton, 0);
			// 
			// ExtractButton
			// 
			this.ExtractButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.ExtractButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.ExtractButton.Location = new System.Drawing.Point(510, 0);
			this.ExtractButton.Margin = new System.Windows.Forms.Padding(0);
			this.ExtractButton.Name = "ExtractButton";
			this.ExtractButton.Size = new System.Drawing.Size(70, 23);
			this.ExtractButton.TabIndex = 3;
			this.ExtractButton.Text = "Extract";
			this.ExtractButton.UseVisualStyleBackColor = true;
			this.ExtractButton.Click += new System.EventHandler(this.ExtractButton_Click);
			// 
			// SceneObjectExtractableGroupBox
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Name = "SceneObjectExtractableGroupBox";
			this.ContainerGroupBox.ResumeLayout(false);
			this.ContainerGroupBox.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		protected System.Windows.Forms.Button ExtractButton;
	}
}
