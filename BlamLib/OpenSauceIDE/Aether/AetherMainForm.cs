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
		#region Constructor
		public AetherMainForm()
		{
			InitializeComponent();
		}
		#endregion Constructor

		public void InitSettings(BlamLib.BlamVersion blamVersion)
		{
			var controls = AutoUIFactory.Instance.BuildAutoUI(Aether.Instance.CurrentProject.Settings);
			var fieldControls = AutoUIFactory.Instance.BuildAutoUIFieldContainers(controls);

			fieldControls.ForEach(fieldControl => SettingsContainer.Controls.Add(fieldControl));
		}

		public void InitForProject(List<SceneObjectBase> rootObjects)
		{
			rootObjects.ForEach(root => ObjectsContainer.AddSceneObject(root));
		}

		public void Clear()
		{
			ObjectsContainer.ClearSceneObjects();
		}

		private void OpenScenario(object sender, EventArgs e)
		{
			if(!Directory.Exists(Aether.Instance.CurrentProject.Settings.TagsPath))
			{
				return;
			}

			string scenarioExtension = Aether.Instance.CurrentProject.GetTagExtension("SCENARIO");

			OpenFileDialog openFileDialog = new OpenFileDialog();
			openFileDialog.CheckPathExists = true;
			openFileDialog.Filter = String.Format("*.{0}|*.{0}", scenarioExtension);
			openFileDialog.InitialDirectory = Aether.Instance.CurrentProject.Settings.TagsPath;

			DialogResult result = openFileDialog.ShowDialog();
			switch (result)
			{
				case DialogResult.OK:
				case DialogResult.Yes:
					Aether.Instance.OpenScenario(openFileDialog.FileName);
					break;
				default:
					break;
			}
		}

		private void CloseScenario(object sender, EventArgs e)
		{
			Aether.Instance.CloseScenario();
		}
	}
}
