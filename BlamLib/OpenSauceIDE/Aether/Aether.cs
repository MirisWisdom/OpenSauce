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

namespace OpenSauceIDE.Aether
{
	public class Aether
	{
		/// <summary>The valid engines which CheApe can be used with</summary>
		public const BlamLib.BlamVersion kTargetPlatforms = BlamLib.BlamVersion.Halo1_CE;

		#region Fields
		public AetherBaseProject CurrentProject;
		#endregion

		#region Properties
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the main form. </summary>
		/// <value>	The main form. </value>
		///-------------------------------------------------------------------------------------------------
		public AetherMainForm MainForm { get; private set; }
		#endregion

		#region Constructor
		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Constructor that prevents a default instance of this class from being created.
		/// </summary>
		///-------------------------------------------------------------------------------------------------
		private Aether()
		{
			MainForm = new AetherMainForm();
		}
		#endregion

		#region Singleton
		private static Aether mInstance = new Aether();

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the Aether instance. </summary>
		/// <value>	The Aether instance. </value>
		///-------------------------------------------------------------------------------------------------
		public static Aether Instance
		{
			get { return mInstance; }
		}
		#endregion Singleton
		
		#region Members
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates a new project for the intended Blam engine version.. </summary>
		/// <param name="version">	The intended engine version. </param>
		///-------------------------------------------------------------------------------------------------
		public void SetBlamVersion(BlamVersion version)
		{
			switch (version)
			{
				case BlamVersion.Halo1_CE:
					CurrentProject = new Project.Halo1.AetherProjectHalo1(version);
					break;
			}
			MainForm.InitSettings(version);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Formats the scenario location string and instructs the current project to open the tag.
		/// </summary>
		/// <exception cref="NotImplementedException">
		/// 	Thrown when the requested operation is unimplemented.
		/// </exception>
		/// <param name="location">	. </param>
		///-------------------------------------------------------------------------------------------------
		public void OpenScenario(string location)
		{
			CloseScenario();
			
			if (CurrentProject == null)
			{
				throw new NotImplementedException();
			}

			location = location.Replace(CurrentProject.Settings.TagsPath, "");
			location = Path.ChangeExtension(location, null);
			location = location.TrimStart('\\');

			CurrentProject.CreateTagManager();
			CurrentProject.OpenScenario(location);

			MainForm.InitForProject(CurrentProject.GetSceneObjects(SceneObjectClassEnum.Root));
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Instructs the current project to close the current scenario. </summary>
		/// <exception cref="NotImplementedException">
		/// 	Thrown when the requested operation is unimplemented.
		/// </exception>
		///-------------------------------------------------------------------------------------------------
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

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Extracts the current scenario. </summary>
		/// <exception cref="NotImplementedException">
		/// 	Thrown when the requested operation is unimplemented.
		/// </exception>
		///-------------------------------------------------------------------------------------------------
		public void ExtractCurrent()
		{
			if (CurrentProject == null)
			{
				throw new NotImplementedException();
			}

			CurrentProject.Extract();
		}
		#endregion Members
	}
}
