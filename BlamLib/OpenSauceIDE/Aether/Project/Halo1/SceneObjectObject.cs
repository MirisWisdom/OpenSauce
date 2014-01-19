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
	/// <summary>	Base class for Halo1 objects. </summary>
	public abstract class SceneObjectObject : SceneObjectBase, IObjectExtractable
	{
		#region Fields
		private ObjectData mObjectData = null;
		private TagIndexBase mTagIndex = null;
		private BlamLib.Managers.TagManager mTagManager = null;
		#endregion Fields
		
		#region Constructor
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="objectData">	The parent scene object. </param>
		/// <param name="type">		 	The type of object. </param>
		/// <param name="tagIndex">  	Tag index the tag is associated with. </param>
		/// <param name="tagManager">	Datum index of the tag. </param>
		public SceneObjectObject(ObjectData objectData
			, BlamLib.Blam.Halo1.TypeEnums.ObjectType type
			, TagIndexBase tagIndex
			, BlamLib.Managers.TagManager tagManager)
		{
			mObjectData = objectData;
			mTagIndex = tagIndex;
			mTagManager = tagManager;

			EnableExtraction = true;
		}
		#endregion Constructor

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
		#endregion Properties
		
		/// <summary>	Extracts this object. </summary>
		public bool Extract()
		{
			if (!mObjectData.Model.IsValid)
			{
				return false;
			}

			var modelTagManager = mTagIndex[mObjectData.Model];

			// Export the model
			ModelData modelData = new ModelData();
			ModelShaderData modelShaderData = new ModelShaderData();

			modelData.CollectData(mTagIndex, modelTagManager);
			modelShaderData.CollectData(mTagIndex, modelTagManager);

			var extractor = new ColladaModelExporter(Aether.Instance.CurrentProject.Settings,
				mTagIndex,
				modelTagManager);
			
			extractor.ErrorOccured +=
				(sender, args) =>
				{
					Aether.Instance.Output.WriteLine(Output.OutputManager.OutputTypeEnum.Error, args.ErrorMessage);
				};

			extractor.AddDataProvider(modelData);
			extractor.AddDataProvider(modelShaderData);

			if (extractor.BuildColladaInstance())
			{
				extractor.SaveDAE(Path.Combine(Aether.Instance.CurrentProject.Settings.DataPath, modelData.GetRelativeURL()) + ".dae");
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
			get { return Path.GetFileNameWithoutExtension(mObjectData.TagPath); }
		}
	}

	/// <summary>	Scene object for Halo1 scenery. </summary>
	[SceneObject(SceneObjectClassEnum.Object, "SCENERY"), AutoUINameFormatted("Scenery: {0}", "ObjectName")]
	public class SceneObjectScenery : SceneObjectObject
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="objectData">	Information describing the object. </param>
		/// <param name="tagIndex">  	Tag index containing the tag. </param>
		/// <param name="tagManager">	The tags manager object. </param>
		public SceneObjectScenery(ObjectData objectData
			, TagIndexBase tagIndex
			, BlamLib.Managers.TagManager tagManager)
			: base(objectData, BlamLib.Blam.Halo1.TypeEnums.ObjectType.Scenery, tagIndex, tagManager)
		{ }
	}

	/// <summary>	Scene object for Halo1 device machine. </summary>
	[SceneObject(SceneObjectClassEnum.Object, "DEVICE_MACHINE"), AutoUINameFormatted("Machine: {0}", "ObjectName")]
	public class SceneObjectMachine : SceneObjectObject
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="objectData">	Information describing the object. </param>
		/// <param name="tagIndex">  	Tag index containing the tag. </param>
		/// <param name="tagManager">	The tags manager object. </param>
		public SceneObjectMachine(ObjectData objectData
			, TagIndexBase tagIndex
			, BlamLib.Managers.TagManager tagManager)
			: base(objectData, BlamLib.Blam.Halo1.TypeEnums.ObjectType.DeviceMachine, tagIndex, tagManager)
		{ }
	}

	/// <summary>	Scene object for Halo1 device control. </summary>
	[SceneObject(SceneObjectClassEnum.Object, "DEVICE_CONTROL"), AutoUINameFormatted("Control: {0}", "ObjectName")]
	public class SceneObjectControl : SceneObjectObject
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="objectData">	Information describing the object. </param>
		/// <param name="tagIndex">  	Tag index containing the tag. </param>
		/// <param name="tagManager">	The tags manager object. </param>
		public SceneObjectControl(ObjectData objectData
			, TagIndexBase tagIndex
			, BlamLib.Managers.TagManager tagManager)
			: base(objectData, BlamLib.Blam.Halo1.TypeEnums.ObjectType.DeviceControl, tagIndex, tagManager)
		{ }
	}

	/// <summary>	Scene object for Halo1 device light fixture. </summary>
	[SceneObject(SceneObjectClassEnum.Object, "DEVICE_LIGHT_FIXTURE"), AutoUINameFormatted("Light Fixture: {0}", "ObjectName")]
	public class SceneObjectLightFixture : SceneObjectObject
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="objectData">	Information describing the object. </param>
		/// <param name="tagIndex">  	Tag index containing the tag. </param>
		/// <param name="tagManager">	The tags manager object. </param>
		public SceneObjectLightFixture(ObjectData objectData
			, TagIndexBase tagIndex
			, BlamLib.Managers.TagManager tagManager)
			: base(objectData, BlamLib.Blam.Halo1.TypeEnums.ObjectType.DeviceLightFixture, tagIndex, tagManager)
		{ }
	}

	/// <summary>	Scene object for Halo1 sound scenery. </summary>
	[SceneObject(SceneObjectClassEnum.Object, "SOUND_SCENERY"), AutoUINameFormatted("Sound Scenery: {0}", "ObjectName")]
	public class SceneObjectSoundScenery : SceneObjectObject
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Constructor. </summary>
		///
		/// <param name="objectData">	Information describing the object. </param>
		/// <param name="tagIndex">  	Tag index containing the tag. </param>
		/// <param name="tagManager">	The tags manager object. </param>
		public SceneObjectSoundScenery(ObjectData objectData
			, TagIndexBase tagIndex
			, BlamLib.Managers.TagManager tagManager)
			: base(objectData, BlamLib.Blam.Halo1.TypeEnums.ObjectType.SoundScenery, tagIndex, tagManager)
		{ }
	}
}
