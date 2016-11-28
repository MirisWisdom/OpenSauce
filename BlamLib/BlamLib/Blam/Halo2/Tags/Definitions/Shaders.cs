/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo2.Tags
{
	#region pixel_shader
	[TI.TagGroup((int)TagGroups.Enumerated.pixl, 1, 24)]
	public partial class pixel_shader_group : TI.Definition
	{
		public TI.Data CompiledShader;
	};
	#endregion

	public enum GeometryClassifcationNames // vertex_shader -> geometry classifications
	{
		World,
		Rigid,
		RigidBoned,
		SkinnedBone1,
		SkinnedBone2,
		SkinnedBone3,
		SkinnedBone4,
		Screen,
		Parallel,
		Perpendicular,
		Vertical,
		Horizontal,

		Count,
	};

	#region vertex_shader
	[TI.TagGroup((int)TagGroups.Enumerated.vrtx, 1, 20)]
	public partial class vertex_shader_group : TI.Definition
	{
		#region vertex_shader_classification_block
		[TI.Definition(1, 44)]
		public partial class vertex_shader_classification_block : TI.Definition
		{
			public TI.Data Compiled;
			public TI.Data Source;
		}
		#endregion

		public TI.Enum Platform;
		public TI.Block<vertex_shader_classification_block> GeometryClassifications;
		public TI.LongInteger OutputSwizzles;
	};
	#endregion


	class c_shader_postprocesser
	{
		enum ShaderTemplatePropertyType
		{
			Bitmap,
			Value,
			Color,
			Switch,
		};

		enum ShaderTemplateProperty
		{
			Unused,
			DiffuseMap,				// Bitmap
			LightmapEmissiveMap,	// Bitmap
			LightmapEmissiveColor,	// Color
			LightmapEmissivePower,	// Value
			LightmapResolutionScale,// Value
			LightmapHalfLife,		// Value
			LightmapDiffuseScale,	// Value
			LightmapAlphaTestMap,	// Bitmap
			LightmapTranslucentMap,	// Map
			LightmapTranslucentColor,	// Color
			LightmapTranslucentAlpha,	// Value
			ActiveCamoMap,			// Bitmap
			LightmapFoliageScale,	// Value
		};

		enum ShaderTemplatePassReferenceLayer
		{
			TexAccum,
			EnvironmentMap,
			SelfIllumination,
			Overlay,
			Transparent,
			Lightmap,		// Transparent Indirect
			Diffuse,		// Light Diffuse
			Specular,		// Light Specular
			ShadowGenerate,
			ShadowApply,
			Boom,			// Unused
			Fog,			// Unused
			ShPrt,			// Unused
			ActiveCamo,		// Unused
			WaterEdgeBlend,	// Unused
			ActiveCamoStencilModulate,	// Unused
			Hologram,		// Unused
			LightAlbedo,
		};

		enum ShaderPassTextureMode
		{
			_2d,
			_3d,
			CubeMap,
			Passthrough,
			TexKill,
			_2dDependentAR,
			_2dDependentGB,
			_2dBumpEnv,
			_2dBumpEnvLuminance,
			_3dBrdf,
			DotProduct,
			DotProduct2d,
			DotProduct3d,
			DotProductCubeMap,
			DotReflectZW,
			DotReflectDiffuse,
			DotReflectSpecular,
			DotReflectSpecularConst,
			None,
		};

		enum ShaderPassTextureSourceExtern
		{
			None,
			VectorNormalization,
			BumpAccum,		// Unused
			TexAccum,
			Utility,		// Unused
			LightAccum,		// Frame Buffer
			Z,
			FrontBuffer,	// Unused
			Shadow,
			LightFalloff,
			LightGel,
			Lightmap,
			E3LightTaco,	// Unused
			ShadowBuffer,
			GradientSeparate,
			GradientProduct,
			HudBitmap,
			ActiveCamo,
			TextureCamera,
			WaterReflection,
			WaterRefraction,
			Aux1,
			Aux2,
			ParticleDistortion,
			Convolution1,
			Convolution2,
			ActiveCamoBump,
			FirstPersonScope,
		};
	};

	#region global_shader_parameter_block
	[TI.Definition(1, 52)]
	public partial class global_shader_parameter_block : TI.Definition, IShaderParameter
	{
		#region shader_animation_property_block
		[TI.Definition(1, 28)]
		public partial class shader_animation_property_block : TI.Definition
		{
			public TI.Enum Type;
			public TI.StringId InputName;
			public TI.StringId RangeName;
			public TI.Real TimePeriod;
			public TI.Struct<mapping_function> Function;
		}
		#endregion

		#region Fields
		[TI.Field(TI.FieldType.StringId, "Name", IsBlockName = true)]
		public TI.StringId Name;
		
		[TI.EnumField(TI.FieldType.Enum, "Type",
							"Bitmap",
							"Value",
							"Color",
							"Switch")]
		public TI.Enum Type;

		[TI.FlagField(TI.FieldType.WordFlags, "Runtime Flags",
							"Animated")]
		public TI.Flags RuntimeFlags;

		[TI.ReferenceField("Bitmap", (int)TagGroups.Enumerated.bitm)]
		public TI.TagReference Bitmap;

		[TI.Field(TI.FieldType.Real, "Const Value")]
		public TI.Real ConstValue;

		[TI.Field(TI.FieldType.RealRgbColor, "Const Color")]
		public TI.RealColor ConstColor;

		[TI.BlockField(typeof(shader_animation_property_block), "Animation Properties", "animation property", 14)]
		public TI.Block<shader_animation_property_block> AnimationProperties;
		#endregion
	};
	#endregion


	#region shader_gpu_state_reference_struct
	[TI.Struct((int)StructGroups.Enumerated.GPUR, 1, 14)]
	public partial class shader_gpu_state_reference_struct : TI.Definition
	{
	}
	#endregion

	#region render_state_block
	[TI.Definition(1, 5)]
	public partial class render_state_block : TI.Definition
	{
		public TI.ByteInteger StateIndex;
		public TI.LongInteger StateValue;
	}
	#endregion

	#region shader_gpu_state_struct
	[TI.Struct((int)StructGroups.Enumerated.GPUS, 1, 84)]
	public partial class shader_gpu_state_struct : TI.Definition
	{
		#region texture_stage_state_block
		[TI.Definition(1, 6)]
		public partial class texture_stage_state_block : TI.Definition
		{
			public TI.ByteInteger StateIndex, StageIndex;
			public TI.LongInteger StateValue;
		}
		#endregion

		#region render_state_parameter_block
		[TI.Definition(1, 3)]
		public partial class render_state_parameter_block : TI.Definition
		{
		}
		#endregion

		#region texture_stage_state_parameter_block
		[TI.Definition(1, 4)]
		public partial class texture_stage_state_parameter_block : TI.Definition
		{
		}
		#endregion

		#region texture_block
		[TI.Definition(1, 4)]
		public partial class texture_block : TI.Definition
		{
		}
		#endregion

		#region vertex_shader_constant_block
		[TI.Definition(1, 4)]
		public partial class vertex_shader_constant_block : TI.Definition
		{
		}
		#endregion
	}
	#endregion


	#region shader_postprocess_definition_new_block
	[TI.Definition(1, 184)]
	public partial class shader_postprocess_definition_new_block : TI.Definition
	{
		#region shader_postprocess_bitmap_new_block
		[TI.Definition(1, 12)]
		public partial class shader_postprocess_bitmap_new_block : TI.Definition
		{
			public TI.LongInteger BitmapGroup;
			public TI.LongInteger BitmapIndex;
			public TI.Real LogBitmapDimension;
		}
		#endregion

		#region shader_postprocess_level_of_detail_new_block
		[TI.Definition(1, 6)]
		public partial class shader_postprocess_level_of_detail_new_block : TI.Definition
		{
			public TI.LongInteger AvailLayerFlags;
			public TI.Struct<tag_block_index_struct> Layers;
		}
		#endregion

		#region shader_postprocess_implementation_new_block
		[TI.Definition(1, 10)]
		public partial class shader_postprocess_implementation_new_block : TI.Definition
		{
		}
		#endregion

		#region shader_postprocess_overlay_new_block
		[TI.Definition(1, 24)]
		public partial class shader_postprocess_overlay_new_block : TI.Definition
		{
			public TI.StringId InputName;
			public TI.StringId RangeName;
			public TI.Real TimePeriod;
			public TI.Struct<scalar_function_struct> Function;
		}
		#endregion

		#region shader_postprocess_overlay_reference_new_block
		[TI.Definition(1, 4)]
		public partial class shader_postprocess_overlay_reference_new_block : TI.Definition
		{
			public TI.ShortInteger OverlayIndex; // parameters->animation properties->function inputs
			public TI.ShortInteger TransformIndex; // parameters->animation properties->type
		}
		#endregion

		#region shader_postprocess_animated_parameter_new_block
		[TI.Definition(1, 2)]
		public partial class shader_postprocess_animated_parameter_new_block : TI.Definition
		{
		}
		#endregion

		#region shader_postprocess_animated_parameter_reference_new_block
		[TI.Definition(1, 4)]
		public partial class shader_postprocess_animated_parameter_reference_new_block : TI.Definition
		{
		}
		#endregion

		#region shader_postprocess_bitmap_property_block
		[TI.Definition(1, 4)]
		public partial class shader_postprocess_bitmap_property_block : TI.Definition
		{
		}
		#endregion

		#region shader_postprocess_color_property_block
		[TI.Definition(1, 12)]
		public partial class shader_postprocess_color_property_block : TI.Definition
		{
		}
		#endregion

		#region shader_postprocess_value_property_block
		[TI.Definition(1, 4)]
		public partial class shader_postprocess_value_property_block : TI.Definition
		{
		}
		#endregion

		#region shader_postprocess_level_of_detail_block
		[TI.Definition(1, 224)]
		public partial class shader_postprocess_level_of_detail_block : TI.Definition
		{
			#region shader_postprocess_layer_block
			[TI.Definition(1, 2)]
			public partial class shader_postprocess_layer_block : TI.Definition
			{
			}
			#endregion

			#region shader_postprocess_pass_block
			[TI.Definition(1, 18)]
			public partial class shader_postprocess_pass_block : TI.Definition
			{
			}
			#endregion

			#region shader_postprocess_implementation_block
			[TI.Definition(1, 44)]
			public partial class shader_postprocess_implementation_block : TI.Definition
			{
			}
			#endregion

			#region shader_postprocess_bitmap_block
			[TI.Definition(1, 10)]
			public partial class shader_postprocess_bitmap_block : TI.Definition
			{
			}
			#endregion

			#region shader_postprocess_bitmap_transform_block
			[TI.Definition(1, 6)]
			public partial class shader_postprocess_bitmap_transform_block : TI.Definition
			{
			}
			#endregion

			#region shader_postprocess_value_block
			[TI.Definition(1, 5)]
			public partial class shader_postprocess_value_block : TI.Definition
			{
			}
			#endregion

			#region shader_postprocess_color_block
			[TI.Definition(1, 13)]
			public partial class shader_postprocess_color_block : TI.Definition
			{
			}
			#endregion

			#region shader_postprocess_bitmap_transform_overlay_block
			[TI.Definition(1, 27)]
			public partial class shader_postprocess_bitmap_transform_overlay_block : TI.Definition
			{
			}
			#endregion

			#region shader_postprocess_value_overlay_block
			[TI.Definition(1, 25)]
			public partial class shader_postprocess_value_overlay_block : TI.Definition
			{
			}
			#endregion

			#region shader_postprocess_color_overlay_block
			[TI.Definition(1, 25)]
			public partial class shader_postprocess_color_overlay_block : TI.Definition
			{
			}
			#endregion

			#region shader_postprocess_vertex_shader_constant_block
			[TI.Definition(1, 18)]
			public partial class shader_postprocess_vertex_shader_constant_block : TI.Definition
			{
			}
			#endregion
		}
		#endregion
	}
	#endregion


	#region shader
	[TI.TagGroup((int)TagGroups.Enumerated.shad, 1, 128)]
	public partial class shader_group : TI.Definition
	{
		#region shader_properties_block
		[TI.Definition(1, 112)]
		public partial class shader_properties_block : TI.Definition
		{
			public TI.TagReference DiffuseMap;
			public TI.TagReference LightmapEmissiveMap;
			public TI.RealColor LightmapEmissiveColor;
			public TI.Real LightmapEmissivePower;
			public TI.Real LightmapResolutionScale;
			public TI.Real LightmapHalfLife;
			public TI.Real LightmapDiffuseScale;
			public TI.TagReference AlphaTestMap;
			public TI.TagReference TranslucentMap;
			public TI.RealColor LightmapTransparentColor;
			public TI.Real LightmapTransparentAlpha;
			public TI.Real LightmapFoliageScale;
		}
		#endregion

		#region long_block
		[TI.Definition(1, 4)]
		public partial class long_block : TI.Definition
		{
			public TI.LongInteger BitmapGroupIndex;
		}
		#endregion

		public TI.TagReference Template;
		public TI.StringId MaterialName;
		public TI.Block<shader_properties_block> RuntimeProperties;
		public TI.Flags RuntimeFlags;
		public TI.Flags Flags;
		public TI.Block<global_shader_parameter_block> Parameters;
		public TI.Block<shader_postprocess_definition_new_block> PostprocessDefinition;
		public TI.Block<predicted_resource_block> PredictedResources;
		public TI.TagReference LightResponse;
		public TI.Enum ShaderLodBias;
		public TI.Enum SpecularType;
		public TI.Enum LightmapType;
		public TI.Real LightmapSpecularBrightness;
		public TI.Real LightmapAmbientBias;
		public TI.Block<long_block> PostprocessProperties;
		public TI.Real AddedDepthBiasOffset;
		public TI.Real AddedDepthBiasSlopeScale;
	};
	#endregion


	#region shader_template_category_block
	[TI.Definition(1, 16)]
	public partial class shader_template_category_block : TI.Definition
	{
		#region shader_template_parameter_block
		[TI.Definition(1, 72)]
		public partial class shader_template_parameter_block : TI.Definition, IShaderParameter
		{
			public TI.StringId Name;
			public TI.Enum Type;
			public TI.TagReference DefaultBitmap;
			public TI.Real DefaultConstValue;
			public TI.RealColor DefaultConstColor;
		}
		#endregion

		public TI.StringId Name;
		public TI.Block<shader_template_parameter_block> Parameters;
	}
	#endregion

	#region shader_template_level_of_detail_block
	[TI.Definition(1, 16)]
	public partial class shader_template_level_of_detail_block : TI.Definition
	{
		#region shader_template_pass_reference_block
		[TI.Definition(1, 32)]
		public partial class shader_template_pass_reference_block : TI.Definition
		{
		}
		#endregion
	}
	#endregion


	#region shader_light_response
	[TI.TagGroup((int)TagGroups.Enumerated.slit, 2, 28)]
	public partial class shader_light_response_group : TI.Definition
	{
	};
	#endregion


	#region shader_pass_implementation_block
	[TI.Definition(1, 184)]
	public partial class shader_pass_implementation_block : TI.Definition
	{
		#region shader_texture_state_address_state_block
		[TI.Definition(1, 8)]
		public partial class shader_texture_state_address_state_block : TI.Definition
		{
		}
		#endregion

		#region shader_texture_state_filter_state_block
		[TI.Definition(1, 16)]
		public partial class shader_texture_state_filter_state_block : TI.Definition
		{
		}
		#endregion

		#region shader_texture_state_kill_state_block
		[TI.Definition(1, 12)]
		public partial class shader_texture_state_kill_state_block : TI.Definition
		{
		}
		#endregion

		#region shader_texture_state_misc_state_block
		[TI.Definition(1, 8)]
		public partial class shader_texture_state_misc_state_block : TI.Definition
		{
		}
		#endregion

		#region shader_texture_state_constant_block
		[TI.Definition(1, 8)]
		public partial class shader_texture_state_constant_block : TI.Definition
		{
		}
		#endregion

		#region shader_pass_texture_block
		[TI.Definition(1, 80)]
		public partial class shader_pass_texture_block : TI.Definition
		{
		}
		#endregion

		#region shader_pass_vertex_shader_constant_block
		[TI.Definition(1, 12)]
		public partial class shader_pass_vertex_shader_constant_block : TI.Definition
		{
		}
		#endregion

		#region shader_state_channels_state_block
		[TI.Definition(1, 4)]
		public partial class shader_state_channels_state_block : TI.Definition
		{
		}
		#endregion

		#region shader_state_alpha_blend_state_block
		[TI.Definition(1, 16)]
		public partial class shader_state_alpha_blend_state_block : TI.Definition
		{
		}
		#endregion

		#region shader_state_alpha_test_state_block
		[TI.Definition(1, 8)]
		public partial class shader_state_alpha_test_state_block : TI.Definition
		{
		}
		#endregion

		#region shader_state_depth_state_block
		[TI.Definition(1, 16)]
		public partial class shader_state_depth_state_block : TI.Definition
		{
		}
		#endregion

		#region shader_state_cull_state_block
		[TI.Definition(1, 4)]
		public partial class shader_state_cull_state_block : TI.Definition
		{
		}
		#endregion

		#region shader_state_fill_state_block
		[TI.Definition(1, 12)]
		public partial class shader_state_fill_state_block : TI.Definition
		{
		}
		#endregion

		#region shader_state_misc_state_block
		[TI.Definition(1, 8)]
		public partial class shader_state_misc_state_block : TI.Definition
		{
		}
		#endregion

		#region shader_state_constant_block
		[TI.Definition(1, 8)]
		public partial class shader_state_constant_block : TI.Definition
		{
		}
		#endregion
	}
	#endregion

	#region shader_pass_postprocess_definition_new_block
	[TI.Definition(1, 132)]
	public partial class shader_pass_postprocess_definition_new_block : TI.Definition
	{
		#region shader_pass_postprocess_implementation_new_block
		[TI.Definition(1, 350)]
		public partial class shader_pass_postprocess_implementation_new_block : TI.Definition
		{
			#region shader_postprocess_pixel_shader_constant_defaults
			[TI.Definition(1, 4)]
			public partial class shader_postprocess_pixel_shader_constant_defaults : TI.Definition
			{
			}
			#endregion

			#region shader_postprocess_pixel_shader
			[TI.Definition(1, 84)]
			public partial class shader_postprocess_pixel_shader : TI.Definition
			{
			}
			#endregion

			#region pixel_shader_extern_map_block
			[TI.Definition(1, 2)]
			public partial class pixel_shader_extern_map_block : TI.Definition
			{
			}
			#endregion

			#region pixel_shader_index_block
			[TI.Definition(1, 1)]
			public partial class pixel_shader_index_block : TI.Definition
			{
			}
			#endregion
		}
		#endregion

		#region shader_pass_postprocess_texture_new_block
		[TI.Definition(1, 4)]
		public partial class shader_pass_postprocess_texture_new_block : TI.Definition
		{
		}
		#endregion

		#region shader_pass_postprocess_texture_state_block
		[TI.Definition(1, 24)]
		public partial class shader_pass_postprocess_texture_state_block : TI.Definition
		{
		}
		#endregion

		#region pixel_shader_fragment_block
		[TI.Definition(1, 3)]
		public partial class pixel_shader_fragment_block : TI.Definition
		{
		}
		#endregion

		#region pixel_shader_permutation_new_block
		[TI.Definition(1, 6)]
		public partial class pixel_shader_permutation_new_block : TI.Definition
		{
		}
		#endregion

		#region pixel_shader_combiner_block
		[TI.Definition(1, 32)]
		public partial class pixel_shader_combiner_block : TI.Definition
		{
		}
		#endregion

		#region shader_pass_postprocess_extern_new_block
		[TI.Definition(1, 4)]
		public partial class shader_pass_postprocess_extern_new_block : TI.Definition
		{
		}
		#endregion

		#region shader_pass_postprocess_constant_new_block
		[TI.Definition(1, 7)]
		public partial class shader_pass_postprocess_constant_new_block : TI.Definition
		{
		}
		#endregion

		#region shader_pass_postprocess_constant_info_new_block
		[TI.Definition(1, 7)]
		public partial class shader_pass_postprocess_constant_info_new_block : TI.Definition
		{
		}
		#endregion

		#region texture_stage_state_block
		[TI.Definition(1, 6)]
		public partial class texture_stage_state_block : TI.Definition
		{
		}
		#endregion

		#region extern_reference_block
		[TI.Definition(1, 2)]
		public partial class extern_reference_block : TI.Definition
		{
		}
		#endregion

		#region pixel_shader_permutation_block
		[TI.Definition(1, 16)]
		public partial class pixel_shader_permutation_block : TI.Definition
		{
		}
		#endregion

		#region pixel_shader_constant_block
		[TI.Definition(1, 6)]
		public partial class pixel_shader_constant_block : TI.Definition
		{
		}
		#endregion

		#region shader_pass_postprocess_implementation_block
		[TI.Definition(1, 502)]
		public partial class shader_pass_postprocess_implementation_block : TI.Definition
		{
		}
		#endregion
	}
	#endregion

	#region shader_pass
	[TI.TagGroup((int)TagGroups.Enumerated.spas, 1, 60)]
	public partial class shader_pass_group : TI.Definition
	{
		#region shader_pass_parameter_block
		[TI.Definition(1, 64)]
		public partial class shader_pass_parameter_block : TI.Definition
		{
		}
		#endregion
	};
	#endregion


	#region shader_template
	[TI.TagGroup((int)TagGroups.Enumerated.stem, 1, 156)]
	public partial class shader_template_group : TI.Definition
	{
		#region shader_template_property_block
		[TI.Definition(1, 8)]
		public partial class shader_template_property_block : TI.Definition
		{
		}
		#endregion

		#region shader_template_runtime_external_light_response_index_block
		[TI.Definition(1, 4)]
		public partial class shader_template_runtime_external_light_response_index_block : TI.Definition
		{
		}
		#endregion

		#region shader_template_postprocess_definition_new_block
		[TI.Definition(1, 60)]
		public partial class shader_template_postprocess_definition_new_block : TI.Definition
		{
			#region shader_template_postprocess_level_of_detail_new_block
			[TI.Definition(1, 10)]
			public partial class shader_template_postprocess_level_of_detail_new_block : TI.Definition
			{
			}
			#endregion

			#region shader_template_postprocess_pass_new_block
			[TI.Definition(1, 18)]
			public partial class shader_template_postprocess_pass_new_block : TI.Definition
			{
			}
			#endregion

			#region shader_template_postprocess_implementation_new_block
			[TI.Definition(1, 6)]
			public partial class shader_template_postprocess_implementation_new_block : TI.Definition
			{
			}
			#endregion

			#region shader_template_postprocess_remapping_new_block
			[TI.Definition(1, 4)]
			public partial class shader_template_postprocess_remapping_new_block : TI.Definition
			{
			}
			#endregion
		}
		#endregion

		public TI.Block<shader_template_category_block> Categories;
		TI.Block<shader_template_postprocess_definition_new_block> PostprocessDefinition;
	};
	#endregion
};