/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo3.Tags
{
	#region render_method_definition
	[TI.TagGroup((int)TagGroups.Enumerated.rmdf, 1, 92)]
	public class render_method_definition_group : TI.Definition
	{
		#region Fields
		public TI.TagReference Options;
		public TI.TagReference PixelShader, VertexShader;
		#endregion

		#region Ctor
		public render_method_definition_group()
		{
			Add(Options = new TI.TagReference(this, TagGroups.rmop));
			// tag block [0x18] [categories]
				// string id
				// tag block [0x1C] [options]
					// string id
					// tag reference [rmop]
					// unknown [0x8]
				// unknown [0x8]
			// tag block [0x10]
				// word
				// word (enum?)
				// tag block [0x4]
					// word
					// word
			// tag block [0x10]
				// word
				// unknown [0xE]
			Add(PixelShader = new TI.TagReference(this, TagGroups.glps));
			Add(VertexShader = new TI.TagReference(this, TagGroups.glvs));
			Add(new TI.UnknownPad(
				4 +	// only seen this be 0 or 1 as a dword
				4));// never seen this used
		}
		#endregion
	};
	#endregion

	#region render_method_option
	// _render_method_extern_none
	// _render_method_extern_texture_global_target_texaccum
	// _render_method_extern_texture_global_target_normal
	// _render_method_extern_texture_global_target_z
	// _render_method_extern_texture_global_target_shadow_buffer1
	// _render_method_extern_texture_global_target_shadow_buffer2
	// _render_method_extern_texture_global_target_shadow_buffer3
	// _render_method_extern_texture_global_target_shadow_buffer4
	// _render_method_extern_texture_global_target_texture_camera
	// _render_method_extern_texture_global_target_reflection
	// _render_method_extern_texture_global_target_refraction
	// _render_method_extern_texture_lightprobe_texture
	// _render_method_extern_texture_dominant_light_intensity_map
	// _render_method_extern_texture_unused1
	// _render_method_extern_texture_unused2
	// _render_method_extern_object_change_color_primary
	// _render_method_extern_object_change_color_secondary
	// _render_method_extern_object_change_color_tertiary
	// _render_method_extern_object_change_color_quaternary
	// _render_method_extern_object_emblem_color_background
	// _render_method_extern_object_emblem_color_primary
	// _render_method_extern_object_emblem_color_secondary
	// _render_method_extern_texture_dynamic_environment_map_0
	// _render_method_extern_texture_dynamic_environment_map_1
	// _render_method_extern_texture_cook_torrance_cc0236
	// _render_method_extern_texture_cook_torrance_dd0236
	// _render_method_extern_texture_cook_torrance_c78d78
	// _render_method_extern_light_dir_0
	// _render_method_extern_light_color_0
	// _render_method_extern_light_dir_1
	// _render_method_extern_light_color_1
	// _render_method_extern_light_dir_2
	// _render_method_extern_light_color_2
	// _render_method_extern_light_dir_3
	// _render_method_extern_light_color_3
	// _render_method_extern_texture_unused_3
	// _render_method_extern_texture_unused_4
	// _render_method_extern_texture_unused_5
	// _render_method_extern_texture_dynamic_light_gel_0
	// _render_method_extern_flat_envmap_matrix_x
	// _render_method_extern_flat_envmap_matrix_y
	// _render_method_extern_flat_envmap_matrix_z
	// _render_method_extern_debug_tint
	// _render_method_extern_screen_constants
	// _render_method_extern_active_camo_distortion_texture
	// _render_method_extern_scene_ldr_texture
	// _render_method_extern_scene_hdr_texture
	// _render_method_extern_water_memory_export_address
	// _render_method_extern_tree_animation_timer

	[TI.TagGroup((int)TagGroups.Enumerated.rmop, 1, 12)]
	public class render_method_option_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public render_method_option_group()
		{
			// tag block [0x48] parameters
				// string id [name]
				// dword [flags?]
				// long enum [extern type]
				// tag reference [bitm] [default bitmap]
				// real [default const value]
				// unknown [0x4]
				// long [default const value]
				// unknown [0x4]
				// argb color [default color value]
				// unknown [0x18]
		}
		#endregion
	};
	#endregion

	#region render_method_template
	[TI.TagGroup((int)TagGroups.Enumerated.rmt2, 1, -1)]
	public class render_method_template_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public render_method_template_group()
		{
		}
		#endregion
	};
	#endregion

	#region render_water_ripple
	[TI.TagGroup((int)TagGroups.Enumerated.rwrd, 1, -1)]
	public class render_water_ripple_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public render_water_ripple_group()
		{
		}
		#endregion
	};
	#endregion

	#region render methods
	#region render_method
	[TI.TagGroup((int)TagGroups.Enumerated.rm__, 1, -1)]
	public class render_method_group : TI.Definition
	{
		#region Fields
		public TI.TagReference MethodDefinition;
		#endregion

		#region Ctor
		protected render_method_group(int dummy) {}
		public render_method_group()
		{
			Add(MethodDefinition = new TI.TagReference(this, TagGroups.rmdf));
		}
		#endregion
	};
	#endregion

	#region shader_contrail
	[TI.TagGroup((int)TagGroups.Enumerated._rmc, 1, -1)]
	public class shader_contrail_group : render_method_group
	{
		#region Fields
		#endregion

		#region Ctor
		public shader_contrail_group(int dummy) : base(dummy) {}
		public shader_contrail_group()
		{
		}
		#endregion
	};
	#endregion

	#region shader_particle
	[TI.TagGroup((int)TagGroups.Enumerated._rmp, 1, -1)]
	public class shader_particle_group : render_method_group
	{
		#region Fields
		#endregion

		#region Ctor
		public shader_particle_group(int dummy) : base(dummy) {}
		public shader_particle_group()
		{
		}
		#endregion
	};
	#endregion

	#region shader_beam
	[TI.TagGroup((int)TagGroups.Enumerated.rmb_, 1, -1)]
	public class shader_beam_group : render_method_group
	{
		#region Fields
		#endregion

		#region Ctor
		public shader_beam_group(int dummy) : base(dummy) {}
		public shader_beam_group()
		{
		}
		#endregion
	};
	#endregion

	#region shader_custom
	[TI.TagGroup((int)TagGroups.Enumerated.rmcs, 1, -1)]
	public class shader_custom_group : render_method_group
	{
		#region Fields
		#endregion

		#region Ctor
		public shader_custom_group(int dummy) : base(dummy) {}
		public shader_custom_group()
		{
		}
		#endregion
	};
	#endregion

	#region shader_cortana
	[TI.TagGroup((int)TagGroups.Enumerated.rmct, 1, -1)]
	public class shader_cortana_group : render_method_group
	{
		#region Fields
		#endregion

		#region Ctor
		public shader_cortana_group(int dummy) : base(dummy) {}
		public shader_cortana_group()
		{
		}
		#endregion
	};
	#endregion

	#region shader_decal
	[TI.TagGroup((int)TagGroups.Enumerated.rmd_, 1, -1)]
	public class shader_decal_group : render_method_group
	{
		#region Fields
		#endregion

		#region Ctor
		public shader_decal_group(int dummy) : base(dummy) {}
		public shader_decal_group()
		{
		}
		#endregion
	};
	#endregion

	#region shader_foliage
	[TI.TagGroup((int)TagGroups.Enumerated.rmfl, 1, -1)]
	public class shader_foliage_group : render_method_group
	{
		#region Fields
		#endregion

		#region Ctor
		public shader_foliage_group(int dummy) : base(dummy) {}
		public shader_foliage_group()
		{
		}
		#endregion
	};
	#endregion

	#region shader_halogram
	[TI.TagGroup((int)TagGroups.Enumerated.rmhg, 1, -1)]
	public class shader_halogram_group : render_method_group
	{
		#region Fields
		#endregion

		#region Ctor
		public shader_halogram_group(int dummy) : base(dummy) {}
		public shader_halogram_group()
		{
		}
		#endregion
	};
	#endregion

	#region shader_light_volume
	[TI.TagGroup((int)TagGroups.Enumerated.rmlv, 1, -1)]
	public class shader_light_volume_group : render_method_group
	{
		#region Fields
		#endregion

		#region Ctor
		public shader_light_volume_group(int dummy) : base(dummy) {}
		public shader_light_volume_group()
		{
		}
		#endregion
	};
	#endregion

	#region shader
	[TI.TagGroup((int)TagGroups.Enumerated.rmsh, 1, -1)]
	public class shader_group : render_method_group
	{
		#region Fields
		#endregion

		#region Ctor
		public shader_group(int dummy) : base(dummy) {}
		public shader_group()
		{
		}
		#endregion
	};
	#endregion

	#region shader_skin
	[TI.TagGroup((int)TagGroups.Enumerated.rmsk, 1, -1)]
	public class shader_skin_group : render_method_group
	{
		#region Fields
		#endregion

		#region Ctor
		public shader_skin_group(int dummy) : base(dummy) {}
		public shader_skin_group()
		{
		}
		#endregion
	};
	#endregion

	#region shader_terrain
	[TI.TagGroup((int)TagGroups.Enumerated.rmtr, 1, -1)]
	public class shader_terrain_group : render_method_group
	{
		#region Fields
		#endregion

		#region Ctor
		public shader_terrain_group(int dummy) : base(dummy) {}
		public shader_terrain_group()
		{
		}
		#endregion
	};
	#endregion

	#region shader_water
	[TI.TagGroup((int)TagGroups.Enumerated.rmw_, 1, 64)]
	public class shader_water_group : render_method_group
	{
		#region Fields
		#endregion

		#region Ctor
		public shader_water_group(int dummy) : base(dummy) {}
		public shader_water_group()
		{
		}
		#endregion
	};
	#endregion
	#endregion

	#region global_pixel_shader
	[TI.TagGroup((int)TagGroups.Enumerated.glps, 1, -1)]
	public class global_pixel_shader_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public global_pixel_shader_group()
		{
		}
		#endregion
	};
	#endregion

	#region global_vertex_shader
	[TI.TagGroup((int)TagGroups.Enumerated.glvs, 1, -1)]
	public class global_vertex_shader_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public global_vertex_shader_group()
		{
		}
		#endregion
	};
	#endregion
}