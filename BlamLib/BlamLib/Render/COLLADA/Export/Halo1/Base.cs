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
using System.IO;
using BlamLib.Blam;

using H1 = BlamLib.Blam.Halo1;

namespace BlamLib.Render.COLLADA.Halo1
{
	/// <summary>
	/// Base class for Halo1 exporters to derive from. Since shaders and bitmaps are used by both models and BSP
	/// they are handled in the base class.
	/// </summary>
	public class ColladaModelExporterHalo1 : ColladaModelExporterBase
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
		public ColladaModelExporterHalo1(IHaloShaderDatumList info, Managers.TagIndexBase tag_index, Managers.TagManager tag_manager)
			: base(info, tag_index, tag_manager)
		{
		}
		#endregion

		#region Element Creation
		#region Create Effects
		void CreatePhong(Fx.ColladaPhong phong, H1.Tags.shader_environment_group shader)
		{
			if (shader.BaseMap.Datum == DatumIndex.Null)
				phong.Diffuse.Color.SetColor(shader.MaterialColor, 1);
			else
			{
				phong.Diffuse.Color = null;
				phong.Diffuse.Texture = CreateTexture(shader.BaseMap.Datum, 0);
			}

			if (shader.Flags.Test(1) && (shader.BumpMap.Datum != DatumIndex.Null))
			{
				phong.Transparent.Color = null;
				phong.Transparent.Texture = CreateTexture(shader.BumpMap.Datum, 0);
				phong.Transparent.Opaque = Enums.ColladaFXOpaqueEnum.A_ONE;
			}

			phong.Specular.Color.SetColor(shader.PerpendicularColor, 1);
		}
		void CreatePhong(Fx.ColladaPhong phong, H1.Tags.shader_model_group shader)
		{
			if (shader.BaseMap.Datum != DatumIndex.Null)
			{
				phong.Diffuse.Color = null;
				phong.Diffuse.Texture = CreateTexture(shader.BaseMap.Datum, 0);
			}

			phong.Specular.Color.SetColor(shader.PerpendicularTintColor, 1);
		}
		void CreatePhong(Fx.ColladaPhong phong, H1.Tags.shader_transparent_chicago_group shader)
		{
			if (shader.Maps.Count > 0 && !shader.Maps[0].Map.Datum.IsNull)
			{
				phong.Diffuse.Color = null;
				phong.Diffuse.Texture = CreateTexture(shader.Maps[0].Map.Datum, 0);
			}
		}
		void CreatePhong(Fx.ColladaPhong phong, H1.Tags.shader_transparent_chicago_extended_group shader)
		{
			DatumIndex bitmap_datum = DatumIndex.Null;
			if (shader._4StageMaps.Count > 0)
				bitmap_datum = shader._4StageMaps[0].Map.Datum;
			else if (shader._2StageMaps.Count > 0)
				bitmap_datum = shader._2StageMaps[0].Map.Datum;

			if (bitmap_datum != DatumIndex.Null)
			{
				phong.Diffuse.Color = null;
				phong.Diffuse.Texture = CreateTexture(bitmap_datum, 0);
			}
		}
		void CreatePhong(Fx.ColladaPhong phong, H1.Tags.shader_transparent_generic_group shader)
		{
			if (shader.Maps.Count > 0 && !shader.Maps[0].Map.Datum.IsNull)
			{
				phong.Diffuse.Color = null;
				phong.Diffuse.Texture = CreateTexture(shader.Maps[0].Map.Datum, 0);
			}
		}
		void CreatePhong(Fx.ColladaPhong phong, H1.Tags.shader_transparent_glass_group shader)
		{
			if (shader.DiffuseMap.Datum != DatumIndex.Null)
			{
				phong.Diffuse.Color = null;
				phong.Diffuse.Texture = CreateTexture(shader.DiffuseMap.Datum, 0);
			}
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

			if (shader_man.GroupTag == Blam.Halo1.TagGroups.senv)
			{
				var shader_senv = GetTagDefinition<H1.Tags.shader_environment_group>(
					shader_datum, shader_man.GroupTag, Blam.Halo1.TagGroups.senv);

				CreatePhong(phong, shader_senv);
			}
			else if (shader_man.GroupTag == Blam.Halo1.TagGroups.soso)
			{
				var shader_soso = GetTagDefinition<H1.Tags.shader_model_group>(
					shader_datum, shader_man.GroupTag, Blam.Halo1.TagGroups.soso);

				CreatePhong(phong, shader_soso);
			}
			else if (shader_man.GroupTag == Blam.Halo1.TagGroups.schi)
			{
				var shader_schi = GetTagDefinition<H1.Tags.shader_transparent_chicago_group>(
					shader_datum, shader_man.GroupTag, Blam.Halo1.TagGroups.schi);

				CreatePhong(phong, shader_schi);
			}
			else if (shader_man.GroupTag == Blam.Halo1.TagGroups.scex)
			{
				var shader_scex = GetTagDefinition<H1.Tags.shader_transparent_chicago_extended_group>(
					shader_datum, shader_man.GroupTag, Blam.Halo1.TagGroups.scex);

				CreatePhong(phong, shader_scex);
			}
			else if (shader_man.GroupTag == Blam.Halo1.TagGroups.sotr)
			{
				var shader_sotr = GetTagDefinition<H1.Tags.shader_transparent_generic_group>(
					shader_datum, shader_man.GroupTag, Blam.Halo1.TagGroups.sotr);

				CreatePhong(phong, shader_sotr);
			}
			else if (shader_man.GroupTag == Blam.Halo1.TagGroups.sgla)
			{
				var shader_sgla = GetTagDefinition<H1.Tags.shader_transparent_glass_group>(
					shader_datum, shader_man.GroupTag, Blam.Halo1.TagGroups.sgla);

				CreatePhong(phong, shader_sgla);
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

			if (shader_man.GroupTag == Blam.Halo1.TagGroups.senv)
			{
				H1.Tags.shader_environment_group shader_senv = GetTagDefinition<H1.Tags.shader_environment_group>(
					shader_datum, shader_man.GroupTag, Blam.Halo1.TagGroups.senv);

				if (shader_senv.BaseMap.Datum != DatumIndex.Null)
					bitmap_datums.Add(shader_senv.BaseMap.Datum);

				if (shader_senv.Flags.Test(1) && (shader_senv.BumpMap.Datum != DatumIndex.Null))
					bitmap_datums.Add(shader_senv.BumpMap.Datum);
			}
			else if (shader_man.GroupTag == Blam.Halo1.TagGroups.soso)
			{
				H1.Tags.shader_model_group shader_soso = GetTagDefinition<H1.Tags.shader_model_group>(
					shader_datum, shader_man.GroupTag, Blam.Halo1.TagGroups.soso);

				if (shader_soso.BaseMap.Datum != DatumIndex.Null)
					bitmap_datums.Add(shader_soso.BaseMap.Datum);
			}
			else if (shader_man.GroupTag == Blam.Halo1.TagGroups.schi)
			{
				H1.Tags.shader_transparent_chicago_group shader_schi = GetTagDefinition<H1.Tags.shader_transparent_chicago_group>(
					shader_datum, shader_man.GroupTag, Blam.Halo1.TagGroups.schi);

				if ((shader_schi.Maps.Count > 0) && (shader_schi.Maps[0].Map.Datum != DatumIndex.Null))
					bitmap_datums.Add(shader_schi.Maps[0].Map.Datum);
			}
			else if (shader_man.GroupTag == Blam.Halo1.TagGroups.scex)
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
			else if (shader_man.GroupTag == Blam.Halo1.TagGroups.sotr)
			{
				H1.Tags.shader_transparent_generic_group shader_sotr = GetTagDefinition<H1.Tags.shader_transparent_generic_group>(
					shader_datum, shader_man.GroupTag, Blam.Halo1.TagGroups.sotr);

				if ((shader_sotr.Maps.Count > 0) && (shader_sotr.Maps[0].Map.Datum != DatumIndex.Null))
					bitmap_datums.Add(shader_sotr.Maps[0].Map.Datum);
			}
			else if (shader_man.GroupTag == Blam.Halo1.TagGroups.sgla)
			{
				H1.Tags.shader_transparent_glass_group shader_sgla = GetTagDefinition<H1.Tags.shader_transparent_glass_group>(
					shader_datum, shader_man.GroupTag, Blam.Halo1.TagGroups.sgla);

				if (shader_sgla.DiffuseMap.Datum != DatumIndex.Null)
					bitmap_datums.Add(shader_sgla.DiffuseMap.Datum);
			}

			return bitmap_datums;
		}
		#endregion
	};
}