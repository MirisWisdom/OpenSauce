/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
namespace BlamLib.Forms
{
	partial class Field
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
				components.Dispose();
			base.Dispose(disposing);
		}

		#region Component Designer generated code

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			this.toolTip = new System.Windows.Forms.ToolTip(this.components);
			this.SuspendLayout();
			// 
			// toolTip
			// 
			this.toolTip.ToolTipTitle = "Field Help";
			// 
			// Field
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Name = "Field";
			this.Size = new System.Drawing.Size(584, 24);
			this.MouseLeave += new System.EventHandler(this.OnMouseLeave);
			this.MouseHover += new System.EventHandler(this.OnMouseHover);
			this.ResumeLayout(false);

		}

		#endregion

		protected System.Windows.Forms.ToolTip toolTip;
	}
}