/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.IO;

using H2 = BlamLib.Blam.Halo2;

namespace BlamLib.Render.COLLADA.Halo2
{
	public class ColladaBSPExporter : ColladaModelExporterHalo2
	{
		#region Class Fields
		IHalo2BSPInterface bspInfo;
		#endregion

		#region Constructor
		/// <summary>
		/// Halo2 Lightmap exporter class
		/// </summary>
		/// <param name="lightmap_info">An object implementing IHalo2LightmapInterface to define what meshes are to be included in the collada file</param>
		/// <param name="tag_index">The tag index that contains the tag being exported</param>
		/// <param name="tag_manager">The tag manager of the tag being exported</param>
		public ColladaBSPExporter(IColladaSettings settings, IHalo2BSPInterface info, Managers.TagIndexBase tag_index, Managers.TagManager tag_manager)
			: base(settings, info, tag_index, tag_manager)
		{
			bspInfo = info;
		}
		#endregion

		#region Element Creation
		#region Create Effects
		/// <summary>
		/// Creates a semitransparent, green effect for portals
		/// </summary>
		/// <returns></returns>
		Fx.ColladaEffect CreatePortalsEffect()
		{
			Fx.ColladaEffect effect = CreateDefaultEffect("portals");
			effect.ProfileCOMMON[0].Technique.Phong.Emission.Color.SetColor(0, 1, 0, 1);
			effect.ProfileCOMMON[0].Technique.Phong.Transparency.Float.Value = 0.25f;
			effect.ProfileCOMMON[0].Technique.Phong.Diffuse.Color.SetColor(0, 1, 0, 1);
			return effect;
		}
		/// <summary>
		/// Creates a semitransparent, yellow effect for fogplanes
		/// </summary>
		/// <returns></returns>
		Fx.ColladaEffect CreateFogPlanesEffect()
		{
			Fx.ColladaEffect effect = CreateDefaultEffect("fogplanes");
			effect.ProfileCOMMON[0].Technique.Phong.Emission.Color.SetColor(1, 1, 0, 1);
			effect.ProfileCOMMON[0].Technique.Phong.Transparency.Float.Value = 0.25f;
			effect.ProfileCOMMON[0].Technique.Phong.Diffuse.Color.SetColor(1, 1, 0, 1);
			return effect;
		}
		/// <summary>
		/// Adds the portals effect to the effect list
		/// </summary>
		void CreateEffectListPortals()
		{
			listEffect.Add(CreatePortalsEffect());
		}
		/// <summary>
		/// Adds the fog planes effect to the effect list
		/// </summary>
		void CreateEffectListFogPlanes()
		{
			listEffect.Add(CreateFogPlanesEffect());
		}
		#endregion
		#region Create Materials
		/// <summary>
		/// Populate the material list for portals geometry
		/// </summary>
		protected void CreateMaterialListPortals()
		{
			const string k_shader_name = "portals";
			listMaterial.Add(CreateMaterial(k_shader_name));
		}
		/// <summary>
		/// Populate the material list for fogplane geometry
		/// </summary>
		protected void CreateMaterialListFogPlanes()
		{
			const string k_shader_name = "fogplanes";
			listMaterial.Add(CreateMaterial(k_shader_name));
		}
		#endregion
		#region Create Geometry
		/// <summary>
		/// Creates a geometry element for a single cluster portal
		/// </summary>
		/// <param name="portal_index">Index of the portal to create a geometry element for</param>
		/// <returns></returns>
		void CreatePortalsGeometry(int index)
		{
			//H2.Tags.scenario_structure_bsp_group definition = tagManager.TagDefinition as H2.Tags.scenario_structure_bsp_group;

			//List<Vertex> common_vertices = new List<Vertex>();
			//// add the vertex position information to the position source
			//foreach(var vertex in definition.ClusterPortals[index].Vertices)
			//    common_vertices.Add(new Vertex(vertex.Point.ToPoint3D(100)));

			//List<Part> common_parts = new List<Part>();

			//// only one part is needed since one one material is used
			//Part common_part = new Part("portals");
			//common_part.AddIndices(BuildFaceIndices(definition.ClusterPortals[index].Vertices.Count));
			//common_parts.Add(common_part);

			//// create the geometry element
			//CreateGeometry("portal-" + index.ToString(), 0, VertexComponent.POSITION,
			//    common_vertices, common_parts);
		}
		/// <summary>
		/// Creates geometries for the relevant lightmap meshes that are to be included in the collada file
		/// </summary>
		void CreateGeometryList()
		{
			//H2.Tags.scenario_structure_bsp_group definition = tagManager.TagDefinition as H2.Tags.scenario_structure_bsp_group;

			//if (bspInfo.IncludeRenderMesh())
			//{
			//    // create a list of all the shaders used
			//    List<string> shader_list = new List<string>();
			//    foreach (var material in definition.Materials)
			//        shader_list.Add(Path.GetFileNameWithoutExtension(material.Shader.ToString()));

			//    // create a geometry element for each cluster
			//    for (int i = 0; i < definition.Clusters.Count; i++)
			//    {
			//        string name = String.Format("{0}-{1}", ColladaUtilities.FormatName(tagName, " ", "_"), i);

			//        // create the geometry element
			//        CreateGeometryHalo2(name, false,
			//            definition.Clusters[i].SectionInfo,
			//            definition.Clusters[i].ClusterData[0].Section.Value,
			//            shader_list);
			//    }
			//}
			//if (bspInfo.IncludePortalsMesh())
			//{
			//    // create a geometry element for each cluster portal
			//    for (int i = 0; i < definition.ClusterPortals.Count; i++)
			//        CreatePortalsGeometry(i);
			//}
		}
		#endregion
		#region Create Nodes
		/// <summary>
		/// Creates nodes for all the geometry elements in the collada file
		/// </summary>
		void CreateNodeList()
		{
			//H2.Tags.scenario_structure_bsp_group definition = tagManager.TagDefinition as H2.Tags.scenario_structure_bsp_group;

			// create a list of all the shaders used
			//List<string> shader_list = new List<string>();
			//for (int shader_index = 0; shader_index < shaderInfo.GetShaderCount(); shader_index++)
			//    shader_list.Add(ColladaUtilities.FormatName(Path.GetFileNameWithoutExtension(shaderInfo.GetShaderName(shader_index)), " ", "_"));

			// if portals are included and the portals material to the list
			//if (bspInfo.IncludePortalsMesh())
			//    shader_list.Add("portals");

			//// create a geometry instances for the render geometry
			//int geometry_offset = 0;
			//if (bspInfo.IncludeRenderMesh())
			//{
			//    for (int i = 0; i < definition.Clusters.Count; i++)
			//        CreateNodeInstanceGeometry(listGeometry[geometry_offset + i].Name, geometry_offset + i, null);
			//    geometry_offset += definition.Clusters.Count;
			//}

			//// create a geometry instances for the portal geometry
			//if (bspInfo.IncludePortalsMesh())
			//{
			//    for (int i = 0; i < definition.ClusterPortals.Count; i++)
			//        CreateNodeInstanceGeometry(listGeometry[geometry_offset + i].Name, geometry_offset + i, null);
			//    geometry_offset += definition.ClusterPortals.Count;
			//}
		}
		#endregion
		#region Create Markers
		void CreateMarkerList()
		{
			//H2.Tags.scenario_structure_bsp_group definition = tagManager.TagDefinition as H2.Tags.scenario_structure_bsp_group;

			//List<Marker> marker_list = new List<Marker>();
			//// create a list of generic marker definitions
			//foreach (var marker in definition.Markers)
			//{
			//    Marker common_marker = new Marker(marker.Name.ToString(),
			//        marker.Position.ToPoint3D(100),
			//        marker.Rotation.ToQuaternion(),
			//        -1);

			//    marker_list.Add(common_marker);
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

			Core.ColladaNode frame = new BlamLib.Render.COLLADA.Core.ColladaNode();
			frame.Name = "frame";
			frame.AddRange(listNode);

			COLLADAFile.LibraryVisualScenes.VisualScene[0].Node.Add(frame);
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

			if (bspInfo.IncludeRenderMesh())
			{
				CreateImageList();
				CreateEffectList();
				CreateMaterialList();
			}
			
			if(bspInfo.IncludePortalsMesh())
			{
				CreateEffectListPortals();
				CreateMaterialListPortals();
			}

			CreateGeometryList();
			CreateNodeList();
			CreateMarkerList();

			AddLibraryImages();
			AddLibraryEffects();
			AddLibraryMaterials();
			AddLibraryGeometries();
			AddLibraryVisualScenes();
			AddScene("main");

			return true;
		}
	};
}