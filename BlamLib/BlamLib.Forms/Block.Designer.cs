/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
namespace BlamLib.Forms
{
	partial class Block
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
			this.FieldTopPanel = new System.Windows.Forms.Panel();
			this.FieldIndex = new System.Windows.Forms.ComboBox();
			this.FieldName = new System.Windows.Forms.Label();
			this.FieldPanelSide = new System.Windows.Forms.Panel();
			this.FieldView = new System.Windows.Forms.Panel();
			this.MainMenu = new System.Windows.Forms.MenuStrip();
			this.ShowMenu = new System.Windows.Forms.ToolStripMenuItem();
			this.FileMenu = new System.Windows.Forms.ToolStripMenuItem();
			this.FileLoad = new System.Windows.Forms.ToolStripMenuItem();
			this.FileSave = new System.Windows.Forms.ToolStripMenuItem();
			this.EditMenu = new System.Windows.Forms.ToolStripMenuItem();
			this.EditCopy = new System.Windows.Forms.ToolStripMenuItem();
			this.EditCopyAll = new System.Windows.Forms.ToolStripMenuItem();
			this.EditPaste = new System.Windows.Forms.ToolStripMenuItem();
			this.OptionsMenu = new System.Windows.Forms.ToolStripMenuItem();
			this.SeperaterMenu = new System.Windows.Forms.ToolStripMenuItem();
			this.FieldAdd = new System.Windows.Forms.ToolStripMenuItem();
			this.FieldInsert = new System.Windows.Forms.ToolStripMenuItem();
			this.FieldDuplicate = new System.Windows.Forms.ToolStripMenuItem();
			this.FieldDelete = new System.Windows.Forms.ToolStripMenuItem();
			this.FieldDeleteAll = new System.Windows.Forms.ToolStripMenuItem();
			this.OpenDialog = new System.Windows.Forms.OpenFileDialog();
			this.SaveDialog = new System.Windows.Forms.SaveFileDialog();
			this.FieldTopPanel.SuspendLayout();
			this.MainMenu.SuspendLayout();
			this.SuspendLayout();
			// 
			// FieldTopPanel
			// 
			this.FieldTopPanel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.FieldTopPanel.BackColor = System.Drawing.SystemColors.ControlDark;
			this.FieldTopPanel.Controls.Add(this.FieldIndex);
			this.FieldTopPanel.Controls.Add(this.FieldName);
			this.FieldTopPanel.Location = new System.Drawing.Point(10, 25);
			this.FieldTopPanel.Name = "FieldTopPanel";
			this.FieldTopPanel.Size = new System.Drawing.Size(571, 32);
			this.FieldTopPanel.TabIndex = 0;
			// 
			// FieldIndex
			// 
			this.FieldIndex.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.FieldIndex.Location = new System.Drawing.Point(257, 7);
			this.FieldIndex.Name = "FieldIndex";
			this.FieldIndex.Size = new System.Drawing.Size(300, 21);
			this.FieldIndex.TabIndex = 1;
			this.FieldIndex.SelectionChangeCommitted += new System.EventHandler(this.OnIndexChangeCommited);
			this.FieldIndex.SelectedIndexChanged += new System.EventHandler(this.OnIndexChanged);
			// 
			// FieldName
			// 
			this.FieldName.Font = new System.Drawing.Font("Arial Black", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
			this.FieldName.Location = new System.Drawing.Point(16, 0);
			this.FieldName.Name = "FieldName";
			this.FieldName.Size = new System.Drawing.Size(235, 32);
			this.FieldName.TabIndex = 0;
			this.FieldName.Text = "block";
			this.FieldName.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// FieldPanelSide
			// 
			this.FieldPanelSide.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)));
			this.FieldPanelSide.BackColor = System.Drawing.SystemColors.ControlDark;
			this.FieldPanelSide.Location = new System.Drawing.Point(10, 50);
			this.FieldPanelSide.Name = "FieldPanelSide";
			this.FieldPanelSide.Size = new System.Drawing.Size(10, 67);
			this.FieldPanelSide.TabIndex = 1;
			// 
			// FieldView
			// 
			this.FieldView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.FieldView.Location = new System.Drawing.Point(26, 60);
			this.FieldView.Name = "FieldView";
			this.FieldView.Size = new System.Drawing.Size(555, 57);
			this.FieldView.TabIndex = 2;
			// 
			// MainMenu
			// 
			this.MainMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ShowMenu,
            this.FileMenu,
            this.EditMenu,
            this.OptionsMenu,
            this.SeperaterMenu,
            this.FieldAdd,
            this.FieldInsert,
            this.FieldDuplicate,
            this.FieldDelete,
            this.FieldDeleteAll});
			this.MainMenu.Location = new System.Drawing.Point(0, 0);
			this.MainMenu.Name = "MainMenu";
			this.MainMenu.Size = new System.Drawing.Size(584, 24);
			this.MainMenu.TabIndex = 3;
			this.MainMenu.Text = "menuStrip1";
			// 
			// ShowMenu
			// 
			this.ShowMenu.Name = "ShowMenu";
			this.ShowMenu.Size = new System.Drawing.Size(26, 20);
			this.ShowMenu.Text = " -";
			this.ShowMenu.ToolTipText = "Shows\\Hides this block\'s field view";
			this.ShowMenu.Click += new System.EventHandler(this.OnShowMenuClick);
			// 
			// FileMenu
			// 
			this.FileMenu.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.FileLoad,
            this.FileSave});
			this.FileMenu.Name = "FileMenu";
			this.FileMenu.Size = new System.Drawing.Size(35, 20);
			this.FileMenu.Text = "File";
			// 
			// FileLoad
			// 
			this.FileLoad.Name = "FileLoad";
			this.FileLoad.Size = new System.Drawing.Size(152, 22);
			this.FileLoad.Text = "Load";
			this.FileLoad.ToolTipText = "Loads block data from a file into this block";
			this.FileLoad.Click += new System.EventHandler(this.OnFileLoad);
			// 
			// FileSave
			// 
			this.FileSave.Name = "FileSave";
			this.FileSave.Size = new System.Drawing.Size(152, 22);
			this.FileSave.Text = "Save";
			this.FileSave.ToolTipText = "Saves this block\'s data to a file";
			this.FileSave.Click += new System.EventHandler(this.OnFileSave);
			// 
			// EditMenu
			// 
			this.EditMenu.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.EditCopy,
            this.EditCopyAll,
            this.EditPaste});
			this.EditMenu.Name = "EditMenu";
			this.EditMenu.Size = new System.Drawing.Size(37, 20);
			this.EditMenu.Text = "Edit";
			// 
			// EditCopy
			// 
			this.EditCopy.Name = "EditCopy";
			this.EditCopy.Size = new System.Drawing.Size(152, 22);
			this.EditCopy.Text = "Copy";
			this.EditCopy.ToolTipText = "Copies the current block element data to the clipboard";
			this.EditCopy.Click += new System.EventHandler(this.OnEditCopy);
			// 
			// EditCopyAll
			// 
			this.EditCopyAll.Name = "EditCopyAll";
			this.EditCopyAll.Size = new System.Drawing.Size(152, 22);
			this.EditCopyAll.Text = "Copy All";
			this.EditCopyAll.ToolTipText = "Copies all of this block\'s elements to the clipboard";
			this.EditCopyAll.Click += new System.EventHandler(this.OnEditCopyAll);
			// 
			// EditPaste
			// 
			this.EditPaste.Name = "EditPaste";
			this.EditPaste.Size = new System.Drawing.Size(152, 22);
			this.EditPaste.Text = "Paste";
			this.EditPaste.ToolTipText = "Pastes the data in the clipboard into this block";
			this.EditPaste.Click += new System.EventHandler(this.OnEditPaste);
			// 
			// OptionsMenu
			// 
			this.OptionsMenu.Name = "OptionsMenu";
			this.OptionsMenu.Size = new System.Drawing.Size(56, 20);
			this.OptionsMenu.Text = "Options";
			// 
			// SeperaterMenu
			// 
			this.SeperaterMenu.Enabled = false;
			this.SeperaterMenu.Name = "SeperaterMenu";
			this.SeperaterMenu.Size = new System.Drawing.Size(23, 20);
			this.SeperaterMenu.Text = "|";
			// 
			// FieldAdd
			// 
			this.FieldAdd.Name = "FieldAdd";
			this.FieldAdd.Size = new System.Drawing.Size(38, 20);
			this.FieldAdd.Text = "Add";
			this.FieldAdd.Click += new System.EventHandler(this.OnAdd);
			// 
			// FieldInsert
			// 
			this.FieldInsert.Name = "FieldInsert";
			this.FieldInsert.Size = new System.Drawing.Size(48, 20);
			this.FieldInsert.Text = "Insert";
			this.FieldInsert.Click += new System.EventHandler(this.OnInsert);
			// 
			// FieldDuplicate
			// 
			this.FieldDuplicate.Name = "FieldDuplicate";
			this.FieldDuplicate.Size = new System.Drawing.Size(63, 20);
			this.FieldDuplicate.Text = "Duplicate";
			this.FieldDuplicate.Click += new System.EventHandler(this.OnDuplicate);
			// 
			// FieldDelete
			// 
			this.FieldDelete.Name = "FieldDelete";
			this.FieldDelete.Size = new System.Drawing.Size(50, 20);
			this.FieldDelete.Text = "Delete";
			this.FieldDelete.Click += new System.EventHandler(this.OnDelete);
			// 
			// FieldDeleteAll
			// 
			this.FieldDeleteAll.Name = "FieldDeleteAll";
			this.FieldDeleteAll.Size = new System.Drawing.Size(64, 20);
			this.FieldDeleteAll.Text = "Delete All";
			this.FieldDeleteAll.Click += new System.EventHandler(this.OnDeleteAll);
			// 
			// OpenDialog
			// 
			this.OpenDialog.Title = "Load tag block data";
			// 
			// SaveDialog
			// 
			this.SaveDialog.Title = "Save tag block data";
			// 
			// Block
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.Controls.Add(this.FieldView);
			this.Controls.Add(this.FieldTopPanel);
			this.Controls.Add(this.FieldPanelSide);
			this.Controls.Add(this.MainMenu);
			this.Name = "Block";
			this.Size = new System.Drawing.Size(584, 120);
			this.FieldTopPanel.ResumeLayout(false);
			this.MainMenu.ResumeLayout(false);
			this.MainMenu.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Panel FieldTopPanel;
		private System.Windows.Forms.Label FieldName;
		private System.Windows.Forms.Panel FieldPanelSide;
		private System.Windows.Forms.Panel FieldView;
		private System.Windows.Forms.ComboBox FieldIndex;
		private System.Windows.Forms.MenuStrip MainMenu;
		private System.Windows.Forms.ToolStripMenuItem FileMenu;
		private System.Windows.Forms.ToolStripMenuItem OptionsMenu;
		private System.Windows.Forms.ToolStripMenuItem ShowMenu;
		private System.Windows.Forms.ToolStripMenuItem FileLoad;
		private System.Windows.Forms.ToolStripMenuItem FileSave;
		private System.Windows.Forms.ToolStripMenuItem EditMenu;
		private System.Windows.Forms.ToolStripMenuItem EditCopy;
		private System.Windows.Forms.ToolStripMenuItem EditCopyAll;
		private System.Windows.Forms.ToolStripMenuItem EditPaste;
		private System.Windows.Forms.OpenFileDialog OpenDialog;
		private System.Windows.Forms.SaveFileDialog SaveDialog;
		private System.Windows.Forms.ToolStripMenuItem SeperaterMenu;
		private System.Windows.Forms.ToolStripMenuItem FieldAdd;
		private System.Windows.Forms.ToolStripMenuItem FieldInsert;
		private System.Windows.Forms.ToolStripMenuItem FieldDuplicate;
		private System.Windows.Forms.ToolStripMenuItem FieldDelete;
		private System.Windows.Forms.ToolStripMenuItem FieldDeleteAll;
	}
}
