namespace Yelo_Neighborhood
{
    partial class LEDStateChanger
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
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
            this.cmdApply = new System.Windows.Forms.Button();
            this.cboState1 = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.cboState2 = new System.Windows.Forms.ComboBox();
            this.label3 = new System.Windows.Forms.Label();
            this.cboState3 = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.cboState4 = new System.Windows.Forms.ComboBox();
            this.SuspendLayout();
            // 
            // cmdApply
            // 
            this.cmdApply.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.cmdApply.Location = new System.Drawing.Point(151, 126);
            this.cmdApply.Name = "cmdApply";
            this.cmdApply.Size = new System.Drawing.Size(75, 23);
            this.cmdApply.TabIndex = 24;
            this.cmdApply.Text = "Apply";
            this.cmdApply.Click += new System.EventHandler(this.cmdApply_Click);
            // 
            // cboState1
            // 
            this.cboState1.FormattingEnabled = true;
            this.cboState1.Location = new System.Drawing.Point(62, 12);
            this.cboState1.Name = "cboState1";
            this.cboState1.Size = new System.Drawing.Size(164, 21);
            this.cboState1.TabIndex = 25;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 15);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(44, 13);
            this.label1.TabIndex = 26;
            this.label1.Text = "State 1:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 42);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(44, 13);
            this.label2.TabIndex = 28;
            this.label2.Text = "State 2:";
            // 
            // cboState2
            // 
            this.cboState2.FormattingEnabled = true;
            this.cboState2.Location = new System.Drawing.Point(62, 39);
            this.cboState2.Name = "cboState2";
            this.cboState2.Size = new System.Drawing.Size(164, 21);
            this.cboState2.TabIndex = 27;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 69);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(44, 13);
            this.label3.TabIndex = 30;
            this.label3.Text = "State 3:";
            // 
            // cboState3
            // 
            this.cboState3.FormattingEnabled = true;
            this.cboState3.Location = new System.Drawing.Point(62, 66);
            this.cboState3.Name = "cboState3";
            this.cboState3.Size = new System.Drawing.Size(164, 21);
            this.cboState3.TabIndex = 29;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 96);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(44, 13);
            this.label4.TabIndex = 32;
            this.label4.Text = "State 4:";
            // 
            // cboState4
            // 
            this.cboState4.FormattingEnabled = true;
            this.cboState4.Location = new System.Drawing.Point(62, 93);
            this.cboState4.Name = "cboState4";
            this.cboState4.Size = new System.Drawing.Size(164, 21);
            this.cboState4.TabIndex = 31;
            // 
            // LEDStateChanger
            // 
            this.AcceptButton = this.cmdApply;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(238, 161);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.cboState4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.cboState3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.cboState2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.cboState1);
            this.Controls.Add(this.cmdApply);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "LEDStateChanger";
            this.Padding = new System.Windows.Forms.Padding(9);
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "LED State";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button cmdApply;
        private System.Windows.Forms.ComboBox cboState1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox cboState2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ComboBox cboState3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.ComboBox cboState4;

    }
}
