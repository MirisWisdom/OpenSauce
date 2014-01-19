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
	/// <summary>	A collapsible group box type that allows extraction of a scene object and it's children. </summary>
	public partial class SceneObjectGroupBox : SceneObjectExtractableGroupBox, ISceneObjectListUI
	{
		/// <summary>	Scene object proxy for sorting a scene object and it's name. </summary>
		private struct SceneObjectProxy
		{
			public SceneObject.SceneObjectBase SceneObject { get; set; }
			public string Name { get; set; }
		}

		#region Fields
		private BindingList<SceneObjectProxy> mSceneObjects = new BindingList<SceneObjectProxy>();
		private Dictionary<string, SceneObjectGroupBox> mSubsectionLists = new Dictionary<string, SceneObjectGroupBox>();
		#endregion Fields
		
		#region Constructor
		/// <summary>	Default constructor. </summary>
		public SceneObjectGroupBox()
		{
			InitializeComponent();

			// Bind the combobox to the scene objects list
			ObjectSelectionCombo.DataSource = mSceneObjects;
			ObjectSelectionCombo.DisplayMember = "Name";
			ObjectSelectionCombo.ValueMember = "SceneObject";

			// Add an event handler to manage when the selected object is changed
			ObjectSelectionCombo.SelectedValueChanged += ObjectSelectionCombo_SelectedValueChanged;
		}
		#endregion Constructor

		#region Scene Object Display
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Adds a scene object to the list. </summary>
		///
		/// <param name="sceneObject">	The scene object. </param>
		public void AddSceneObject(SceneObject.SceneObjectBase sceneObject)
		{
			mSceneObjects.Add(new SceneObjectProxy { SceneObject = sceneObject, Name = AutoUI.AutoUIFactory.GetAutoUIName(sceneObject) });

			if (mSceneObjects.Count == 1)
			{
				ObjectSelectionCombo_SelectedValueChanged(null, EventArgs.Empty);
			}
		}

		/// <summary>	Clears the scene objects list. </summary>
		public void ClearSceneObjects()
		{
			SetSceneObject(null);
			mSceneObjects.Clear();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the scene object to display. </summary>
		///
		/// <exception cref="Exception">
		/// 	Thrown if a scene object type does not have a scene object attribute defined.
		/// </exception>
		///
		/// <param name="sceneObject">	The scene object. </param>
		public override void SetSceneObject(SceneObject.SceneObjectBase sceneObject)
		{
			// Set the base values for the control and the container name
			base.SetSceneObject(sceneObject);

			// Clear any existing controls
			MembersPanel.Controls.Clear();
			ChildrenPanel.Controls.Clear();
			mSubsectionLists.Clear();

			if (sceneObject == null)
			{
				// If the scene object is null, hide the contents panel
				ContentsPanel.Hide();
				return;
			}
			else
			{
				// If the scene object is not null, show the contents panel
				ContentsPanel.Show();
			}

			MembersPanel.SuspendLayout();

			//  Build the UI members for the scene objects properties
			var controls = AutoUI.AutoUIFactory.Instance.BuildAutoUI(sceneObject);
			var fieldContainerControls = AutoUI.AutoUIFactory.Instance.BuildAutoUIFieldContainers(controls);

			fieldContainerControls.ForEach(control => MembersPanel.Controls.Add(control));

			MembersPanel.ResumeLayout();

			ChildrenPanel.SuspendLayout();

			// Add the scene objects children
			var children = sceneObject.GetChildren();
			foreach(var child in children)
			{
				// Get the objects scene object attribute
				var attributes = child.GetType().GetCustomAttributes(typeof(SceneObject.SceneObjectAttribute), true);

				if (attributes.Length == 0)
				{
					throw new Exception(String.Format("A scene object type does not define the SceneObject attribute: {0}", child.GetType().Name));
				}

				SceneObject.SceneObjectAttribute sceneObjectAttribute =  attributes[0] as SceneObject.SceneObjectAttribute;

				// If a control has not been added for this object set create one
				SceneObjectGroupBox control = null;
				if (!mSubsectionLists.ContainsKey(sceneObjectAttribute.SceneObjectSet))
				{
					// Create the new control and add it to the list and contents panel
					var newControl = new SceneObjectGroupBox();

					mSubsectionLists.Add(sceneObjectAttribute.SceneObjectSet, newControl);
					ChildrenPanel.Controls.Add(newControl);
				}

				// Get the control for the childs object set
				control = mSubsectionLists[sceneObjectAttribute.SceneObjectSet];

				control.AddSceneObject(child);
			}
			ChildrenPanel.ResumeLayout();
		}
		#endregion Scene Object Display

		#region Events
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Event handler. Called by the ObjectSelectionCombo when the selected value is changed, to
		/// 	update the content panel.
		/// </summary>
		///
		/// <param name="sender">	Source of the event. </param>
		/// <param name="e">	 	Event information. </param>
		private void ObjectSelectionCombo_SelectedValueChanged(object sender, EventArgs e)
		{
			// Change the contents panel to display the selected item
			if (ObjectSelectionCombo.SelectedItem != null)
			{
				SetSceneObject(((SceneObjectProxy)ObjectSelectionCombo.SelectedItem).SceneObject);
				ExtractAllButton.Visible = true;
				ExtractAllButton.Enabled = true;
			}
			else
			{
				SetSceneObject(null);
				ExtractAllButton.Visible = false;
				ExtractAllButton.Enabled = false;
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Event handler. Called by ExtractAllButton when clicked. </summary>
		///
		/// <param name="sender">	Source of the event. </param>
		/// <param name="e">	 	Event information. </param>
		private void ExtractAllButton_Click(object sender, EventArgs e)
		{
			if ((ObjectSelectionCombo.SelectedItem != null) && (ObjectSelectionCombo.SelectedItem is SceneObjectProxy))
			{
				// Extract the selected object
				var sceneObjectProxy = (SceneObjectProxy)ObjectSelectionCombo.SelectedItem;

				if (sceneObjectProxy.SceneObject is OpenSauceIDE.Aether.Extraction.IObjectExtractable)
				{
					var sceneObject = sceneObjectProxy.SceneObject as Extraction.IObjectExtractable;
					string name = AutoUI.AutoUIFactory.GetAutoUIName(sceneObject);

					Aether.Instance.Extraction.RegisterExtractionJob(sceneObject, name);
				}

				// Extract the selected objects children
				sceneObjectProxy.SceneObject.GetChildren().ForEach(
					childObject =>
					{
						if (childObject is OpenSauceIDE.Aether.Extraction.IObjectExtractable)
						{
							var sceneObject = childObject as Extraction.IObjectExtractable;
							string name = AutoUI.AutoUIFactory.GetAutoUIName(sceneObject);

							Aether.Instance.Extraction.RegisterExtractionJob(sceneObject, name);
						}
					}
				);

				Aether.Instance.Extraction.RunCurrentJobs();
			}
		}
		#endregion Events
	}
}
