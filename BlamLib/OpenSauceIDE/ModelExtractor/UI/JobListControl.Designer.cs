namespace OpenSauceIDE.ModelExtractor.UI
{
	partial class JobListControl
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
			this.mAddJobButton = new System.Windows.Forms.Button();
			this.mClearCompletedJobsBtn = new System.Windows.Forms.Button();
			this.mJobListBox = new System.Windows.Forms.ListBox();
			this.SuspendLayout();
			// 
			// mAddJobButton
			// 
			this.mAddJobButton.Dock = System.Windows.Forms.DockStyle.Top;
			this.mAddJobButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.mAddJobButton.Location = new System.Drawing.Point(0, 0);
			this.mAddJobButton.Name = "mAddJobButton";
			this.mAddJobButton.Size = new System.Drawing.Size(300, 32);
			this.mAddJobButton.TabIndex = 0;
			this.mAddJobButton.Text = "Add Job";
			this.mAddJobButton.UseVisualStyleBackColor = true;
			this.mAddJobButton.Click += new System.EventHandler(this.AddJobButtonClick);
			// 
			// mClearCompletedJobsBtn
			// 
			this.mClearCompletedJobsBtn.Dock = System.Windows.Forms.DockStyle.Bottom;
			this.mClearCompletedJobsBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.mClearCompletedJobsBtn.Location = new System.Drawing.Point(0, 468);
			this.mClearCompletedJobsBtn.Name = "mClearCompletedJobsBtn";
			this.mClearCompletedJobsBtn.Size = new System.Drawing.Size(300, 32);
			this.mClearCompletedJobsBtn.TabIndex = 2;
			this.mClearCompletedJobsBtn.Text = "Clear Completed Jobs";
			this.mClearCompletedJobsBtn.UseVisualStyleBackColor = true;
			this.mClearCompletedJobsBtn.Click += new System.EventHandler(this.ClearCompletedJobs);
			// 
			// mJobListBox
			// 
			this.mJobListBox.Dock = System.Windows.Forms.DockStyle.Fill;
			this.mJobListBox.FormattingEnabled = true;
			this.mJobListBox.Location = new System.Drawing.Point(0, 32);
			this.mJobListBox.Name = "mJobListBox";
			this.mJobListBox.ScrollAlwaysVisible = true;
			this.mJobListBox.SelectionMode = System.Windows.Forms.SelectionMode.None;
			this.mJobListBox.Size = new System.Drawing.Size(300, 436);
			this.mJobListBox.TabIndex = 3;
			// 
			// JobListControl
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.Controls.Add(this.mJobListBox);
			this.Controls.Add(this.mClearCompletedJobsBtn);
			this.Controls.Add(this.mAddJobButton);
			this.ForeColor = System.Drawing.Color.LightGreen;
			this.Name = "JobListControl";
			this.Size = new System.Drawing.Size(300, 500);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Button mAddJobButton;
		private System.Windows.Forms.Button mClearCompletedJobsBtn;
		private System.Windows.Forms.ListBox mJobListBox;
	}
}
