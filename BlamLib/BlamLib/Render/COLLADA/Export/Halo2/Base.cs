/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.IO;
using BlamLib.Blam;

using H2 = BlamLib.Blam.Halo2;

namespace BlamLib.Render.COLLADA.Halo2
{
	/// <summary>
	/// Base class for Halo2 exporters to derive from. All renderable meshes use the same storage format so can be handled
	/// in the base class. The same goes for shaders.
	/// </summary>
	public class ColladaModelExporterHalo2 : ColladaExporter
	{
		#region Class Members
		protected static readonly LowLevel.Math.real_vector3d RotationVectorY = new LowLevel.Math.real_vector3d(0, 1, 0);
		protected static readonly LowLevel.Math.real_vector3d RotationVectorP = new LowLevel.Math.real_vector3d(0, 0, 1);
		protected static readonly LowLevel.Math.real_vector3d RotationVectorR = new LowLevel.Math.real_vector3d(1, 0, 0);
		#endregion

		#region Constructor
		/// <summary>
		/// Base class constructor
		/// </summary>
		/// <param name="info">An object implementing IHaloShaderDatumList to provide a list of shader datums</param>
		/// <param name="tag_index">The tag index that contains the tag being exported</param>
		/// <param name="tag_manager">The tag manager of the tag being exported</param>
		public ColladaModelExporterHalo2(IColladaSettings settings, IHaloShaderDatumList info, Managers.TagIndexBase tag_index, Managers.TagManager tag_manager)
			: base(settings)
		{
		}
		#endregion

		#region Element Creation
		#region Create Geometry
		/// <summary>
		/// Creates a list of vertex indices for a Halo2 geometry section, assuming the data is in a triangle list
		/// </summary>
		/// <param name="section">The geometry section containing the requested part</param>
		/// <param name="part_index">The part index to create indices for</param>
		/// <returns></returns>
		protected List<int> CreateIndicesWorldSpace(H2.Tags.global_geometry_section_struct section, int part_index)
		{
			List<int> indices = new List<int>();

			// add the strip indices to an easier to handle index list, ignoring invalid indices
			List<int> index_list = new List<int>();
			{
				var part = section.Parts[part_index];
				for (int i = 0; i < part.StripLength; i++)
					if (section.StripIndices[part.StripStartIndex + i].Index.Value != -1)
						index_list.Add(section.StripIndices[part.StripStartIndex + i].Index.Value);
			}

			// add indices to the list, assuming each 3 indices is one triangle
			for (int surface_index = 0; surface_index < index_list.Count; surface_index += 3)
			{
				indices.Add((int)index_list[surface_index + 0]);
				indices.Add((int)index_list[surface_index + 1]);
				indices.Add((int)index_list[surface_index + 2]);
			}
			return indices;
		}
		/// <summary>
		/// Creates a list of vertex indices for a Halo2 geometry section, assuming the data is in a triangle strip
		/// </summary>
		/// <param name="section">The geometry section containing the requested part</param>
		/// <param name="part_index">The part index to create indices for</param>
		/// <returns></returns>
		protected List<int> CreateIndicesSkinned(H2.Tags.global_geometry_section_struct section, int part_index)
		{
			List<int> indices = new List<int>();

			// the strip indices for all of the parts are generated in one go and are all connected, so the strip start and length
			// variables have to be used, so that the useless faces connecting the parts are ignored

			// add the strip indices to an easier to handle index list, ignoring invalid indices
			List<int> index_list = new List<int>();
			for (int i = 0; i < section.Parts[part_index].StripLength; i++)
				if (section.StripIndices[section.Parts[part_index].StripStartIndex + i].Index.Value != -1)
					index_list.Add(section.StripIndices[section.Parts[part_index].StripStartIndex + i].Index.Value);

			// return the index list after converting it to a triangle list
			return ConvertTriStripToList(index_list);
		}
		/// <summary>
		/// Creates a geometry element for a Halo geometry section
		/// </summary>
		/// <param name="name">The name for the geometry</param>
		/// <param name="is_skinned">Controls whether the strip indices should be treated as a triangle strip or a triangle list</param>
		/// <param name="section">The geometry section to create an element for</param>
		/// <param name="shader_names">A string list containing all of the shader names</param>
		protected void CreateGeometryHalo2(string name, bool is_skinned, 
			H2.Tags.global_geometry_section_info_struct section_info, H2.Tags.global_geometry_section_struct section, 
			List<string> shader_names)
		{
			//List<Vertex> common_vertices = new List<Vertex>();
			//// create a generic list of vertices
			//foreach (var vertex in section.RawVertices)
			//{
			//    Vertex common_vertex = new Vertex(
			//        vertex.Point.Position.ToPoint3D(100),
			//        vertex.Normal.ToVector3D(),
			//        vertex.Binormal.ToVector3D(),
			//        vertex.Tangent.ToVector3D());

			//    common_vertex.AddTexcoord(new LowLevel.Math.real_point2d(
			//        vertex.Texcoord.X,
			//        (vertex.Texcoord.Y * -1) + 1));
			//    common_vertex.AddTexcoord(new LowLevel.Math.real_point2d(
			//        vertex.SecondaryTexcoord.X,
			//        (vertex.SecondaryTexcoord.Y * -1) + 1));
			//    common_vertex.AddTexcoord(new LowLevel.Math.real_point2d(
			//        vertex.PrimaryLightmapTexcoord.X,
			//        (vertex.PrimaryLightmapTexcoord.Y * -1) + 1));

			//    common_vertices.Add(common_vertex);
			//}

			//List<Part> commom_parts = new List<Part>();
			//// create the generic part definition list
			//for (int part_index = 0; part_index < section.Parts.Count; part_index++)
			//{
			//    string shader_name;
			//    if (shader_names.Count == 0)
			//        shader_name = "";
			//    else
			//        shader_name = shader_names[section.Parts[part_index].Material];

			//    Part common_part = new Part(shader_name);

			//    if(is_skinned)
			//        common_part.AddIndices(CreateIndicesSkinned(section, part_index));
			//    else
			//        common_part.AddIndices(CreateIndicesWorldSpace(section, part_index));

			//    commom_parts.Add(common_part);
			//}

			//// create the geometry element
			//CreateGeometry(name, 3,
			//    VertexComponent.POSITION | VertexComponent.NORMAL | VertexComponent.BINORMAL | VertexComponent.TANGENT | VertexComponent.TEXCOORD,
			//    common_vertices, commom_parts);
		}
		#endregion
		#region Create Effects
		/// <summary>
		/// Searches a shader for a specific parameter by name
		/// </summary>
		/// <param name="shader">The shader to search in</param>
		/// <param name="parameter_name">The name of the parameter to find</param>
		/// <returns></returns>
		H2.Tags.global_shader_parameter_block FindParameter(H2.Tags.shader_group shader, string parameter_name)
		{
			foreach (var block in shader.Parameters)
				if (block.Name.ToString().Equals(parameter_name))
					return block;
			return null;
		}
		/// <summary>
		///// Creates a phong definition using values from a shader tag
		///// </summary>
		///// <param name="shader_datum">DatumIndex of the shader to create a phong definition from</param>
		///// <returns></returns>
		//protected override Fx.ColladaPhong CreatePhong(DatumIndex shader_datum)
		//{
		//    Managers.TagManager shader_man = tagIndex[shader_datum];

