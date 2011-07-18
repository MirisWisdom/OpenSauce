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
using System.Xml;
using System.Xml.Serialization;
using System.IO;
using BlamLib.Blam;
using BlamLib.Render.COLLADA.Validation;

namespace BlamLib.Render.COLLADA
{
	public class ColladaExportArgs
	{
		public bool Overwrite { get; private set; }
		public string RelativeDataPath { get; private set; }
		public string BitmapFormatString { get; private set; }

		/// <summary>
		/// Argument class constructor
		/// </summary>
		/// <param name="overwrite">When true, existing files are overwritten when saving</param>
		/// <param name="data_path">The absolute path that relative paths should start at</param>
		/// <param name="bitmap_format">The bitmap extension the materials should use (tga, jpg, tif, etc)</param>
		public ColladaExportArgs(bool overwrite, string data_path, string bitmap_format)
		{
			// make sure the data path ends with a '\'
			if (!data_path.EndsWith("\\"))
				data_path += "\\";

			Overwrite = overwrite;
			RelativeDataPath = data_path;
			BitmapFormatString = bitmap_format;
		}
	}
	/// <summary>
	/// Base class for exporting a COLLADA file
	/// </summary>
	public abstract class ColladaExporter
	{
		#region Events
		/// <summary>
		/// This event is fired when an error has occured, with an error string describing the error in detail.
		/// </summary>
		public event EventHandler<ColladaErrorEventArgs> ErrorOccured;
		
		protected void OnErrorOccured(string message)
		{
			if (ErrorOccured != null)
				ErrorOccured(this, new ColladaErrorEventArgs(message));
		}
		#endregion

		#region Class Members
		protected ColladaExportArgs exportArguments;
		protected ColladaFile COLLADAFile = new ColladaFile();

		protected Managers.TagIndexBase tagIndex;
		protected Managers.TagManager tagManager;
		protected string tagName;
		#endregion

		protected ColladaExporter(ColladaExportArgs arguments, Managers.TagIndexBase tag_index, Managers.TagManager tag_manager)
		{
			exportArguments = arguments;
			tagIndex = tag_index;
			tagManager = tag_manager;
			tagName = System.IO.Path.GetFileNameWithoutExtension(tagManager.Name);
		}

