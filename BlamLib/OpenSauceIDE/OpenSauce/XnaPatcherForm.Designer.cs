namespace OpenSauceIDE.OpenSauce
{
	partial class XnaPatcherForm
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
			System.Windows.Forms.Label label5;
			System.Windows.Forms.Label label4;
			this.btApply = new System.Windows.Forms.Button();
			this.dlgOpenFile = new System.Windows.Forms.OpenFileDialog();
			this.txtPathOutput = new System.Windows.Forms.TextBox();
			this.dlgFolderBrowser = new System.Windows.Forms.FolderBrowserDialog();
			this.btPathOutput = new System.Windows.Forms.Button();
			this.btPathXnaNative = new System.Windows.Forms.Button();
			this.txtPathXnaNative = new System.Windows.Forms.TextBox();
			label5 = new System.Windows.Forms.Label();
			label4 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// btApply
			// 
			this.btApply.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btApply.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btApply.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.btApply.ForeColor = System.Drawing.Color.LightGreen;
			this.btApply.Location = new System.Drawing.Point(565, 5);
			this.btApply.Name = "btApply";
			this.btApply.Size = new System.Drawing.Size(129, 23);
			this.btApply.TabIndex = 21;
			this.btApply.Text = "Apply";
			this.btApply.UseVisualStyleBackColor = true;
			this.btApply.Click += new System.EventHandler(this.OnApply);
			// 
			// dlgOpenFile
			// 
			this.dlgOpenFile.Filter = "EXE files|*.exe";
			this.dlgOpenFile.Title = "Select Exe...";
			// 
			// txtPathOutput
			// 
			this.txtPathOutput.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.txtPathOutput.BackColor = System.Drawing.Color.Gray;
			this.txtPathOutput.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.txtPathOutput.ForeColor = System.Drawing.Color.LightGreen;
			this.txtPathOutput.Location = new System.Drawing.Point(96, 67);
			this.txtPathOutput.Name = "txtPathOutput";
			this.txtPathOutput.ReadOnly = true;
			this.txtPathOutput.Size = new System.Drawing.Size(561, 20);
			this.txtPathOutput.TabIndex = 19;
			// 
			// dlgFolderBrowser
			// 
			this.dlgFolderBrowser.Description = "Select output path...";
			this.dlgFolderBrowser.RootFolder = System.Environment.SpecialFolder.MyComputer;
			// 
			// btPathOutput
			// 
			this.btPathOutput.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btPathOutput.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btPathOutput.ForeColor = System.Drawing.Color.LightGreen;
			this.btPathOutput.Location = new System.Drawing.Point(662, 65);
			this.btPathOutput.Name = "btPathOutput";
			this.btPathOutput.Size = new System.Drawing.Size(32, 23);
			this.btPathOutput.TabIndex = 20;
			this.btPathOutput.Text = "...";
			this.btPathOutput.UseVisualStyleBackColor = true;
			this.btPathOutput.Click += new System.EventHandler(this.OnPathOutput);
			// 
			// label5
			// 
			label5.AutoSize = true;
			label5.ForeColor = System.Drawing.Color.LightGreen;
			label5.Location = new System.Drawing.Point(12, 70);
			label5.Name = "label5";
			label5.Size = new System.Drawing.Size(64, 13);
			label5.TabIndex = 18;
			label5.Text = "Output Path";
			// 
			// btPathXnaNative
			// 
			this.btPathXnaNative.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btPathXnaNative.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btPathXnaNative.ForeColor = System.Drawing.Color.LightGreen;
			this.btPathXnaNative.Location = new System.Drawing.Point(662, 34);
			this.btPathXnaNative.Name = "btPathXnaNative";
			this.btPathXnaNative.Size = new System.Drawing.Size(32, 23);
			this.btPathXnaNative.TabIndex = 17;
			this.btPathXnaNative.Text = "...";
			this.btPathXnaNative.UseVisualStyleBackColor = true;
			this.btPathXnaNative.Click += new System.EventHandler(this.OnPathXnaNative);
			// 
			// txtPathXnaNative
			// 
			this.txtPathXnaNative.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.txtPathXnaNative.BackColor = System.Drawing.Color.Gray;
			this.txtPathXnaNative.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.txtPathXnaNative.ForeColor = System.Drawing.Color.LightGreen;
			this.txtPathXnaNative.Location = new System.Drawing.Point(96, 36);
			this.txtPathXnaNative.Name = "txtPathXnaNative";
			this.txtPathXnaNative.ReadOnly = true;
			this.txtPathXnaNative.Size = new System.Drawing.Size(561, 20);
			this.txtPathXnaNative.TabIndex = 16;
			// 
			// label4
			// 
			label4.AutoSize = true;
			label4.ForeColor = System.Drawing.Color.LightGreen;
			label4.Location = new System.Drawing.Point(12, 39);
			label4.Name = "label4";
			label4.Size = new System.Drawing.Size(82, 13);
			label4.TabIndex = 15;
			label4.Text = "XnaNative Path";
			// 
			// XnaPatcherForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.ClientSize = new System.Drawing.Size(706, 96);
			this.Controls.Add(this.btApply);
			this.Controls.Add(this.txtPathOutput);
			this.Controls.Add(this.btPathOutput);
			this.Controls.Add(label5);
			this.Controls.Add(this.btPathXnaNative);
			this.Controls.Add(this.txtPathXnaNative);
			this.Controls.Add(label4);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "XnaPatcherForm";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "XnaPatcher";
			this.TopMost = true;
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button btApply;
		private System.Windows.Forms.OpenFileDialog dlgOpenFile;
		private System.Windows.Forms.TextBox txtPathOutput;
		private System.Windows.Forms.FolderBrowserDialog dlgFolderBrowser;
		private System.Windows.Forms.Button btPathOutput;
		private System.Windows.Forms.Button btPathXnaNative;
		private System.Windows.Forms.TextBox txtPathXnaNative;
	}
}