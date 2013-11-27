using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ComponentModel;
using BlamLib;
using BlamLib.IO;
using BlamLib.Blam;
using BlamLib.Managers;
using OpenSauceIDE.Aether.TagIO;

using H1 = BlamLib.Blam.Halo1;
using COLLADA = BlamLib.Render.COLLADA;
using OpenSauceIDE.Aether.Settings;
using System.IO;
using OpenSauceIDE.Aether.SceneObject;
using BlamLib.Render.COLLADA.Halo1;
using OpenSauceIDE.Aether.AutoUI.Attributes;

namespace OpenSauceIDE.Aether.Project.Halo1
{
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Project class used for the Halo 1 version of Aether. </summary>
	///-------------------------------------------------------------------------------------------------
	public class AetherProjectHalo1 : AetherBaseProject
	{
		#region Fields
		private Dictionary<string, string> mTagExtensions = new Dictionary<string, string>();
		#endregion Fields

		#region Constructor
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Constructor. </summary>
		/// <param name="blamVersion">	The halo 1 blam version. </param>
		///-------------------------------------------------------------------------------------------------
		public AetherProjectHalo1(BlamLib.BlamVersion blamVersion)
		{
			Settings = AetherSettingsHandler.Instance.GetSettings(blamVersion);
			mTagExtensions.Add("SCENARIO", "scenario");
		}
		#endregion Constructor

