namespace Yelo_Neighborhood
{
    partial class SystemProperties
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
            this.xboxProperties = new System.Windows.Forms.PropertyGrid();
            this.SuspendLayout();
            // 
            // xboxProperties
            // 
            this.xboxProperties.Dock = System.Windows.Forms.DockStyle.Fill;
            this.xboxProperties.Location = new System.Drawing.Point(0, 0);
            this.xboxProperties.Name = "xboxProperties";
            this.xboxProperties.PropertySort = System.Windows.Forms.PropertySort.NoSort;
            this.xboxProperties.Size = new System.Drawing.Size(444, 484);
            this.xboxProperties.TabIndex = 27;
            this.xboxProperties.ToolbarVisible = false;
            // 
            // SystemProperties
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(444, 484);
            this.Controls.Add(this.xboxProperties);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "SystemProperties";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "System Properties";
            this.TopMost = true;
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PropertyGrid xboxProperties;

    }
}
