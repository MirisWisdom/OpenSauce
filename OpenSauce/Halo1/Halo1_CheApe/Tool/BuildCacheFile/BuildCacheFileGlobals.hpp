/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once
#if PLATFORM_TYPE == PLATFORM_TOOL
#include <blamlib/Halo1/cache/data_file.hpp>
#include <blamlib/Halo1/tool/tool.hpp>

namespace Yelo
{
	namespace Cache
	{
		struct s_cache_header;
		struct s_build_cache_file_globals;

		class c_data_files;
	};
	namespace Tool
	{

struct s_build_cache_file_for_scenario {
	bool build_failed;
	PAD24;
	cstring scenario_path;

	typedef bool (PLATFORM_API* _data_file_open) (Enums::data_file_reference_type type, cstring name, bool writable);
	typedef bool (PLATFORM_API* _data_file_close)(Enums::data_file_reference_type type);

	cstring*			data_file_open_path_format;	// address which references "maps\\%s.map"
	_data_file_open		data_file_open;
	_data_file_close	data_file_close;

	Cache::s_build_cache_file_globals* globals;

	typedef bool (PLATFORM_API* _build_cache_file_cull_tags)();
	typedef bool (PLATFORM_API* _build_structure_bsp_predicted_resources)();
	typedef bool (PLATFORM_API* _build_cache_file_add_tags)(Cache::s_cache_header& header, void* scratch_buffer, datum_index tag_indexes[], int32 prior_tag_memory_space_used);
	typedef void (PLATFORM_API* __build_cache_file_for_scenario)(cstring scenario_name);
	typedef bool (PLATFORM_API* _build_cache_file_begin)(cstring scenario_name);
	typedef bool (PLATFORM_API* _build_cache_file_end)(Cache::s_cache_header* header);
	typedef void (PLATFORM_API* _build_cache_file_failed)();
	typedef bool (PLATFORM_API* _build_cache_file_add_resource)(const void* buffer, size_t buffer_size, uint32* out_file_offset, bool update_crc);

	_build_cache_file_cull_tags		build_cache_file_cull_tags;
	_build_structure_bsp_predicted_resources build_structure_bsp_predicted_resources;
	_build_cache_file_add_tags		build_cache_file_add_tags;
	__build_cache_file_for_scenario	_build_cache_file_for_scenario;
	proc_import_class				build_cache_file_for_scenario_command;
	_build_cache_file_begin			build_cache_file_begin;
	_build_cache_file_end			build_cache_file_end;
	_build_cache_file_failed		build_cache_file_failed;
	_build_cache_file_add_resource	build_cache_file_add_resource;

	void*		build_cache_file_add_structure_bsps;	// address of the build_cache_file_add_structure_bsps (requires wrapper due to non-stack parameters)
	void*		build_cache_file_write;					// address of the build_cache_file_write function (requires wrapper due to non-stack parameters)
	void*		build_cache_file_end_sprintf_call;		// address of the call to sprintf for building the output map file path
	cstring*	build_cache_file_output_path_format;	// address which references "%s%s%s.map"
	char*		build_cache_file_output_directory;
	static const size_t k_build_cache_file_output_directory_size = 256;


	static bool AddStructureBsps(void* scratch_buffer, datum_index tag_indexes[], int32& out_largest_bsp_size);
	static void Write(void* buffer, size_t buffer_size, int32* out_file_offset);

	// Initialize the cache and data_file file systems
	static void InitializeFileSystem(Cache::c_data_files& data_files, 
		cstring mod_name, bool using_mod_sets);

	void DataFilesOpen(const Cache::c_data_files& data_files, bool store_resources);
	void DataFilesClose(bool store_resources);
	void BuildPostprocess(cstring mod_name, bool using_mod_sets);

	// Forces tool to use our own sprintf-like function which uses the OS cache file naming convention
	void InitializeBuildCacheFileEndSprintfOverride();
	static void __cdecl BuildCacheFileEndSprintfOverride(char* buffer, cstring format, 
		cstring output_directory, cstring map_dir, cstring scenario_name);

	// Force Tool to use the Yelo data_file path scheme
	// If the user has defined their maps directory in the settings, this will use it.
	void UseYeloDataFilePathScheme();	// "maps\data_files\"
	// Force Tool to use the Yelo map file naming scheme
	void UseYeloMapNamingScheme();		// "<map-name>.yelo"

};
extern s_build_cache_file_for_scenario build_cache_file_for_scenario_internals;

	};
};

#endif