/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "TagGroups/CacheFiles.hpp"

#include <blamlib/Halo1/scenario/scenario_definitions.hpp>

#include <YeloLib/Halo1/cache/data_file_yelo.hpp>
#include <YeloLib/Halo1/open_sauce/blam_memory_upgrades.hpp>

#include "Memory/MemoryInterface.hpp"
#include "Common/YeloSettings.hpp"
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

	namespace DataFiles
	{
		// Will cause the the data files to unload, then reload
		// Will also cause the s_cache_file_globals to be reinitialized
		API_FUNC_NAKED static void CacheFilesReInitialize()
		{
			static const uintptr_t INITIALIZE = GET_FUNC_PTR(CACHE_FILES_INITIALIZE);
			static const uintptr_t DISPOSE = GET_FUNC_PTR(CACHE_FILES_DISPOSE);

			__asm {
				call	DISPOSE
				call	INITIALIZE
				retn
			}
		}

		// Force the data files system to use the specified file names.
		// Default arguments cause the data files system to use original game code.
		static void DataFilesUseNewFiles(cstring bitmaps = "bitmaps",
			cstring sounds = "sounds",
			cstring locale = "loc")
		{
			int32 x;

			for(x = 0; x < NUMBEROF(K_DATA_FILE_BITMAPS_NAME_REFERENCES); x++)	*K_DATA_FILE_BITMAPS_NAME_REFERENCES[x] = bitmaps;
			for(x = 0; x < NUMBEROF(K_DATA_FILE_SOUNDS_NAME_REFERENCES);  x++)	*K_DATA_FILE_SOUNDS_NAME_REFERENCES[x] = sounds;
			for(x = 0; x < NUMBEROF(K_DATA_FILE_LOCALE_NAME_REFERENCES);  x++)	*K_DATA_FILE_LOCALE_NAME_REFERENCES[x] = locale;
		}

		static void ScenarioTagsLoadPreprocess(Cache::s_cache_header* cache_header)
		{
			// when a yelo made cache is loaded, this will be set to true, causing non-yelo-made 
			// caches to load the original data files. This isn't required as yelo bases 
			// its data files off the original ones, but I thought it would be good practice %-)
			static bool revert_file_names = false;

			if(cache_header->yelo.flags.uses_mod_data_files && !is_null_or_empty(cache_header->yelo.mod_name))
			{
				const char (*bitmaps)[MAX_PATH];
				const char (*sounds) [MAX_PATH];
				const char (*locale) [MAX_PATH];
				bool set_is_registered = FindSet(cache_header->yelo.mod_name,
					bitmaps, sounds, locale);

				if(!set_is_registered)
				{
					char error_text[MAX_PATH+64];
					sprintf_s(error_text, "Mod-set '%s' for map '%s' is not registered in the user's settings, crashing...",
						cache_header->yelo.mod_name, cache_header->name);
					PrepareToDropError(error_text);

					Engine::GatherException(cache_header->yelo.mod_name, 0x89, 0x7E, 1);
				}
				
				// Validate the files in the mod-set exist on disk and exception (with details) if any don't.
				char missing_set_file[MAX_PATH];
				if(!VerifySetFilesExist(*bitmaps, *sounds, *locale, missing_set_file))
					Engine::GatherException(missing_set_file, 0x89, 0x7E, 1);

				DataFilesUseNewFiles(*bitmaps, *sounds, *locale);

				int16 original_index = Cache::CacheFileGlobals()->open_map_file_index;
				Cache::CacheFileGlobals()->open_map_file_index = NONE;
				CacheFilesReInitialize();
				Cache::CacheFileGlobals()->open_map_file_index = original_index;

				revert_file_names = true;
			}
			else if(revert_file_names)
			{
				revert_file_names = false;
				DataFilesUseNewFiles();
			}
		}

		// Called by the game (via our hook) whenever it loads a scenario's 
		// cache and verifies its header. Ran before the tag index and such 
		// is loaded into memory.
		API_FUNC_NAKED static void PLATFORM_API ScenarioTagsLoadHook()
		{
			__asm {
				push	edi

				call	Cache::CacheFileGlobals
				push	eax

				lea		ecx, [eax+4]
				push	ecx
				call	ScenarioTagsLoadPreprocess

				pop		eax
				pop		edi
				mov		ecx, [eax+(4 + 0x14)]
				retn
			}
		}

		void Initialize()
		{
			SharedInitialize();

#if PLATFORM_IS_DEDI // adds|misses|hits bullshit
			for(int32 x = 0; x < NUMBEROF(K_DATA_FILES_STUPID_STRINGS_GET_NULLED); x++)
				*K_DATA_FILES_STUPID_STRINGS_GET_NULLED[x] = '\0';
#endif

			Memory::CreateHookRelativeCall(&ScenarioTagsLoadHook,
				GET_FUNC_VPTR(SCENARIO_TAGS_LOAD_HOOK),
				0x90);
		}

		void Dispose()
		{
			SharedDispose();
		}
	};


