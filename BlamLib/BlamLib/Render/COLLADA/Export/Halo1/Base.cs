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
using BlamLib.Render;

using H1 = BlamLib.Blam.Halo1;

namespace BlamLib.Render.COLLADA.Halo1
{
	/// <summary>
	/// Base class for Halo1 exporters to derive from. Since shaders and bitmaps are used by both models and BSP
	/// they are handled in the base class.
	/// </summary>
	public class ColladaExporterHalo1 : ColladaExporter
	{
		#region Class Fields
		private IHalo1ShaderDatumList shaderInfo;

		protected Managers.TagIndexBase tagIndex;
		protected Managers.TagManager tagManager;
		protected string tagName;

		protected List<DatumIndex> bitmapDatums = new List<DatumIndex>();

		protected List<Fx.ColladaImage> listImage = new List<Fx.ColladaImage>();
		protected List<Fx.ColladaEffect> listEffect = new List<Fx.ColladaEffect>();
		protected List<Fx.ColladaMaterial> listMaterial = new List<Fx.ColladaMaterial>();
		#endregion

		#region Constructor
		/// <summary>
		/// Private constructor since this class MUST be initialised with arguments
		/// </summary>
		protected ColladaExporterHalo1() { }
		/// <summary>
		/// Base class constructor
		/// </summary>
		/// <param name="info">An object implementing IHalo1ShaderDatumList to provide a list of shader datums that the model/BSP uses</param>
		/// <param name="tag_index">The tag index that contains the tag being exported</param>
		/// <param name="tag_manager">The tag manager of the tag being exported</param>
		public ColladaExporterHalo1(IHalo1ShaderDatumList info, Managers.TagIndexBase tag_index, Managers.TagManager tag_manager)
			: base()
		{
			shaderInfo = info;
			tagIndex = tag_index;
			tagManager = tag_manager;
			tagName = Path.GetFileNameWithoutExtension(tagManager.Name);
		}
		#endregion

		#region Helpers
		/// <summary>
		/// Determines whether a DatumIndex is valid
		/// </summary>
		/// <param name="index">The index to validate</param>
		/// <returns>Returns true if the DatumIndex is neither null, nor sentinel</returns>
		protected bool IsDatumValid(DatumIndex index)
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

