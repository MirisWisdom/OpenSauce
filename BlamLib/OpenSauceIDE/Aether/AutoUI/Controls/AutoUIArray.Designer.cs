namespace OpenSauceIDE.Aether.AutoUI.Controls
{
	//partial class AutoUIArray
	//{
	//	/// <summary> 
	//	/// Required designer variable.
	//	/// </summary>
	//	private System.ComponentModel.IContainer components = null;

	//	/// <summary> 
	//	/// Clean up any resources being used.
	//	/// </summary>
	//	/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
	//	protected override void Dispose(bool disposing)
	//	{
	//		if (disposing && (components != null))
	//		{
	//			components.Dispose();
	//		}
	//		base.Dispose(disposing);
	//	}

	//	#region Component Designer generated code

	//	/// <summary> 
	//	/// Required method for Designer support - do not modify 
	//	/// the contents of this method with the code editor.
	//	/// </summary>
	//	private void InitializeComponent()
	//	{
	//		this.comboBox1 = new System.Windows.Forms.ComboBox();
	//		this.ButtonAdd = new System.Windows.Forms.Button();
	//		this.ButtonInsert = new System.Windows.Forms.Button();
	//		this.ButtonDuplicate = new System.Windows.Forms.Button();
	//		this.ButtonDelete = new System.Windows.Forms.Button();
	//		this.ButtonDeleteAll = new System.Windows.Forms.Button();
	//		this.ButtonShowHide = new System.Windows.Forms.Button();
	//		this.PanelDisplay = new OpenSauceIDE.Aether.AutoUI.Controls.AutoUIClass();
	//		this.SuspendLayout();
	//		// 
	//		// comboBox1
	//		// 
	//		this.comboBox1.FormattingEnabled = true;
	//		this.comboBox1.Location = new System.Drawing.Point(20, 0);
	//		this.comboBox1.Margin = new System.Windows.Forms.Padding(0);
	//		this.comboBox1.Name = "comboBox1";
	//		this.comboBox1.Size = new System.Drawing.Size(280, 21);
	//		this.comboBox1.TabIndex = 0;
	//		// 
	//		// ButtonAdd
	//		// 
	//		this.ButtonAdd.Location = new System.Drawing.Point(300, 0);
	//		this.ButtonAdd.Margin = new System.Windows.Forms.Padding(0);
	//		this.ButtonAdd.Name = "ButtonAdd";
	//		this.ButtonAdd.Size = new System.Drawing.Size(60, 21);
	//		this.ButtonAdd.TabIndex = 1;
	//		this.ButtonAdd.Text = "Add";
	//		this.ButtonAdd.UseVisualStyleBackColor = true;
	//		// 
	//		// ButtonInsert
	//		// 
	//		this.ButtonInsert.Location = new System.Drawing.Point(360, 0);
	//		this.ButtonInsert.Margin = new System.Windows.Forms.Padding(0);
	//		this.ButtonInsert.Name = "ButtonInsert";
	//		this.ButtonInsert.Size = new System.Drawing.Size(60, 21);
	//		this.ButtonInsert.TabIndex = 2;
	//		this.ButtonInsert.Text = "Insert";
	//		this.ButtonInsert.UseVisualStyleBackColor = true;
	//		// 
	//		// ButtonDuplicate
	//		// 
	//		this.ButtonDuplicate.Location = new System.Drawing.Point(420, 0);
	//		this.ButtonDuplicate.Margin = new System.Windows.Forms.Padding(0);
	//		this.ButtonDuplicate.Name = "ButtonDuplicate";
	//		this.ButtonDuplicate.Size = new System.Drawing.Size(60, 21);
	//		this.ButtonDuplicate.TabIndex = 3;
	//		this.ButtonDuplicate.Text = "Duplicate";
	//		this.ButtonDuplicate.UseVisualStyleBackColor = true;
	//		// 
	//		// ButtonDelete
	//		// 
	//		this.ButtonDelete.Location = new System.Drawing.Point(480, 0);
	//		this.ButtonDelete.Margin = new System.Windows.Forms.Padding(0);
	//		this.ButtonDelete.Name = "ButtonDelete";
	//		this.ButtonDelete.Size = new System.Drawing.Size(60, 21);
	//		this.ButtonDelete.TabIndex = 4;
	//		this.ButtonDelete.Text = "Delete";
	//		this.ButtonDelete.UseVisualStyleBackColor = true;
	//		// 
	//		// ButtonDeleteAll
	//		// 
	//		this.ButtonDeleteAll.Location = new System.Drawing.Point(540, 0);
	//		this.ButtonDeleteAll.Margin = new System.Windows.Forms.Padding(0);
	//		this.ButtonDeleteAll.Name = "ButtonDeleteAll";
	//		this.ButtonDeleteAll.Size = new System.Drawing.Size(60, 21);
	//		this.ButtonDeleteAll.TabIndex = 5;
	//		this.ButtonDeleteAll.Text = "Delete All";
	//		this.ButtonDeleteAll.UseVisualStyleBackColor = true;
	//		// 
	//		// ButtonShowHide
	//		// 
	//		this.ButtonShowHide.Location = new System.Drawing.Point(0, -1);
	//		this.ButtonShowHide.Margin = new System.Windows.Forms.Padding(0);
	//		this.ButtonShowHide.Name = "ButtonShowHide";
	//		this.ButtonShowHide.Size = new System.Drawing.Size(20, 22);
	//		this.ButtonShowHide.TabIndex = 6;
	//		this.ButtonShowHide.Text = "+";
	//		this.ButtonShowHide.UseVisualStyleBackColor = true;
	//		// 
	//		// PanelDisplay
	//		// 
	//		this.PanelDisplay.AutoSize = true;
	//		this.PanelDisplay.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
	//		this.PanelDisplay.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
	//		this.PanelDisplay.Location = new System.Drawing.Point(0, 21);
	//		this.PanelDisplay.Margin = new System.Windows.Forms.Padding(0);
	//		this.PanelDisplay.MinimumSize = new System.Drawing.Size(600, 0);
	//		this.PanelDisplay.Name = "PanelDisplay";
	//		this.PanelDisplay.Size = new System.Drawing.Size(600, 2);
	//		this.PanelDisplay.TabIndex = 7;
	//		// 
	//		// AutoUIEnumerable
	//		// 
	//		this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
	//		this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
	//		this.AutoSize = true;
	//		this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
	//		this.Controls.Add(this.PanelDisplay);
	//		this.Controls.Add(this.ButtonShowHide);
	//		this.Controls.Add(this.ButtonDeleteAll);
	//		this.Controls.Add(this.ButtonDelete);
	//		this.Controls.Add(this.ButtonDuplicate);
	//		this.Controls.Add(this.ButtonInsert);
	//		this.Controls.Add(this.ButtonAdd);
	//		this.Controls.Add(this.comboBox1);
	//		this.Name = "AutoUIEnumerable";
	//		this.Size = new System.Drawing.Size(600, 23);
	//		this.ResumeLayout(false);
	//		this.PerformLayout();

	//	}

	//	#endregion

	//	private System.Windows.Forms.ComboBox comboBox1;
	//	private System.Windows.Forms.Button ButtonAdd;
	//	private System.Windows.Forms.Button ButtonInsert;
	//	private System.Windows.Forms.Button ButtonDuplicate;
	//	private System.Windows.Forms.Button ButtonDelete;
	//	private System.Windows.Forms.Button ButtonDeleteAll;
	//	private System.Windows.Forms.Button ButtonShowHide;
	//	private AutoUIClass PanelDisplay;
	//}
}
