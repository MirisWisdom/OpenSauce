/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
namespace OpenSauceIDE
{
	partial class CheApe
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
			System.Windows.Forms.ToolStripSeparator FileMenuBreak1;
			this.MainMenu = new System.Windows.Forms.MenuStrip();
			this.FileMenu = new System.Windows.Forms.ToolStripMenuItem();
			this.FileNew = new System.Windows.Forms.ToolStripMenuItem();
			this.FileOpen = new System.Windows.Forms.ToolStripMenuItem();
			this.FileClose = new System.Windows.Forms.ToolStripMenuItem();
			this.FileSave = new System.Windows.Forms.ToolStripMenuItem();
			this.FileSaveAs = new System.Windows.Forms.ToolStripMenuItem();
			this.BuildMenu = new System.Windows.Forms.ToolStripMenuItem();
			this.FileViewMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.FileViewAdd = new System.Windows.Forms.ToolStripMenuItem();
			this.FileViewAddNew = new System.Windows.Forms.ToolStripMenuItem();
			this.FileViewAddExisting = new System.Windows.Forms.ToolStripMenuItem();
			this.FileViewRemove = new System.Windows.Forms.ToolStripMenuItem();
			this.FileViewRemoveFromProject = new System.Windows.Forms.ToolStripMenuItem();
			this.FileViewRemoveFromDisk = new System.Windows.Forms.ToolStripMenuItem();
			this.FileView = new System.Windows.Forms.TreeView();
			this.ProjectProperties = new System.Windows.Forms.PropertyGrid();
			this.OpenDialog = new System.Windows.Forms.OpenFileDialog();
			this.OpenProjDialog = new System.Windows.Forms.OpenFileDialog();
			this.SaveDialog = new System.Windows.Forms.SaveFileDialog();
			this.SaveProjDialog = new System.Windows.Forms.SaveFileDialog();
			FileMenuBreak1 = new System.Windows.Forms.ToolStripSeparator();
			this.MainMenu.SuspendLayout();
			this.FileViewMenu.SuspendLayout();
			this.SuspendLayout();
			// 
			// FileMenuBreak1
			// 
			FileMenuBreak1.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			FileMenuBreak1.ForeColor = System.Drawing.Color.LightGreen;
			FileMenuBreak1.Name = "FileMenuBreak1";
			FileMenuBreak1.Size = new System.Drawing.Size(149, 6);
			// 
			// MainMenu
			// 
			this.MainMenu.AllowMerge = false;
			this.MainMenu.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.MainMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.FileMenu,
            this.BuildMenu});
			this.MainMenu.Location = new System.Drawing.Point(0, 0);
			this.MainMenu.Name = "MainMenu";
			this.MainMenu.Size = new System.Drawing.Size(624, 24);
			this.MainMenu.TabIndex = 0;
			this.MainMenu.Text = "Main Menu";
			// 
			// FileMenu
			// 
			this.FileMenu.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.FileMenu.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.FileNew,
            this.FileOpen,
            this.FileClose,
            FileMenuBreak1,
            this.FileSave,
            this.FileSaveAs});
			this.FileMenu.ForeColor = System.Drawing.Color.LightGreen;
			this.FileMenu.Name = "FileMenu";
			this.FileMenu.Size = new System.Drawing.Size(35, 20);
			this.FileMenu.Text = "File";
			// 
			// FileNew
			// 
			this.FileNew.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.FileNew.ForeColor = System.Drawing.Color.LightGreen;
			this.FileNew.Name = "FileNew";
			this.FileNew.Size = new System.Drawing.Size(152, 22);
			this.FileNew.Text = "New";
			this.FileNew.Click += new System.EventHandler(this.OnFileNew);
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
			// FileClose
			// 
			this.FileClose.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.FileClose.ForeColor = System.Drawing.Color.LightGreen;
			this.FileClose.Name = "FileClose";
			this.FileClose.Size = new System.Drawing.Size(152, 22);
			this.FileClose.Text = "Close";
			this.FileClose.Click += new System.EventHandler(this.OnFileClose);
			// 
			// FileSave
			// 
			this.FileSave.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.FileSave.ForeColor = System.Drawing.Color.LightGreen;
			this.FileSave.Name = "FileSave";
			this.FileSave.Size = new System.Drawing.Size(152, 22);
			this.FileSave.Text = "Save";
			this.FileSave.Click += new System.EventHandler(this.OnFileSave);
			// 
			// FileSaveAs
			// 
			this.FileSaveAs.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.FileSaveAs.ForeColor = System.Drawing.Color.LightGreen;
			this.FileSaveAs.Name = "FileSaveAs";
			this.FileSaveAs.Size = new System.Drawing.Size(152, 22);
			this.FileSaveAs.Text = "Save As";
			this.FileSaveAs.Click += new System.EventHandler(this.OnFileSaveAs);
			// 
			// BuildMenu
			// 
			this.BuildMenu.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.BuildMenu.ForeColor = System.Drawing.Color.LightGreen;
			this.BuildMenu.Name = "BuildMenu";
			this.BuildMenu.Size = new System.Drawing.Size(41, 20);
			this.BuildMenu.Text = "Build";
			this.BuildMenu.Click += new System.EventHandler(this.OnBuild);
			// 
			// FileViewMenu
			// 
			this.FileViewMenu.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.FileViewMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.FileViewAdd,
            this.FileViewRemove});
			this.FileViewMenu.Name = "FileViewMenu";
			this.FileViewMenu.Size = new System.Drawing.Size(114, 48);
			this.FileViewMenu.Opening += new System.ComponentModel.CancelEventHandler(this.OnFileViewMenuOpening);
			// 
			// FileViewAdd
			// 
			this.FileViewAdd.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.FileViewAdd.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.FileViewAddNew,
            this.FileViewAddExisting});
			this.FileViewAdd.ForeColor = System.Drawing.Color.LightGreen;
			this.FileViewAdd.Name = "FileViewAdd";
			this.FileViewAdd.Size = new System.Drawing.Size(113, 22);
			this.FileViewAdd.Text = "Add";
			this.FileViewAdd.Click += new System.EventHandler(this.OnFileViewAdd);
			// 
			// FileViewAddNew
			// 
			this.FileViewAddNew.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.FileViewAddNew.ForeColor = System.Drawing.Color.LightGreen;
			this.FileViewAddNew.Name = "FileViewAddNew";
			this.FileViewAddNew.Size = new System.Drawing.Size(111, 22);
			this.FileViewAddNew.Text = "New";
			this.FileViewAddNew.Click += new System.EventHandler(this.OnFileViewAddNew);
			// 
			// FileViewAddExisting
			// 
			this.FileViewAddExisting.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.FileViewAddExisting.ForeColor = System.Drawing.Color.LightGreen;
			this.FileViewAddExisting.Name = "FileViewAddExisting";
			this.FileViewAddExisting.Size = new System.Drawing.Size(111, 22);
			this.FileViewAddExisting.Text = "Existing";
			this.FileViewAddExisting.Click += new System.EventHandler(this.OnFileViewAdd);
			// 
			// FileViewRemove
			// 
			this.FileViewRemove.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.FileViewRemove.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.FileViewRemoveFromProject,
            this.FileViewRemoveFromDisk});
			this.FileViewRemove.ForeColor = System.Drawing.Color.LightGreen;
			this.FileViewRemove.Name = "FileViewRemove";
			this.FileViewRemove.Size = new System.Drawing.Size(113, 22);
			this.FileViewRemove.Text = "Remove";
			this.FileViewRemove.Click += new System.EventHandler(this.OnFileViewRemove);
			// 
			// FileViewRemoveFromProject
			// 
			this.FileViewRemoveFromProject.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.FileViewRemoveFromProject.ForeColor = System.Drawing.Color.LightGreen;
			this.FileViewRemoveFromProject.Name = "FileViewRemoveFromProject";
			this.FileViewRemoveFromProject.Size = new System.Drawing.Size(135, 22);
			this.FileViewRemoveFromProject.Text = "From Project";
			this.FileViewRemoveFromProject.Click += new System.EventHandler(this.OnFileViewRemove);
			// 
			// FileViewRemoveFromDisk
			// 
			this.FileViewRemoveFromDisk.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.FileViewRemoveFromDisk.ForeColor = System.Drawing.Color.LightGreen;
			this.FileViewRemoveFromDisk.Name = "FileViewRemoveFromDisk";
			this.FileViewRemoveFromDisk.Size = new System.Drawing.Size(135, 22);
			this.FileViewRemoveFromDisk.Text = "From Disk";
			this.FileViewRemoveFromDisk.Click += new System.EventHandler(this.OnFileViewRemovePermently);
			// 
			// FileView
			// 
			this.FileView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.FileView.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.FileView.ForeColor = System.Drawing.Color.LightGreen;
			this.FileView.Location = new System.Drawing.Point(0, 27);
			this.FileView.Name = "FileView";
			this.FileView.Size = new System.Drawing.Size(341, 387);
			this.FileView.TabIndex = 2;
			this.FileView.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.OnFileViewKeyPress);
			// 
			// ProjectProperties
			// 
			this.ProjectProperties.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.ProjectProperties.HelpBackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.ProjectProperties.HelpForeColor = System.Drawing.Color.LightGreen;
			this.ProjectProperties.Location = new System.Drawing.Point(347, 27);
			this.ProjectProperties.Name = "ProjectProperties";
			this.ProjectProperties.Size = new System.Drawing.Size(277, 387);
			this.ProjectProperties.TabIndex = 3;
			this.ProjectProperties.ViewBackColor = System.Drawing.SystemColors.ControlDarkDark;
			// 
			// OpenDialog
			// 
			this.OpenDialog.Multiselect = true;
			// 
			// OpenProjDialog
			// 
			this.OpenProjDialog.Title = "Open Project";
			// 
			// SaveProjDialog
			// 
			this.SaveProjDialog.Title = "Save Project";
			// 
			// CheApe
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.ClientSize = new System.Drawing.Size(624, 426);
			this.Controls.Add(this.MainMenu);
			this.Controls.Add(this.ProjectProperties);
			this.Controls.Add(this.FileView);
			this.ForeColor = System.Drawing.Color.LightGreen;
			this.MainMenuStrip = this.MainMenu;
			this.Name = "CheApe";
			this.ShowInTaskbar = false;
			this.Text = "Che Ape";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.OnFormClosing);
			this.MainMenu.ResumeLayout(false);
			this.MainMenu.PerformLayout();
			this.FileViewMenu.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.MenuStrip MainMenu;
		private System.Windows.Forms.ToolStripMenuItem FileMenu;
		private System.Windows.Forms.ToolStripMenuItem FileOpen;
		private System.Windows.Forms.ToolStripMenuItem FileClose;
		private System.Windows.Forms.ToolStripMenuItem FileSave;
		private System.Windows.Forms.ToolStripMenuItem FileSaveAs;
		private System.Windows.Forms.ToolStripMenuItem FileNew;
		private System.Windows.Forms.ToolStripMenuItem BuildMenu;
		private System.Windows.Forms.ContextMenuStrip FileViewMenu;
		private System.Windows.Forms.ToolStripMenuItem FileViewAdd;
		private System.Windows.Forms.ToolStripMenuItem FileViewAddNew;
		private System.Windows.Forms.ToolStripMenuItem FileViewAddExisting;
		private System.Windows.Forms.ToolStripMenuItem FileViewRemove;
		private System.Windows.Forms.ToolStripMenuItem FileViewRemoveFromProject;
		private System.Windows.Forms.ToolStripMenuItem FileViewRemoveFromDisk;
		private System.Windows.Forms.TreeView FileView;
		private System.Windows.Forms.PropertyGrid ProjectProperties;
		private System.Windows.Forms.OpenFileDialog OpenDialog;
		private System.Windows.Forms.OpenFileDialog OpenProjDialog;
		private System.Windows.Forms.SaveFileDialog SaveDialog;
		private System.Windows.Forms.SaveFileDialog SaveProjDialog;
	}
}