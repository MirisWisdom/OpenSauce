namespace Updater
{
    partial class Downloader
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
            this.probar = new System.Windows.Forms.ProgressBar();
            this.lblStatus = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.txtChangeLog = new System.Windows.Forms.TextBox();
            this.cmdClose = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // probar
            // 
            this.probar.Location = new System.Drawing.Point(12, 232);
            this.probar.Name = "probar";
            this.probar.Size = new System.Drawing.Size(338, 23);
            this.probar.Style = System.Windows.Forms.ProgressBarStyle.Marquee;
            this.probar.TabIndex = 0;
            // 
            // lblStatus
            // 
            this.lblStatus.AutoSize = true;
            this.lblStatus.Location = new System.Drawing.Point(12, 216);
            this.lblStatus.Name = "lblStatus";
            this.lblStatus.Size = new System.Drawing.Size(78, 13);
            this.lblStatus.TabIndex = 1;
            this.lblStatus.Text = "Downloading...";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(65, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Change Log";
            // 
            // txtChangeLog
            // 
            this.txtChangeLog.Enabled = false;
            this.txtChangeLog.Location = new System.Drawing.Point(12, 25);
            this.txtChangeLog.Multiline = true;
            this.txtChangeLog.Name = "txtChangeLog";
            this.txtChangeLog.Size = new System.Drawing.Size(338, 188);
            this.txtChangeLog.TabIndex = 3;
            // 
            // cmdClose
            // 
            this.cmdClose.Location = new System.Drawing.Point(100, 232);
            this.cmdClose.Name = "cmdClose";
            this.cmdClose.Size = new System.Drawing.Size(164, 23);
            this.cmdClose.TabIndex = 4;
            this.cmdClose.Text = "Close";
            this.cmdClose.UseVisualStyleBackColor = true;
            this.cmdClose.Visible = false;
            this.cmdClose.Click += new System.EventHandler(this.cmdClose_Click);
            // 
            // Downloader
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(362, 267);
            this.Controls.Add(this.cmdClose);
            this.Controls.Add(this.txtChangeLog);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.lblStatus);
            this.Controls.Add(this.probar);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Downloader";
            this.Text = "Updater";
            this.Load += new System.EventHandler(this.Downloader_Load);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Downloader_FormClosed);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ProgressBar probar;
        private System.Windows.Forms.Label lblStatus;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtChangeLog;
        private System.Windows.Forms.Button cmdClose;
    }
}

