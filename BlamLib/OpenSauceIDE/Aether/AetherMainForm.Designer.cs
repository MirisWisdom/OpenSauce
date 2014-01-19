namespace OpenSauceIDE.Aether
{
    partial class AetherMainForm
	{

        #region Windows Form Designer generated code


		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			this.FormFlowLayout = new System.Windows.Forms.FlowLayoutPanel();
			this.SettingsGroupBox = new System.Windows.Forms.GroupBox();
			this.SettingsContainer = new System.Windows.Forms.FlowLayoutPanel();
			this.ObjectsContainer = new OpenSauceIDE.Aether.Controls.SceneObjectGroupBox();
			this.AetherMenu = new System.Windows.Forms.MenuStrip();
			this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.closeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.MainSplit = new System.Windows.Forms.SplitContainer();
			this.OutputGroupBox = new System.Windows.Forms.GroupBox();
			this.OutputListBox = new System.Windows.Forms.ListBox();
			this.OutputContextMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.copyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.clearToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.FormFlowLayout.SuspendLayout();
			this.SettingsGroupBox.SuspendLayout();
			this.AetherMenu.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.MainSplit)).BeginInit();
			this.MainSplit.Panel1.SuspendLayout();
			this.MainSplit.Panel2.SuspendLayout();
			this.MainSplit.SuspendLayout();
			this.OutputGroupBox.SuspendLayout();
			this.OutputContextMenu.SuspendLayout();
			this.SuspendLayout();
			// 
			// FormFlowLayout
			// 
			this.FormFlowLayout.AutoScroll = true;
			this.FormFlowLayout.Controls.Add(this.SettingsGroupBox);
			this.FormFlowLayout.Controls.Add(this.ObjectsContainer);
			this.FormFlowLayout.Dock = System.Windows.Forms.DockStyle.Fill;
			this.FormFlowLayout.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
			this.FormFlowLayout.Location = new System.Drawing.Point(0, 0);
			this.FormFlowLayout.Margin = new System.Windows.Forms.Padding(0);
			this.FormFlowLayout.Name = "FormFlowLayout";
			this.FormFlowLayout.Size = new System.Drawing.Size(784, 378);
			this.FormFlowLayout.TabIndex = 0;
			this.FormFlowLayout.WrapContents = false;
			// 
			// SettingsGroupBox
			// 
			this.SettingsGroupBox.AutoSize = true;
			this.SettingsGroupBox.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.SettingsGroupBox.Controls.Add(this.SettingsContainer);
			this.SettingsGroupBox.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.SettingsGroupBox.Location = new System.Drawing.Point(3, 3);
			this.SettingsGroupBox.MinimumSize = new System.Drawing.Size(609, 10);
			this.SettingsGroupBox.Name = "SettingsGroupBox";
			this.SettingsGroupBox.Size = new System.Drawing.Size(609, 19);
			this.SettingsGroupBox.TabIndex = 0;
			this.SettingsGroupBox.TabStop = false;
			this.SettingsGroupBox.Text = "Settings";
			// 
			// SettingsContainer
			// 
			this.SettingsContainer.AutoSize = true;
			this.SettingsContainer.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.SettingsContainer.Dock = System.Windows.Forms.DockStyle.Fill;
			this.SettingsContainer.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
			this.SettingsContainer.Location = new System.Drawing.Point(3, 16);
			this.SettingsContainer.Name = "SettingsContainer";
			this.SettingsContainer.Size = new System.Drawing.Size(603, 0);
			this.SettingsContainer.TabIndex = 0;
			this.SettingsContainer.WrapContents = false;
			// 
			// ObjectsContainer
			// 
			this.ObjectsContainer.AutoSize = true;
			this.ObjectsContainer.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
			this.ObjectsContainer.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.ObjectsContainer.ForeColor = System.Drawing.Color.LightGreen;
			this.ObjectsContainer.Location = new System.Drawing.Point(3, 28);
			this.ObjectsContainer.MinimumSize = new System.Drawing.Size(600, 36);
			this.ObjectsContainer.Name = "ObjectsContainer";
			this.ObjectsContainer.Size = new System.Drawing.Size(609, 48);
			this.ObjectsContainer.TabIndex = 1;
			// 
			// AetherMenu
			// 
			this.AetherMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
			this.AetherMenu.Location = new System.Drawing.Point(0, 0);
			this.AetherMenu.Name = "AetherMenu";
			this.AetherMenu.Size = new System.Drawing.Size(784, 24);
			this.AetherMenu.TabIndex = 1;
			this.AetherMenu.Text = "AetherMenu";
			// 
			// fileToolStripMenuItem
			// 
			this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripMenuItem,
            this.closeToolStripMenuItem,
            this.toolStripSeparator1,
            this.exitToolStripMenuItem});
			this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
			this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
			this.fileToolStripMenuItem.Text = "File";
			// 
			// openToolStripMenuItem
			// 
			this.openToolStripMenuItem.Name = "openToolStripMenuItem";
			this.openToolStripMenuItem.Size = new System.Drawing.Size(103, 22);
			this.openToolStripMenuItem.Text = "Open";
			this.openToolStripMenuItem.Click += new System.EventHandler(this.OpenScenario);
			// 
			// closeToolStripMenuItem
			// 
			this.closeToolStripMenuItem.Name = "closeToolStripMenuItem";
			this.closeToolStripMenuItem.Size = new System.Drawing.Size(103, 22);
			this.closeToolStripMenuItem.Text = "Close";
			this.closeToolStripMenuItem.Click += new System.EventHandler(this.CloseScenario);
			// 
			// toolStripSeparator1
			// 
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			this.toolStripSeparator1.Size = new System.Drawing.Size(100, 6);
			// 
			// exitToolStripMenuItem
			// 
			this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
			this.exitToolStripMenuItem.Size = new System.Drawing.Size(103, 22);
			this.exitToolStripMenuItem.Text = "Exit";
			this.exitToolStripMenuItem.Click += new System.EventHandler(this.Exit);
			// 
			// MainSplit
			// 
			this.MainSplit.Dock = System.Windows.Forms.DockStyle.Fill;
			this.MainSplit.Location = new System.Drawing.Point(0, 24);
			this.MainSplit.Name = "MainSplit";
			this.MainSplit.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// MainSplit.Panel1
			// 
			this.MainSplit.Panel1.Controls.Add(this.FormFlowLayout);
			// 
			// MainSplit.Panel2
			// 
			this.MainSplit.Panel2.Controls.Add(this.OutputGroupBox);
			this.MainSplit.Size = new System.Drawing.Size(784, 538);
			this.MainSplit.SplitterDistance = 378;
			this.MainSplit.TabIndex = 2;
			// 
			// OutputGroupBox
			// 
			this.OutputGroupBox.Controls.Add(this.OutputListBox);
			this.OutputGroupBox.Dock = System.Windows.Forms.DockStyle.Fill;
			this.OutputGroupBox.Location = new System.Drawing.Point(0, 0);
			this.OutputGroupBox.Name = "OutputGroupBox";
			this.OutputGroupBox.Size = new System.Drawing.Size(784, 156);
			this.OutputGroupBox.TabIndex = 0;
			this.OutputGroupBox.TabStop = false;
			this.OutputGroupBox.Text = "Output";
			// 
			// OutputListBox
			// 
			this.OutputListBox.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.OutputListBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.OutputListBox.ContextMenuStrip = this.OutputContextMenu;
			this.OutputListBox.Dock = System.Windows.Forms.DockStyle.Fill;
			this.OutputListBox.ForeColor = System.Drawing.Color.LightGreen;
			this.OutputListBox.FormattingEnabled = true;
			this.OutputListBox.HorizontalScrollbar = true;
			this.OutputListBox.Location = new System.Drawing.Point(3, 16);
			this.OutputListBox.Name = "OutputListBox";
			this.OutputListBox.ScrollAlwaysVisible = true;
			this.OutputListBox.Size = new System.Drawing.Size(778, 137);
			this.OutputListBox.TabIndex = 0;
			// 
			// OutputContextMenu
			// 
			this.OutputContextMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.copyToolStripMenuItem,
            this.clearToolStripMenuItem});
			this.OutputContextMenu.Name = "OutputContextMenu";
			this.OutputContextMenu.Size = new System.Drawing.Size(153, 70);
			// 
			// copyToolStripMenuItem
			// 
			this.copyToolStripMenuItem.Name = "copyToolStripMenuItem";
			this.copyToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.copyToolStripMenuItem.Text = "Copy";
			this.copyToolStripMenuItem.Click += new System.EventHandler(this.CopyOutput);
			// 
			// clearToolStripMenuItem
			// 
			this.clearToolStripMenuItem.Name = "clearToolStripMenuItem";
			this.clearToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
			this.clearToolStripMenuItem.Text = "Clear";
			this.clearToolStripMenuItem.Click += new System.EventHandler(this.ClearOutput);
			// 
			// AetherMainForm
			// 
			this.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.ClientSize = new System.Drawing.Size(784, 562);
			this.Controls.Add(this.MainSplit);
			this.Controls.Add(this.AetherMenu);
			this.ForeColor = System.Drawing.Color.LightGreen;
			this.MinimumSize = new System.Drawing.Size(800, 600);
			this.Name = "AetherMainForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Aether";
			this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ExitClosing);
			this.FormFlowLayout.ResumeLayout(false);
			this.FormFlowLayout.PerformLayout();
			this.SettingsGroupBox.ResumeLayout(false);
			this.SettingsGroupBox.PerformLayout();
			this.AetherMenu.ResumeLayout(false);
			this.AetherMenu.PerformLayout();
			this.MainSplit.Panel1.ResumeLayout(false);
			this.MainSplit.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.MainSplit)).EndInit();
			this.MainSplit.ResumeLayout(false);
			this.OutputGroupBox.ResumeLayout(false);
			this.OutputContextMenu.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion




		private System.Windows.Forms.FlowLayoutPanel FormFlowLayout;
		private System.Windows.Forms.GroupBox SettingsGroupBox;
		private System.Windows.Forms.FlowLayoutPanel SettingsContainer;
		private Controls.SceneObjectGroupBox ObjectsContainer;
		private System.Windows.Forms.MenuStrip AetherMenu;
		private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem closeToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
		private System.Windows.Forms.SplitContainer MainSplit;
		private System.Windows.Forms.GroupBox OutputGroupBox;
		private System.Windows.Forms.ListBox OutputListBox;
		private System.Windows.Forms.ContextMenuStrip OutputContextMenu;
		private System.ComponentModel.IContainer components;
		private System.Windows.Forms.ToolStripMenuItem clearToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem copyToolStripMenuItem;
	}
}