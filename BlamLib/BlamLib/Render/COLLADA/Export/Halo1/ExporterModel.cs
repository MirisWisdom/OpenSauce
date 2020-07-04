/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using BlamLib.Blam;

using H1 = BlamLib.Blam.Halo1;

namespace BlamLib.Render.COLLADA.Halo1
{
	///-------------------------------------------------------------------------------------------------
	/// <summary>   Interface for halo 1 model data provider. </summary>
	///-------------------------------------------------------------------------------------------------
	public interface IHalo1ModelDataProvider : IColladaDataProvider
	{
		///-------------------------------------------------------------------------------------------------
		/// <summary>   Gets the bone list for the model. </summary>
		/// <returns>   The models bones. </returns>
		///-------------------------------------------------------------------------------------------------
		ModelData.ModelBoneList GetBones();

		///-------------------------------------------------------------------------------------------------
		/// <summary>   Gets a list of all the geometries in the model. </summary>
		/// <returns>   The geometries contained in the model. </returns>
		///-------------------------------------------------------------------------------------------------
		ModelData.ModelGeometrySetList GetGeometries();

		///-------------------------------------------------------------------------------------------------
		/// <summary>   Gets a list of all the geometries in the model for a specific permutation and level of detial. </summary>
		/// <param name="permutation">  The permutation. </param>
		/// <param name="lod">          The LOD. </param>
		/// <returns>   The geometries contained in the model. </returns>
		///-------------------------------------------------------------------------------------------------
		ModelData.ModelGeometrySetList GetGeometries(string permutation, Blam.Halo1.TypeEnums.LevelOfDetailEnum lod);

		///-------------------------------------------------------------------------------------------------
		/// <summary>   Gets the models marker instances. </summary>
		/// <returns>   The the models marker instances. </returns>
		///-------------------------------------------------------------------------------------------------
		ModelData.ModelMarkerInstanceList GetMarkerInstances();
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>   Collada Halo1 GBXModel exporter. </summary>
	///-------------------------------------------------------------------------------------------------
	public class ColladaModelExporter : ColladaExporterHalo1
	{
		#region Constructor
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Halo1 Model exporter class. </summary>
		/// <param name="arguments"> 	Export arguments for the collada interface. </param>
		/// <param name="tagIndex">  	The tag index containing the tag being exported. </param>
		/// <param name="tagManager">	The tag manager of the tag being exported. </param>
		///-------------------------------------------------------------------------------------------------
		public ColladaModelExporter(IColladaSettings settings, Managers.TagIndexBase tagIndex, Managers.TagManager tagManager)
			: base(settings, tagIndex)
		{
			mTagManager = tagManager;
		}
		#endregion

		#region Fields
		private IHalo1ModelDataProvider mModelDataProvider = null;
		private Managers.TagManager mTagManager;
		private string TagName
		{
			get { return Path.GetFileNameWithoutExtension(mTagManager.Name); }
		}
		#endregion Fields

