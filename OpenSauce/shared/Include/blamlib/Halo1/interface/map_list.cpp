/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/interface/map_list.hpp>

#include <blamlib/Halo1/cache/cache_files.hpp>
#include <blamlib/Halo1/cache/cache_files_structures.hpp>
#include <YeloLib/Halo1/cache/cache_files_yelo.hpp>
#include <YeloLib/files/files.hpp>
#include <YeloLib/Halo1/open_sauce/settings/yelo_shared_settings.hpp>

namespace Yelo
{
	namespace Interface
	{
#if !PLATFORM_IS_EDITOR
		void s_map_list_map::Initialize(int32 system_map_index)
		{
			memset(this, 0, sizeof(*this)); // engine doesn't do this, it does explicit field init, but we have custom fields
			this->system_map_index = system_map_index;
			this->valid = true; // engine defaults valid to true...
			this->crc = NONE;
		}
		bool s_map_list_map::InitializeNameFromPath(cstring map_path)
		{
			char local_map_name[_MAX_FNAME];
			auto map_file_type = Cache::GetMapNameFromPath(local_map_name, map_path);

			// we were somehow given an invalid map_path...this should never happen through normal map_list APIs
			if (map_file_type == Cache::e_map_path_file_type::invalid)
				return false;

			// allocate and set this entry's name
			size_t map_name_length = strlen(local_map_name);
			char* map_name = CAST_PTR(char*, BLAM_MALLOC(map_name_length+1));
			this->name = map_name;

			strcpy(map_name, local_map_name);

			// calculate the flags related to the input map_path
			this->yelo_flags.is_yelo_file = map_file_type == Cache::e_map_path_file_type::yelo;

			return true;
		}
		bool s_map_list_map::ReadHeader(cstring map_path)
		{
			Cache::s_cache_header cache_header;
			this->valid = Cache::ReadHeader(map_path, cache_header) == Cache::e_cache_read_header_result::success;

			this->yelo_flags.has_yelo_header = cache_header.yelo.HasHeader();
			this->yelo_flags.is_mp_map = cache_header.cache_type == Enums::_cache_file_type_multiplayer;

			return this->valid;
		}
		void s_map_list_map::Dispose()
		{
			BLAM_FREE(this->name);
			this->name = nullptr;
		}

		bool MultiplayerMapIsSystemMap(cstring map_name)
		{
			for (auto mp_map : blam::k_map_list_mp_maps)
				if (!strcmp(map_name, mp_map.name))
					return true;

			return false;
		}

		int32 MapListAddMapFromPath(cstring maps_path, cstring map_file_name,
			int32 system_map_index)
		{
			YELO_ASSERT(maps_path);
			YELO_ASSERT(map_file_name);

			auto& multiplayer_maps = *MultiplayerMaps();

			if (multiplayer_maps.count >= multiplayer_maps.capacity)
			{
				// engine resizes by 0x19, which just so happens to be the same size as this list, so...
				multiplayer_maps.capacity += NUMBEROF(blam::k_map_list_mp_maps);

				size_t new_size = sizeof(s_map_list_map) * multiplayer_maps.capacity;
				multiplayer_maps.elements = CAST_PTR(s_map_list_map*, BLAM_REALLOC(multiplayer_maps.elements, new_size));
			}
			int mp_map_index = multiplayer_maps.count++;

			s_map_list_map* entry = multiplayer_maps[mp_map_index];
			entry->Initialize(system_map_index);
			bool name_initialized = entry->InitializeNameFromPath(map_file_name);
			YELO_ASSERT(name_initialized);

			// default full_map_path to map_file_name. If maps_path is empty, then map_file_name should actually be a map_path,
			// in which case we don't need to waste time sprintf'ing an empty string with another string
			cstring full_map_path = map_file_name;
			char full_map_path_buffer[MAX_PATH] = "";
			if (!is_null_or_empty(maps_path))
			{
				sprintf_s(full_map_path_buffer, "%s%s", maps_path, map_file_name);
				full_map_path = full_map_path_buffer;
			}

			if (entry->ReadHeader(full_map_path) && entry->yelo_flags.is_mp_map)
			{
				// this is where the engine would have calculated the map's checksum.
				// we would only want to do checksums for MP maps only too. unless co-op somehow became a thing...
			}
			// system maps must ALWAYS be added to the map list, so only remove invalid custom maps
			else if (system_map_index == Enums::_system_multiplayer_map_user_created)
			{
				// not a playable MP map. undefine the entry from the mp list
				entry->Dispose();
				multiplayer_maps.count--;
				mp_map_index = NONE;
			}

			return mp_map_index;
		}

