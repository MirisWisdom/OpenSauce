namespace Yelo_Neighborhood
{
    partial class Main
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Main));
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.probar = new System.Windows.Forms.ToolStripProgressBar();
            this.lblStatus = new System.Windows.Forms.ToolStripStatusLabel();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.cmdBack = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdForward = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdUpDir = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdRefresh = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdNewFolder = new System.Windows.Forms.ToolStripMenuItem();
            this.listFiles = new System.Windows.Forms.ListView();
            this.mnuFiles = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.cmdRename = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdDelete = new System.Windows.Forms.ToolStripMenuItem();
            this.sepAddToScriptsMenu = new System.Windows.Forms.ToolStripSeparator();
            this.cmdAddToScriptsMenu = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdLaunch = new System.Windows.Forms.ToolStripMenuItem();
            this.imgLstFiles = new System.Windows.Forms.ImageList(this.components);
            this.menuStrip2 = new System.Windows.Forms.MenuStrip();
            this.xBoxToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.screenshotToolToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.systemToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.moduleManagerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.propertiesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.syncTimeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.lEDStateToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.cmdCyclePower = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdReset = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdShutdown = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdCustomReboot = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.cmdEjectTray = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdLoadTray = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdCheckForUpdates = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuScripts = new System.Windows.Forms.ToolStripMenuItem();
            this.launchTitleWorker = new System.ComponentModel.BackgroundWorker();
            this.sendFileWorker = new System.ComponentModel.BackgroundWorker();
            this.deleteFileWorker = new System.ComponentModel.BackgroundWorker();
            this.cmdDownload = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.SFD = new System.Windows.Forms.SaveFileDialog();
            this.downloadFileWorker = new System.ComponentModel.BackgroundWorker();
            this.statusStrip1.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.mnuFiles.SuspendLayout();
            this.menuStrip2.SuspendLayout();
            this.SuspendLayout();
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.probar,
            this.lblStatus});
            this.statusStrip1.Location = new System.Drawing.Point(0, 394);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(662, 22);
            this.statusStrip1.TabIndex = 0;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // probar
            // 
            this.probar.Name = "probar";
            this.probar.Size = new System.Drawing.Size(100, 16);
            // 
            // lblStatus
            // 
            this.lblStatus.Name = "lblStatus";
            this.lblStatus.Size = new System.Drawing.Size(42, 17);
            this.lblStatus.Text = "Ready.";
            // 
            // menuStrip1
            // 
            this.menuStrip1.AutoSize = false;
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.cmdBack,
            this.cmdForward,
            this.cmdUpDir,
            this.cmdRefresh,
            this.cmdNewFolder});
            this.menuStrip1.Location = new System.Drawing.Point(0, 24);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(662, 28);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // cmdBack
            // 
            this.cmdBack.Enabled = false;
            this.cmdBack.Image = ((System.Drawing.Image)(resources.GetObject("cmdBack.Image")));
            this.cmdBack.Name = "cmdBack";
            this.cmdBack.Size = new System.Drawing.Size(60, 24);
            this.cmdBack.Text = "Back";
            this.cmdBack.Click += new System.EventHandler(this.cmdBack_Click);
            // 
            // cmdForward
            // 
            this.cmdForward.Enabled = false;
            this.cmdForward.Image = ((System.Drawing.Image)(resources.GetObject("cmdForward.Image")));
            this.cmdForward.Name = "cmdForward";
            this.cmdForward.Size = new System.Drawing.Size(28, 24);
            this.cmdForward.Click += new System.EventHandler(this.cmdForward_Click);
            // 
            // cmdUpDir
            // 
            this.cmdUpDir.Image = ((System.Drawing.Image)(resources.GetObject("cmdUpDir.Image")));
            this.cmdUpDir.Name = "cmdUpDir";
            this.cmdUpDir.Size = new System.Drawing.Size(28, 24);
            this.cmdUpDir.Click += new System.EventHandler(this.cmdUpDir_Click);
            // 
            // cmdRefresh
            // 
            this.cmdRefresh.Image = ((System.Drawing.Image)(resources.GetObject("cmdRefresh.Image")));
            this.cmdRefresh.Name = "cmdRefresh";
            this.cmdRefresh.Size = new System.Drawing.Size(28, 24);
            this.cmdRefresh.Click += new System.EventHandler(this.cmdRefresh_Click);
            // 
            // cmdNewFolder
            // 
            this.cmdNewFolder.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.cmdNewFolder.Enabled = false;
            this.cmdNewFolder.Image = ((System.Drawing.Image)(resources.GetObject("cmdNewFolder.Image")));
            this.cmdNewFolder.Name = "cmdNewFolder";
            this.cmdNewFolder.Size = new System.Drawing.Size(28, 24);
            this.cmdNewFolder.Text = "New Folder";
            this.cmdNewFolder.Click += new System.EventHandler(this.cmdNewFolder_Click);
            // 
            // listFiles
            // 
            this.listFiles.AllowDrop = true;
            this.listFiles.ContextMenuStrip = this.mnuFiles;
            this.listFiles.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listFiles.LargeImageList = this.imgLstFiles;
            this.listFiles.Location = new System.Drawing.Point(0, 52);
            this.listFiles.Name = "listFiles";
            this.listFiles.Size = new System.Drawing.Size(662, 342);
            this.listFiles.SmallImageList = this.imgLstFiles;
            this.listFiles.Sorting = System.Windows.Forms.SortOrder.Ascending;
            this.listFiles.TabIndex = 2;
            this.listFiles.UseCompatibleStateImageBehavior = false;
            this.listFiles.ItemActivate += new System.EventHandler(this.listFiles_ItemActivate);
            this.listFiles.AfterLabelEdit += new System.Windows.Forms.LabelEditEventHandler(this.listFiles_AfterLabelEdit);
            this.listFiles.DragDrop += new System.Windows.Forms.DragEventHandler(this.listFiles_DragDrop);
            this.listFiles.DragEnter += new System.Windows.Forms.DragEventHandler(this.listFiles_DragEnter);
            this.listFiles.KeyUp += new System.Windows.Forms.KeyEventHandler(this.listFiles_KeyUp);
            // 
            // mnuFiles
            // 
            this.mnuFiles.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.cmdRename,
            this.cmdDelete,
            this.toolStripSeparator5,
            this.cmdDownload,
            this.sepAddToScriptsMenu,
            this.cmdAddToScriptsMenu,
            this.cmdLaunch});
            this.mnuFiles.Name = "mnuFiles";
            this.mnuFiles.Size = new System.Drawing.Size(186, 126);
            this.mnuFiles.Opening += new System.ComponentModel.CancelEventHandler(this.mnuFiles_Opening);
            // 
            // cmdRename
            // 
            this.cmdRename.Name = "cmdRename";
            this.cmdRename.Size = new System.Drawing.Size(185, 22);
            this.cmdRename.Text = "Rename";
            this.cmdRename.Click += new System.EventHandler(this.cmdRename_Click);
            // 
            // cmdDelete
            // 
            this.cmdDelete.Name = "cmdDelete";
            this.cmdDelete.Size = new System.Drawing.Size(185, 22);
            this.cmdDelete.Text = "Delete";
            this.cmdDelete.Click += new System.EventHandler(this.cmdDelete_Click);
            // 
            // sepAddToScriptsMenu
            // 
            this.sepAddToScriptsMenu.Name = "sepAddToScriptsMenu";
            this.sepAddToScriptsMenu.Size = new System.Drawing.Size(182, 6);
            // 
            // cmdAddToScriptsMenu
            // 
            this.cmdAddToScriptsMenu.Name = "cmdAddToScriptsMenu";
            this.cmdAddToScriptsMenu.Size = new System.Drawing.Size(185, 22);
            this.cmdAddToScriptsMenu.Text = "Add To Scripts Menu";
            this.cmdAddToScriptsMenu.Click += new System.EventHandler(this.cmdAddToScriptsMenu_Click);
            // 
            // cmdLaunch
            // 
            this.cmdLaunch.Name = "cmdLaunch";
            this.cmdLaunch.Size = new System.Drawing.Size(185, 22);
            this.cmdLaunch.Text = "Launch";
            this.cmdLaunch.Click += new System.EventHandler(this.ExecutableLaunch_Click);
            // 
            // imgLstFiles
            // 
            this.imgLstFiles.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imgLstFiles.ImageStream")));
            this.imgLstFiles.TransparentColor = System.Drawing.Color.Transparent;
            this.imgLstFiles.Images.SetKeyName(0, "Xbox HD.png");
            this.imgLstFiles.Images.SetKeyName(1, "folder_black.png");
            this.imgLstFiles.Images.SetKeyName(2, "xbe.png");
            this.imgLstFiles.Images.SetKeyName(3, "map.png");
            this.imgLstFiles.Images.SetKeyName(4, "bik.png");
            this.imgLstFiles.Images.SetKeyName(5, "UnknownFile.png");
            // 
            // menuStrip2
            // 
            this.menuStrip2.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.xBoxToolStripMenuItem,
            this.cmdCheckForUpdates,
            this.mnuScripts});
            this.menuStrip2.Location = new System.Drawing.Point(0, 0);
            this.menuStrip2.Name = "menuStrip2";
            this.menuStrip2.Size = new System.Drawing.Size(662, 24);
            this.menuStrip2.TabIndex = 3;
            this.menuStrip2.Text = "menuStrip2";
            // 
            // xBoxToolStripMenuItem
            // 
            this.xBoxToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.screenshotToolToolStripMenuItem,
            this.toolStripSeparator3,
            this.systemToolStripMenuItem,
            this.toolStripSeparator1,
            this.cmdCyclePower,
            this.cmdReset,
            this.cmdShutdown,
            this.cmdCustomReboot,
            this.toolStripSeparator2,
            this.cmdEjectTray,
            this.cmdLoadTray});
            this.xBoxToolStripMenuItem.Name = "xBoxToolStripMenuItem";
            this.xBoxToolStripMenuItem.Size = new System.Drawing.Size(45, 20);
            this.xBoxToolStripMenuItem.Text = "XBox";
            this.xBoxToolStripMenuItem.DropDownOpening += new System.EventHandler(this.xBoxToolStripMenuItem_DropDownOpening);
            // 
            // screenshotToolToolStripMenuItem
            // 
            this.screenshotToolToolStripMenuItem.Name = "screenshotToolToolStripMenuItem";
            this.screenshotToolToolStripMenuItem.Size = new System.Drawing.Size(159, 22);
            this.screenshotToolToolStripMenuItem.Text = "Screenshot Tool";
            this.screenshotToolToolStripMenuItem.Click += new System.EventHandler(this.screenshotToolToolStripMenuItem_Click);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(156, 6);
            // 
            // systemToolStripMenuItem
            // 
            this.systemToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.moduleManagerToolStripMenuItem,
            this.propertiesToolStripMenuItem,
            this.toolStripSeparator4,
            this.syncTimeToolStripMenuItem,
            this.lEDStateToolStripMenuItem});
            this.systemToolStripMenuItem.Name = "systemToolStripMenuItem";
            this.systemToolStripMenuItem.Size = new System.Drawing.Size(159, 22);
            this.systemToolStripMenuItem.Text = "System";
            // 
            // moduleManagerToolStripMenuItem
            // 
            this.moduleManagerToolStripMenuItem.Name = "moduleManagerToolStripMenuItem";
            this.moduleManagerToolStripMenuItem.Size = new System.Drawing.Size(165, 22);
            this.moduleManagerToolStripMenuItem.Text = "Module Manager";
            this.moduleManagerToolStripMenuItem.Click += new System.EventHandler(this.moduleManagerToolStripMenuItem_Click);
            // 
            // propertiesToolStripMenuItem
            // 
            this.propertiesToolStripMenuItem.Name = "propertiesToolStripMenuItem";
            this.propertiesToolStripMenuItem.Size = new System.Drawing.Size(165, 22);
            this.propertiesToolStripMenuItem.Text = "Properties";
            this.propertiesToolStripMenuItem.Click += new System.EventHandler(this.propertiesToolStripMenuItem_Click);
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(162, 6);
            // 
            // syncTimeToolStripMenuItem
            // 
            this.syncTimeToolStripMenuItem.Name = "syncTimeToolStripMenuItem";
            this.syncTimeToolStripMenuItem.Size = new System.Drawing.Size(165, 22);
            this.syncTimeToolStripMenuItem.Text = "Sync Time";
            this.syncTimeToolStripMenuItem.Click += new System.EventHandler(this.syncTimeToolStripMenuItem_Click);
            // 
            // lEDStateToolStripMenuItem
            // 
            this.lEDStateToolStripMenuItem.Name = "lEDStateToolStripMenuItem";
            this.lEDStateToolStripMenuItem.Size = new System.Drawing.Size(165, 22);
            this.lEDStateToolStripMenuItem.Text = "LED State";
            this.lEDStateToolStripMenuItem.Click += new System.EventHandler(this.lEDStateToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(156, 6);
            // 
            // cmdCyclePower
            // 
            this.cmdCyclePower.Name = "cmdCyclePower";
            this.cmdCyclePower.Size = new System.Drawing.Size(159, 22);
            this.cmdCyclePower.Text = "Cycle Power";
            this.cmdCyclePower.Click += new System.EventHandler(this.cmdCyclePower_Click);
            // 
            // cmdReset
            // 
            this.cmdReset.Name = "cmdReset";
            this.cmdReset.Size = new System.Drawing.Size(159, 22);
            this.cmdReset.Text = "Reset";
            this.cmdReset.Click += new System.EventHandler(this.cmdReset_Click);
            // 
            // cmdShutdown
            // 
            this.cmdShutdown.Name = "cmdShutdown";
            this.cmdShutdown.Size = new System.Drawing.Size(159, 22);
            this.cmdShutdown.Text = "Shutdown";
            this.cmdShutdown.Click += new System.EventHandler(this.cmdShutdown_Click);
            // 
            // cmdCustomReboot
            // 
            this.cmdCustomReboot.Name = "cmdCustomReboot";
            this.cmdCustomReboot.Size = new System.Drawing.Size(159, 22);
            this.cmdCustomReboot.Text = "Custom Reboot";
            this.cmdCustomReboot.Visible = false;
            this.cmdCustomReboot.Click += new System.EventHandler(this.cmdCustomReboot_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(156, 6);
            // 
            // cmdEjectTray
            // 
            this.cmdEjectTray.Name = "cmdEjectTray";
            this.cmdEjectTray.Size = new System.Drawing.Size(159, 22);
            this.cmdEjectTray.Text = "Eject Tray";
            this.cmdEjectTray.Click += new System.EventHandler(this.cmdEjectTray_Click);
            // 
            // cmdLoadTray
            // 
            this.cmdLoadTray.Name = "cmdLoadTray";
            this.cmdLoadTray.Size = new System.Drawing.Size(159, 22);
            this.cmdLoadTray.Text = "Load Tray";
            this.cmdLoadTray.Click += new System.EventHandler(this.cmdLoadTray_Click);
            // 
            // cmdCheckForUpdates
            // 
            this.cmdCheckForUpdates.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.cmdCheckForUpdates.Name = "cmdCheckForUpdates";
            this.cmdCheckForUpdates.Size = new System.Drawing.Size(118, 20);
            this.cmdCheckForUpdates.Text = "Check For Updates";
            this.cmdCheckForUpdates.Click += new System.EventHandler(this.cmdCheckForUpdates_Click);
            // 
            // mnuScripts
            // 
            this.mnuScripts.Name = "mnuScripts";
            this.mnuScripts.Size = new System.Drawing.Size(54, 20);
            this.mnuScripts.Text = "Scripts";
            // 
            // launchTitleWorker
            // 
            this.launchTitleWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.launchTitleWorker_DoWork);
            this.launchTitleWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.Worker_RunWorkerCompleted);
            // 
            // sendFileWorker
            // 
            this.sendFileWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.sendFileWorker_DoWork);
            this.sendFileWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.Worker_RunWorkerCompleted);
            // 
            // deleteFileWorker
            // 
            this.deleteFileWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.deleteFileWorker_DoWork);
            this.deleteFileWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.Worker_RunWorkerCompleted);
            // 
            // cmdDownload
            // 
            this.cmdDownload.Name = "cmdDownload";
            this.cmdDownload.Size = new System.Drawing.Size(185, 22);
            this.cmdDownload.Text = "Download...";
            this.cmdDownload.Click += new System.EventHandler(this.cmdDownload_Click);
            // 
            // toolStripSeparator5
            // 
            this.toolStripSeparator5.Name = "toolStripSeparator5";
            this.toolStripSeparator5.Size = new System.Drawing.Size(182, 6);
            // 
            // downloadFileWorker
            // 
            this.downloadFileWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.downloadFileWorker_DoWork);
            this.downloadFileWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.Worker_RunWorkerCompleted);
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(662, 416);
            this.Controls.Add(this.listFiles);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.menuStrip1);
            this.Controls.Add(this.menuStrip2);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.KeyPreview = true;
            this.Name = "Main";
            this.Text = "Yelo Neighborhood";
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.mnuFiles.ResumeLayout(false);
            this.menuStrip2.ResumeLayout(false);
            this.menuStrip2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem cmdBack;
        private System.Windows.Forms.ToolStripMenuItem cmdForward;
        private System.Windows.Forms.ToolStripMenuItem cmdUpDir;
        private System.Windows.Forms.ListView listFiles;
        private System.Windows.Forms.ImageList imgLstFiles;
        private System.Windows.Forms.MenuStrip menuStrip2;
        private System.Windows.Forms.ToolStripMenuItem xBoxToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem screenshotToolToolStripMenuItem;
        private System.ComponentModel.BackgroundWorker launchTitleWorker;
        private System.Windows.Forms.ToolStripMenuItem cmdRefresh;
        private System.ComponentModel.BackgroundWorker sendFileWorker;
        private System.ComponentModel.BackgroundWorker deleteFileWorker;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem cmdCyclePower;
        private System.Windows.Forms.ToolStripMenuItem cmdReset;
        private System.Windows.Forms.ToolStripMenuItem cmdShutdown;
        private System.Windows.Forms.ToolStripMenuItem cmdCustomReboot;
        private System.Windows.Forms.ToolStripMenuItem cmdCheckForUpdates;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripMenuItem cmdEjectTray;
        private System.Windows.Forms.ToolStripMenuItem cmdLoadTray;
        private System.Windows.Forms.ContextMenuStrip mnuFiles;
        private System.Windows.Forms.ToolStripMenuItem cmdRename;
        private System.Windows.Forms.ToolStripMenuItem cmdDelete;
        private System.Windows.Forms.ToolStripMenuItem cmdNewFolder;
        private System.Windows.Forms.ToolStripMenuItem mnuScripts;
        private System.Windows.Forms.ToolStripSeparator sepAddToScriptsMenu;
        private System.Windows.Forms.ToolStripMenuItem cmdAddToScriptsMenu;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripMenuItem systemToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem propertiesToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator4;
        private System.Windows.Forms.ToolStripMenuItem syncTimeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem lEDStateToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem moduleManagerToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cmdLaunch;
        public System.Windows.Forms.ToolStripStatusLabel lblStatus;
        public System.Windows.Forms.ToolStripProgressBar probar;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator5;
        private System.Windows.Forms.ToolStripMenuItem cmdDownload;
        private System.Windows.Forms.SaveFileDialog SFD;
        private System.ComponentModel.BackgroundWorker downloadFileWorker;

    }
}

