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
using BlamLib.Blam;

namespace BlamLib.Render.COLLADA
{
	/// <summary>
	/// Base class for exporting a COLLADA file
	/// </summary>
	public abstract class ColladaExporter
	{
		#region Settings
		protected bool bOverwrite = false;
		protected string relativeDataPath = "";
		protected string bitmapFormatString = "";

		/// <summary>
		/// When true, existing files are overwritten when saving
		/// </summary>
		public bool Overwrite
		{
			set { bOverwrite = value; }
		}
		/// <summary>
		/// Set this to the absoluts path that relative data references should start at
		/// </summary>
		public string RelativeDataPath
		{
			set { relativeDataPath = value; }
		}
		/// <summary>
		/// Set this to the bitmap extension the materials should use (tga, jpg, tif, etc)
		/// </summary>
		public string BitmapFormat
		{
			set { bitmapFormatString = value; }
		}
		#endregion

		#region Events
		public class ColladaErrorEventArgs : EventArgs
		{
			public string ErrorMessage { get; private set; }

			public ColladaErrorEventArgs(string message)
			{
				ErrorMessage = message;
			}
		};

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
		protected ColladaFile COLLADAFile = new ColladaFile();

		protected Managers.TagIndexBase tagIndex;
		protected Managers.TagManager tagManager;
		protected string tagName;
		#endregion

		protected ColladaExporter(Managers.TagIndexBase tag_index, Managers.TagManager tag_manager)
		{
			tagIndex = tag_index;
			tagManager = tag_manager;
			tagName = System.IO.Path.GetFileNameWithoutExtension(tagManager.Name);
		}

		#region Element Creation
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
		protected Core.ColladaTriangles BuildFaceIndices(int vertex_count)
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

			COLLADA.Core.ColladaTriangles triangles = new COLLADA.Core.ColladaTriangles();
			triangles.P = new COLLADA.ColladaValueArray<int>();
			for (int i = 0; i < surface_count; i++)
			{
				// create a triangle
				triangles.P.Add(
					index_array[vertex_index + 2],
					index_array[vertex_index + 1],
					index_array[vertex_index]);

				index_array.RemoveAt(vertex_index + 1);
				vertex_index++;

				if (vertex_index + 2 >= index_array.Count)
					vertex_index = 0;
			}
			return triangles;
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
			COLLADAFile.Scene.InstanceVisualScene.URL = ColladaUtilities.BuildUri(scene_id);
		}
		#endregion

