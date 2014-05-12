/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "TagGroups/CacheFiles.hpp"

#include <blamlib/Halo1/cache/cache_files.hpp>
#include <blamlib/Halo1/cache/cache_files_globals.hpp>
#include <blamlib/Halo1/interface/map_list.hpp>
#include <blamlib/Halo1/scenario/scenario_definitions.hpp>

#include <YeloLib/configuration/c_configuration_container.hpp>
#include <YeloLib/configuration/c_configuration_value.hpp>
#include "Settings/c_settings_singleton.hpp"

#include <YeloLib/Halo1/cache/cache_files_yelo.hpp>
#include <YeloLib/Halo1/cache/data_file_yelo.hpp>
#include <YeloLib/Halo1/open_sauce/blam_memory_upgrades.hpp>

#include "Memory/MemoryInterface.hpp"
#include "Common/FileIO.hpp"
#include "Settings/YeloSettings.hpp"
#include "Game/EngineFunctions.hpp"
#include "Game/GameState.hpp"

#if !PLATFORM_IS_DEDI
#include "Networking/HTTP/MapDownloadClient.hpp"
#endif

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_TAGGROUPS
#define __EL_INCLUDE_FILE_ID	__EL_TAGGROUPS_CACHE_FILES
#include "Memory/_EngineLayout.inl"

	namespace Interface
	{
#include "TagGroups/CacheFiles.MapList.inl"
	};
};

#include "TagGroups/CacheFiles.DataFiles.inl"

namespace Yelo
{
	namespace DataFiles
	{
		static void ScenarioTagsLoadPreprocess(Cache::s_cache_header* cache_header)
		{
			// when a yelo made cache is loaded, this will be set to true, causing non-yelo-made 
			// caches to load the original data files. This isn't required as yelo bases 
			// its data files off the original ones, but I thought it would be good practice %-)
			static bool revert_file_names = false;
			static char exception_details[128];

			if(cache_header->yelo.flags.uses_mod_data_files && !is_null_or_empty(cache_header->yelo.mod_name))
			{
				if (!mod_data_files_globals.TryAndUseModSet(cache_header->yelo.mod_name))
				{
					sprintf_s(exception_details, "%s's data files (bitmaps, sounds, and/or loc)", 
						cache_header->yelo.mod_name);
					Engine::GatherException(exception_details, 0x89, 0x7E, 1);
				}

				revert_file_names = true;
			}
			else if(revert_file_names)
			{
				revert_file_names = false;
				mod_data_files_globals.UseStockDataFiles();
			}
		}

		// Called by the game (via our hook) whenever it loads a scenario's 
		// cache and verifies its header. Ran before the tag index and such 
		// is loaded into memory.
		API_FUNC_NAKED static void PLATFORM_API ScenarioTagsLoadHook()
		{
			using namespace Yelo::Cache;

			__asm {
				push	edi

				call	Cache::CacheFileGlobals
				push	eax // save the globals pointer for the end

				lea		ecx, [eax]s_cache_file_globals.cache_header
				push	ecx
				call	ScenarioTagsLoadPreprocess

				pop		eax
				pop		edi
				mov		ecx, [eax]s_cache_file_globals.cache_header.tag_memory_size
				mov		edx, [eax]s_cache_file_globals.cache_header.offset_to_index
				retn
			}
		}

		void Initialize()
		{
			Memory::WriteRelativeJmp(CacheFilesInitialize_DataFilesOpenHook,
				CAST_PTR(void*, GET_FUNC_PTR(CACHE_FILES_INITIALIZE)+0xB), true);
			Memory::WriteRelativeJmp(CacheFilesDispose_DataFilesCloseHook,
				CAST_PTR(void*, GET_FUNC_PTR(CACHE_FILES_DISPOSE)+0x48), true);
			Memory::WriteRelativeJmp(DataFileReadHook,
				GET_FUNC_VPTR(DATA_FILE_READ), true);

			Memory::CreateHookRelativeCall(&ScenarioTagsLoadHook,
				GET_FUNC_VPTR(SCENARIO_TAGS_LOAD_HOOK),
				Enums::_x86_opcode_nop);
		}

		void Dispose()
		{
		}
	};


//////////////////////////////////////////////////////////////////////////


	namespace Cache
	{
		struct s_yelo_settings {
#if PLATFORM_IS_USER
			// Scenario tag name of the mainmenu the user wants to use
			string256 mainmenu_scenario_name;

			bool InitializeMainmenuOverride(const std::string& override_name)
			{
				static cstring k_stock_ui = "levels\\ui\\ui";

				if(!override_name.empty())
				{
					// If the override name fits and if it's not the same as the stock ui
					if((override_name.length() <= Enums::k_string_256_length) && (override_name.compare(k_stock_ui) != 0))
					{
						return strcpy_s(mainmenu_scenario_name, override_name.c_str()) == k_errnone;
					}
				}

				mainmenu_scenario_name[0] = '\0';

				return false;
			}

