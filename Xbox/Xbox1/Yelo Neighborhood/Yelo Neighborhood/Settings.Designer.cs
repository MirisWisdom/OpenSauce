namespace Yelo_Neighborhood
{
    partial class Settings
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
            this.cmdConnect = new System.Windows.Forms.Button();
            this.checkAutoDiscover = new System.Windows.Forms.CheckBox();
            this.txtIP = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // cmdConnect
            // 
            this.cmdConnect.Location = new System.Drawing.Point(201, 38);
            this.cmdConnect.Name = "cmdConnect";
            this.cmdConnect.Size = new System.Drawing.Size(75, 23);
            this.cmdConnect.TabIndex = 0;
            this.cmdConnect.Text = "Try Again";
            this.cmdConnect.UseVisualStyleBackColor = true;
            this.cmdConnect.Click += new System.EventHandler(this.cmdConnect_Click);
            // 
            // checkAutoDiscover
            // 
            this.checkAutoDiscover.AutoSize = true;
            this.checkAutoDiscover.Checked = true;
            this.checkAutoDiscover.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkAutoDiscover.Location = new System.Drawing.Point(102, 42);
            this.checkAutoDiscover.Name = "checkAutoDiscover";
            this.checkAutoDiscover.Size = new System.Drawing.Size(93, 17);
            this.checkAutoDiscover.TabIndex = 2;
            this.checkAutoDiscover.Text = "Auto Discover";
            this.checkAutoDiscover.UseVisualStyleBackColor = true;
            this.checkAutoDiscover.CheckedChanged += new System.EventHandler(this.checkAutoDiscover_CheckedChanged);
            // 
            // txtIP
            // 
            this.txtIP.Enabled = false;
            this.txtIP.Location = new System.Drawing.Point(122, 12);
            this.txtIP.Name = "txtIP";
            this.txtIP.Size = new System.Drawing.Size(154, 20);
            this.txtIP.TabIndex = 3;
            this.txtIP.Text = "192.168.1.2";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(104, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "IP Address or Name:";
            // 
            // Settings
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(288, 73);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.txtIP);
            this.Controls.Add(this.checkAutoDiscover);
            this.Controls.Add(this.cmdConnect);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Name = "Settings";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Unable To Find XBox";
            this.TopMost = true;
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button cmdConnect;
        private System.Windows.Forms.CheckBox checkAutoDiscover;
        private System.Windows.Forms.TextBox txtIP;
        private System.Windows.Forms.Label label1;
    }
}