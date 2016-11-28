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
	partial class pixel_shader_group
	{
		public pixel_shader_group() : base(2)
		{
			Add(new TI.Pad(4));
			Add(CompiledShader = new TI.Data(this));
		}
	};
	#endregion

	#region vertex_shader
	partial class vertex_shader_group
	{
		#region vertex_shader_classification_block
		partial class vertex_shader_classification_block
		{
			public vertex_shader_classification_block() : base(3)
			{
				Add(new TI.Pad(4));
				Add(Compiled = new TI.Data(this));
				Add(Source = new TI.Data(this));
			}
		};
		#endregion

		public vertex_shader_group() : base(4)
		{
			Add(Platform = new TI.Enum());
			Add(new TI.Pad(2));
			Add(GeometryClassifications = new TI.Block<vertex_shader_classification_block>(this, (int)GeometryClassifcationNames.Count));
			Add(OutputSwizzles = new TI.LongInteger());
		}
	};
	#endregion


	#region global_shader_parameter_block
	partial class global_shader_parameter_block
	{
		#region shader_animation_property_block
		partial class shader_animation_property_block
		{
			public shader_animation_property_block() : base(6)
			{
				Add(Type = new TI.Enum());
				Add(TI.Pad.Word);
				Add(InputName = new TI.StringId());
				Add(RangeName = new TI.StringId());
				Add(TimePeriod = new TI.Real());
				Add(Function = new TI.Struct<mapping_function>(this));
			}
		};
		#endregion

		public global_shader_parameter_block() : base(7)
		{
			/*0x00*/Add(Name = new TI.StringId());
			/*0x04*/Add(Type = new TI.Enum());
			/*0x06*/Add(RuntimeFlags = new TI.Flags(TI.FieldType.WordFlags)); // was Pad
			/*0x08*/Add(Bitmap = new TI.TagReference(this, TagGroups.bitm));
			/*0x18*/Add(ConstValue = new TI.Real());
			/*0x1C*/Add(ConstColor = new TI.RealColor());
			/*0x28*/Add(AnimationProperties = new TI.Block<shader_animation_property_block>(this, 14));
		}
	};
	#endregion


	#region shader_gpu_state_reference_struct
	partial class shader_gpu_state_reference_struct
	{
		public shader_gpu_state_reference_struct() : base(7)
		{
			Add(/*render states = */ new TI.Struct<tag_block_index_struct>(this));
			Add(/*texture stage states = */ new TI.Struct<tag_block_index_struct>(this));
			Add(/*render state parameters = */ new TI.Struct<tag_block_index_struct>(this));
			Add(/*texture stage parameters = */ new TI.Struct<tag_block_index_struct>(this));
			Add(/*textures = */ new TI.Struct<tag_block_index_struct>(this));
			Add(/*Vn Constants = */ new TI.Struct<tag_block_index_struct>(this));
			Add(/*Cn Constants = */ new TI.Struct<tag_block_index_struct>(this));
		}
	};
	#endregion

	#region render_state_block
	partial class render_state_block
	{
		public render_state_block() : base(2)
		{
			Add(StateIndex = new TI.ByteInteger());
			Add(StateValue = new TI.LongInteger());
		}
	};
	#endregion

	#region shader_gpu_state_struct
	partial class shader_gpu_state_struct
	{
		#region texture_stage_state_block
		partial class texture_stage_state_block
		{
			public texture_stage_state_block() : base(3)
			{
				Add(StateIndex = new TI.ByteInteger());
				Add(StageIndex = new TI.ByteInteger());
				Add(StateValue = new TI.LongInteger());
			}
		};
		#endregion

		#region render_state_parameter_block
		partial class render_state_parameter_block
		{
			public render_state_parameter_block() : base(3)
			{
				Add(/*parameter index = */ new TI.ByteInteger());
				Add(/*parameter type = */ new TI.ByteInteger());
				Add(/*state index = */ new TI.ByteInteger());
			}
		};
		#endregion

		#region texture_stage_state_parameter_block
		partial class texture_stage_state_parameter_block
		{
			public texture_stage_state_parameter_block() : base(4)
			{
				Add(/*parameter index = */ new TI.ByteInteger());
				Add(/*parameter type = */ new TI.ByteInteger());
				Add(/*state index = */ new TI.ByteInteger());
				Add(/*stage index = */ new TI.ByteInteger());
			}
		};
		#endregion

		#region texture_block
		partial class texture_block
		{
			public texture_block() : base(4)
			{
				Add(/*stage index = */ new TI.ByteInteger());
				Add(/*parameter index = */ new TI.ByteInteger());
				Add(/*global texture index = */ new TI.ByteInteger());
				Add(/*flags = */ new TI.ByteInteger());
			}
		};
		#endregion

		#region vertex_shader_constant_block
		partial class vertex_shader_constant_block
		{
			public vertex_shader_constant_block() : base(4)
			{
				Add(/*register index = */ new TI.ByteInteger());
				Add(/*parameter index = */ new TI.ByteInteger());
				Add(/*destination mask = */ new TI.ByteInteger());
				Add(/*scale by texture stage = */ new TI.ByteInteger());
			}
		};
		#endregion

		public shader_gpu_state_struct() : base(7)
		{
			Add(/*render states = */ new TI.Block<render_state_block>(this, 1024));
			Add(/*texture stage states = */ new TI.Block<texture_stage_state_block>(this, 1024));
			Add(/*render state parameters = */ new TI.Block<render_state_parameter_block>(this, 1024));
			Add(/*texture stage parameters = */ new TI.Block<texture_stage_state_parameter_block>(this, 1024));
			Add(/*textures = */ new TI.Block<texture_block>(this, 1024));
			Add(/*Vn Constants = */ new TI.Block<vertex_shader_constant_block>(this, 1024));
			Add(/*Cn Constants = */ new TI.Block<vertex_shader_constant_block>(this, 1024));
		}
	};
	#endregion


	#region shader_postprocess_definition_new_block
	partial class shader_postprocess_definition_new_block
	{
		#region shader_postprocess_bitmap_new_block
		partial class shader_postprocess_bitmap_new_block
		{
			public shader_postprocess_bitmap_new_block() : base(3)
			{
				Add(BitmapGroup = new TI.LongInteger());
				Add(BitmapIndex = new TI.LongInteger());
				Add(LogBitmapDimension = new TI.Real());
			}
		};
		#endregion

		#region shader_postprocess_level_of_detail_new_block
		partial class shader_postprocess_level_of_detail_new_block
		{
			public shader_postprocess_level_of_detail_new_block() : base(2)
			{
				Add(AvailLayerFlags = new TI.LongInteger());
				Add(Layers = new TI.Struct<tag_block_index_struct>(this));
			}
		};
		#endregion

		#region shader_postprocess_implementation_new_block
		partial class shader_postprocess_implementation_new_block
		{
			public shader_postprocess_implementation_new_block() : base(5)
			{
				Add(/*bitmap transforms = */ new TI.Struct<tag_block_index_struct>(this));
				Add(/*render states = */ new TI.Struct<tag_block_index_struct>(this));
				Add(/*texture states = */ new TI.Struct<tag_block_index_struct>(this));
				Add(/*pixel constants = */ new TI.Struct<tag_block_index_struct>(this));
				Add(/*vertex constants = */ new TI.Struct<tag_block_index_struct>(this));
			}
		};
		#endregion

		#region shader_postprocess_overlay_new_block
		partial class shader_postprocess_overlay_new_block
		{
			public shader_postprocess_overlay_new_block() : base(4)
			{
				Add(InputName = new TI.StringId());
				Add(RangeName = new TI.StringId());
				Add(TimePeriod = new TI.Real());
				Add(Function = new TI.Struct<scalar_function_struct>(this));
			}
		};
		#endregion

		#region shader_postprocess_overlay_reference_new_block
		partial class shader_postprocess_overlay_reference_new_block
		{
			public shader_postprocess_overlay_reference_new_block() : base(2)
			{
				Add(OverlayIndex = new TI.ShortInteger());
				Add(TransformIndex = new TI.ShortInteger());
			}
		};
		#endregion

		#region shader_postprocess_animated_parameter_new_block
		partial class shader_postprocess_animated_parameter_new_block
		{
			public shader_postprocess_animated_parameter_new_block() : base(1)
			{
				Add(/*overlay references = */ new TI.Struct<tag_block_index_struct>(this));
			}
		};
		#endregion

		#region shader_postprocess_animated_parameter_reference_new_block
		partial class shader_postprocess_animated_parameter_reference_new_block
		{
			public shader_postprocess_animated_parameter_reference_new_block() : base(2)
			{
				Add(new TI.Skip(3));
				Add(/*parameter index = */ new TI.ByteInteger());
			}
		};
		#endregion

		#region shader_postprocess_bitmap_property_block
		partial class shader_postprocess_bitmap_property_block
		{
			public shader_postprocess_bitmap_property_block() : base(2)
			{
				Add(/*bitmap index = */ new TI.ShortInteger());
				Add(/*animated parameter index = */ new TI.ShortInteger());
			}
		}
		#endregion

		#region shader_postprocess_color_property_block
		partial class shader_postprocess_color_property_block
		{
			public shader_postprocess_color_property_block() : base(1)
			{
				Add(/*color = */ new TI.RealColor());
			}
		};
		#endregion

		#region shader_postprocess_value_property_block
		partial class shader_postprocess_value_property_block
		{
			public shader_postprocess_value_property_block() : base(1)
			{
				Add(/*value = */ new TI.Real());
			}
		};
		#endregion

		#region shader_postprocess_level_of_detail_block
		partial class shader_postprocess_level_of_detail_block
		{
			#region shader_postprocess_layer_block
			partial class shader_postprocess_layer_block
			{
				public shader_postprocess_layer_block() : base(1)
				{
					Add(/*passes = */ new TI.Struct<tag_block_index_struct>(this));
				}
			};
			#endregion

			#region shader_postprocess_pass_block
			partial class shader_postprocess_pass_block
			{
				public shader_postprocess_pass_block() : base(2)
				{
					Add(/*shader pass = */ new TI.TagReference(this, TagGroups.spas));
					Add(/*implementations = */ new TI.Struct<tag_block_index_struct>(this));
				}
			};
			#endregion

			#region shader_postprocess_implementation_block
			partial class shader_postprocess_implementation_block
			{
				public shader_postprocess_implementation_block() : base(10)
				{
					Add(/*GPU Constant State = */ new TI.Struct<shader_gpu_state_reference_struct>(this));
					Add(/*GPU Volatile State = */ new TI.Struct<shader_gpu_state_reference_struct>(this));
					Add(/*bitmap parameters = */ new TI.Struct<tag_block_index_struct>(this));
					Add(/*bitmap transforms = */ new TI.Struct<tag_block_index_struct>(this));
					Add(/*value parameters = */ new TI.Struct<tag_block_index_struct>(this));
					Add(/*color parameters = */ new TI.Struct<tag_block_index_struct>(this));
					Add(/*bitmap transform overlays = */ new TI.Struct<tag_block_index_struct>(this));
					Add(/*value overlays = */ new TI.Struct<tag_block_index_struct>(this));
					Add(/*color overlays = */ new TI.Struct<tag_block_index_struct>(this));
					Add(/*vertex shader constants = */ new TI.Struct<tag_block_index_struct>(this));
				}
			};
			#endregion

			#region shader_postprocess_bitmap_block
			partial class shader_postprocess_bitmap_block
			{
				public shader_postprocess_bitmap_block() : base(4)
				{
					Add(/*parameter index = */ new TI.ByteInteger());
					Add(/*flags = */ new TI.ByteInteger());
					Add(/*bitmap group index = */ new TI.LongInteger());
					Add(/*log bitmap dimension = */ new TI.Real());
				}
			};
			#endregion

			#region shader_postprocess_bitmap_transform_block
			partial class shader_postprocess_bitmap_transform_block
			{
				public shader_postprocess_bitmap_transform_block() : base(3)
				{
					Add(/*parameter index = */ new TI.ByteInteger());
					Add(/*bitmap transform index = */ new TI.ByteInteger());
					Add(/*value = */ new TI.Real());
				}
			};
			#endregion

			#region shader_postprocess_value_block
			partial class shader_postprocess_value_block
			{
				public shader_postprocess_value_block() : base(2)
				{
					Add(/*parameter index = */ new TI.ByteInteger());
					Add(/*value = */ new TI.Real());
				}
			};
			#endregion

			#region shader_postprocess_color_block
			partial class shader_postprocess_color_block
			{
				public shader_postprocess_color_block() : base(2)
				{
					Add(/*parameter index = */ new TI.ByteInteger());
					Add(/*color = */ new TI.RealColor());
				}
			};
			#endregion

			#region shader_postprocess_bitmap_transform_overlay_block
			partial class shader_postprocess_bitmap_transform_overlay_block
			{
				public shader_postprocess_bitmap_transform_overlay_block() : base(7)
				{
					Add(/*parameter index = */ new TI.ByteInteger());
					Add(/*transform index = */ new TI.ByteInteger());
					Add(/*animation property type = */ new TI.ByteInteger());
					Add(/*input name = */ new TI.StringId());
					Add(/*range name = */ new TI.StringId());
					Add(/*time period in seconds = */ new TI.Real());
					Add(/*function = */ new TI.Struct<scalar_function_struct>(this));
				}
			};
			#endregion

			#region shader_postprocess_value_overlay_block
			partial class shader_postprocess_value_overlay_block
			{
				public shader_postprocess_value_overlay_block() : base(5)
				{
					Add(/*parameter index = */ new TI.ByteInteger());
					Add(/*input name = */ new TI.StringId());
					Add(/*range name = */ new TI.StringId());
					Add(/*time period in seconds = */ new TI.Real());
					Add(/*function = */ new TI.Struct<scalar_function_struct>(this));
				}
			};
			#endregion

			#region shader_postprocess_color_overlay_block
			partial class shader_postprocess_color_overlay_block
			{
				public shader_postprocess_color_overlay_block() : base(5)
				{
					Add(/*parameter index = */ new TI.ByteInteger());
					Add(/*input name = */ new TI.StringId());
					Add(/*range name = */ new TI.StringId());
					Add(/*time period in seconds = */ new TI.Real());
					Add(/*function = */ new TI.Struct<color_function_struct>(this));
				}
			};
			#endregion

			#region shader_postprocess_vertex_shader_constant_block
			partial class shader_postprocess_vertex_shader_constant_block
			{
				public shader_postprocess_vertex_shader_constant_block() : base(6)
				{
					Add(/*register index = */ new TI.ByteInteger());
					Add(/*register bank = */ new TI.ByteInteger());
					Add(/*data = */ new TI.Real());
					Add(/*data = */ new TI.Real());
					Add(/*data = */ new TI.Real());
					Add(/*data = */ new TI.Real());
				}
			};
			#endregion

			public shader_postprocess_level_of_detail_block() : base(14)
			{
				Add(/*projected height percentage = */ new TI.Real());
				Add(/*available layers = */ new TI.LongInteger());
				Add(/*layers = */ new TI.Block<shader_postprocess_layer_block>(this, 25));
				Add(/*passes = */ new TI.Block<shader_postprocess_pass_block>(this, 1024));
				Add(/*implementations = */ new TI.Block<shader_postprocess_implementation_block>(this, 1024));
				Add(/*bitmaps = */ new TI.Block<shader_postprocess_bitmap_block>(this, 1024));
				Add(/*bitmap transforms = */ new TI.Block<shader_postprocess_bitmap_transform_block>(this, 1024));
				Add(/*values = */ new TI.Block<shader_postprocess_value_block>(this, 1024));
				Add(/*colors = */ new TI.Block<shader_postprocess_color_block>(this, 1024));
				Add(/*bitmap transform overlays = */ new TI.Block<shader_postprocess_bitmap_transform_overlay_block>(this, 1024));
				Add(/*value overlays = */ new TI.Block<shader_postprocess_value_overlay_block>(this, 1024));
				Add(/*color overlays = */ new TI.Block<shader_postprocess_color_overlay_block>(this, 1024));
				Add(/*vertex shader constants = */ new TI.Block<shader_postprocess_vertex_shader_constant_block>(this, 1024));
				Add(/*GPU State = */ new TI.Struct<shader_gpu_state_struct>(this));
			}
		};
		#endregion

		public shader_postprocess_definition_new_block() : base(16)
		{
			Add(/*shader template index = */ new TI.LongInteger());
			Add(/*bitmaps = */ new TI.Block<shader_postprocess_bitmap_new_block>(this, 1024));
			Add(/*pixel constants = */ new TI.Block<pixel32_block>(this, 1024));
			Add(/*vertex constants = */ new TI.Block<real_vector4d_block>(this, 1024));
			Add(/*levels of detail = */ new TI.Block<shader_postprocess_level_of_detail_new_block>(this, 1024));
			Add(/*layers = */ new TI.Block<tag_block_index_block>(this, 1024));
			Add(/*passes = */ new TI.Block<tag_block_index_block>(this, 1024));
			Add(/*implementations = */ new TI.Block<shader_postprocess_implementation_new_block>(this, 1024));
			Add(/*overlays = */ new TI.Block<shader_postprocess_overlay_new_block>(this, 1024));
			Add(/*overlay references = */ new TI.Block<shader_postprocess_overlay_reference_new_block>(this, 1024));
			Add(/*animated parameters = */ new TI.Block<shader_postprocess_animated_parameter_new_block>(this, 1024));
			Add(/*animated parameter references = */ new TI.Block<shader_postprocess_animated_parameter_reference_new_block>(this, 1024));
			Add(/*bitmap properties = */ new TI.Block<shader_postprocess_bitmap_property_block>(this, 5));
			Add(/*color properties = */ new TI.Block<shader_postprocess_color_property_block>(this, 2));
			Add(/*value properties = */ new TI.Block<shader_postprocess_value_property_block>(this, 6));
			Add(/*old levels of detail = */ new TI.Block<shader_postprocess_level_of_detail_block>(this, 1024));
		}
	};
	#endregion


	#region shader
	partial class shader_group
	{
		#region shader_properties_block
		partial class shader_properties_block
		{
			[TI.VersionCtorHalo2(1, 108, BlamVersion.Halo2_Alpha)]
			public shader_properties_block(int major, int minor) : this()
			{
				if (major == 1)
				{
					switch (minor)
					{
						case 108:
							this.RemoveAt(this.Count - 1); // LightmapFoliageScale
							break;
					}
				}
			}

			public shader_properties_block() : base(12)
			{
				Add(DiffuseMap = new TI.TagReference(this, TagGroups.bitm));
				Add(LightmapEmissiveMap = new TI.TagReference(this, TagGroups.bitm));
				Add(LightmapEmissiveColor = new TI.RealColor());
				Add(LightmapEmissivePower = new TI.Real());
				Add(LightmapResolutionScale = new TI.Real());
				Add(LightmapHalfLife = new TI.Real());
				Add(LightmapDiffuseScale = new TI.Real());
				Add(AlphaTestMap = new TI.TagReference(this, TagGroups.bitm));
				Add(TranslucentMap = new TI.TagReference(this, TagGroups.bitm));
				Add(LightmapTransparentColor = new TI.RealColor());
				Add(LightmapTransparentAlpha = new TI.Real());

				// Not in Alpha
				Add(LightmapFoliageScale = new TI.Real());
			}
		};
		#endregion

		#region long_block
		partial class long_block
		{
			public long_block() : base(1)
			{
				Add(BitmapGroupIndex = new TI.LongInteger());
			}
		};
		#endregion

		[TI.VersionCtorHalo2(0, 120, BlamVersion.Halo2_Alpha)] // 'major' hack
		[TI.VersionCtorHalo2(1, 120, BlamVersion.Halo2_Epsilon)]
		[TI.VersionCtorHalo2(1, 120, BlamVersion.Halo2_Xbox)]
		public shader_group(int major, int minor) : this()
		{
			//if(major == 1)
			{
				switch(minor)
				{
					case 120:
						// HACK: Since Alpha uses UselessPadding, and there is a TagBlock 
						// useless padding field in the definition, and PostprocessProperties 
						// didn't exist in Alpha, we have to use a sneaky hack
						if(major == 0)
							this.RemoveAt(this.Count - 1); // PostprocessProperties

						this.RemoveAt(this.Count - 1); // AddedDepthBiasSlopeScale
						this.RemoveAt(this.Count - 1); // AddedDepthBiasOffset
						break;
				}
			}
		}

		public shader_group() : base(20)
		{
			/*0x00*/Add(Template = new TI.TagReference(this, TagGroups.stem));
			/*0x10*/Add(MaterialName = new TI.StringId());
			/*0x14*/Add(RuntimeProperties = new TI.Block<shader_properties_block>(this, 1));
			// 0 = postprocessed
			// 1 = no postprocess definition
			// 2 = has transparent passes
			/*0x20*/Add(RuntimeFlags = new TI.Flags(TI.FieldType.WordFlags)); // was Pad
			/*0x22*/Add(Flags = new TI.Flags(TI.FieldType.WordFlags));
			/*0x24*/Add(Parameters = new TI.Block<global_shader_parameter_block>(this, 64));

			// In Alpha, this is a shader_postprocess_level_of_detail_block, and only 212 bytes
			// I *THINK* 'shader_postprocess_vertex_shader_constant_block' may have been added after Alpha 
			// or maybe a block in shader_gpu_state_struct was (like 'Cn Constants')
			/*0x30*/Add(PostprocessDefinition = new TI.Block<shader_postprocess_definition_new_block>(this, 1));

			/*0x3C*/Add(new TI.Pad(4)); // some sort of index set in post-process?
			Add(new TI.UselessPad(12));
			/*0x40*/Add(PredictedResources = new TI.Block<predicted_resource_block>(this, 2048));
			/*0x4C*/Add(LightResponse = new TI.TagReference(this, TagGroups.slit));
			/*0x5C*/Add(ShaderLodBias = new TI.Enum());
			/*0x5E*/Add(SpecularType = new TI.Enum());
			/*0x60*/Add(LightmapType = new TI.Enum());
			/*0x62*/Add(new TI.Pad(2));
			/*0x64*/Add(LightmapSpecularBrightness = new TI.Real());
			/*0x68*/Add(LightmapAmbientBias = new TI.Real());

			// Not in Alpha
			/*0x6C*/Add(PostprocessProperties = new TI.Block<long_block>(this, 5));
			
			// PC only?
			/*0x78*/Add(AddedDepthBiasOffset = new TI.Real());
			/*0x7C*/Add(AddedDepthBiasSlopeScale = new TI.Real());
		}
	};
	#endregion


	#region shader_template_category_block
	partial class shader_template_category_block
	{
		#region shader_template_parameter_block
		partial class shader_template_parameter_block
		{
			public shader_template_parameter_block() : base(12)
			{
				/*0x00*/Add(Name = new TI.StringId());
				/*0x04*/Add(/*Explanation = */ new TI.Data(this));
				/*0x18*/Add(Type = new TI.Enum());
				/*0x1A*/Add(/*Flags = */ new TI.Flags(TI.FieldType.WordFlags));
				/*0x1C*/Add(DefaultBitmap = new TI.TagReference(this, TagGroups.bitm));
				/*0x2C*/Add(DefaultConstValue = new TI.Real());
				/*0x30*/Add(DefaultConstColor = new TI.RealColor());
				/*0x3C*/Add(/*Bitmap Type = */ new TI.Enum());
				/*0x3E*/Add(new TI.Pad(2));
				/*0x40*/Add(/*Bitmap Animation Flags = */ new TI.Flags(TI.FieldType.WordFlags));
				/*0x42*/Add(new TI.Pad(2));
				/*0x44*/Add(/*Bitmap Scale = */ new TI.Real());
			}
		};
		#endregion

		public shader_template_category_block() : base(2)
		{
			Add(Name = new TI.StringId());
			Add(Parameters = new TI.Block<shader_template_parameter_block>(this, 64));
		}
	};
	#endregion

	#region shader_template_level_of_detail_block
	partial class shader_template_level_of_detail_block
	{
		#region shader_template_pass_reference_block
		partial class shader_template_pass_reference_block
		{
			public shader_template_pass_reference_block() : base(4)
			{
				Add(/*Layer = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*Pass = */ new TI.TagReference(this, TagGroups.spas));
				Add(new TI.Pad(12));
			}
		};
		#endregion

		public shader_template_level_of_detail_block() : base(2)
		{
			Add(/*Projected Diameter = */ new TI.Real());
			Add(/*Pass = */ new TI.Block<shader_template_pass_reference_block>(this, 16));
		}
	};
	#endregion


	#region shader_light_response
	partial class shader_light_response_group
	{
		public shader_light_response_group() : base(3)
		{
			Add(/*categories = */ new TI.Block<shader_template_category_block>(this, 16));
			Add(/*shader LODs = */ new TI.Block<shader_template_level_of_detail_block>(this, 8));
			Add(new TI.Pad(2 + 2));
		}
	};
	#endregion


	#region shader_pass_implementation_block
	partial class shader_pass_implementation_block
	{
		#region shader_texture_state_address_state_block
		partial class shader_texture_state_address_state_block
		{
			public shader_texture_state_address_state_block() : base(4)
			{
				Add(/*U address mode = */ new TI.Enum());
				Add(/*V address mode = */ new TI.Enum());
				Add(/*W address mode = */ new TI.Enum());
				Add(new TI.Pad(2));
			}
		};
		#endregion

		#region shader_texture_state_filter_state_block
		partial class shader_texture_state_filter_state_block
		{
			public shader_texture_state_filter_state_block() : base(7)
			{
				Add(/*mag filter = */ new TI.Enum());
				Add(/*min filter = */ new TI.Enum());
				Add(/*mip filter = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*mipmap bias = */ new TI.Real());
				Add(/*max mipmap index = */ new TI.ShortInteger());
				Add(/*anisotropy = */ new TI.Enum());
			}
		};
		#endregion

		#region shader_texture_state_kill_state_block
		partial class shader_texture_state_kill_state_block
		{
			public shader_texture_state_kill_state_block() : base(5)
			{
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2));
				Add(/*colorkey mode = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*colorkey color = */ new TI.Color(TI.FieldType.RgbColor));
			}
		};
		#endregion

		#region shader_texture_state_misc_state_block
		partial class shader_texture_state_misc_state_block
		{
			public shader_texture_state_misc_state_block() : base(3)
			{
				Add(/*component sign flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2));
				Add(/*border color = */ new TI.Color());
			}
		};
		#endregion

		#region shader_texture_state_constant_block
		partial class shader_texture_state_constant_block
		{
			public shader_texture_state_constant_block() : base(3)
			{
				Add(/*source parameter = */ new TI.StringId());
				Add(new TI.Pad(2));
				Add(/*constant = */ new TI.Enum());
			}
		};
		#endregion

		#region shader_pass_texture_block
		partial class shader_pass_texture_block
		{
			public shader_pass_texture_block() : base(14)
			{
				Add(/*Source Parameter = */ new TI.StringId());
				Add(/*Source Extern = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(new TI.Skip(2));
				Add(/*Mode = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*Dot Mapping = */ new TI.Enum());
				Add(/*Input Stage = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(/*address state = */ new TI.Block<shader_texture_state_address_state_block>(this, 1));
				Add(/*filter state = */ new TI.Block<shader_texture_state_filter_state_block>(this, 1));
				Add(/*kill state = */ new TI.Block<shader_texture_state_kill_state_block>(this, 1));
				Add(/*misc state = */ new TI.Block<shader_texture_state_misc_state_block>(this, 1));
				Add(/*constants = */ new TI.Block<shader_texture_state_constant_block>(this, 10));
			}
		};
		#endregion

		#region shader_pass_vertex_shader_constant_block
		partial class shader_pass_vertex_shader_constant_block
		{
			public shader_pass_vertex_shader_constant_block() : base(5)
			{
				Add(/*Source Parameter = */ new TI.StringId());
				Add(/*Scale by Texture Stage = */ new TI.Enum());
				Add(/*Register Bank = */ new TI.Enum());
				Add(/*Register Index = */ new TI.ShortInteger());
				Add(/*Component Mask = */ new TI.Enum());
			}
		};
		#endregion

		#region shader_state_channels_state_block
		partial class shader_state_channels_state_block
		{
			public shader_state_channels_state_block() : base(2)
			{
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2));
			}
		};
		#endregion

		#region shader_state_alpha_blend_state_block
		partial class shader_state_alpha_blend_state_block
		{
			public shader_state_alpha_blend_state_block() : base(7)
			{
				Add(/*blend function = */ new TI.Enum());
				Add(/*blend src factor = */ new TI.Enum());
				Add(/*blend dst factor = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*blend color = */ new TI.Color());
				Add(/*logic-op flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2));
			}
		};
		#endregion

		#region shader_state_alpha_test_state_block
		partial class shader_state_alpha_test_state_block
		{
			public shader_state_alpha_test_state_block() : base(4)
			{
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*alpha compare function = */ new TI.Enum());
				Add(/*alpha-test ref = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
			}
		};
		#endregion

		#region shader_state_depth_state_block
		partial class shader_state_depth_state_block
		{
			public shader_state_depth_state_block() : base(6)
			{
				Add(/*mode = */ new TI.Enum());
				Add(/*depth compare function = */ new TI.Enum());
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2));
				Add(/*depth bias slope scale = */ new TI.Real());
				Add(/*depth bias = */ new TI.Real());
			}
		};
		#endregion

		#region shader_state_cull_state_block
		partial class shader_state_cull_state_block
		{
			public shader_state_cull_state_block() : base(2)
			{
				Add(/*mode = */ new TI.Enum());
				Add(/*front face = */ new TI.Enum());
			}
		};
		#endregion

		#region shader_state_fill_state_block
		partial class shader_state_fill_state_block
		{
			public shader_state_fill_state_block() : base(5)
			{
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*fill mode = */ new TI.Enum());
				Add(/*back fill mode = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*line width = */ new TI.Real());
			}
		};
		#endregion

		#region shader_state_misc_state_block
		partial class shader_state_misc_state_block
		{
			public shader_state_misc_state_block() : base(3)
			{
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2));
				Add(/*fog color = */ new TI.Color(TI.FieldType.RgbColor));
			}
		};
		#endregion

		#region shader_state_constant_block
		partial class shader_state_constant_block
		{
			public shader_state_constant_block() : base(3)
			{
				Add(/*source parameter = */ new TI.StringId());
				Add(new TI.Pad(2));
				Add(/*constant = */ new TI.Enum());
			}
		};
		#endregion

		public shader_pass_implementation_block() : base(22)
		{
			Add(/*Flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(/*Textures = */ new TI.Block<shader_pass_texture_block>(this, 8));
			Add(/*Vertex Shader = */ new TI.TagReference(this, TagGroups.vrtx));
			Add(/*vs Constants = */ new TI.Block<shader_pass_vertex_shader_constant_block>(this, 32));
			Add(new TI.UselessPad(4));
			Add(/*Pixel Shader Code [NO LONGER USED] = */ new TI.Data(this));
			Add(new TI.UselessPad(12));
			Add(/*channels = */ new TI.Enum());
			Add(/*alpha-blend = */ new TI.Enum());
			Add(/*depth = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(/*channel state = */ new TI.Block<shader_state_channels_state_block>(this, 1));
			Add(/*alpha-blend state = */ new TI.Block<shader_state_alpha_blend_state_block>(this, 1));
			Add(/*alpha-test state = */ new TI.Block<shader_state_alpha_test_state_block>(this, 1));
			Add(/*depth state = */ new TI.Block<shader_state_depth_state_block>(this, 1));
			Add(/*cull state = */ new TI.Block<shader_state_cull_state_block>(this, 1));
			Add(/*fill state = */ new TI.Block<shader_state_fill_state_block>(this, 1));
			Add(/*misc state = */ new TI.Block<shader_state_misc_state_block>(this, 1));
			Add(/*constants = */ new TI.Block<shader_state_constant_block>(this, 7));
			Add(/*Pixel Shader = */ new TI.TagReference(this, TagGroups.pixl));
			Add(new TI.UselessPad(224));
		}
	};
	#endregion

	#region shader_pass_postprocess_definition_new_block
	partial class shader_pass_postprocess_definition_new_block
	{
		#region shader_pass_postprocess_implementation_new_block
		partial class shader_pass_postprocess_implementation_new_block
		{
			#region shader_postprocess_pixel_shader_constant_defaults
			partial class shader_postprocess_pixel_shader_constant_defaults
			{
				public shader_postprocess_pixel_shader_constant_defaults() : base(1)
				{
					Add(/*defaults = */ new TI.LongInteger());
				}
			};
			#endregion

			#region shader_postprocess_pixel_shader
			partial class shader_postprocess_pixel_shader
			{
				public shader_postprocess_pixel_shader() : base(7)
				{
					Add(/*pixel shader handle (runtime) = */ new TI.LongInteger());
					Add(/*pixel shader handle (runtime) = */ new TI.LongInteger());
					Add(/*pixel shader handle (runtime) = */ new TI.LongInteger());
					Add(/*constant register defaults = */ new TI.Block<shader_postprocess_pixel_shader_constant_defaults>(this, 32));
					Add(/*compiled shader = */ new TI.Data(this));
					Add(/*compiled shader = */ new TI.Data(this));
					Add(/*compiled shader = */ new TI.Data(this));
				}
			};
			#endregion

			#region pixel_shader_extern_map_block
			partial class pixel_shader_extern_map_block
			{
				public pixel_shader_extern_map_block() : base(2)
				{
					Add(/*switch parameter = */ new TI.ByteInteger());
					Add(/*case scalar = */ new TI.ByteInteger());
				}
			};
			#endregion

			#region pixel_shader_index_block
			partial class pixel_shader_index_block
			{
				public pixel_shader_index_block() : base(1)
				{
					Add(/*pixel shader index = */ new TI.ByteInteger());
				}
			};
			#endregion

			public shader_pass_postprocess_implementation_new_block() : base(23)
			{
				Add(/*textures = */ new TI.Struct<tag_block_index_struct>(this));
				Add(/*render states = */ new TI.Struct<tag_block_index_struct>(this));
				Add(/*texture states = */ new TI.Struct<tag_block_index_struct>(this));
				Add(new TI.Skip(240));
				Add(/*ps fragments = */ new TI.Struct<tag_block_index_struct>(this));
				Add(/*ps permutations = */ new TI.Struct<tag_block_index_struct>(this));
				Add(/*ps combiners = */ new TI.Struct<tag_block_index_struct>(this));
				Add(/*vertex shader = */ new TI.TagReference(this, TagGroups.vrtx));
				Add(new TI.Skip(8 + 8 + 4 + 4));
				Add(/*default render states = */ new TI.Struct<tag_block_index_struct>(this));
				Add(/*render state externs = */ new TI.Struct<tag_block_index_struct>(this));
				Add(/*texture state externs = */ new TI.Struct<tag_block_index_struct>(this));
				Add(/*pixel constant externs = */ new TI.Struct<tag_block_index_struct>(this));
				Add(/*vertex constant externs = */ new TI.Struct<tag_block_index_struct>(this));
				Add(/*ps constants = */ new TI.Struct<tag_block_index_struct>(this));
				Add(/*vs constants = */ new TI.Struct<tag_block_index_struct>(this));
				Add(/*pixel constant info = */ new TI.Struct<tag_block_index_struct>(this));
				Add(/*vertex constant info = */ new TI.Struct<tag_block_index_struct>(this));
				Add(/*render state info = */ new TI.Struct<tag_block_index_struct>(this));
				Add(/*texture state info = */ new TI.Struct<tag_block_index_struct>(this));
				Add(/*pixel shader = */ new TI.Block<shader_postprocess_pixel_shader>(this, 100));
				Add(/*pixel shader switch extern map = */ new TI.Block<pixel_shader_extern_map_block>(this, 6));
				Add(/*pixel shader index block = */ new TI.Block<pixel_shader_index_block>(this, 100));
			}
		};
		#endregion

		#region shader_pass_postprocess_texture_new_block
		partial class shader_pass_postprocess_texture_new_block
		{
			public shader_pass_postprocess_texture_new_block() : base(4)
			{
				Add(/*bitmap parameter index = */ new TI.ByteInteger());
				Add(/*bitmap extern index = */ new TI.ByteInteger());
				Add(/*texture stage index = */ new TI.ByteInteger());
				Add(/*flags = */ new TI.ByteInteger());
			}
		};
		#endregion

		#region shader_pass_postprocess_texture_state_block
		partial class shader_pass_postprocess_texture_state_block
		{
			public shader_pass_postprocess_texture_state_block() : base(1)
			{
				Add(new TI.Skip(24));
			}
		};
		#endregion

		#region pixel_shader_fragment_block
		partial class pixel_shader_fragment_block
		{
			public pixel_shader_fragment_block() : base(2)
			{
				Add(/*switch parameter index = */ new TI.ByteInteger());
				Add(/*permutations index = */ new TI.Struct<tag_block_index_struct>(this));
			}
		};
		#endregion

		#region pixel_shader_permutation_new_block
		partial class pixel_shader_permutation_new_block
		{
			public pixel_shader_permutation_new_block() : base(3)
			{
				Add(/*enum index = */ new TI.ShortInteger());
				Add(/*flags = */ new TI.ShortInteger());
				Add(/*combiners = */ new TI.Struct<tag_block_index_struct>(this));
			}
		};
		#endregion

		#region pixel_shader_combiner_block
		partial class pixel_shader_combiner_block
		{
			public pixel_shader_combiner_block() : base(11)
			{
				Add(new TI.Pad(16));
				Add(/*constant color0 = */ new TI.Color());
				Add(/*constant color1 = */ new TI.Color());
				Add(/*color A register ptr index = */ new TI.ByteInteger());
				Add(/*color B register ptr index = */ new TI.ByteInteger());
				Add(/*color C register ptr index = */ new TI.ByteInteger());
				Add(/*color D register ptr index = */ new TI.ByteInteger());
				Add(/*alpha A register ptr index = */ new TI.ByteInteger());
				Add(/*alpha B register ptr index = */ new TI.ByteInteger());
				Add(/*alpha C register ptr index = */ new TI.ByteInteger());
				Add(/*alpha D register ptr index = */ new TI.ByteInteger());
			}
		};
		#endregion

		#region shader_pass_postprocess_extern_new_block
		partial class shader_pass_postprocess_extern_new_block
		{
			public shader_pass_postprocess_extern_new_block() : base(2)
			{
				Add(new TI.Skip(3));
				Add(/*extern index = */ new TI.ByteInteger());
			}
		};
		#endregion

		#region shader_pass_postprocess_constant_new_block
		partial class shader_pass_postprocess_constant_new_block
		{
			public shader_pass_postprocess_constant_new_block() : base(4)
			{
				Add(/*parameter name = */ new TI.StringId());
				Add(/*component mask = */ new TI.ByteInteger());
				Add(/*scale by texture stage = */ new TI.ByteInteger());
				Add(/*function index = */ new TI.ByteInteger());
			}
		};
		#endregion

		#region shader_pass_postprocess_constant_info_new_block
		partial class shader_pass_postprocess_constant_info_new_block
		{
			public shader_pass_postprocess_constant_info_new_block() : base(2)
			{
				Add(/*parameter name = */ new TI.StringId());
				Add(new TI.Pad(3));
			}
		};
		#endregion

		#region texture_stage_state_block
		partial class texture_stage_state_block
		{
			public texture_stage_state_block() : base(3)
			{
				Add(/*state index = */ new TI.ByteInteger());
				Add(/*stage index = */ new TI.ByteInteger());
				Add(/*state value = */ new TI.LongInteger());
			}
		};
		#endregion

		#region extern_reference_block
		partial class extern_reference_block
		{
			public extern_reference_block() : base(2)
			{
				Add(/*parameter index = */ new TI.ByteInteger());
				Add(/*extern index = */ new TI.ByteInteger());
			}
		};
		#endregion

		#region pixel_shader_permutation_block
		partial class pixel_shader_permutation_block
		{
			public pixel_shader_permutation_block() : base(5)
			{
				Add(/*enum index = */ new TI.ShortInteger());
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*constants = */ new TI.Struct<tag_block_index_struct>(this));
				Add(/*combiners = */ new TI.Struct<tag_block_index_struct>(this));
				Add(new TI.Skip(4 + 4));
			}
		};
		#endregion

		#region pixel_shader_constant_block
		partial class pixel_shader_constant_block
		{
			public pixel_shader_constant_block() : base(5)
			{
				Add(/*parameter type = */ new TI.Enum(TI.FieldType.ByteEnum));
				Add(/*combiner index = */ new TI.ByteInteger());
				Add(/*register index = */ new TI.ByteInteger());
				Add(/*component mask = */ new TI.Enum(TI.FieldType.ByteEnum));
				Add(new TI.Pad(1 + 1));
			}
		};
		#endregion

		#region shader_pass_postprocess_implementation_block
		partial class shader_pass_postprocess_implementation_block
		{
			public shader_pass_postprocess_implementation_block() : base(17)
			{
				Add(/*GPU State = */ new TI.Struct<shader_gpu_state_struct>(this));
				Add(/*GPU Constant State = */ new TI.Struct<shader_gpu_state_reference_struct>(this));
				Add(/*GPU Volatile State = */ new TI.Struct<shader_gpu_state_reference_struct>(this));
				Add(/*GPU default state = */ new TI.Struct<shader_gpu_state_reference_struct>(this));
				Add(/*vertex shader = */ new TI.TagReference(this, TagGroups.vrtx));
				Add(new TI.Skip(8 + 8 + 4 + 4));
				Add(/*value externs = */ new TI.Block<extern_reference_block>(this, 1024));
				Add(/*color externs = */ new TI.Block<extern_reference_block>(this, 1024));
				Add(/*switch externs = */ new TI.Block<extern_reference_block>(this, 1024));
				Add(/*bitmap parameter count = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(new TI.Skip(240));
				Add(/*pixel shader fragments = */ new TI.Block<pixel_shader_fragment_block>(this, 1024));
				Add(/*pixel shader permutations = */ new TI.Block<pixel_shader_permutation_block>(this, 1024));
				Add(/*pixel shader combiners = */ new TI.Block<pixel_shader_combiner_block>(this, 1024));
				Add(/*pixel shader constants = */ new TI.Block<pixel_shader_constant_block>(this, 1024));
				Add(new TI.Skip(4 + 4));
			}
		};
		#endregion

		public shader_pass_postprocess_definition_new_block() : base(11)
		{
			Add(/*implementations = */ new TI.Block<shader_pass_postprocess_implementation_new_block>(this, 1024));
			Add(/*textures = */ new TI.Block<shader_pass_postprocess_texture_new_block>(this, 1024));
			Add(/*render states = */ new TI.Block<render_state_block>(this, 1024));
			Add(/*texture states = */ new TI.Block<shader_pass_postprocess_texture_state_block>(this, 1024));
			Add(/*ps fragments = */ new TI.Block<pixel_shader_fragment_block>(this, 1024));
			Add(/*ps permutations = */ new TI.Block<pixel_shader_permutation_new_block>(this, 1024));
			Add(/*ps combiners = */ new TI.Block<pixel_shader_combiner_block>(this, 1024));
			Add(/*externs = */ new TI.Block<shader_pass_postprocess_extern_new_block>(this, 1024));
			Add(/*constants = */ new TI.Block<shader_pass_postprocess_constant_new_block>(this, 1024));
			Add(/*constant info = */ new TI.Block<shader_pass_postprocess_constant_info_new_block>(this, 1024));
			Add(/*old implementations = */ new TI.Block<shader_pass_postprocess_implementation_block>(this, 1024));
		}
	};
	#endregion

	#region shader_pass
	partial class shader_pass_group
	{
		#region shader_pass_parameter_block
		partial class shader_pass_parameter_block
		{
			public shader_pass_parameter_block() : base(9)
			{
				Add(/*Name = */ new TI.StringId());
				Add(/*Explanation = */ new TI.Data(this));
				Add(/*Type = */ new TI.Enum());
				Add(/*Flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*Default Bitmap = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*Default Const Value = */ new TI.Real());
				Add(/*Default Const Color = */ new TI.RealColor());
				Add(/*Source Extern = */ new TI.Enum());
				Add(new TI.Pad(2));
			}
		};
		#endregion

		public shader_pass_group() : base(5)
		{
			Add(/*Documentation = */ new TI.Data(this));
			Add(/*Parameters = */ new TI.Block<shader_pass_parameter_block>(this, 64));
			Add(new TI.Pad(2 + 2));
			Add(/*Implementations = */ new TI.Block<shader_pass_implementation_block>(this, 32));

			// In Alpha, this is a shader_pass_postprocess_implementation_block and is only 500 bytes 
			// plus appears to have some fields different around the fields that are after the 'vertex shader' field
			Add(/*Postprocess Definition = */ new TI.Block<shader_pass_postprocess_definition_new_block>(this, 1));
		}
	};
	#endregion


	#region shader_template
	partial class shader_template_group
	{
		#region shader_template_property_block
		partial class shader_template_property_block
		{
			public shader_template_property_block() : base(3)
			{
				Add(/*Property = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*Parameter Name = */ new TI.StringId());
			}
		};
		#endregion

		#region shader_template_runtime_external_light_response_index_block
		partial class shader_template_runtime_external_light_response_index_block
		{
			public shader_template_runtime_external_light_response_index_block() : base(1)
			{
				Add(/* = */ new TI.LongInteger());
			}
		};
		#endregion

		#region shader_template_postprocess_definition_new_block
		partial class shader_template_postprocess_definition_new_block
		{
			#region shader_template_postprocess_level_of_detail_new_block
			partial class shader_template_postprocess_level_of_detail_new_block
			{
				public shader_template_postprocess_level_of_detail_new_block() : base(3)
				{
					Add(/*layers = */ new TI.Struct<tag_block_index_struct>(this));
					Add(/*available layers = */ new TI.LongInteger());
					Add(/*projected height percentage = */ new TI.Real());
				}
			};
			#endregion

			#region shader_template_postprocess_pass_new_block
			partial class shader_template_postprocess_pass_new_block
			{
				public shader_template_postprocess_pass_new_block() : base(2)
				{
					Add(/*pass = */ new TI.TagReference(this, TagGroups.spas));
					Add(/*implementations = */ new TI.Struct<tag_block_index_struct>(this));
				}
			};
			#endregion

			#region shader_template_postprocess_implementation_new_block
			partial class shader_template_postprocess_implementation_new_block
			{
				public shader_template_postprocess_implementation_new_block() : base(3)
				{
					Add(/*bitmaps = */ new TI.Struct<tag_block_index_struct>(this));
					Add(/*pixel constants = */ new TI.Struct<tag_block_index_struct>(this));
					Add(/*vertex constants = */ new TI.Struct<tag_block_index_struct>(this));
				}
			};
			#endregion

			#region shader_template_postprocess_remapping_new_block
			partial class shader_template_postprocess_remapping_new_block
			{
				public shader_template_postprocess_remapping_new_block() : base(2)
				{
					Add(new TI.Skip(3));
					Add(/*source index = */ new TI.ByteInteger());
				}
			};
			#endregion

			public shader_template_postprocess_definition_new_block() : base(5)
			{
				Add(/*levels of detail = */ new TI.Block<shader_template_postprocess_level_of_detail_new_block>(this, 1024));
				Add(/*layers = */ new TI.Block<tag_block_index_block>(this, 1024));
				Add(/*passes = */ new TI.Block<shader_template_postprocess_pass_new_block>(this, 1024));
				Add(/*implementations = */ new TI.Block<shader_template_postprocess_implementation_new_block>(this, 1024));
				Add(/*remappings = */ new TI.Block<shader_template_postprocess_remapping_new_block>(this, 1024));
			}
		};
		#endregion

		[TI.VersionCtorHalo2(1, 144, BlamVersion.Halo2_Alpha)]
		public shader_template_group(int major, int minor) : this()
		{
			if(major == 1)
			{
				switch(minor)
				{
					case 144:
						this.RemoveAt(this.Count - 1); // PostprocessDefinition
						break;
				}
			}
		}

		public shader_template_group() : base(17)
		{
			/*0x00*/Add(/*Documentation = */ new TI.Data(this));
			/*0x14*/Add(/*Default Material Name = */ new TI.StringId());
			/*0x18*/Add(new TI.Pad(2)); // internal flags
			/*0x1A*/Add(/*Flags = */ new TI.Flags(TI.FieldType.WordFlags));
			/*0x1C*/Add(/*Properties = */ new TI.Block<shader_template_property_block>(this, 14));
			/*0x28*/Add(Categories = new TI.Block<shader_template_category_block>(this, 16));
			/*0x34*/Add(/*Light Response = */ new TI.TagReference(this, TagGroups.slit));
			/*0x44*/Add(/*LODs = */ new TI.Block<shader_template_level_of_detail_block>(this, 8));
			/*0x50*/Add(/* = */ new TI.Block<shader_template_runtime_external_light_response_index_block>(this, 65535));
			/*0x5C*/Add(/* = */ new TI.Block<shader_template_runtime_external_light_response_index_block>(this, 65535));
			/*0x68*/Add(/*Aux 1 Shader = */ new TI.TagReference(this, TagGroups.shad));
			/*0x78*/Add(/*Aux 1 Layer = */ new TI.Enum());
			/*0x7A*/Add(new TI.Pad(2));
			/*0x7C*/Add(/*Aux 2 Shader = */ new TI.TagReference(this, TagGroups.shad));
			/*0x8C*/Add(/*Aux 2 Layer = */ new TI.Enum());
			/*0x8E*/Add(new TI.Pad(2));

			// Only field in this tag that is:
			// *Not in Alpha*
			/*0x90*/Add(PostprocessDefinition = new TI.Block<shader_template_postprocess_definition_new_block>(this, 1));
		}
	};
	#endregion
};