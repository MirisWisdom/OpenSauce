namespace Yelo_Neighborhood
{
    partial class ScreenshotTool
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
            System.Windows.Forms.ListViewGroup listViewGroup1 = new System.Windows.Forms.ListViewGroup("Screenshots", System.Windows.Forms.HorizontalAlignment.Left);
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ScreenshotTool));
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.checkLiveStream = new System.Windows.Forms.ToolStripMenuItem();
            this.captureF5ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cboViewStyle = new System.Windows.Forms.ToolStripComboBox();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.imageBox = new System.Windows.Forms.PictureBox();
            this.listImages = new System.Windows.Forms.ListView();
            this.imageList = new System.Windows.Forms.ImageList(this.components);
            this.menuStrip2 = new System.Windows.Forms.MenuStrip();
            this.cmdSaveChecked = new System.Windows.Forms.ToolStripMenuItem();
            this.cmdSaveSelected = new System.Windows.Forms.ToolStripMenuItem();
            this.FBD = new System.Windows.Forms.FolderBrowserDialog();
            this.menuStrip1.SuspendLayout();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.imageBox)).BeginInit();
            this.menuStrip2.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.checkLiveStream,
            this.captureF5ToolStripMenuItem,
            this.cboViewStyle});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(809, 27);
            this.menuStrip1.TabIndex = 1;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // checkLiveStream
            // 
            this.checkLiveStream.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
            this.checkLiveStream.CheckOnClick = true;
            this.checkLiveStream.Name = "checkLiveStream";
            this.checkLiveStream.Size = new System.Drawing.Size(108, 23);
            this.checkLiveStream.Text = "Live Stream (Off)";
            this.checkLiveStream.CheckedChanged += new System.EventHandler(this.checkLiveStream_CheckedChanged);
            // 
            // captureF5ToolStripMenuItem
            // 
            this.captureF5ToolStripMenuItem.Name = "captureF5ToolStripMenuItem";
            this.captureF5ToolStripMenuItem.Size = new System.Drawing.Size(84, 23);
            this.captureF5ToolStripMenuItem.Text = "Capture (F5)";
            this.captureF5ToolStripMenuItem.Click += new System.EventHandler(this.captureF5ToolStripMenuItem_Click);
            // 
            // cboViewStyle
            // 
            this.cboViewStyle.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboViewStyle.Name = "cboViewStyle";
            this.cboViewStyle.Size = new System.Drawing.Size(121, 23);
            this.cboViewStyle.SelectedIndexChanged += new System.EventHandler(this.cboViewStyle_SelectedIndexChanged);
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 27);
            this.splitContainer1.Name = "splitContainer1";
            this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.imageBox);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.listImages);
            this.splitContainer1.Panel2.Controls.Add(this.menuStrip2);
            this.splitContainer1.Size = new System.Drawing.Size(809, 697);
            this.splitContainer1.SplitterDistance = 477;
            this.splitContainer1.TabIndex = 2;
            // 
            // imageBox
            // 
            this.imageBox.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
            this.imageBox.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.imageBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.imageBox.Location = new System.Drawing.Point(0, 0);
            this.imageBox.Name = "imageBox";
            this.imageBox.Size = new System.Drawing.Size(809, 477);
            this.imageBox.TabIndex = 1;
            this.imageBox.TabStop = false;
            // 
            // listImages
            // 
            this.listImages.AutoArrange = false;
            this.listImages.CheckBoxes = true;
            this.listImages.Dock = System.Windows.Forms.DockStyle.Fill;
            listViewGroup1.Header = "Screenshots";
            listViewGroup1.Name = "ImagesGroup";
            this.listImages.Groups.AddRange(new System.Windows.Forms.ListViewGroup[] {
            listViewGroup1});
            this.listImages.LabelEdit = true;
            this.listImages.LargeImageList = this.imageList;
            this.listImages.Location = new System.Drawing.Point(0, 24);
            this.listImages.Name = "listImages";
            this.listImages.Size = new System.Drawing.Size(809, 192);
            this.listImages.SmallImageList = this.imageList;
            this.listImages.TabIndex = 0;
            this.listImages.UseCompatibleStateImageBehavior = false;
            this.listImages.ItemSelectionChanged += new System.Windows.Forms.ListViewItemSelectionChangedEventHandler(this.listImages_ItemSelectionChanged);
            // 
            // imageList
            // 
            this.imageList.ColorDepth = System.Windows.Forms.ColorDepth.Depth32Bit;
            this.imageList.ImageSize = new System.Drawing.Size(160, 120);
            this.imageList.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // menuStrip2
            // 
            this.menuStrip2.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.cmdSaveChecked,
            this.cmdSaveSelected});
            this.menuStrip2.Location = new System.Drawing.Point(0, 0);
            this.menuStrip2.Name = "menuStrip2";
            this.menuStrip2.Size = new System.Drawing.Size(809, 24);
            this.menuStrip2.TabIndex = 1;
            this.menuStrip2.Text = "menuStrip2";
            // 
            // cmdSaveChecked
            // 
            this.cmdSaveChecked.Enabled = false;
            this.cmdSaveChecked.Name = "cmdSaveChecked";
            this.cmdSaveChecked.Size = new System.Drawing.Size(92, 20);
            this.cmdSaveChecked.Text = "Save Checked";
            this.cmdSaveChecked.Click += new System.EventHandler(this.cmdSaveChecked_Click);
            // 
            // cmdSaveSelected
            // 
            this.cmdSaveSelected.Enabled = false;
            this.cmdSaveSelected.Name = "cmdSaveSelected";
            this.cmdSaveSelected.Size = new System.Drawing.Size(90, 20);
            this.cmdSaveSelected.Text = "Save Selected";
            this.cmdSaveSelected.Click += new System.EventHandler(this.cmdSaveSelected_Click);
            // 
            // ScreenshotTool
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(809, 724);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.menuStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.KeyPreview = true;
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "ScreenshotTool";
            this.Text = "Screenshot Tool";
            this.TopMost = true;
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ScreenshotTool_FormClosing);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.ScreenshotTool_KeyDown);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.Panel2.PerformLayout();
            this.splitContainer1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.imageBox)).EndInit();
            this.menuStrip2.ResumeLayout(false);
            this.menuStrip2.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem captureF5ToolStripMenuItem;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.PictureBox imageBox;
        private System.Windows.Forms.ListView listImages;
        private System.Windows.Forms.ImageList imageList;
        private System.Windows.Forms.MenuStrip menuStrip2;
        private System.Windows.Forms.ToolStripMenuItem cmdSaveChecked;
        private System.Windows.Forms.ToolStripMenuItem cmdSaveSelected;
        public System.Windows.Forms.ToolStripMenuItem checkLiveStream;
        private System.Windows.Forms.FolderBrowserDialog FBD;
        private System.Windows.Forms.ToolStripComboBox cboViewStyle;
    }
}