		int32 MapListMapGetIndexFromName(cstring map_name)
		{
			auto& multiplayer_maps = *MultiplayerMaps();

			for (int32 x = 0; x < multiplayer_maps.count; x++)
			{
				const auto* entry = multiplayer_maps[x];

				// NOTE: engine uses strstr().
				// We don't use _stricmp as it performs a lowercase comparison however, map_name's should always lowercase
				if (!strcmp(map_name, entry->name))
					return x;
			}

			return NONE;
		}

		static void MapListAddSystemMultiplayerMaps(cstring root_path = "")
		{
			// the full pathname to the "maps\" directory, relative to root_path
			auto maps_path = std::string();
			// the full pathname to the system map file
			auto map_path = std::string();

			maps_path.reserve(MAX_PATH);
			maps_path.append(root_path);
			maps_path.append(Cache::K_MAP_FILES_DIRECTORY);

			map_path.reserve(MAX_PATH);
			map_path.append(maps_path);
			// cache the position of the end of the maps_path portion of the string
			// and use it as the map_name insertion index
			auto map_file_name_index = map_path.length();

			for (const auto& info : blam::k_map_list_mp_maps)
			{
				map_path.erase(map_file_name_index);
				map_path.append(info.name);
				map_path.append(Cache::K_MAP_FILE_EXTENSION);

				// engine uses info.name and assumes CWD, but we use the full path in case we use a different root_path
				cstring map_file_name = &map_path[map_file_name_index];
				MapListAddMapFromPath(maps_path.c_str(), map_file_name, info.system_map_index);
			}
		}
		// Add all the map files that appear in the "maps\" directory under a specified root path
		static void MapListAddMapsInPath(WIN32_FIND_DATA& fd,
			cstring root_path, cstring map_extension)
		{
			YELO_ASSERT(root_path);

			// the full pathname to the "maps\" directory, relative to root_path
			auto maps_path = std::string();
			// the find files search pattern string
			auto find_filename = std::string();

			maps_path.reserve(MAX_PATH);
			// we assume root_path is empty or ends with a path separator 
			maps_path.append(root_path);
			YELO_ASSERT(maps_path.length() == 0 || maps_path.back() == '\\');
			maps_path.append(Cache::K_MAP_FILES_DIRECTORY);

			find_filename.reserve(MAX_PATH);
			find_filename.append(maps_path);
			find_filename.append("*"); // search pattern. we want all files that use the desired extension
			find_filename.append(map_extension);

			// TODO: FindExInfoBasic was only introduced in WINS2008 and W7. Need to test the executing platform and change which API we call when it is XP (ugh) or Vista (meh)
			// Ex() is only in WinXP+, but using FindExInfoBasic we can skip querying the short names of files
			// The engine uses FindFirstFile
			HANDLE h = FindFirstFileEx(find_filename.c_str(), FindExInfoBasic, &fd, FindExSearchNameMatch, nullptr, 0);
			if (h != INVALID_HANDLE_VALUE)
			{
				do {
					// FindFile results will all be relative, so just 'filename.extension', no root path
					cstring map_file_name = fd.cFileName;

					if (!blam::map_list_should_ignore(map_file_name))
						MapListAddMapFromPath(maps_path.c_str(), map_file_name);
				} while (FindNextFile(h, &fd));

				FindClose(h);
			}
		}
		// Add all the map file types (.map, .yelo) that appear in the "maps\" directory under a specified root path
		static void MapListAddMapTypessInPath(WIN32_FIND_DATA& fd, cstring root_path)
		{
			// .map
			MapListAddMapsInPath(fd, root_path, Cache::K_MAP_FILE_EXTENSION);
			// .yelo
			MapListAddMapsInPath(fd, root_path, Cache::K_MAP_FILE_EXTENSION_YELO);
		}
#endif
	};

