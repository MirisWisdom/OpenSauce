/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
namespace BlamLib.Forms
{
	partial class Explanation
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
			//this.fieldPanel = new DevComponents.DotNetBar.ExpandablePanel();
			this.fieldPanel = new System.Windows.Forms.Panel();
			this.field = new System.Windows.Forms.Label();
			this.fieldPanel.SuspendLayout();
			this.SuspendLayout();
			// 
			// fieldPanel
			// 
			//this.fieldPanel.CanvasColor = System.Drawing.SystemColors.Control;
			//this.fieldPanel.ColorSchemeStyle = DevComponents.DotNetBar.eDotNetBarStyle.VS2005;
			this.fieldPanel.Controls.Add(this.field);
			this.fieldPanel.Dock = System.Windows.Forms.DockStyle.Fill;
			this.fieldPanel.Location = new System.Drawing.Point(0, 0);
			this.fieldPanel.Name = "fieldPanel";
			this.fieldPanel.Size = new System.Drawing.Size(584, 88);
			//this.fieldPanel.Style.Alignment = System.Drawing.StringAlignment.Center;
			//this.fieldPanel.Style.BackColor1.ColorSchemePart = DevComponents.DotNetBar.eColorSchemePart.PanelBackground;
			//this.fieldPanel.Style.BackColor2.ColorSchemePart = DevComponents.DotNetBar.eColorSchemePart.PanelBackground2;
			//this.fieldPanel.Style.Border = DevComponents.DotNetBar.eBorderType.SingleLine;
			//this.fieldPanel.Style.BorderColor.ColorSchemePart = DevComponents.DotNetBar.eColorSchemePart.BarDockedBorder;
			//this.fieldPanel.Style.ForeColor.ColorSchemePart = DevComponents.DotNetBar.eColorSchemePart.ItemText;
			//this.fieldPanel.Style.GradientAngle = 90;
			this.fieldPanel.TabIndex = 0;
			//this.fieldPanel.ThemeAware = true;
			//this.fieldPanel.TitleStyle.Alignment = System.Drawing.StringAlignment.Center;
			//this.fieldPanel.TitleStyle.BackColor1.ColorSchemePart = DevComponents.DotNetBar.eColorSchemePart.PanelBackground;
			//this.fieldPanel.TitleStyle.BackColor2.ColorSchemePart = DevComponents.DotNetBar.eColorSchemePart.PanelBackground2;
			//this.fieldPanel.TitleStyle.Border = DevComponents.DotNetBar.eBorderType.SingleLine;
			//this.fieldPanel.TitleStyle.BorderColor.ColorSchemePart = DevComponents.DotNetBar.eColorSchemePart.PanelBorder;
			//this.fieldPanel.TitleStyle.ForeColor.ColorSchemePart = DevComponents.DotNetBar.eColorSchemePart.PanelText;
			//this.fieldPanel.TitleStyle.GradientAngle = 90;
			//this.fieldPanel.TitleText = "title";
			//this.fieldPanel.ExpandedChanged += new DevComponents.DotNetBar.ExpandChangeEventHandler(this.OnFieldPanelExpandedChanged);
			//this.fieldPanel.ExpandedChanging += new DevComponents.DotNetBar.ExpandChangeEventHandler(this.OnFieldPanelExpandedChanging);
			// 
			// field
			// 
			this.field.Dock = System.Windows.Forms.DockStyle.Fill;
			this.field.Location = new System.Drawing.Point(0, 26);
			this.field.Name = "field";
			this.field.Size = new System.Drawing.Size(584, 62);
			this.field.TabIndex = 1;
			this.field.Text = "explanation";
			// 
			// Explanation
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.Controls.Add(this.fieldPanel);
			this.Name = "Explanation";
			this.Size = new System.Drawing.Size(584, 88);
			this.fieldPanel.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		//private DevComponents.DotNetBar.ExpandablePanel fieldPanel;
		private System.Windows.Forms.Panel fieldPanel;
		private System.Windows.Forms.Label field;
	}
}
