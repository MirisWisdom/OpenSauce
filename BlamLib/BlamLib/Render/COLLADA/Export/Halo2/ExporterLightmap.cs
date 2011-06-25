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

using H2 = BlamLib.Blam.Halo2;

namespace BlamLib.Render.COLLADA.Halo2
{
	public class ColladaLightmapExporter : ColladaModelExporterHalo2
	{
		#region Class Fields
		IHalo2LightmapInterface lightmapInfo;

		List<Core.ColladaGeometry> listGeometry = new List<Core.ColladaGeometry>();
		List<Core.ColladaNode> listNode = new List<Core.ColladaNode>();
		#endregion

		#region Constructor
		public ColladaLightmapExporter(IHalo2LightmapInterface lightmap_info, Managers.TagIndexBase tag_index, Managers.TagManager tag_manager)
			: base(lightmap_info, tag_index, tag_manager)
		{
			lightmapInfo = lightmap_info;
		}
		#endregion

		#region Element Creation
		#region Create Geometry
		/// <summary>
		/// Creates a geometry element for a lightmap
		/// </summary>
		/// <param name="index">The lightmap index to create a geometry from</param>
		/// <returns></returns>
		private Core.ColladaGeometry CreateRenderGeometry(int lightmap_index, int cluster_index)
		{
			H2.Tags.scenario_structure_lightmap_group definition = tagManager.TagDefinition as H2.Tags.scenario_structure_lightmap_group;

			Core.ColladaGeometry geometry = new Core.ColladaGeometry();

			// initialise the geometry attributes
			geometry.Name = ColladaUtilities.FormatName(tagName, " ", "_") + "-group" + lightmap_index.ToString() + "-cluster" + cluster_index.ToString();
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
			geometry.Mesh.Source.Add(CreateSource(Enums.ColladaInputSharedSemantic.TEXCOORD, geometry.ID, 2));

			// link the POSITION source to the vertex position element
			geometry.Mesh.Vertices = CreateVertices(geometry.ID, geometry.Mesh.Source[0].ID);

			geometry.Mesh.Triangles = new List<Core.ColladaTriangles>();
			// loop through each lightmap material, adding data to the source elements
			H2.Tags.global_geometry_section_info_struct geometry_section_info = definition.LightmapGroups[lightmap_index].Clusters[cluster_index].GeometryInfo;
			H2.Tags.global_geometry_section_struct geometry_section = definition.LightmapGroups[lightmap_index].Clusters[cluster_index].CacheData[0].Geometry.Value;

			geometry.Mesh.Triangles.Add(new Core.ColladaTriangles());
			geometry.Mesh.Triangles[0].Count = (uint)geometry_section_info.TotalVertexCount.Value;
			geometry.Mesh.Triangles[0].Input = new List<Core.ColladaInputShared>();

			// link to data sources
			Core.ColladaInputShared input;

			input = new Core.ColladaInputShared();
			input.Semantic = Enums.ColladaInputSharedSemantic.VERTEX;
			input.Source = ColladaUtilities.BuildUri(geometry.Mesh.Vertices.ID);
			input.Offset = 0;
			geometry.Mesh.Triangles[0].Input.Add(input);

			input = new Core.ColladaInputShared();
			input.Semantic = Enums.ColladaInputSharedSemantic.NORMAL;
			input.Source = ColladaUtilities.BuildUri(geometry.Mesh.Source[1].ID);
			input.Offset = 1;
			geometry.Mesh.Triangles[0].Input.Add(input);

			input = new Core.ColladaInputShared();
			input.Semantic = Enums.ColladaInputSharedSemantic.BINORMAL;
			input.Source = ColladaUtilities.BuildUri(geometry.Mesh.Source[2].ID);
			input.Offset = 1;
			geometry.Mesh.Triangles[0].Input.Add(input);

			input = new Core.ColladaInputShared();
			input.Semantic = Enums.ColladaInputSharedSemantic.TANGENT;
			input.Source = ColladaUtilities.BuildUri(geometry.Mesh.Source[3].ID);
			input.Offset = 1;
			geometry.Mesh.Triangles[0].Input.Add(input);

			input = new Core.ColladaInputShared();
			input.Semantic = Enums.ColladaInputSharedSemantic.TEXCOORD;
			input.Source = ColladaUtilities.BuildUri(geometry.Mesh.Source[4].ID);
			input.Offset = 2;
			input.Set = 0;
			geometry.Mesh.Triangles[0].Input.Add(input);

			input = new Core.ColladaInputShared();
			input.Semantic = Enums.ColladaInputSharedSemantic.TEXCOORD;
			input.Source = ColladaUtilities.BuildUri(geometry.Mesh.Source[5].ID);
			input.Offset = 3;
			input.Set = 1;
			geometry.Mesh.Triangles[0].Input.Add(input);

			input = new Core.ColladaInputShared();
			input.Semantic = Enums.ColladaInputSharedSemantic.TEXCOORD;
			input.Source = ColladaUtilities.BuildUri(geometry.Mesh.Source[6].ID);
			input.Offset = 4;
			input.Set = 2;
			geometry.Mesh.Triangles[0].Input.Add(input);

			// read vertex information from the uncompressed vertex data
			for (int vertex_index = 0; vertex_index < geometry_section.RawVertices.Count; vertex_index++)
			{
				//RealPoint3D   position
				geometry.Mesh.Source[0].FloatArray.Add(
					geometry_section.RawVertices[vertex_index].Point.Position.X * 100,
					geometry_section.RawVertices[vertex_index].Point.Position.Y * 100,
					geometry_section.RawVertices[vertex_index].Point.Position.Z * 100);
				//RealVector3D  normal
				geometry.Mesh.Source[1].FloatArray.Add(
					geometry_section.RawVertices[vertex_index].Normal.I * 100,
					geometry_section.RawVertices[vertex_index].Normal.J * 100,
					geometry_section.RawVertices[vertex_index].Normal.K * 100);
				//RealVector3D  binormal
				geometry.Mesh.Source[2].FloatArray.Add(
					geometry_section.RawVertices[vertex_index].Binormal.I,
					geometry_section.RawVertices[vertex_index].Binormal.J,
					geometry_section.RawVertices[vertex_index].Binormal.K);
				//RealVector3D  tangent
				geometry.Mesh.Source[3].FloatArray.Add(
					geometry_section.RawVertices[vertex_index].Tangent.I,
					geometry_section.RawVertices[vertex_index].Tangent.J,
					geometry_section.RawVertices[vertex_index].Tangent.K);
				//RealPoint2D   texcoord0
				geometry.Mesh.Source[4].FloatArray.Add(geometry_section.RawVertices[vertex_index].Texcoord.X);
				geometry.Mesh.Source[4].FloatArray.Add((geometry_section.RawVertices[vertex_index].Texcoord.Y * -1) + 1);
				//RealPoint2D   texcoord1
				geometry.Mesh.Source[5].FloatArray.Add(geometry_section.RawVertices[vertex_index].SecondaryTexcoord.X);
				geometry.Mesh.Source[5].FloatArray.Add((geometry_section.RawVertices[vertex_index].SecondaryTexcoord.Y * -1) + 1);
				//RealPoint2D   texcoord2
				geometry.Mesh.Source[6].FloatArray.Add(geometry_section.RawVertices[vertex_index].PrimaryLightmapTexcoord.X);
				geometry.Mesh.Source[6].FloatArray.Add((geometry_section.RawVertices[vertex_index].PrimaryLightmapTexcoord.Y * -1) + 1);
			};

			// add surface index information
			geometry.Mesh.Triangles[0].P = new ColladaValueArray<int>();

			int strip_index = 0;
			for (int surface_index = 0; surface_index < geometry_section_info.TotalTriangleCount.Value; surface_index++)
			{
				ColladaValueArray<uint> poly = new ColladaValueArray<uint>();
				int k = 0;
				for (k = 0; k < 5; k++)
					geometry.Mesh.Triangles[0].P.Add((int)geometry_section.StripIndices[strip_index + 0].Index.Value);
				for (k = 0; k < 5; k++)
					geometry.Mesh.Triangles[0].P.Add((int)geometry_section.StripIndices[strip_index + 1].Index.Value);
				for (k = 0; k < 5; k++)
					geometry.Mesh.Triangles[0].P.Add((int)geometry_section.StripIndices[strip_index + 2].Index.Value);

				strip_index += 3;
			}

			geometry.Mesh.Source[0].TechniqueCommon.Accessor.SetCount(geometry.Mesh.Source[0].FloatArray.Count);
			geometry.Mesh.Source[1].TechniqueCommon.Accessor.SetCount(geometry.Mesh.Source[1].FloatArray.Count);
			geometry.Mesh.Source[2].TechniqueCommon.Accessor.SetCount(geometry.Mesh.Source[2].FloatArray.Count);
			geometry.Mesh.Source[3].TechniqueCommon.Accessor.SetCount(geometry.Mesh.Source[3].FloatArray.Count);
			geometry.Mesh.Source[4].TechniqueCommon.Accessor.SetCount(geometry.Mesh.Source[4].FloatArray.Count);
			geometry.Mesh.Source[5].TechniqueCommon.Accessor.SetCount(geometry.Mesh.Source[5].FloatArray.Count);
			geometry.Mesh.Source[6].TechniqueCommon.Accessor.SetCount(geometry.Mesh.Source[6].FloatArray.Count);

			return geometry;
		}
		/// <summary>
		/// Creates geometries for the relevant BSP meshes that are to be included in the collada file
		/// </summary>
		void CreateGeometryList()
		{
			H2.Tags.scenario_structure_lightmap_group definition = tagManager.TagDefinition as H2.Tags.scenario_structure_lightmap_group;

			for (int i = 0; i < definition.LightmapGroups.Count; i++)
			{
				for (int j = 0; j < definition.LightmapGroups[i].Clusters.Count; j++)
					listGeometry.Add(CreateRenderGeometry(i, j));
			}
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
			model_node.Add(instance_geometry);

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

			CreateGeometryList();
			CreateNodeList();

			AddLibraryGeometries();
			AddLibraryVisualScenes();
			AddScene("main");

			return true;
		}
	};
}