		#region Element Creation
		#region Create Geometry
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates a vertex index list from a gbxmodel geometry part. </summary>
		/// <param name="part">		   	The part to create an index list from. </param>
		/// <param name="index_offset">	The amount to offset the indices by. </param>
		/// <returns>	The new indices model. </returns>
		///-------------------------------------------------------------------------------------------------
		private List<int> CreateIndicesModel(H1.Tags.gbxmodel_group.model_geometry_block.model_geometry_part_block part,
			int index_offset)
		{
			// add the strip indices to an easier to handle index list, ignoring invalid indices
			var index_list = new List<int>();
			foreach(var triangle in part.Triangles)
			{
				index_list.Add(triangle.VertexIndex0 + index_offset);
				if(triangle.VertexIndex1 != -1) index_list.Add(triangle.VertexIndex1 + index_offset);
				if(triangle.VertexIndex2 != -1) index_list.Add(triangle.VertexIndex2 + index_offset);
			}

			// return the index list after converting to a triangle list
			return ConvertTriStripToList(index_list, true);
		}

		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates geometry elements for all of the included geometry blocks. </summary>
		///-------------------------------------------------------------------------------------------------
		void CreateGeometryList()
		{
			var shaderList = new List<string>();
			if (mShaderDataProvider != null)
			{
				// Create a list of every shader used
				foreach (var effect in mShaderDataProvider.GetEffectsMap())
				{
					shaderList.Add(ColladaUtilities.FormatName(Path.GetFileNameWithoutExtension(effect.Name), " ", "_"));
				}
			}

			var definition = mTagManager.TagDefinition as H1.Tags.gbxmodel_group;

			var geometrySetList = mModelDataProvider.GetGeometries();

			foreach(var geometrySet in geometrySetList)
			{
				string name = ColladaUtilities.FormatName(geometrySet.Name, " ", "_");

				var geometryData = new Geometry(name
					, 1
					, Geometry.VertexComponent.POSITION
					| Geometry.VertexComponent.NORMAL
					| Geometry.VertexComponent.BINORMAL
					| Geometry.VertexComponent.TANGENT
					| Geometry.VertexComponent.TEXCOORD);

				// collect the vertices for all of the geometries parts
				foreach (var part in geometrySet.Geometry.Parts)
				{
					foreach(var vertex in part.UncompressedVertices)
					{
						var common_vertex = new Geometry.Vertex(vertex.Position.ToPoint3D(100),
							vertex.Normal.ToVector3D(),
							vertex.Binormal.ToVector3D(),
							vertex.Tangent.ToVector3D());

						// if the texture coordinate scale is 0.0, default to 1.0
						float u_scale = (definition.BaseMapUScale.Value == 0.0f ? 1.0f : definition.BaseMapUScale.Value);
						float v_scale = (definition.BaseMapVScale.Value == 0.0f ? 1.0f : definition.BaseMapVScale.Value);

						// add the texture coordinate data
						common_vertex.AddTexcoord(new LowLevel.Math.real_point2d(
							vertex.TextureCoords.X * u_scale,
							((vertex.TextureCoords.Y * v_scale) * -1) + 1));

						geometryData.AddVertex(common_vertex);
					}
				}
				
				// create a new Part for each geometry part
				int index_offset = 0;
				foreach (var part in geometrySet.Geometry.Parts)
				{
					var shader_index = part.ShaderIndex;
					if(shader_index >= shaderList.Count)
					{
						shader_index.Value = shaderList.Count - 1;
					}

					var common_part = new Geometry.Part(shaderList[shader_index]);
					common_part.AddIndices(CreateIndicesModel(part, index_offset));

					index_offset += part.UncompressedVertices.Count;

					geometryData.AddPart(common_part);
				}

				// create the geometry element
				CreateGeometry(geometryData);
			}
		}
		#endregion
		#region Create Controllers
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates controllers to skin each geometry in the collada file. </summary>
		///-------------------------------------------------------------------------------------------------
		void CreateControllerList()
		{
			var boneList = mModelDataProvider.GetBones();

			// if there are no nodes then no skinning is possible
			if (boneList.Count == 0)
			{
				return;
			}

			// create a controller for each geometry
			var geometrySetList = mModelDataProvider.GetGeometries();
			for (int i = 0; i < geometrySetList.Count; i++)
			{
				var geometrySet = geometrySetList[i];

				var skinData = new Skin(geometrySet.Name, listGeometry[i].ID);

				//  create a list of vertex weights from all of the geometry parts
				foreach (var part in geometrySet.Geometry.Parts)
				{
					foreach (var vertex in part.UncompressedVertices)
					{
						var vertex_weight = new Skin.VertexWeight();

						int node1 = vertex.NodeIndex1;
						int node2 = vertex.NodeIndex2;

						// if the bone index count is 0 then the index references the main node list,
						// otherwise it references a local node map for this part
						if (part.NodeMapCount != 0)
						{
							node1 = part.NodeMap[node1];
							node2 = (node2 != -1 ? part.NodeMap[node2].Value : node2);
						}

						vertex_weight.AddWeight(node1, vertex.NodeWeight1);

						// if the first weight is 1 the vertex is weighted to one bone only so the second weight is not needed
						if (vertex.NodeWeight1 != 1)
						{
							vertex_weight.AddWeight(node2, vertex.NodeWeight2);
						}

						skinData.AddVertexWeight(vertex_weight);
					}
				}

				// create the controller element
				CreateSkinController(skinData);
			}
		}
		#endregion
		#region Create Bones
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Populates the bone list with 1 collada node for each bone. </summary>
		///-------------------------------------------------------------------------------------------------
		void CreateBoneList()
		{
			var boneList = mModelDataProvider.GetBones();

			// no bones? no bones!
			if (boneList.Count == 0)
			{
				return;
			}

			var bone_list = new List<Bone>();
			// create a list of common bone definitions
			foreach (var bone in boneList)
			{
				bone_list.Add(new Bone(bone.Name,
					bone.Position.ToPoint3D(100),
					TagInterface.RealQuaternion.Invert(bone.Rotation),
					1.0f,
					bone.Parent,
					bone.Child,
					bone.Sibling));
			}

			// create the bone node elements
			CreateBones(bone_list
				, new LowLevel.Math.real_vector3d(1, 0, 0)
				, new LowLevel.Math.real_vector3d(0, 0, 1)
				, new LowLevel.Math.real_vector3d(0, 1, 0)
				, ColladaUtilities.ColladaRotationOrder.ZYX
			);
		}
		#endregion
		#region Create Nodes
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates instances of all the controllers. </summary>
		///-------------------------------------------------------------------------------------------------
		void CreateNodeList()
		{
			var materialReferences = new MaterialReferenceList();
			if (mShaderDataProvider != null)
			{
				// create a list of every shader used 
				foreach (var effect in mShaderDataProvider.GetEffects())
				{
					string effectName = ColladaUtilities.FormatName(Path.GetFileNameWithoutExtension(effect.Name), " ", "_");

					materialReferences.Add(new MaterialReference(
						effectName,
						ColladaUtilities.BuildUri(ColladaElement.FormatID<Fx.ColladaMaterial>(effectName)),
						effectName));
				}
			}

			// create the controller instances
			var geometrySetList = mModelDataProvider.GetGeometries();
			for (int i = 0; i < geometrySetList.Count; i++)
			{
				var node = CreateNode(geometrySetList[i].Name, "", geometrySetList[i].Name, Enums.ColladaNodeType.NODE);

				// If there are no bones instance the static geometry, otherwise create a skinned instance
				string name = ColladaUtilities.FormatName(geometrySetList[i].Name, " ", "_");
				if (mModelDataProvider.GetBones().Count == 0)
				{
					node.Add(
						CreateInstanceGeometry(
							ColladaUtilities.BuildUri(ColladaElement.FormatID<Core.ColladaGeometry>(name)),
							name,
							materialReferences
						)
					);
				}
				else
				{
					node.Add(
						CreateInstanceController(
							ColladaUtilities.BuildUri(ColladaElement.FormatID<Core.ColladaController>(name)),
							name,
							materialReferences
						)
					);
				}

				listNode.Add(node);
			}
		}
		#endregion
		#region Create Markers
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Create the markers. </summary>
		///-------------------------------------------------------------------------------------------------
		void CreateMarkerList()
		{
			var markerInstances = mModelDataProvider.GetMarkerInstances();

			var markerList = new List<Marker>();

			// create a list of generic marker definitions
			foreach (var markerInstance in markerInstances)
			{
				// if multiple permutations are being exported, append the marker permutation to its name
				string name = ColladaUtilities.FormatName(markerInstance.MarkerType.Name, " ", "_");

				name += "-perm" + markerInstance.Permutation.ToString();

				var common_marker = new Marker(name,
					markerInstance.Position.ToPoint3D(100),
					TagInterface.RealQuaternion.Invert(markerInstance.Rotation),
					markerInstance.Bone);

				markerList.Add(common_marker);
			}

			// create the marker node elements
			CreateMarkers(markerList
				, new LowLevel.Math.real_vector3d(1, 0, 0)
				, new LowLevel.Math.real_vector3d(0, 0, 1)
				, new LowLevel.Math.real_vector3d(0, 1, 0)
				, ColladaUtilities.ColladaRotationOrder.ZYX
			);
		}
		#endregion
		#endregion

