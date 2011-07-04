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
using System.IO;
using System.Collections.Generic;
using BlamLib.Blam;

using H2 = BlamLib.Blam.Halo2;

namespace BlamLib.Render.COLLADA.Halo2
{
	public class ColladaRenderModelExporter : ColladaModelExporterHalo2
	{
		#region Class Fields
		IHalo2RenderModelInterface modelInfo;

		List<Core.ColladaGeometry> listGeometry = new List<Core.ColladaGeometry>();
		List<Core.ColladaController> listController = new List<Core.ColladaController>();
		List<Core.ColladaNode> listNode = new List<Core.ColladaNode>();
		List<Core.ColladaNode> listBone = new List<Core.ColladaNode>();
		List<ColladaBoneMatrix> listBoneMatrix = new List<ColladaBoneMatrix>();
		#endregion

		#region Constructor
		/// <summary>
		/// Halo1 Render Model exporter class
		/// </summary>
		/// <param name="model_info">An object implementing IHalo1ModelInterface to provide geometry name and index pairs</param>
		/// <param name="tag_index">The tag index containing the tag being exported</param>
		/// <param name="tag_manager">The tag manager of the tag being exported</param>
		public ColladaRenderModelExporter(IHalo2RenderModelInterface model_info, Managers.TagIndexBase tag_index, Managers.TagManager tag_manager)
			: base(model_info, tag_index, tag_manager)
		{
			modelInfo = model_info;
		}
		#endregion

