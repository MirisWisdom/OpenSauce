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
	/// <summary>	A collapsible group box type that allows extraction of a scene object. </summary>
	public partial class SceneObjectExtractableGroupBox : CollapsibleGroupBox, ISceneObjectUI
	{
		#region Fields
		private OpenSauceIDE.Aether.Extraction.IObjectExtractable mSceneObjectExtractable = null;
		#endregion Fields

		#region Constructor
		/// <summary>	Initializes a new instance of the SceneObjectExtractableGroupBox class. </summary>
		public SceneObjectExtractableGroupBox()
		{
			InitializeComponent();
		}
		#endregion Constructor

		#region ISceneObjectUI
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the scene object. </summary>
		///
		/// <param name="sceneObject">	The scene object. </param>
		public virtual void SetSceneObject(SceneObject.SceneObjectBase sceneObject)
		{
			if ((sceneObject != null) && (sceneObject is OpenSauceIDE.Aether.Extraction.IObjectExtractable))
			{
				// If the scene obejct is extractable, display the extract button
				mSceneObjectExtractable = sceneObject as OpenSauceIDE.Aether.Extraction.IObjectExtractable;
				ExtractButton.Enabled = true;
				ExtractButton.Visible = true;
			}
			else
			{
				// If the object is set to null or is not extractable, hide the extract button
				mSceneObjectExtractable = null;
				ExtractButton.Enabled = false;
				ExtractButton.Visible = false;
			}

			// Since the object has changed, force the group box to expand
			SetVisible(true);
		}
		#endregion ISceneObjectUI

		#region Events
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Event handler. Called by ExtractButton when clicked. </summary>
		///
		/// <param name="sender">	Source of the event. </param>
		/// <param name="e">	 	Event information. </param>
		private void ExtractButton_Click(object sender, EventArgs e)
		{
			// Extract the current scene obejct if it is not null
			if (mSceneObjectExtractable == null)
			{
				return;
			}

			string name = AutoUI.AutoUIFactory.GetAutoUIName(mSceneObjectExtractable);

			Aether.Instance.Extraction.RegisterExtractionJob(mSceneObjectExtractable, name);
			Aether.Instance.Extraction.RunCurrentJobs();
		}
		#endregion Events
	}
}
