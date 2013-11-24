namespace OpenSauceIDE.Aether
{
    partial class AetherMainForm
	{

        #region Windows Form Designer generated code


		private void InitializeComponent()
		{
			this.FormFlowLayout = new System.Windows.Forms.FlowLayoutPanel();
			this.SettingsGroupBox = new System.Windows.Forms.GroupBox();
			this.SettingsContainer = new System.Windows.Forms.FlowLayoutPanel();
			this.AetherMenu = new System.Windows.Forms.MenuStrip();
			this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.closeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.ObjectsContainer = new OpenSauceIDE.Aether.Controls.SceneObjectGroupBox();
			this.FormFlowLayout.SuspendLayout();
			this.SettingsGroupBox.SuspendLayout();
			this.AetherMenu.SuspendLayout();
			this.SuspendLayout();
			// 
			// FormFlowLayout
			// 
			this.FormFlowLayout.AutoScroll = true;
			this.FormFlowLayout.Controls.Add(this.SettingsGroupBox);
			this.FormFlowLayout.Controls.Add(this.ObjectsContainer);
			this.FormFlowLayout.Dock = System.Windows.Forms.DockStyle.Fill;
			this.FormFlowLayout.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
			this.FormFlowLayout.Location = new System.Drawing.Point(0, 24);
			this.FormFlowLayout.Margin = new System.Windows.Forms.Padding(0);
			this.FormFlowLayout.Name = "FormFlowLayout";
			this.FormFlowLayout.Size = new System.Drawing.Size(784, 538);
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
			// AetherMainForm
			// 
			this.BackColor = System.Drawing.SystemColors.ControlDarkDark;
			this.ClientSize = new System.Drawing.Size(784, 562);
			this.Controls.Add(this.FormFlowLayout);
			this.Controls.Add(this.AetherMenu);
			this.ForeColor = System.Drawing.Color.LightGreen;
			this.MinimumSize = new System.Drawing.Size(800, 600);
			this.Name = "AetherMainForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Aether";
			this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
			this.FormFlowLayout.ResumeLayout(false);
			this.FormFlowLayout.PerformLayout();
			this.SettingsGroupBox.ResumeLayout(false);
			this.SettingsGroupBox.PerformLayout();
			this.AetherMenu.ResumeLayout(false);
			this.AetherMenu.PerformLayout();
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
	}
}