			// Returns true if the override is valid and can be used
			bool UseMainmenuOverride() { return mainmenu_scenario_name[0] != '\0'; }
#endif

			void InitializeMemoryOverrides()
			{
#if PLATFORM_IS_USER
				if(UseMainmenuOverride())
				{
					for(auto ptr : K_UI_SCENARIO_NAME_REFERENCES)
						*ptr = mainmenu_scenario_name;
				}
#endif
			}
		}g_yelo_settings;
//////////////////////////////////////////////////////////////////////////
// Cache Size upgrades

		static void MemoryUpgradesInitialize()
		{
			for(auto ptr : K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_LIST)
				*ptr = Enums::k_max_cache_size_upgrade;
		}

		static void MemoryUpgradesDispose()
		{
		}

//////////////////////////////////////////////////////////////////////////

		s_cache_file_globals* CacheFileGlobals()		PTR_IMP_GET2(cache_file_globals);
		char* RootDirectory()							PTR_IMP_GET2(maps_folder_parent_dir);
		
#include "TagGroups/CacheFiles.Settings.inl"
#include "TagGroups/CacheFiles.YeloFiles.inl"
#include "TagGroups/CacheFiles.CRC.inl"

		void Initialize()
		{
			c_settings_cache::Register();

			MemoryUpgradesInitialize();
			c_cache_format_path_hacks::Initialize();
			g_yelo_settings.InitializeMemoryOverrides();

			Interface::MapListInitializeHooks();
		}

		void Dispose()
		{
			MemoryUpgradesDispose();
			
			c_settings_cache::Unregister();
		}

		static bool MapIsOriginal(cstring map_name)
		{
			for (auto mp_map : blam::k_original_multiplayer_maps)
				if( !strcmp(map_name, mp_map.name) )
					return true;

			return false;
		}
		bool ReadHeader(cstring relative_map_name, s_cache_header& out_header, bool& yelo_is_ok, 
			bool exception_on_fail, bool for_map_list_add_map)
		{
			bool file_exists = false;
			bool result = false;
			yelo_is_ok = true;

			static string256 map_path;
			c_cache_format_path_hacks::PathHack(map_path, "%s%s%s.map", RootDirectory(), K_MAP_FILES_DIRECTORY, relative_map_name);

			HANDLE f = CreateFileA(map_path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
			if(f != INVALID_HANDLE_VALUE)
			{
				file_exists = true;

				DWORD lpNumberOfBytesRead;
				if(ReadFile(f, &out_header, sizeof(out_header), &lpNumberOfBytesRead, nullptr) != FALSE && lpNumberOfBytesRead == sizeof(out_header))
				{
					if(out_header.ValidSignatures())
					{
						if( out_header.ValidFileSize(Enums::k_max_cache_size_upgrade) && 
							out_header.ValidName() && 
							out_header.version == s_cache_header::k_version && 
							(yelo_is_ok = out_header.yelo.IsValid())) // IsValid returns true on non-OS (ie, stock) maps
							result = true;
						else
							YELO_DEBUG_FORMAT("[%s] has proper signatures but invalid header values!", map_path);
					}
				}

				CloseHandle(f);

				// Return false early on MP maps for map_list_add_map to avoid the exception logic
				if(for_map_list_add_map && result)
				{
					if(out_header.cache_type != Enums::_cache_file_type_multiplayer)
						return false;
				}

				if(file_exists && result)
				{
					int map_entry_index = Engine::Cache::GetMapEntryIndexFromName(relative_map_name);
					if(map_entry_index != NONE)
					{
						auto* map_data = Interface::MultiplayerMaps();
						auto* map_entry = (*map_data)[map_entry_index];
						if(map_entry->crc == 0xFFFFFFFF)
							map_entry->crc = Cache::CalculateCRC(map_path);
					}
				}
			}

			if(!result && exception_on_fail)
			{
				if(!file_exists && !MapIsOriginal(relative_map_name))
				{
					if(!yelo_is_ok)
					{
						PrepareToDropError("Detected an invalid (probably old) .yelo map. See next message for map that needs removing.");
					}
					Engine::GatherException(map_path, 0x89, 0x7E, 1);
				}
			}
			else if (!result && !exception_on_fail && out_header.cache_type == Enums::_cache_file_type_multiplayer)
			{
#if !PLATFORM_IS_DEDI
				// insert map download here as this is probably only reached on multiplayer
				Networking::HTTP::Client::MapDownload::AddMapForDownload(relative_map_name);
#endif
			}

			return result;
		}
	};
};