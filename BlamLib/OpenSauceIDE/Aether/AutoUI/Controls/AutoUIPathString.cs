using System;
using System.ComponentModel;
using System.Drawing;
using System.IO;
using System.Windows.Forms;

namespace OpenSauceIDE.Aether.AutoUI.Controls
{
	/// <summary>	An automatic user interface path string control. </summary>
	public partial class AutoUIPathString : UserControl, IAutoUIControl
	{
		#region Fields
		private string mLastValidPath = "";
		#endregion Fields

		#region Constructor
		/// <summary>	Initializes a new instance of the AutoUIPathString class. </summary>
		public AutoUIPathString()
		{
			InitializeComponent();
		}
		#endregion Constructor

		#region Interface
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Displays the folder browser dialog to select a new path. </summary>
		///
		/// <param name="sender">	Source of the event. </param>
		/// <param name="e">	 	Event information. </param>
		private void BrowseForPath(object sender, EventArgs e)
		{
			// Set the initial directory to the last valid directory
			PathBrowser.SelectedPath = mLastValidPath;
			PathBrowser.Description = "Select a directory";

			// Set the path string if the dialog was accepted
			var dialogResult = PathBrowser.ShowDialog();
			if((dialogResult == DialogResult.OK) ||
				(dialogResult == DialogResult.Yes))
			{
				PathStringTextBox.Text = PathBrowser.SelectedPath;
			}
		}

		/// <summary>	Sets text color depending on whether the path is valid or not. </summary>
		private void SetTextColor()
		{
			if (Directory.Exists(PathStringTextBox.Text))
			{
				PathStringTextBox.ForeColor = Color.LightGreen;
			}
			else
			{
				PathStringTextBox.ForeColor = Color.Salmon;
			}
		}

		/// <summary>	Sets the last valid path if the current path exists. </summary>
		private void SetLastValid()
		{
			if(Directory.Exists(PathStringTextBox.Text))
			{
				mLastValidPath = PathStringTextBox.Text;
			}
		}
		#endregion Interface

		#region Events
		private void PathChanged(object sender, EventArgs e)
		{
			// Update the text color
			SetTextColor();
			SetLastValid();
		}
		#endregion Events

		#region IAutoUIControl Members
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets information describing the target member. </summary>
		///
		/// <value>	Information describing the target member. </value>
		public Controls.IAutoUIMemberInfo MemberInfo
		{
			get;
			private set;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the target member info for the control. </summary>
		///
		/// <param name="memberInfo">	Information describing the target member. </param>
		public void SetMemberInfo(Controls.IAutoUIMemberInfo memberInfo)
		{
			MemberInfo = memberInfo;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the source object to bind the control to. </summary>
		///
		/// <exception cref="NotImplementedException">	Thrown when the requested operation is unimplemented. </exception>
		///
		/// <param name="sourceObject">	Source object. </param>
		public void SetSource(object sourceObject)
		{
			// Get the current field value
			object fieldValue = MemberInfo.GetValue(sourceObject);

			if (fieldValue == null)
			{
				Enabled = false;
				return;
			}

			// Bind control to the path string
			PathStringTextBox.SetMemberInfo(MemberInfo);
			PathStringTextBox.SetSource(sourceObject);

			SetTextColor();
			SetLastValid();
		}
		#endregion
	}
}
