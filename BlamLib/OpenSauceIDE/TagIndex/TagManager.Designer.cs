/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿namespace OpenSauceIDE
{
	partial class TagManager
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
			System.Windows.Forms.ToolStripMenuItem FileMenu;
			System.Windows.Forms.ToolStripMenuItem FileExit;
			System.Windows.Forms.ToolStripMenuItem ToolsMenu;
			System.Windows.Forms.ColumnHeader TagIndexListViewColumnName;
			System.Windows.Forms.ColumnHeader TagIndexListViewColumnState;
			System.Windows.Forms.ColumnHeader TagIndexListViewColumnRootTag;
			System.Windows.Forms.Button TagIndexListViewAdd;
			this.MainMenu = new System.Windows.Forms.MenuStrip();
			this.OutputBox = new System.Windows.Forms.RichTextBox();
			this.TagIndexListView = new System.Windows.Forms.ListView();
			this.TagIndexListViewMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.TagIndexMenuLoad = new System.Windows.Forms.ToolStripMenuItem();
			this.TagIndexMenuRemove = new System.Windows.Forms.ToolStripMenuItem();
			this.TagIndexMenuView = new System.Windows.Forms.ToolStripMenuItem();
			FileMenu = new System.Windows.Forms.ToolStripMenuItem();
			FileExit = new System.Windows.Forms.ToolStripMenuItem();
			ToolsMenu = new System.Windows.Forms.ToolStripMenuItem();
			TagIndexListViewColumnName = new System.Windows.Forms.ColumnHeader();
			TagIndexListViewColumnState = new System.Windows.Forms.ColumnHeader();
			TagIndexListViewColumnRootTag = new System.Windows.Forms.ColumnHeader();
			TagIndexListViewAdd = new System.Windows.Forms.Button();
			this.MainMenu.SuspendLayout();
			this.TagIndexListViewMenu.SuspendLayout();
			this.SuspendLayout();
			// 
			// FileMenu
			// 
			FileMenu.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            FileExit});
			FileMenu.Name = "FileMenu";
			FileMenu.Size = new System.Drawing.Size(35, 20);
			FileMenu.Text = "File";
			// 
			// FileExit
			// 
			FileExit.Name = "FileExit";
			FileExit.Size = new System.Drawing.Size(92, 22);
			FileExit.Text = "Exit";
			FileExit.Click += new System.EventHandler(this.OnFileExit);
			// 
			// ToolsMenu
			// 
			ToolsMenu.Name = "ToolsMenu";
			ToolsMenu.Size = new System.Drawing.Size(44, 20);
			ToolsMenu.Text = "Tools";
			// 
			// TagIndexListViewColumnName
			// 
			TagIndexListViewColumnName.Text = "Tag Index Name";
			TagIndexListViewColumnName.Width = 127;
			// 
			// TagIndexListViewColumnState
			// 
			TagIndexListViewColumnState.Text = "State";
			TagIndexListViewColumnState.Width = 79;
			// 
			// TagIndexListViewColumnRootTag
			// 
			TagIndexListViewColumnRootTag.Text = "Root Tag";
			TagIndexListViewColumnRootTag.Width = 563;
			// 
			// TagIndexListViewAdd
			// 
			TagIndexListViewAdd.Location = new System.Drawing.Point(0, 27);
			TagIndexListViewAdd.Name = "TagIndexListViewAdd";
			TagIndexListViewAdd.Size = new System.Drawing.Size(75, 23);
			TagIndexListViewAdd.TabIndex = 3;
			TagIndexListViewAdd.Text = "Add Index";
			TagIndexListViewAdd.UseVisualStyleBackColor = true;
			TagIndexListViewAdd.Click += new System.EventHandler(this.OnAddIndex);
			// 
			// MainMenu
			// 
			this.MainMenu.AllowMerge = false;
			this.MainMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            FileMenu,
            ToolsMenu});
			this.MainMenu.Location = new System.Drawing.Point(0, 0);
			this.MainMenu.Name = "MainMenu";
			this.MainMenu.Size = new System.Drawing.Size(792, 24);
			this.MainMenu.TabIndex = 0;
			this.MainMenu.Text = "Main Menu";
			// 
			// OutputBox
			// 
			this.OutputBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.OutputBox.DetectUrls = false;
			this.OutputBox.Location = new System.Drawing.Point(0, 477);
			this.OutputBox.Name = "OutputBox";
			this.OutputBox.ReadOnly = true;
			this.OutputBox.Size = new System.Drawing.Size(792, 96);
			this.OutputBox.TabIndex = 1;
			this.OutputBox.Text = "";
			// 
			// TagIndexListView
			// 
			this.TagIndexListView.Activation = System.Windows.Forms.ItemActivation.TwoClick;
			this.TagIndexListView.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TagIndexListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            TagIndexListViewColumnName,
            TagIndexListViewColumnState,
            TagIndexListViewColumnRootTag});
			this.TagIndexListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
			this.TagIndexListView.HideSelection = false;
			this.TagIndexListView.Location = new System.Drawing.Point(0, 56);
			this.TagIndexListView.Name = "TagIndexListView";
			this.TagIndexListView.Size = new System.Drawing.Size(792, 97);
			this.TagIndexListView.TabIndex = 2;
			this.TagIndexListView.UseCompatibleStateImageBehavior = false;
			this.TagIndexListView.View = System.Windows.Forms.View.Details;
			// 
			// TagIndexListViewMenu
			// 
			this.TagIndexListViewMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.TagIndexMenuLoad,
            this.TagIndexMenuRemove,
            this.TagIndexMenuView});
			this.TagIndexListViewMenu.Name = "TagIndexListViewMenu";
			this.TagIndexListViewMenu.Size = new System.Drawing.Size(114, 70);
			this.TagIndexListViewMenu.Text = "Tag Index Menu";
			// 
			// TagIndexMenuLoad
			// 
			this.TagIndexMenuLoad.Name = "TagIndexMenuLoad";
			this.TagIndexMenuLoad.Size = new System.Drawing.Size(113, 22);
			this.TagIndexMenuLoad.Text = "Load";
			this.TagIndexMenuLoad.Click += new System.EventHandler(this.OnTagIndexLoad);
			// 
			// TagIndexMenuRemove
			// 
			this.TagIndexMenuRemove.Name = "TagIndexMenuRemove";
			this.TagIndexMenuRemove.Size = new System.Drawing.Size(113, 22);
			this.TagIndexMenuRemove.Text = "Remove";
			this.TagIndexMenuRemove.Click += new System.EventHandler(this.OnTagIndexRemove);
			// 
			// TagIndexMenuView
			// 
			this.TagIndexMenuView.Name = "TagIndexMenuView";
			this.TagIndexMenuView.Size = new System.Drawing.Size(113, 22);
			this.TagIndexMenuView.Text = "View";
			this.TagIndexMenuView.Click += new System.EventHandler(this.OnTagIndexView);
			// 
			// TagManager
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(792, 573);
			this.Controls.Add(TagIndexListViewAdd);
			this.Controls.Add(this.TagIndexListView);
			this.Controls.Add(this.OutputBox);
			this.Controls.Add(this.MainMenu);
			this.MainMenuStrip = this.MainMenu;
			this.Name = "TagManager";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Tag Manager";
			this.MainMenu.ResumeLayout(false);
			this.MainMenu.PerformLayout();
			this.TagIndexListViewMenu.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.MenuStrip MainMenu;
		private System.Windows.Forms.RichTextBox OutputBox;
		private System.Windows.Forms.ListView TagIndexListView;
		private System.Windows.Forms.ContextMenuStrip TagIndexListViewMenu;
		private System.Windows.Forms.ToolStripMenuItem TagIndexMenuLoad;
		private System.Windows.Forms.ToolStripMenuItem TagIndexMenuRemove;
		private System.Windows.Forms.ToolStripMenuItem TagIndexMenuView;
	}
}