//////////////////////////////////////////////////////////////////////////


	namespace Cache
	{
		struct s_yelo_settings {
			bool check_for_yelo_files_first; // if true, checks for .yelo files first before .map files
			PAD24;
#if PLATFORM_IS_USER
			// Scenario tag name of the mainmenu the user wants to use
			string256 mainmenu_scenario_name;

			bool InitializeMainmenuOverride(cstring override_name)
			{
				static cstring k_stock_ui = "levels\\ui\\ui";

				if(override_name != NULL && override_name[0] != '\0')
				{
					size_t name_length = strlen(override_name);

					// If the override name fits and if it's not the same as the stock ui
					if(name_length <= Enums::k_string_256_length && 
						strcmp(override_name, k_stock_ui) != 0)
						return strcpy_s(mainmenu_scenario_name, override_name) == k_errnone;
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
					for(int32 x = 0; x < NUMBEROF(K_UI_SCENARIO_NAME_REFERENCES); x++)
						*K_UI_SCENARIO_NAME_REFERENCES[x] = mainmenu_scenario_name;
				}
#endif
			}

			void LoadSettings(TiXmlElement* cf_element)
			{
				if(cf_element != NULL)
				{
					check_for_yelo_files_first = Settings::ParseBoolean(cf_element->Attribute("checkForYeloFilesFirst"));

#if PLATFORM_IS_USER
					cstring mainmenu_override_name = cf_element->Attribute("mainmenuScenario");

					InitializeMainmenuOverride(mainmenu_override_name);
#endif
				}
			}
			void SaveSettings(TiXmlElement* cf_element)
			{
				cf_element->SetAttribute("checkForYeloFilesFirst", Settings::BooleanToString(check_for_yelo_files_first));

#if PLATFORM_IS_USER
				if(UseMainmenuOverride())
					cf_element->SetAttribute("mainmenuScenario", mainmenu_scenario_name);
#endif
			}

		}g_yelo_settings;
//////////////////////////////////////////////////////////////////////////
// Cache Size upgrades

		static void MemoryUpgradesInitialize()
		{
			for(int32 x = 0; x < NUMBEROF(K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_LIST); x++)
				*K_MAX_CACHE_SIZE_UPGRADE_ADDRESS_LIST[x] = Enums::k_max_cache_size_upgrade;
		}

		static void MemoryUpgradesDispose()
		{
		}

//////////////////////////////////////////////////////////////////////////

		static const s_original_multipler_map k_original_multiplayer_maps[] = {
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

		s_cache_file_globals* CacheFileGlobals()		PTR_IMP_GET2(cache_file_globals);
		char* RootDirectory()							PTR_IMP_GET2(maps_folder_parent_dir);

#include "TagGroups/CacheFiles.YeloFiles.inl"
#include "TagGroups/CacheFiles.MapList.inl"
#include "TagGroups/CacheFiles.CRC.inl"

		void Initialize()
		{
			MemoryUpgradesInitialize();
			c_cache_format_path_hacks::Initialize();
			g_yelo_settings.InitializeMemoryOverrides();

			Memory::WriteRelativeCall(MapListInitialize, GET_FUNC_VPTR(MULTIPLAYER_MAP_LIST_INITIALIZE_CALL));
		}

		void Dispose()
		{
			MemoryUpgradesDispose();
		}

		static bool MapIsOriginal(cstring map_name)
		{
			for(int x = 0; x < NUMBEROF(k_original_multiplayer_maps); x++)
				if( !strcmp(map_name, k_original_multiplayer_maps[x].name) )
					return true;

			return false;
		}
		bool ReadHeader(cstring relative_map_name, s_cache_header& out_header, bool& yelo_is_ok, bool exception_on_fail)
		{
			bool file_exists = false;
			bool result = false;
			yelo_is_ok = true;

			string256 map_path;
			c_cache_format_path_hacks::PathHack(map_path, "%s%s%s.map", RootDirectory(), "maps\\", relative_map_name);

			HANDLE f = CreateFileA(map_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
			if(f != INVALID_HANDLE_VALUE)
			{
				file_exists = true;

				DWORD lpNumberOfBytesRead;
				if(ReadFile(f, &out_header, sizeof(out_header), &lpNumberOfBytesRead, NULL) != FALSE && lpNumberOfBytesRead == sizeof(out_header))
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
			}

			if(!result && exception_on_fail)
			{
				if(!file_exists && !MapIsOriginal(relative_map_name))
				{
					if(!yelo_is_ok)
					{
						PrepareToDropError("Detected an invalid (probably old) .yelo map. See next message for map that needs removing.");
					}
					// This isn't actually specific to cache_file_read_header, but to the exception code. I'm just too lazy to add it to EngineFunctions.hpp
					*K_CACHE_FILE_READ_HEADER_EXCEPTION_MAP_NAME = map_path;
					Engine::GatherException(map_path, 0x89, 0x7E, 1);
				}
			}
			else if(!result && !exception_on_fail)
			{
#if !PLATFORM_IS_DEDI
				// insert map download here as this is probably only reached on multiplayer
				Networking::HTTP::Client::MapDownload::AddMapForDownload(relative_map_name);
#endif
			}

			return result;
		}

		void LoadSettings(TiXmlElement* cf_element)
		{
			g_yelo_settings.LoadSettings(cf_element);
		}

		void SaveSettings(TiXmlElement* cf_element)
		{
			g_yelo_settings.SaveSettings(cf_element);
		}
	};
};