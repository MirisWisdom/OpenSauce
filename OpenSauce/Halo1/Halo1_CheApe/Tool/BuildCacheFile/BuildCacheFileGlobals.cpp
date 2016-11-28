/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/BuildCacheFile/BuildCacheFileGlobals.hpp"
#if PLATFORM_TYPE == PLATFORM_TOOL

#include <blamlib/Halo1/cache/cache_file_builder.hpp>
#include <YeloLib/Halo1/cache/data_file_yelo.hpp>
#include <YeloLib/Halo1/open_sauce/settings/c_settings_cheape.hpp>

#include "Memory/MemoryInterface.hpp"

namespace Yelo { namespace Tool {

	s_build_cache_file_for_scenario build_cache_file_for_scenario_internals = {
		false,
		nullptr,

		CAST_PTR(cstring*,															0x4B9FB6),
		CAST_PTR(s_build_cache_file_for_scenario::_data_file_open,					0x4B9F10),
		CAST_PTR(s_build_cache_file_for_scenario::_data_file_close,					0x4BA100),

		CAST_PTR(Cache::s_build_cache_file_globals*,								0x10A1030),

		CAST_PTR(s_build_cache_file_for_scenario::_build_cache_file_cull_tags,		0x453260),
		CAST_PTR(s_build_cache_file_for_scenario::_build_structure_bsp_predicted_resources,		0x453860),
		CAST_PTR(s_build_cache_file_for_scenario::_build_cache_file_add_tags,		0x454D40),
		CAST_PTR(s_build_cache_file_for_scenario::__build_cache_file_for_scenario,	0x4553A0),
		CAST_PTR(proc_import_class,													0x455640),
		CAST_PTR(s_build_cache_file_for_scenario::_build_cache_file_begin,			0x4B9250),
		CAST_PTR(s_build_cache_file_for_scenario::_build_cache_file_end,			0x4B93B0),
		CAST_PTR(s_build_cache_file_for_scenario::_build_cache_file_failed,			0x4B9030),
		CAST_PTR(s_build_cache_file_for_scenario::_build_cache_file_add_resource,	0x4B9350),

		CAST_PTR(void*,																0x454B70),
		CAST_PTR(void*,																0x4B9180),
		CAST_PTR(void*,																0x4B944D),
		CAST_PTR(cstring*,															0x4B9448),
		CAST_PTR(char*,																0x10FD510),
	};

	API_FUNC_NAKED bool s_build_cache_file_for_scenario::AddStructureBsps(void* scratch_buffer, datum_index tag_indexes[], int32& out_largest_bsp_size)
	{
		API_FUNC_NAKED_START()
			push	out_largest_bsp_size
			push	tag_indexes
			mov		eax, scratch_buffer
			call	build_cache_file_for_scenario_internals.build_cache_file_add_structure_bsps
			add		esp, 4 * 2
		API_FUNC_NAKED_END(3);
	}
	API_FUNC_NAKED void s_build_cache_file_for_scenario::Write(void* buffer, size_t buffer_size, int32* out_file_offset)
	{
		API_FUNC_NAKED_START()
			mov		edi, buffer_size
			mov		ecx, buffer
			call	build_cache_file_for_scenario_internals.build_cache_file_write
			add		esp, 4 * 1
		API_FUNC_NAKED_END(3);
	}

	void s_build_cache_file_for_scenario::InitializeFileSystem(Cache::c_data_files& data_files,
		cstring mod_name, bool using_mod_sets)
	{
		cstring settings_path = Settings::c_settings_cheape::Profile().GetMapsPath();

		char maps_path[MAX_PATH];
		strcpy_s(maps_path, settings_path);
		_mkdir(maps_path);

		data_files.InitializeForCache(using_mod_sets, mod_name, maps_path);

		if (using_mod_sets)
			build_cache_file_for_scenario_internals.UseYeloDataFilePathScheme();
	}

	void s_build_cache_file_for_scenario::DataFilesOpen(const Cache::c_data_files& data_files, bool store_resources)
	{
		using namespace Enums;

		data_file_open(_data_file_reference_bitmaps,data_files.m_names[_data_file_type_bitmaps], store_resources);
		data_file_open(_data_file_reference_sounds, data_files.m_names[_data_file_type_sounds], store_resources);
		data_file_open(_data_file_reference_locale, data_files.m_names[_data_file_type_locale], store_resources);

		if (store_resources)
			globals->data_files.PreprocessForSave();
	}

	void s_build_cache_file_for_scenario::DataFilesClose(bool store_resources)
	{
		using namespace Enums;

		if (store_resources)
			globals->data_files.Save();

		data_file_close(_data_file_reference_bitmaps);
		data_file_close(_data_file_reference_sounds);
		data_file_close(_data_file_reference_locale);
	}

	void s_build_cache_file_for_scenario::BuildPostprocess(cstring mod_name, bool using_mod_sets)
	{
		if (!using_mod_sets)
			return;
	}

	void s_build_cache_file_for_scenario::InitializeBuildCacheFileEndSprintfOverride()
	{
		Memory::WriteRelativeCall(BuildCacheFileEndSprintfOverride, build_cache_file_end_sprintf_call);
	}

	void __cdecl s_build_cache_file_for_scenario::BuildCacheFileEndSprintfOverride(
		char* buffer,				// [256]
		cstring format,				// same pointer as [build_cache_file_output_path_format]
		cstring output_directory,	// same pointer as [build_cache_file_output_directory]
		cstring map_dir, cstring scenario_name)
	{
		const size_t k_buffer_size = sizeof(*buffer) * 256;

		sprintf_s(buffer, k_buffer_size, "%s%s.yelo", Settings::c_settings_cheape::Profile().GetMapsPath(), scenario_name);
	}
	void s_build_cache_file_for_scenario::UseYeloDataFilePathScheme()
	{
		static char data_file_path_format[MAX_PATH];

		strcpy_s(data_file_path_format, Settings::c_settings_cheape::Profile().GetMapsPath());
		strcat_s(data_file_path_format, "%s.map");

		*data_file_open_path_format = data_file_path_format;
	}
	void s_build_cache_file_for_scenario::UseYeloMapNamingScheme()
	{
		*build_cache_file_output_path_format = "%s%s%s.yelo";
	}

}; };

namespace Yelo
{
	namespace Cache
	{
		s_build_cache_file_globals* BuildCacheFileGlobals()
		{
			return Tool::build_cache_file_for_scenario_internals.globals;
		}
	};
};
#endif