		#region Library Creation
		///-------------------------------------------------------------------------------------------------
		/// <summary>	Creates the library_visual_scenes element in the collada file. </summary>
		///-------------------------------------------------------------------------------------------------
		void AddLibraryVisualScenes()
		{
			COLLADAFile.LibraryVisualScenes = new Core.ColladaLibraryVisualScenes();
			COLLADAFile.LibraryVisualScenes.VisualScene = new List<Core.ColladaVisualScene>();
			COLLADAFile.LibraryVisualScenes.VisualScene.Add(new Core.ColladaVisualScene());
			COLLADAFile.LibraryVisualScenes.VisualScene[0].ID = "main";
			COLLADAFile.LibraryVisualScenes.VisualScene[0].Node = new List<Core.ColladaNode>();

			if (listBone.Count > 0)
			{
				COLLADAFile.LibraryVisualScenes.VisualScene[0].Node.Add(listBone[0]);
			}
			COLLADAFile.LibraryVisualScenes.VisualScene[0].Node.AddRange(listNode);
		}
		#endregion
		
		protected override bool BuildColladaInstanceImpl()
		{
			// reset and repopulate the collada file
			COLLADAFile = new ColladaFile();

			COLLADAFile.Version = "1.4.1";
			AddAsset(
				System.Environment.UserName,
				"OpenSauceIDE:ColladaBuilder",
				"meter", 0.0254, Enums.ColladaUpAxisEnum.Z_UP);

			mModelDataProvider = GetDataProvider<IHalo1ModelDataProvider>();

			CreateBoneList();
			CreateMarkerList();
			CreateGeometryList();
			CreateControllerList();
			CreateNodeList();

			CreateImageList();
			CreateEffectList();
			CreateMaterialList();

			AddLibraryImages();
			AddLibraryEffects();
			AddLibraryMaterials();
			AddLibraryGeometries();
			AddLibraryControllers();
			AddLibraryVisualScenes();
			AddScene("main");

			COLLADAFile.Validate();

			return true;
		}
	};
}