		#region Element Creation
		#region Create Effect
		/// <summary>
		/// Creates an effect element from a shader tag
		/// </summary>
		/// <param name="shader_datum">DatumIndex of the shader to create an effect from</param>
		/// <returns></returns>
		protected Fx.ColladaEffect CreateEffect(string effect_id, Fx.ColladaPhong phong, List<string> bitmaps)
		{
			Fx.ColladaEffect effect = new Fx.ColladaEffect();

			string shader_name = ColladaUtilities.FormatName(effect_id, " ", "_");
			effect.ID = ColladaElement.FormatID<Fx.ColladaEffect>(shader_name);
			effect.Name = shader_name;
			effect.ProfileCOMMON = new List<COLLADA.Fx.ColladaProfileCOMMON>();
			effect.ProfileCOMMON.Add(new COLLADA.Fx.ColladaProfileCOMMON());
			effect.ProfileCOMMON[0].Technique = new COLLADA.Fx.ColladaTechnique();
			effect.ProfileCOMMON[0].Technique.sID = "common";
			effect.ProfileCOMMON[0].Technique.Phong = phong;
			effect.ProfileCOMMON[0].Newparam = new List<COLLADA.Fx.ColladaNewparam>();

			for (int i = 0; i < bitmaps.Count; i++)
			{
				string bitmap_name = ColladaUtilities.FormatName(System.IO.Path.GetFileNameWithoutExtension(bitmaps[i]), " ", "_");

				COLLADA.Fx.ColladaNewparam newparam_surface = new COLLADA.Fx.ColladaNewparam();
				COLLADA.Fx.ColladaNewparam newparam_sampler = new COLLADA.Fx.ColladaNewparam();

				newparam_surface.sID = String.Concat(bitmap_name, "-surface");
				newparam_sampler.sID = String.Concat(bitmap_name, "-surface-sampler");

				COLLADA.Fx.ColladaSurface surface = new COLLADA.Fx.ColladaSurface();
				COLLADA.Fx.ColladaSampler2D sampler2d = new COLLADA.Fx.ColladaSampler2D();

				surface.Type = COLLADA.Enums.ColladaFXSurfaceTypeEnum._2D;
				surface.InitFrom = new COLLADA.Fx.ColladaInitFrom();
				surface.InitFrom.Text = ColladaElement.FormatID<Fx.ColladaImage>(bitmap_name);

				sampler2d.Source = newparam_surface.sID;

				newparam_surface.Surface = surface;
				newparam_sampler.Sampler2D = sampler2d;

				effect.ProfileCOMMON[0].Newparam.Add(newparam_surface);
				effect.ProfileCOMMON[0].Newparam.Add(newparam_sampler);
			}

			return effect;
		}
		#endregion
		/// <summary>
		/// Adds an asset element to the current collada file
		/// </summary>
		/// <param name="author">String containing the authors name</param>
		/// <param name="authoring_tool">String containing the name of the program that created the file</param>
		/// <param name="unit_name">The name of the units the scene is scaled against</param>
		/// <param name="unit_meter_scale">The ratio of the units in relation to metres</param>
		/// <param name="up_axis">The axis that is considered to be "up"</param>
		public void AddAsset(string author,
			string authoring_tool,
			string unit_name,
			double unit_meter_scale,
			Enums.ColladaUpAxisEnum up_axis)
		{
			COLLADAFile.Asset = new COLLADA.Core.ColladaAsset();
			COLLADAFile.Asset.Contributor = new List<COLLADA.Core.ColladaContributor>();
			COLLADAFile.Asset.Contributor.Add(new COLLADA.Core.ColladaContributor());
			COLLADAFile.Asset.Contributor[0].Author = author;
			COLLADAFile.Asset.Contributor[0].AuthoringTool = authoring_tool;

			COLLADAFile.Asset.Created = DateTime.Now;
			COLLADAFile.Asset.Modified = DateTime.Now;
			COLLADAFile.Asset.Unit = new Core.ColladaAssetUnit();
			COLLADAFile.Asset.Unit.Meter = unit_meter_scale;
			COLLADAFile.Asset.Unit.Name = unit_name;
			COLLADAFile.Asset.UpAxis = up_axis;
		}
		/// <summary>
		/// Creates a generic source element for geometric data
		/// </summary>
		/// <param name="semantic">The use for the data in the source element</param>
		/// <param name="geometry_id">Name of the geometry the element contains data for</param>
		/// <param name="set">Data set that the sources data is intended for</param>
		/// <returns>A ColladaSource element</returns>
		protected Core.ColladaSource CreateSource(Enums.ColladaInputSharedSemantic semantic,
			string geometry_id,
			uint set)
		{
			Core.ColladaSource source = new Core.ColladaSource();

			source.FloatArray = new Core.ColladaFloatArray();
			source.TechniqueCommon = new Core.ColladaTechniqueCommon();
			source.TechniqueCommon.Accessor = new Core.ColladaAccessor();
			source.TechniqueCommon.Accessor.Param = new List<Core.ColladaParam>();

			string source_id = "";
			switch (semantic)
			{
				case Enums.ColladaInputSharedSemantic.POSITION:
					source_id = String.Concat(geometry_id, "-positions");
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param[0].Name = "X";
					source.TechniqueCommon.Accessor.Param[1].Name = "Y";
					source.TechniqueCommon.Accessor.Param[2].Name = "Z";
					break;
				case Enums.ColladaInputSharedSemantic.NORMAL:
					source_id = String.Concat(geometry_id, "-normals");
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param[0].Name = "X";
					source.TechniqueCommon.Accessor.Param[1].Name = "Y";
					source.TechniqueCommon.Accessor.Param[2].Name = "Z";
					break;
				case Enums.ColladaInputSharedSemantic.BINORMAL:
					source_id = String.Concat(geometry_id, "-binormals");
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param[0].Name = "X";
					source.TechniqueCommon.Accessor.Param[1].Name = "Y";
					source.TechniqueCommon.Accessor.Param[2].Name = "Z";
					break;
				case Enums.ColladaInputSharedSemantic.TANGENT:
					source_id = String.Concat(geometry_id, "-tangents");
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param[0].Name = "X";
					source.TechniqueCommon.Accessor.Param[1].Name = "Y";
					source.TechniqueCommon.Accessor.Param[2].Name = "Z";
					break;
				case Enums.ColladaInputSharedSemantic.TEXCOORD:
					source_id = String.Format("{0}-texcoord{1}", geometry_id, set);
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param.Add(new Core.ColladaParam());
					source.TechniqueCommon.Accessor.Param[0].Name = "S";
					source.TechniqueCommon.Accessor.Param[1].Name = "T";
					break;
				default:
					throw new ColladaException(String.Format(
						ColladaExceptionStrings.ImplimentationBug,
						"invalid semantic enum passed to CreateSource function"));
			}
			source.ID = source_id;
			source.FloatArray.ID = String.Concat(source.ID, "-array");
			source.TechniqueCommon.Accessor.Source = ColladaUtilities.BuildUri(source.FloatArray.ID);

			foreach (Core.ColladaParam param in source.TechniqueCommon.Accessor.Param)
				param.Type = "float";

			return source;
		}
		/// <summary>
		/// Creates a vertices element that defines which source element contains the position data
		/// </summary>
		/// <param name="geometry_id">The id of the geometry the element is associated with</param>
		/// <param name="source_id">The id of the source element that contains the position data</param>
		/// <returns>A ColladaVertices element</returns>
		protected Core.ColladaVertices CreateVertices(string geometry_id, string source_id)
		{
			Core.ColladaVertices vertices = new Core.ColladaVertices();
			vertices.ID = String.Concat(geometry_id, "-vertices");
			vertices.Input = new List<Core.ColladaInputUnshared>();
			vertices.Input.Add(new Core.ColladaInputUnshared());
			vertices.Input[0].Semantic = Enums.ColladaInputSharedSemantic.POSITION;
			vertices.Input[0].Source = ColladaUtilities.BuildUri(source_id);

			return vertices;
		}
		/// <summary>
		/// Creates a polygon index list for a set of planar, convex vertices
		/// </summary>
		/// <param name="vertex_count">Number of vertices in the plane</param>
		/// <returns></returns>
		protected List<int> BuildFaceIndices(int vertex_count)
		{
			// create an array of vertex indices
			List<ushort> index_array = new List<ushort>();
			for (ushort j = 0; j < vertex_count; j++) { index_array.Add(j); }

			// in a triangle list without degenerates face count = vertex count - 2
			int vertex_index = 0;
			int surface_count = vertex_count - 2;

			// the triangle data is calculated by assuming the surface is coplanar and convex,
			// which means we can make the triangles by using 3 consecutive indices, removing the
			// middle index and repeating until only 3 indices are left, which makes the final triangle

			List<int> indices = new List<int>();
			for (int i = 0; i < surface_count; i++)
			{
				// create a triangle
				indices.Add(index_array[vertex_index + 2]);
				indices.Add(index_array[vertex_index + 1]);
				indices.Add(index_array[vertex_index + 0]);

				index_array.RemoveAt(vertex_index + 1);
				vertex_index++;

				if (vertex_index + 2 >= index_array.Count)
					vertex_index = 0;
			}
			return indices;
		}
		#endregion

		#region Scene Creation
		/// <summary>
		/// Adds a scene to the collada file that instances a visual scene with the specified ID
		/// </summary>
		protected void AddScene(string scene_id)
		{
			COLLADAFile.Scene = new Core.ColladaScene();
			COLLADAFile.Scene.InstanceVisualScene = new Core.ColladaInstanceVisualScene();
			COLLADAFile.Scene.InstanceVisualScene.URL = ColladaUtilities.BuildUri(ColladaElement.FormatID<Core.ColladaVisualScene>(scene_id));
		}
		#endregion

		/// <summary>
		/// Saves the Collada file in its current state to file.
		/// </summary>
		/// <param name="location">Location to save the Collada file to</param>
		public void SaveDAE(string location)
		{
			// if the file exists but overwriting is disabled, report this and return
			if (System.IO.File.Exists(location) && !exportArguments.Overwrite)
			{
				OnErrorOccured(String.Format(ColladaExceptionStrings.FileExists, location));
				return;
			}

			// serialize the COLLADA file to the xml file
			System.IO.Directory.CreateDirectory(System.IO.Path.GetDirectoryName(location));

			XmlSerializer serializer = new XmlSerializer(typeof(ColladaFile));
			using (var writer = new XmlTextWriter(location, null))
			{
				writer.Formatting = Formatting.Indented;
				serializer.Serialize(writer, COLLADAFile);
			}
		}
		/// <summary>
		/// Performs the actual collada file population. Override this in derived classes to define how this is done.
		/// </summary>
		/// <returns>True if no errors occurred</returns>
		protected virtual bool BuildColladaInstanceImpl() { return true; }
		/// <summary>
		/// Populates the collada file object with the current data set
		/// </summary>
		/// <returns>True if no errors occurred</returns>
		public bool BuildColladaInstance()
		{
			bool success = false;
			try
			{
				if (BuildColladaInstanceImpl())
				{
					var validator = new ColladaFileValidator();

					validator.ErrorOccured += new EventHandler<ColladaErrorEventArgs>(ValidatorErrorOccured);
					success = validator.ValidateFile(COLLADAFile);
					validator.ErrorOccured -= new EventHandler<ColladaErrorEventArgs>(ValidatorErrorOccured);
				}
			}
			catch (Exception e)
			{
				// if an exception occurred, report it and return gracefully
				OnErrorOccured(e.Message);
				OnErrorOccured(e.StackTrace);

				for (var except = e.InnerException; except != null; except = except.InnerException)
					OnErrorOccured(except.Message);
			}
			return success;
		}

