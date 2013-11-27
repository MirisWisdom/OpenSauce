using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace OpenSauceIDE.Aether.Controls
{
	///-------------------------------------------------------------------------------------------------
	/// <summary>
	/// 	A custom group box that can be collapsed using a button in the top right corner.
	/// </summary>
	///-------------------------------------------------------------------------------------------------
	public partial class CollapsibleGroupBox : UserControl
	{
		#region Fields
		private bool mContentsVisible = true;
		#endregion Fields

		#region Events
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Event handler. Called when the VisibleToggle button is clicked. </summary>
		/// <param name="sender">	Source of the event. </param>
		/// <param name="e">	 	Event information. </param>
		///-------------------------------------------------------------------------------------------------
		private void VisibleToggle_Click(object sender, EventArgs e)
		{
			SetVisible(!mContentsVisible);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>   Sets whether the group box contents should be visible. </summary>
		/// <param name="visible">  true to show, false to hide. </param>
		///-------------------------------------------------------------------------------------------------
		protected void SetVisible(bool visible)
		{
			mContentsVisible = visible;

			if (mContentsVisible)
			{
				VisibleToggle.Text = "-";
				ContentsPanel.Show();
			}
			else
			{
				VisibleToggle.Text = "+";
				ContentsPanel.Hide();
			}
		}
		#endregion Events

		#region Constructor
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Default constructor. </summary>
		///-------------------------------------------------------------------------------------------------
		public CollapsibleGroupBox()
		{
			InitializeComponent();
		}
		#endregion Constructor

		#region Display
		///-------------------------------------------------------------------------------------------------
		/// <summary>   Sets text of the group box. </summary>
		/// <param name="name"> The name to use. </param>
		///-------------------------------------------------------------------------------------------------
		public virtual void SetContainerName(string name)
		{
			ContainerGroupBox.Text = name;
		}
		#endregion Display
	}
}
