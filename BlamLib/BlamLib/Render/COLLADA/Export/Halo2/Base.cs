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
using BlamLib.Blam;

using H2 = BlamLib.Blam.Halo2;

namespace BlamLib.Render.COLLADA.Halo2
{
	/// <summary>
	/// Base class for Halo1 exporters to derive from. All renderable meshes use the same storage so can be handled
	/// in the base class. The same goes for shaders.
	/// </summary>
	public class ColladaModelExporterHalo2 : ColladaModelExporterBase
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
		public ColladaModelExporterHalo2(IHaloShaderDatumList info, Managers.TagIndexBase tag_index, Managers.TagManager tag_manager)
			: base(info, tag_index, tag_manager)
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
		/// <param name="data_count">The number of components being referenced by the index list</param>
		/// <returns></returns>
		private List<int> CreateIndicesWorldSpace(H2.Tags.global_geometry_section_struct section, int part_index, int data_count)
		{
			List<int> indices = new List<int>();
			
			// add the strip indices to an easier to handle index list, ignoring invalid indices
			List<int> index_list = new List<int>();
			for (int i = 0; i < section.Parts[part_index].StripLength; i++)
				if(section.StripIndices[section.Parts[part_index].StripStartIndex + i].Index.Value != -1)
					index_list.Add(section.StripIndices[section.Parts[part_index].StripStartIndex + i].Index.Value);

			// add indices to the list, assuming each 3 strip indices is one triangle
			for (int surface_index = 0; surface_index < index_list.Count; surface_index += 3)
			{
				int k = 0;
				for (k = 0; k < data_count; k++)
					indices.Add((int)section.StripIndices[surface_index + 0].Index.Value);
				for (k = 0; k < data_count; k++)
					indices.Add((int)section.StripIndices[surface_index + 1].Index.Value);
				for (k = 0; k < data_count; k++)
					indices.Add((int)section.StripIndices[surface_index + 2].Index.Value);
			}
			return indices;
		}
		/// <summary>
		/// Creates a list of vertex indices for a Halo2 geometry section, assuming the data is in a triangle strip
		/// </summary>
		/// <param name="section">The geometry section containing the requested part</param>
		/// <param name="part_index">The part index to create indices for</param>
		/// <param name="data_count">The number of components being referenced by the index list</param>
		/// <returns></returns>
		private List<int> CreateIndicesSkinned(H2.Tags.global_geometry_section_struct section, int part_index, int data_count)
		{
			List<int> indices = new List<int>();

			// the strip indices for all of the parts are generated in one go and are all connected, so the strip start and length
			// variables have to be used, so that the useless faces connecting the parts are ignored

			// add the strip indices to an easier to handle index list, ignoring invalid indices
			List<int> index_list = new List<int>();
			for (int i = 0; i < section.Parts[part_index].StripLength; i++)
				if (section.StripIndices[section.Parts[part_index].StripStartIndex + i].Index.Value != -1)
					index_list.Add(section.StripIndices[section.Parts[part_index].StripStartIndex + i].Index.Value);

			// add indices to the list, assuming that after the first 3 strip indices, each index is another triangle
			// using itself and the previous two indices
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

				if (swap)
				{
					int temp = index2;
					index2 = index0;
					index0 = temp;
				}

				// add the final indices
				int k = 0;
				for (k = 0; k < data_count; k++)
					indices.Add(index0);
				for (k = 0; k < data_count; k++)
					indices.Add(index1);
				for (k = 0; k < data_count; k++)
					indices.Add(index2);
			}
			return indices;
		}
		/// <summary>
		/// Creates a geometry element for a Halo2 geometry section
		/// </summary>
		/// <param name="name">The name to assign to the geometry</param>
		/// <param name="section_info">The information struct associated with the geometry section</param>
		/// <param name="section">The geometry section to create an element from</param>
		/// <param name="shader_names">A string list containing the shader names used in the model</param>
		/// <returns></returns>
		protected Core.ColladaGeometry CreateGeometry(string name, H2.Tags.global_geometry_section_info_struct section_info,
			H2.Tags.global_geometry_section_struct section,
			List<string> shader_names)
		{
			Core.ColladaGeometry geometry = new Core.ColladaGeometry();

			// initialise the geometry attributes
			geometry.Name = ColladaUtilities.FormatName(name, " ", "_");
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

			// add vertex information to the source arrays
			for (int vertex_index = 0; vertex_index < section.RawVertices.Count; vertex_index++)
			{
				var vertex = section.RawVertices[vertex_index];

				//RealPoint3D   position
				geometry.Mesh.Source[0].FloatArray.Add(vertex.Point.Position.ToPoint3D(100));
				//RealVector3D  normal
				geometry.Mesh.Source[1].FloatArray.Add(vertex.Normal.ToVector3D());
				//RealVector3D  binormal
				geometry.Mesh.Source[2].FloatArray.Add(vertex.Binormal.ToVector3D());
				//RealVector3D  tangent
				geometry.Mesh.Source[3].FloatArray.Add(vertex.Tangent.ToVector3D());
				//RealPoint2D   texcoord0
				geometry.Mesh.Source[4].FloatArray.Add(vertex.Texcoord.X);
				geometry.Mesh.Source[4].FloatArray.Add((vertex.Texcoord.Y * -1) + 1);
				//RealPoint2D   texcoord1
				geometry.Mesh.Source[5].FloatArray.Add(vertex.SecondaryTexcoord.X);
				geometry.Mesh.Source[5].FloatArray.Add((vertex.SecondaryTexcoord.Y * -1) + 1);
				//RealPoint2D   texcoord2
				geometry.Mesh.Source[6].FloatArray.Add(vertex.PrimaryLightmapTexcoord.X);
				geometry.Mesh.Source[6].FloatArray.Add((vertex.PrimaryLightmapTexcoord.Y * -1) + 1);
			};

			geometry.Mesh.Triangles = new List<Core.ColladaTriangles>();
			// create triangle sets for the geometry parts
			for (int part_index = 0; part_index < section.Parts.Count; part_index++)
			{
				geometry.Mesh.Triangles.Add(new Core.ColladaTriangles());
				var part = geometry.Mesh.Triangles[part_index];

				string shader_name;

				if (shader_names.Count == 0)
					shader_name = "";
				else
					shader_name = ColladaUtilities.FormatName(shader_names[section.Parts[part_index].Material], " ", "_");

				part.Material = shader_name;
				part.Input = new List<Core.ColladaInputShared>();

				// link to data sources
				Core.ColladaInputShared input;

				input = new Core.ColladaInputShared();
				input.Semantic = Enums.ColladaInputSharedSemantic.VERTEX;
				input.Source = ColladaUtilities.BuildUri(geometry.Mesh.Vertices.ID);
				input.Offset = 0;
				part.Input.Add(input);

				input = new Core.ColladaInputShared();
				input.Semantic = Enums.ColladaInputSharedSemantic.NORMAL;
				input.Source = ColladaUtilities.BuildUri(geometry.Mesh.Source[1].ID);
				input.Offset = 1;
				part.Input.Add(input);

				input = new Core.ColladaInputShared();
				input.Semantic = Enums.ColladaInputSharedSemantic.BINORMAL;
				input.Source = ColladaUtilities.BuildUri(geometry.Mesh.Source[2].ID);
				input.Offset = 1;
				part.Input.Add(input);

				input = new Core.ColladaInputShared();
				input.Semantic = Enums.ColladaInputSharedSemantic.TANGENT;
				input.Source = ColladaUtilities.BuildUri(geometry.Mesh.Source[3].ID);
				input.Offset = 1;
				part.Input.Add(input);

				input = new Core.ColladaInputShared();
				input.Semantic = Enums.ColladaInputSharedSemantic.TEXCOORD;
				input.Source = ColladaUtilities.BuildUri(geometry.Mesh.Source[4].ID);
				input.Offset = 2;
				input.Set = 0;
				part.Input.Add(input);

				input = new Core.ColladaInputShared();
				input.Semantic = Enums.ColladaInputSharedSemantic.TEXCOORD;
				input.Source = ColladaUtilities.BuildUri(geometry.Mesh.Source[5].ID);
				input.Offset = 3;
				input.Set = 1;
				part.Input.Add(input);

				input = new Core.ColladaInputShared();
				input.Semantic = Enums.ColladaInputSharedSemantic.TEXCOORD;
				input.Source = ColladaUtilities.BuildUri(geometry.Mesh.Source[6].ID);
				input.Offset = 4;
				input.Set = 2;
				part.Input.Add(input);

				part.P = new ColladaValueArray<int>();
				// add surface index information
				switch (section_info.GeometryClassification)
				{
					case 0:
						{
							part.Count = (uint)section.Parts[part_index].StripLength / 3;
							part.P.Add(CreateIndicesWorldSpace(section, part_index, 5)); break;
						}
					default:
						{
							part.Count = (uint)section.Parts[part_index].StripLength - 2;
							part.P.Add(CreateIndicesSkinned(section, part_index, 5)); break;
						}
				}
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
		#endregion
		#region Create Effects
		/// <summary>
		/// Searches a shader parameters for a specific parameter
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
		/// Creates a phong definition using values from a shader tag
		/// </summary>
		/// <param name="shader_datum">DatumIndex of the shader to create a phong definition from</param>
		/// <returns></returns>
		protected Fx.ColladaPhong CreatePhong(DatumIndex shader_datum)
		{
			Managers.TagManager shader_man = tagIndex[shader_datum];

			// get the shader tag definition
			H2.Tags.shader_group shader = GetTagDefinition<H2.Tags.shader_group>(
					shader_datum, shader_man.GroupTag, Blam.Halo1.TagGroups.shdr);

			Fx.ColladaPhong phong = CreateDefaultPhong();

			H2.Tags.global_shader_parameter_block block;

			phong.Diffuse.Color.SetColor(1, 1, 1, 1);
			phong.Shininess.Float.Value = 30.0f;

			// set up the effects variables with those from the shader
			if ((block = FindParameter(shader, "emissive_color")) != null)
			{
				phong.Emission = new Fx.ColladaCommonColorOrTextureType();
				phong.Emission.Color = new Core.ColladaColor(block.ConstColor, 1.0f);
			}

			if (((block = FindParameter(shader, "base_map")) != null) && IsDatumValid(block.Bitmap.Datum))
			{
				phong.Diffuse.Color = null;
				phong.Diffuse.Texture = CreateTexture(block.Bitmap.Datum, 0);
			}
			else if ((block = FindParameter(shader, "diffuse_color")) != null)
				phong.Diffuse.Color.SetColor(block.ConstColor, 1.0f);

			if ((block = FindParameter(shader, "specular_color")) != null)
				phong.Specular.Color.SetColor(block.ConstColor, 1.0f);

			if (((block = FindParameter(shader, "alpha_test_map")) != null) && IsDatumValid(block.Bitmap.Datum))
			{
				phong.Transparent.Color = null;
				phong.Transparent.Texture = CreateTexture(block.Bitmap.Datum, 0);
				phong.Transparent.Opaque = Enums.ColladaFXOpaqueEnum.A_ONE;
			}

			return phong;
		}
		/// <summary>
		/// Creates an effect element from a shader tag
		/// </summary>
		/// <param name="shader_datum">DatumIndex of the shader to create an effect from</param>
		/// <returns></returns>
		protected Fx.ColladaEffect CreateEffect(DatumIndex shader_datum)
		{
			Managers.TagManager shader_man = tagIndex[shader_datum];

			Fx.ColladaEffect effect = new Fx.ColladaEffect();

			string shader_name = ColladaUtilities.FormatName(System.IO.Path.GetFileNameWithoutExtension(shader_man.Name), " ", "_");
			effect.Name = shader_name;
			effect.ID = shader_name;
			effect.ProfileCOMMON = new List<COLLADA.Fx.ColladaProfileCOMMON>();
			effect.ProfileCOMMON.Add(new COLLADA.Fx.ColladaProfileCOMMON());
			effect.ProfileCOMMON[0].Technique = new COLLADA.Fx.ColladaTechnique();
			effect.ProfileCOMMON[0].Technique.sID = "common";
			effect.ProfileCOMMON[0].Technique.Phong = CreatePhong(shader_datum);
			effect.ProfileCOMMON[0].Newparam = new List<COLLADA.Fx.ColladaNewparam>();

			List<DatumIndex> bitmap_datums = GetShaderBitmaps(shader_datum);
			// add surface, sampler pairs for all of the bitmaps used in the shader
			for (int i = 0; i < bitmap_datums.Count; i++)
			{
				BlamLib.Managers.TagManager bitmap = tagIndex[bitmap_datums[i]];
				string bitmap_name = ColladaUtilities.FormatName(System.IO.Path.GetFileNameWithoutExtension(bitmap.Name), " ", "_");

				COLLADA.Fx.ColladaNewparam newparam_surface = new COLLADA.Fx.ColladaNewparam();
				COLLADA.Fx.ColladaNewparam newparam_sampler = new COLLADA.Fx.ColladaNewparam();

				newparam_surface.sID = String.Concat(bitmap_name, "-surface");
				newparam_sampler.sID = String.Concat(bitmap_name, "-surface-sampler");

				COLLADA.Fx.ColladaSurface surface = new COLLADA.Fx.ColladaSurface();
				COLLADA.Fx.ColladaSampler2D sampler2d = new COLLADA.Fx.ColladaSampler2D();

				surface.Type = COLLADA.Enums.ColladaFXSurfaceTypeEnum._2D;
				surface.InitFrom = new COLLADA.Fx.ColladaInitFrom();
				surface.InitFrom.Text = bitmap_name;

				sampler2d.Source = newparam_surface.sID;

				newparam_surface.Surface = surface;
				newparam_sampler.Sampler2D = sampler2d;

				effect.ProfileCOMMON[0].Newparam.Add(newparam_surface);
				effect.ProfileCOMMON[0].Newparam.Add(newparam_sampler);
			}

			return effect;
		}
		/// <summary>
		/// Populate the effect list with the shaders used in the model
		/// </summary>
		protected void CreateEffectList()
		{
			int effect_count = shaderInfo.GetShaderCount();
			// for each shader, create a new effect
			for (int i = 0; i < effect_count; i++)
			{
				DatumIndex shader_datum = shaderInfo.GetShaderDatum(i);
				if (!IsDatumValid(shader_datum))
				{
					OnErrorOccured(String.Format(ColladaExceptionStrings.InvalidDatumIndex, shader_datum.ToString(), "unknown"));
					continue;
				}

				listEffect.Add(CreateEffect(shader_datum));
			}
		}
		#endregion
		#endregion

		#region Data Collection
		protected List<DatumIndex> GetShaderBitmaps(DatumIndex shader_datum)
		{
			Managers.TagManager shader_man = tagIndex[shader_datum];

			H2.Tags.shader_group shader = shader_man.TagDefinition as H2.Tags.shader_group;

			List<DatumIndex> bitmap_datums = new List<DatumIndex>();

			H2.Tags.global_shader_parameter_block block;

			if (((block = FindParameter(shader, "base_map")) != null) && IsDatumValid(block.Bitmap.Datum))
				bitmap_datums.Add(block.Bitmap.Datum);
			if (((block = FindParameter(shader, "alpha_test_map")) != null) && IsDatumValid(block.Bitmap.Datum))
				bitmap_datums.Add(block.Bitmap.Datum);
			//else if (((block = FindParameter(shader, "bump_map")) != null) && IsDatumValid(block.Bitmap.Datum))
			//    bitmap_datums.Add(block.Bitmap.Datum);

			return bitmap_datums;
		}
		#endregion
	};
}