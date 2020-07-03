namespace LaymansXboxEmulator
{
    partial class Form1
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

            if (Video != null) Video.End();
            if (Xbox != null) Xbox.Disconnect();
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.renderWindow = new System.Windows.Forms.PictureBox();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.status = new System.Windows.Forms.ToolStripStatusLabel();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.mnuXbox = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuStart = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuPause = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuStep = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuStop = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuReset = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.mnuScreenshot = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.mnuExit = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuSettings = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuInput = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuX360Input = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuGamepadInput = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuKeyboardInput = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuNone = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuResolution = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuMediumResolution = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuFullResolution = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuCustomWindow = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuReboot = new System.Windows.Forms.ToolStripMenuItem();
            ((System.ComponentModel.ISupportInitialize)(this.renderWindow)).BeginInit();
            this.statusStrip1.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // renderWindow
            // 
            this.renderWindow.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.renderWindow.Location = new System.Drawing.Point(0, 24);
            this.renderWindow.Name = "renderWindow";
            this.renderWindow.Size = new System.Drawing.Size(320, 245);
            this.renderWindow.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.renderWindow.TabIndex = 0;
            this.renderWindow.TabStop = false;
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.status});
            this.statusStrip1.Location = new System.Drawing.Point(0, 263);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(322, 22);
            this.statusStrip1.TabIndex = 1;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // status
            // 
            this.status.Name = "status";
            this.status.Size = new System.Drawing.Size(79, 17);
            this.status.Text = "Disconnected";
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuXbox,
            this.mnuSettings});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(322, 24);
            this.menuStrip1.TabIndex = 2;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // mnuXbox
            // 
            this.mnuXbox.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuStart,
            this.mnuPause,
            this.mnuStep,
            this.mnuStop,
            this.mnuReset,
            this.toolStripSeparator1,
            this.mnuScreenshot,
            this.mnuReboot,
            this.toolStripSeparator2,
            this.mnuExit});
            this.mnuXbox.Name = "mnuXbox";
            this.mnuXbox.Size = new System.Drawing.Size(45, 20);
            this.mnuXbox.Text = "Xbox";
            // 
            // mnuStart
            // 
            this.mnuStart.Name = "mnuStart";
            this.mnuStart.ShortcutKeys = System.Windows.Forms.Keys.F1;
            this.mnuStart.Size = new System.Drawing.Size(172, 22);
            this.mnuStart.Text = "Start";
            this.mnuStart.Click += new System.EventHandler(this.mnuStart_Click);
            // 
            // mnuPause
            // 
            this.mnuPause.Name = "mnuPause";
            this.mnuPause.ShortcutKeys = System.Windows.Forms.Keys.F2;
            this.mnuPause.Size = new System.Drawing.Size(172, 22);
            this.mnuPause.Text = "Pause";
            this.mnuPause.Click += new System.EventHandler(this.mnuPause_Click);
            // 
            // mnuStep
            // 
            this.mnuStep.Enabled = false;
            this.mnuStep.Name = "mnuStep";
            this.mnuStep.ShortcutKeys = System.Windows.Forms.Keys.F3;
            this.mnuStep.Size = new System.Drawing.Size(172, 22);
            this.mnuStep.Text = "Step";
            this.mnuStep.Click += new System.EventHandler(this.mnuStep_Click);
            // 
            // mnuStop
            // 
            this.mnuStop.Name = "mnuStop";
            this.mnuStop.ShortcutKeys = System.Windows.Forms.Keys.F4;
            this.mnuStop.Size = new System.Drawing.Size(172, 22);
            this.mnuStop.Text = "Stop";
            this.mnuStop.Click += new System.EventHandler(this.mnuStop_Click);
            // 
            // mnuReset
            // 
            this.mnuReset.Enabled = false;
            this.mnuReset.Name = "mnuReset";
            this.mnuReset.ShortcutKeys = System.Windows.Forms.Keys.F5;
            this.mnuReset.Size = new System.Drawing.Size(172, 22);
            this.mnuReset.Text = "Reset";
            this.mnuReset.Click += new System.EventHandler(this.mnuReset_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(169, 6);
            // 
            // mnuScreenshot
            // 
            this.mnuScreenshot.Name = "mnuScreenshot";
            this.mnuScreenshot.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.mnuScreenshot.Size = new System.Drawing.Size(172, 22);
            this.mnuScreenshot.Text = "Screenshot";
            this.mnuScreenshot.Click += new System.EventHandler(this.mnuScreenshot_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(169, 6);
            // 
            // mnuExit
            // 
            this.mnuExit.Name = "mnuExit";
            this.mnuExit.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.X)));
            this.mnuExit.Size = new System.Drawing.Size(172, 22);
            this.mnuExit.Text = "Exit";
            this.mnuExit.Click += new System.EventHandler(this.mnuExit_Click);
            // 
            // mnuSettings
            // 
            this.mnuSettings.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuInput,
            this.mnuResolution,
            this.mnuCustomWindow});
            this.mnuSettings.Name = "mnuSettings";
            this.mnuSettings.Size = new System.Drawing.Size(61, 20);
            this.mnuSettings.Text = "Settings";
            // 
            // mnuInput
            // 
            this.mnuInput.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuX360Input,
            this.mnuGamepadInput,
            this.mnuKeyboardInput,
            this.mnuNone});
            this.mnuInput.Name = "mnuInput";
            this.mnuInput.Size = new System.Drawing.Size(163, 22);
            this.mnuInput.Text = "Input";
            // 
            // mnuX360Input
            // 
            this.mnuX360Input.Name = "mnuX360Input";
            this.mnuX360Input.Size = new System.Drawing.Size(177, 22);
            this.mnuX360Input.Text = "Xbox 360 Controller";
            this.mnuX360Input.Click += new System.EventHandler(this.mnuX360Input_Click);
            // 
            // mnuGamepadInput
            // 
            this.mnuGamepadInput.Enabled = false;
            this.mnuGamepadInput.Name = "mnuGamepadInput";
            this.mnuGamepadInput.Size = new System.Drawing.Size(177, 22);
            this.mnuGamepadInput.Text = "Gamepad";
            // 
            // mnuKeyboardInput
            // 
            this.mnuKeyboardInput.Name = "mnuKeyboardInput";
            this.mnuKeyboardInput.Size = new System.Drawing.Size(177, 22);
            this.mnuKeyboardInput.Text = "Keyboard";
            this.mnuKeyboardInput.Click += new System.EventHandler(this.mnuKeyboardInput_Click);
            // 
            // mnuNone
            // 
            this.mnuNone.Checked = true;
            this.mnuNone.CheckState = System.Windows.Forms.CheckState.Checked;
            this.mnuNone.Name = "mnuNone";
            this.mnuNone.Size = new System.Drawing.Size(177, 22);
            this.mnuNone.Text = "None";
            this.mnuNone.Click += new System.EventHandler(this.mnuNone_Click);
            // 
            // mnuResolution
            // 
            this.mnuResolution.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuMediumResolution,
            this.mnuFullResolution});
            this.mnuResolution.Name = "mnuResolution";
            this.mnuResolution.Size = new System.Drawing.Size(163, 22);
            this.mnuResolution.Text = "Resolution";
            // 
            // mnuMediumResolution
            // 
            this.mnuMediumResolution.Checked = true;
            this.mnuMediumResolution.CheckState = System.Windows.Forms.CheckState.Checked;
            this.mnuMediumResolution.Name = "mnuMediumResolution";
            this.mnuMediumResolution.Size = new System.Drawing.Size(119, 22);
            this.mnuMediumResolution.Text = "Medium";
            this.mnuMediumResolution.Click += new System.EventHandler(this.mnuMediumResolution_Click);
            // 
            // mnuFullResolution
            // 
            this.mnuFullResolution.Name = "mnuFullResolution";
            this.mnuFullResolution.Size = new System.Drawing.Size(119, 22);
            this.mnuFullResolution.Text = "Full";
            this.mnuFullResolution.Click += new System.EventHandler(this.mnuFullResolution_Click);
            // 
            // mnuCustomWindow
            // 
            this.mnuCustomWindow.CheckOnClick = true;
            this.mnuCustomWindow.Name = "mnuCustomWindow";
            this.mnuCustomWindow.Size = new System.Drawing.Size(163, 22);
            this.mnuCustomWindow.Text = "Custom Window";
            this.mnuCustomWindow.Click += new System.EventHandler(this.mnuCustomWindow_Click);
            // 
            // mnuReboot
            // 
            this.mnuReboot.Name = "mnuReboot";
            this.mnuReboot.Size = new System.Drawing.Size(172, 22);
            this.mnuReboot.Text = "Reboot";
            this.mnuReboot.Click += new System.EventHandler(this.mnuReboot_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(322, 285);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.menuStrip1);
            this.Controls.Add(this.renderWindow);
            this.DoubleBuffered = true;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MainMenuStrip = this.menuStrip1;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Form1";
            this.ShowIcon = false;
            this.Text = "Layman\'s Xbox Emulator";
            this.TopMost = true;
            ((System.ComponentModel.ISupportInitialize)(this.renderWindow)).EndInit();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox renderWindow;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem mnuXbox;
        private System.Windows.Forms.ToolStripStatusLabel status;
        private System.Windows.Forms.ToolStripMenuItem mnuSettings;
        private System.Windows.Forms.ToolStripMenuItem mnuStart;
        private System.Windows.Forms.ToolStripMenuItem mnuStop;
        private System.Windows.Forms.ToolStripMenuItem mnuScreenshot;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem mnuInput;
        private System.Windows.Forms.ToolStripMenuItem mnuPause;
        private System.Windows.Forms.ToolStripMenuItem mnuResolution;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripMenuItem mnuExit;
        private System.Windows.Forms.ToolStripMenuItem mnuReset;
        private System.Windows.Forms.ToolStripMenuItem mnuStep;
        private System.Windows.Forms.ToolStripMenuItem mnuX360Input;
        private System.Windows.Forms.ToolStripMenuItem mnuGamepadInput;
        private System.Windows.Forms.ToolStripMenuItem mnuKeyboardInput;
        private System.Windows.Forms.ToolStripMenuItem mnuFullResolution;
        private System.Windows.Forms.ToolStripMenuItem mnuMediumResolution;
        private System.Windows.Forms.ToolStripMenuItem mnuCustomWindow;
        private System.Windows.Forms.ToolStripMenuItem mnuNone;
        private System.Windows.Forms.ToolStripMenuItem mnuReboot;
    }
}

