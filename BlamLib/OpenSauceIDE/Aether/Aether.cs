using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using BlamLib;
using BlamLib.IO;
using BlamLib.Blam;
using BlamLib.TagInterface;
using BlamLib.Managers;
using BlamLib.Render;
using OpenSauceIDE.Aether.Settings;
using OpenSauceIDE.Aether.Project;
using OpenSauceIDE.Aether.SceneObject;
using System.IO;
using System.Threading;
using OpenSauceIDE.Aether.Output;

namespace OpenSauceIDE.Aether
{
	public class Aether
	{
		/// <summary>	The valid engines which CheApe can be used with. </summary>
		public const BlamLib.BlamVersion kTargetPlatforms = BlamLib.BlamVersion.Halo1_CE;

		#region Fields
		public AetherBaseProject CurrentProject;
		public Output.OutputManager Output;
		public Extraction.ExtractionManager Extraction;
		#endregion

		#region Properties
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the main form. </summary>
		///
		/// <value>	The main form. </value>
		public AetherMainForm MainForm { get; private set; }
		#endregion

		#region Events
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Callback function called when the user interacts with the main form. </summary>
		///
		/// <param name="sender">	Source of the event. </param>
		/// <param name="e">	 	Form event information. </param>
		void MainFormEventTriggered(object sender, AetherMainForm.FormEventArgs e)
		{
			switch (e.FormEvent)
			{
				case AetherMainForm.FormEventsEnum.OpenScenario:
					OpenScenario();
					break;
				case AetherMainForm.FormEventsEnum.CloseScenario:
					CloseScenario();
					break;
				case AetherMainForm.FormEventsEnum.Exit:
					Exit();
					break;
			}
		}

		void ExtractionProgressed(object sender, Extraction.ExtractionManager.ExtractionEventArgs e)
		{
			switch(e.ExtractionResult)
			{
				case OpenSauceIDE.Aether.Extraction.ExtractionManager.ExtractionResultEnum.InProgress:
					Output.WriteLine(OutputManager.OutputTypeEnum.Normal, "Extraction in progress : " + e.JobID);
					break;
				case OpenSauceIDE.Aether.Extraction.ExtractionManager.ExtractionResultEnum.Succeeded:
					Output.WriteLine(OutputManager.OutputTypeEnum.Normal, "Extraction succeeded : " + e.JobID);
					break;
				case OpenSauceIDE.Aether.Extraction.ExtractionManager.ExtractionResultEnum.Failed:
					Output.WriteLine(OutputManager.OutputTypeEnum.Normal, "Extraction failed : " + e.JobID);
					break;
			}
		}
		#endregion Events

		#region Constructor
		/// <summary>	Constructor that prevents a default instance of this class from being created. </summary>
		private Aether()
		{
			MainForm = new AetherMainForm();
			Output = new Output.OutputManager();
			Extraction = new Extraction.ExtractionManager();

			MainForm.FormEventTriggered += MainFormEventTriggered;
			MainForm.SetOutputManager(Output);

			Extraction.ExtractionProgressed += ExtractionProgressed;
		}
		#endregion

		#region Singleton
		private static Aether mInstance = new Aether();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the Aether instance. </summary>
		///
		/// <value>	The Aether instance. </value>
		public static Aether Instance
		{
			get { return mInstance; }
		}
		#endregion Singleton

		#region Members
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Creates a new project for the intended Blam engine version. </summary>
		///
		/// <exception cref="NotImplementedException">	Thrown when the requested operation is unimplemented. </exception>
		///
		/// <param name="version">	The intended engine version. </param>
		public void SetBlamVersion(BlamVersion version)
		{
			switch (version)
			{
				case BlamVersion.Halo1_CE:
					CurrentProject = new Project.Halo1.AetherProjectHalo1(version);
					break;
				default:
					throw new NotImplementedException("Attempted to set Aether to an unsupported blam version");
			}

			MainForm.SetSettingsObject(Aether.Instance.CurrentProject.Settings);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Formats the scenario location string and instructs the current project to open the tag. </summary>
		///
		/// <exception cref="NotImplementedException">	Thrown when the requested operation is unimplemented. </exception>
		public void OpenScenario()
		{
			if (!Directory.Exists(Aether.Instance.CurrentProject.Settings.InstallationDirectory))
			{
				Output.WriteLine(OutputManager.OutputTypeEnum.Error, "The current installation directory does not exist");
				return;
			}

			if (!Directory.Exists(Aether.Instance.CurrentProject.Settings.TagsPath))
			{
				Output.WriteLine(OutputManager.OutputTypeEnum.Error, "The current installation directory does not contain a tags directory");
				return;
			}

			if (!Directory.Exists(Aether.Instance.CurrentProject.Settings.DataPath))
			{
				Output.WriteLine(OutputManager.OutputTypeEnum.Error, "The current installation directory does not contain a data directory");
				return;
			}

			string scenarioExtension = Aether.Instance.CurrentProject.GetTagExtension("SCENARIO");

			System.Windows.Forms.OpenFileDialog openFileDialog = new System.Windows.Forms.OpenFileDialog();
			openFileDialog.CheckPathExists = true;
			openFileDialog.Filter = System.String.Format("*.{0}|*.{0}", scenarioExtension);
			openFileDialog.InitialDirectory = Aether.Instance.CurrentProject.Settings.TagsPath;

			System.Windows.Forms.DialogResult result = openFileDialog.ShowDialog();
			switch (result)
			{
				case System.Windows.Forms.DialogResult.OK:
				case System.Windows.Forms.DialogResult.Yes:
					break;
				default:
					return;
			}

			CloseScenario();
			
			if (CurrentProject == null)
			{
				throw new NotImplementedException();
			}

			string location = openFileDialog.FileName;
			location = location.Replace(CurrentProject.Settings.TagsPath, "");
			location = Path.ChangeExtension(location, null);
			location = location.TrimStart('\\');

			CurrentProject.CreateTagManager();
			CurrentProject.OpenScenario(location);

			MainForm.SetRootObjects(CurrentProject.GetSceneObjects(SceneObjectClassEnum.Root));
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Instructs the current project to close the current scenario. </summary>
		///
		/// <exception cref="NotImplementedException">	Thrown when the requested operation is unimplemented. </exception>
		public void CloseScenario()
		{
			if (CurrentProject == null)
			{
				throw new NotImplementedException();
			}

			MainForm.Clear();
			CurrentProject.CloseScenario();
			CurrentProject.DisposeTagManager();
		}

		/// <summary>	Closes the current scenario and deletes the project. </summary>
		public void Exit()
		{
			CloseScenario();

			CurrentProject = null;
		}
		#endregion Members
	}
}