	namespace blam
	{
#if !PLATFORM_IS_EDITOR
		// The display name strings in this tag should match the elements in k_map_list_mp_maps
		static cstring k_map_list_mp_names_unicode_string_list_name = R"(ui\shell\main_menu\mp_map_list)";

		cstring const k_map_list_ignored_map_names[10+1+3] = {
			"a10", "a30", "a50",
			"b30", "b40",
			"c10", "c20", "c40",
			"d20", "d40",
			"ui",
			"bitmaps",
			"sounds",
			"loc",
		};

		const Interface::s_map_list_map_info k_map_list_mp_maps[Enums::k_number_of_system_multiplayer_maps] = {
			{0,  "beavercreek", true},
			{1,  "sidewinder", true},
			{2,  "damnation", true},
			{3,  "ratrace", true},
			{4,  "prisoner", true},
			{5,  "hangemhigh", true},
			{6,  "chillout", true},
			{7,  "carousel", true},
			{8,  "boardingaction", true},
			{9,  "bloodgulch", true},
			{10, "wizard", true},
			{11, "putput", true},
			{12, "longest", true},

			{13, "icefields"},
			{14, "deathisland"},
			{15, "dangercanyon"},
			{16, "infinity"},
			{17, "timberland"},
			{18, "gephyrophobia"},
		};

		int32 map_list_map_get_index(cstring map_path)
		{
			char map_name[_MAX_FNAME] = "";
			auto map_file_type = Cache::GetMapNameFromPath(map_name, map_path);

			return Interface::MapListMapGetIndexFromName(map_name);
		}

		bool map_list_should_ignore(cstring map_path)
		{
			YELO_ASSERT(map_path);

			char map_name[_MAX_FNAME] = "";
			auto map_file_type = Cache::GetMapNameFromPath(map_name, map_path);

			if (map_file_type == Cache::e_map_path_file_type::invalid)
				return true;

			for (cstring name : k_map_list_ignored_map_names)
			{
				// NOTE: engine uses _stricmp()
				// We don't use _stricmp as it performs a lowercase comparison however, map_name's should always lowercase
				if (!_stricmp(map_name, name))
					return true;
			}

			int index = Interface::MapListMapGetIndexFromName(map_name);
			return index != NONE;
		}

		void map_list_initialize()
		{
			auto& multiplayer_maps = *Interface::MultiplayerMaps();

			//YELO_ASSERT(multiplayer_maps != nullptr);
			YELO_ASSERT(multiplayer_maps.count == 0);
			YELO_ASSERT(multiplayer_maps.capacity == 0);

			Interface::MapListAddSystemMultiplayerMaps();

			// first add maps from the current working directory, then try adding from the user profile
			WIN32_FIND_DATA fd;
			Interface::MapListAddMapTypessInPath(fd, "");

			// only try adding maps from the user profile if we detected the presence of an actual maps directory
			if (Settings::UserProfileMapsPathExists())
			{
				cstring user_profile_path = Settings::UserProfilePath();

				Interface::MapListAddMapTypessInPath(fd, user_profile_path);
			}
		}

		void map_list_dispose()
		{
			auto& multiplayer_maps = *Interface::MultiplayerMaps();

			//YELO_ASSERT(multiplayer_maps != nullptr);

			for (int x = 0; x < multiplayer_maps.count; x++)
				multiplayer_maps[x]->Dispose();

			BLAM_FREE(multiplayer_maps.elements);
			memset(&multiplayer_maps, 0, sizeof(multiplayer_maps));
		}
#endif
	};
};