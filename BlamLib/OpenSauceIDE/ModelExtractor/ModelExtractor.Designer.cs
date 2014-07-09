namespace OpenSauceIDE.ModelExtractor
{
    partial class ModelExtractor
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
            this.mExtractButton = new System.Windows.Forms.Button();
            this.mModelTypeComboBox = new System.Windows.Forms.ComboBox();
            this.mControlPanel = new System.Windows.Forms.Panel();
            this.panel1 = new System.Windows.Forms.Panel();
            this.label1 = new System.Windows.Forms.Label();
            this.mMessageList = new OpenSauceIDE.ModelExtractor.UI.MessageListControl();
            this.mDataPathControl = new OpenSauceIDE.ModelExtractor.UI.PathControl();
            this.mTagsPathControl = new OpenSauceIDE.ModelExtractor.UI.PathControl();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // mExtractButton
            // 
            this.mExtractButton.BackColor = System.Drawing.SystemColors.ControlDarkDark;
            this.mExtractButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.mExtractButton.Location = new System.Drawing.Point(421, 41);
            this.mExtractButton.Name = "mExtractButton";
            this.mExtractButton.Size = new System.Drawing.Size(75, 23);
            this.mExtractButton.TabIndex = 2;
            this.mExtractButton.Text = "Extract";
            this.mExtractButton.UseVisualStyleBackColor = false;
            this.mExtractButton.Click += new System.EventHandler(this.Extract);
            // 
            // mModelTypeComboBox
            // 
            this.mModelTypeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.mModelTypeComboBox.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.mModelTypeComboBox.FormattingEnabled = true;
            this.mModelTypeComboBox.Location = new System.Drawing.Point(120, 42);
            this.mModelTypeComboBox.Name = "mModelTypeComboBox";
            this.mModelTypeComboBox.Size = new System.Drawing.Size(298, 21);
            this.mModelTypeComboBox.TabIndex = 3;
            // 
            // mControlPanel
            // 
            this.mControlPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.mControlPanel.Location = new System.Drawing.Point(0, 74);
            this.mControlPanel.Name = "mControlPanel";
            this.mControlPanel.Size = new System.Drawing.Size(624, 290);
            this.mControlPanel.TabIndex = 0;
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.label1);
            this.panel1.Controls.Add(this.mExtractButton);
            this.panel1.Controls.Add(this.mModelTypeComboBox);
            this.panel1.Controls.Add(this.mDataPathControl);
            this.panel1.Controls.Add(this.mTagsPathControl);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(624, 74);
            this.panel1.TabIndex = 7;
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(-3, 41);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(123, 23);
            this.label1.TabIndex = 6;
            this.label1.Text = "Model Type";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            // 
            // mMessageList
            // 
            this.mMessageList.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.mMessageList.Location = new System.Drawing.Point(0, 364);
            this.mMessageList.MaxMessageCount = 500;
            this.mMessageList.MinimumSize = new System.Drawing.Size(600, 72);
            this.mMessageList.Name = "mMessageList";
            this.mMessageList.Size = new System.Drawing.Size(624, 152);
            this.mMessageList.TabIndex = 8;
            // 
            // mDataPathControl
            // 
            this.mDataPathControl.BackColorExists = System.Drawing.Color.White;
            this.mDataPathControl.BackColorMissing = System.Drawing.Color.White;
            this.mDataPathControl.Description = "Select your data directory...";
            this.mDataPathControl.Dock = System.Windows.Forms.DockStyle.Top;
            this.mDataPathControl.ForeColorExists = System.Drawing.Color.Black;
            this.mDataPathControl.ForeColorMissing = System.Drawing.Color.Red;
            this.mDataPathControl.Location = new System.Drawing.Point(0, 21);
            this.mDataPathControl.Name = "mDataPathControl";
            this.mDataPathControl.SelectedPath = null;
            this.mDataPathControl.Size = new System.Drawing.Size(624, 21);
            this.mDataPathControl.TabIndex = 4;
            this.mDataPathControl.Title = "Data Directory";
            // 
            // mTagsPathControl
            // 
            this.mTagsPathControl.BackColorExists = System.Drawing.Color.White;
            this.mTagsPathControl.BackColorMissing = System.Drawing.Color.White;
            this.mTagsPathControl.Description = "Select your tags directory...";
            this.mTagsPathControl.Dock = System.Windows.Forms.DockStyle.Top;
            this.mTagsPathControl.ForeColorExists = System.Drawing.Color.Black;
            this.mTagsPathControl.ForeColorMissing = System.Drawing.Color.Red;
            this.mTagsPathControl.Location = new System.Drawing.Point(0, 0);
            this.mTagsPathControl.Name = "mTagsPathControl";
            this.mTagsPathControl.SelectedPath = null;
            this.mTagsPathControl.Size = new System.Drawing.Size(624, 21);
            this.mTagsPathControl.TabIndex = 5;
            this.mTagsPathControl.Title = "Tags Directory";
            // 
            // ModelExtractor
            // 
            this.AllowDrop = true;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoSize = true;
            this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.BackColor = System.Drawing.SystemColors.ControlDarkDark;
            this.ClientSize = new System.Drawing.Size(624, 516);
            this.Controls.Add(this.mControlPanel);
            this.Controls.Add(this.mMessageList);
            this.Controls.Add(this.panel1);
            this.ForeColor = System.Drawing.Color.LightGreen;
            this.MinimumSize = new System.Drawing.Size(640, 480);
            this.Name = "ModelExtractor";
            this.Text = "Model Extractor";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.ModelExtractorClosed);
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button mExtractButton;
        private System.Windows.Forms.ComboBox mModelTypeComboBox;
        private UI.PathControl mTagsPathControl;
        private UI.PathControl mDataPathControl;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label1;
        private UI.MessageListControl mMessageList;
        private System.Windows.Forms.Panel mControlPanel;
    }
}