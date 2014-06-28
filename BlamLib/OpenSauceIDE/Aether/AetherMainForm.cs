using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Windows.Forms;
using OpenSauceIDE.Aether.SceneObject;
using OpenSauceIDE.Aether.AutoUI;
using OpenSauceIDE.Aether.AutoUI.Controls;
using OpenSauceIDE.Aether.Controls;
using OpenSauceIDE.Aether.Settings;
using System.IO;

namespace OpenSauceIDE.Aether
{
	public partial class AetherMainForm : Form
	{
		#region Events
		/// <summary>	Values that represent the possible main form actions. </summary>
		public enum FormEventsEnum
		{
			OpenScenario,
			CloseScenario,
			Exit
		}

		/// <summary>	Additional information for form events. </summary>
		public class FormEventArgs : EventArgs
		{
			public FormEventsEnum FormEvent { get; private set; }

			public FormEventArgs(FormEventsEnum formEvent)
			{
				FormEvent = formEvent;
			}
		}
		
		/// <summary>	Occurs when the user interacts with the main form. </summary>
		public event EventHandler<FormEventArgs> FormEventTriggered;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sends an event to any objects listening to the main form events. </summary>
		///
		/// <param name="formEvent">	The form event tp send. </param>
		private void SendEvent(FormEventsEnum formEvent)
		{
			var eventHandler = FormEventTriggered;

			if (eventHandler != null)
			{
				eventHandler(this, new FormEventArgs(formEvent));
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Opens a scenario. </summary>
		///
		/// <param name="sender">	Source of the event. </param>
		/// <param name="e">	 	Event information. </param>
		private void OpenScenario(object sender, EventArgs e)
		{
			SendEvent(FormEventsEnum.OpenScenario);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Closes the current scenario. </summary>
		///
		/// <param name="sender">	Source of the event. </param>
		/// <param name="e">	 	Event information. </param>
		private void CloseScenario(object sender, EventArgs e)
		{
			SendEvent(FormEventsEnum.CloseScenario);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Exits Aether. </summary>
		///
		/// <param name="sender">	Source of the event. </param>
		/// <param name="e">	 	Event information. </param>
		private void Exit(object sender, EventArgs e)
		{
			SendEvent(FormEventsEnum.Exit);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Event callback triggered when the user closes the form. </summary>
		///
		/// <param name="sender">	Source of the event. </param>
		/// <param name="e">	 	Form closing event information. </param>
		private void ExitClosing(object sender, FormClosingEventArgs e)
		{
			SendEvent(FormEventsEnum.Exit);
		}
		#endregion Events

		#region Constructor
		/// <summary>	Initializes a new instance of the AetherMainForm class. </summary>
		public AetherMainForm()
		{
			InitializeComponent();
		}
		#endregion Constructor

		#region Settings Display
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the current settings object. </summary>
		///
		/// <param name="settings">	The settings object to display. </param>
		public void SetSettingsObject(AetherSettings settings)
		{
			var controls = AutoUIFactory.Instance.BuildAutoUI(settings);
			var fieldControls = AutoUIFactory.Instance.BuildAutoUIFieldContainers(controls);

			fieldControls.ForEach(fieldControl => SettingsContainer.Controls.Add(fieldControl));
		}
		#endregion Settings Display

		#region SceneObject Display
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Sets the root scene objects on the form. </summary>
		///
		/// <param name="rootObjects">	The root objects to display. </param>
		public void SetRootObjects(List<SceneObjectBase> rootObjects)
		{
			Clear();

			rootObjects.ForEach(root => ObjectsContainer.AddSceneObject(root));
		}

		/// <summary>	Clears the objects container to its blank/initial state. </summary>
		public void Clear()
		{
			ObjectsContainer.ClearSceneObjects();
		}
		#endregion SceneObject Display

		#region Output
		private uint kMaxLines = 512;

		public void SetOutputManager(Output.OutputManager manager)
		{
			manager.LineAdded += OutputLineAdded;
			manager.OutputCleared += OutputCleared;
		}

		private void OutputCleared(object sender, EventArgs e)
		{
			OutputListBox.Items.Clear();
		}

		private void OutputLineAdded(object sender, Output.OutputManager.OutputManagerEventArgs e)
		{
			bool forceScroll = (OutputListBox.SelectedIndex == -1) || (OutputListBox.SelectedIndex == OutputListBox.Items.Count -1);

			OutputListBox.Items.Add(e.Line);

			while (OutputListBox.Items.Count > kMaxLines)
			{
				OutputListBox.Items.RemoveAt(0);
			}

			if (forceScroll)
			{
				OutputListBox.SelectedIndex = OutputListBox.Items.Count - 1;
			}
		}

		private void CopyOutput(object sender, EventArgs e)
		{
			Aether.Instance.Output.CopyToClipboard();
		}

		private void ClearOutput(object sender, EventArgs e)
		{
			Aether.Instance.Output.Clear();
		}
		#endregion Output
	}
}
