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
			this.components = new System.ComponentModel.Container();
			System.Windows.Forms.SplitContainer splitContainer1;
			System.Windows.Forms.StatusStrip StatusStrip;
			this.TagTreeView = new System.Windows.Forms.TreeView();
			this.PropGrigTag = new System.Windows.Forms.PropertyGrid();
			this.PropGridCache = new System.Windows.Forms.PropertyGrid();
			this.FileOpen = new System.Windows.Forms.ToolStripMenuItem();
			this.StatusProgressBar = new System.Windows.Forms.ToolStripProgressBar();
			this.StatusProgressCancel = new System.Windows.Forms.ToolStripStatusLabel();
			this.StatusProgressText = new System.Windows.Forms.ToolStripStatusLabel();
			this.MainMenu = new System.Windows.Forms.MenuStrip();
			this.FileMenu = new System.Windows.Forms.ToolStripMenuItem();
			this.FileClose = new System.Windows.Forms.ToolStripMenuItem();
			this.ToolsMenu = new System.Windows.Forms.ToolStripMenuItem();
			this.CacheToolsExtractAll = new System.Windows.Forms.ToolStripMenuItem();
			this.CacheToolsExtractAllChecked = new System.Windows.Forms.ToolStripMenuItem();
			this.CacheToolsExtractAllUnchecked = new System.Windows.Forms.ToolStripMenuItem();
			this.CacheToolsDontOverwrite = new System.Windows.Forms.ToolStripMenuItem();
			this.CacheToolsOutputTagDatabase = new System.Windows.Forms.ToolStripMenuItem();
			this.OpenFileDlg = new System.Windows.Forms.OpenFileDialog();
			this.SaveFileDlg = new System.Windows.Forms.SaveFileDialog();
			this.FolderDlg = new System.Windows.Forms.FolderBrowserDialog();
			this.MenuTagInstance = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.MenuTagInstanceExtractAs = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuTagInstanceExtractFolder = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuTagInstanceExtractFolderAll = new System.Windows.Forms.ToolStripMenuItem();
			this.MenuTagInstanceExtractRsrc = new System.Windows.Forms.ToolStripMenuItem();
			this.toolTip1 = new System.Windows.Forms.ToolTip(this.components);
			this.bgwTagExtract = new System.ComponentModel.BackgroundWorker();
			this.bgwCacheOpen = new System.ComponentModel.BackgroundWorker();
			splitContainer1 = new System.Windows.Forms.SplitContainer();
			StatusStrip = new System.Windows.Forms.StatusStrip();
			splitContainer1.Panel1.SuspendLayout();
			splitContainer1.Panel2.SuspendLayout();
			splitContainer1.SuspendLayout();
			StatusStrip.SuspendLayout();
			this.MainMenu.SuspendLayout();
			this.MenuTagInstance.SuspendLayout();
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
			splitContainer1.Panel2.Controls.Add(this.PropGrigTag);
			splitContainer1.Panel2.Controls.Add(this.PropGridCache);
			splitContainer1.Size = new System.Drawing.Size(792, 531);
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
			this.TagTreeView.Size = new System.Drawing.Size(473, 531);
			this.TagTreeView.TabIndex = 1;
			this.TagTreeView.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.OnTagTreeAfterSelect);
			this.TagTreeView.BeforeCheck += new System.Windows.Forms.TreeViewCancelEventHandler(this.OnTagTreeBeforeCheck);
			// 
			// PropGrigTag
			// 
			this.PropGrigTag.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.PropGrigTag.BackColor = System.Drawing.SystemColors.Control;
			this.PropGrigTag.CommandsBackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.PropGrigTag.HelpBackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.PropGrigTag.HelpForeColor = System.Drawing.Color.LightGreen;
			this.PropGrigTag.LineColor = System.Drawing.SystemColors.ControlDarkDark;
			this.PropGrigTag.Location = new System.Drawing.Point(0, 310);
			this.PropGrigTag.Name = "PropGrigTag";
			this.PropGrigTag.PropertySort = System.Windows.Forms.PropertySort.Categorized;
			this.PropGrigTag.Size = new System.Drawing.Size(315, 221);
			this.PropGrigTag.TabIndex = 3;
			this.PropGrigTag.ToolbarVisible = false;
			this.toolTip1.SetToolTip(this.PropGrigTag, "Tag Instance Properties");
			this.PropGrigTag.ViewBackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.PropGrigTag.ViewForeColor = System.Drawing.Color.LightGreen;
			// 
			// PropGridCache
			// 
			this.PropGridCache.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.PropGridCache.BackColor = System.Drawing.SystemColors.Control;
			this.PropGridCache.CommandsBackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.PropGridCache.HelpBackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.PropGridCache.HelpForeColor = System.Drawing.Color.LightGreen;
			this.PropGridCache.LineColor = System.Drawing.SystemColors.ControlDarkDark;
			this.PropGridCache.Location = new System.Drawing.Point(0, 0);
			this.PropGridCache.Name = "PropGridCache";
			this.PropGridCache.PropertySort = System.Windows.Forms.PropertySort.Categorized;
			this.PropGridCache.Size = new System.Drawing.Size(315, 304);
			this.PropGridCache.TabIndex = 2;
			this.PropGridCache.ToolbarVisible = false;
			this.toolTip1.SetToolTip(this.PropGridCache, "Cache Properties");
			this.PropGridCache.ViewBackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.PropGridCache.ViewForeColor = System.Drawing.Color.LightGreen;
			// 
			// FileOpen
			// 
			this.FileOpen.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.FileOpen.ForeColor = System.Drawing.Color.LightGreen;
			this.FileOpen.Name = "FileOpen";
			this.FileOpen.Size = new System.Drawing.Size(152, 22);
			this.FileOpen.Text = "Open";
			this.FileOpen.Click += new System.EventHandler(this.OnFileOpen);
			// 
			// StatusStrip
			// 
			StatusStrip.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			StatusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.StatusProgressBar,
            this.StatusProgressCancel,
            this.StatusProgressText});
			StatusStrip.Location = new System.Drawing.Point(0, 561);
			StatusStrip.Name = "StatusStrip";
			StatusStrip.ShowItemToolTips = true;
			StatusStrip.Size = new System.Drawing.Size(792, 22);
			StatusStrip.TabIndex = 6;
			StatusStrip.Text = "StatusStrip";
			// 
			// StatusProgressBar
			// 
			this.StatusProgressBar.Name = "StatusProgressBar";
			this.StatusProgressBar.Size = new System.Drawing.Size(200, 16);
			this.StatusProgressBar.ToolTipText = "Progress of the current task";
			// 
			// StatusProgressCancel
			// 
			this.StatusProgressCancel.Enabled = false;
			this.StatusProgressCancel.Name = "StatusProgressCancel";
			this.StatusProgressCancel.Size = new System.Drawing.Size(64, 17);
			this.StatusProgressCancel.Text = "Cancel Task";
			this.StatusProgressCancel.ToolTipText = "Cancel the current task";
			// 
			// StatusProgressText
			// 
			this.StatusProgressText.Name = "StatusProgressText";
			this.StatusProgressText.Padding = new System.Windows.Forms.Padding(10, 0, 0, 0);
			this.StatusProgressText.Size = new System.Drawing.Size(511, 17);
			this.StatusProgressText.Spring = true;
			this.StatusProgressText.Text = "trololo";
			this.StatusProgressText.ToolTipText = "Description of the current task";
			this.StatusProgressText.Click += new System.EventHandler(this.OnCancelTask);
			// 
			// MainMenu
			// 
			this.MainMenu.AllowMerge = false;
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
            this.FileOpen,
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
			this.FileClose.Size = new System.Drawing.Size(152, 22);
			this.FileClose.Text = "Close";
			this.FileClose.Click += new System.EventHandler(this.OnFileClose);
			// 
			// ToolsMenu
			// 
			this.ToolsMenu.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.ToolsMenu.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.CacheToolsExtractAll,
            this.CacheToolsDontOverwrite,
            this.CacheToolsOutputTagDatabase});
			this.ToolsMenu.ForeColor = System.Drawing.Color.LightGreen;
			this.ToolsMenu.Name = "ToolsMenu";
			this.ToolsMenu.Size = new System.Drawing.Size(77, 20);
			this.ToolsMenu.Text = "Cache Tools";
			// 
			// CacheToolsExtractAll
			// 
			this.CacheToolsExtractAll.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.CacheToolsExtractAll.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.CacheToolsExtractAllChecked,
            this.CacheToolsExtractAllUnchecked});
			this.CacheToolsExtractAll.Enabled = false;
			this.CacheToolsExtractAll.ForeColor = System.Drawing.Color.LimeGreen;
			this.CacheToolsExtractAll.Name = "CacheToolsExtractAll";
			this.CacheToolsExtractAll.Size = new System.Drawing.Size(178, 22);
			this.CacheToolsExtractAll.Text = "Extract All";
			this.CacheToolsExtractAll.Click += new System.EventHandler(this.OnTagInstanceExtractAll);
			// 
			// CacheToolsExtractAllChecked
			// 
			this.CacheToolsExtractAllChecked.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.CacheToolsExtractAllChecked.ForeColor = System.Drawing.Color.LimeGreen;
			this.CacheToolsExtractAllChecked.Name = "CacheToolsExtractAllChecked";
			this.CacheToolsExtractAllChecked.Size = new System.Drawing.Size(126, 22);
			this.CacheToolsExtractAllChecked.Text = "Checked";
			this.CacheToolsExtractAllChecked.Click += new System.EventHandler(this.OnTagInstanceExtractAll);
			// 
			// CacheToolsExtractAllUnchecked
			// 
			this.CacheToolsExtractAllUnchecked.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.CacheToolsExtractAllUnchecked.ForeColor = System.Drawing.Color.LimeGreen;
			this.CacheToolsExtractAllUnchecked.Name = "CacheToolsExtractAllUnchecked";
			this.CacheToolsExtractAllUnchecked.Size = new System.Drawing.Size(126, 22);
			this.CacheToolsExtractAllUnchecked.Text = "Unchecked";
			this.CacheToolsExtractAllUnchecked.Click += new System.EventHandler(this.OnTagInstanceExtractAll);
			// 
			// CacheToolsDontOverwrite
			// 
			this.CacheToolsDontOverwrite.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.CacheToolsDontOverwrite.Checked = true;
			this.CacheToolsDontOverwrite.CheckOnClick = true;
			this.CacheToolsDontOverwrite.CheckState = System.Windows.Forms.CheckState.Checked;
			this.CacheToolsDontOverwrite.ForeColor = System.Drawing.Color.LimeGreen;
			this.CacheToolsDontOverwrite.Name = "CacheToolsDontOverwrite";
			this.CacheToolsDontOverwrite.Size = new System.Drawing.Size(178, 22);
			this.CacheToolsDontOverwrite.Text = "Don\'t overwrite files";
			this.CacheToolsDontOverwrite.ToolTipText = "Toggles existing file overwriting behavior";
			// 
			// CacheToolsOutputTagDatabase
			// 
			this.CacheToolsOutputTagDatabase.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.CacheToolsOutputTagDatabase.Checked = true;
			this.CacheToolsOutputTagDatabase.CheckOnClick = true;
			this.CacheToolsOutputTagDatabase.CheckState = System.Windows.Forms.CheckState.Checked;
			this.CacheToolsOutputTagDatabase.ForeColor = System.Drawing.Color.LimeGreen;
			this.CacheToolsOutputTagDatabase.Name = "CacheToolsOutputTagDatabase";
			this.CacheToolsOutputTagDatabase.Size = new System.Drawing.Size(178, 22);
			this.CacheToolsOutputTagDatabase.Text = "Output tag_database";
			this.CacheToolsOutputTagDatabase.ToolTipText = "Toggles the creation of a tag_database when extracting groups of tags";
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
			// MenuTagInstance
			// 
			this.MenuTagInstance.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.MenuTagInstance.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.MenuTagInstanceExtractAs,
            this.MenuTagInstanceExtractFolder,
            this.MenuTagInstanceExtractRsrc});
			this.MenuTagInstance.Name = "MenuTagInstance";
			this.MenuTagInstance.Size = new System.Drawing.Size(163, 70);
			this.MenuTagInstance.Opening += new System.ComponentModel.CancelEventHandler(this.OnMenuTagInstanceOpening);
			// 
			// MenuTagInstanceExtractAs
			// 
			this.MenuTagInstanceExtractAs.ForeColor = System.Drawing.Color.LimeGreen;
			this.MenuTagInstanceExtractAs.Name = "MenuTagInstanceExtractAs";
			this.MenuTagInstanceExtractAs.Size = new System.Drawing.Size(162, 22);
			this.MenuTagInstanceExtractAs.Text = "Extract As...";
			this.MenuTagInstanceExtractAs.ToolTipText = "Extract the tag to a user defined filename";
			this.MenuTagInstanceExtractAs.Click += new System.EventHandler(this.OnTagInstanceExtract);
			// 
			// MenuTagInstanceExtractFolder
			// 
			this.MenuTagInstanceExtractFolder.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.MenuTagInstanceExtractFolderAll});
			this.MenuTagInstanceExtractFolder.ForeColor = System.Drawing.Color.LimeGreen;
			this.MenuTagInstanceExtractFolder.Name = "MenuTagInstanceExtractFolder";
			this.MenuTagInstanceExtractFolder.Size = new System.Drawing.Size(162, 22);
			this.MenuTagInstanceExtractFolder.Text = "Extract to folder";
			this.MenuTagInstanceExtractFolder.ToolTipText = "Extract tag to a folder";
			this.MenuTagInstanceExtractFolder.Click += new System.EventHandler(this.OnTagInstanceExtract);
			// 
			// MenuTagInstanceExtractFolderAll
			// 
			this.MenuTagInstanceExtractFolderAll.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.MenuTagInstanceExtractFolderAll.ForeColor = System.Drawing.Color.LimeGreen;
			this.MenuTagInstanceExtractFolderAll.Name = "MenuTagInstanceExtractFolderAll";
			this.MenuTagInstanceExtractFolderAll.Size = new System.Drawing.Size(156, 22);
			this.MenuTagInstanceExtractFolderAll.Text = "With dependents";
			this.MenuTagInstanceExtractFolderAll.ToolTipText = "Extract the tag to a folder with all its dependents";
			this.MenuTagInstanceExtractFolderAll.Click += new System.EventHandler(this.OnTagInstanceExtract);
			// 
			// MenuTagInstanceExtractRsrc
			// 
			this.MenuTagInstanceExtractRsrc.ForeColor = System.Drawing.Color.LimeGreen;
			this.MenuTagInstanceExtractRsrc.Name = "MenuTagInstanceExtractRsrc";
			this.MenuTagInstanceExtractRsrc.Size = new System.Drawing.Size(162, 22);
			this.MenuTagInstanceExtractRsrc.Text = "Extract Resources";
			this.MenuTagInstanceExtractRsrc.Click += new System.EventHandler(this.OnTagInstanceExtract);
			// 
			// bgwTagExtract
			// 
			this.bgwTagExtract.WorkerReportsProgress = true;
			this.bgwTagExtract.WorkerSupportsCancellation = true;
			this.bgwTagExtract.DoWork += new System.ComponentModel.DoWorkEventHandler(this.bgwTagExtractDoWork);
			this.bgwTagExtract.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.bgwTagExtractRunWorkerCompleted);
			this.bgwTagExtract.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.bgwTagExtractProgressChanged);
			// 
			// bgwCacheOpen
			// 
			this.bgwCacheOpen.DoWork += new System.ComponentModel.DoWorkEventHandler(this.bgwCacheOpenDoWork);
			this.bgwCacheOpen.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.bgwCacheOpenRunWorkerCompleted);
			this.bgwCacheOpen.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.bgwCacheOpenProgressChanged);
			// 
			// CacheView
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.ClientSize = new System.Drawing.Size(792, 583);
			this.Controls.Add(splitContainer1);
			this.Controls.Add(this.MainMenu);
			this.Controls.Add(StatusStrip);
			this.ForeColor = System.Drawing.Color.LightGreen;
			this.MainMenuStrip = this.MainMenu;
			this.Name = "CacheView";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Cache Viewer";
			this.Shown += new System.EventHandler(this.OnFormShown);
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.OnFormClosing);
			splitContainer1.Panel1.ResumeLayout(false);
			splitContainer1.Panel2.ResumeLayout(false);
			splitContainer1.ResumeLayout(false);
			StatusStrip.ResumeLayout(false);
			StatusStrip.PerformLayout();
			this.MainMenu.ResumeLayout(false);
			this.MainMenu.PerformLayout();
			this.MenuTagInstance.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.MenuStrip MainMenu;
		private System.Windows.Forms.TreeView TagTreeView;
		private System.Windows.Forms.PropertyGrid PropGridCache;
		private System.Windows.Forms.OpenFileDialog OpenFileDlg;
		private System.Windows.Forms.SaveFileDialog SaveFileDlg;
		private System.Windows.Forms.ToolStripMenuItem FileMenu;
		private System.Windows.Forms.ToolStripMenuItem FileClose;
		private System.Windows.Forms.FolderBrowserDialog FolderDlg;
		private System.Windows.Forms.ToolStripMenuItem ToolsMenu;
		private System.Windows.Forms.PropertyGrid PropGrigTag;
		private System.Windows.Forms.ContextMenuStrip MenuTagInstance;
		private System.Windows.Forms.ToolStripMenuItem MenuTagInstanceExtractAs;
		private System.Windows.Forms.ToolTip toolTip1;
		private System.Windows.Forms.ToolStripMenuItem MenuTagInstanceExtractFolder;
		private System.Windows.Forms.ToolStripMenuItem MenuTagInstanceExtractFolderAll;
		private System.Windows.Forms.ToolStripStatusLabel StatusProgressText;
		private System.Windows.Forms.ToolStripProgressBar StatusProgressBar;
		private System.Windows.Forms.ToolStripMenuItem MenuTagInstanceExtractRsrc;
		private System.Windows.Forms.ToolStripMenuItem CacheToolsExtractAll;
		private System.Windows.Forms.ToolStripMenuItem CacheToolsExtractAllChecked;
		private System.Windows.Forms.ToolStripMenuItem CacheToolsExtractAllUnchecked;
		private System.Windows.Forms.ToolStripMenuItem CacheToolsDontOverwrite;
		private System.Windows.Forms.ToolStripMenuItem CacheToolsOutputTagDatabase;
		private System.ComponentModel.BackgroundWorker bgwTagExtract;
		private System.Windows.Forms.ToolStripStatusLabel StatusProgressCancel;
		private System.ComponentModel.BackgroundWorker bgwCacheOpen;
		private System.Windows.Forms.ToolStripMenuItem FileOpen;
	}
}