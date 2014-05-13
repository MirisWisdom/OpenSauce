using System;

namespace OpenSauceIDE.Aether.AutoUI.Controls
{
	/// <summary>	An field container for displaying a member with a name. </summary>
	public partial class AutoUIFieldContainer : System.Windows.Forms.UserControl
	{
		/// <summary>	Default constructor. </summary>
		public AutoUIFieldContainer()
		{
			InitializeComponent();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the title of the field container. </summary>
		///
		/// <param name="title">	The title to use. </param>
		public void SetTitle(string title)
		{
			TitleLabel.Text = title;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds a control to the containers panel. </summary>
		///
		/// <param name="control">	The control to add. </param>
		public void AddControl(System.Windows.Forms.Control control)
		{
			control.Margin = new System.Windows.Forms.Padding(0);
			ControlPanel.Controls.Add(control);
		}
	}
}
