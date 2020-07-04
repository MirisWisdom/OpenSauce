namespace OpenSauceIDE.Controls
{
    partial class MessageListControl
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.mMessageListBox = new System.Windows.Forms.ListBox();
            this.mCopyButton = new System.Windows.Forms.Button();
            this.mClearButton = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            this.splitContainer1.IsSplitterFixed = true;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.mMessageListBox);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.mCopyButton);
            this.splitContainer1.Panel2.Controls.Add(this.mClearButton);
            this.splitContainer1.Size = new System.Drawing.Size(600, 72);
            this.splitContainer1.SplitterDistance = 525;
            this.splitContainer1.TabIndex = 0;
            // 
            // mMessageListBox
            // 
            this.mMessageListBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mMessageListBox.FormattingEnabled = true;
            this.mMessageListBox.Location = new System.Drawing.Point(0, 0);
            this.mMessageListBox.Name = "mMessageListBox";
            this.mMessageListBox.Size = new System.Drawing.Size(525, 72);
            this.mMessageListBox.TabIndex = 0;
            // 
            // mCopyButton
            // 
            this.mCopyButton.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.mCopyButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.mCopyButton.Location = new System.Drawing.Point(3, 39);
            this.mCopyButton.Name = "mCopyButton";
            this.mCopyButton.Size = new System.Drawing.Size(65, 30);
            this.mCopyButton.TabIndex = 1;
            this.mCopyButton.Text = "Copy";
            this.mCopyButton.UseVisualStyleBackColor = true;
            this.mCopyButton.Click += new System.EventHandler(this.Copy);
            // 
            // mClearButton
            // 
            this.mClearButton.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.mClearButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.mClearButton.Location = new System.Drawing.Point(3, 3);
            this.mClearButton.Name = "mClearButton";
            this.mClearButton.Size = new System.Drawing.Size(65, 30);
            this.mClearButton.TabIndex = 0;
            this.mClearButton.Text = "Clear";
            this.mClearButton.UseVisualStyleBackColor = true;
            this.mClearButton.Click += new System.EventHandler(this.Clear);
            // 
            // MessageList
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.splitContainer1);
            this.MinimumSize = new System.Drawing.Size(600, 72);
            this.Name = "MessageList";
            this.Size = new System.Drawing.Size(600, 72);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.Button mCopyButton;
        private System.Windows.Forms.Button mClearButton;
        private System.Windows.Forms.ListBox mMessageListBox;
    }
}
