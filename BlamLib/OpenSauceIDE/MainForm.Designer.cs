/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
namespace OpenSauceIDE
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
			System.Windows.Forms.ToolStripMenuItem FileMenu;
			System.Windows.Forms.ToolStripSeparator ToolsMenuBreak1;
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
			this.FileExit = new System.Windows.Forms.ToolStripMenuItem();
			this.ToolsMenu = new System.Windows.Forms.ToolStripMenuItem();
			this.ToolsHalo1 = new System.Windows.Forms.ToolStripMenuItem();
			this.ToolsHalo2 = new System.Windows.Forms.ToolStripMenuItem();
			this.ToolsHalo3 = new System.Windows.Forms.ToolStripMenuItem();
			this.ToolsStubbs = new System.Windows.Forms.ToolStripMenuItem();
			this.MainMenu = new System.Windows.Forms.MenuStrip();
			this.WindowsMenu = new System.Windows.Forms.ToolStripMenuItem();
			FileMenu = new System.Windows.Forms.ToolStripMenuItem();
			ToolsMenuBreak1 = new System.Windows.Forms.ToolStripSeparator();
			this.MainMenu.SuspendLayout();
			this.SuspendLayout();
			// 
			// FileMenu
			// 
			FileMenu.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			FileMenu.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.FileExit});
			FileMenu.ForeColor = System.Drawing.Color.LightGreen;
			FileMenu.Name = "FileMenu";
			FileMenu.Size = new System.Drawing.Size(35, 20);
			FileMenu.Text = "&File";
			// 
			// FileExit
			// 
			this.FileExit.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.FileExit.ForeColor = System.Drawing.Color.LightGreen;
			this.FileExit.Name = "FileExit";
			this.FileExit.Size = new System.Drawing.Size(92, 22);
			this.FileExit.Text = "Exit";
			this.FileExit.Click += new System.EventHandler(this.OnFileExit);
			// 
			// ToolsMenuBreak1
			// 
			ToolsMenuBreak1.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			ToolsMenuBreak1.ForeColor = System.Drawing.Color.LightGreen;
			ToolsMenuBreak1.Name = "ToolsMenuBreak1";
			ToolsMenuBreak1.Size = new System.Drawing.Size(104, 6);
			// 
			// ToolsMenu
			// 
			this.ToolsMenu.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.ToolsMenu.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ToolsHalo1,
            this.ToolsHalo2,
            this.ToolsHalo3,
            this.ToolsStubbs,
            ToolsMenuBreak1});
			this.ToolsMenu.ForeColor = System.Drawing.Color.LightGreen;
			this.ToolsMenu.Name = "ToolsMenu";
			this.ToolsMenu.Size = new System.Drawing.Size(44, 20);
			this.ToolsMenu.Text = "Tools";
			// 
			// ToolsHalo1
			// 
			this.ToolsHalo1.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.ToolsHalo1.ForeColor = System.Drawing.Color.LightGreen;
			this.ToolsHalo1.Name = "ToolsHalo1";
			this.ToolsHalo1.Size = new System.Drawing.Size(107, 22);
			this.ToolsHalo1.Text = "Halo1";
			// 
			// ToolsHalo2
			// 
			this.ToolsHalo2.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.ToolsHalo2.Enabled = false;
			this.ToolsHalo2.ForeColor = System.Drawing.Color.LightGreen;
			this.ToolsHalo2.Name = "ToolsHalo2";
			this.ToolsHalo2.Size = new System.Drawing.Size(107, 22);
			this.ToolsHalo2.Text = "Halo2";
			// 
			// ToolsHalo3
			// 
			this.ToolsHalo3.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.ToolsHalo3.Enabled = false;
			this.ToolsHalo3.ForeColor = System.Drawing.Color.LightGreen;
			this.ToolsHalo3.Name = "ToolsHalo3";
			this.ToolsHalo3.Size = new System.Drawing.Size(107, 22);
			this.ToolsHalo3.Text = "Halo3";
			// 
			// ToolsStubbs
			// 
			this.ToolsStubbs.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.ToolsStubbs.Enabled = false;
			this.ToolsStubbs.ForeColor = System.Drawing.Color.LightGreen;
			this.ToolsStubbs.Name = "ToolsStubbs";
			this.ToolsStubbs.Size = new System.Drawing.Size(107, 22);
			this.ToolsStubbs.Text = "Stubbs";
			// 
			// MainMenu
			// 
			this.MainMenu.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.MainMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            FileMenu,
            this.ToolsMenu,
            this.WindowsMenu});
			this.MainMenu.Location = new System.Drawing.Point(0, 0);
			this.MainMenu.MdiWindowListItem = this.WindowsMenu;
			this.MainMenu.Name = "MainMenu";
			this.MainMenu.Size = new System.Drawing.Size(792, 24);
			this.MainMenu.TabIndex = 1;
			this.MainMenu.Text = "Main Menu";
			// 
			// WindowsMenu
			// 
			this.WindowsMenu.ForeColor = System.Drawing.Color.LightGreen;
			this.WindowsMenu.Name = "WindowsMenu";
			this.WindowsMenu.Size = new System.Drawing.Size(62, 20);
			this.WindowsMenu.Text = "Windows";
			// 
			// MainForm
			// 
			this.AllowDrop = true;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(792, 573);
			this.Controls.Add(this.MainMenu);
			this.ForeColor = System.Drawing.Color.LightGreen;
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.IsMdiContainer = true;
			this.MainMenuStrip = this.MainMenu;
			this.Name = "MainForm";
			this.Text = "OpenSauce IDE";
			this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
			this.Load += new System.EventHandler(this.OnFormLoad);
			this.DragLeave += new System.EventHandler(this.OnDragLeave);
			this.DragDrop += new System.Windows.Forms.DragEventHandler(this.OnDragDrop);
			this.DragEnter += new System.Windows.Forms.DragEventHandler(this.OnDragEnter);
			this.GiveFeedback += new System.Windows.Forms.GiveFeedbackEventHandler(this.OnDragGiveFeedback);
			this.QueryContinueDrag += new System.Windows.Forms.QueryContinueDragEventHandler(this.OnDragQueryContinueDrag);
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.OnFormClosing);
			this.DragOver += new System.Windows.Forms.DragEventHandler(this.OnDragOver);
			this.MainMenu.ResumeLayout(false);
			this.MainMenu.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.MenuStrip MainMenu;
		private System.Windows.Forms.ToolStripMenuItem FileExit;
		private System.Windows.Forms.ToolStripMenuItem ToolsHalo1;
		private System.Windows.Forms.ToolStripMenuItem ToolsHalo2;
		private System.Windows.Forms.ToolStripMenuItem ToolsHalo3;
		private System.Windows.Forms.ToolStripMenuItem ToolsStubbs;
		private System.Windows.Forms.ToolStripMenuItem ToolsMenu;
		private System.Windows.Forms.ToolStripMenuItem WindowsMenu;
	}
}

