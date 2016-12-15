namespace OpenSauceIDE.Cache
{
	partial class CachePacker
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
			System.Windows.Forms.Label label4;
			System.Windows.Forms.Label label5;
			this.btUnpack = new System.Windows.Forms.Button();
			this.btPathSource = new System.Windows.Forms.Button();
			this.txtPathSource = new System.Windows.Forms.TextBox();
			this.btPack = new System.Windows.Forms.Button();
			this.dlgOpenFile = new System.Windows.Forms.OpenFileDialog();
			this.txtPathOutput = new System.Windows.Forms.TextBox();
			this.btPathOutput = new System.Windows.Forms.Button();
			this.dlgSaveFile = new System.Windows.Forms.SaveFileDialog();
			label4 = new System.Windows.Forms.Label();
			label5 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// label4
			// 
			label4.AutoSize = true;
			label4.ForeColor = System.Drawing.Color.LightGreen;
			label4.Location = new System.Drawing.Point(12, 39);
			label4.Name = "label4";
			label4.Size = new System.Drawing.Size(60, 13);
			label4.TabIndex = 22;
			label4.Text = "Source File";
			// 
			// label5
			// 
			label5.AutoSize = true;
			label5.ForeColor = System.Drawing.Color.LightGreen;
			label5.Location = new System.Drawing.Point(12, 70);
			label5.Name = "label5";
			label5.Size = new System.Drawing.Size(57, 13);
			label5.TabIndex = 27;
			label5.Text = "Target File";
			// 
			// btUnpack
			// 
			this.btUnpack.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btUnpack.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btUnpack.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.btUnpack.ForeColor = System.Drawing.Color.LightGreen;
			this.btUnpack.Location = new System.Drawing.Point(565, 5);
			this.btUnpack.Name = "btUnpack";
			this.btUnpack.Size = new System.Drawing.Size(129, 23);
			this.btUnpack.TabIndex = 25;
			this.btUnpack.Text = "Unpack";
			this.btUnpack.UseVisualStyleBackColor = true;
			this.btUnpack.Click += new System.EventHandler(this.OnPackage);
			// 
			// btPathSource
			// 
			this.btPathSource.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btPathSource.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btPathSource.ForeColor = System.Drawing.Color.LightGreen;
			this.btPathSource.Location = new System.Drawing.Point(662, 34);
			this.btPathSource.Name = "btPathSource";
			this.btPathSource.Size = new System.Drawing.Size(32, 23);
			this.btPathSource.TabIndex = 24;
			this.btPathSource.Text = "...";
			this.btPathSource.UseVisualStyleBackColor = true;
			this.btPathSource.Click += new System.EventHandler(this.OnPathSource);
			// 
			// txtPathSource
			// 
			this.txtPathSource.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.txtPathSource.BackColor = System.Drawing.Color.Gray;
			this.txtPathSource.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.txtPathSource.ForeColor = System.Drawing.Color.LightGreen;
			this.txtPathSource.Location = new System.Drawing.Point(96, 36);
			this.txtPathSource.Name = "txtPathSource";
			this.txtPathSource.ReadOnly = true;
			this.txtPathSource.Size = new System.Drawing.Size(561, 20);
			this.txtPathSource.TabIndex = 23;
			// 
			// btPack
			// 
			this.btPack.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btPack.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btPack.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.btPack.ForeColor = System.Drawing.Color.LightGreen;
			this.btPack.Location = new System.Drawing.Point(430, 5);
			this.btPack.Name = "btPack";
			this.btPack.Size = new System.Drawing.Size(129, 23);
			this.btPack.TabIndex = 26;
			this.btPack.Text = "Pack";
			this.btPack.UseVisualStyleBackColor = true;
			this.btPack.Click += new System.EventHandler(this.OnPackage);
			// 
			// dlgOpenFile
			// 
			this.dlgOpenFile.Filter = "Cache files|*.map|Saber3D Pak files|*.s3dpak";
			this.dlgOpenFile.Title = "Select File...";
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
			this.txtPathOutput.TabIndex = 28;
			// 
			// btPathOutput
			// 
			this.btPathOutput.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btPathOutput.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.btPathOutput.ForeColor = System.Drawing.Color.LightGreen;
			this.btPathOutput.Location = new System.Drawing.Point(662, 65);
			this.btPathOutput.Name = "btPathOutput";
			this.btPathOutput.Size = new System.Drawing.Size(32, 23);
			this.btPathOutput.TabIndex = 29;
			this.btPathOutput.Text = "...";
			this.btPathOutput.UseVisualStyleBackColor = true;
			this.btPathOutput.Click += new System.EventHandler(this.OnPathOutput);
			// 
			// dlgSaveFile
			// 
			this.dlgSaveFile.Filter = "Cache files|*.map|Saber3D Pak files|*.s3dpak|All files|*.*";
			this.dlgSaveFile.Title = "Save As...";
			// 
			// CachePacker
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.ClientSize = new System.Drawing.Size(706, 96);
			this.Controls.Add(this.txtPathOutput);
			this.Controls.Add(this.btPathOutput);
			this.Controls.Add(label5);
			this.Controls.Add(this.btPack);
			this.Controls.Add(this.btUnpack);
			this.Controls.Add(this.btPathSource);
			this.Controls.Add(this.txtPathSource);
			this.Controls.Add(label4);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "CachePacker";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "CachePacker";
			this.TopMost = true;
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button btUnpack;
		private System.Windows.Forms.Button btPathSource;
		private System.Windows.Forms.TextBox txtPathSource;
		private System.Windows.Forms.Button btPack;
		private System.Windows.Forms.OpenFileDialog dlgOpenFile;
		private System.Windows.Forms.TextBox txtPathOutput;
		private System.Windows.Forms.Button btPathOutput;
		private System.Windows.Forms.SaveFileDialog dlgSaveFile;

	}
}