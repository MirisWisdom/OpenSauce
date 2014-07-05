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
					ColladaNCName objectName = "Unknown";

					if (objectInstance.ObjectType.ObjectTagDatum.IsValid)
					{
						objectName = Path.GetFileNameWithoutExtension(mTagIndex[objectInstance.ObjectType.ObjectTagDatum].Name);
					}

					nodeName = i.ToString() + "-" + objectName;
				}
				else
				{
					nodeName = objectInstance.ObjectName.Name;
				}

				Core.ColladaNode node = CreateNode(nodeName, "", "", Enums.ColladaNodeType.NODE);

				// Set the nodes position
				Core.ColladaTranslate translate = new Core.ColladaTranslate();
				translate.SetTranslate(objectInstance.Position, 100);
				node.Add(translate);

				// Set the nodes rotation
				// Have to sue a different transform matrix here, Not sure why...
				node.AddRange(
					ColladaUtilities.CreateRotationSet(
						objectInstance.Rotation.Y, objectInstance.Rotation.P, objectInstance.Rotation.R,
						new LowLevel.Math.real_matrix3x3(
							new LowLevel.Math.real_vector3d(0, 0, 1),
							new LowLevel.Math.real_vector3d(0, 1, 0),
							new LowLevel.Math.real_vector3d(1, 0, 0))
						)
				);
				
				// Add geometry instances to the node
				if (objectInstance.ObjectType.ObjectTagDatum.IsValid)
				{
					// Collect data about the object
					ObjectData objectData = new ObjectData();
					objectData.CollectData(mTagIndex, mTagIndex[objectInstance.ObjectType.ObjectTagDatum]);

					if(objectData.Model.IsValid)
					{
						// Collect data about the model
						ModelData modelData = new ModelData();

						modelData.CollectData(mTagIndex, mTagIndex[objectData.Model]);

						// Get all of the geometries that make up the permutation at the highest lod
						var geometryList = modelData.GetGeometries(objectInstance.Permutation, Blam.Halo1.TypeEnums.LevelOfDetailEnum.SuperHigh);

						// Add geometry instances for all geometries
						geometryList.ForEach(
							geometrySet =>
							{
								// Create shader references for all shaders used by the geometry
								MaterialReferenceList materialReferences = new MaterialReferenceList();
								foreach(var shader in geometrySet.Shaders)
								{
									ColladaID<Fx.ColladaMaterial> shaderName = shader.MaterialName;
									var url = ColladaUtilities.BuildExternalReference(modelData,
										colladaSettings.RootDirectory,
										shaderName);

									materialReferences.Add(new MaterialReference(url, shaderName));
								}

								// Build the geometry reference URL and add the geometry instance
								string geometryURL = ColladaUtilities.BuildExternalReference(modelData,
									colladaSettings.RootDirectory,
									new ColladaID<Core.ColladaGeometry>(geometrySet.Name));

								node.Add(CreateInstanceGeometry(geometryURL, geometrySet.Name, materialReferences));
							}
						);
					}
				}

				listNode.Add(node);
			}
		}

		#region Library Creation
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

			Core.ColladaNode frame = new BlamLib.Render.COLLADA.Core.ColladaNode();				
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
