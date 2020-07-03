/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using BlamLib.Render.COLLADA.Halo1;
using BlamLib.TagInterface;
using BlamLib.Render.COLLADA.Core;
using System.IO;
using BlamLib.Managers;

namespace BlamLib.Render.COLLADA.Halo1
{
	///-------------------------------------------------------------------------------------------------
	/// <summary>	Interface for halo 1 scenario data provider. </summary>
	///-------------------------------------------------------------------------------------------------
	public interface IHalo1ScenarioDataProvider : IColladaDataProvider
	{
		bool IncludeScenery { get; set; }
		bool IncludeDeviceMachines { get; set; }
		bool IncludeDeviceControls { get; set; }
		bool IncludeDeviceLightFixtures { get; set; }
		bool IncludeSoundScenery { get; set; }

		List<ScenarioData.ScenarioObjectInstance> GetObjectInstances();
	}

	public class ColladaScenarioExporter : ColladaExporterHalo1
	{
		#region Class Members
		private IHalo1ScenarioDataProvider mScenarioDataProvider = null;
		private Managers.TagManager mTagManager;
		private string TagName
		{
			get { return Path.GetFileNameWithoutExtension(mTagManager.Name); }
		}
		#endregion Class Members

		#region Constructor
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Halo1 Scenario exporter class. </summary>
		/// <param name="settings">  	Export arguments for the collada interface. </param>
		/// <param name="tagIndex">  	The tag index containing the tag being exported. </param>
		/// <param name="tagManager">	The tag manager of the tag being exported. </param>
		///-------------------------------------------------------------------------------------------------
		public ColladaScenarioExporter(IColladaSettings settings
			, Managers.TagIndexBase tagIndex
			, Managers.TagManager tagManager)
			: base(settings, tagIndex)
		{
			mTagManager = tagManager;
		}
		#endregion