		#region Element Creation
		#region Create Geometry
		/// <summary>
		/// Creates geometries for the relevant meshes that are to be included in the collada file
		/// </summary>
		void CreateGeometryList()
		{
			H2.Tags.render_model_group definition = tagManager.TagDefinition as H2.Tags.render_model_group;

			// create a list of shader names
			List<string> shader_names = new List<string>();
			foreach (var material in definition.Materials)
				shader_names.Add(Path.GetFileNameWithoutExtension(tagIndex[material.Shader.Datum].Name));

			for (int i = 0; i < modelInfo.GetGeometryCount(); i++)
			{
				string name = ColladaUtilities.FormatName(modelInfo.GetGeometryName(i), " ", "_");
				listGeometry.Add(CreateGeometry(name,
					definition.Sections[modelInfo.GetGeometryIndex(i)].SectionInfo,
					definition.Sections[modelInfo.GetGeometryIndex(i)].SectionData[0].Section.Value,
					shader_names));
			}
		}
		#endregion
		#region Create Controllers
		/// <summary>
		/// Creates a controller for skinning one geometry object
		/// </summary>
		/// <param name="geometry_index">The geometry part the controller will control</param>
		/// <param name="geometry_id">String ID of the geometry being skinned</param>
		/// <returns></return>
		Core.ColladaController CreateSkinController(string name, List<int> node_map, H2.Tags.global_geometry_section_struct section)
		{
			H2.Tags.render_model_group model_tag = tagManager.TagDefinition as H2.Tags.render_model_group;

			// create the controller node
			Core.ColladaController controller = new BlamLib.Render.COLLADA.Core.ColladaController();

			// set the controllers name and id
			controller.Name = name + "-skin";
			controller.ID = String.Format(Core.ColladaController.ElementIDFormat, controller.Name);

			// create a skin element for the controller and set it to reference the correct geometry
			controller.Skin = new Core.ColladaSkin();
			controller.Skin.SourceAttrib = ColladaUtilities.BuildUri(name);

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

			foreach (H2.Tags.render_model_node_block node in model_tag.Nodes)
				controller.Skin.Source[0].NameArray.Add(ColladaUtilities.FormatName(node.Name.ToString(), " ", "_"));

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
			
			//TODO: handle old/new nodes
			foreach (var vertex in section.RawVertices)
			{
				// find out how many nodes the vertex is weighted to
				int weight_count = 0;
				foreach (int node_index in vertex.Point.NodeIndex)
				{
					if (node_index == -1)
						break;
					weight_count++;
				}

				// add the weight count and weight values to the relevant arrays
				controller.Skin.VertexWeights.VCount.Add(weight_count);

				for (int i = 0; i < weight_count; i++)
				{
					controller.Skin.Source[2].FloatArray.Add(vertex.Point.NodeWeight[i]);
					controller.Skin.VertexWeights.V.Add(node_map[vertex.Point.NodeIndex[i]], controller.Skin.Source[2].FloatArray.Values.Count - 1);
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
			H2.Tags.render_model_group model_tag = tagManager.TagDefinition as H2.Tags.render_model_group;

			if (model_tag.Nodes.Count == 0)
				return;

			for (int i = 0; i < modelInfo.GetGeometryCount(); i++)
			{
				// the node map contains a list of bone indices that the section is rigged to
				List<int> node_map = new List<int>();

				foreach (var node in model_tag.Sections[modelInfo.GetGeometryIndex(i)].SectionData[0].NodeMap)
					node_map.Add(node.NodeIndex.Value);

				listController.Add(CreateSkinController(listGeometry[i].ID, node_map, model_tag.Sections[modelInfo.GetGeometryIndex(i)].SectionData[0].Section.Value));
			}
		}
		#endregion
		#region Create Bones
		/// <summary>
		/// Creates a bone tree list that contains the matrices necessary for skinning
		/// </summary>
		void CreateBoneMatrixList()
		{
			H2.Tags.render_model_group definition = tagManager.TagDefinition as H2.Tags.render_model_group;
			// add nodes for all of the bones in order
			int i;
			for (i = 0; i < definition.Nodes.Count; i++)
			{
				listBoneMatrix.Add(new ColladaBoneMatrix(BlamVersion.Halo2,
					definition.Nodes[i].DefaultTranslation,
					100.0f,
					definition.Nodes[i].DefaultRotation,
					1.0f));
			}
			// now that all the nodes have been created, set the parent node value, and calculate the matrices
			for (i = 0; i < definition.Nodes.Count; i++)
			{
				if (definition.Nodes[i].ParentNode.Value != -1)
					listBoneMatrix[i].ParentNode = listBoneMatrix[definition.Nodes[i].ParentNode.Value];

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
			H2.Tags.render_model_group definition = tagManager.TagDefinition as H2.Tags.render_model_group;

			// bone has no children, return
			if (definition.Nodes[parent_node_index].FirstChildNode.Value == -1)
				return null;

			List<Core.ColladaNode> children = new List<Core.ColladaNode>();

			// cycle through the first child and its siblings
			int bone_index = definition.Nodes[parent_node_index].FirstChildNode.Value;
			while (bone_index != -1)
			{
				children.Add(listBone[bone_index]);

				bone_index = definition.Nodes[bone_index].NextSiblingNode.Value;
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
			H2.Tags.render_model_group definition = tagManager.TagDefinition as H2.Tags.render_model_group;

			// create a node and set its attributes
			Core.ColladaNode bone = new Core.ColladaNode();
			bone.Name = bone.sID = ColladaUtilities.FormatName(definition.Nodes[bone_index].Name.ToString(), " ", "_");
			bone.ID = String.Format(Core.ColladaNode.ElementIDFormat, bone.sID);
			bone.Type = Enums.ColladaNodeType.JOINT;

			// set the nodes position
			Core.ColladaTranslate translation = new Core.ColladaTranslate(definition.Nodes[bone_index].DefaultTranslation, 100);
			translation.sID = "translation";
			bone.Add(translation);

			// set the nodes rotation
			bone.AddRange(ColladaUtilities.CreateRotationSet(definition.Nodes[bone_index].DefaultRotation.ToEuler3D(BlamVersion.Halo2), 
				RotationVectorY, RotationVectorP, RotationVectorR));

			return bone;
		}
		/// <summary>
		/// Populates the bone list with 1 collada node for each bone
		/// </summary>
		void CreateBoneList()
		{
			H2.Tags.render_model_group definition = tagManager.TagDefinition as H2.Tags.render_model_group;

			if (definition.Nodes.Count == 0)
				return;

			// create the bones
			for (int i = 0; i < definition.Nodes.Count; i++)
				listBone.Add(CreateBone(i));

			// link the bones together
			for (int i = 0; i < definition.Nodes.Count; i++)
			    listBone[i].AddRange(GetChildBones(i));

			CreateBoneMatrixList();
		}
		#endregion
		#region Create Nodes
		/// <summary>
		/// Creates a collada node for a single render geometry
		/// </summary>
		/// <param name="index">The render geometry index to create a node for</param>
		/// <returns></returns>
		Core.ColladaNode CreateNodeRender(int index)
		{
			H2.Tags.render_model_group model_tag = tagManager.TagDefinition as H2.Tags.render_model_group;

			Core.ColladaNode model_node = new Core.ColladaNode();
			model_node.Name = ColladaUtilities.FormatName(listGeometry[index].Name, " ", "_");
			model_node.ID = String.Format(Core.ColladaNode.ElementIDFormat, model_node.Name);
			model_node.Type = Enums.ColladaNodeType.NODE;

			// create a new controller instance and set its attributes
			Core.ColladaInstanceController instance_controller = new Core.ColladaInstanceController();

			instance_controller.URL = ColladaUtilities.BuildUri(listController[index].ID);
			instance_controller.Skeleton = new List<Core.ColladaSkeleton>();
			instance_controller.Skeleton.Add(new Core.ColladaSkeleton());
			instance_controller.Skeleton[0].Value = ColladaUtilities.BuildUri(listBone[0].ID);

			// bind materials to the geometry
			instance_controller.BindMaterial = new List<Fx.ColladaBindMaterial>();
			instance_controller.BindMaterial.Add(new Fx.ColladaBindMaterial());
			instance_controller.BindMaterial[0].TechniqueCommon = new Core.ColladaTechniqueCommon();

			// get a list of the shaders used by the geometry
			List<string> shader_list = new List<string>();
			foreach (var part in model_tag.Sections[modelInfo.GetGeometryIndex(index)].SectionData[0].Section.Value.Parts)
			{
				// if the datum index is bad ignore the shader
				if (!IsDatumValid(model_tag.Materials[part.Material].Shader.Datum))
					continue;

				// get the tag manager of the shader
				BlamLib.Managers.TagManager shader_tag = tagIndex[model_tag.Materials[part.Material].Shader.Datum];

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
		/// Creates nodes for all the geometry elements in the collada file
		/// </summary>
		void CreateNodeList()
		{
			for(int i = 0; i < listGeometry.Count; i++)
				listNode.Add(CreateNodeRender(i));
		}
		#endregion
		#endregion

		#region Data Collection
		/// <summary>
		/// Populates the bitmap datums list
		/// </summary>
		protected void CollectBitmaps()
		{
			int effect_count = shaderInfo.GetShaderCount();
			// for each shader, create a new effect
			for (int i = 0; i < effect_count; i++)
			{
				List<DatumIndex> bitmaps = GetShaderBitmaps(shaderInfo.GetShaderDatum(i));

				foreach (var datum in bitmaps)
					if (!bitmapDatums.Contains(datum))
						bitmapDatums.Add(datum);
			}
		}
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
		/// Creates the library_visual_scenes element in the collada file. The node list is added under a node named "frame" since that is
		/// required when creating new BSPs.
		/// </summary>
		void AddLibraryVisualScenes()
		{
			// add the main scene node
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

			return true;
		}
	}
}