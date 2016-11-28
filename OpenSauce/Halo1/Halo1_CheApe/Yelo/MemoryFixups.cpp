/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Yelo/MemoryFixups.hpp"

#include <blamlib/Halo1/tag_files/files.hpp>
#include <blamlib/Halo1/tool/tool.hpp>

#include <YeloLib/Halo1/open_sauce/settings/c_settings_cheape.hpp>

#include "Settings/Settings.hpp"
#include "Engine/EngineFunctions.hpp"
#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
#include "Yelo/MemoryFixups.Paths.inl"
#include "Yelo/MemoryFixups.TagTextIO.inl"
	void c_memory_fixups::InitializePaths()
	{
		FixupsInitializeFilePaths();
	}
	void c_memory_fixups::DisposePaths()
	{
	}

	void c_memory_fixups::ProcessFixup(s_fixup* fixup)
	{
		if(fixup->type <= _fixup_type_none || fixup->type >= _fixup_type)
			return;

		void* address = fixup->address[PLATFORM_TYPE - PLATFORM_EDITOR_START_ID];
		void* definition = fixup->definition[PLATFORM_TYPE - PLATFORM_EDITOR_START_ID];

#if 0
 		YELO_DEBUG_FORMAT("CheApe: fixup @%p @%p type=%d", address, definition, fixup->type);
#endif

		if(address == nullptr) return; // if address is NULL, definition should be as well...

		switch(fixup->type)
		{
		case _fixup_type_replace_memory:
			Memory::WriteMemory(address, definition, CAST(int32, fixup->type_data));
			break;

		case _fixup_type_replace_pointer:
			Memory::WriteMemory(address, definition);
			break;

		case _fixup_type_replace_tag_field:
			if( IN_RANGE_ENUM(fixup->type_data, Enums::_field_terminator) )
			{
				Memory::WriteMemory(address, definition, sizeof(tag_field));
			}
			break;
		}
	}

	void c_memory_fixups::ProcessFixups()
	{
		CheApe::s_cache_header& header = CheApe::GlobalCacheHeader();

		if(header.Fixups.Count > 0)
		{
			s_fixup** fixups = CAST_PTR(s_fixup**, header.Fixups.Address);
			for(int32 x = 0; x < header.Fixups.Count; x++)
				ProcessFixup(fixups[x]);
		}
	}

	void c_memory_fixups::Initialize()
	{
		c_memory_fixups::ProcessFixups();
		c_memory_fixups::FixupsInitializeTagTextIOFixes();

#if PLATFORM_TYPE == PLATFORM_TOOL
		uint32* k_tool_import_find_files_references_size = CAST_PTR(uint32*, 0x415099);
		*k_tool_import_find_files_references_size = Enums::k_maximum_tool_import_files_upgrade * sizeof(s_file_reference);

		uint32* k_tool_import_find_files_references_count = CAST_PTR(uint32*, 0x4150E1);
		*k_tool_import_find_files_references_count = Enums::k_maximum_tool_import_files_upgrade;
#endif

#if 0
		c_memory_fixups::DebugPaths();
#endif
	}

	void c_memory_fixups::Dispose()
	{
	}

	void c_memory_fixups::DebugPaths()
	{
		static const size_t k_buffer_size = MAX_PATH*11;

		auto buffer = std::unique_ptr<char>(new char[k_buffer_size]);

		auto& profile = Settings::c_settings_cheape::Profile();
		sprintf_s(buffer.get(), k_buffer_size, "\r\n"
			"root: %s\r\n"
			"\tdata: %s\r\n"
			"\tmaps: %s\r\n"
			"\ttags: %s\r\n"
			"\ttags name: %s",
			profile.GetRootPath(),
			profile.GetDataPath(),
			profile.GetMapsPath(),
			profile.GetTagsPath(),
			profile.GetTagsName());
		Debug::Write(buffer.get());

		const s_override_paths& o = _override_paths;
		sprintf_s(buffer.get(), k_buffer_size, "\r\n"
			"root: %s\r\n"

			"data: %s\r\n"

			"maps: %s\r\n"
				"\tslash: %s\r\n"
				"\ttemp: %s\r\n"

			"tags: %s\r\n"
				"\tslash: %s\r\n"

				"\tname slash single: %s\r\n"
				"\tname slash single rel: %s\r\n"

				"\tname slash: %s\r\n"
				"\tname slash rel: %s",
			o.root,

			o.data.path,
			
			o.maps.path,
				o.maps.path,
				o.maps.path,
			
			o.tags.path,
				o.tags.path_with_slash,
				
				o.tags.folder_name_with_slash_single,
				o.tags.folder_name_with_slash_single_relative,
				
				o.tags.folder_name_with_slash,
				o.tags.folder_name_with_slash_relative);
		Debug::Write(buffer.get());
	}
};