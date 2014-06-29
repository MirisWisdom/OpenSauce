using BlamLib.Managers;
using BlamLib.Render.COLLADA.Halo1;
using OpenSauceIDE.Aether.AutoUI.Attributes;
using OpenSauceIDE.Aether.Extraction;
using OpenSauceIDE.Aether.SceneObject;
using OpenSauceIDE.Aether.Settings;
using System;
using System.IO;

namespace OpenSauceIDE.Aether.Project.Halo1
{
	/// <summary>	Scene object describing a Halo1 scenario. </summary>
	[SceneObject(SceneObjectClassEnum.Root, "SCENARIO"), AutoUINameFormatted("Scenario: {0}", "ObjectName")]
	public class SceneObjectScenario : SceneObjectBase, IObjectExtractable
	{
		#region Constructor
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="scenarioData">	The scenario data class for the scenario. </param>
		/// <param name="tagIndex">	   	Tag tag index containing the tag. </param>
		/// <param name="tagManager">  	The scenario's tag manager. </param>
		public SceneObjectScenario(ScenarioData scenarioData
			, TagIndexBase tagIndex
			, BlamLib.Managers.TagManager tagManager)
		{
			mScenarioData = scenarioData;
			mTagIndex = tagIndex;
			mTagManager = tagManager;

			EnableExtraction = true;
			Scenery = true;
			Machines = true;
			Controls = true;
			LightFixtures = true;
			SoundScenery = true;
		}
		#endregion Constructor

		#region Fields
		private ScenarioData mScenarioData = null;
		private TagIndexBase mTagIndex = null;
		private BlamLib.Managers.TagManager mTagManager = null;
		#endregion Fields

		#region Extraction
		#region Fields
		private bool mEnableExtraction;
		#endregion Fields

		#region Properties
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets or sets a value indicating whether extraction is enabled. </summary>
		///
		/// <value>	true if extraction is enabled, false if not. </value>
		[AutoUI, AutoUIName("Enabled")]
		public bool EnableExtraction
		{
			get
			{
				return mEnableExtraction;
			}
			set
			{
				mEnableExtraction = value;
				OnPropertyChanged("EnableExtraction");
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets or sets a value indicating whether the scenery should be extracted. </summary>
		///
		/// <value>	true if scenery should be extracted, false if not. </value>
		[AutoUI, AutoUIName("Scenery")]
		public bool Scenery
		{
			get
			{
				return mScenarioData.IncludeScenery;
			}
			set
			{
				mScenarioData.IncludeScenery = value;
				OnPropertyChanged("Scenery");
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets or sets a value indicating whether the machines should be extracted. </summary>
		///
		/// <value>	true if machines should be extracted, false if not. </value>
		[AutoUI, AutoUIName("Machines")]
		public bool Machines
		{
			get
			{
				return mScenarioData.IncludeDeviceMachines;
			}
			set
			{
				mScenarioData.IncludeDeviceMachines = value;
				OnPropertyChanged("Machines");
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets or sets a value indicating whether the controls should be extracted. </summary>
		///
		/// <value>	true if controls should be extracted, false if not. </value>
		[AutoUI, AutoUIName("Controls")]
		public bool Controls
		{
			get
			{
				return mScenarioData.IncludeDeviceControls;
			}
			set
			{
				mScenarioData.IncludeDeviceControls = value;
				OnPropertyChanged("Controls");
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets or sets a value indicating whether the light fixtures should be extracted. </summary>
		///
		/// <value>	true if light fixtures should be extracted, false if not. </value>
		[AutoUI, AutoUIName("Light Fixtures")]
		public bool LightFixtures
		{
			get
			{
				return mScenarioData.IncludeDeviceLightFixtures;
			}
			set
			{
				mScenarioData.IncludeDeviceLightFixtures = value;
				OnPropertyChanged("LightFixtures");
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets or sets a value indicating whether the sound scenery should be extracted. </summary>
		///
		/// <value>	true if sound scenery should be extracted, false if not. </value>
		[AutoUI, AutoUIName("Sound Scenery")]
		public bool SoundScenery
		{
			get
			{
				return mScenarioData.IncludeSoundScenery;
			}
			set
			{
				mScenarioData.IncludeSoundScenery = value;
				OnPropertyChanged("SoundScenery");
			}
		}
		#endregion Properties

		/// <summary>	Extracts this object. </summary>
		public bool Extract()
		{
			string outputScenarioPath = Path.Combine(Aether.Instance.CurrentProject.Settings.DataPath, mTagManager.Name);

			var extractor = new ColladaScenarioExporter(Aether.Instance.CurrentProject.Settings,
				mTagIndex,
				mTagManager);
			
			extractor.ErrorOccured +=
				(sender, args) =>
				{
					Aether.Instance.Output.WriteLine(Output.OutputManager.OutputTypeEnum.Error, args.ErrorMessage);
				};

			extractor.AddDataProvider(mScenarioData);
			if (extractor.BuildColladaInstance())
			{
				extractor.SaveDAE(outputScenarioPath + "-objects.dae");
				return true;
			}
			return false;
		}
		#endregion Extraction

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Gets the name of the object. </summary>
		///
		/// <value>	The name of the object. </value>
		public override string ObjectName
		{
			get { return Path.GetFileNameWithoutExtension(mScenarioData.TagPath); }
		}
	}
}