		void ValidatorErrorOccured(object sender, ColladaErrorEventArgs e)
		{
			OnErrorOccured(e.ErrorMessage);
		}

		#region Helpers
		/// <summary>
		/// Determines whether a DatumIndex is valid
		/// </summary>
		/// <param name="index">The index to validate</param>
		/// <returns>Returns true if the DatumIndex is neither null, nor sentinel</returns>
		protected static bool IsDatumValid(DatumIndex index)
		{
			return (!index.Equals(DatumIndex.Null) && !Managers.TagIndex.IsSentinel(index));
		}

		/// <summary>
		/// Gets a tag definition instance from the tag index
		/// </summary>
		/// <typeparam name="T">Tag definition type to return</typeparam>
		/// <param name="tag_datum">Datum index of the tag</param>
		/// <param name="cast_from_group">The tag group of the tag</param>
		/// <param name="cast_to_group">The tag group of the definition we are casting to</param>
		/// <returns></returns>
		protected T GetTagDefinition<T>(DatumIndex tag_datum,
			TagInterface.TagGroup cast_from_group,
			TagInterface.TagGroup cast_to_group)
			where T : TagInterface.Definition
		{
			// report if the datum index is invalid
			if (!IsDatumValid(tag_datum))
			{
				OnErrorOccured(String.Format(ColladaExceptionStrings.InvalidDatumIndex,
					tag_datum.ToString()));
				return null;
			}

			// attempt to cast the definition to the specified type
			T tag = tagIndex[tag_datum].TagDefinition as T;
			// if the cast failed, throw an exception
			if (tag == null)
				throw new ColladaException(String.Format(
					ColladaExceptionStrings.InvalidDefinitionCast,
					tagIndex[tag_datum].Name,
					cast_from_group.ToString(), cast_to_group.ToString()));

			return tag;
		}
		#endregion
	};

	public abstract class ColladaModelExporterBase : ColladaExporter
	{
		[Flags]
		protected enum VertexComponent
		{
			POSITION,
			NORMAL,
			BINORMAL,
			TANGENT,
			TEXCOORD,
			WEIGHT
		}

		#region Class Members
		protected IHaloShaderDatumList shaderInfo;

		protected List<DatumIndex> bitmapDatums = new List<DatumIndex>();

		protected List<Core.ColladaGeometry> listGeometry = new List<Core.ColladaGeometry>();
		protected List<Core.ColladaController> listController = new List<Core.ColladaController>();
		protected List<Fx.ColladaImage> listImage = new List<Fx.ColladaImage>();
		protected List<Fx.ColladaEffect> listEffect = new List<Fx.ColladaEffect>();
		protected List<Fx.ColladaMaterial> listMaterial = new List<Fx.ColladaMaterial>();
		protected List<Core.ColladaNode> listBone = new List<Core.ColladaNode>();
		protected List<ColladaBoneMatrix> listBoneMatrix = new List<ColladaBoneMatrix>();
		protected List<Core.ColladaNode> listNode = new List<Core.ColladaNode>();
		protected List<Core.ColladaNode> listMarker = new List<Core.ColladaNode>();
		#endregion

		#region Constructor
		protected ColladaModelExporterBase(ColladaExportArgs arguments, IHaloShaderDatumList info, Managers.TagIndexBase tag_index, Managers.TagManager tag_manager) :
			base(arguments, tag_index, tag_manager)
		{
			shaderInfo = info;
		}
		#endregion

		#region Element Creation
		#region Create Geometry
		/// <summary>
		/// Generic vertex information class
		/// </summary>
		protected class Vertex
		{
			public LowLevel.Math.real_point3d Position { get; private set; }
			public LowLevel.Math.real_vector3d Normal { get; private set; }
			public LowLevel.Math.real_vector3d Binormal { get; private set; }
			public LowLevel.Math.real_vector3d Tangent { get; private set; }
			public List<LowLevel.Math.real_point2d> Texcoords { get; private set; }

			public void AddTexcoord(LowLevel.Math.real_point2d texcoord)
			{
				if (Texcoords == null)
					Texcoords = new List<LowLevel.Math.real_point2d>();

				Texcoords.Add(texcoord);
			}

			public Vertex(LowLevel.Math.real_point3d position, LowLevel.Math.real_vector3d normal, LowLevel.Math.real_vector3d binormal,
				LowLevel.Math.real_vector3d tangent)
			{
				Position = position;
				Normal = normal;
				Binormal = binormal;
				Tangent = tangent;
			}
			public Vertex(LowLevel.Math.real_point3d position)
			{
				Position = position;
			}
		}
		/// <summary>
		/// Generic part information class
		/// </summary>
		protected class Part
		{
			public string MaterialName { get; private set; }
			public List<int> Indices { get; private set; }

			public void AddIndices(params int[] indices)
			{
				if (Indices == null)
					Indices = new List<int>();

				Indices.AddRange(indices);
			}
			public void AddIndices(List<int> indices)
			{
				if (Indices == null)
					Indices = new List<int>();

				Indices.AddRange(indices);
			}

