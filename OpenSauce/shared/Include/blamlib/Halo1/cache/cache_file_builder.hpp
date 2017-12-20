/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once
#if PLATFORM_TYPE == PLATFORM_TOOL

#include <blamlib/Halo1/cache/cache_files_structures.hpp>
#include <blamlib/cseries/cseries_base.hpp>
#include <blamlib/tag_files/tag_groups_base.hpp>
#include <blamlib/memory/datum_index.hpp>
#include <YeloLib/cseries/cseries_yelo_base.hpp>
#include <blamlib/cseries/enum_templates.h>
#include <blamlib/Halo1/cache/s_data_file_globals.h>

namespace Yelo { namespace Cache
{
	enum
	{
		k_cache_file_page_alignment_bit = Flags::k_alignment_4096bit,
		k_cache_file_page_size = FLAG(k_cache_file_page_alignment_bit),
		k_cache_file_page_size_mask = FLAG(k_cache_file_page_alignment_bit) - 1,
	};

	namespace e_build_cache_file_begin
	{
		typedef enum : byte_flags
		{
			building_yelo_bit,
			mod_sets_create_anew_bit,
			mod_sets_store_scenario_resources_bit,
			use_memory_upgrades_bit,
			dump_tag_group_allocation_stats_bit,

			k_count
		} type_t;

		BOOST_STATIC_ASSERT(k_count <= BIT_COUNT(byte));

		typedef flags_type<type_t> flags_t;
	}

	struct s_build_cache_file_globals
	{
		static cstring k_temp_cache_file_name;
		static cstring k_cache_file_extension;

		bool building;
		char scenario_name[Enums::k_max_tag_name_length + 1];
		e_build_cache_file_begin::flags_t begin_flags; // NOTE: non-standard member
		bool canceled; // NOTE: non-standard member
		PAD8;
		uint32 crc;
		HANDLE file_handle;
		int32 cache_stream_size;
		s_data_file_globals data_files;

		DWORD get_file_size() const;

		bool write_to_file(
			const void* buffer,
			const int32 buffer_size);

		bool temporary_file_open(
			cstring filename = k_temp_cache_file_name);
		void temporary_file_close(
			cstring filename = k_temp_cache_file_name);
		bool temporary_file_copy(
			cstring new_filename,
			cstring filename = k_temp_cache_file_name);

		void scenario_name_to_cache_file_path(
			std::string& cache_file_path);
	};

	s_build_cache_file_globals* build_cache_file_globals();

	// Is the cache file that is being built in the .yelo format?
	bool build_cache_file_for_yelo();

	bool scenario_load_for_cache_build(
		cstring scenario_name);
}

namespace blam
{
	int32 build_cache_file_size();

	uint32 build_cache_file_checksum();

	bool build_cache_file_begin(
		cstring scenario_name,
		const Cache::e_build_cache_file_begin::flags_t flags);

	bool build_cache_file_add_resource(
		const void* buffer,
		int32 buffer_size,
		int32* return_file_offset = nullptr,
		bool include_in_crc = true);

	void build_cache_file_cancel();

	bool build_cache_file_end(
		Cache::s_cache_header& header);

	size_t stream_tag_to_buffer(
		datum_index tag_index,
		void* stream,
		size_t& return_stream_offset,
		uintptr_t virtual_base_address,
		tag_reference_name_reference* tag_names);

	bool PLATFORM_API scenario_load_all_structure_bsps();

	void build_cache_file_for_scenario(
		cstring scenario_path,
		const Cache::e_build_cache_file_begin::flags_t begin_flags);
}}
#endif
