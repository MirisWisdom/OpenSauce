/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo1.Tags
{
	#region shader_map
	[TI.Struct(-1, -1, 180)]
	public class shader_map_struct : TI.Definition
	{
		#region Fields
		public TI.Flags ShaderFlags;
		public TI.Enum FramebufferBlendFunction, FramebufferFadeMode;
		public TI.Flags ShaderMapFlags;

		public TI.TagReference MapBitmap;
		public TI.Enum MapAnchor;
		public TI.Flags MapFlags;
		public TI.Struct<shader_animation_struct> MapAnimation;

		public TI.Real ZSpriteRadiusScale;
		#endregion

		public shader_map_struct() : base(13)
		{
			// Stubb's code has byte swapping for this padding:
			// word
			// word
			// dword
			// dword
			// dword
			// dword
			// dword
			// dword
			// dword
			// word
			// word
			// word
			// word
			Add(new TI.Pad(40));
			Add(ShaderFlags = TI.Flags.Word);
			Add(FramebufferBlendFunction = new TI.Enum());
			Add(FramebufferFadeMode = new TI.Enum());
			Add(ShaderMapFlags = TI.Flags.Word);
			Add(new TI.Pad(28));

			Add(MapBitmap = new TI.TagReference(this, TagGroups.bitm));
			
			Add(MapAnchor = new TI.Enum());
			Add(MapFlags = TI.Flags.Word);
			
			Add(MapAnimation = new TI.Struct<shader_animation_struct>(this));
			
			// 28
			Add(new TI.Pad(4));
			Add(ZSpriteRadiusScale = new TI.Real());
			Add(new TI.Pad(20));
		}
	};
	#endregion

	#region shader_color_function
	[TI.Struct(-1, -1, 60)]
	public class shader_color_function_struct : TI.Definition
	{
		public TI.RealColor OnColor, OffColor;
		public TI.Enum AnimationFunction;
		public TI.Real AnimationPeriod, AnimationPhase;

		public shader_color_function_struct() : base(7)
		{
			Add(OnColor = new TI.RealColor());
			Add(OffColor = new TI.RealColor());
			Add(AnimationFunction = new TI.Enum());
			Add(TI.Pad.Word);
			Add(AnimationPeriod = new TI.Real());
			Add(AnimationPhase = new TI.Real());
			Add(new TI.Pad(24));
		}
	};
	#endregion

	#region shader_animation_function
	[TI.Struct(-1, -1, 16)]
	public class shader_animation_function_struct : TI.Definition
	{
		public TI.Enum Source, Function;
		public TI.Real Period, Phase, Scale;

		public shader_animation_function_struct() : base(5)
		{
			Add(Source = new TI.Enum());
			Add(Function = new TI.Enum());
			Add(Period = new TI.Real());
			Add(Phase = new TI.Real());
			Add(Scale = new TI.Real());
		}
	};
	#endregion

	#region shader_animation
	[TI.Struct(-1, -1, 56)]
	public class shader_animation_struct : TI.Definition
	{
		public TI.Struct<shader_animation_function_struct> AnimU, AnimV, AnimRot;
		public TI.RealPoint2D AnimRotCenter;

		public shader_animation_struct() : base(4)
		{
			Add(AnimU = new TI.Struct<shader_animation_function_struct>(this));
			Add(AnimV = new TI.Struct<shader_animation_function_struct>(this));
			Add(AnimRot = new TI.Struct<shader_animation_function_struct>(this));
			Add(AnimRotCenter = new TI.RealPoint2D());
		}
	};
	#endregion

	#region shader_texture_animation_function
	[TI.Struct(-1, -1, 12)]
	public class shader_texture_animation_function_struct : TI.Definition
	{
		public TI.Enum Function;
		public TI.Real Period, Scale;

		public shader_texture_animation_function_struct() : base(4)
		{
			Add(Function = new TI.Enum());
			Add(TI.Pad.Word);
			Add(Period = new TI.Real());
			Add(Scale = new TI.Real());
		}
	};
	#endregion

	#region shader_frame_buffer_function
	[TI.Struct(-1, -1, 8)]
	public class shader_frame_buffer_function_struct : TI.Definition
	{
		public TI.Enum FramebufferBlendFunction;
		public TI.Enum FramebufferBlendFadeMode;
		public TI.Enum FramebufferFadeSource;

		public shader_frame_buffer_function_struct() : base(4)
		{
			Add(FramebufferBlendFunction = new TI.Enum());
			Add(FramebufferBlendFadeMode = new TI.Enum());
			Add(FramebufferFadeSource = new TI.Enum());
			Add(TI.Pad.Word);
		}
	};
	#endregion

	#region shader_radiosity_properties
	[TI.Struct(-1, -1, 12)]
	public class shader_radiosity_properties_struct : TI.Definition
	{
		public TI.ByteInteger NumericCounterLimit;
		public TI.Flags Flags;
		public TI.Enum FirstMapType;
		public TI.Struct<shader_frame_buffer_function_struct> FrameBufferFunction;

		public shader_radiosity_properties_struct() : base(4)
		{
			Add(NumericCounterLimit = new TI.ByteInteger());
			Add(Flags = TI.Flags.Byte);
			Add(FirstMapType = new TI.Enum());
			Add(FrameBufferFunction = new TI.Struct<shader_frame_buffer_function_struct>(this));
		}
	};
	#endregion


	#region shader_group
	[TI.TagGroup((int)TagGroups.Enumerated.shdr, 1, shader_group.k_shader_size)]
	public class shader_group : TI.Definition
	{
		internal const int k_shader_size = 40;

		#region Fields
		public TI.Flags RadiosityFlags;
		public TI.Enum RadiosityDetailLevel;
		public TI.Real RadiosityPower;
		public TI.RealColor RadiosityEmittedLightColor;
		public TI.RealColor RadiosityTintColor;
		public TI.Flags PhysicsFlags;
		public TI.Enum MaterialType;
		public TI.Skip ShaderType; // enum
		#endregion

		public shader_group() : this(0) {}
		protected shader_group(int field_count) : base(field_count + 9)
		{
			Add(RadiosityFlags = TI.Flags.Word);
			Add(RadiosityDetailLevel = new TI.Enum());
			Add(RadiosityPower = new TI.Real());
			Add(RadiosityEmittedLightColor = new TI.RealColor());
			Add(RadiosityTintColor = new TI.RealColor());
			Add(PhysicsFlags = TI.Flags.Word);
			Add(MaterialType = new TI.Enum());
			Add(ShaderType = new TI.Skip(2));
			Add(new TI.Pad(2));
		}
	};
	#endregion

	#region shader_effect
	[TI.TagGroup((int)TagGroups.Enumerated.seff, 1, /*shader_group.k_shader_size +*/ 56, typeof(shader_group))]
	public class shader_effect_group : shader_group
	{
		#region Fields
		public TI.Flags Flags;
		public TI.Struct<shader_frame_buffer_function_struct> FrameBufferFunction;
		public TI.TagReference Bitmap;
		public TI.Struct<shader_animation_struct> Animation;
		#endregion

		public shader_effect_group() : base(8)
		{
			// 1 << 0 - nonlinear or linear
			Add(Flags = TI.Flags.Byte);
			Add(TI.Pad.Byte);
			Add(FrameBufferFunction = new TI.Struct<shader_frame_buffer_function_struct>(this));
			Add(new TI.UnknownPad(32));
			Add(Bitmap = new TI.TagReference(this, TagGroups.bitm)); // pretty sure it's a bitm
			Add(new TI.Enum()); // value judges vertex shader permutation
			Add(TI.Pad.Word);
			Add(Animation = new TI.Struct<shader_animation_struct>(this));
		}
	};
	#endregion

	#region shader_environment
	[TI.TagGroup((int)TagGroups.Enumerated.senv, 2, /*shader_group.k_shader_size +*/ 796, typeof(shader_group))]
	public class shader_environment_group : shader_group
	{
		#region shader_environment_extension_block
		[TI.Definition()]
		public class shader_environment_extension_block : TI.Definition
		{
            #region Fields
            public TI.Flags Flags;
            public TI.Real BumpAmount;

			public TI.TagReference SpecularColorMap;
			public TI.Real SpecularColorCoefficient;
			public TI.Real SpecularColorExponent;
			public TI.Flags SpecularColorFlags;

			public TI.Real PerpendicularBrightness;
			public TI.RealColor PerpendicularTintColor;
			public TI.Real ParallelBrightness;
			public TI.RealColor ParallelTintColor;

			public TI.Real SpecularLightingExponent;
			public TI.Real SpecularLightingCoefficient;
			#endregion

			public shader_environment_extension_block()
				: base(15)
            {
                Add(Flags = TI.Flags.Word);
				Add(new TI.Pad(2));
				Add(BumpAmount = new TI.Real());

				Add(SpecularColorMap = new TI.TagReference(this, TagGroups.bitm));
				Add(SpecularColorCoefficient = new TI.Real());
				Add(SpecularColorExponent = new TI.Real());
				Add(SpecularColorFlags = TI.Flags.Word);
				Add(new TI.Pad(2));

				Add(PerpendicularBrightness = TI.Real.Fraction);
				Add(PerpendicularTintColor = new TI.RealColor());
				Add(ParallelBrightness = TI.Real.Fraction);
				Add(ParallelTintColor = new TI.RealColor());

				Add(SpecularLightingExponent = new TI.Real());
				Add(SpecularLightingCoefficient = new TI.Real());

				Add(new TI.Pad(24));
			}
		}
		#endregion

		#region Fields
		public TI.Flags Flags;
		public TI.Enum Type;
		public TI.Real LensFlareSpacing;
		public TI.TagReference LensFlare;

		public TI.Block<shader_environment_extension_block> ShaderExtension;

		public TI.Flags DiffuseFlags;
		public TI.TagReference BaseMap;
		public TI.Enum DetailMapFunctions;
		public TI.Real PrimaryDetailMapScale;
		public TI.TagReference PrimaryDetailMap;
		public TI.Real SecondaryDetailMapScale;
		public TI.TagReference SecondaryDetailMap;
		public TI.Enum MicroDetailMapFunction;
		public TI.Real MicroDetailMapScale;
		public TI.TagReference MicroDetailMap;
		public TI.RealColor MaterialColor;

		public TI.Real BumpMapScale;
		public TI.TagReference BumpMap;

		public TI.Struct<shader_texture_animation_function_struct> UAnimation, VAnimation;

		public TI.Flags SelfIlluminationFlags;
		public TI.Struct<shader_color_function_struct> PrimaryAnimation, SecondaryAnimation, PlasmaAnimation;
		public TI.Real MapScale;
		public TI.TagReference Map;

		public TI.Flags SpecularFlags;
		public TI.Real Brightness;
		public TI.RealColor PerpendicularColor;
		public TI.RealColor ParallelColor;

		public TI.Flags ReflectionFlags;
		public TI.Enum ReflectionType;
		public TI.Real LightmapBrightnessScale;
		public TI.Real PerpendicularBrightness;
		public TI.Real ParallelBrightness;
		public TI.TagReference ReflectionCubeMap;
		#endregion

		public shader_environment_group() : base(54)
		{
			Add(Flags = TI.Flags.Word);
			Add(Type = new TI.Enum());
			Add(LensFlareSpacing = new TI.Real());
			Add(LensFlare = new TI.TagReference(this, TagGroups.lens));
			Add(ShaderExtension = new TI.Block<shader_environment_extension_block>(this, 1));
			Add(new TI.Pad(32));

			Add(DiffuseFlags = TI.Flags.Word);
			Add(new TI.Pad(2 + 24));
			Add(BaseMap = new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Pad(24));
			Add(DetailMapFunctions = new TI.Enum());
			Add(new TI.Pad(2));
			Add(PrimaryDetailMapScale = new TI.Real());
			Add(PrimaryDetailMap = new TI.TagReference(this, TagGroups.bitm));
			Add(SecondaryDetailMapScale = new TI.Real());
			Add(SecondaryDetailMap = new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Pad(24));
			Add(MicroDetailMapFunction = new TI.Enum());
			Add(new TI.Pad(2));
			Add(MicroDetailMapScale = new TI.Real());
			Add(MicroDetailMap = new TI.TagReference(this, TagGroups.bitm));
			Add(MaterialColor = new TI.RealColor());

			Add(new TI.Pad(12));
			Add(BumpMapScale = new TI.Real());
			Add(BumpMap = new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Pad(8 + 16));

			Add(UAnimation = new TI.Struct<shader_texture_animation_function_struct>(this));
			Add(VAnimation = new TI.Struct<shader_texture_animation_function_struct>(this));
			Add(new TI.Pad(24));

			Add(SelfIlluminationFlags = TI.Flags.Word);
			Add(new TI.Pad(2 + 24));
			Add(PrimaryAnimation = new TI.Struct<shader_color_function_struct>(this));
			Add(SecondaryAnimation = new TI.Struct<shader_color_function_struct>(this));
			Add(PlasmaAnimation = new TI.Struct<shader_color_function_struct>(this));
			Add(MapScale = new TI.Real());
			Add(Map = new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Pad(24));

			Add(SpecularFlags = TI.Flags.Word);
			Add(new TI.Pad(2 + 16));
			Add(Brightness = TI.Real.Fraction);
			Add(new TI.Pad(20));
			Add(PerpendicularColor = new TI.RealColor());
			Add(ParallelColor = new TI.RealColor());
			Add(new TI.Pad(16));

			Add(ReflectionFlags = TI.Flags.Word);
			Add(ReflectionType = new TI.Enum());
			Add(LightmapBrightnessScale = TI.Real.Fraction);
			Add(new TI.Pad(28));
			Add(PerpendicularBrightness = TI.Real.Fraction);
			Add(ParallelBrightness = TI.Real.Fraction);
			Add(new TI.Pad(16 + 8 + 16));
			Add(ReflectionCubeMap = new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Pad(16));
		}
	};
	#endregion

	#region shader_model
	[TI.TagGroup((int)TagGroups.Enumerated.soso, 2, /*shader_group.k_shader_size +*/ 400, typeof(shader_group))]
	public class shader_model_group : shader_group
	{
		#region shader_model_extension_block
		[TI.Definition()]
		public class shader_model_extension_block : TI.Definition
		{
			#region Fields
			public TI.TagReference SpecularColorMap;
			public TI.Real SpecularColorCoefficient;
			public TI.Real SpecularColorExponent;
			
			public TI.Flags SpecularColorFlags;
			public TI.TagReference BaseNormalMap;
			public TI.Real BaseNormalCoefficient;

			public TI.TagReference DetailNormal1Map;
			public TI.Real DetailNormal1Coefficient;
			public TI.Real DetailNormal1Scale;
			public TI.Real DetailNormal1VScale;

			public TI.TagReference DetailNormal2Map;
			public TI.Real DetailNormal2Coefficient;
			public TI.Real DetailNormal2Scale;
			public TI.Real DetailNormal2VScale;

			public TI.Real PerpendicularBrightness;
			public TI.RealColor PerpendicularTintColor;
			public TI.Real ParallelBrightness;
			public TI.RealColor ParallelTintColor;

			public TI.Real SpecularLightingCoefficient;
			public TI.Real SpecularLightingExponent;
			#endregion

			public shader_model_extension_block()
				: base(27)
			{
				Add(SpecularColorMap = new TI.TagReference(this, TagGroups.bitm));
				Add(SpecularColorCoefficient = new TI.Real());
				Add(new TI.Pad(4));
				Add(SpecularColorExponent = new TI.Real());
			
				Add(SpecularColorFlags = TI.Flags.Word);
				Add(new TI.Pad(2));
				Add(BaseNormalMap = new TI.TagReference(this, TagGroups.bitm));
				Add(BaseNormalCoefficient = new TI.Real());

				Add(new TI.Pad(4* 3));

				Add(DetailNormal1Map = new TI.TagReference(this, TagGroups.bitm));
				Add(DetailNormal1Coefficient = new TI.Real());
				Add(DetailNormal1Scale = new TI.Real());
				Add(DetailNormal1VScale = new TI.Real());
				Add(new TI.Pad(4));

				Add(DetailNormal2Map = new TI.TagReference(this, TagGroups.bitm));
				Add(DetailNormal2Coefficient = new TI.Real());
				Add(DetailNormal2Scale = new TI.Real());
				Add(DetailNormal2VScale = new TI.Real());
				Add(new TI.Pad(4));

				Add(PerpendicularBrightness = new TI.Real());
				Add(PerpendicularTintColor = new TI.RealColor(TI.FieldType.RealRgbColor));
				Add(ParallelBrightness = new TI.Real());
				Add(ParallelTintColor = new TI.RealColor(TI.FieldType.RealRgbColor));

				Add(new TI.Pad(4 * 2));

				Add(SpecularLightingCoefficient = new TI.Real());
				Add(SpecularLightingExponent = new TI.Real());

				Add(new TI.Pad(16));
			}
		}
		#endregion

		#region Fields
		public TI.Flags Flags;
		public TI.Real Translucency;

		public TI.Enum ChangeColorSource;

		public TI.Flags SelfIlluminationFlags;
		public TI.Enum ColorSource;
		public TI.Enum AnimationFunction;
		public TI.Real AnimationPeriod;
		public TI.RealColor AnimationColorLowerBound, AnimationColorUpperBound;

		public TI.Real MapUScale, MapVScale;
		public TI.TagReference BaseMap;
		public TI.TagReference MultipurposeMap;
		public TI.Enum DetailFunction, DetailMask;
		public TI.Real DetailMapScale;
		public TI.TagReference DetailMap;
		public TI.Real DetailMapVScale;

		public TI.Block<shader_model_extension_block> ShaderExtension;

		public TI.Struct<shader_animation_struct> Animation;

		public TI.Real ReflectionFalloffDist;
		public TI.Real ReflectionCutoffDist;
		public TI.Real PerpendicularBrightness;
		public TI.RealColor PerpendicularTintColor;
		public TI.Real ParallelBrightness;
		public TI.RealColor ParallelTintColor;
		public TI.TagReference ReflectionCubemap;
		#endregion

		public shader_model_group() : base(36)
		{
			Add(Flags = TI.Flags.Word);
			Add(new TI.Pad(2 + 12));
			Add(Translucency = TI.Real.Fraction);
			Add(new TI.Pad(16));
			Add(ChangeColorSource = new TI.Enum());
			Add(new TI.Pad(2 + 28));

			Add(SelfIlluminationFlags = TI.Flags.Word);
			Add(new TI.Pad(2));
			Add(ColorSource = new TI.Enum());
			Add(AnimationFunction = new TI.Enum());
			Add(AnimationPeriod = new TI.Real());
			Add(AnimationColorLowerBound = new TI.RealColor());
			Add(AnimationColorUpperBound = new TI.RealColor());
			Add(new TI.Pad(12));

			Add(MapUScale = new TI.Real());
			Add(MapVScale = new TI.Real());
			Add(BaseMap = new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Pad(8));
			Add(MultipurposeMap = new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Pad(8));
			Add(DetailFunction = new TI.Enum());
			Add(DetailMask = new TI.Enum());
			Add(DetailMapScale = new TI.Real());
			Add(DetailMap = new TI.TagReference(this, TagGroups.bitm));
			Add(DetailMapVScale = new TI.Real());
			Add(ShaderExtension = new TI.Block<shader_model_extension_block>(this, 1));
			Add(Animation = new TI.Struct<shader_animation_struct>(this));
			Add(new TI.Pad(8));
			
			Add(ReflectionFalloffDist = new TI.Real());
			Add(ReflectionCutoffDist = new TI.Real());
			Add(PerpendicularBrightness = TI.Real.Fraction);
			Add(PerpendicularTintColor = new TI.RealColor());
			Add(ParallelBrightness = TI.Real.Fraction);
			Add(ParallelTintColor = new TI.RealColor());
			Add(ReflectionCubemap = new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Pad(16 + 4 + 16 + 32));
		}
	};
	#endregion

	#region shader_transparent_chicago
	[TI.TagGroup((int)TagGroups.Enumerated.schi, 1, /*shader_group.k_shader_size +*/ 68, typeof(shader_group))]
	public class shader_transparent_chicago_group : shader_group
	{
		// shader_transparent_layer_block, field_block<TI.TagReference>

		#region shader_transparent_chicago_map_block
		[TI.Definition(-1, 220)]
		public class shader_transparent_chicago_map_block : TI.Definition
		{
			#region Fields
			public TI.Flags Flags;
			public TI.Enum ColorFunction;
			public TI.Enum AlphaFunction;
			public TI.Real MapUScale, MapVScale;
			public TI.Real MapUOffset, MapVOffset;
			public TI.Real MapRotation;
			public TI.Real MipmapBias;
			public TI.TagReference Map;

			public TI.Struct<shader_animation_struct> Animation;
			#endregion

			public shader_transparent_chicago_map_block() : base(17)
			{
				Add(Flags = TI.Flags.Word);
				Add(new TI.Pad(2 + 40));
				Add(ColorFunction = new TI.Enum());
				Add(AlphaFunction = new TI.Enum());
				Add(new TI.Pad(36));
				Add(MapUScale = new TI.Real());
				Add(MapVScale = new TI.Real());
				Add(MapUOffset = new TI.Real());
				Add(MapVOffset = new TI.Real());
				Add(MapRotation = new TI.Real());
				Add(MipmapBias = TI.Real.Fraction);
				Add(Map = new TI.TagReference(this, TagGroups.bitm));
				Add(new TI.Pad(40));

				Add(Animation = new TI.Struct<shader_animation_struct>(this));
			}
		};
		#endregion

		#region Fields
		public TI.Struct<shader_radiosity_properties_struct> Radiosity;

		public TI.Real LensFlareSpacing;
		public TI.TagReference LensFlare;
		public TI.Block<field_block<TI.TagReference>> ExtraLayers;
		public TI.Block<shader_transparent_chicago_map_block> Maps;
		public TI.Flags ExtraFlags;
		#endregion

		public shader_transparent_chicago_group() : base(7)
		{
			Add(Radiosity = new TI.Struct<shader_radiosity_properties_struct>(this));

			Add(LensFlareSpacing = new TI.Real());
			Add(LensFlare = new TI.TagReference(this, TagGroups.lens));
			Add(ExtraLayers = new TI.Block<field_block<TI.TagReference>>(this, 4));
			Add(Maps = new TI.Block<shader_transparent_chicago_map_block>(this, 4));
			Add(ExtraFlags = new TI.Flags());
			Add(new TI.Pad(8));

			//ExtraLayers.Definition.Value.ResetReferenceGroupTag(TagGroups.shdr);
		}
	};
	#endregion

	#region shader_transparent_chicago_extended
	[TI.TagGroup((int)TagGroups.Enumerated.scex, 1, /*shader_group.k_shader_size +*/ 80, typeof(shader_group))]
	public class shader_transparent_chicago_extended_group : shader_group
	{
		#region Fields
		public TI.Struct<shader_radiosity_properties_struct> Radiosity;

		public TI.Real LensFlareSpacing;
		public TI.TagReference LensFlare;
		public TI.Block<field_block<TI.TagReference>> ExtraLayers;
		public TI.Block<shader_transparent_chicago_group.shader_transparent_chicago_map_block> _4StageMaps;
		public TI.Block<shader_transparent_chicago_group.shader_transparent_chicago_map_block> _2StageMaps;
		public TI.Flags ExtraFlags;
		#endregion

		public shader_transparent_chicago_extended_group() : base(8)
		{
			Add(Radiosity = new TI.Struct<shader_radiosity_properties_struct>(this));

			Add(LensFlareSpacing = new TI.Real());
			Add(LensFlare = new TI.TagReference(this, TagGroups.lens));
			Add(ExtraLayers = new TI.Block<field_block<TI.TagReference>>(this, 4));
			Add(_4StageMaps = new TI.Block<shader_transparent_chicago_group.shader_transparent_chicago_map_block>(this, 4));
			Add(_2StageMaps = new TI.Block<shader_transparent_chicago_group.shader_transparent_chicago_map_block>(this, 4));
			Add(ExtraFlags = new TI.Flags());
			Add(new TI.Pad(8));

			//ExtraLayers.Definition.Value.ResetReferenceGroupTag(TagGroups.shdr);
		}
	};
	#endregion

	#region shader_transparent_generic
	[TI.TagGroup((int)TagGroups.Enumerated.sotr, 1, /*shader_group.k_shader_size +*/ 68, typeof(shader_group))]
	public class shader_transparent_generic_group : shader_group
	{
		#region shader_transparent_generic_map_block
		[TI.Definition(-1, 100)]
		public class shader_transparent_generic_map_block : TI.Definition
		{
			#region Fields
			public TI.Flags Flags;
			public TI.Real MapUScale, MapVScale;
			public TI.Real MapUOffset, MapVOffset;
			public TI.Real MapRotation;
			public TI.Real MipmapBias;
			public TI.TagReference Map;

			public TI.Struct<shader_animation_struct> Animation;
			#endregion

			public shader_transparent_generic_map_block() : base(10)
			{
				Add(Flags = TI.Flags.Word);
				Add(new TI.Pad(2));
				Add(MapUScale = new TI.Real());
				Add(MapVScale = new TI.Real());
				Add(MapUOffset = new TI.Real());
				Add(MapVOffset = new TI.Real());
				Add(MapRotation = new TI.Real());
				Add(MipmapBias = TI.Real.Fraction);
				Add(Map = new TI.TagReference(this, TagGroups.bitm));
				Add(Animation = new TI.Struct<shader_animation_struct>(this));

			}
		};
		#endregion

		#region shader_transparent_generic_stage_block
		[TI.Definition(-1, 112)]
		public class shader_transparent_generic_stage_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public shader_transparent_generic_stage_block() : base(34)
			{
				Add(/*flags = */ TI.Flags.Word);
				Add(new TI.Pad(2));
				// Explanation here
				Add(/*color0 source = */ new TI.Enum());
				Add(/*color0 animation function = */ new TI.Enum());
				Add(/*color0 animation period = */ new TI.Real());
				Add(/*color0 animation lower bound = */ TI.RealColor.Argb);
				Add(/*color0 animation upper bound = */ TI.RealColor.Argb);
				Add(/*color1 = */ TI.RealColor.Argb);
				// Explanation here
				Add(/*input A = */ new TI.Enum());
				Add(/*input A mapping = */ new TI.Enum());
				Add(/*input B = */ new TI.Enum());
				Add(/*input B mapping = */ new TI.Enum());
				Add(/*input C = */ new TI.Enum());
				Add(/*input C mapping = */ new TI.Enum());
				Add(/*input D = */ new TI.Enum());
				Add(/*input D mapping = */ new TI.Enum());
				// Explanation here
				Add(/*output AB = */ new TI.Enum());
				Add(/*output AB function = */ new TI.Enum());
				Add(/*output CD = */ new TI.Enum());
				Add(/*output CD function = */ new TI.Enum());
				Add(/*output AB CD mux/sum = */ new TI.Enum());
				Add(/*output mapping = */ new TI.Enum());
				// Explanation here
				Add(/*input A = */ new TI.Enum());
				Add(/*input A mapping = */ new TI.Enum());
				Add(/*input B = */ new TI.Enum());
				Add(/*input B mapping = */ new TI.Enum());
				Add(/*input C = */ new TI.Enum());
				Add(/*input C mapping = */ new TI.Enum());
				Add(/*input D = */ new TI.Enum());
				Add(/*input D mapping = */ new TI.Enum());
				// Explanation here
				Add(/*output AB = */ new TI.Enum());
				Add(/*output CD = */ new TI.Enum());
				Add(/*output AB CD mux/sum = */ new TI.Enum());
				Add(/*output mapping = */ new TI.Enum());
			}
			#endregion
		}
		#endregion

		#region Fields
		public TI.Struct<shader_radiosity_properties_struct> Radiosity;

		public TI.Real LensFlareSpacing;
		public TI.TagReference LensFlare;
		public TI.Block<field_block<TI.TagReference>> ExtraLayers;
		public TI.Block<shader_transparent_generic_map_block> Maps;
		public TI.Block<shader_transparent_generic_stage_block> Stages;
		#endregion

		public shader_transparent_generic_group() : base(6)
		{
			Add(Radiosity = new TI.Struct<shader_radiosity_properties_struct>(this));

			Add(LensFlareSpacing = new TI.Real());
			Add(LensFlare = new TI.TagReference(this, TagGroups.lens));
			Add(ExtraLayers = new TI.Block<field_block<TI.TagReference>>(this, 4));
			Add(Maps = new TI.Block<shader_transparent_generic_map_block>(this, 4));
			Add(Stages = new TI.Block<shader_transparent_generic_stage_block>(this, 7));

			//ExtraLayers.Definition.Value.ResetReferenceGroupTag(TagGroups.shdr);
		}
	};
	#endregion

	#region shader_transparent_glass
	[TI.TagGroup((int)TagGroups.Enumerated.sgla, 1, /*shader_group.k_shader_size +*/ 440, typeof(shader_group))]
	public class shader_transparent_glass_group : shader_group
	{
		#region Fields
		public TI.Flags Flags;
		public TI.RealColor BackgroundTintColor;
		public TI.Real BackgroundTintMapScale;
		public TI.TagReference BackgroundTintMap;

		public TI.Enum ReflectionType;
		public TI.Real PerpendicularBrightness;
		public TI.RealColor PerpendicularTintColor;
		public TI.Real ParallelBrightness;
		public TI.RealColor ParallelTintColor;
		public TI.TagReference ReflectionMap;
		public TI.Real BumpMapScale;
		public TI.TagReference BumpMap;

		public TI.Real DiffuseMapScale;
		public TI.TagReference DiffuseMap;
		public TI.Real DiffuseDetailMapScale;
		public TI.TagReference DiffuseDetailMap;

		public TI.Real SpecularMapScale;
		public TI.TagReference SpecularMap;
		public TI.Real SpecularDetailMapScale;
		public TI.TagReference SpecularDetailMap;
		#endregion

		public shader_transparent_glass_group() : base(25)
		{
			Add(Flags = TI.Flags.Word);
			Add(new TI.Pad(2 + 40));
			Add(BackgroundTintColor = new TI.RealColor());
			Add(BackgroundTintMapScale = new TI.Real());
			Add(BackgroundTintMap = new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Pad(20 + 2));
			
			Add(ReflectionType = new TI.Enum());
			Add(PerpendicularBrightness = TI.Real.Fraction);
			Add(PerpendicularTintColor = new TI.RealColor());
			Add(ParallelBrightness = TI.Real.Fraction);
			Add(ParallelTintColor = new TI.RealColor());
			Add(ReflectionMap = new TI.TagReference(this, TagGroups.bitm));
			Add(BumpMapScale = new TI.Real());
			Add(BumpMap = new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Pad(128 + 4));
			Add(DiffuseMapScale = new TI.Real());
			Add(DiffuseMap = new TI.TagReference(this, TagGroups.bitm));
			Add(DiffuseDetailMapScale = new TI.Real());
			Add(DiffuseDetailMap = new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Pad(28 + 4));
			Add(SpecularMapScale = new TI.Real());
			Add(SpecularMap = new TI.TagReference(this, TagGroups.bitm));
			Add(SpecularDetailMapScale = new TI.Real());
			Add(SpecularDetailMap = new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Pad(28));
		}
	};
	#endregion

	#region shader_transparent_meter
	[TI.TagGroup((int)TagGroups.Enumerated.smet, 1, /*shader_group.k_shader_size +*/ 220, typeof(shader_group))]
	public class shader_transparent_meter_group : shader_group
	{
		#region Fields
		public TI.Flags Flags;
		public TI.TagReference Map;

		public TI.RealColor GradientMinColor, GradientMaxColor, 
			BackgroundColor, FlashColor, TintColor;
		public TI.Real MeterTransparency, BackgroundTransparency;

		public TI.Enum MeterBrightnessSource, FlashBrightnessSource, ValueSource, 
			GradientSource, FlashExtensionSource;
		#endregion

		public shader_transparent_meter_group() : base(18)
		{
			Add(Flags = TI.Flags.Word);
			Add(new TI.Pad(2 + 32));
			Add(Map = new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Pad(32));

			Add(GradientMinColor = new TI.RealColor());
			Add(GradientMaxColor = new TI.RealColor());
			Add(BackgroundColor = new TI.RealColor());
			Add(FlashColor = new TI.RealColor());
			Add(TintColor = new TI.RealColor());
			Add(MeterTransparency = TI.Real.Fraction);
			Add(BackgroundTransparency = TI.Real.Fraction);
			Add(new TI.Pad(24));

			Add(MeterBrightnessSource = new TI.Enum());
			Add(FlashBrightnessSource = new TI.Enum());
			Add(ValueSource = new TI.Enum());
			Add(GradientSource = new TI.Enum());
			Add(FlashExtensionSource = new TI.Enum());
			Add(new TI.Pad(2 + 32));
		}
	};
	#endregion

	#region shader_transparent_plasma
	[TI.TagGroup((int)TagGroups.Enumerated.spla, 1, /*shader_group.k_shader_size +*/ 292, typeof(shader_group))]
	public class shader_transparent_plasma_group : shader_group
	{
		#region Fields
		public TI.Enum IntensitySource;
		public TI.Real IntensityExponent;

		public TI.Enum OffsetSource;
		public TI.Real OffsetAmount, OffsetExponent;

		public TI.Real PerpendicularBrightness;
		public TI.RealColor PerpendicularTintColor;
		public TI.Real ParallelBrightness;
		public TI.RealColor ParallelTintColor;
		public TI.Enum ParallelTintColorSource;

		public TI.Real PrimaryAnimationPeriod;
		public TI.RealVector3D PrimaryAnimationDirection;
		public TI.Real PrimaryNoiseMapScale;
		public TI.TagReference PrimaryNoiseMap;

		public TI.Real SecondaryAnimationPeriod;
		public TI.RealVector3D SecondaryAnimationDirection;
		public TI.Real SecondaryNoiseMapScale;
		public TI.TagReference SecondaryNoiseMap;
		#endregion

		public shader_transparent_plasma_group() : base(25)
		{
			Add(new TI.Pad(2 + 2));
			Add(IntensitySource = new TI.Enum());
			Add(new TI.Pad(2));
			Add(IntensityExponent = new TI.Real());
			Add(OffsetSource = new TI.Enum());
			Add(new TI.Pad(2));
			Add(OffsetAmount = new TI.Real());
			Add(OffsetExponent = new TI.Real());
			Add(new TI.Pad(32));

			Add(PerpendicularBrightness = TI.Real.Fraction);
			Add(PerpendicularTintColor = new TI.RealColor());
			Add(ParallelBrightness = TI.Real.Fraction);
			Add(ParallelTintColor = new TI.RealColor());
			Add(ParallelTintColorSource = new TI.Enum());
			Add(new TI.Pad(2 + 32 + 2 + 2 + 16 + 4 + 4));
			Add(PrimaryAnimationPeriod = new TI.Real());
			Add(PrimaryAnimationDirection = new TI.RealVector3D());
			Add(PrimaryNoiseMapScale = new TI.Real());
			Add(PrimaryNoiseMap = new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Pad(32 + 4));
			Add(SecondaryAnimationPeriod = new TI.Real());
			Add(SecondaryAnimationDirection = new TI.RealVector3D());
			Add(SecondaryNoiseMapScale = new TI.Real());
			Add(SecondaryNoiseMap = new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Pad(32));
		}
	};
	#endregion

	#region shader_transparent_water
	[TI.TagGroup((int)TagGroups.Enumerated.swat, 2, /*shader_group.k_shader_size +*/ 280, typeof(shader_group))]
	public class shader_transparent_water_group : shader_group
	{
		#region shader_transparent_water_ripple_block
		[TI.Definition(-1, 76)]
		public class shader_transparent_water_ripple_block : TI.Definition
		{
			public TI.Real ContributionFactor, AnimationAngle, AnimationVelocity;
			public TI.RealVector2D MapOffset;
			public TI.ShortInteger MapRepeats, MapIndex;

			public shader_transparent_water_ripple_block() : base(9)
			{
				Add(new TI.Pad(2 + 2));
				Add(ContributionFactor = TI.Real.Fraction);
				Add(new TI.Pad(32));
				Add(AnimationAngle = TI.Real.Angle);
				Add(AnimationVelocity = new TI.Real());
				Add(MapOffset = new TI.RealVector2D());
				Add(MapRepeats = new TI.ShortInteger());
				Add(MapIndex = new TI.ShortInteger());
				Add(new TI.Pad(16));
			}
		};
		#endregion

		#region Fields
		public TI.Flags Flags;
		public TI.TagReference BaseMap;
		public TI.Real ViewPerpendicularBrightness;
		public TI.RealColor ViewPerpendicularTintColor;
		public TI.Real ViewParallelBrightness;
		public TI.RealColor ViewParallelTintColor;
		public TI.TagReference ReflectionMap;
		public TI.Real RippleAnimationAngle, RippleAnimationVelocity, RippleScale;
		public TI.TagReference RippleMaps;
		public TI.ShortInteger RippleMipmapLevels;
		public TI.Real RippleMipmapFadeFactor, RippleMipmapDetailBias;
		public TI.Block<shader_transparent_water_ripple_block> Ripples;
		#endregion

		public shader_transparent_water_group() : base(22)
		{
			Add(Flags = TI.Flags.Word);
			Add(new TI.Pad(2 + 32));
			Add(BaseMap = new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Pad(16));
			Add(ViewPerpendicularBrightness = TI.Real.Fraction);
			Add(ViewPerpendicularTintColor = new TI.RealColor());
			Add(ViewParallelBrightness = TI.Real.Fraction);
			Add(ViewParallelTintColor = new TI.RealColor());
			Add(new TI.Pad(16));
			Add(ReflectionMap = new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Pad(16));
			Add(RippleAnimationAngle = TI.Real.Angle);
			Add(RippleAnimationVelocity = new TI.Real());
			Add(RippleScale = new TI.Real());
			Add(RippleMaps = new TI.TagReference(this, TagGroups.bitm));
			Add(RippleMipmapLevels = new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(RippleMipmapFadeFactor = TI.Real.Fraction);
			Add(RippleMipmapDetailBias = new TI.Real());
			Add(new TI.Pad(64));
			Add(Ripples = new TI.Block<shader_transparent_water_ripple_block>(this, 4));
			Add(new TI.Pad(16));
		}
	};
	#endregion
}