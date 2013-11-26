/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.IO;
using BlamLib.Blam;
using BlamLib.Managers;
using BlamLib.TagInterface;

namespace BlamLib.Render.COLLADA.Halo1
{
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Interface for generic collection of data from a source tag. </summary>
	///-------------------------------------------------------------------------------------------------
	public interface ITagDataSource
	{
		string TagPath { get; }

		void CollectData(TagIndexBase tagIndex, TagManager tagManager);
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Base class to simplify block object access for classes 
	/// 			that map directly to block memebrs. </summary>
	/// <typeparam name="T">	TagInterface.Definition type parameter. </typeparam>
	///-------------------------------------------------------------------------------------------------
	public class DataBlockReference<T>
		where T : TagInterface.Definition
	{
		protected T mBlock;

		protected DataBlockReference(T block)
		{
			mBlock = block;
		}
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Data collection class for the Halo1 scenario tag. </summary>
	///-------------------------------------------------------------------------------------------------
	public class ScenarioData : ITagDataSource, IHalo1ScenarioDataProvider
	{
		#region Fields
		public string TagPath { get; private set; }
		#endregion Fields

		#region Constructor
		public ScenarioData()
		{			
			IncludeScenery = false;
			IncludeDeviceMachines = false;
			IncludeDeviceControls = false;
			IncludeDeviceLightFixtures = false;
			IncludeSoundScenery = false;
		}
		#endregion Constructor

		#region Data Classes
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Proxy class to get data from a child scenario block. </summary>
		///-------------------------------------------------------------------------------------------------
		public class ScenarioChildScenario
			: DataBlockReference<Blam.Halo1.Tags.scenario_group.scenario_child_scenario_block>
		{
			public DatumIndex ScenarioDatum
			{
				get { return mBlock.Child.Datum; }
			}

			public ScenarioChildScenario(Blam.Halo1.Tags.scenario_group.scenario_child_scenario_block block)
				: base(block)
			{ }
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Proxy class to get data from an object name block. </summary>
		///-------------------------------------------------------------------------------------------------
		public class ScenarioObjectName
			: DataBlockReference<Blam.Halo1.Tags.scenario_group.scenario_object_names_block>
		{
			public string Name
			{
				get { return mBlock.Name; }
			}

			public ScenarioObjectName(Blam.Halo1.Tags.scenario_group.scenario_object_names_block block)
				: base(block)
			{ }
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Proxy class to get data from object type block. </summary>
		///-------------------------------------------------------------------------------------------------
		public class ScenarioObject
			: DataBlockReference<Blam.Halo1.Tags.scenario_group.scenario_object_palette_block>
		{
			public Blam.Halo1.TypeEnums.ObjectType ObjectType { get; private set; }

			public DatumIndex ObjectTagDatum
			{
				get { return mBlock.Name.Datum; }
			}

			public ScenarioObject(Blam.Halo1.Tags.scenario_group.scenario_object_palette_block block, Blam.Halo1.TypeEnums.ObjectType type)
				: base(block)
			{
				ObjectType = type;
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Proxy class to get data from an object instance block. </summary>
		///-------------------------------------------------------------------------------------------------
		public class ScenarioObjectInstance
			: DataBlockReference<Blam.Halo1.Tags.scenario_group.scenario_object_block>
		{
			public ScenarioObjectName ObjectName
			{
				get;
				private set;
			}

			public ScenarioObject ObjectType
			{
				get;
				private set;
			}

			public int Permutation
			{
				get { return mBlock.DesiredPermutation; }
			}

			public RealPoint3D Position
			{
				get { return mBlock.Position; }
			}

			public RealEulerAngles3D Rotation
			{
				get { return mBlock.Rotation; }
			}

			public ScenarioObjectInstance(Blam.Halo1.Tags.scenario_group.scenario_object_block block, ScenarioObjectName name, ScenarioObject type)
				: base(block)
			{
				ObjectName = name;
				ObjectType = type;
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Proxy class to get data from a structure bsp block. </summary>
		///-------------------------------------------------------------------------------------------------
		public class ScenarioStructureBSP
			: DataBlockReference<Blam.Halo1.Tags.scenario_structure_bsps_block>
		{
			public DatumIndex BSPTagDatum
			{
				get { return mBlock.StructureBsp.Datum; }
			}

			public ScenarioStructureBSP(Blam.Halo1.Tags.scenario_structure_bsps_block block)
				: base(block)
			{ }
		}
		#endregion Data Classes

		#region Data List Classes
		public class ScenarioChildScenarioList : List<ScenarioChildScenario> { }
		public class ScenarioObjectNameList : List<ScenarioObjectName> { }
		public class ScenarioObjectListDictionary : Dictionary<Blam.Halo1.TypeEnums.ObjectType, List<ScenarioObject>> { }
		public class ScenarioObjectInstanceListDictionary : Dictionary<Blam.Halo1.TypeEnums.ObjectType, List<ScenarioObjectInstance>> { }
		public class ScenarioStructureBSPList : List<ScenarioStructureBSP> { }
		#endregion Data List Classes

		#region Data Lists
		public ScenarioChildScenarioList ScenarioChildScenarios = new ScenarioChildScenarioList();
		public ScenarioObjectNameList ScenarioObjectNames = new ScenarioObjectNameList();
		public ScenarioObjectListDictionary ScenarioObjectLists = new ScenarioObjectListDictionary();
		public ScenarioObjectInstanceListDictionary ScenarioObjectInstanceLists = new ScenarioObjectInstanceListDictionary();
		public ScenarioStructureBSPList ScenarioStructureBSPs = new ScenarioStructureBSPList();
		#endregion Data Lists

		#region Add Scenario Objects and Instances
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the number of instances of the specified object type. </summary>
		/// <param name="type">	The object type. </param>
		/// <returns>	The number of instances. </returns>
		///-------------------------------------------------------------------------------------------------
		public int GetObjectInstanceCount(Blam.Halo1.TypeEnums.ObjectType type)
		{
			if (ScenarioObjectLists[type] == null)
			{
				return 0;
			}

			return ScenarioObjectLists[type].Count;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds object type entries for the objects in the specified palette block. </summary>
		/// <param name="type">   	The object type. </param>
		/// <param name="pallete">	The pallete block. </param>
		///-------------------------------------------------------------------------------------------------
		private void AddObjectPalleteEntries(Blam.Halo1.TypeEnums.ObjectType type, Block<Blam.Halo1.Tags.scenario_group.scenario_object_palette_block> pallete)
		{
			if (!ScenarioObjectLists.ContainsKey(type))
			{
				ScenarioObjectLists[type] = new List<ScenarioObject>();
			}

			foreach(var palleteBlock in pallete.Elements)
			{
				ScenarioObjectLists[type].Add(new ScenarioObject(palleteBlock, type));
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds object type entries for the objects in the specified 
		/// 			instances block. </summary>
		/// <param name="type">			The object type. </param>
		/// <param name="instances">	The instances block. </param>
		///-------------------------------------------------------------------------------------------------
		private void AddObjectInstanceEntries(Blam.Halo1.TypeEnums.ObjectType type, List<Blam.Halo1.Tags.scenario_group.scenario_object_block> instances)
		{
			if (!ScenarioObjectInstanceLists.ContainsKey(type))
			{
				ScenarioObjectInstanceLists[type] = new List<ScenarioObjectInstance>();
			}

			foreach(var instance in instances)
			{
				// If the object has a defined name add a reference to it
				ScenarioObjectName objectName = null;
				if (instance.Name.Value != -1)
				{
					objectName = ScenarioObjectNames[instance.Name.Value];
				}

				// If the objects type is value add a reference to it
				ScenarioObject objectType = null;
				if (instance.Type.Value != -1)
				{
					objectType = ScenarioObjectLists[type][instance.Type.Value];
				}

				ScenarioObjectInstanceLists[type].Add(new ScenarioObjectInstance(instance, objectName, objectType));
			}
		}
		#endregion Add Scenario Objects and Instances

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Collects data from a scenario tag. </summary>
		/// <param name="tagIndex">			 	The tag index containing the scenario tag. </param>
		/// <param name="scenarioTagManager">	Tag manager for the scenario tag. </param>
		///-------------------------------------------------------------------------------------------------
		public void CollectData(TagIndexBase tagIndex, TagManager scenarioTagManager)
		{
			TagPath = scenarioTagManager.Name;

			Blam.Halo1.Tags.scenario_group scenarioDefinition = scenarioTagManager.TagDefinition as Blam.Halo1.Tags.scenario_group;

			// Add child scenarios
			scenarioDefinition.ChildScenarios.Elements.ForEach(
				scenario =>
				{
					ScenarioChildScenarios.Add(new ScenarioChildScenario(scenario));
				}
			);

			// Add all object names
			scenarioDefinition.ObjectNames.Elements.ForEach(
				namesBlock =>
				{
					ScenarioObjectNames.Add(new ScenarioObjectName(namesBlock));
				}
			);

			// Add all object types
			AddObjectPalleteEntries(Blam.Halo1.TypeEnums.ObjectType.Scenery, scenarioDefinition.SceneryPalette);
			AddObjectPalleteEntries(Blam.Halo1.TypeEnums.ObjectType.DeviceMachine, scenarioDefinition.MachinesPalette);
			AddObjectPalleteEntries(Blam.Halo1.TypeEnums.ObjectType.DeviceControl, scenarioDefinition.ControlsPalette);
			AddObjectPalleteEntries(Blam.Halo1.TypeEnums.ObjectType.DeviceLightFixture, scenarioDefinition.LightFixturesPalette);
			AddObjectPalleteEntries(Blam.Halo1.TypeEnums.ObjectType.SoundScenery, scenarioDefinition.SoundSceneryPalette);

			// Add all object instances
			var sceneryInstances = scenarioDefinition.Scenery.Elements.ConvertAll<Blam.Halo1.Tags.scenario_group.scenario_object_block>(
				block => { return block as Blam.Halo1.Tags.scenario_group.scenario_object_block; });
			AddObjectInstanceEntries(Blam.Halo1.TypeEnums.ObjectType.Scenery, sceneryInstances);
			var machineInstances = scenarioDefinition.Machines.Elements.ConvertAll<Blam.Halo1.Tags.scenario_group.scenario_object_block>(
				block => { return block as Blam.Halo1.Tags.scenario_group.scenario_object_block; });
			AddObjectInstanceEntries(Blam.Halo1.TypeEnums.ObjectType.DeviceMachine, machineInstances);
			var contolInstances = scenarioDefinition.Controls.Elements.ConvertAll<Blam.Halo1.Tags.scenario_group.scenario_object_block>(
				block => { return block as Blam.Halo1.Tags.scenario_group.scenario_object_block; });
			AddObjectInstanceEntries(Blam.Halo1.TypeEnums.ObjectType.DeviceControl, contolInstances);
			var lightFixtureInstances = scenarioDefinition.LightFixtures.Elements.ConvertAll<Blam.Halo1.Tags.scenario_group.scenario_object_block>(
				block => { return block as Blam.Halo1.Tags.scenario_group.scenario_object_block; });
			AddObjectInstanceEntries(Blam.Halo1.TypeEnums.ObjectType.DeviceLightFixture, lightFixtureInstances);
			var soundSceneryInstances = scenarioDefinition.SoundScenery.Elements.ConvertAll<Blam.Halo1.Tags.scenario_group.scenario_object_block>(
				block => { return block as Blam.Halo1.Tags.scenario_group.scenario_object_block; });
			AddObjectInstanceEntries(Blam.Halo1.TypeEnums.ObjectType.SoundScenery, soundSceneryInstances);

			// Add all BSP's
			scenarioDefinition.StructureBsps.Elements.ForEach(
				bsp =>
				{
					ScenarioStructureBSPs.Add(new ScenarioStructureBSP(bsp));
				}
			);
		}

		#region IHalo1ScenarioDataProvider Members
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets a list containing all of the objects instances defined
		/// 			by the include flags. </summary>
		/// <returns>	A list of object instances. </returns>
		///-------------------------------------------------------------------------------------------------
		public List<ScenarioObjectInstance> GetObjectInstances()
		{
			List<ScenarioObjectInstance> fullList = new List<ScenarioObjectInstance>();

			if (IncludeScenery && (ScenarioObjectInstanceLists[Blam.Halo1.TypeEnums.ObjectType.Scenery] != null))
			{
				fullList.AddRange(ScenarioObjectInstanceLists[Blam.Halo1.TypeEnums.ObjectType.Scenery]);
			}
			if (IncludeDeviceMachines && (ScenarioObjectInstanceLists[Blam.Halo1.TypeEnums.ObjectType.DeviceMachine] != null))
			{
				fullList.AddRange(ScenarioObjectInstanceLists[Blam.Halo1.TypeEnums.ObjectType.DeviceMachine]);
			}
			if (IncludeDeviceControls && (ScenarioObjectInstanceLists[Blam.Halo1.TypeEnums.ObjectType.DeviceControl] != null))
			{
				fullList.AddRange(ScenarioObjectInstanceLists[Blam.Halo1.TypeEnums.ObjectType.DeviceControl]);
			}
			if (IncludeDeviceLightFixtures && (ScenarioObjectInstanceLists[Blam.Halo1.TypeEnums.ObjectType.DeviceLightFixture] != null))
			{
				fullList.AddRange(ScenarioObjectInstanceLists[Blam.Halo1.TypeEnums.ObjectType.DeviceLightFixture]);
			}
			if (IncludeSoundScenery && (ScenarioObjectInstanceLists[Blam.Halo1.TypeEnums.ObjectType.SoundScenery] != null))
			{
				fullList.AddRange(ScenarioObjectInstanceLists[Blam.Halo1.TypeEnums.ObjectType.SoundScenery]);
			}

			return fullList;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Gets or sets a value indicating whether scenery should be included.
		/// </summary>
		/// <value>	true if include scenery, false if not. </value>
		///-------------------------------------------------------------------------------------------------
		public bool IncludeScenery { get; set; }

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Gets or sets a value indicating whether device machines should be included.
		/// </summary>
		/// <value>	true if include machines, false if not. </value>
		///-------------------------------------------------------------------------------------------------
		public bool IncludeDeviceMachines { get; set; }

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Gets or sets a value indicating whether device controls should be included.
		/// </summary>
		/// <value>	true if include device controls, false if not. </value>
		///-------------------------------------------------------------------------------------------------
		public bool IncludeDeviceControls { get; set; }

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Gets or sets a value indicating whether device light fixtures should be included.
		/// </summary>
		/// <value>	true if include device light fixtures, false if not. </value>
		///-------------------------------------------------------------------------------------------------
		public bool IncludeDeviceLightFixtures { get; set; }

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Gets or sets a value indicating whether sound scenery should be included.
		/// </summary>
		/// <value>	true if include sound scenery, false if not. </value>
		///-------------------------------------------------------------------------------------------------
		public bool IncludeSoundScenery { get; set; }
		#endregion IHalo1ScenarioDataProvider Members
	};

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Data collection class for Halo1 object tags. </summary>
	///-------------------------------------------------------------------------------------------------
	public class ObjectData : ITagDataSource
	{
		#region Fields
		public string TagPath { get; private set; }
		#endregion Fields

		#region Data Classes
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Proxy class to get data from a light attachment block. </summary>
		///-------------------------------------------------------------------------------------------------
		public class ObjectLight
			: DataBlockReference<Blam.Halo1.Tags.object_group.object_attachment_block>
		{
			public string Marker
			{
				get { return mBlock.Marker; }
			}

			public DatumIndex LightDatum
			{
				get { return mBlock.Type.Datum; }
			}

			public ObjectLight(Blam.Halo1.Tags.object_group.object_attachment_block block)
				: base(block)
			{ }
		}
		#endregion Data Classes

		#region Data List Classes
		public class ObjectLightList : List<ObjectLight> { }
		#endregion Data List Classes

		#region Data
		public DatumIndex Model = DatumIndex.Null;
		public ObjectLightList Lights = new ObjectLightList();
		#endregion Data

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Collects data from an object tag. </summary>
		/// <param name="tagIndex">		   	The tag index containing the object tag. </param>
		/// <param name="objectTagManager">	Tag manager for the object tag. </param>
		///-------------------------------------------------------------------------------------------------
		public void CollectData(TagIndexBase tagIndex, TagManager objectTagManager)
		{
			TagPath = objectTagManager.Name;

			Blam.Halo1.Tags.object_group objectTag = objectTagManager.TagDefinition as Blam.Halo1.Tags.object_group;

			// Model
			Model = objectTag.Model.Datum;

			// Attachments (Lights)
			objectTag.Attachments.Elements.ForEach(
				attachment =>
				{
					if (attachment.Type.Datum.IsNull
						|| TagIndex.IsSentinel(attachment.Type.Datum)
						|| !tagIndex[attachment.Type.Datum].GroupTag.Equals(Blam.Halo1.TagGroups.ligh))
					{
						return;
					}

					Lights.Add(new ObjectLight(attachment));
				}
			);
		}
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Data collection class for Halo1 light tags. </summary>
	///-------------------------------------------------------------------------------------------------
	public class LightData : ITagDataSource, IHalo1LightDataProvider, IColladaExternalReference
	{
		#region Fields
		public string TagPath { get; private set; }
		#endregion Fields

		#region Data Classes
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Proxy class to get data from a light tag. </summary>
		///-------------------------------------------------------------------------------------------------
		public class LightDetails
			: DataBlockReference<Blam.Halo1.Tags.light_group>
		{
			public string Name { get; private set; }

			public bool IsDynamic
			{
				get { return mBlock.Flags.Test(1); }
			}

			public TagInterface.RealColor Color
			{
				get { return mBlock.ColorUpperBound; }
			}

			public TagInterface.RealColor RadiosityColor
			{
				get { return mBlock.RadiosityColor; }
			}

			public float ConstantAttenuation
			{
				get { return 1; }
			}

			public float LinearAttenuation
			{
				get { return mBlock.ShapeRadius; }
			}

			public float QuadraticAttenuation
			{
				get { return 1; }
			}

			public float FalloffAngle
			{
				get { return mBlock.FalloffAngle; }
			}

			public float FalloffExponent
			{
				get { return 1; }
			}

			public LightDetails(Blam.Halo1.Tags.light_group block, string name)
				: base(block)
			{
				Name = name;
			}
		}
		#endregion Data Classes

		#region Data
		public LightDetails Light = null;
		#endregion Data

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Collects data from a light tag. </summary>
		/// <param name="tagIndex">		  	The tag index containing the light tag. </param>
		/// <param name="lightTagManager">	Tag manager for the light tag. </param>
		///-------------------------------------------------------------------------------------------------
		public void CollectData(TagIndexBase tagIndex, TagManager lightTagManager)
		{
			TagPath = lightTagManager.Name;

			var lightDefinition = lightTagManager.TagDefinition as Blam.Halo1.Tags.light_group;

			Light = new LightDetails(lightDefinition, Path.GetFileNameWithoutExtension(TagPath));
		}

		#region IColladaExternalReference Members
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the relative URL of the tag. </summary>
		/// <returns>	The relative URL of the tag. </returns>
		///-------------------------------------------------------------------------------------------------
		public string GetRelativeURL()
		{
			return TagPath;
		}
		#endregion

		#region IHalo1LightDataProvider Members
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the light details. </summary>
		/// <returns>	The light details. </returns>
		///-------------------------------------------------------------------------------------------------
		public LightDetails GetLight()
		{
			return Light;
		}
		#endregion
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Data collection class for Halo1 model tags. </summary>
	///-------------------------------------------------------------------------------------------------
	public class ModelData : ITagDataSource, IHalo1ModelDataProvider, IColladaExternalReference
	{
		#region Fields
		public string TagPath { get; private set; }
		#endregion Fields

		#region Data Classes
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Class holding a reference to a shader datum. </summary>
		///-------------------------------------------------------------------------------------------------
		public class ModelShaderReference
		{
			public DatumIndex ShaderDatum { get; private set; }
			public string MaterialName { get; private set; }

			public ModelShaderReference(DatumIndex shaderDatum, string materialName)
			{
				ShaderDatum = shaderDatum;
				MaterialName = materialName;
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Data class for a single piece of geometry. </summary>
		///-------------------------------------------------------------------------------------------------
		public class ModelGeometrySet
		{
			public string Name
			{
				get;
				private set;
			}

			public List<int> Permutations
			{
				get;
				private set;
			}

			public Blam.Halo1.TypeEnums.LevelOfDetailEnum LevelOfDetail
			{
				get;
				private set;
			}

			public Blam.Halo1.Tags.gbxmodel_group.model_geometry_block Geometry
			{
				get;
				private set;
			}

			public ModelShaderReferenceList Shaders
			{
				get;
				private set;
			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Adds a shader to the geometries shader list. </summary>
			/// <param name="shaderType">	The shader type reference. </param>
			///-------------------------------------------------------------------------------------------------
			public void AddShader(ModelShaderReference shaderType)
			{
				if (!Shaders.Exists(shader => { return shader.ShaderDatum == shaderType.ShaderDatum; }))
				{
					Shaders.Add(shaderType);
				}
			}

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Adds a permutation index that this geometry is used in. </summary>
			/// <param name="permutation">	The permutation index. </param>
			///-------------------------------------------------------------------------------------------------
			public void AddPermutation(int permutation)
			{
				if (!Permutations.Contains(permutation))
				{
					Permutations.Add(permutation);
				}
			}

			public ModelGeometrySet(string name
				, Blam.Halo1.TypeEnums.LevelOfDetailEnum lod
				, Blam.Halo1.Tags.gbxmodel_group.model_geometry_block geometry)
			{
				Name = name;
				Permutations = new List<int>();
				LevelOfDetail = lod;
				Geometry = geometry;
				Shaders = new ModelShaderReferenceList();
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Proxy class to get data from a node block. </summary>
		///-------------------------------------------------------------------------------------------------
		public class ModelBone
			: DataBlockReference<Blam.Halo1.Tags.model_group.model_node_block>
		{
			public string Name
			{
				get { return mBlock.Name; }
			}

			public int Parent
			{
				get { return mBlock.ParentNode; }
			}

			public int Child
			{
				get { return mBlock.FirstChildNode; }
			}

			public int Sibling
			{
				get { return mBlock.NextSiblingNode; }
			}

			public TagInterface.RealPoint3D Position
			{
				get { return mBlock.DefaultTranslation; }
			}

			public TagInterface.RealQuaternion Rotation
			{
				get { return mBlock.DefaultRotation; }
			}

			public ModelBone(Blam.Halo1.Tags.model_group.model_node_block block)
				: base(block)
			{ }
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Proxy class to get data from a marker block. </summary>
		///-------------------------------------------------------------------------------------------------
		public class ModelMarker
			: DataBlockReference<Blam.Halo1.Tags.model_group.model_markers_block>
		{
			public string Name
			{
				get { return mBlock.Name; }
			}

			public ModelMarker(Blam.Halo1.Tags.model_group.model_markers_block block)
				: base(block)
			{ }
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Proxy class to get data from a marker instance block. </summary>
		///-------------------------------------------------------------------------------------------------
		public class ModelMarkerInstance
			: DataBlockReference<Blam.Halo1.Tags.model_group.model_markers_block.model_marker_instance_block>
		{
			public ModelMarker MarkerType
			{
				get; 
				private set;
			}

			public int Region
			{
				get { return mBlock.RegionIndex; }
			}

			public int Permutation
			{
				get { return mBlock.PermutationIndex; }
			}

			public int Bone
			{
				get { return mBlock.NodeIndex; }
			}

			public TagInterface.RealPoint3D Position
			{
				get { return mBlock.Translation; }
			}

			public TagInterface.RealQuaternion Rotation
			{
				get { return mBlock.Rotation; }
			}

			public ModelMarkerInstance(Blam.Halo1.Tags.model_group.model_markers_block.model_marker_instance_block block, ModelMarker markerType)
				: base(block)
			{
				MarkerType = markerType;
			}
		}
		#endregion Data Classes

		#region Data List Classes
		public class ModelBoneList : List<ModelBone> { }
		public class ModelGeometrySetList : List<ModelGeometrySet> { }
		public class ModelMarkerList : List<ModelMarker> { }
		public class ModelMarkerInstanceList : List<ModelMarkerInstance> { }
		public class ModelShaderReferenceList : List<ModelShaderReference> { }
		#endregion Data List Classes

		#region Data
		public ModelBoneList ModelBones = new ModelBoneList();
		public ModelGeometrySetList ModelGeometrySets = new ModelGeometrySetList();
		public ModelMarkerList ModelMarkers = new ModelMarkerList();
		public ModelMarkerInstanceList ModelMarkerInstances = new ModelMarkerInstanceList();
		#endregion Data

		#region Add Bones, Geometry, Shaders and Markers
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the number of permutations in a model group. </summary>
		/// <param name="modelDefinition">	The model definition. </param>
		/// <returns>	The permutation count. </returns>
		///-------------------------------------------------------------------------------------------------
		private int GetPermutationCount(Blam.Halo1.Tags.gbxmodel_group modelDefinition)
		{
			int permutationCount = 0;
			foreach (var region in modelDefinition.Regions)
			{
				permutationCount = (region.Permutations.Count > permutationCount ? region.Permutations.Count : permutationCount);
			}
			return permutationCount;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds the bones contained in a model to the bones list. </summary>
		/// <param name="modelDefinitionmodelDefinition">	The model tag. </param>
		///-------------------------------------------------------------------------------------------------
		private void AddBones(Blam.Halo1.Tags.gbxmodel_group modelDefinition)
		{
			// Add all nodes in order
			foreach (var node in modelDefinition.Nodes)
			{
				ModelBones.Add(new ModelBone(node));
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds the geometry from a model tag to the geometry list. </summary>
		/// <param name="modelDefinition">	The model definition. </param>
		/// <param name="permutation">	  	The permutation to process. </param>
		/// <param name="levelOfDetail">  	The level of detail to process. </param>
		///-------------------------------------------------------------------------------------------------
		private void AddGeometry(Blam.Halo1.Tags.gbxmodel_group modelDefinition
			, int permutation
			, Blam.Halo1.TypeEnums.LevelOfDetailEnum levelOfDetail)
		{
			// Add a model poiece from every region
			foreach (var region in modelDefinition.Regions)
			{
				// If the requested permutation index is higher than the regions permutation count, use the highest permutation possible
				int permutationIndex = permutation;

				if (permutation >= region.Permutations.Count)
				{
					permutationIndex = region.Permutations.Count - 1;
				}

				// Build the models name
				string name = string.Format("{0}-{1}-lod{2}", region.Name.Value, region.Permutations[permutationIndex].Name, ((int)levelOfDetail).ToString());

				int index = 0;
				switch (levelOfDetail)
				{
					case Blam.Halo1.TypeEnums.LevelOfDetailEnum.SuperHigh: index = region.Permutations[permutationIndex].SuperHigh; break;
					case Blam.Halo1.TypeEnums.LevelOfDetailEnum.High: index = region.Permutations[permutationIndex].High; break;
					case Blam.Halo1.TypeEnums.LevelOfDetailEnum.Medium: index = region.Permutations[permutationIndex].Medium; break;
					case Blam.Halo1.TypeEnums.LevelOfDetailEnum.Low: index = region.Permutations[permutationIndex].Low; break;
					case Blam.Halo1.TypeEnums.LevelOfDetailEnum.SuperLow: index = region.Permutations[permutationIndex].SuperLow; break;
				};

				// Find and existing geometry set if present
				ModelGeometrySet geometrySet = ModelGeometrySets.Find(
					geometry =>
					{
						return (geometry.Name == name) && (geometry.LevelOfDetail == levelOfDetail);
					}
				);

				// Create a new geometry set for the current geometry if there is no pre-existing one
				if (geometrySet == null)
				{
					geometrySet = new ModelGeometrySet(name, levelOfDetail, modelDefinition.Geometries[index]);
					ModelGeometrySets.Add(geometrySet);
				}

				// Add the permutation index to the geometry set
				geometrySet.AddPermutation(permutation);
			}
		}
		
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds shaders to all of the geometry sets</summary>
		/// <param name="tagIndex">	The tag index containing the model tag. </param>
		/// <param name="modelDefinition">	The model definition. </param>
		///-------------------------------------------------------------------------------------------------
		private void AddShadersToGeometry(TagIndexBase tagIndex, Blam.Halo1.Tags.gbxmodel_group modelDefinition)
		{
			// For every geometry set, add references to the shaders used by its parts
			foreach (var geometrySet in ModelGeometrySets)
			{
				foreach (var part in geometrySet.Geometry.Parts)
				{
					DatumIndex shaderDatum = modelDefinition.Shaders[part.ShaderIndex].Shader.Datum;

					string shaderName = Path.GetFileNameWithoutExtension(tagIndex[shaderDatum].Name);

					geometrySet.AddShader(new ModelShaderReference(shaderDatum, shaderName));
				}
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds the markers and marker instances from the model tag to the markers lists. </summary>
		/// <param name="modelDefinition">	The model definition. </param>
		///-------------------------------------------------------------------------------------------------
		private void AddMarkers(Blam.Halo1.Tags.gbxmodel_group modelDefinition)
		{
			// Add all marker type
			foreach (var marker in modelDefinition.Markers)
			{
				ModelMarkers.Add(new ModelMarker(marker));
			}

			// Add all marker instances
			for (int i = 0; i < modelDefinition.Markers.Count; i++)
			{
				var modelMarker = ModelMarkers[i];

				foreach (var instance in modelDefinition.Markers[i].Instances)
				{
					ModelMarkerInstances.Add(new ModelMarkerInstance(instance, modelMarker));
				}
			}
		}
		#endregion Add Bones, Geometry, Shaders and Markers

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Collects data from a model tag. </summary>
		/// <param name="tagIndex">		  	The tag index containing the model tag. </param>
		/// <param name="modelTagManager">	Tag manager for the model tag. </param>
		///-------------------------------------------------------------------------------------------------
		public void CollectData(TagIndexBase tagIndex, TagManager modelTagManager)
		{
			TagPath = modelTagManager.Name;

			var modelTag = modelTagManager.TagDefinition as Blam.Halo1.Tags.gbxmodel_group;

			// Get the total number of permutations in the model
			int permutationCount = GetPermutationCount(modelTag);

			// Create a geometry set for every permutation at every level of detail
			for (int i = 0; i < permutationCount; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					AddGeometry(modelTag, i, (BlamLib.Blam.Halo1.TypeEnums.LevelOfDetailEnum)j);
				}
			}

			// Add shaders, bones and markers to the relevant lists
			AddShadersToGeometry(tagIndex, modelTag);
			AddBones(modelTag);
			AddMarkers(modelTag);
		}

		#region IColladaExternalReference Members
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the relative URL of the model tag. </summary>
		/// <returns>	The relative URL of the model tag. </returns>
		///-------------------------------------------------------------------------------------------------
		public string GetRelativeURL()
		{
			return TagPath;
		}
		#endregion

		#region IHalo1ModelDataProvider Members
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the bones list. </summary>
		/// <returns>	The bones list. </returns>
		///-------------------------------------------------------------------------------------------------
		public ModelData.ModelBoneList GetBones()
		{
			return ModelBones;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the geometry list. </summary>
		/// <returns>	The geometry list. </returns>
		///-------------------------------------------------------------------------------------------------
		public ModelGeometrySetList GetGeometries()
		{
			return ModelGeometrySets;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the geometry list for a specific permutation and LOD. </summary>
		/// <param name="permutation">	The permutation to get. </param>
		/// <param name="lod">		  	The LOD to get. </param>
		/// <returns>	The geometry list for the specific permutation and LOD. </returns>
		///-------------------------------------------------------------------------------------------------
		public ModelGeometrySetList GetGeometries(int permutation, Blam.Halo1.TypeEnums.LevelOfDetailEnum lod)
		{
			var geometryList = new ModelGeometrySetList();
			foreach (var geometrySet in ModelGeometrySets)
			{
				if (geometrySet.Permutations.Contains(permutation) && (geometrySet.LevelOfDetail == lod))
				{
					geometryList.Add(geometrySet);
				}
			}

			return geometryList;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets marker instances list. </summary>
		/// <returns>	The marker instances list. </returns>
		///-------------------------------------------------------------------------------------------------
		public ModelMarkerInstanceList GetMarkerInstances()
		{
			return ModelMarkerInstances;
		}
		#endregion
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Data collection class for Halo1 structure bsp tags. </summary>
	///-------------------------------------------------------------------------------------------------
	public class StructureBSPData : ITagDataSource, IHalo1BSPDataProvider
	{
		#region Fields
		public string TagPath { get; private set; }
		#endregion Fields

		#region Constructor
		public StructureBSPData()
		{
			IncludeRenderMesh = false;
			IncludePortals = false;
			IncludeFogPlanes = false;
		}
		#endregion Constructor

		#region Data Classes
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Proxy class to get data from a marker block. </summary>
		///-------------------------------------------------------------------------------------------------
		public class StructureBSPMarker
			: DataBlockReference<Blam.Halo1.Tags.structure_bsp_group.structure_bsp_marker_block>
		{
			public string Name
			{
				get { return mBlock.Name; }
			}

			public TagInterface.RealPoint3D Position
			{
				get { return mBlock.Position; }
			}

			public TagInterface.RealQuaternion Rotation
			{
				get { return mBlock.Rotation; }
			}

			public StructureBSPMarker(Blam.Halo1.Tags.structure_bsp_group.structure_bsp_marker_block block)
				: base(block)
			{ }
		}
		#endregion Data Classes

		#region Data List Classes
		public class StructureBSPMarkerList : List<StructureBSPMarker> { }
		#endregion Data List Classes

		#region Data
		public StructureBSPMarkerList StructureBSPMarkers = new StructureBSPMarkerList();
		#endregion Data
		
		#region Add Markers
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds the markers from the bsp to the markers list. </summary>
		/// <param name="bspDefinition">	The bsp definition. </param>
		///-------------------------------------------------------------------------------------------------
		private void AddMarkers(Blam.Halo1.Tags.structure_bsp_group bspDefinition)
		{
			foreach (var marker in bspDefinition.Markers)
			{
				StructureBSPMarkers.Add(new StructureBSPMarker(marker));
			}
		}
		#endregion Add Markers

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Collects data from a structure bsp tag. </summary>
		/// <param name="tagIndex">			Tag index containing the bsp tag. </param>
		/// <param name="bspTagManager">	Manager for the bsp tag. </param>
		///-------------------------------------------------------------------------------------------------
		public void CollectData(TagIndexBase tagIndex, TagManager bspTagManager)
		{
			TagPath = bspTagManager.Name;

			var bspTag = bspTagManager.TagDefinition as Blam.Halo1.Tags.structure_bsp_group;

			AddMarkers(bspTag);
		}

		#region IHalo1BSPDataProvider Members
		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Gets or sets a value indicating whether the render mesh should be included.
		/// </summary>
		/// <value>	true if include render mesh, false if not. </value>
		///-------------------------------------------------------------------------------------------------
		public bool IncludeRenderMesh { get; set; }

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Gets or sets a value indicating whether the portals should be included.
		/// </summary>
		/// <value>	true if include portals, false if not. </value>
		///-------------------------------------------------------------------------------------------------
		public bool IncludePortals { get; set; }

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Gets or sets a value indicating whether the fog planes should be included.
		/// </summary>
		/// <value>	true if include fog planes, false if not. </value>
		///-------------------------------------------------------------------------------------------------
		public bool IncludeFogPlanes { get; set; }

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the markers list. </summary>
		/// <returns>	The markers list. </returns>
		///-------------------------------------------------------------------------------------------------
		public StructureBSPMarkerList GetMarkers()
		{
			return StructureBSPMarkers;
		}
		#endregion
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Generic data collection class for Halo1 shader tags. </summary>
	///-------------------------------------------------------------------------------------------------
	public abstract class ShaderData : ITagDataSource, IColladaShaderDataProvider
	{
		#region Fields
		public string TagPath { get; private set; }
		#endregion Fields

		#region Data Classes
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Shader type class contaning a shader datum and it's collada
		/// 			effect definition. </summary>
		///-------------------------------------------------------------------------------------------------
		public class ShaderType
		{
			public DatumIndex ShaderDatum { get; private set; }
			public ColladaExporter.Effect EffectReference { get; private set; }

			///-------------------------------------------------------------------------------------------------
			/// <summary>	Sets shaders collada effect definition reference. </summary>
			/// <param name="reference">	The collada effect definition. </param>
			///-------------------------------------------------------------------------------------------------
			public void SetEffectReference(ColladaExporter.Effect reference)
			{
				EffectReference = reference;
			}

			public ShaderType(DatumIndex shaderDatum)
			{
				ShaderDatum = shaderDatum;
			}
		}
		#endregion

		#region Data List Classes
		public class ShaderTypeList : List<ShaderType> { }
		#endregion Data List Classes

		#region Data
		private ColladaExporter.ImageList mImages = new ColladaExporter.ImageList();
		private ColladaExporter.EffectList mEffects = new ColladaExporter.EffectList();
		private ColladaExporter.MaterialList mMaterials = new ColladaExporter.MaterialList();
		protected ShaderTypeList mShaderTypeList = new ShaderTypeList();
		#endregion Data

		#region Add Shaders and Bitmaps
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets up an effect definition from a shader environment tag. </summary>
		/// <param name="effectDefinition">	The effect definition to set up. </param>
		/// <param name="tagIndex">		   	The tag index containing the shader tag. </param>
		/// <param name="shader">		   	The shader tag definition. </param>
		///-------------------------------------------------------------------------------------------------
		private void SetupEffectDefinition(ColladaExporter.Effect effectDefinition
			, TagIndexBase tagIndex
			, Blam.Halo1.Tags.shader_environment_group shader)
		{
			// Set diffuse texture/color
			if (!shader.BaseMap.Datum.IsNull)
			{
				var image = new ColladaExporter.Image(tagIndex[shader.BaseMap.Datum].Name);
				mImages.Add(image);

				effectDefinition.AddImage(image);
				effectDefinition.AddPhongValue("Diffuse", new ColladaExporter.Phong.Texture(image.ImagePath, 0));
			}
			else
			{
				effectDefinition.AddPhongValue("Diffuse", new TagInterface.RealColor(shader.MaterialColor, 1));
			}

			// Set alpha tested bump map
			if (shader.Flags.Test(1) && (!shader.BumpMap.Datum.IsNull))
			{
				var image = new ColladaExporter.Image(tagIndex[shader.BumpMap.Datum].Name);
				mImages.Add(image);

				effectDefinition.AddImage(image);
				effectDefinition.AddPhongValue("Transparent", new ColladaExporter.Phong.Texture(image.ImagePath, 0));
			}

			// Set specular color
			effectDefinition.AddPhongValue("Specular", new TagInterface.RealColor(shader.PerpendicularColor, 1));
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets up an effect definition from a shader model tag. </summary>
		/// <param name="effectDefinition">	The effect definition to set up. </param>
		/// <param name="tagIndex">		   	The tag index containing the shader tag. </param>
		/// <param name="shader">		   	The shader tag definition. </param>
		///-------------------------------------------------------------------------------------------------
		private void SetupEffectDefinition(ColladaExporter.Effect effectDefinition
			, TagIndexBase tagIndex
			, Blam.Halo1.Tags.shader_model_group shader)
		{
			// Set diffuse texture/color
			if (!shader.BaseMap.Datum.IsNull)
			{
				var image = new ColladaExporter.Image(tagIndex[shader.BaseMap.Datum].Name);
				mImages.Add(image);

				effectDefinition.AddImage(image);
				effectDefinition.AddPhongValue("Diffuse", new ColladaExporter.Phong.Texture(image.ImagePath, 0));
			}
			
			// Set specular color
			effectDefinition.AddPhongValue("Specular", new TagInterface.RealColor(shader.PerpendicularTintColor, 1));
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets up an effect definition from a shader transparent chicago tag. </summary>
		/// <param name="effectDefinition">	The effect definition to set up. </param>
		/// <param name="tagIndex">		   	The tag index containing the shader tag. </param>
		/// <param name="shader">		   	The shader tag definition. </param>
		///-------------------------------------------------------------------------------------------------
		private void SetupEffectDefinition(ColladaExporter.Effect effectDefinition
			, TagIndexBase tagIndex
			, Blam.Halo1.Tags.shader_transparent_chicago_group shader)
		{
			// Set diffuse texture
			if ((shader.Maps.Count > 0) && (!shader.Maps[0].Map.Datum.IsNull))
			{
				var image = new ColladaExporter.Image(tagIndex[shader.Maps[0].Map.Datum].Name);
				mImages.Add(image);

				effectDefinition.AddImage(image);
				effectDefinition.AddPhongValue("Diffuse", new ColladaExporter.Phong.Texture(image.ImagePath, 0));
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets up an effect definition from a shader transparent chicago extended tag. </summary>
		/// <param name="effectDefinition">	The effect definition to set up. </param>
		/// <param name="tagIndex">		   	The tag index containing the shader tag. </param>
		/// <param name="shader">		   	The shader tag definition. </param>
		///-------------------------------------------------------------------------------------------------
		private void SetupEffectDefinition(ColladaExporter.Effect effectDefinition
			, TagIndexBase tagIndex
			, Blam.Halo1.Tags.shader_transparent_chicago_extended_group shader)
		{
			// Set diffuse texture
			DatumIndex bitmapDatum = DatumIndex.Null;
			if (shader._4StageMaps.Count > 0)
			{
				bitmapDatum = shader._4StageMaps[0].Map.Datum;
			}
			else if (shader._2StageMaps.Count > 0)
			{
				bitmapDatum = shader._2StageMaps[0].Map.Datum;
			}

			if (!bitmapDatum.IsNull)
			{
				var image = new ColladaExporter.Image(tagIndex[bitmapDatum].Name);
				mImages.Add(image);

				effectDefinition.AddImage(image);
				effectDefinition.AddPhongValue("Diffuse", new ColladaExporter.Phong.Texture(image.ImagePath, 0));
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets up an effect definition from a shader transparent generic tag. </summary>
		/// <param name="effectDefinition">	The effect definition to set up. </param>
		/// <param name="tagIndex">		   	The tag index containing the shader tag. </param>
		/// <param name="shader">		   	The shader tag definition. </param>
		///-------------------------------------------------------------------------------------------------
		private void SetupEffectDefinition(ColladaExporter.Effect effectDefinition
			, TagIndexBase tagIndex
			, Blam.Halo1.Tags.shader_transparent_generic_group shader)
		{
			if ((shader.Maps.Count > 0) && (!shader.Maps[0].Map.Datum.IsNull))
			{
				var image = new ColladaExporter.Image(tagIndex[shader.Maps[0].Map.Datum].Name);
				mImages.Add(image);

				effectDefinition.AddImage(image);
				effectDefinition.AddPhongValue("Diffuse", new ColladaExporter.Phong.Texture(image.ImagePath, 0));
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets up an effect definition from a shader transparent glass tag. </summary>
		/// <param name="effectDefinition">	The effect definition to set up. </param>
		/// <param name="tagIndex">		   	The tag index containing the shader tag. </param>
		/// <param name="shader">		   	The shader tag definition. </param>
		///-------------------------------------------------------------------------------------------------
		private void SetupEffectDefinition(ColladaExporter.Effect effectDefinition
			, TagIndexBase tagIndex
			, Blam.Halo1.Tags.shader_transparent_glass_group shader)
		{
			if (!shader.DiffuseMap.Datum.IsNull)
			{
				var image = new ColladaExporter.Image(tagIndex[shader.DiffuseMap.Datum].Name);
				mImages.Add(image);

				effectDefinition.AddImage(image);
				effectDefinition.AddPhongValue("Diffuse", new ColladaExporter.Phong.Texture(image.ImagePath, 0));
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets up an effect definition from a shader transparent water tag. </summary>
		/// <param name="effectDefinition">	The effect definition to set up. </param>
		/// <param name="tagIndex">		   	The tag index containing the shader tag. </param>
		/// <param name="shader">		   	The shader tag definition. </param>
		///-------------------------------------------------------------------------------------------------
		private void SetupEffectDefinition(ColladaExporter.Effect effectDefinition
			, TagIndexBase tagIndex
			, Blam.Halo1.Tags.shader_transparent_water_group shader)
		{
			if (!shader.BaseMap.Datum.IsNull)
			{
				var image = new ColladaExporter.Image(tagIndex[shader.BaseMap.Datum].Name);
				mImages.Add(image);

				effectDefinition.AddImage(image);
				effectDefinition.AddPhongValue("Diffuse", new ColladaExporter.Phong.Texture(image.ImagePath, 0));

				// Set specular color
				effectDefinition.AddPhongValue("Specular", new TagInterface.RealColor(shader.ViewPerpendicularTintColor, 1));
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets up an effect definition from a shader transparent meter tag. </summary>
		/// <param name="effectDefinition">	The effect definition to set up. </param>
		/// <param name="tagIndex">		   	The tag index containing the shader tag. </param>
		/// <param name="shader">		   	The shader tag definition. </param>
		///-------------------------------------------------------------------------------------------------
		private void SetupEffectDefinition(ColladaExporter.Effect effectDefinition
			, TagIndexBase tagIndex
			, Blam.Halo1.Tags.shader_transparent_meter_group shader)
		{
			if (!shader.Map.Datum.IsNull)
			{
				var image = new ColladaExporter.Image(tagIndex[shader.Map.Datum].Name);
				mImages.Add(image);

				effectDefinition.AddImage(image);
				effectDefinition.AddPhongValue("Diffuse", new ColladaExporter.Phong.Texture(image.ImagePath, 0));
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Sets up an effect definition from a shader transparent plasma tag. </summary>
		/// <param name="effectDefinition">	The effect definition to set up. </param>
		/// <param name="tagIndex">		   	The tag index containing the shader tag. </param>
		/// <param name="shader">		   	The shader tag definition. </param>
		///-------------------------------------------------------------------------------------------------
		private void SetupEffectDefinition(ColladaExporter.Effect effectDefinition
			, TagIndexBase tagIndex
			, Blam.Halo1.Tags.shader_transparent_plasma_group shader)
		{
			effectDefinition.AddPhongValue("Diffuse", new TagInterface.RealColor(shader.ParallelTintColor, 1));
			effectDefinition.AddPhongValue("Specular", new TagInterface.RealColor(shader.PerpendicularTintColor, 1));
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds an effect definitions. </summary>
		/// <exception cref="Exception">	Thrown when an exception error condition occurs. </exception>
		/// <param name="tagIndex">			The tag index containing the shader tags. </param>
		///-------------------------------------------------------------------------------------------------
		protected void AddEffectDefinitions(TagIndexBase tagIndex)
		{
			foreach (var shader in mShaderTypeList)
			{
				/// Create a new collada effect definition
				string shaderName = Path.GetFileNameWithoutExtension(tagIndex[shader.ShaderDatum].Name);

				ColladaExporter.Effect effectDefinition = new ColladaExporter.Effect(shaderName);

				// Set the effects emission and shininess from the base shader group data
				Blam.Halo1.Tags.shader_group shaderTag = tagIndex[shader.ShaderDatum].TagDefinition as Blam.Halo1.Tags.shader_group;
				effectDefinition.AddPhongValue("Emission", new TagInterface.RealColor(shaderTag.RadiosityEmittedLightColor, 1));
				effectDefinition.AddPhongValue("Shininess", 30.0f);

				// Set up the other effects values using the shader specific functions
				if (shaderTag is Blam.Halo1.Tags.shader_environment_group)
				{
					SetupEffectDefinition(effectDefinition, tagIndex, shaderTag as Blam.Halo1.Tags.shader_environment_group);
				}
				else if (shaderTag is Blam.Halo1.Tags.shader_model_group)
				{
					SetupEffectDefinition(effectDefinition, tagIndex, shaderTag as Blam.Halo1.Tags.shader_model_group);
				}
				else if (shaderTag is Blam.Halo1.Tags.shader_transparent_chicago_group)
				{
					SetupEffectDefinition(effectDefinition, tagIndex, shaderTag as Blam.Halo1.Tags.shader_transparent_chicago_group);
				}
				else if (shaderTag is Blam.Halo1.Tags.shader_transparent_chicago_extended_group)
				{
					SetupEffectDefinition(effectDefinition, tagIndex, shaderTag as Blam.Halo1.Tags.shader_transparent_chicago_extended_group);
				}
				else if (shaderTag is Blam.Halo1.Tags.shader_transparent_generic_group)
				{
					SetupEffectDefinition(effectDefinition, tagIndex, shaderTag as Blam.Halo1.Tags.shader_transparent_generic_group);
				}
				else if (shaderTag is Blam.Halo1.Tags.shader_transparent_glass_group)
				{
					SetupEffectDefinition(effectDefinition, tagIndex, shaderTag as Blam.Halo1.Tags.shader_transparent_glass_group);
				}
				else if (shaderTag is Blam.Halo1.Tags.shader_transparent_water_group)
				{
					SetupEffectDefinition(effectDefinition, tagIndex, shaderTag as Blam.Halo1.Tags.shader_transparent_water_group);
				}
				else if (shaderTag is Blam.Halo1.Tags.shader_transparent_meter_group)
				{
					SetupEffectDefinition(effectDefinition, tagIndex, shaderTag as Blam.Halo1.Tags.shader_transparent_meter_group);
				}
				else if (shaderTag is Blam.Halo1.Tags.shader_transparent_plasma_group)
				{
					SetupEffectDefinition(effectDefinition, tagIndex, shaderTag as Blam.Halo1.Tags.shader_transparent_plasma_group);
				}
				else
				{
					throw new Exception("Unsupported shader definition type");
				}

				// Add the effect definition and create a material for it
				mEffects.Add(effectDefinition);
				mMaterials.Add(new ColladaExporter.Material(shaderName, shaderName));
			}
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Implemented by derived classes to get the shaders in tag specific ways. </summary>
		/// <param name="tagManager">	Manager for tag. </param>
		///-------------------------------------------------------------------------------------------------
		protected abstract void AddShaders(TagManager tagManager);
		#endregion

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Collects shader data from a tag. </summary>
		/// <param name="tagIndex">  	The tag index containing the tag. </param>
		/// <param name="tagManager">	Manager for the tag. </param>
		///-------------------------------------------------------------------------------------------------
		public void CollectData(TagIndexBase tagIndex, TagManager tagManager)
		{
			AddShaders(tagManager);
			AddEffectDefinitions(tagIndex);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets shader type reference by searching for the supplied datum index. </summary>
		/// <param name="shaderDatum">	The shader datum. </param>
		/// <returns>	The shader type reference. </returns>
		///-------------------------------------------------------------------------------------------------
		public ShaderType GetShaderTypeFromDatum(DatumIndex shaderDatum)
		{
			return mShaderTypeList.Find(shaderType => shaderType.ShaderDatum == shaderDatum);
		}

		#region IColladaShaderDataProvider Members
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the images list. </summary>
		/// <returns>	The images list. </returns>
		///-------------------------------------------------------------------------------------------------
		public ColladaExporter.ImageList GetImages()
		{
			return mImages;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the effects list. </summary>
		/// <returns>	The effects list. </returns>
		///-------------------------------------------------------------------------------------------------
		public ColladaExporter.EffectList GetEffects()
		{
			return mEffects;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Gets the materials list. </summary>
		/// <returns>	The materials list. </returns>
		///-------------------------------------------------------------------------------------------------
		public ColladaExporter.MaterialList GetMaterials()
		{
			return mMaterials;
		}
		#endregion
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Data collection class for the shaders in a Halo1 structure bsp tag. </summary>
	///-------------------------------------------------------------------------------------------------
	public class StructureBSPShaderData : ShaderData
	{
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds shaders from the structure bsp tag to the shaders list. </summary>
		/// <param name="tagManager">	Manager for the structure bsp tag. </param>
		///-------------------------------------------------------------------------------------------------
		protected override void AddShaders(TagManager tagManager)
		{
			Blam.Halo1.Tags.structure_bsp_group bspTag = tagManager.TagDefinition as Blam.Halo1.Tags.structure_bsp_group;

			// Get the shaders from the lightmap material blocks
			foreach (var lightmap in bspTag.Lightmaps)
			{
				foreach (var material in lightmap.Materials)
				{
					// Only add a shader if it has not yet been added
					if (!mShaderTypeList.Exists(shader => shader.ShaderDatum == material.Shader.Datum))
					{
						mShaderTypeList.Add(new ShaderType(material.Shader.Datum));
					}
				}
			}
		}
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>	Data collection class for the shaders in a Halo1 model tag. </summary>
	///-------------------------------------------------------------------------------------------------
	public class ModelShaderData : ShaderData
	{
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Adds shaders from the model tag to the shaders list. </summary>
		/// <param name="tagManager">	Manager for the model tag. </param>
		///-------------------------------------------------------------------------------------------------
		protected override void AddShaders(TagManager tagManager)
		{
			Blam.Halo1.Tags.gbxmodel_group modelTag = tagManager.TagDefinition as Blam.Halo1.Tags.gbxmodel_group;

			// Add all of the shaders listed in the shaders block
			foreach (var modelShader in modelTag.Shaders)
			{
				// Only add a shader if it has not yet been added
				if (!mShaderTypeList.Exists(shader => shader.ShaderDatum == modelShader.Shader.Datum))
				{
					mShaderTypeList.Add(new ShaderType(modelShader.Shader.Datum));
				}
			}
		}
	}
}