		#region Project Setup
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates a tag manager. </summary>
		///-------------------------------------------------------------------------------------------------
		public override void CreateTagManager()
		{
			mTagIndexManager = new AetherTagIndexHandler<BlamLib.Managers.TagIndex>(BlamVersion.Halo1_CE, Settings.TagsPath);

			mTagIndexManager.IndexInterface.SetupIgnoreList(
				H1.TagGroups.actr, H1.TagGroups.actv, H1.TagGroups.ant_, H1.TagGroups.antr,
				H1.TagGroups.bipd, H1.TagGroups.boom, H1.TagGroups.cdmg, H1.TagGroups.coll,
				H1.TagGroups.colo, H1.TagGroups.cont, H1.TagGroups.deca, H1.TagGroups.DeLa,
				H1.TagGroups.devc, H1.TagGroups.dobc, H1.TagGroups.effe, H1.TagGroups.elec,
				H1.TagGroups.eqip, H1.TagGroups.flag, H1.TagGroups.fog_, H1.TagGroups.font,
				H1.TagGroups.foot, H1.TagGroups.garb, H1.TagGroups.gelo, H1.TagGroups.glw_,
				H1.TagGroups.grhi, H1.TagGroups.hmt_, H1.TagGroups.hud_, H1.TagGroups.hudg,
				H1.TagGroups.item, H1.TagGroups.itmc, H1.TagGroups.jpt_, H1.TagGroups.lens,
				H1.TagGroups.lsnd, H1.TagGroups.matg, H1.TagGroups.metr, H1.TagGroups.mgs2,
				H1.TagGroups.mply, H1.TagGroups.ngpr, H1.TagGroups.part, H1.TagGroups.pctl,
				H1.TagGroups.phys, H1.TagGroups.plac, H1.TagGroups.pphy,
				H1.TagGroups.proj, H1.TagGroups.rain, H1.TagGroups.seff, H1.TagGroups.sky_,
				H1.TagGroups.snd_, H1.TagGroups.snde, H1.TagGroups.Soul, H1.TagGroups.str_,
				H1.TagGroups.tag_, H1.TagGroups.tagc, H1.TagGroups.trak, H1.TagGroups.udlg,
				H1.TagGroups.unhi, H1.TagGroups.unit, H1.TagGroups.ustr, H1.TagGroups.vcky,
				H1.TagGroups.vehi, H1.TagGroups.weap, H1.TagGroups.wind, H1.TagGroups.wphi,
				H1.TagGroups.yelo);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Disposes of the tag manager. </summary>
		///-------------------------------------------------------------------------------------------------
		public override void DisposeTagManager()
		{
			if (mTagIndexManager != null)
			{
				mTagIndexManager.IndexInterface.UnloadAll();
				mTagIndexManager.Dispose();
				mTagIndexManager = null;
			}
		}
		#endregion Project Setup

		#region Tag Processing
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Process the Halo1 light tag. </summary>
		/// <param name="parent">	 	The parent scene object. </param>
		/// <param name="tagIndex">  	The tag index for the tag. </param>
		/// <param name="tagManager">	The tags manager object. </param>
		///-------------------------------------------------------------------------------------------------
		private void ProcessLight(SceneObjectBase parent
			, TagIndex tagIndex
			, BlamLib.Managers.TagManager tagManager)
		{
			// Create a new light data source
			LightData lightData = new LightData();
			lightData.CollectData(tagIndex, tagManager);

			var sceneObject = new SceneObjectLight(lightData, tagIndex, tagManager);

			// Set the scene objects parent and add it to the list
			if (parent != null)
			{
				sceneObject.SetParent(parent);
			}

			AddSceneObject(sceneObject);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Process a Halo1 object. </summary>
		/// <param name="parent">	 	The parent scene object. </param>
		/// <param name="objectType">	Type of Halo1 object. </param>
		/// <param name="tagIndex">  	The tag index for the tag. </param>
		/// <param name="tagManager">	The tags manager object. </param>
		///-------------------------------------------------------------------------------------------------
		private void ProcessObject(SceneObjectBase parent
			, BlamLib.Blam.Halo1.TypeEnums.ObjectType objectType
			, TagIndex tagIndex
			, BlamLib.Managers.TagManager tagManager)
		{
			// Create a new generic object data source
			ObjectData objectData = new ObjectData();
			objectData.CollectData(tagIndex, tagManager);

			// Create a type specific scene object
			SceneObjectBase sceneObject = null;
			switch (objectType)
			{
				case BlamLib.Blam.Halo1.TypeEnums.ObjectType.Scenery:
					sceneObject = new SceneObjectScenery(objectData, tagIndex, tagManager);
					break;
				case BlamLib.Blam.Halo1.TypeEnums.ObjectType.DeviceMachine:
					sceneObject = new SceneObjectMachine(objectData, tagIndex, tagManager);
					break;
				case BlamLib.Blam.Halo1.TypeEnums.ObjectType.DeviceControl:
					sceneObject = new SceneObjectControl(objectData, tagIndex, tagManager);
					break;
				case BlamLib.Blam.Halo1.TypeEnums.ObjectType.DeviceLightFixture:
					sceneObject = new SceneObjectLightFixture(objectData, tagIndex, tagManager);
					break;
				case BlamLib.Blam.Halo1.TypeEnums.ObjectType.SoundScenery:
					sceneObject = new SceneObjectSoundScenery(objectData, tagIndex, tagManager);
					break;
				default:
					return;
			}

			// Set the parent object and add to the list
			if (parent != null)
			{
				sceneObject.SetParent(parent);
			}

			AddSceneObject(sceneObject);

			// Add all lights in the object
			objectData.Lights.ForEach(
				light =>
				{
					ProcessLight(sceneObject, tagIndex, tagIndex[light.LightDatum]);
				}
			);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Process a structure bsp tag. </summary>
		/// <param name="parent">	 	The parent scene object. </param>
		/// <param name="tagIndex">  	The tag index for the tag. </param>
		/// <param name="tagManager">	The tags manager object. </param>
		///-------------------------------------------------------------------------------------------------
		private void ProcessBSP(SceneObjectBase parent
			, TagIndex tagIndex
			, BlamLib.Managers.TagManager tagManager)
		{
			// Create a bsp data source
			StructureBSPData bspData = new StructureBSPData();
			bspData.CollectData(tagIndex, tagManager);

			// Create a bsp scene obejct
			var sceneObjectStructureBSP = new SceneObjectStructureBSP(parent, bspData, tagIndex, tagManager);
			
			// Set the scene objects parent and add it to the list
			if (parent != null)
			{
				sceneObjectStructureBSP.SetParent(parent);
			}

			AddSceneObject(sceneObjectStructureBSP);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Process a scenario tag. </summary>
		/// <param name="parent">	 	The parent scene object. </param>
		/// <param name="tagIndex">  	The tag index for the tag. </param>
		/// <param name="tagManager">	The tags manager object. </param>
		///-------------------------------------------------------------------------------------------------
		private void ProcessScenario(SceneObjectBase parent
			, TagIndex tagIndex
			, BlamLib.Managers.TagManager tagManager)
		{
			// Create a scenario data source
			ScenarioData scenarioData = new ScenarioData();
			scenarioData.CollectData(tagIndex, tagManager);

			// Create a scenario scene object
			var sceneObjectScenario = new SceneObjectScenario(scenarioData, tagIndex, tagManager);

			// Se the scene obejcts parent and add it to the list
			if (parent != null)
			{
				sceneObjectScenario.SetParent(parent);
			}

			AddSceneObject(sceneObjectScenario);

			// Add all of the objects referenced in the scenario
			// TODO: Duplicates checking
			foreach (var objectTypeList in scenarioData.ScenarioObjectLists)
			{
				objectTypeList.Value.ForEach(
					scenarioObject =>
					{
						ProcessObject(sceneObjectScenario, scenarioObject.ObjectType, tagIndex, tagIndex[scenarioObject.ObjectTagDatum]);
					}
				);
			}

			// Add all BSP's
			// TODO: Duplicates checking
			foreach (var structureBSP in scenarioData.ScenarioStructureBSPs)
			{
				ProcessBSP(sceneObjectScenario, tagIndex, tagIndex[structureBSP.BSPTagDatum]);
			}

			// Add all child scenarios
			// TODO: Duplicates checking
			foreach (var scenario in scenarioData.ScenarioChildScenarios)
			{
				ProcessScenario(sceneObjectScenario, tagIndex, tagIndex[scenario.ScenarioDatum]);
			}
		}
		#endregion Tag Processing

		#region Scenario Open/Close
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Opens a scenario tag from the specified location. </summary>
		/// <param name="location">	The scenario tag to be loaded. </param>
		///-------------------------------------------------------------------------------------------------
		public override void OpenScenario(string location)
		{
			BlamLib.Managers.TagIndex tagIndex = mTagIndexManager.IndexInterface;
			DatumIndex scenarioDatumIndex = tagIndex.Open(location, BlamLib.Blam.Halo1.TagGroups.scnr, ITagStreamFlags.LoadDependents);

			ProcessScenario(null, tagIndex, tagIndex[scenarioDatumIndex]);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Unloads the current scenario and its dependents from memory. </summary>
		///-------------------------------------------------------------------------------------------------
		public override void CloseScenario()
		{
			ClearSceneObjects();
		}
		#endregion Scenario Open/Close

		#region Project Variables
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the tag extension for the specfied tag type. </summary>
		/// <exception cref="Exception">
		/// 	Thrown when a missing tag type extension is requested.
		/// </exception>
		/// <param name="tagTypeID">	Identifier for the tag type. </param>
		/// <returns>	The tag extension. </returns>
		///-------------------------------------------------------------------------------------------------
		public override string GetTagExtension(string tagTypeID)
		{
			if (!mTagExtensions.ContainsKey(tagTypeID))
			{
				throw new Exception("Requested a missing tag extension from the Halo 1 project");
			}

			return mTagExtensions[tagTypeID];
		}
		#endregion Project Variables
	}
}
