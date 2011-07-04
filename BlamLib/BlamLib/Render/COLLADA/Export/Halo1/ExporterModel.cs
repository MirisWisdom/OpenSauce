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
using BlamLib.Blam;

using H1 = BlamLib.Blam.Halo1;

namespace BlamLib.Render.COLLADA.Halo1
{
	public class ColladaModelExporter : ColladaModelExporterHalo1
	{
		#region Class Fields
		IHalo1ModelInterface modelInfo;

		List<Core.ColladaGeometry> listGeometry = new List<Core.ColladaGeometry>();
		List<Core.ColladaController> listController = new List<Core.ColladaController>();
		List<Core.ColladaNode> listBone = new List<Core.ColladaNode>();
		List<ColladaBoneMatrix> listBoneMatrix = new List<ColladaBoneMatrix>();
		List<Core.ColladaNode> listNode = new List<Core.ColladaNode>();
		List<Core.ColladaNode> listMarker = new List<Core.ColladaNode>();
		#endregion

		#region Constructor
		/// <summary>
		/// Halo1 Model exporter class
		/// </summary>
		/// <param name="model_info">An object implementing IHalo1ModelInterface to provide geometry name and index pairs</param>
		/// <param name="tag_index">The tag index containing the tag being exported</param>
		/// <param name="tag_manager">The tag manager of the tag being exported</param>
		public ColladaModelExporter(IHalo1ModelInterface model_info, Managers.TagIndexBase tag_index, Managers.TagManager tag_manager)
			: base(model_info, tag_index, tag_manager)
		{
			modelInfo = model_info;
		}
		#endregion

