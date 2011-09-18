/*
    Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition
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
#include "Common/Precompile.hpp"
#include "Yelo/MemoryFixups.hpp"

#include "Common/YeloSettings.hpp"
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

		void* address = fixup->address[PLATFORM_ID-1];
		void* definition = fixup->definition[PLATFORM_ID-1];

#if 0
 		YELO_ERROR(_error_message_priority_warning, 
 			"CheApe: fixup @%p @%p type=%d", address, definition, fixup->type);
#endif

		if(address == NULL) return; // if address is NULL, definition should be as well...

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

		char* buffer = new char[k_buffer_size];

		const Settings::s_profile& p = Settings::Get().active_profile;
		sprintf_s(buffer, k_buffer_size, "\r\n"
			"root: %s\r\n"
				"\tdata: %s\r\n"
				"\tmaps: %s\r\n"
				"\ttags: %s\r\n"
				"\ttags name: %s",
			p.root_path,
				p.paths.data,
				p.paths.maps,
				p.paths.tags,
				p.paths.tags_folder_name);
		EngineFunctions::error(Enums::_error_message_priority_warning, buffer);

		const s_override_paths& o = _override_paths;
		sprintf_s(buffer, k_buffer_size, "\r\n"
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
		EngineFunctions::error(Enums::_error_message_priority_warning, buffer);

		delete[] buffer;
	}
};