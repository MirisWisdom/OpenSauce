/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
namespace BlamLib.Forms
{
	partial class BlockIndex
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
			this.name = new System.Windows.Forms.Label();
			this.field = new System.Windows.Forms.ComboBox();
			this.SuspendLayout();
			// 
			// name
			// 
			this.name.Location = new System.Drawing.Point(16, 4);
			this.name.Name = "name";
			this.name.Size = new System.Drawing.Size(162, 16);
			this.name.TabIndex = 1;
			this.name.Text = "block index";
			this.name.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
			// 
			// field
			// 
			this.field.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.field.FormattingEnabled = true;
			this.field.Location = new System.Drawing.Point(184, 0);
			this.field.Name = "field";
			this.field.Size = new System.Drawing.Size(128, 21);
			this.field.TabIndex = 2;
			// 
			// BlockIndex
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.Controls.Add(this.field);
			this.Controls.Add(this.name);
			this.Name = "BlockIndex";
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Label name;
		private System.Windows.Forms.ComboBox field;
	}
}
