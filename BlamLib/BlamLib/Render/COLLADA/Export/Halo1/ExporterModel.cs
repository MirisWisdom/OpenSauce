/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using BlamLib.Blam;

using H1 = BlamLib.Blam.Halo1;

namespace BlamLib.Render.COLLADA.Halo1
{
	public class ColladaModelExporter : ColladaModelExporterHalo1
	{
		#region Class Fields
		IHalo1ModelInterface modelInfo;
		#endregion

		#region Constructor
		/// <summary>
		/// Halo1 Model exporter class
		/// </summary>
		/// <param name="model_info">An object implementing IHalo1ModelInterface to provide geometry name and index pairs</param>
		/// <param name="tag_index">The tag index containing the tag being exported</param>
		/// <param name="tag_manager">The tag manager of the tag being exported</param>
		public ColladaModelExporter(ColladaExportArgs arguments, IHalo1ModelInterface model_info, Managers.TagIndexBase tag_index, Managers.TagManager tag_manager)
			: base(arguments, model_info, tag_index, tag_manager)
		{
			modelInfo = model_info;
		}
		#endregion

		#region Element Creation
		#region Create Geometry
		/// <summary>
		/// Creates a vertex index list from a gbxmodel geometry part
		/// </summary>
		/// <param name="part">The part to create an index list from</param>
		/// <param name="index_offset">The amount to offset the indices by</param>
		/// <returns></returns>
		private List<int> CreateIndicesModel(H1.Tags.gbxmodel_group.model_geometry_block.model_geometry_part_block part,
			int index_offset)
		{
			// add the strip indices to an easier to handle index list, ignoring invalid indices
			List<int> index_list = new List<int>();
			foreach(var triangle in part.Triangles)
			{
				index_list.Add(triangle.VertexIndex0 + index_offset);
				if(triangle.VertexIndex1 != -1) index_list.Add(triangle.VertexIndex1 + index_offset);
				if(triangle.VertexIndex2 != -1) index_list.Add(triangle.VertexIndex2 + index_offset);
			}

			// return the index list after converting to a triangle list
			return ConvertTriStripToList(index_list);
		}
		/// <summary>
		/// Creates geometry elements for all of the included geometry blocks
		/// </summary>
		void CreateGeometryList()
		{
			H1.Tags.gbxmodel_group definition = tagManager.TagDefinition as H1.Tags.gbxmodel_group;

			// create a list contining the names of all the shaders being used
			List<string> shader_names = new List<string>();
			foreach (var shader in definition.Shaders)
				shader_names.Add(Path.GetFileNameWithoutExtension(shader.Shader.ToString()));

			for (int i = 0; i < modelInfo.GetGeometryCount(); i++)
			{
				string name = ColladaUtilities.FormatName(modelInfo.GetGeometryName(i), " ", "_");
				
				List<Vertex> common_vertices = new List<Vertex>();

				H1.Tags.gbxmodel_group.model_geometry_block geometry = definition.Geometries[modelInfo.GetGeometryIndex(i)];

				// collect the vertices for all of the geometries parts
				foreach(var part in geometry.Parts)
				{
					foreach(var vertex in part.UncompressedVertices)
					{
						Vertex common_vertex = new Vertex(vertex.Position.ToPoint3D(100),
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

						common_vertices.Add(common_vertex);
					}
				}
				
				List<Part> common_parts = new List<Part>();
				// create a new Part for each geometry part
				int index_offset = 0;
				foreach(var part in geometry.Parts)
				{
					Part common_part = new Part(shader_names[part.ShaderIndex]);
					common_part.AddIndices(CreateIndicesModel(part, index_offset));

					index_offset += part.UncompressedVertices.Count;

					common_parts.Add(common_part);
				}

				// create the geometry element
				CreateGeometry(name, 1,
					VertexComponent.POSITION | VertexComponent.NORMAL | VertexComponent.BINORMAL | VertexComponent.TANGENT | VertexComponent.TEXCOORD,
					common_vertices, common_parts);
			}
		}
		#endregion
		#region Create Controllers
		/// <summary>
		/// Creates controllers to skin each geometry in the collada file
		/// </summary>
		void CreateControllerList()
		{
			H1.Tags.gbxmodel_group definition = tagManager.TagDefinition as H1.Tags.gbxmodel_group;

			// if there are no nodes then no skinning is possible
			if (definition.Nodes.Count == 0)
				return;

			// create a controller for each geometry
			for (int i = 0; i < modelInfo.GetGeometryCount(); i++)
			{
				List<VertexWeight> vertex_weights = new List<VertexWeight>();

				//  create a list of vertex weights from all of the geometry parts
				foreach (var part in definition.Geometries[modelInfo.GetGeometryIndex(i)].Parts)
				{
					foreach (var vertex in part.UncompressedVertices)
					{
						VertexWeight vertex_weight = new VertexWeight();

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
						if(vertex.NodeWeight1 != 1)
							vertex_weight.AddWeight(node2, vertex.NodeWeight2);

						vertex_weights.Add(vertex_weight);
					}
				}

				// create the controller element
				CreateSkinController(listGeometry[i].ID, vertex_weights);
			}
		}
		#endregion
		#region Create Bones
		/// <summary>
		/// Populates the bone list with 1 collada node for each bone
		/// </summary>
		void CreateBoneList()
		{
			H1.Tags.gbxmodel_group definition = tagManager.TagDefinition as H1.Tags.gbxmodel_group;

			// no bones? no bones!
			if (definition.Nodes.Count == 0)
				return;

			List<Bone> bone_list = new List<Bone>();
			// create a list of common bone definitions
			foreach (var node in definition.Nodes)
			{
				bone_list.Add(new Bone(node.Name,
					node.DefaultTranslation.ToPoint3D(100),
					TagInterface.RealQuaternion.Invert(node.DefaultRotation),
					1.0f,
					node.ParentNode,
					node.FirstChildNode,
					node.NextSiblingNode));
			}

			// create the bone node elements
			CreateBones(bone_list, RotationVectorY, RotationVectorP, RotationVectorR);
		}
		#endregion
		#region Create Nodes
		/// <summary>
		/// Creates instances of all the controllers
		/// </summary>
		void CreateNodeList()
		{
			// create a string list of all the shaders being used
			List<string> shader_list = new List<string>();
			for (int shader_index = 0; shader_index < shaderInfo.GetShaderCount(); shader_index++)
				shader_list.Add(ColladaUtilities.FormatName(Path.GetFileNameWithoutExtension(shaderInfo.GetShaderName(shader_index)), " ", "_"));

			// create the controller instances
			for (int i = 0; i < modelInfo.GetGeometryCount(); i++)
				CreateNodeInstanceController(modelInfo.GetGeometryName(i), i, shader_list);
		}
		#endregion
		#region Create Markers
		/// <summary>
		/// Create the markers
		/// </summary>
		void CreateMarkerList()
		{
			H1.Tags.gbxmodel_group definition = tagManager.TagDefinition as H1.Tags.gbxmodel_group;

			List<Marker> marker_list = new List<Marker>();

			// create a list of generic marker definitions
			foreach(var marker in definition.Markers)
			{
				foreach(var instance in marker.Instances)
				{
					// if we are only exporting one permutation and the marker permunation doesnt match, skip it
					if (!modelInfo.GetIsMultiplePerms())
						if (modelInfo.GetPermutation() != instance.PermutationIndex.Value)
							continue;

					// if multiple permutations are being exported, append the marker permutation to its name
					string name = ColladaUtilities.FormatName(marker.Name, " ", "_");
					if(modelInfo.GetIsMultiplePerms())
						name += "-perm" + instance.PermutationIndex.Value.ToString();

					Marker common_marker = new Marker(name, instance.Translation.ToPoint3D(100),
						TagInterface.RealQuaternion.Invert(instance.Rotation), instance.NodeIndex);

					marker_list.Add(common_marker);
				}
			}

			// create the marker node elements
			CreateMarkers(marker_list, RotationVectorY, RotationVectorP, RotationVectorR);
		}
		#endregion
		#endregion

		#region Library Creation
		/// <summary>
		/// Creates the library_visual_scenes element in the collada file
		/// </summary>
		void AddLibraryVisualScenes()
		{
			COLLADAFile.LibraryVisualScenes = new Core.ColladaLibraryVisualScenes();
			COLLADAFile.LibraryVisualScenes.VisualScene = new List<Core.ColladaVisualScene>();
			COLLADAFile.LibraryVisualScenes.VisualScene.Add(new Core.ColladaVisualScene());
			COLLADAFile.LibraryVisualScenes.VisualScene[0].ID = ColladaElement.FormatID<Core.ColladaVisualScene>("main");
			COLLADAFile.LibraryVisualScenes.VisualScene[0].Node = new List<Core.ColladaNode>();

			COLLADAFile.LibraryVisualScenes.VisualScene[0].Node.Add(listBone[0]);
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

			CollectBitmaps();

			CreateImageList();
			CreateEffectList();
			CreateMaterialList();
			CreateBoneList();
			CreateMarkerList();
			CreateGeometryList();
			CreateControllerList();
			CreateNodeList();

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