		//    // get the shader tag definition
		//    H2.Tags.shader_group shader = GetTagDefinition<H2.Tags.shader_group>(
		//            shader_datum, shader_man.GroupTag, Blam.Halo1.TagGroups.shdr);

		//    Fx.ColladaPhong phong = CreateDefaultPhong();

		//    H2.Tags.global_shader_parameter_block block;

		//    phong.Diffuse.Color.SetColor(1, 1, 1, 1);
		//    phong.Shininess.Float.Value = 30.0f;

		//    // set up the effects variables with those from the shader
		//    if ((block = FindParameter(shader, "emissive_color")) != null)
		//    {
		//        phong.Emission = new Fx.ColladaCommonColorOrTextureType();
		//        phong.Emission.Color = new Core.ColladaColor(block.ConstColor, 1.0f);
		//    }

		//    if ((block = FindParameter(shader, "base_map")) != null && IsDatumValid(block.Bitmap.Datum))
		//    {
		//        phong.Diffuse.Color = null;
		//        phong.Diffuse.Texture = CreateTexture(block.Bitmap.Datum, 0);
		//    }
		//    else if ((block = FindParameter(shader, "diffuse_color")) != null)
		//        phong.Diffuse.Color.SetColor(block.ConstColor, 1.0f);

		//    if ((block = FindParameter(shader, "specular_color")) != null)
		//        phong.Specular.Color.SetColor(block.ConstColor, 1.0f);

		//    if ((block = FindParameter(shader, "alpha_test_map")) != null && IsDatumValid(block.Bitmap.Datum))
		//    {
		//        phong.Transparent.Color = null;
		//        phong.Transparent.Texture = CreateTexture(block.Bitmap.Datum, 0);
		//        phong.Transparent.Opaque = Enums.ColladaFXOpaqueEnum.A_ONE;
		//    }

		//    return phong;
		//}
		#endregion
		#endregion

		#region Data Collection
		///// <summary>
		///// Creates a DatumIndex list of the bitmaps used in a shader
		///// </summary>
		///// <param name="shader_datum"></param>
		///// <returns></returns>
		//protected override List<DatumIndex> GetShaderBitmaps(DatumIndex shader_datum)
		//{
		//    List<DatumIndex> bitmap_datums = new List<DatumIndex>();

		//    if (!IsDatumValid(shader_datum))
		//        return bitmap_datums;

		//    Managers.TagManager shader_man = tagIndex[shader_datum];

		//    H2.Tags.shader_group shader = shader_man.TagDefinition as H2.Tags.shader_group;

		//    H2.Tags.global_shader_parameter_block block;

		//    if ((block = FindParameter(shader, "base_map")) != null && IsDatumValid(block.Bitmap.Datum))
		//        bitmap_datums.Add(block.Bitmap.Datum);
		//    if ((block = FindParameter(shader, "alpha_test_map")) != null && IsDatumValid(block.Bitmap.Datum))
		//        bitmap_datums.Add(block.Bitmap.Datum);

		//    return bitmap_datums;
		//}
		#endregion
	};
}