		#region Element Creation
		#region Create Geometry
		/// <summary>
		/// Creates a geometry element from a model geometry block
		/// </summary>
		/// <param name="index">The geometry block index</param>
		/// <param name="name">The name for the resulting geometry element</param>
		/// <returns></returns>
		Core.ColladaGeometry CreateGeometry(int index, string name)
		{
			H1.Tags.gbxmodel_group model_tag = tagManager.TagDefinition as H1.Tags.gbxmodel_group;

			Core.ColladaGeometry geometry = new Core.ColladaGeometry();
			// set the geometry attributes
			geometry.Name = ColladaUtilities.FormatName(name, " ", "_");
			geometry.ID = String.Format(Core.ColladaGeometry.ElementIDFormat, geometry.Name);

			geometry.Mesh = new Core.ColladaMesh();
			// create the source elements for the vertex data
			geometry.Mesh.Source = new List<Core.ColladaSource>();
			geometry.Mesh.Source.Add(CreateSource(Enums.ColladaInputSharedSemantic.POSITION, geometry.ID, 0));
			geometry.Mesh.Source.Add(CreateSource(Enums.ColladaInputSharedSemantic.NORMAL, geometry.ID, 0));
			geometry.Mesh.Source.Add(CreateSource(Enums.ColladaInputSharedSemantic.BINORMAL, geometry.ID, 0));
			geometry.Mesh.Source.Add(CreateSource(Enums.ColladaInputSharedSemantic.TANGENT, geometry.ID, 0));
			geometry.Mesh.Source.Add(CreateSource(Enums.ColladaInputSharedSemantic.TEXCOORD, geometry.ID, 0));

			// create the vertices element
			geometry.Mesh.Vertices = CreateVertices(geometry.ID, geometry.Mesh.Source[0].ID);

			geometry.Mesh.Triangles = new List<Core.ColladaTriangles>();
			// populate the source elements with the geometry data
			int index_offset = 0;
			for (int part_index = 0; part_index < model_tag.Geometries[index].Parts.Count; part_index++)
			{
				geometry.Mesh.Triangles.Add(new Core.ColladaTriangles());

				string shader_name = ColladaUtilities.FormatName(System.IO.Path.GetFileNameWithoutExtension(
					model_tag.Shaders[model_tag.Geometries[index].Parts[part_index].ShaderIndex].Shader.ToString())
					, " ", "_");

				geometry.Mesh.Triangles[part_index].Material = shader_name;
				geometry.Mesh.Triangles[part_index].Count = (uint)model_tag.Geometries[index].Parts[part_index].Triangles.Count;
				geometry.Mesh.Triangles[part_index].Input = new List<Core.ColladaInputShared>();

				Core.ColladaInputShared input;
				// set the vertex input type and reference
				input = new Core.ColladaInputShared();
				input.Semantic = Enums.ColladaInputSharedSemantic.VERTEX;
				input.Source = ColladaUtilities.BuildUri(geometry.Mesh.Vertices.ID);
				input.Offset = 0;
				geometry.Mesh.Triangles[part_index].Input.Add(input);

				// set the normals input type and reference
				input = new Core.ColladaInputShared();
				input.Semantic = Enums.ColladaInputSharedSemantic.NORMAL;
				input.Source = ColladaUtilities.BuildUri(geometry.Mesh.Source[1].ID);
				input.Offset = 1;
				geometry.Mesh.Triangles[part_index].Input.Add(input);

				// set the binormals input type and reference
				input = new Core.ColladaInputShared();
				input.Semantic = Enums.ColladaInputSharedSemantic.BINORMAL;
				input.Source = ColladaUtilities.BuildUri(geometry.Mesh.Source[2].ID);
				input.Offset = 1;
				geometry.Mesh.Triangles[part_index].Input.Add(input);

				// set the tengent input type and reference
				input = new Core.ColladaInputShared();
				input.Semantic = Enums.ColladaInputSharedSemantic.TANGENT;
				input.Source = ColladaUtilities.BuildUri(geometry.Mesh.Source[3].ID);
				input.Offset = 1;
				geometry.Mesh.Triangles[part_index].Input.Add(input);

				// set the texture coordinate input type and reference
				input = new Core.ColladaInputShared();
				input.Semantic = Enums.ColladaInputSharedSemantic.TEXCOORD;
				input.Source = ColladaUtilities.BuildUri(geometry.Mesh.Source[4].ID);
				input.Offset = 2;
				input.Set = 0;
				geometry.Mesh.Triangles[part_index].Input.Add(input);

				// populate the float arrays with the vertex data
				for (int vertex_index = 0; vertex_index < model_tag.Geometries[index].Parts[part_index].UncompressedVertices.Count; vertex_index++)
				{
					// add the position data
					geometry.Mesh.Source[0].FloatArray.Add(
						model_tag.Geometries[index].Parts[part_index].UncompressedVertices[vertex_index].Position.X * 100);
					geometry.Mesh.Source[0].FloatArray.Add(
						model_tag.Geometries[index].Parts[part_index].UncompressedVertices[vertex_index].Position.Y * 100);
					geometry.Mesh.Source[0].FloatArray.Add(
						model_tag.Geometries[index].Parts[part_index].UncompressedVertices[vertex_index].Position.Z * 100);

					// add the normals data
					geometry.Mesh.Source[1].FloatArray.Add(
						model_tag.Geometries[index].Parts[part_index].UncompressedVertices[vertex_index].Normal.I);
					geometry.Mesh.Source[1].FloatArray.Add(
						model_tag.Geometries[index].Parts[part_index].UncompressedVertices[vertex_index].Normal.J);
					geometry.Mesh.Source[1].FloatArray.Add(
						model_tag.Geometries[index].Parts[part_index].UncompressedVertices[vertex_index].Normal.K);

					// add the binormals data
					geometry.Mesh.Source[2].FloatArray.Add(
						model_tag.Geometries[index].Parts[part_index].UncompressedVertices[vertex_index].Binormal.I);
					geometry.Mesh.Source[2].FloatArray.Add(
						model_tag.Geometries[index].Parts[part_index].UncompressedVertices[vertex_index].Binormal.J);
					geometry.Mesh.Source[2].FloatArray.Add(
						model_tag.Geometries[index].Parts[part_index].UncompressedVertices[vertex_index].Binormal.K);

					// add the tangent data
					geometry.Mesh.Source[3].FloatArray.Add(
						model_tag.Geometries[index].Parts[part_index].UncompressedVertices[vertex_index].Tangent.I);
					geometry.Mesh.Source[3].FloatArray.Add(
						model_tag.Geometries[index].Parts[part_index].UncompressedVertices[vertex_index].Tangent.J);
					geometry.Mesh.Source[3].FloatArray.Add(
						model_tag.Geometries[index].Parts[part_index].UncompressedVertices[vertex_index].Tangent.K);


					float u_coord = model_tag.Geometries[index].Parts[part_index].UncompressedVertices[vertex_index].TextureCoords.X;
					float v_coord = model_tag.Geometries[index].Parts[part_index].UncompressedVertices[vertex_index].TextureCoords.Y;
					// if the texture coordinate scale is 0.0, default to 1.0
					float u_scale = (model_tag.BaseMapUScale.Value == 0.0f ? 1.0f : model_tag.BaseMapUScale.Value);
					float v_scale = (model_tag.BaseMapVScale.Value == 0.0f ? 1.0f : model_tag.BaseMapVScale.Value);

					// add the texture coordinate data
					geometry.Mesh.Source[4].FloatArray.Add(
						(u_coord * u_scale));
					geometry.Mesh.Source[4].FloatArray.Add(
						(1.0f - (v_coord * v_scale)) + 1.0f);
				};

				geometry.Mesh.Triangles[part_index].P = new ColladaValueArray<int>();

				// create an array of all the triangle indices
				List<int> index_list = new List<int>();
				foreach (var triangle in model_tag.Geometries[index].Parts[part_index].Triangles)
				{
					index_list.Add(triangle.VertexIndex0);
					if (triangle.VertexIndex1 != -1)
						index_list.Add(triangle.VertexIndex1);
					if (triangle.VertexIndex2 != -1)
						index_list.Add(triangle.VertexIndex2);
				}

				// create the triangles
				for (int i = 0; i < index_list.Count - 2; i++)
				{
					int index0;
					int index1;
					int index2;

					index0 = index_list[i];
					index1 = index_list[i + 1];
					index2 = index_list[i + 2];

					// triangle strips contain degenerate triangles, we don't want these
					if (index2 == index1 || index1 == index0 || index0 == index2)
						continue;

					// each new triangle flips it's ordering, so it has to be swapped on each index so that it faces
					// the right way
					bool swap = (i % 2 != 0);

					// add the final indices
					if (swap)
					{
						int temp = index2;
						index2 = index0;
						index0 = temp;
					}

					int k = 0;
					// add the final indices
					for (k = 0; k < 3; k++)
						geometry.Mesh.Triangles[part_index].P.Add(index0 + index_offset);
					for (k = 0; k < 3; k++)
						geometry.Mesh.Triangles[part_index].P.Add(index1 + index_offset);
					for (k = 0; k < 3; k++)
						geometry.Mesh.Triangles[part_index].P.Add(index2 + index_offset);
				}
				// to all all parts to a single geometry the indices have to be offset
				index_offset += model_tag.Geometries[index].Parts[part_index].UncompressedVertices.Count;
			}

			// set the final accessor value counts
			geometry.Mesh.Source[0].TechniqueCommon.Accessor.SetCount((uint)geometry.Mesh.Source[0].FloatArray.Count);
			geometry.Mesh.Source[1].TechniqueCommon.Accessor.SetCount((uint)geometry.Mesh.Source[1].FloatArray.Count);
			geometry.Mesh.Source[2].TechniqueCommon.Accessor.SetCount((uint)geometry.Mesh.Source[2].FloatArray.Count);
			geometry.Mesh.Source[3].TechniqueCommon.Accessor.SetCount((uint)geometry.Mesh.Source[3].FloatArray.Count);
			geometry.Mesh.Source[4].TechniqueCommon.Accessor.SetCount((uint)geometry.Mesh.Source[4].FloatArray.Count);

			return geometry;
		}
		/// <summary>
		/// Creates geometry elements for all of the included geometry blocks
		/// </summary>
		void CreateGeometryList()
		{
			for(int i = 0; i < modelInfo.GetGeometryCount(); i++)
				listGeometry.Add(CreateGeometry(modelInfo.GetGeometryIndex(i), modelInfo.GetGeometryName(i)));
		}
		#endregion
		#region Create Controllers
		/// <summary>
		/// Creates a controller for skinning one geometry object
		/// </summary>
		/// <param name="geometry_index">The geometry part the controller will control</param>
		/// <param name="geometry_id">String ID of the geometry being skinned</param>
		/// <returns></return>
		Core.ColladaController CreateSkinController(int geometry_index, string geometry_id)
		{
			H1.Tags.gbxmodel_group model_tag = tagManager.TagDefinition as H1.Tags.gbxmodel_group;

			// create the controller node
			Core.ColladaController controller = new BlamLib.Render.COLLADA.Core.ColladaController();

			// set the controllers name and id
			controller.Name = geometry_id + "-skin";
			controller.ID = String.Format(Core.ColladaController.ElementIDFormat, controller.Name);

			// create a skin element for the controller and set it to reference the correct geometry
			controller.Skin = new Core.ColladaSkin();
			controller.Skin.SourceAttrib = ColladaUtilities.BuildUri(geometry_id);

			// create the bind shape matrix
			controller.Skin.BindShapeMatrix = new ColladaValueArray<float>();
			controller.Skin.BindShapeMatrix.Add(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1);

			// create a source element containing the names of all of the bones
			controller.Skin.Source = new List<Core.ColladaSource>();
			controller.Skin.Source.Add(new Core.ColladaSource());
			controller.Skin.Source[0].ID = controller.ID + "-joints";
			controller.Skin.Source[0].NameArray = new Core.ColladaNameArray();
			controller.Skin.Source[0].NameArray.ID = controller.Skin.Source[0].ID + "-array";

			foreach (H1.Tags.model_group.model_node_block node in model_tag.Nodes)
				controller.Skin.Source[0].NameArray.Add(ColladaUtilities.FormatName(node.Name, " ", "_"));

			// create a technique that describes what the joint name array is for
			controller.Skin.Source[0].TechniqueCommon = CreateControllerTechniqueCommon(
				controller.Skin.Source[0].NameArray.ID, controller.Skin.Source[0].NameArray.Count,
				"JOINT", "Name", 1);

			// create a float array source for the bind pose matrices
			controller.Skin.Source.Add(new Core.ColladaSource());
			controller.Skin.Source[1].ID = controller.ID + "-bindposes";
			controller.Skin.Source[1].FloatArray = new Core.ColladaFloatArray();
			controller.Skin.Source[1].FloatArray.ID = controller.Skin.Source[1].ID + "-array";

			foreach (var bone_tree_node in listBoneMatrix)
			{
				// a bind pose matrix is the interse of the world transform of the bone
				SlimDX.Matrix world_matrix = SlimDX.Matrix.Invert(bone_tree_node.TransformMatrixWorld);

				controller.Skin.Source[1].FloatArray.Add(
					world_matrix.M11, world_matrix.M21, world_matrix.M31, world_matrix.M41,
					world_matrix.M12, world_matrix.M22, world_matrix.M32, world_matrix.M42,
					world_matrix.M13, world_matrix.M23, world_matrix.M33, world_matrix.M43,
					world_matrix.M14, world_matrix.M24, world_matrix.M34, world_matrix.M44);
			}

			// create a technique that describes what the bind pose array is for
			controller.Skin.Source[1].TechniqueCommon = CreateControllerTechniqueCommon(
				controller.Skin.Source[1].FloatArray.ID, controller.Skin.Source[1].FloatArray.Count,
				"TRANSFORM", "float4x4", 16);

			// create a source element for the vertex weights
			controller.Skin.Source.Add(new Core.ColladaSource());
			controller.Skin.Source[2].ID = controller.ID + "-weights";
			controller.Skin.Source[2].FloatArray = new Core.ColladaFloatArray();
			controller.Skin.Source[2].FloatArray.ID = controller.Skin.Source[2].ID + "-array";

			// create a technique that describes what the weights array is for
			controller.Skin.Source[2].TechniqueCommon = CreateControllerTechniqueCommon(
				controller.Skin.Source[2].FloatArray.ID, 0,
				"WEIGHT", "float", 1);

			// create a joints element that references the joints and bind matrices
			controller.Skin.Joints = new Core.ColladaJoints();
			controller.Skin.Joints.Input = new List<Core.ColladaInputUnshared>();
			controller.Skin.Joints.Input.Add(new Core.ColladaInputUnshared());
			controller.Skin.Joints.Input.Add(new Core.ColladaInputUnshared());
			controller.Skin.Joints.Input[0].Semantic = Enums.ColladaInputSharedSemantic.JOINT;
			controller.Skin.Joints.Input[1].Semantic = Enums.ColladaInputSharedSemantic.INV_BIND_MATRIX;
			controller.Skin.Joints.Input[0].Source = ColladaUtilities.BuildUri(controller.Skin.Source[0].ID);
			controller.Skin.Joints.Input[1].Source = ColladaUtilities.BuildUri(controller.Skin.Source[1].ID);

			// create a vertex weights element that references the joints and weights
			controller.Skin.VertexWeights = new Core.ColladaVertexWeights();
			controller.Skin.VertexWeights.Input = new List<Core.ColladaInputShared>();
			controller.Skin.VertexWeights.Input.Add(new Core.ColladaInputShared());
			controller.Skin.VertexWeights.Input.Add(new Core.ColladaInputShared());
			controller.Skin.VertexWeights.Input[0].Semantic = Enums.ColladaInputSharedSemantic.JOINT;
			controller.Skin.VertexWeights.Input[1].Semantic = Enums.ColladaInputSharedSemantic.WEIGHT;
			controller.Skin.VertexWeights.Input[0].Source = ColladaUtilities.BuildUri(controller.Skin.Source[0].ID);
			controller.Skin.VertexWeights.Input[1].Source = ColladaUtilities.BuildUri(controller.Skin.Source[2].ID);
			controller.Skin.VertexWeights.Input[0].Offset = 0;
			controller.Skin.VertexWeights.Input[1].Offset = 1;

			controller.Skin.VertexWeights.VCount = new ColladaValueArray<int>();
			controller.Skin.VertexWeights.V = new ColladaValueArray<int>();

			// all the parts in a specified geometry are skinned with one controller since they are all part of one geometry
			// loop through all of the geometries parts
			for (int part_index = 0; part_index < model_tag.Geometries[geometry_index].Parts.Count; part_index++)
			{
				// loop through each vertex in the part
				for (int vertex_index = 0; vertex_index < model_tag.Geometries[geometry_index].Parts[part_index].UncompressedVertices.Count; vertex_index++)
				{
					// get the node indices and weights
					int node0_index = model_tag.Geometries[geometry_index].Parts[part_index].UncompressedVertices[vertex_index].NodeIndex1;
					int node1_index = model_tag.Geometries[geometry_index].Parts[part_index].UncompressedVertices[vertex_index].NodeIndex2;
					float node0_weight = model_tag.Geometries[geometry_index].Parts[part_index].UncompressedVertices[vertex_index].NodeWeight1;
					float node1_weight = model_tag.Geometries[geometry_index].Parts[part_index].UncompressedVertices[vertex_index].NodeWeight2;

					// the vertex will be fully weighted to one bone if node0_index is -1 or is equal to node1_index
					bool single_bone = (node1_index == -1) || (node0_index == node1_index);

					// if the parts bone count is zero, then the node indices reference the full bone list
					// otherwise it references a list of bone indices within the part block

					// if the parts bone count is zero, replace the node indices with those in the parts bone index list
					if (model_tag.Geometries[geometry_index].Parts[part_index].BoneIndexCount != 0)
					{
						node0_index = model_tag.Geometries[geometry_index].Parts[part_index].BoneIndexArray.Data[node0_index];

						if (!single_bone)
							node1_index = model_tag.Geometries[geometry_index].Parts[part_index].BoneIndexArray.Data[node1_index];
					}

					// add the weights and indices to the appropriate arrays
					if (single_bone)
					{
						controller.Skin.VertexWeights.VCount.Add(1);
						controller.Skin.Source[2].FloatArray.Add(node0_weight);

						controller.Skin.VertexWeights.V.Add(
							node0_index, controller.Skin.Source[2].FloatArray.Values.Count - 1);
					}
					else
					{
						controller.Skin.VertexWeights.VCount.Add(2);
						controller.Skin.Source[2].FloatArray.Add(node0_weight, node1_weight);

						controller.Skin.VertexWeights.V.Add(
							node0_index, controller.Skin.Source[2].FloatArray.Values.Count - 2,
							node1_index, controller.Skin.Source[2].FloatArray.Values.Count - 1);
					}
				}
			}
			controller.Skin.Source[2].TechniqueCommon.Accessor.Count = controller.Skin.Source[2].FloatArray.Count;

			return controller;
		}
		/// <summary>
		/// Adds controllers to skin each geometry in the collada file
		/// </summary>
		void CreateControllerList()
		{
			H1.Tags.gbxmodel_group model_tag = tagManager.TagDefinition as H1.Tags.gbxmodel_group;

			if (model_tag.Nodes.Count == 0)
				return;

			for (int i = 0; i < modelInfo.GetGeometryCount(); i++)
				listController.Add(CreateSkinController(modelInfo.GetGeometryIndex(i), listGeometry[i].ID));
		}
		#endregion
		#region Create Bones
		/// <summary>
		/// Creates a bone tree list that contains the matrices necessary for skinning
		/// </summary>
		void CreateBoneMatrixList()
		{
			H1.Tags.gbxmodel_group model_tag = tagManager.TagDefinition as H1.Tags.gbxmodel_group;
			// add nodes for all of the bones in order
			int i;
			for (i = 0; i < model_tag.Nodes.Count; i++)
			{
				listBoneMatrix.Add(new ColladaBoneMatrix(BlamVersion.Halo1,
					model_tag.Nodes[i].DefaultTranslation,
					100.0f,
					model_tag.Nodes[i].DefaultRotation,
					1.0f));
			}
			// now that all the nodes have been created, set the parent node value, and calculate the matrices
			for (i = 0; i < model_tag.Nodes.Count; i++)
			{
				if(model_tag.Nodes[i].ParentNode.Value != -1)
					listBoneMatrix[i].ParentNode = listBoneMatrix[model_tag.Nodes[i].ParentNode.Value];

				listBoneMatrix[i].CreateMatrices();
			}
		}
		/// <summary>
		/// Creates a list of nodes representing the child bones of a single bone
		/// </summary>
		/// <param name="parent_node_index">The parent node to get the children for</param>
		/// <returns></returns>
		List<Core.ColladaNode> GetChildBones(int parent_node_index)
		{
			H1.Tags.gbxmodel_group model_tag = tagManager.TagDefinition as H1.Tags.gbxmodel_group;

			// bone has no children, return
			if (model_tag.Nodes[parent_node_index].FirstChildNode.Value == -1)
				return null;

			List<Core.ColladaNode> children = new List<Core.ColladaNode>();

			// cycle through the first child and its siblings
			int bone_index = model_tag.Nodes[parent_node_index].FirstChildNode.Value;
			while (bone_index != -1)
			{
				children.Add(listBone[bone_index]);

				bone_index = model_tag.Nodes[bone_index].NextSiblingNode.Value;
			}
			return children;
		}
		/// <summary>
		/// Creates a collada node for a single bone
		/// </summary>
		/// <param name="bone_index">The index of the bone in the tag</param>
		/// <returns></returns>
		Core.ColladaNode CreateBone(int bone_index)
		{
			H1.Tags.gbxmodel_group model_tag = tagManager.TagDefinition as H1.Tags.gbxmodel_group;

			// create a node and set its attributes
			Core.ColladaNode bone = new Core.ColladaNode();
			bone.Name = bone.sID = ColladaUtilities.FormatName(model_tag.Nodes[bone_index].Name, " ", "_");
			bone.ID = String.Format(Core.ColladaNode.ElementIDFormat, bone.sID);
			bone.Type = Enums.ColladaNodeType.JOINT;

			// set the nodes position
			Core.ColladaTranslate translation = new Core.ColladaTranslate(model_tag.Nodes[bone_index].DefaultTranslation, 100);
			translation.sID = "translation";
			bone.Add(translation);

			// set the nodes rotation
			bone.AddRange(ColladaUtilities.CreateRotationSet(model_tag.Nodes[bone_index].DefaultRotation.ToEuler3D(BlamVersion.Halo1), 
				RotationVectorY, RotationVectorP, RotationVectorR));

			return bone;
		}
		/// <summary>
		/// Populates the bone list with 1 collada node for each bone
		/// </summary>
		void CreateBoneList()
		{
			H1.Tags.gbxmodel_group model_tag = tagManager.TagDefinition as H1.Tags.gbxmodel_group;

			if (model_tag.Nodes.Count == 0)
				return;

			// create the bones
			for (int i = 0; i < model_tag.Nodes.Count; i++)
				listBone.Add(CreateBone(i));

			// link the bones together
			for (int i = 0; i < model_tag.Nodes.Count; i++)
				listBone[i].AddRange(GetChildBones(i));

			CreateBoneMatrixList();
		}
		#endregion
		#region Create Models
		/// <summary>
		/// Creates a node element for a single geometry
		/// </summary>
		/// <param name="geometry_index">Index of the geometry to create a node for</param>
		/// <returns></returns>
		Core.ColladaNode CreateNode(int geometry_index)
		{
			H1.Tags.gbxmodel_group model_tag = tagManager.TagDefinition as H1.Tags.gbxmodel_group;

			// create the node and set its attributes
			Core.ColladaNode model_node = new Core.ColladaNode();
			model_node.Name = modelInfo.GetGeometryName(geometry_index);
			model_node.Name = ColladaUtilities.FormatName(model_node.Name, " ", "_");
			model_node.ID = String.Format(Core.ColladaNode.ElementIDFormat, model_node.Name);
			model_node.Type = Enums.ColladaNodeType.NODE;

			// create a new controller instance and set its attributes
			Core.ColladaInstanceController instance_controller = new Core.ColladaInstanceController();
			instance_controller.URL = ColladaUtilities.BuildUri(listController[geometry_index].ID);
			instance_controller.Skeleton = new List<Core.ColladaSkeleton>();
			instance_controller.Skeleton.Add(new Core.ColladaSkeleton());
			instance_controller.Skeleton[0].Value = ColladaUtilities.BuildUri(listBone[0].ID);

			// bind materials to the geometry
			instance_controller.BindMaterial = new List<Fx.ColladaBindMaterial>();
			instance_controller.BindMaterial.Add(new Fx.ColladaBindMaterial());
			instance_controller.BindMaterial[0].TechniqueCommon = new Core.ColladaTechniqueCommon();

			// get a list of the shaders used by the geometry
			List<string> shader_list = new List<string>();
			foreach (var part in model_tag.Geometries[modelInfo.GetGeometryIndex(geometry_index)].Parts)
			{
				// if the datum index is bad ignore the shader
				if (!IsDatumValid(model_tag.Shaders[part.ShaderIndex.Value].Shader.Datum))
					continue;

				// get the tag manager of the shader
				BlamLib.Managers.TagManager shader_tag = tagIndex[model_tag.Shaders[part.ShaderIndex.Value].Shader.Datum];

				// format the shaders name and add it to the shader list
				string shader_name = ColladaUtilities.FormatName(System.IO.Path.GetFileNameWithoutExtension(shader_tag.Name), " ", "_");
				if (!shader_list.Contains(shader_name))
					shader_list.Add(shader_name);
			}

			instance_controller.BindMaterial[0].TechniqueCommon.InstanceMaterial = new List<Fx.ColladaInstanceMaterial>();
			for (int shader_index = 0; shader_index < shader_list.Count; shader_index++)
			{
				// create a new material instance referencing the required material
				Fx.ColladaInstanceMaterial instance_material = new Fx.ColladaInstanceMaterial();
				instance_material.Symbol = shader_list[shader_index];
				instance_material.Target = ColladaUtilities.BuildUri(String.Format(Fx.ColladaMaterial.ElementIDFormat, shader_list[shader_index]));

				// add the material instance to the list
				instance_controller.BindMaterial[0].TechniqueCommon.InstanceMaterial.Add(instance_material);
			}
			model_node.Add(instance_controller);

			return model_node;
		}
		/// <summary>
		/// Populates the node list
		/// </summary>
		void CreateNodeList()
		{
			for (int i = 0; i < modelInfo.GetGeometryCount(); i++)
				listNode.Add(CreateNode(i));
		}
		#endregion
		#region Create Markers
		/// <summary>
		/// Create the markers
		/// </summary>
		void CreateMarkerList()
		{
			H1.Tags.gbxmodel_group model_tag = tagManager.TagDefinition as H1.Tags.gbxmodel_group;

			// add instances to the bones
			for (int i = 0; i < model_tag.Markers.Count; i++)
			{
				for (int j = 0; j < model_tag.Markers[i].Instances.Count; j++)
				{
					if (!modelInfo.GetIsMultiplePerms())
						if (modelInfo.GetPermutation() != model_tag.Markers[i].Instances[j].PermutationIndex.Value)
							continue;

					Core.ColladaNode marker = new BlamLib.Render.COLLADA.Core.ColladaNode();
					marker.Name = ColladaUtilities.FormatName(model_tag.Markers[i].Name, " ", "_");

					if(modelInfo.GetIsMultiplePerms())
						marker.Name += "-perm" + model_tag.Markers[i].Instances[j].PermutationIndex.Value.ToString();

					marker.Type = Enums.ColladaNodeType.NODE;

					// set the nodes position
					Core.ColladaTranslate translate = new Core.ColladaTranslate();
					translate.SetTranslate(model_tag.Markers[i].Instances[j].Translation, 100);
					marker.Add(translate);

					// set the nodes rotation
					marker.AddRange(
						ColladaUtilities.CreateRotationSet(model_tag.Markers[i].Instances[j].Rotation.ToEuler3D(BlamVersion.Halo1),
							RotationVectorY, RotationVectorP, RotationVectorR));

					listBone[model_tag.Markers[i].Instances[j].NodeIndex].Add(marker);
				}
			}
		}
		#endregion
		#endregion