		/// <summary>
		/// Saves the Collada file in its current state to file.
		/// </summary>
		/// <param name="location">Location to save the Collada file to</param>
		public void SaveDAE(string location)
		{
			// check whether the COLLADA file is valid
			try
			{
				COLLADAFile.Validate();
			}
			catch (Exception exception)
			{
				// if the collada file is not valid, add a report detailing why it is not valid
				OnErrorOccured(ColladaExceptionStrings.ValidationFailed);

				// report an error for all inner exceptions
				for (var except = exception; except != null; except = except.InnerException)
				{
					OnErrorOccured(except.Message);

					// if the exception was a validation exception, report the element details
					var validation_exception = except as ColladaValidationException;
					if ((validation_exception != null) && (validation_exception.ElementDetails != null))
					{
						foreach (string detail in validation_exception.ElementDetails)
							OnErrorOccured(String.Format("COLLADA_DETAIL: {0}", detail));
					}
				}
				return;
			}

			// if the file exists but overwriting is disabled, report this and return
			if (System.IO.File.Exists(location) && !bOverwrite)
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
			// make sure the data path ends with a '\'
			if (!relativeDataPath.EndsWith("\\"))
				relativeDataPath += "\\";

			try
			{
				return BuildColladaInstanceImpl();
			}
			catch (Exception e)
			{
				// if an exception occurred, report it and return gracefully
				OnErrorOccured(e.Message);
				OnErrorOccured(e.StackTrace);

				for (var except = e.InnerException; except != null; except = except.InnerException)
					OnErrorOccured(except.Message);
			}
			return false;
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
		#region Class Members
		protected IHaloShaderDatumList shaderInfo;

		protected List<DatumIndex> bitmapDatums = new List<DatumIndex>();

		protected List<Fx.ColladaImage> listImage = new List<Fx.ColladaImage>();
		protected List<Fx.ColladaEffect> listEffect = new List<Fx.ColladaEffect>();
		protected List<Fx.ColladaMaterial> listMaterial = new List<Fx.ColladaMaterial>();
		#endregion

		protected ColladaModelExporterBase(IHaloShaderDatumList info, Managers.TagIndexBase tag_index, Managers.TagManager tag_manager) :
			base(tag_index, tag_manager)
		{
			shaderInfo = info;
		}

		#region Element Creation
		#region Create Controllers
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
				image.ID = ColladaUtilities.FormatName(System.IO.Path.GetFileNameWithoutExtension(bitmap_tag.Name), " ", "_");

				image.InitFrom = new Fx.ColladaInitFrom();
				image.InitFrom.Text = ColladaUtilities.BuildUri("file://",
					String.Concat(relativeDataPath, bitmap_tag.Name, ".", bitmapFormatString)); ;

				listImage.Add(image);
			}
		}
		#endregion
		#region Create Effects
		protected Fx.ColladaTexture CreateTexture(DatumIndex bitmap_datum, int uv_channel)
		{
			Managers.TagManager bitmap = tagIndex[bitmap_datum];
			string bitmap_name = ColladaUtilities.FormatName(System.IO.Path.GetFileNameWithoutExtension(bitmap.Name), " ", "_");

			Fx.ColladaTexture texture = new Fx.ColladaTexture();
			texture.Texture = bitmap_name + "-surface-sampler";
			texture.TexCoord = "CHANNEL" + uv_channel.ToString();
			return texture;
		}
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
			phong.Diffuse.Color = new Core.ColladaColor(0, 0, 0, 1);

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
		protected static Fx.ColladaEffect CreateDefaultEffect(string effect_id)
		{
			Fx.ColladaEffect effect = new Fx.ColladaEffect();

			// create a common profile element
			effect.ID = effect_id;
			effect.ProfileCOMMON = new List<Fx.ColladaProfileCOMMON>();
			effect.ProfileCOMMON.Add(new Fx.ColladaProfileCOMMON());
			effect.ProfileCOMMON[0].Technique = new Fx.ColladaTechnique();
			effect.ProfileCOMMON[0].Technique.sID = "common";
			effect.ProfileCOMMON[0].Technique.Phong = CreateDefaultPhong();

			return effect;
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
		protected static Fx.ColladaMaterial CreateMaterial(string id, string name, string effect_sid, string effect)
		{
			Fx.ColladaMaterial material = new Fx.ColladaMaterial();

			material.ID = id;
			material.Name = name;
			material.InstanceEffect = new Fx.ColladaInstanceEffect();
			material.InstanceEffect.sID = effect_sid;
			material.InstanceEffect.URL = COLLADA.ColladaUtilities.BuildUri(effect);

			return material;
		}
		/// <summary>
		/// Populate the material list
		/// </summary>
		protected void CreateMaterialList()
		{
			// for each shader, create a new material
			for (int i = 0; i < shaderInfo.GetShaderCount(); i++)
			{
				DatumIndex shader_datum = shaderInfo.GetShaderDatum(i);
				if (!IsDatumValid(shader_datum))
				{
					OnErrorOccured(String.Format(ColladaExceptionStrings.InvalidDatumIndex, bitmapDatums[i].ToString(), "unknown"));
					continue;
				}

				BlamLib.Managers.TagManager shader_tag = tagIndex[shader_datum];

				string shader_name = ColladaUtilities.FormatName(System.IO.Path.GetFileNameWithoutExtension(shader_tag.Name), " ", "_");
				listMaterial.Add(
					CreateMaterial(String.Format(Fx.ColladaMaterial.ElementIDFormat, shader_name),
						shader_name,
						shader_name,
						shader_name));
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
		#endregion
	};
}