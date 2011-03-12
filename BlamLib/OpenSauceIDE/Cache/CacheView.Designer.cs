/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
namespace OpenSauceIDE.Cache
{
	partial class CacheView
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
			System.Windows.Forms.SplitContainer splitContainer1;
			System.Windows.Forms.ToolStripMenuItem FileOpen;
			this.TagTreeView = new System.Windows.Forms.TreeView();
			this.PropGrid = new System.Windows.Forms.PropertyGrid();
			this.MainMenu = new System.Windows.Forms.MenuStrip();
			this.FileMenu = new System.Windows.Forms.ToolStripMenuItem();
			this.FileClose = new System.Windows.Forms.ToolStripMenuItem();
			this.ToolsMenu = new System.Windows.Forms.ToolStripMenuItem();
			this.OpenFileDlg = new System.Windows.Forms.OpenFileDialog();
			this.SaveFileDlg = new System.Windows.Forms.SaveFileDialog();
			this.FolderDlg = new System.Windows.Forms.FolderBrowserDialog();
			splitContainer1 = new System.Windows.Forms.SplitContainer();
			FileOpen = new System.Windows.Forms.ToolStripMenuItem();
			splitContainer1.Panel1.SuspendLayout();
			splitContainer1.Panel2.SuspendLayout();
			splitContainer1.SuspendLayout();
			this.MainMenu.SuspendLayout();
			this.SuspendLayout();
			// 
			// splitContainer1
			// 
			splitContainer1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			splitContainer1.Location = new System.Drawing.Point(0, 27);
			splitContainer1.Name = "splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			splitContainer1.Panel1.Controls.Add(this.TagTreeView);
			// 
			// splitContainer1.Panel2
			// 
			splitContainer1.Panel2.Controls.Add(this.PropGrid);
			splitContainer1.Size = new System.Drawing.Size(792, 534);
			splitContainer1.SplitterDistance = 473;
			splitContainer1.TabIndex = 3;
			// 
			// TagTreeView
			// 
			this.TagTreeView.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.TagTreeView.CheckBoxes = true;
			this.TagTreeView.Dock = System.Windows.Forms.DockStyle.Fill;
			this.TagTreeView.ForeColor = System.Drawing.Color.LightGreen;
			this.TagTreeView.Location = new System.Drawing.Point(0, 0);
			this.TagTreeView.Name = "TagTreeView";
			this.TagTreeView.Size = new System.Drawing.Size(473, 534);
			this.TagTreeView.TabIndex = 1;
			// 
			// PropGrid
			// 
			this.PropGrid.BackColor = System.Drawing.SystemColors.Control;
			this.PropGrid.CommandsBackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.PropGrid.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PropGrid.HelpBackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.PropGrid.HelpForeColor = System.Drawing.Color.LightGreen;
			this.PropGrid.Location = new System.Drawing.Point(0, 0);
			this.PropGrid.Name = "PropGrid";
			this.PropGrid.PropertySort = System.Windows.Forms.PropertySort.Categorized;
			this.PropGrid.Size = new System.Drawing.Size(315, 534);
			this.PropGrid.TabIndex = 2;
			this.PropGrid.ViewBackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.PropGrid.ViewForeColor = System.Drawing.Color.LightGreen;
			// 
			// FileOpen
			// 
			FileOpen.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			FileOpen.ForeColor = System.Drawing.Color.LightGreen;
			FileOpen.Name = "FileOpen";
			FileOpen.Size = new System.Drawing.Size(100, 22);
			FileOpen.Text = "Open";
			FileOpen.Click += new System.EventHandler(this.OnFileOpen);
			// 
			// MainMenu
			// 
			this.MainMenu.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.MainMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.FileMenu,
            this.ToolsMenu});
			this.MainMenu.Location = new System.Drawing.Point(0, 0);
			this.MainMenu.Name = "MainMenu";
			this.MainMenu.Size = new System.Drawing.Size(792, 24);
			this.MainMenu.TabIndex = 0;
			this.MainMenu.Text = "Main Menu";
			// 
			// FileMenu
			// 
			this.FileMenu.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.FileMenu.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            FileOpen,
            this.FileClose});
			this.FileMenu.ForeColor = System.Drawing.Color.LightGreen;
			this.FileMenu.Name = "FileMenu";
			this.FileMenu.Size = new System.Drawing.Size(49, 20);
			this.FileMenu.Text = "Cache";
			// 
			// FileClose
			// 
			this.FileClose.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.FileClose.Enabled = false;
			this.FileClose.ForeColor = System.Drawing.Color.LightGreen;
			this.FileClose.Name = "FileClose";
			this.FileClose.Size = new System.Drawing.Size(100, 22);
			this.FileClose.Text = "Close";
			this.FileClose.Click += new System.EventHandler(this.OnFileClose);
			// 
			// ToolsMenu
			// 
			this.ToolsMenu.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.ToolsMenu.ForeColor = System.Drawing.Color.LightGreen;
			this.ToolsMenu.Name = "ToolsMenu";
			this.ToolsMenu.Size = new System.Drawing.Size(77, 20);
			this.ToolsMenu.Text = "Cache Tools";
			// 
			// OpenFileDlg
			// 
			this.OpenFileDlg.Filter = "Cache files (.map)|*.map|All files|*.*";
			this.OpenFileDlg.Title = "Open Cache file...";
			// 
			// SaveFileDlg
			// 
			this.SaveFileDlg.Title = "Save File";
			// 
			// CacheView
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.ClientSize = new System.Drawing.Size(792, 573);
			this.Controls.Add(splitContainer1);
			this.Controls.Add(this.MainMenu);
			this.ForeColor = System.Drawing.Color.LightGreen;
			this.MainMenuStrip = this.MainMenu;
			this.Name = "CacheView";
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Cache Viewer";
			this.Shown += new System.EventHandler(this.OnFormShown);
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.OnFormClosing);
			splitContainer1.Panel1.ResumeLayout(false);
			splitContainer1.Panel2.ResumeLayout(false);
			splitContainer1.ResumeLayout(false);
			this.MainMenu.ResumeLayout(false);
			this.MainMenu.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.MenuStrip MainMenu;
		private System.Windows.Forms.TreeView TagTreeView;
		private System.Windows.Forms.PropertyGrid PropGrid;
		private System.Windows.Forms.OpenFileDialog OpenFileDlg;
		private System.Windows.Forms.SaveFileDialog SaveFileDlg;
		private System.Windows.Forms.ToolStripMenuItem FileMenu;
		private System.Windows.Forms.ToolStripMenuItem FileClose;
		private System.Windows.Forms.FolderBrowserDialog FolderDlg;
		private System.Windows.Forms.ToolStripMenuItem ToolsMenu;
	}
}