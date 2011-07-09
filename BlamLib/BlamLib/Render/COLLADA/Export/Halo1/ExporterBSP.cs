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
	public class ColladaBSPExporter : ColladaModelExporterHalo1
	{
		#region Class Fields
		IHalo1BSPInterface bspInfo;
		#endregion

		#region Constructor
		/// <summary>
		/// Halo1 BSP exporter class
		/// </summary>
		/// <param name="bsp_info">An object implementing IHalo1BSPInterface to define what meshes are to be included in the collada file</param>
		/// <param name="tag_index">The tag index that contains the tag being exported</param>
		/// <param name="tag_manager">The tag manager of the tag being exported</param>
		public ColladaBSPExporter(IHalo1BSPInterface bsp_info, Managers.TagIndexBase tag_index, Managers.TagManager tag_manager)
			: base(bsp_info, tag_index, tag_manager)
		{
			bspInfo = bsp_info;
		}
		#endregion

		#region Element Creation
		#region Create Effects
		/// <summary>
		/// Creates a semitransparent, green effect for portals
		/// </summary>
		/// <returns></returns>
		static Fx.ColladaEffect CreatePortalsEffect()
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
		static Fx.ColladaEffect CreateFogPlanesEffect()
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
			string shader_name = "portals";
			listMaterial.Add(
				CreateMaterial(shader_name,
					shader_name,
					shader_name,
					shader_name));
		}
		/// <summary>
		/// Populate the material list for fogplane geometry
		/// </summary>
		protected void CreateMaterialListFogPlanes()
		{
			string shader_name = "fogplanes";
			listMaterial.Add(
				CreateMaterial(shader_name,
					shader_name,
					shader_name,
					shader_name));
		}
		#endregion
		#region Create Geometry
		/// <summary>
		/// Creates a vertex index list from a set of bsp surfaces
		/// </summary>
		/// <param name="definition">The bsp tag definition</param>
		/// <param name="surface_offset">The surface index to start collecting indices from</param>
		/// <param name="surface_count">The number of surfaces to collect indices from</param>
		/// <param name="index_offset">The amount to offset the collected indices by</param>
		/// <returns></returns>
		List<int> CreateIndicesBSP(H1.Tags.structure_bsp_group definition, int surface_offset, int surface_count, int index_offset)
		{
			List<int> indices = new List<int>();

			for (int i = 0; i < surface_count; i++)
			{
				indices.Add(definition.Surfaces[surface_offset + i].A3 + index_offset);
				indices.Add(definition.Surfaces[surface_offset + i].A2 + index_offset);
				indices.Add(definition.Surfaces[surface_offset + i].A1 + index_offset);
			}

			return indices;
		}
		/// <summary>
		/// Creates a geometry element for a BSP lightmap
		/// </summary>
		/// <param name="index">The lightmap index to create a geometry from</param>
		/// <returns></returns>
		void CreateRenderGeometry(int index)
		{
			H1.Tags.structure_bsp_group definition = tagManager.TagDefinition as H1.Tags.structure_bsp_group;

			List<Vertex> common_vertices = new List<Vertex>();

			// add all of the vertices used in the render geometry
			foreach (var material in definition.Lightmaps[index].Materials)
			{
				// read vertex information from the uncompressed vertex data
				System.IO.BinaryReader uncompressed_reader = new System.IO.BinaryReader(
					new System.IO.MemoryStream(material.UncompressedVertices.Value));

				int vertex_count = material.VertexBuffersCount1;

				for (int vertex_index = 0; vertex_index < vertex_count; vertex_index++)
				{
					Vertex common_vertex = new Vertex(
						//RealPoint3D position
						new LowLevel.Math.real_point3d(
							uncompressed_reader.ReadSingle() * 100,
							uncompressed_reader.ReadSingle() * 100,
							uncompressed_reader.ReadSingle() * 100),
						//RealVector3D  normal
						new LowLevel.Math.real_vector3d(
							uncompressed_reader.ReadSingle(),
							uncompressed_reader.ReadSingle(),
							uncompressed_reader.ReadSingle()),
						//RealVector3D  binormal
						new LowLevel.Math.real_vector3d(
							uncompressed_reader.ReadSingle(),
							uncompressed_reader.ReadSingle(),
							uncompressed_reader.ReadSingle()),
						//RealVector3D  tangent
						new LowLevel.Math.real_vector3d(
							uncompressed_reader.ReadSingle(),
							uncompressed_reader.ReadSingle(),
							uncompressed_reader.ReadSingle()));

					//RealPoint2D   texcoord0
					common_vertex.AddTexcoord(new LowLevel.Math.real_point2d(
						uncompressed_reader.ReadSingle(),
						(uncompressed_reader.ReadSingle() * -1) + 1));

					//RealPoint2D   texcoord1
					if (material.VertexBuffersCount2 != 0)
					{
						int position = (int)uncompressed_reader.BaseStream.Position;
						uncompressed_reader.BaseStream.Position = (material.VertexBuffersCount1 * 56) + (vertex_index * 20) + 12;

						common_vertex.AddTexcoord(new LowLevel.Math.real_point2d(
							uncompressed_reader.ReadSingle(),
							(uncompressed_reader.ReadSingle() * -1) + 1));

						uncompressed_reader.BaseStream.Position = position;
					}
					else
						common_vertex.AddTexcoord(new LowLevel.Math.real_point2d(0, 1));

					common_vertices.Add(common_vertex);
				};
			}

			List<Part> common_parts = new List<Part>();

			// add part definitions for the lightmap materials
			// an index offset is necessary since the vertex list is global for this geometry, rather than local to each material
			int index_offset = 0;
			foreach (var material in definition.Lightmaps[index].Materials)
			{
				Part common_part = new Part(Path.GetFileNameWithoutExtension(material.Shader.ToString()));
				common_part.AddIndices(CreateIndicesBSP(definition, material.Surfaces, material.SurfaceCount, index_offset));

				index_offset += material.VertexBuffersCount1;

				common_parts.Add(common_part);
			}

			// create the geometry element
			CreateGeometry(ColladaUtilities.FormatName(tagName, " ", "_") + "-" + definition.Lightmaps[index].Bitmap.ToString(), 2,
				VertexComponent.POSITION | VertexComponent.NORMAL | VertexComponent.BINORMAL | VertexComponent.TANGENT | VertexComponent.TEXCOORD,
				common_vertices, common_parts);
		}
		/// <summary>
		/// Creates a geometry element for a single cluster portal
		/// </summary>
		/// <param name="portal_index">Index of the portal to create a geometry element for</param>
		/// <returns></returns>
		void CreatePortalsGeometry(int index)
		{
			H1.Tags.structure_bsp_group definition = tagManager.TagDefinition as H1.Tags.structure_bsp_group;
			
			List<Vertex> common_vertices = new List<Vertex>();
			foreach (var vertex in definition.ClusterPortals[index].Vertices)
				common_vertices.Add(new Vertex(vertex.Value.ToPoint3D(100)));

			List<Part> common_parts = new List<Part>();

			// we only have one part since it only has one material
			Part common_part = new Part("portals");
			common_part.AddIndices(BuildFaceIndices(definition.ClusterPortals[index].Vertices.Count));
			common_parts.Add(common_part);

			// create the geometry element
			CreateGeometry("portal-" + index.ToString(), 0, VertexComponent.POSITION,
				common_vertices, common_parts);
		}
		/// <summary>
		/// Creates a geometry element for a single fog plane
		/// </summary>
		/// <param name="index">Index of the fog plane to create a geometry element for</param>
		/// <returns></returns>
		void CreateFogPlaneGeometry(int index)
		{
			H1.Tags.structure_bsp_group definition = tagManager.TagDefinition as H1.Tags.structure_bsp_group;

			List<Vertex> common_vertices = new List<Vertex>();

			foreach (var vertex in definition.FogPlanes[index].Vertices)
				common_vertices.Add(new Vertex(vertex.Value.ToPoint3D(100)));

			List<Part> common_parts = new List<Part>();

			// we only have one part since it only has one material
			Part common_part = new Part("fogplanes");
			common_part.AddIndices(BuildFaceIndices(definition.FogPlanes[index].Vertices.Count));
			common_parts.Add(common_part);

			// create the geometry element
			CreateGeometry("fogplane-" + index.ToString(), 0, VertexComponent.POSITION,
				common_vertices, common_parts);
		}
		/// <summary>
		/// Creates geometries for the relevant BSP meshes that are to be included in the collada file
		/// </summary>
		void CreateGeometryList()
		{
			H1.Tags.structure_bsp_group definition = tagManager.TagDefinition as H1.Tags.structure_bsp_group;

			if(bspInfo.IncludeRenderMesh())
				for (int i = 0; i < definition.Lightmaps.Count; i++)
					CreateRenderGeometry(i);

			if(bspInfo.IncludePortalsMesh())
				for (int i = 0; i < definition.ClusterPortals.Count; i++)
					CreatePortalsGeometry(i);

			if(bspInfo.IncludeFogPlanesMesh())
				for (int i = 0; i < definition.FogPlanes.Count; i++)
					CreateFogPlaneGeometry(i);
		}
		#endregion
		#region Create Markers
		/// <summary>
		/// Creates nodes for the BSP markers
		/// </summary>
		void CreateMarkerList()
		{
			H1.Tags.structure_bsp_group definition = tagManager.TagDefinition as H1.Tags.structure_bsp_group;

			List<Marker> marker_list = new List<Marker>();
			// create common marker definitions for the bsp markers
			foreach(var marker in definition.Markers)
			{
				Marker common_marker = new Marker(marker.Name,
					marker.Position.ToPoint3D(100), 
					TagInterface.RealQuaternion.Invert(marker.Rotation),
					-1);

				marker_list.Add(common_marker);
			}

			// create the marker node elements
			CreateMarkers(marker_list, RotationVectorY, RotationVectorP, RotationVectorR);
		}
		#endregion
		#region Create Nodes
		/// <summary>
		/// Creates nodes for all the geometry elements in the collada file
		/// </summary>
		void CreateNodeList()
		{
			H1.Tags.structure_bsp_group definition = tagManager.TagDefinition as H1.Tags.structure_bsp_group;

			// create a list of ever shader used
			List<string> shader_list = new List<string>();
			for (int shader_index = 0; shader_index < shaderInfo.GetShaderCount(); shader_index++)
				shader_list.Add(ColladaUtilities.FormatName(Path.GetFileNameWithoutExtension(shaderInfo.GetShaderName(shader_index)), " ", "_"));
			// if portals are included add the portals shader to the names
			if (bspInfo.IncludePortalsMesh())
				shader_list.Add("portals");
			// if fogplanes are included add the fogplanes shader to the names
			if (bspInfo.IncludeFogPlanesMesh())
				shader_list.Add("fogplanes");

			int geometry_offset = 0;
			if (bspInfo.IncludeRenderMesh())
			{
				// create geometry instance for all of the lightmaps
				for (int i = 0; i < definition.Lightmaps.Count; i++)
					CreateNodeInstanceGeometry(listGeometry[geometry_offset + i].Name, geometry_offset + i, shader_list);
				geometry_offset += definition.Lightmaps.Count;
			}

			if (bspInfo.IncludePortalsMesh())
			{
				// create geometry instance for all of the portal meshes
				for (int i = 0; i < definition.ClusterPortals.Count; i++)
					CreateNodeInstanceGeometry(listGeometry[geometry_offset + i].Name, geometry_offset + i, shader_list);
				geometry_offset += definition.ClusterPortals.Count;
			}

			if (bspInfo.IncludeFogPlanesMesh())
			{
				// create geometry instance for all of the fogplane meshes
				for (int i = 0; i < definition.FogPlanes.Count; i++)
					CreateNodeInstanceGeometry(listGeometry[geometry_offset + i].Name, geometry_offset + i, shader_list);
				geometry_offset += definition.FogPlanes.Count;
			}
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
			
			if(bspInfo.IncludeRenderMesh())
			{
				CollectBitmaps();

				CreateImageList();
				CreateEffectList();
				CreateMaterialList();
			}
			if(bspInfo.IncludePortalsMesh())
			{
				CreateEffectListPortals();
				CreateMaterialListPortals();
			}
			if(bspInfo.IncludeFogPlanesMesh())
			{
				CreateEffectListFogPlanes();
				CreateMaterialListFogPlanes();
			}

			CreateGeometryList();
			CreateMarkerList();
			CreateNodeList();

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