			public Part(string material)
			{
				MaterialName = material;
			}
		}
		/// <summary>
		/// Takes an triangle strip index list and converts it into a triangle list
		/// </summary>
		/// <param name="tri_strip">The triangle strip to convert</param>
		/// <returns></returns>
		protected List<int> ConvertTriStripToList(List<int> tri_strip)
		{
			List<int> tri_list = new List<int>();
			// add indices to the list, assuming that after the first 3 strip indices, each index is another triangle
			// using itself and the previous two indices to make up the face
			for (int i = 0; i < tri_strip.Count - 2; i++)
			{
				int index0;
				int index1;
				int index2;

				index0 = tri_strip[i];
				index1 = tri_strip[i + 1];
				index2 = tri_strip[i + 2];

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
				tri_list.Add(index0);
				tri_list.Add(index1);
				tri_list.Add(index2);
			}
			return tri_list;
		}
		/// <summary>
		/// Creates a geometry element
		/// </summary>
		/// <param name="name">The name of the geometry</param>
		/// <param name="texcoord_count">The number of texture coordinates to use</param>
		/// <param name="component_mask">Defines which components to use from the vertex data</param>
		/// <param name="vertices">A list of vertices</param>
		/// <param name="parts">A list of parts</param>
		protected void CreateGeometry(string name, uint texcoord_count, VertexComponent component_mask, List<Vertex> vertices, List<Part> parts)
		{
			Core.ColladaGeometry geometry = new Core.ColladaGeometry();

			// initialise the geometry attributes
			geometry.Name = ColladaUtilities.FormatName(name, " ", "_");
			geometry.ID = ColladaElement.FormatID<Core.ColladaGeometry>(geometry.Name);

			// create a new mesh element
			geometry.Mesh = new Core.ColladaMesh();
			geometry.Mesh.Source = new List<Core.ColladaSource>();

			Core.ColladaSource position_source = null;
			Core.ColladaSource normal_source = null;
			Core.ColladaSource binormal_source = null;
			Core.ColladaSource tangent_source = null;
			List<Core.ColladaSource> texcoord_sources = null;

			// create the source elements according to the component mask
			if ((component_mask & VertexComponent.POSITION) == VertexComponent.POSITION)
			{
				position_source = CreateSource(Enums.ColladaInputSharedSemantic.POSITION, geometry.ID, 0);
				geometry.Mesh.Vertices = CreateVertices(geometry.ID, position_source.ID);
			}
			if ((component_mask & VertexComponent.NORMAL) == VertexComponent.NORMAL)
				normal_source = CreateSource(Enums.ColladaInputSharedSemantic.NORMAL, geometry.ID, 0);
			if ((component_mask & VertexComponent.BINORMAL) == VertexComponent.BINORMAL)
				binormal_source = CreateSource(Enums.ColladaInputSharedSemantic.BINORMAL, geometry.ID, 0);
			if ((component_mask & VertexComponent.TANGENT) == VertexComponent.TANGENT)
				tangent_source = CreateSource(Enums.ColladaInputSharedSemantic.TANGENT, geometry.ID, 0);

			if (((component_mask & VertexComponent.TEXCOORD) == VertexComponent.TEXCOORD) && (texcoord_count > 0))
			{
				texcoord_sources = new List<Core.ColladaSource>();
				for (uint i = 0; i < texcoord_count; i++)
					texcoord_sources.Add(CreateSource(Enums.ColladaInputSharedSemantic.TEXCOORD, geometry.ID, i));
			}

			// add vertex information to the source arrays
			for (int vertex_index = 0; vertex_index < vertices.Count; vertex_index++)
			{
				var vertex = vertices[vertex_index];

				//RealPoint3D   position
				if (position_source != null)
					position_source.FloatArray.Add(vertex.Position);
				//RealVector3D  normal
				if (normal_source != null)
					normal_source.FloatArray.Add(vertex.Normal);
				//RealVector3D  binormal
				if (binormal_source != null)
					binormal_source.FloatArray.Add(vertex.Binormal);
				//RealVector3D  tangent
				if (tangent_source != null)
					binormal_source.FloatArray.Add(vertex.Tangent);
				//RealPoint2D  texcoords
				if (texcoord_sources != null)
				{
					for (int i = 0; i < texcoord_count; i++)
					{
						if (i >= vertex.Texcoords.Count)
							texcoord_sources[i].FloatArray.Add(0, 1);
						else
							texcoord_sources[i].FloatArray.Add(vertex.Texcoords[i].X, vertex.Texcoords[i].Y);
					}
				}
			};

			// create triangle sets for the geometry parts
			geometry.Mesh.Triangles = new List<Core.ColladaTriangles>();
			for (int part_index = 0; part_index < parts.Count; part_index++)
			{
				Core.ColladaTriangles part_triangles = new Core.ColladaTriangles();
				part_triangles.Material = ColladaUtilities.FormatName(parts[part_index].MaterialName, " ", "_");
				part_triangles.Input = new List<Core.ColladaInputShared>();

				// link to data sources
				Core.ColladaInputShared input;

				if (position_source != null)
				{
					input = new Core.ColladaInputShared();
					input.Semantic = Enums.ColladaInputSharedSemantic.VERTEX;
					input.Source = ColladaUtilities.BuildUri(geometry.Mesh.Vertices.ID);
					input.Offset = 0;
					part_triangles.Input.Add(input);
				}

				if (normal_source != null)
				{
					input = new Core.ColladaInputShared();
					input.Semantic = Enums.ColladaInputSharedSemantic.NORMAL;
					input.Source = ColladaUtilities.BuildUri(normal_source.ID);
					input.Offset = 0;
					part_triangles.Input.Add(input);
				}

				if (binormal_source != null)
				{
					input = new Core.ColladaInputShared();
					input.Semantic = Enums.ColladaInputSharedSemantic.BINORMAL;
					input.Source = ColladaUtilities.BuildUri(binormal_source.ID);
					input.Offset = 0;
					part_triangles.Input.Add(input);
				}

				if (tangent_source != null)
				{
					input = new Core.ColladaInputShared();
					input.Semantic = Enums.ColladaInputSharedSemantic.TANGENT;
					input.Source = ColladaUtilities.BuildUri(tangent_source.ID);
					input.Offset = 0;
					part_triangles.Input.Add(input);
				}

				if (texcoord_sources != null)
				{
					for (int i = 0; i < texcoord_count; i++)
					{
						input = new Core.ColladaInputShared();
						input.Semantic = Enums.ColladaInputSharedSemantic.TEXCOORD;
						input.Source = ColladaUtilities.BuildUri(texcoord_sources[i].ID);
						input.Offset = 0;
						input.Set = (uint)i;
						part_triangles.Input.Add(input);
					}
				}

				part_triangles.P = new ColladaValueArray<int>();

				part_triangles.Count = (uint)parts[part_index].Indices.Count / 3;
				part_triangles.P.Add(parts[part_index].Indices);

				geometry.Mesh.Triangles.Add(part_triangles);
			}

			// update the source data counts and add them to the mesh element
			if (position_source != null)
			{
				position_source.TechniqueCommon.Accessor.SetCount(position_source.FloatArray.Count);
				geometry.Mesh.Source.Add(position_source);
			}
			if (normal_source != null)
			{
				normal_source.TechniqueCommon.Accessor.SetCount(normal_source.FloatArray.Count);
				geometry.Mesh.Source.Add(normal_source);
			}
			if (binormal_source != null)
			{
				binormal_source.TechniqueCommon.Accessor.SetCount(binormal_source.FloatArray.Count);
				geometry.Mesh.Source.Add(binormal_source);
			}
			if (tangent_source != null)
			{
				tangent_source.TechniqueCommon.Accessor.SetCount(tangent_source.FloatArray.Count);
				geometry.Mesh.Source.Add(tangent_source);
			}

			if (texcoord_sources != null)
			{
				foreach (var source in texcoord_sources)
					source.TechniqueCommon.Accessor.SetCount(source.FloatArray.Count);
				geometry.Mesh.Source.AddRange(texcoord_sources);
			}

			// add the complete geometry to the list
			listGeometry.Add(geometry);
		}
		#endregion
		#region Create Controllers
		/// <summary>
		/// Genertic vertex weight information class
		/// </summary>
		protected class VertexWeight
		{
			public class Weight
			{
				public int BoneIndex { get; private set; }
				public float BoneWeight { get; private set; }

