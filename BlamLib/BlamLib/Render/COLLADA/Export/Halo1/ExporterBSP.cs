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
	public class ColladaBSPExporter : ColladaModelExporterHalo1
	{
		#region Class Fields
		IHalo1BSPInterface bspInfo;

		List<Core.ColladaGeometry> listGeometry = new List<Core.ColladaGeometry>();
		List<Core.ColladaNode> listMarkers = new List<Core.ColladaNode>();
		List<Core.ColladaNode> listNode = new List<Core.ColladaNode>();
		#endregion

		#region Constructor
		/// <summary>
		/// Halo1 BSP exporter class
		/// </summary>
		/// <param name="bsp_info">An object implementing IHalo1BSPInterface to define what meshes ate to be included in the collada file</param>
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
				CreateMaterial(String.Format(Fx.ColladaMaterial.ElementIDFormat, shader_name),
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
				CreateMaterial(String.Format(Fx.ColladaMaterial.ElementIDFormat, shader_name),
					shader_name,
					shader_name,
					shader_name));
		}
		#endregion
		#region Create Geometry
		/// <summary>
		/// Creates a geometry element for a BSP lightmap
		/// </summary>
		/// <param name="index">The lightmap index to create a geometry from</param>
		/// <returns></returns>
		Core.ColladaGeometry CreateRenderGeometry(int index)
		{
			H1.Tags.structure_bsp_group definition = tagManager.TagDefinition as H1.Tags.structure_bsp_group;

			Core.ColladaGeometry geometry = new Core.ColladaGeometry();

			// initialize the geometry attributes
			geometry.Name = ColladaUtilities.FormatName(tagName, " ", "_") + "-" + definition.Lightmaps[index].Bitmap.ToString();
			geometry.ID = String.Format(Core.ColladaGeometry.ElementIDFormat, geometry.Name);

			// create a new mesh element and add the necessary source elements
			geometry.Mesh = new Core.ColladaMesh();
			geometry.Mesh.Source = new List<Core.ColladaSource>();
			geometry.Mesh.Source.Add(CreateSource(Enums.ColladaInputSharedSemantic.POSITION, geometry.ID, 0));
			geometry.Mesh.Source.Add(CreateSource(Enums.ColladaInputSharedSemantic.NORMAL, geometry.ID, 0));
			geometry.Mesh.Source.Add(CreateSource(Enums.ColladaInputSharedSemantic.BINORMAL, geometry.ID, 0));
			geometry.Mesh.Source.Add(CreateSource(Enums.ColladaInputSharedSemantic.TANGENT, geometry.ID, 0));
			geometry.Mesh.Source.Add(CreateSource(Enums.ColladaInputSharedSemantic.TEXCOORD, geometry.ID, 0));
			geometry.Mesh.Source.Add(CreateSource(Enums.ColladaInputSharedSemantic.TEXCOORD, geometry.ID, 1));

			// link the POSITION source to the vertex position element
			geometry.Mesh.Vertices = CreateVertices(geometry.ID, geometry.Mesh.Source[0].ID);

			geometry.Mesh.Triangles = new List<Core.ColladaTriangles>();
			// loop through each lightmap material, adding data to the source elements
			int index_offset = 0;
			for (int material_index = 0; material_index < definition.Lightmaps[index].Materials.Count; material_index++)
			{
				geometry.Mesh.Triangles.Add(new Core.ColladaTriangles());

				// initialize attributes
				string shader_name = ColladaUtilities.FormatName(
					System.IO.Path.GetFileNameWithoutExtension(
						definition.Lightmaps[index].Materials[material_index].Shader.ToString()), 
						" ", "_");

				geometry.Mesh.Triangles[material_index].Material = shader_name;
				geometry.Mesh.Triangles[material_index].Count = (uint)definition.Lightmaps[index].Materials[material_index].Count;
				geometry.Mesh.Triangles[material_index].Input = new List<Core.ColladaInputShared>();

				// link to data sources
				Core.ColladaInputShared input;

				input = new Core.ColladaInputShared();
				input.Semantic = Enums.ColladaInputSharedSemantic.VERTEX;
				input.Source = ColladaUtilities.BuildUri(geometry.Mesh.Vertices.ID);
				input.Offset = 0;
				geometry.Mesh.Triangles[material_index].Input.Add(input);

				input = new Core.ColladaInputShared();
				input.Semantic = Enums.ColladaInputSharedSemantic.NORMAL;
				input.Source = ColladaUtilities.BuildUri(geometry.Mesh.Source[1].ID);
				input.Offset = 1;
				geometry.Mesh.Triangles[material_index].Input.Add(input);

				input = new Core.ColladaInputShared();
				input.Semantic = Enums.ColladaInputSharedSemantic.BINORMAL;
				input.Source = ColladaUtilities.BuildUri(geometry.Mesh.Source[2].ID);
				input.Offset = 1;
				geometry.Mesh.Triangles[material_index].Input.Add(input);

				input = new Core.ColladaInputShared();
				input.Semantic = Enums.ColladaInputSharedSemantic.TANGENT;
				input.Source = ColladaUtilities.BuildUri(geometry.Mesh.Source[3].ID);
				input.Offset = 1;
				geometry.Mesh.Triangles[material_index].Input.Add(input);

				input = new Core.ColladaInputShared();
				input.Semantic = Enums.ColladaInputSharedSemantic.TEXCOORD;
				input.Source = ColladaUtilities.BuildUri(geometry.Mesh.Source[4].ID);
				input.Offset = 2;
				input.Set = 0;
				geometry.Mesh.Triangles[material_index].Input.Add(input);

				input = new Core.ColladaInputShared();
				input.Semantic = Enums.ColladaInputSharedSemantic.TEXCOORD;
				input.Source = ColladaUtilities.BuildUri(geometry.Mesh.Source[5].ID);
				input.Offset = 3;
				input.Set = 1;
				geometry.Mesh.Triangles[material_index].Input.Add(input);

				// read vertex information from the uncompressed vertex data
				System.IO.BinaryReader uncompressed_reader = new System.IO.BinaryReader(
					new System.IO.MemoryStream(definition.Lightmaps[index].Materials[material_index].UncompressedVertices.Value));

				int vertex_count = definition.Lightmaps[index].Materials[material_index].VertexBuffersCount1;

				for (int vertex_index = 0; vertex_index < vertex_count; vertex_index++)
				{
					int k = 0;
					//RealPoint3D   position
					for (k = 0; k < 3; k++) geometry.Mesh.Source[0].FloatArray.Add(uncompressed_reader.ReadSingle() * 100);
					//RealVector3D  normal
					for (k = 0; k < 3; k++) geometry.Mesh.Source[1].FloatArray.Add(uncompressed_reader.ReadSingle() * 100);
					//RealVector3D  binormal
					for (k = 0; k < 3; k++) geometry.Mesh.Source[2].FloatArray.Add(uncompressed_reader.ReadSingle());
					//RealVector3D  tangent
					for (k = 0; k < 3; k++) geometry.Mesh.Source[3].FloatArray.Add(uncompressed_reader.ReadSingle());
					//RealPoint2D   texcoord0
					geometry.Mesh.Source[4].FloatArray.Add(uncompressed_reader.ReadSingle());
					geometry.Mesh.Source[4].FloatArray.Add((uncompressed_reader.ReadSingle() * -1) + 1);
					
					//RealPoint2D   texcoord1
					if (definition.Lightmaps[index].Materials[material_index].VertexBuffersCount2 != 0)
					{
						int position = (int)uncompressed_reader.BaseStream.Position;
						uncompressed_reader.BaseStream.Position =
							(definition.Lightmaps[index].Materials[material_index].VertexBuffersCount1 * 56) +
							(vertex_index * 20) + 12;
						geometry.Mesh.Source[5].FloatArray.Add(uncompressed_reader.ReadSingle());
						geometry.Mesh.Source[5].FloatArray.Add((uncompressed_reader.ReadSingle() * -1) + 1);
						uncompressed_reader.BaseStream.Position = position;
					}
					else
					{
						geometry.Mesh.Source[5].FloatArray.Add(0.0f);
						geometry.Mesh.Source[5].FloatArray.Add(0.0f);
					}
				};

				// add surface index information
				geometry.Mesh.Triangles[material_index].P = new ColladaValueArray<int>();

				for (int surface_index = 0; surface_index < definition.Lightmaps[index].Materials[material_index].SurfaceCount; surface_index++)
				{
					ColladaValueArray<uint> poly = new ColladaValueArray<uint>();
					int k = 0;
					for (k = 0; k < 4; k++)
						geometry.Mesh.Triangles[material_index].P.Add((int)(definition.Surfaces[definition.Lightmaps[index].Materials[material_index].Surfaces.Value + surface_index].A3 + index_offset));
					for (k = 0; k < 4; k++)
						geometry.Mesh.Triangles[material_index].P.Add((int)(definition.Surfaces[definition.Lightmaps[index].Materials[material_index].Surfaces.Value + surface_index].A2 + index_offset));
					for (k = 0; k < 4; k++)
						geometry.Mesh.Triangles[material_index].P.Add((int)(definition.Surfaces[definition.Lightmaps[index].Materials[material_index].Surfaces.Value + surface_index].A1 + index_offset));
				}
				index_offset += vertex_count;
			}

			geometry.Mesh.Source[0].TechniqueCommon.Accessor.SetCount(geometry.Mesh.Source[0].FloatArray.Count);
			geometry.Mesh.Source[1].TechniqueCommon.Accessor.SetCount(geometry.Mesh.Source[1].FloatArray.Count);
			geometry.Mesh.Source[2].TechniqueCommon.Accessor.SetCount(geometry.Mesh.Source[2].FloatArray.Count);
			geometry.Mesh.Source[3].TechniqueCommon.Accessor.SetCount(geometry.Mesh.Source[3].FloatArray.Count);
			geometry.Mesh.Source[4].TechniqueCommon.Accessor.SetCount(geometry.Mesh.Source[4].FloatArray.Count);
			geometry.Mesh.Source[5].TechniqueCommon.Accessor.SetCount(geometry.Mesh.Source[5].FloatArray.Count);

			return geometry;
		}
		/// <summary>
		/// Creates a geometry element for a single cluster portal
		/// </summary>
		/// <param name="portal_index">Index of the portal to create a geometry element for</param>
		/// <returns></returns>
		Core.ColladaGeometry CreatePortalsGeometry(int portal_index)
		{
			H1.Tags.structure_bsp_group definition = tagManager.TagDefinition as H1.Tags.structure_bsp_group;

			Core.ColladaGeometry geometry = new Core.ColladaGeometry();

			// initialize the geometry's attributes
			geometry.Name = "portal-" + portal_index.ToString();
			geometry.ID = String.Format(Core.ColladaGeometry.ElementIDFormat, geometry.Name);

			// create a new mesh element and add the necessary source elements
			geometry.Mesh = new Core.ColladaMesh();
			geometry.Mesh.Source = new List<Core.ColladaSource>();
			geometry.Mesh.Source.Add(CreateSource(Enums.ColladaInputSharedSemantic.POSITION, geometry.ID, 0));

			// link the POSITION source to the vertex position element
			geometry.Mesh.Vertices = CreateVertices(geometry.ID, geometry.Mesh.Source[0].ID);

			// add the vertex position information to the position source
			for (int i = 0; i < definition.ClusterPortals[portal_index].Vertices.Count; i++)
			{
				geometry.Mesh.Source[0].FloatArray.Add(definition.ClusterPortals[portal_index].Vertices[i].Value.X * 100);
				geometry.Mesh.Source[0].FloatArray.Add(definition.ClusterPortals[portal_index].Vertices[i].Value.Y * 100);
				geometry.Mesh.Source[0].FloatArray.Add(definition.ClusterPortals[portal_index].Vertices[i].Value.Z * 100);
			};

			geometry.Mesh.Triangles = new List<Core.ColladaTriangles>();
			geometry.Mesh.Triangles.Add(BuildFaceIndices(definition.ClusterPortals[portal_index].Vertices.Count));
			geometry.Mesh.Triangles[0].Count = (uint)definition.ClusterPortals[portal_index].Vertices.Count - 2;
			geometry.Mesh.Triangles[0].Material = "portals";
			geometry.Mesh.Triangles[0].Input = new List<Core.ColladaInputShared>();
			geometry.Mesh.Triangles[0].Input.Add(new Core.ColladaInputShared());
			geometry.Mesh.Triangles[0].Input[0].Semantic = Enums.ColladaInputSharedSemantic.VERTEX;
			geometry.Mesh.Triangles[0].Input[0].Source = ColladaUtilities.BuildUri(geometry.Mesh.Vertices.ID);

			geometry.Mesh.Source[0].TechniqueCommon.Accessor.SetCount((uint)geometry.Mesh.Source[0].FloatArray.Count);

			return geometry;
		}
		/// <summary>
		/// Creates a geometry element for a single fog plane
		/// </summary>
		/// <param name="index">Index of the fog plane to create a geometry element for</param>
		/// <returns></returns>
		Core.ColladaGeometry CreateFogPlaneGeometry(int index)
		{
			H1.Tags.structure_bsp_group definition = tagManager.TagDefinition as H1.Tags.structure_bsp_group;

			Core.ColladaGeometry geometry = new Core.ColladaGeometry();

			// initialize the geometry's attributes
			geometry.Name = "fogplane-" + index.ToString();
			geometry.ID = String.Format(Core.ColladaGeometry.ElementIDFormat, geometry.Name);

			// create a new mesh element and add the necessary source elements
			geometry.Mesh = new Core.ColladaMesh();
			geometry.Mesh.Source = new List<Core.ColladaSource>();
			geometry.Mesh.Source.Add(CreateSource(Enums.ColladaInputSharedSemantic.POSITION, geometry.ID, 0));

			// link the POSITION source to the vertex position element
			geometry.Mesh.Vertices = CreateVertices(geometry.ID, geometry.Mesh.Source[0].ID);

			// add the vertex position information to the position source
			for (int i = 0; i < definition.FogPlanes[index].Vertices.Count; i++)
			{
				geometry.Mesh.Source[0].FloatArray.Add(definition.FogPlanes[index].Vertices[i].Value.X * 100);
				geometry.Mesh.Source[0].FloatArray.Add(definition.FogPlanes[index].Vertices[i].Value.Y * 100);
				geometry.Mesh.Source[0].FloatArray.Add(definition.FogPlanes[index].Vertices[i].Value.Z * 100);
			};

			geometry.Mesh.Triangles = new List<Core.ColladaTriangles>();
			geometry.Mesh.Triangles.Add(BuildFaceIndices(definition.FogPlanes[index].Vertices.Count));
			geometry.Mesh.Triangles[0].Count = (uint)definition.FogPlanes[index].Vertices.Count - 2;
			geometry.Mesh.Triangles[0].Material = "fogplanes";
			geometry.Mesh.Triangles[0].Input = new List<Core.ColladaInputShared>();
			geometry.Mesh.Triangles[0].Input.Add(new Core.ColladaInputShared());
			geometry.Mesh.Triangles[0].Input[0].Semantic = Enums.ColladaInputSharedSemantic.VERTEX;
			geometry.Mesh.Triangles[0].Input[0].Source = ColladaUtilities.BuildUri(geometry.Mesh.Vertices.ID);

			geometry.Mesh.Source[0].TechniqueCommon.Accessor.SetCount((uint)geometry.Mesh.Source[0].FloatArray.Count);

			return geometry;
		}
		/// <summary>
		/// Creates geometries for the relevant BSP meshes that are to be included in the collada file
		/// </summary>
		void CreateGeometryList()
		{
			H1.Tags.structure_bsp_group definition = tagManager.TagDefinition as H1.Tags.structure_bsp_group;

			if(bspInfo.IncludeRenderMesh())
				for (int i = 0; i < definition.Lightmaps.Count; i++)
					listGeometry.Add(CreateRenderGeometry(i));

			if(bspInfo.IncludePortalsMesh())
				for (int i = 0; i < definition.ClusterPortals.Count; i++)
					listGeometry.Add(CreatePortalsGeometry(i));

			if(bspInfo.IncludeFogPlanesMesh())
				for (int i = 0; i < definition.FogPlanes.Count; i++)
					listGeometry.Add(CreateFogPlaneGeometry(i));
		}
		#endregion
		#region Create Markers
		Core.ColladaNode CreateMarker(int index)
		{
			H1.Tags.structure_bsp_group definition = tagManager.TagDefinition as H1.Tags.structure_bsp_group;

			Core.ColladaNode marker = new Core.ColladaNode();

			marker.Name = ColladaUtilities.FormatName(definition.Markers[index].Name.Value);
			marker.Type = Enums.ColladaNodeType.NODE;
			marker.Add(new Core.ColladaTranslate(definition.Markers[index].Position, 100.0f));
			marker.AddRange(ColladaUtilities.CreateRotationSet(definition.Markers[index].Rotation.ToEuler3D()));

			return marker;
		}
		void CreateMarkerList()
		{
			H1.Tags.structure_bsp_group definition = tagManager.TagDefinition as H1.Tags.structure_bsp_group;

			for (int i = 0; i < definition.Markers.Count; i++)
				listMarkers.Add(CreateMarker(i));
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
			Core.ColladaNode model_node = new Core.ColladaNode();
			model_node.Name = listGeometry[index].Name;
			model_node.Name = ColladaUtilities.FormatName(model_node.Name, " ", "_");
			model_node.ID = String.Format(Core.ColladaNode.ElementIDFormat, model_node.Name);
			model_node.Type = Enums.ColladaNodeType.NODE;

			// create a new controller instance and set its attributes
			Core.ColladaInstanceGeometry instance_geometry = new Core.ColladaInstanceGeometry();
			instance_geometry.URL = ColladaUtilities.BuildUri(listGeometry[index].ID);

			// bind materials to the geometry
			instance_geometry.BindMaterial = new Fx.ColladaBindMaterial();
			instance_geometry.BindMaterial.TechniqueCommon = new Core.ColladaTechniqueCommon();

			// get a list of the shaders used by the geometry
			List<string> shader_list = new List<string>();
			for(int i = 0; i < bspInfo.GetShaderCount(); i++)
			{
				// get the tag manager of the shader
				BlamLib.Managers.TagManager shader_tag = tagIndex[bspInfo.GetShaderDatum(i)];

				// format the shaders name and add it to the shader list
				string shader_name = ColladaUtilities.FormatName(System.IO.Path.GetFileNameWithoutExtension(shader_tag.Name), " ", "_");
				if (!shader_list.Contains(shader_name))
					shader_list.Add(shader_name);
			}

			instance_geometry.BindMaterial.TechniqueCommon.InstanceMaterial = new List<Fx.ColladaInstanceMaterial>();
			for (int shader_index = 0; shader_index < shader_list.Count; shader_index++)
			{
				// create a new material instance referencing the required material
				Fx.ColladaInstanceMaterial instance_material = new Fx.ColladaInstanceMaterial();
				instance_material.Symbol = shader_list[shader_index];
				instance_material.Target = ColladaUtilities.BuildUri(String.Format(Fx.ColladaMaterial.ElementIDFormat, shader_list[shader_index]));

				// add the material instance to the list
				instance_geometry.BindMaterial.TechniqueCommon.InstanceMaterial.Add(instance_material);
			}
			model_node.Add(instance_geometry);

			return model_node;
		}
		/// <summary>
		/// Creates a collada node for a single portal geometry
		/// </summary>
		/// <param name="index">The portal geometry index to create a node for</param>
		/// <returns></returns>
		Core.ColladaNode CreateNodePortal(int index)
		{
			Core.ColladaNode model_node = new Core.ColladaNode();
			model_node.Name = listGeometry[index].Name;
			model_node.Name = ColladaUtilities.FormatName(model_node.Name, " ", "_");
			model_node.ID = String.Format(Core.ColladaNode.ElementIDFormat, model_node.Name);
			model_node.Type = Enums.ColladaNodeType.NODE;

			// create a new controller instance and set its attributes
			Core.ColladaInstanceGeometry instance_geometry = new Core.ColladaInstanceGeometry();
			instance_geometry.URL = ColladaUtilities.BuildUri(listGeometry[index].ID);

			// bind materials to the geometry
			instance_geometry.BindMaterial = new Fx.ColladaBindMaterial();
			instance_geometry.BindMaterial.TechniqueCommon = new Core.ColladaTechniqueCommon();
			instance_geometry.BindMaterial.TechniqueCommon.InstanceMaterial = new List<Fx.ColladaInstanceMaterial>();

			Fx.ColladaInstanceMaterial instance_material = new Fx.ColladaInstanceMaterial();
			instance_material.Symbol = "portals";
			instance_material.Target = ColladaUtilities.BuildUri(String.Format(Fx.ColladaMaterial.ElementIDFormat, "portals"));

			// add the material instance to the list
			instance_geometry.BindMaterial.TechniqueCommon.InstanceMaterial.Add(instance_material);

			model_node.Add(instance_geometry);

			return model_node;
		}
		/// <summary>
		/// Creates a collada node for a single fogplane geometry
		/// </summary>
		/// <param name="index">The fogplane geometry index to create a node for</param>
		/// <returns></returns>
		Core.ColladaNode CreateNodeFogPlane(int index)
		{
			Core.ColladaNode model_node = new Core.ColladaNode();
			model_node.Name = listGeometry[index].Name;
			model_node.Name = ColladaUtilities.FormatName(model_node.Name, " ", "_");
			model_node.ID = String.Format(Core.ColladaNode.ElementIDFormat, model_node.Name);
			model_node.Type = Enums.ColladaNodeType.NODE;

			// create a new controller instance and set its attributes
			Core.ColladaInstanceGeometry instance_geometry = new Core.ColladaInstanceGeometry();
			instance_geometry.URL = ColladaUtilities.BuildUri(listGeometry[index].ID);

			// bind materials to the geometry
			instance_geometry.BindMaterial = new Fx.ColladaBindMaterial();
			instance_geometry.BindMaterial.TechniqueCommon = new Core.ColladaTechniqueCommon();
			instance_geometry.BindMaterial.TechniqueCommon.InstanceMaterial = new List<Fx.ColladaInstanceMaterial>();

			Fx.ColladaInstanceMaterial instance_material = new Fx.ColladaInstanceMaterial();
			instance_material.Symbol = "fogplanes";
			instance_material.Target = ColladaUtilities.BuildUri(String.Format(Fx.ColladaMaterial.ElementIDFormat, "fogplanes"));

			// add the material instance to the list
			instance_geometry.BindMaterial.TechniqueCommon.InstanceMaterial.Add(instance_material);

			model_node.Add(instance_geometry);

			return model_node;
		}
		/// <summary>
		/// Creates nodes for all the geometry elements in the collada file
		/// </summary>
		void CreateNodeList()
		{
			H1.Tags.structure_bsp_group definition = tagManager.TagDefinition as H1.Tags.structure_bsp_group;

			int geometry_offset = 0;
			if(bspInfo.IncludeRenderMesh())
				for (int i = 0; i < definition.Lightmaps.Count; i++)
					listNode.Add(CreateNodeRender(geometry_offset + i));
			geometry_offset += definition.Lightmaps.Count;

			if(bspInfo.IncludePortalsMesh())
				for (int i = 0; i < definition.ClusterPortals.Count; i++)
					listNode.Add(CreateNodePortal(geometry_offset + i));
			geometry_offset += definition.ClusterPortals.Count;

			if(bspInfo.IncludeFogPlanesMesh())
				for (int i = 0; i < definition.FogPlanes.Count; i++)
					listNode.Add(CreateNodeFogPlane(geometry_offset + i));
			geometry_offset += definition.FogPlanes.Count;
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

			if (listMarkers.Count > 0)
				frame.AddRange(listMarkers);

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