/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/Halo1/memory/data.hpp>

struct IDirect3DVertexDeclaration9;

namespace Yelo
{
	namespace TagGroups
	{
		struct bitmap_data;
	};

	namespace Enums
	{
		enum {
			k_rasterizer_maximum_texture_stages = 4,
		};
	};

	namespace blam
	{
		bool PLATFORM_API rasterizer_set_texture_bitmap_data(_enum stage, TagGroups::s_bitmap_data* bitmap);
	};


	//////////////////////////////////////////////////////////////////////////
	// rasterizer_dx9_shaders_vdecl9

	namespace Enums
	{
		enum {
			_vsdecl_environment, // _rasterizer_vertex_type_environment_uncompressed
			_vsdecl_environment_c, // _rasterizer_vertex_type_environment_compressed
			_vsdecl_environment_lightmap, // _rasterizer_vertex_type_environment_lightmap_uncompressed
			_vsdecl_environment_lightmap_c, // _rasterizer_vertex_type_environment_lightmap_compressed
			_vsdecl_model,
			_vsdecl_model_c,
			_vsdecl_unlit, // _rasterizer_vertex_type_dynamic_unlit (yes, this is #6)
			_vsdecl_dynamic_unlit,
			_vsdecl_screen, // _rasterizer_vertex_type_dynamic_screen
			_vsdecl_debug, // _rasterizer_vertex_type_debug
			_vsdecl_decal, // _rasterizer_vertex_type_decal
			_vsdecl_detail_object, // _rasterizer_vertex_type_detail_object
			_vsdecl_environment_ff, // _rasterizer_vertex_type_environment_uncompressed_ff
			_vsdecl_environment_lightmap_ff, // _rasterizer_vertex_type_environment_lightmap_uncompressed_ff
			_vsdecl_model_uncompressed_ff,
			_vsdecl_model_processed,
			_vsdecl_unlit_zsprite,
			_vsdecl_screen_transformed_lit,
			_vsdecl_screen_transformed_lit_specular,
			_vsdecl_environment_single_stream_ff,
			//NUM_VERTEX_SHADER_DECLARATIONS
		};
	};

	namespace DX9
	{
		struct s_vertex_shader_declaration
		{
			IDirect3DVertexDeclaration9* decl;
			long_enum format; // D3DFVF
			long_enum processing_method; // D3DUSAGE
		}; BOOST_STATIC_ASSERT( sizeof(s_vertex_shader_declaration) == 0xC );
	};
};