				public Weight(int bone_index, float bone_weight)
				{
					BoneIndex = bone_index;
					BoneWeight = bone_weight;
				}
			}

			public List<Weight> Weights { get; private set; }

			public VertexWeight() { Weights = new List<Weight>(); }

			public void AddWeight(int bone_index, float bone_weight)
			{
				Weights.Add(new Weight(bone_index, bone_weight));
			}
		}
		/// <summary>
		/// Creates a common technique element with one parameter
		/// </summary>
		/// <param name="source_id">The ID of the source element</param>
		/// <param name="source_count">The number of elements in the source array</param>
		/// <param name="param_name">The name of the parameter</param>
		/// <param name="param_type">The type of the the parameter</param>
		/// <param name="stride">the number of elements the parameter takes up (usually 1)</param>
		/// <returns></returns>
		protected Core.ColladaTechniqueCommon CreateControllerTechniqueCommon(string source_id,
			uint source_count, string param_name, string param_type, uint stride)
		{
			Core.ColladaTechniqueCommon technique = new Core.ColladaTechniqueCommon();
			technique.Accessor = new Core.ColladaAccessor();
			technique.Accessor.Source = ColladaUtilities.BuildUri(source_id);
			technique.Accessor.Count = source_count;
			technique.Accessor.Param = new List<Core.ColladaParam>();
			technique.Accessor.Param.Add(new Core.ColladaParam());

			if (stride != 1)
				technique.Accessor.StrideOverride = stride;

			technique.Accessor.Param[0].Name = param_name;
			technique.Accessor.Param[0].Type = param_type;

			return technique;
		}
		/// <summary>
		/// Creates a skin controller from a generic set of vertex weights.
		/// 
		/// This must only be called after the bones have been set up.
		/// </summary>
		/// <param name="name">The geometry name</param>
		/// <param name="vertex_weights">List of generic vertex weights</param>
		protected void CreateSkinController(string name, List<VertexWeight> vertex_weights)
		{
			// create the controller node
			Core.ColladaController controller = new BlamLib.Render.COLLADA.Core.ColladaController();

			// set the controllers name and id
			controller.Name = ColladaUtilities.FormatName(name, " ", "_") + "-skin";
			controller.ID = ColladaElement.FormatID<Core.ColladaController>(controller.Name);

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

			// create a float array source for the bind pose matrices
			controller.Skin.Source.Add(new Core.ColladaSource());
			controller.Skin.Source[1].ID = controller.ID + "-bindposes";
			controller.Skin.Source[1].FloatArray = new Core.ColladaFloatArray();
			controller.Skin.Source[1].FloatArray.ID = controller.Skin.Source[1].ID + "-array";

			foreach (var bone in listBoneMatrix)
			{
				controller.Skin.Source[0].NameArray.Add(ColladaUtilities.FormatName(bone.Name, " ", "_"));

				SlimDX.Matrix bind_pose_matrix = SlimDX.Matrix.Invert(bone.TransformMatrixWorld);

				// a bind pose matrix is the inverse of the world transform of the bone
				controller.Skin.Source[1].FloatArray.Add(
					bind_pose_matrix.M11, bind_pose_matrix.M21, bind_pose_matrix.M31, bind_pose_matrix.M41,
					bind_pose_matrix.M12, bind_pose_matrix.M22, bind_pose_matrix.M32, bind_pose_matrix.M42,
					bind_pose_matrix.M13, bind_pose_matrix.M23, bind_pose_matrix.M33, bind_pose_matrix.M43,
					bind_pose_matrix.M14, bind_pose_matrix.M24, bind_pose_matrix.M34, bind_pose_matrix.M44);
			}

			// create a technique that describes what the joint name array is for
			controller.Skin.Source[0].TechniqueCommon = CreateControllerTechniqueCommon(
				controller.Skin.Source[0].NameArray.ID, controller.Skin.Source[0].NameArray.Count,
				"JOINT", "Name", 1);

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

			// add the vertex weights
			foreach (var vertex_weight in vertex_weights)
			{
				// add the weight count and weight values to the relevant arrays
				controller.Skin.VertexWeights.VCount.Add(vertex_weight.Weights.Count);

				foreach(var weight in vertex_weight.Weights)
				{
					controller.Skin.Source[2].FloatArray.Add(weight.BoneWeight);
					controller.Skin.VertexWeights.V.Add(weight.BoneIndex, controller.Skin.Source[2].FloatArray.Values.Count - 1);
				}
			}
			controller.Skin.Source[2].TechniqueCommon.Accessor.Count = controller.Skin.Source[2].FloatArray.Count;

			// add the complete controller to the list
			listController.Add(controller);
		}
		#endregion
		#region Create Images
		/// <summary>
		/// Populates the image array with bitmap references from the bitmap datum array
		/// </summary>
		protected void CreateImageList()
		{
			// create references to the images
			for (int i = 0; i < bitmapDatums.Count; i++)
			{
				// if the datum index in invalid, report an error
				if (!IsDatumValid(bitmapDatums[i]))
				{
					OnErrorOccured(String.Format(ColladaExceptionStrings.InvalidDatumIndex, bitmapDatums[i].ToString(), "unknown"));
					continue;
				}

				BlamLib.Managers.TagManager bitmap_tag = tagIndex[bitmapDatums[i]];

				Fx.ColladaImage image = new Fx.ColladaImage();
				// set the ID using the bitmaps file name, this must stay consistent so that effects can generate the same ID
				string bitmap_name = ColladaUtilities.FormatName(System.IO.Path.GetFileNameWithoutExtension(bitmap_tag.Name), " ", "_");
				image.ID = ColladaElement.FormatID<Fx.ColladaImage>(bitmap_name);

				image.InitFrom = new Fx.ColladaInitFrom();
				image.InitFrom.Text = ColladaUtilities.BuildUri("file://",
					String.Concat(exportArguments.RelativeDataPath, bitmap_tag.Name, ".", exportArguments.BitmapFormatString));

				listImage.Add(image);
			}
		}
		#endregion
		#region Create Effects
		/// <summary>
		/// Creates a texture element from a bitmap datum
		/// </summary>
		/// <param name="bitmap_datum">The bitmap to create a texture element for</param>
		/// <param name="uv_channel">The texture coordinate channel to use</param>
		/// <returns></returns>
		protected Fx.ColladaTexture CreateTexture(DatumIndex bitmap_datum, int uv_channel)
		{
			Managers.TagManager bitmap = tagIndex[bitmap_datum];
			string bitmap_name = ColladaUtilities.FormatName(System.IO.Path.GetFileNameWithoutExtension(bitmap.Name), " ", "_");

			Fx.ColladaTexture texture = new Fx.ColladaTexture();
			texture.Texture = bitmap_name + "-surface-sampler";
			texture.TexCoord = "CHANNEL" + uv_channel.ToString();
			return texture;
		}
		/// <summary>
		/// Creates a default phong definition with a grey diffuse
		/// </summary>
		/// <returns></returns>
		protected static Fx.ColladaPhong CreateDefaultPhong()
		{
			Fx.ColladaPhong phong = new Fx.ColladaPhong();

			// set the default ambient color
			phong.Ambient = new Fx.ColladaCommonColorOrTextureType();
			phong.Ambient.Color = new Core.ColladaColor(0, 0, 0, 1);

			// set the default emission color
			phong.Emission = new Fx.ColladaCommonColorOrTextureType();
			phong.Emission.Color = new Core.ColladaColor(0, 0, 0, 1);

			// set the default reflective color
			phong.Reflective = new Fx.ColladaCommonColorOrTextureType();
			phong.Reflective.Color = new Core.ColladaColor(0, 0, 0, 1);

			// set the default reflectivity
			phong.Reflectivity = new Fx.ColladaCommonFloatOrParamType();
			phong.Reflectivity.Float = new ColladaSIDValue<float>(0);

			// set the default transparent color
			phong.Transparent = new Fx.ColladaCommonColorOrTextureType();
			phong.Transparent.Color = new Core.ColladaColor(1, 1, 1, 1);

			// set the default transparency
			phong.Transparency = new Fx.ColladaCommonFloatOrParamType();
			phong.Transparency.Float = new ColladaSIDValue<float>(1.0f);

			// set the default diffuse color
			phong.Diffuse = new Fx.ColladaCommonColorOrTextureType();
			phong.Diffuse.Color = new Core.ColladaColor(0.5f, 0.5f, 0.5f, 1);

			// set the default specular color
			phong.Specular = new Fx.ColladaCommonColorOrTextureType();
			phong.Specular.Color = new Core.ColladaColor(1, 1, 1, 1);

			// set the default shininess
			phong.Shininess = new Fx.ColladaCommonFloatOrParamType();
			phong.Shininess.Float = new ColladaSIDValue<float>(0.0f);

			return phong;
		}
		/// <summary>
		/// Creates an effect element with a phong definition set up with default values
		/// </summary>
		/// <param name="effect_id">ID of the effect</param>
		protected static Fx.ColladaEffect CreateDefaultEffect(string effect_name)
		{
			Fx.ColladaEffect effect = new Fx.ColladaEffect();

			// create a common profile element
			effect.ID = ColladaElement.FormatID<Fx.ColladaEffect>(effect_name);
			effect.ProfileCOMMON = new List<Fx.ColladaProfileCOMMON>();
			effect.ProfileCOMMON.Add(new Fx.ColladaProfileCOMMON());
			effect.ProfileCOMMON[0].Technique = new Fx.ColladaTechnique();
			effect.ProfileCOMMON[0].Technique.sID = "common";
			effect.ProfileCOMMON[0].Technique.Phong = CreateDefaultPhong();

			return effect;
		}
		/// <summary>
		/// Returns a phong shader created from a shader tag.
		/// 
		/// This must be overriden in derived exporters for engine specific shader parsing
		/// </summary>
		/// <param name="shader_datum">The shader datum index</param>
		/// <returns></returns>
		protected abstract Fx.ColladaPhong CreatePhong(DatumIndex shader_datum);
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
				// if the datum index is invalid, create a default effect
				if (!IsDatumValid(shader_datum))
				{
					OnErrorOccured(String.Format(ColladaExceptionStrings.InvalidDatumIndex, shader_datum.ToString(), "unknown"));
					listEffect.Add(CreateDefaultEffect(ColladaUtilities.FormatName(Path.GetFileNameWithoutExtension(shaderInfo.GetShaderName(i)))));
					continue;
				}