		private List<string> GetNodeReferences(ScenarioData.ScenarioObject objectType, string permutation)
		{
			var nodeIds = new List<string>(); 

			// Add geometry instances to the node
			if (!TagIndex.IsValid(objectType.ObjectTagDatum))
			{
				throw new ColladaException("Failed to load the object type tag {0}", objectType.ObjectTagPath);
			}

			var objectName = Path.GetFileNameWithoutExtension(objectType.ObjectTagPath);
			objectName += "-perm" + permutation;

			// Collect data about the object
			var objectData = new ObjectData();
			objectData.CollectData(mTagIndex, mTagIndex[objectType.ObjectTagDatum]);
			if (!TagIndex.IsValid(objectData.Model))
			{
				return nodeIds;
			}

			if(COLLADAFile.LibraryNodes == null)
			{
				AddLibraryNodes();
			}

			// Collect data about the model
			var modelData = new ModelData();
			modelData.CollectData(mTagIndex, mTagIndex[objectData.Model]);

			// Get all of the geometries that make up the permutation at the highest lod
			var geometryList = modelData.GetGeometries(permutation, Blam.Halo1.TypeEnums.LevelOfDetailEnum.SuperHigh);

			// Add geometry instances for all geometries
			foreach(var geometrySet in geometryList)
			{
				var name = objectName + "-" + geometrySet.Name;

				ColladaNCName nodeName = name;
				ColladaID<ColladaNode> nodeId = name;

				nodeIds.Add(nodeId);

				if (COLLADAFile.LibraryNodes.Node.Exists(node => node.ID == nodeId.ToString()))
				{
					break;
				}

				// Create shader references for all shaders used by the geometry
				var materialReferences = new MaterialReferenceList();
				foreach (var shader in geometrySet.Shaders)
				{
					ColladaNCName symbolName = shader.MaterialName;
					ColladaID<Fx.ColladaMaterial> shaderName = shader.MaterialName;

					var url = ColladaUtilities.BuildExternalReference(modelData,
						colladaSettings.RootDirectory,
						shaderName);

					materialReferences.Add(new MaterialReference(shaderName, url, symbolName));
				}

				// Build the geometry reference URL and add the geometry instance
				string geometryURL = ColladaUtilities.BuildExternalReference(modelData,
					colladaSettings.RootDirectory,
					new ColladaID<Core.ColladaGeometry>(geometrySet.Name));

				var nodeType = CreateNode(nodeName, "", nodeId, Enums.ColladaNodeType.NODE);
				nodeType.Add(CreateInstanceGeometry(geometryURL, geometrySet.Name, materialReferences));

				COLLADAFile.LibraryNodes.Node.Add(nodeType);
			}

			return nodeIds;
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates the node list. </summary>
		///-------------------------------------------------------------------------------------------------
		private void CreateNodeList()
		{
			// Get all of the object instances to include
			var objectInstances = mScenarioDataProvider.GetObjectInstances();

			for (int i = 0; i < objectInstances.Count; i++)
			{
				var objectInstance = objectInstances[i];

				// Create a node for the object instance
				ColladaNCName nodeName = "";
				if (objectInstance.ObjectName == null)
				{
					if (!TagIndex.IsValid(objectInstance.ObjectType.ObjectTagDatum))
					{
						throw new ColladaException("Failed to load the object type tag {0}", objectInstance.ObjectType.ObjectTagPath);
					}

					ColladaNCName objectName = Path.GetFileNameWithoutExtension(mTagIndex[objectInstance.ObjectType.ObjectTagDatum].Name);

					nodeName = i.ToString() + "-" + objectName;
				}
				else
				{
					nodeName = objectInstance.ObjectName.Name;
				}

				var node = CreateNode(nodeName, "", "", Enums.ColladaNodeType.NODE);

				// Set the nodes position
				var translate = new Core.ColladaTranslate();
				translate.SetTranslate(objectInstance.Position, 100);
				node.Add(translate);

				// Set the nodes rotation
				node.AddRange(
					ColladaUtilities.CreateRotationSet(objectInstance.Rotation.R, objectInstance.Rotation.P, objectInstance.Rotation.Y
						, new LowLevel.Math.real_vector3d(1, 0, 0)
						, new LowLevel.Math.real_vector3d(0, -1, 0)
						, new LowLevel.Math.real_vector3d(0, 0, 1)
						, ColladaUtilities.ColladaRotationOrder.XYZ)
				);

				var nodeIdList = GetNodeReferences(objectInstance.ObjectType, objectInstance.Permutation.ToString("D2"));
				if (nodeIdList.Count > 0)
				{
					node.InstanceNode = new List<ColladaInstanceNode>();
					foreach (var nodeId in nodeIdList)
					{
						node.InstanceNode.Add(new ColladaInstanceNode() { URL = "#" + nodeId });
					}
				}

				listNode.Add(node);
			}
		}

		#region Library Creation
		void AddLibraryNodes()
		{
			COLLADAFile.LibraryNodes = new ColladaLibraryNodes();
			COLLADAFile.LibraryNodes.Node = new List<ColladaNode>();
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Creates the library_visual_scenes element in the collada file. The node list is added
		/// 	under a node named "frame" since that is required when creating new BSPs.
		/// </summary>
		///-------------------------------------------------------------------------------------------------
		void AddLibraryVisualScenes()
		{
			// Add the main scene node
			COLLADAFile.LibraryVisualScenes = new Core.ColladaLibraryVisualScenes();
			COLLADAFile.LibraryVisualScenes.VisualScene = new List<Core.ColladaVisualScene>();
			COLLADAFile.LibraryVisualScenes.VisualScene.Add(new Core.ColladaVisualScene());
			COLLADAFile.LibraryVisualScenes.VisualScene[0].ID = "main";
			COLLADAFile.LibraryVisualScenes.VisualScene[0].Node = new List<Core.ColladaNode>();

			var frame = new BlamLib.Render.COLLADA.Core.ColladaNode();
			frame.Name = "frame_objects";
			frame.AddRange(listNode);

			COLLADAFile.LibraryVisualScenes.VisualScene[0].Node.Add(frame);
		}
		#endregion

		///-------------------------------------------------------------------------------------------------
		/// <summary>
		/// 	Performs the actual collada file population.
		/// </summary>
		/// <returns>	True if no errors occurred. </returns>
		///-------------------------------------------------------------------------------------------------
		protected override bool BuildColladaInstanceImpl()
		{
			COLLADAFile = new ColladaFile();

			COLLADAFile.Version = "1.4.1";
			AddAsset(
				System.Environment.UserName,
				"OpenSauceIDE:ColladaBuilder",
				"meter", 0.0254, Enums.ColladaUpAxisEnum.Z_UP);

			mScenarioDataProvider = GetDataProvider<IHalo1ScenarioDataProvider>();

			CreateNodeList();
			AddLibraryVisualScenes();
			AddScene("main");

			return true;
		}
	}
}