		#region Library Creation
		/// <summary>
		/// Creates the library_geometries element in the collada file
		/// </summary>
		void AddLibraryGeometries()
		{
			COLLADAFile.LibraryGeometries = new Core.ColladaLibraryGeometries();
			COLLADAFile.LibraryGeometries.Geometry = new List<Core.ColladaGeometry>();
			COLLADAFile.LibraryGeometries.Geometry.AddRange(listGeometry);
		}
		/// <summary>
		/// Creates the library_controllers element in the collada file
		/// </summary>
		void AddLibraryControllers()
		{
			COLLADAFile.LibraryControllers = new Core.ColladaLibraryControllers();
			COLLADAFile.LibraryControllers.Controller = new List<Core.ColladaController>();
			COLLADAFile.LibraryControllers.Controller.AddRange(listController);
		}
		/// <summary>
		/// Creates the library_visual_scenes element in the collada file
		/// </summary>
		void AddLibraryVisualScenes()
		{
			COLLADAFile.LibraryVisualScenes = new Core.ColladaLibraryVisualScenes();
			COLLADAFile.LibraryVisualScenes.VisualScene = new List<Core.ColladaVisualScene>();
			COLLADAFile.LibraryVisualScenes.VisualScene.Add(new Core.ColladaVisualScene());
			COLLADAFile.LibraryVisualScenes.VisualScene[0].ID = "main";
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