				List<DatumIndex> bitmaps = GetShaderBitmaps(shader_datum);
				// get the bitmaps used byt the shader
				List<string> bitmap_names = new List<string>();
				if (bitmaps != null)
				{
					foreach (var datum in bitmaps)
					{
						if (!IsDatumValid(datum)) continue;
						bitmap_names.Add(tagIndex[datum].Name);
					}
				}

				// create the effect and add it to the list
				listEffect.Add(CreateEffect(Path.GetFileNameWithoutExtension(shaderInfo.GetShaderName(i)),
					CreatePhong(shader_datum),
					bitmap_names));
			}
		}
		#endregion
		#region Create Materials
		/// <summary>
		/// Creates a material element
		/// </summary>
		/// <param name="id">ID for the material</param>
		/// <param name="name">Name for the material</param>
		/// <param name="effect_sid">The sID for the effect</param>
		/// <param name="effect">The effect the material references</param>
		/// <returns></returns>
		protected static Fx.ColladaMaterial CreateMaterial(string id, string name, string effect)
		{
			Fx.ColladaMaterial material = new Fx.ColladaMaterial();

			material.ID = ColladaElement.FormatID <Fx.ColladaMaterial>(id);
			material.Name = name;
			material.InstanceEffect = new Fx.ColladaInstanceEffect();
			material.InstanceEffect.sID = effect;
			material.InstanceEffect.URL = COLLADA.ColladaUtilities.BuildUri(ColladaElement.FormatID<Fx.ColladaEffect>(effect));

			return material;
		}
		/// <summary>
		/// Creates a material element
		/// </summary>
		/// <param name="id">Name to use for everything (id, name, sID, and effect)</param>
		/// <returns>CreateMaterial(id, id, id, id)</returns>
		protected static Fx.ColladaMaterial CreateMaterial(string id)
		{
			return CreateMaterial(id, id, id);
		}
		/// <summary>
		/// Populate the material list
		/// </summary>
		protected void CreateMaterialList()
		{
			// for each shader, create a new material
			for (int i = 0; i < shaderInfo.GetShaderCount(); i++)
			{
				string shader_name = ColladaUtilities.FormatName(Path.GetFileNameWithoutExtension(shaderInfo.GetShaderName(i)), " ", "_");
				listMaterial.Add(
					CreateMaterial(shader_name,
						shader_name,
						shader_name));
			}
		}
		#endregion
		#region Create Bones
		/// <summary>
		/// Generic bone information class
		/// </summary>
		protected class Bone
		{
			public string Name { get; private set; }
			public LowLevel.Math.real_point3d Translation { get; private set; }
			public LowLevel.Math.real_quaternion Rotation { get; private set; }
			public float Scale { get; private set; }

			public int Parent { get; private set; }
			public int Child { get; private set; }
			public int Sibling { get; private set; }

			public Bone(string name,
				LowLevel.Math.real_point3d translation, LowLevel.Math.real_quaternion rotation, float scale,
				int parent, int child, int sibling)
			{
				Name = name;

				Translation = translation;
				Rotation = rotation;
				Scale = scale;

				Parent = parent;
				Child = child;
				Sibling = sibling;
			}
		}
		/// <summary>
		/// Creates a bone tree list that contains the matrices necessary for skinning
		/// </summary>
		/// <param name="bones">The full bone list to create matrices for</param>
		void CreateBoneMatrixList(List<Bone> bones)
		{
			// add nodes for all of the bones in order
			foreach(var bone in bones)
			{
				listBoneMatrix.Add(new ColladaBoneMatrix(bone.Name,
					bone.Translation,
					bone.Rotation,
					bone.Scale));
			}
			// now that all the nodes have been created, set the parent node values, and calculate the matrices
			for (int i = 0; i < bones.Count; i++)
			{
				if (bones[i].Parent != -1)
					listBoneMatrix[i].ParentNode = listBoneMatrix[bones[i].Parent];

				listBoneMatrix[i].CreateMatrices();
			}
		}
		/// <summary>
		/// Creates a list of nodes containing the child bones of a single bone
		/// </summary>
		/// <param name="bone">The bone to get the children for</param>
		/// <param name="bone_list">The full bone list</param>
		/// <returns></returns>
		List<Core.ColladaNode> GetChildBones(Bone bone, List<Bone> bone_list)
		{
			// bone has no children, return
			if (bone.Child == -1)
				return null;

			List<Core.ColladaNode> children = new List<Core.ColladaNode>();

			// cycle through the first child and its siblings
			int bone_index = bone.Child;
			while (bone_index != -1)
			{
				children.Add(listBone[bone_index]);

				bone_index = bone_list[bone_index].Sibling;
			}
			return children;
		}
		/// <summary>
		/// Creates a collada node for a single bone
		/// </summary>
		/// <param name="bone">The bone to create a node for</param>
		/// <param name="rotation_vector_y">Y column vector for correct euler rotation</param>
		/// <param name="rotation_vector_p">P column vector for correct euler rotation</param>
		/// <param name="rotation_vector_r">R column vector for correct euler rotation</param>
		/// <returns></returns>
		Core.ColladaNode CreateBone(Bone bone,
			LowLevel.Math.real_vector3d rotation_vector_y,
			LowLevel.Math.real_vector3d rotation_vector_p,
			LowLevel.Math.real_vector3d rotation_vector_r)
		{
			// create a node and set its attributes
			Core.ColladaNode node = new Core.ColladaNode();
			node.Name = node.sID = ColladaUtilities.FormatName(bone.Name, " ", "_");
			node.ID = ColladaElement.FormatID<Core.ColladaNode>(node.sID);
			node.Type = Enums.ColladaNodeType.JOINT;

			// set the nodes position
			Core.ColladaTranslate translation = new Core.ColladaTranslate(bone.Translation);
			translation.sID = "translation";
			node.Add(translation);

			// set the nodes rotation
			node.AddRange(ColladaUtilities.CreateRotationSet(TagInterface.RealQuaternion.ToEuler3D(bone.Rotation),
				rotation_vector_y, rotation_vector_p, rotation_vector_r));

			return node;
		}
		/// <summary>
		/// Creates the bone node and bone matrix lists from a generic list of bones
		/// </summary>
		/// <param name="bones">A generic list of connected bones</param>
		/// <param name="rotation_vector_y">Y column vector for correct euler rotation</param>
		/// <param name="rotation_vector_p">P column vector for correct euler rotation</param>
		/// <param name="rotation_vector_r">R column vector for correct euler rotation</param>
		protected void CreateBones(List<Bone> bones,
			LowLevel.Math.real_vector3d rotation_vector_y,
			LowLevel.Math.real_vector3d rotation_vector_p,
			LowLevel.Math.real_vector3d rotation_vector_r)
		{
			// create the bone nodes
			foreach (var bone in bones)
				listBone.Add(CreateBone(bone,
					rotation_vector_y, rotation_vector_p, rotation_vector_r));

			// link the bones together
			for (int i = 0; i < bones.Count; i++)
				listBone[i].AddRange(GetChildBones(bones[i], bones));

			// create the bone matrix list
			CreateBoneMatrixList(bones);
		}
		#endregion
		#region Create Instance
		/// <summary>
		/// Creates a node that is an instance of either a geometry of controller element
		/// </summary>
		/// <param name="name">The name for the node</param>
		/// <param name="index">The object index to create an instance of</param>
		/// <param name="shader_list">String list containing the shaders to bind to the instance</param>
		/// <param name="is_controller">Controls whether to create an geometry or controller instance</param>
		void CreateInstanceNode(string name, int index, List<string> shader_list, bool is_controller)
		{
			// create the node and set its attributes
			Core.ColladaNode node = new Core.ColladaNode();
			node.Name = ColladaUtilities.FormatName(name, " ", "_");
			node.ID = ColladaElement.FormatID<Core.ColladaNode>(node.Name);
			node.Type = Enums.ColladaNodeType.NODE;

			Fx.ColladaBindMaterial bind_material = null;
			if (shader_list.Count != 0)
			{
				// bind materials to the geometry
				bind_material = new Fx.ColladaBindMaterial();
				bind_material.TechniqueCommon = new Core.ColladaTechniqueCommon();
				bind_material.TechniqueCommon.InstanceMaterial = new List<Fx.ColladaInstanceMaterial>();

				for (int shader_index = 0; shader_index < shader_list.Count; shader_index++)
				{
					// create a new material instance referencing the required material
					Fx.ColladaInstanceMaterial instance_material = new Fx.ColladaInstanceMaterial();
					instance_material.Symbol = shader_list[shader_index];
					instance_material.Target = ColladaUtilities.BuildUri(ColladaElement.FormatID<Fx.ColladaMaterial>(shader_list[shader_index]));

					// add the material instance to the list
					bind_material.TechniqueCommon.InstanceMaterial.Add(instance_material);
				}
			}

			if (is_controller)
			{
				// create a new controller instance and set its attributes
				Core.ColladaInstanceController instance_controller = new Core.ColladaInstanceController();
				instance_controller.URL = ColladaUtilities.BuildUri(listController[index].ID);
				instance_controller.Skeleton = new List<Core.ColladaSkeleton>();
				instance_controller.Skeleton.Add(new Core.ColladaSkeleton());
				instance_controller.Skeleton[0].Value = ColladaUtilities.BuildUri(listBone[0].ID);

				// add the bind material element if present
				if (bind_material != null)
				{
					instance_controller.BindMaterial = new List<Fx.ColladaBindMaterial>();
					instance_controller.BindMaterial.Add(bind_material);
				}

				node.Add(instance_controller);
			}
			else
			{
				// create a new geometry instance and set its attributes
				Core.ColladaInstanceGeometry instance_geometry = new Core.ColladaInstanceGeometry();
				instance_geometry.URL = ColladaUtilities.BuildUri(listGeometry[index].ID);

				// add the bind material element if present
				if (bind_material != null)
					instance_geometry.BindMaterial = bind_material;

				node.Add(instance_geometry);
			}

			// add the conpleted instance to the node list
			listNode.Add(node);
		}
		/// <summary>
		/// Creates a node instancing a controller
		/// </summary>
		/// <param name="name">The name for the node</param>
		/// <param name="index">Index of the controller to instance</param>
		/// <param name="shader_list">A string list contining the materials to bind to the instance</param>
		protected void CreateNodeInstanceController(string name, int index, List<string> shader_list)
		{
			CreateInstanceNode(name, index, shader_list, true);
		}
		/// <summary>
		/// Creates a node instancing a geometry
		/// </summary>
		/// <param name="name">The name for the node</param>
		/// <param name="index">Index of the geometry to instance</param>
		/// <param name="shader_list">A string list contining the materials to bind to the instance</param>
		protected void CreateNodeInstanceGeometry(string name, int index, List<string> shader_list)
		{
			CreateInstanceNode(name, index, shader_list, false);
		}
		#endregion
		#region Create Markers
		/// <summary>
		/// Generic marker information class
		/// </summary>
		protected class Marker
		{
			public string Name { get; private set; }
			public LowLevel.Math.real_point3d Translation { get; private set; }
			public LowLevel.Math.real_quaternion Rotation { get; private set; }
			public int BoneIndex { get; private set; }

			public Marker(string name, LowLevel.Math.real_point3d translation, LowLevel.Math.real_quaternion rotation, int bone_index)
			{
				Name = name;
				Translation = translation;
				Rotation = rotation;
				BoneIndex = bone_index;
			}
		}
		/// <summary>
		/// Creates nodes from a generic list of markers
		/// </summary>
		/// <param name="markers">Generic list of markers to create nodes from</param>
		/// <param name="rotation_vector_y">Y column vector for correct euler rotation</param>
		/// <param name="rotation_vector_p">P column vector for correct euler rotation</param>
		/// <param name="rotation_vector_r">R column vector for correct euler rotation</param>
		protected void CreateMarkers(List<Marker> markers,
			LowLevel.Math.real_vector3d rotation_vector_y, LowLevel.Math.real_vector3d rotation_vector_p, LowLevel.Math.real_vector3d rotation_vector_r)
		{
			foreach (var marker in markers)
			{
				// create a node and initialize its values
				Core.ColladaNode node = new BlamLib.Render.COLLADA.Core.ColladaNode();
				node.Name = ColladaUtilities.FormatName(marker.Name, " ", "_");
				node.Type = Enums.ColladaNodeType.NODE;

				// set the nodes position
				Core.ColladaTranslate translate = new Core.ColladaTranslate();
				translate.SetTranslate(marker.Translation);
				node.Add(translate);

				// set the nodes rotation
				node.AddRange(ColladaUtilities.CreateRotationSet(TagInterface.RealQuaternion.ToEuler3D(marker.Rotation),
						rotation_vector_y, rotation_vector_p, rotation_vector_r));

				// if the bone index is -1 add it to the node list, otherwise add it to a bone
				if (marker.BoneIndex == -1)
					listNode.Add(node);
				else
					listBone[marker.BoneIndex].Add(node);
			}
		}
		#endregion
		#endregion

		#region Library Creation
		/// <summary>
		/// Creates the library_images element in the collada file if applicable
		/// </summary>
		protected void AddLibraryImages()
		{
			// if the image list is null or empty, do not create the library element
			if ((listImage == null) || (listImage.Count == 0))
				return;

			COLLADAFile.LibraryImages = new Fx.ColladaLibraryImages();
			COLLADAFile.LibraryImages.Image = new List<Fx.ColladaImage>();
			COLLADAFile.LibraryImages.Image.AddRange(listImage);
		}
		/// <summary>
		/// Creates the library_effects element in the collada file if applicable
		/// </summary>
		protected void AddLibraryEffects()
		{
			// if the effect list is null or empty, do not create the library element
			if ((listEffect == null) || (listEffect.Count == 0))
				return;

			COLLADAFile.LibraryEffects = new Fx.ColladaLibraryEffects();
			COLLADAFile.LibraryEffects.Effect = new List<Fx.ColladaEffect>();
			COLLADAFile.LibraryEffects.Effect.AddRange(listEffect);
		}
		/// <summary>
		/// Creates the library_materials element in the collada file if applicable
		/// </summary>
		protected void AddLibraryMaterials()
		{
			// if the material list is null or empty, do not create the library element
			if ((listMaterial == null) || (listMaterial.Count == 0))
				return;

			COLLADAFile.LibraryMaterials = new Fx.ColladaLibraryMaterials();
			COLLADAFile.LibraryMaterials.Material = new List<Fx.ColladaMaterial>();
			COLLADAFile.LibraryMaterials.Material.AddRange(listMaterial);
		}
		/// <summary>
		/// Creates the library_geometries element in the collada file
		/// </summary>
		protected void AddLibraryGeometries()
		{
			COLLADAFile.LibraryGeometries = new Core.ColladaLibraryGeometries();
			COLLADAFile.LibraryGeometries.Geometry = new List<Core.ColladaGeometry>();
			COLLADAFile.LibraryGeometries.Geometry.AddRange(listGeometry);
		}
		/// <summary>
		/// Creates the library_controllers element in the collada file
		/// </summary>
		protected void AddLibraryControllers()
		{
			COLLADAFile.LibraryControllers = new Core.ColladaLibraryControllers();
			COLLADAFile.LibraryControllers.Controller = new List<Core.ColladaController>();
			COLLADAFile.LibraryControllers.Controller.AddRange(listController);
		}
		#endregion

		#region Data Collection
		/// <summary>
		/// Returns a DatumIndex list of all the bitmaps being used in a shader tag.
		/// 
		/// This must be overriden in derived exporters for engine specific shader parsing
		/// </summary>
		/// <param name="shader_datum">The shader datum index</param>
		/// <returns></returns>
		protected abstract List<DatumIndex> GetShaderBitmaps(DatumIndex shader_datum);
		/// <summary>
		/// Populates the bitmap datums list
		/// </summary>
		protected void CollectBitmaps()
		{
			int effect_count = shaderInfo.GetShaderCount();
			// for each shader get the bitmaps it uses
			for (int i = 0; i < effect_count; i++)
			{
				List<DatumIndex> bitmaps = GetShaderBitmaps(shaderInfo.GetShaderDatum(i));
				// add the bitmap datums to the list if not already present
				foreach (var datum in bitmaps)
					if (!bitmapDatums.Contains(datum))
						bitmapDatums.Add(datum);
			}
		}
		#endregion
	};
}