/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
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
		#endregion

		#region Constructor
		/// <summary>
		/// Halo1 Render Model exporter class
		/// </summary>
		/// <param name="model_info">An object implementing IHalo1ModelInterface to provide geometry name and index pairs</param>
		/// <param name="tag_index">The tag index containing the tag being exported</param>
		/// <param name="tag_manager">The tag manager of the tag being exported</param>
		public ColladaRenderModelExporter(IColladaSettings settings, IHalo2RenderModelInterface model_info, Managers.TagIndexBase tag_index, Managers.TagManager tag_manager)
			: base(settings, model_info, tag_index, tag_manager)
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
			//H2.Tags.render_model_group definition = tagManager.TagDefinition as H2.Tags.render_model_group;

			//// create a list of shader names
			//List<string> shader_names = new List<string>();
			//foreach (var material in definition.Materials)
			//    shader_names.Add(Path.GetFileNameWithoutExtension(material.Shader.ToString()));

			//// create a geometry element for each geometry in the modelInfo
			//for (int i = 0; i < modelInfo.GetGeometryCount(); i++)
			//{
			//    string name = ColladaUtilities.FormatName(modelInfo.GetGeometryName(i), " ", "_");

			//    var section = definition.Sections[modelInfo.GetGeometryIndex(i)];

			//    // create the geometry element
			//    CreateGeometryHalo2(name, true,
			//        section.SectionInfo,
			//        section.SectionData[0].Section,
			//        shader_names);
			//}
		}
		#endregion
		#region Create Controllers
		/// <summary>
		/// Creates a controller to skin each geometry in the file
		/// </summary>
		void CreateControllerList()
		{
			//H2.Tags.render_model_group definition = tagManager.TagDefinition as H2.Tags.render_model_group;

			//// if there are no bones then skinning isnt necessary
			//if (definition.Nodes.Count == 0)
			//    return;

			//// create a controller for each geometry in modelInfo
			//for (int i = 0; i < modelInfo.GetGeometryCount(); i++)
			//{
			//    H2.Tags.render_model_section_block.render_model_section_data_block section_block = 
			//        definition.Sections[modelInfo.GetGeometryIndex(i)].SectionData[0];

			//    // the node map contains a list of bone indices that the section is rigged to
			//    List<int> node_map = new List<int>();
			//    foreach (var node in section_block.NodeMap)
			//        node_map.Add(node.NodeIndex.Value);

			//    List<VertexWeight> vertex_weights = new List<VertexWeight>();
			//    // create a generic list of vertex weights
			//    foreach(var vertex in section_block.Section.Value.RawVertices)
			//    {
			//        VertexWeight common_weight = new VertexWeight();

			//        // only add the weights with valid nodes
			//        for(int weight_index = 0; (weight_index < vertex.Point.NodeIndex.Length) && (vertex.Point.NodeIndex[weight_index] != -1); weight_index++)
			//            common_weight.AddWeight(node_map[vertex.Point.NodeIndex[weight_index]], vertex.Point.NodeWeight[weight_index]);

			//        vertex_weights.Add(common_weight);
			//    }

			//    // create the controller element
			//    CreateSkinController(listGeometry[i].ID, vertex_weights);
			//}
		}
		#endregion
		#region Create Bones
		/// <summary>
		/// Populates the bone list with 1 collada node for each bone
		/// </summary>
		void CreateBoneList()
		{
			//var definition = tagManager.TagDefinition as H2.Tags.render_model_group;

			//// there are no nodes, return
			//if (definition.Nodes.Count == 0)
			//    return;

			//List<Bone> bone_list = new List<Bone>();
			//// create a generic bone list
			//foreach(var node in definition.Nodes)
			//{
			//    bone_list.Add(new Bone(node.Name.ToString(),
			//        node.DefaultTranslation.ToPoint3D(100),
			//        node.DefaultRotation.ToQuaternion(),
			//        1.0f,
			//        node.ParentNode.Value,
			//        node.FirstChildNode.Value,
			//        node.NextSiblingNode.Value));
			//}

			//// create the bone node elements
			//CreateBones(bone_list, RotationVectorY, RotationVectorP, RotationVectorR);
		}
		#endregion
		#region Create Nodes
		/// <summary>
		/// Creates nodes for all the geometry elements in the collada file
		/// </summary>
		void CreateNodeList()
		{
			// create a list of all the shaders used in the file
			//List<string> shader_list = new List<string>();
			//for (int shader_index = 0; shader_index < shaderInfo.GetShaderCount(); shader_index++)
			//    shader_list.Add(ColladaUtilities.FormatName(Path.GetFileNameWithoutExtension(shaderInfo.GetShaderName(shader_index)), " ", "_"));

			// create a controller instance for each geometry
			for (int i = 0; i < listGeometry.Count; i++)
			{
				string url = ColladaUtilities.BuildUri(listGeometry[i].ID);
				string name = listGeometry[i].ID;

				Core.ColladaNode node = CreateNode(name, "", name, Enums.ColladaNodeType.NODE);

				node.Add(CreateInstanceGeometry(url, listGeometry[i].Name, new MaterialReferenceList()));

				listNode.Add(node);
			}
		}
		#endregion
		#region Create Markers
		/// <summary>
		/// Creates nodes for the models marker instances
		/// </summary>
		void CreateMarkerList()
		{
			//H2.Tags.render_model_group definition = tagManager.TagDefinition as H2.Tags.render_model_group;

			//List<Marker> marker_list = new List<Marker>();
			//// create a list of generic marker definitions
			//foreach (var marker in definition.MarkerGroups)
			//{
			//    string marker_name = ColladaUtilities.FormatName(marker.Name.ToString(), " ", "_");
			//    foreach (var instance in marker.Markers)
			//    {
			//        string name = marker_name;

			//        // the permutation index is 255 it is valid for all permutations so add it regardless
			//        if(instance.PermutationIndex.Value != 255)
			//        {
			//            // if exporting a single permutation and the instance permutation doesnt match, continue
			//            // otherwise if we are exporting multiple permutations append the instances permutation index to its name
			//            if (!modelInfo.GetIsMultiplePerms())
			//            {
			//                if (modelInfo.GetPermutation() != instance.PermutationIndex.Value)
			//                    continue;
			//            }
			//            else
			//                name += "-perm" + instance.PermutationIndex.Value.ToString();
			//        }

			//        Marker common_marker = new Marker(name,
			//            instance.Translation.ToPoint3D(100),
			//            instance.Rotation.ToQuaternion(),
			//            instance.NodeIndex);

			//        marker_list.Add(common_marker);
			//    }
			//}

			//// create the marker node elements
			//CreateMarkers(marker_list, RotationVectorY, RotationVectorP, RotationVectorR);
		}
		#endregion
		#endregion

		#region Library Creation
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

			CreateImageList();
			CreateEffectList();
			CreateMaterialList();
			CreateBoneList();
			CreateGeometryList();
			CreateControllerList();
			CreateNodeList();
			CreateMarkerList();

			AddLibraryImages();
			AddLibraryEffects();
			AddLibraryMaterials();
			AddLibraryGeometries();
			AddLibraryControllers();
			AddLibraryVisualScenes();
			AddScene("main");

			return true;
		}
	};
}