		#region Element Creation
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
		protected Fx.ColladaPhong CreateDefaultPhong()
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
		protected Fx.ColladaEffect CreateDefaultEffect(string effect_id)
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
		/// <summary>
		/// Creates a phong definition using values from a shader tag
		/// </summary>
		/// <param name="shader_datum">DatumIndex of the shader to create a phong definition from</param>
		/// <returns></returns>
		protected Fx.ColladaPhong CreatePhong(DatumIndex shader_datum)
		{
			Managers.TagManager shader_man = tagIndex[shader_datum];

			H1.Tags.shader_group shader_base = GetTagDefinition<H1.Tags.shader_group>(
					shader_datum, shader_man.GroupTag, Blam.Halo1.TagGroups.shdr);

			Fx.ColladaPhong phong = CreateDefaultPhong();

			phong.Emission = new Fx.ColladaCommonColorOrTextureType();
			phong.Emission.Color = new Core.ColladaColor(shader_base.RadiosityEmittedLightColor, 1.0f);
			phong.Diffuse.Color.SetColor(1, 1, 1, 1);
			phong.Shininess.Float.Value = 30.0f;

			shader_base = null;

			if (shader_man.GroupTag.ID == Blam.Halo1.TagGroups.senv.ID)
			{
				H1.Tags.shader_environment_group shader_senv = GetTagDefinition<H1.Tags.shader_environment_group>(
					shader_datum, shader_man.GroupTag, Blam.Halo1.TagGroups.senv);

				if (shader_senv.BaseMap.Datum == DatumIndex.Null)
					phong.Diffuse.Color.SetColor(shader_senv.MaterialColor, 1);
				else
				{
					phong.Diffuse.Color = null;
					phong.Diffuse.Texture = CreateTexture(shader_senv.BaseMap.Datum, 0);
				}

				if (shader_senv.Flags.Test(1) && (shader_senv.BumpMap.Datum != DatumIndex.Null))
				{
					phong.Transparent.Color = null;
					phong.Transparent.Texture = CreateTexture(shader_senv.BumpMap.Datum, 0);
					phong.Transparent.Opaque = Enums.ColladaFXOpaqueEnum.A_ONE;
				}

				phong.Specular.Color.SetColor(shader_senv.PerpendicularColor, 1);
			}
			else if (shader_man.GroupTag.ID == Blam.Halo1.TagGroups.soso.ID)
			{
				H1.Tags.shader_model_group shader_soso = GetTagDefinition<H1.Tags.shader_model_group>(
					shader_datum, shader_man.GroupTag, Blam.Halo1.TagGroups.soso);

				if (shader_soso.BaseMap.Datum != DatumIndex.Null)
				{
					phong.Diffuse.Color = null;
					phong.Diffuse.Texture = CreateTexture(shader_soso.BaseMap.Datum, 0);
				}

				phong.Specular.Color.SetColor(shader_soso.PerpendicularTintColor, 1);
			}
			else if (shader_man.GroupTag.ID == Blam.Halo1.TagGroups.schi.ID)
			{
				H1.Tags.shader_transparent_chicago_group shader_schi = GetTagDefinition<H1.Tags.shader_transparent_chicago_group>(
					shader_datum, shader_man.GroupTag, Blam.Halo1.TagGroups.schi);

				if ((shader_schi.Maps.Count > 0) && (shader_schi.Maps[0].Map.Datum != DatumIndex.Null))
				{
					phong.Diffuse.Color = null;
					phong.Diffuse.Texture = CreateTexture(shader_schi.Maps[0].Map.Datum, 0);
				}
			}
			else if (shader_man.GroupTag.ID == Blam.Halo1.TagGroups.scex.ID)
			{
				H1.Tags.shader_transparent_chicago_extended_group shader_scex = GetTagDefinition<H1.Tags.shader_transparent_chicago_extended_group>(
					shader_datum, shader_man.GroupTag, Blam.Halo1.TagGroups.scex);

				DatumIndex bitmap_datum = DatumIndex.Null;
				if (shader_scex._4StageMaps.Count > 0)
					bitmap_datum = shader_scex._4StageMaps[0].Map.Datum;
				else if (shader_scex._2StageMaps.Count > 0)
					bitmap_datum = shader_scex._2StageMaps[0].Map.Datum;

				if (bitmap_datum != DatumIndex.Null)
				{
					phong.Diffuse.Color = null;
					phong.Diffuse.Texture = CreateTexture(bitmap_datum, 0);
				}
			}
			else if (shader_man.GroupTag.ID == Blam.Halo1.TagGroups.sotr.ID)
			{
				H1.Tags.shader_transparent_generic_group shader_sotr = GetTagDefinition<H1.Tags.shader_transparent_generic_group>(
					shader_datum, shader_man.GroupTag, Blam.Halo1.TagGroups.sotr);

				if ((shader_sotr.Maps.Count > 0) && (shader_sotr.Maps[0].Map.Datum != DatumIndex.Null))
				{
					phong.Diffuse.Color = null;
					phong.Diffuse.Texture = CreateTexture(shader_sotr.Maps[0].Map.Datum, 0);
				}
			}
			else if (shader_man.GroupTag.ID == Blam.Halo1.TagGroups.sgla.ID)
			{
				H1.Tags.shader_transparent_glass_group shader_sgla = GetTagDefinition<H1.Tags.shader_transparent_glass_group>(
					shader_datum, shader_man.GroupTag, Blam.Halo1.TagGroups.sgla);

				if (shader_sgla.DiffuseMap.Datum != DatumIndex.Null)
				{
					phong.Diffuse.Color = null;
					phong.Diffuse.Texture = CreateTexture(shader_sgla.DiffuseMap.Datum, 0);
				}
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
			effect.ID = shader_name;
			effect.ProfileCOMMON = new List<COLLADA.Fx.ColladaProfileCOMMON>();
			effect.ProfileCOMMON.Add(new COLLADA.Fx.ColladaProfileCOMMON());
			effect.ProfileCOMMON[0].Technique = new COLLADA.Fx.ColladaTechnique();
			effect.ProfileCOMMON[0].Technique.sID = "common";
			effect.ProfileCOMMON[0].Technique.Phong = CreatePhong(shader_datum);
			effect.ProfileCOMMON[0].Newparam = new List<COLLADA.Fx.ColladaNewparam>();

			List<DatumIndex> bitmap_datums = GetShaderBitmaps(shader_datum);
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
		#region Create Materials
		/// <summary>
		/// Creates a material element
		/// </summary>
		/// <param name="id">ID for the material</param>
		/// <param name="name">Name for the material</param>
		/// <param name="effect_sid">The sID for the effect</param>
		/// <param name="effect">The effect the material references</param>
		/// <returns></returns>
		protected Fx.ColladaMaterial CreateMaterial(string id, string name, string effect_sid, string effect)
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

		#region Data Collection
		/// <summary>
		/// Adds the datum indices for the bitmaps used in the objects shaders, to the global list
		/// </summary>
		protected void CollectBitmaps()
		{
			for(int i = 0; i < shaderInfo.GetShaderCount(); i++)
			{
				DatumIndex shader_datum = shaderInfo.GetShaderDatum(i);

				// ignore invalid shader datums
				if (!IsDatumValid(shader_datum))
				{
					OnErrorOccured(String.Format(ColladaExceptionStrings.InvalidDatumIndex,
						shader_datum.ToString(),
						"unknown"));
					continue;
				}

				// get the shader type and get the bitmaps it uses
				BlamLib.Managers.TagManager shader_man = tagIndex[shader_datum];
				if (shader_man.GroupTag.ID == BlamLib.Blam.Halo1.TagGroups.senv.ID)
				{
					H1.Tags.shader_environment_group senv_definition = GetTagDefinition<H1.Tags.shader_environment_group>(
						shader_datum, shader_man.GroupTag, H1.TagGroups.senv);

					// add the diffuse map if present
					if (senv_definition.BaseMap.Datum != DatumIndex.Null)
					{
						if (!bitmapDatums.Contains(senv_definition.BaseMap.Datum))
							bitmapDatums.Add(senv_definition.BaseMap.Datum);
					}

					// if the shader uses alpha testing/blending with the bump maps alpha channel, add it
					if (senv_definition.Flags.Test(1) && (senv_definition.BumpMap.Datum != DatumIndex.Null))
					{
						if (!bitmapDatums.Contains(senv_definition.BumpMap.Datum))
							bitmapDatums.Add(senv_definition.BumpMap.Datum);
					}
				}
				else if (shader_man.GroupTag.ID == BlamLib.Blam.Halo1.TagGroups.soso.ID)
				{
					H1.Tags.shader_model_group soso_definition = GetTagDefinition<H1.Tags.shader_model_group>(
						shader_datum, shader_man.GroupTag, H1.TagGroups.soso);

					// add the diffuse map if present
					if (soso_definition.BaseMap.Datum != DatumIndex.Null)
					{
						if (!bitmapDatums.Contains(soso_definition.BaseMap.Datum))
							bitmapDatums.Add(soso_definition.BaseMap.Datum);
					}
				}
				else if (shader_man.GroupTag.ID == BlamLib.Blam.Halo1.TagGroups.schi.ID)
				{
					H1.Tags.shader_transparent_chicago_group schi_definition = GetTagDefinition<H1.Tags.shader_transparent_chicago_group>(
						shader_datum, shader_man.GroupTag, H1.TagGroups.schi);

					// add the bitmap from the first map block
					if ((schi_definition.Maps.Count != 0) && (schi_definition.Maps[0].Map.Datum != DatumIndex.Null))
					{
						if (!bitmapDatums.Contains(schi_definition.Maps[0].Map.Datum))
							bitmapDatums.Add(schi_definition.Maps[0].Map.Datum);
					}
				}
				else if (shader_man.GroupTag.ID == BlamLib.Blam.Halo1.TagGroups.scex.ID)
				{
					H1.Tags.shader_transparent_chicago_extended_group scex_definition = GetTagDefinition<H1.Tags.shader_transparent_chicago_extended_group>(
						shader_datum, shader_man.GroupTag, H1.TagGroups.scex);

					// add the bitmap from the first map block
					DatumIndex bitmap_datum = DatumIndex.Null;
					if (scex_definition._4StageMaps.Count > 0)
						bitmap_datum = scex_definition._4StageMaps[0].Map.Datum;
					else if (scex_definition._2StageMaps.Count > 0)
						bitmap_datum = scex_definition._2StageMaps[0].Map.Datum;

					if (bitmap_datum != DatumIndex.Null)
					{
						if (!bitmapDatums.Contains(bitmap_datum))
							bitmapDatums.Add(bitmap_datum);
					}
				}
				else if (shader_man.GroupTag.ID == BlamLib.Blam.Halo1.TagGroups.sotr.ID)
				{
					H1.Tags.shader_transparent_generic_group sotr_definition = GetTagDefinition<H1.Tags.shader_transparent_generic_group>(
						shader_datum, shader_man.GroupTag, H1.TagGroups.sotr);

					// add the bitmap from the first map block
					if ((sotr_definition.Maps.Count != 0) && (sotr_definition.Maps[0].Map.Datum != DatumIndex.Null))
					{
						if (!bitmapDatums.Contains(sotr_definition.Maps[0].Map.Datum))
							bitmapDatums.Add(sotr_definition.Maps[0].Map.Datum);
					}
				}
				else if (shader_man.GroupTag.ID == BlamLib.Blam.Halo1.TagGroups.sgla.ID)
				{
					H1.Tags.shader_transparent_glass_group sgla_definition = GetTagDefinition<H1.Tags.shader_transparent_glass_group>(
						shader_datum, shader_man.GroupTag, H1.TagGroups.sgla);

					// add the diffuse map
					if (sgla_definition.DiffuseMap.Datum != DatumIndex.Null)
					{
						if (!bitmapDatums.Contains(sgla_definition.DiffuseMap.Datum))
							bitmapDatums.Add(sgla_definition.DiffuseMap.Datum);
					}
				}
			}
		}
		/// <summary>
		/// Creates a list of datum indices for the bitmaps used in a shader
		/// </summary>
		/// <param name="shader_tag">DatumIndex of the shader to get bitmaps from</param>
		/// <returns></returns>
		protected List<DatumIndex> GetShaderBitmaps(DatumIndex shader_datum)
		{
			Managers.TagManager shader_man = tagIndex[shader_datum];
			List<DatumIndex> bitmap_datums = new List<DatumIndex>();

			if (shader_man.GroupTag.ID == Blam.Halo1.TagGroups.senv.ID)
			{
				H1.Tags.shader_environment_group shader_senv = GetTagDefinition<H1.Tags.shader_environment_group>(
					shader_datum, shader_man.GroupTag, Blam.Halo1.TagGroups.senv);

				if (shader_senv.BaseMap.Datum != DatumIndex.Null)
					bitmap_datums.Add(shader_senv.BaseMap.Datum);

				if (shader_senv.Flags.Test(1) && (shader_senv.BumpMap.Datum != DatumIndex.Null))
					bitmap_datums.Add(shader_senv.BumpMap.Datum);
			}
			else if (shader_man.GroupTag.ID == Blam.Halo1.TagGroups.soso.ID)
			{
				H1.Tags.shader_model_group shader_soso = GetTagDefinition<H1.Tags.shader_model_group>(
					shader_datum, shader_man.GroupTag, Blam.Halo1.TagGroups.soso);

				if (shader_soso.BaseMap.Datum != DatumIndex.Null)
					bitmap_datums.Add(shader_soso.BaseMap.Datum);
			}
			else if (shader_man.GroupTag.ID == Blam.Halo1.TagGroups.schi.ID)
			{
				H1.Tags.shader_transparent_chicago_group shader_schi = GetTagDefinition<H1.Tags.shader_transparent_chicago_group>(
					shader_datum, shader_man.GroupTag, Blam.Halo1.TagGroups.schi);

				if ((shader_schi.Maps.Count > 0) && (shader_schi.Maps[0].Map.Datum != DatumIndex.Null))
					bitmap_datums.Add(shader_schi.Maps[0].Map.Datum);
			}
			else if (shader_man.GroupTag.ID == Blam.Halo1.TagGroups.scex.ID)
			{
				H1.Tags.shader_transparent_chicago_extended_group shader_scex = GetTagDefinition<H1.Tags.shader_transparent_chicago_extended_group>(
					shader_datum, shader_man.GroupTag, Blam.Halo1.TagGroups.scex);

				DatumIndex bitmap_datum = DatumIndex.Null;
				if (shader_scex._4StageMaps.Count > 0)
					bitmap_datum = shader_scex._4StageMaps[0].Map.Datum;
				else if (shader_scex._2StageMaps.Count > 0)
					bitmap_datum = shader_scex._2StageMaps[0].Map.Datum;

				if (bitmap_datum != DatumIndex.Null)
					bitmap_datums.Add(bitmap_datum);
			}
			else if (shader_man.GroupTag.ID == Blam.Halo1.TagGroups.sotr.ID)
			{
				H1.Tags.shader_transparent_generic_group shader_sotr = GetTagDefinition<H1.Tags.shader_transparent_generic_group>(
					shader_datum, shader_man.GroupTag, Blam.Halo1.TagGroups.sotr);

				if ((shader_sotr.Maps.Count > 0) && (shader_sotr.Maps[0].Map.Datum != DatumIndex.Null))
					bitmap_datums.Add(shader_sotr.Maps[0].Map.Datum);
			}
			else if (shader_man.GroupTag.ID == Blam.Halo1.TagGroups.sgla.ID)
			{
				H1.Tags.shader_transparent_glass_group shader_sgla = GetTagDefinition<H1.Tags.shader_transparent_glass_group>(
					shader_datum, shader_man.GroupTag, Blam.Halo1.TagGroups.sgla);

				if (shader_sgla.DiffuseMap.Datum != DatumIndex.Null)
					bitmap_datums.Add(shader_sgla.DiffuseMap.Datum);
			}

			return bitmap_datums;
		}
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
	}
}