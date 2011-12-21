/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "TagGroups/CacheFiles.hpp"

#include <Blam/Halo1/BlamMemoryUpgrades.hpp>
#include <Blam/Halo1/BlamDataFileUpgrades.hpp>
#include "Memory/MemoryInterface.hpp"
#include "Common/YeloSettings.hpp"
#include "Game/EngineFunctions.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_TAGGROUPS
#define __EL_INCLUDE_FILE_ID	__EL_TAGGROUPS_CACHE_FILES
#include "Memory/_EngineLayout.inl"

	namespace DataFiles
	{
		// Unload the data files, then reload them
		API_FUNC_NAKED static void DataFilesReInitialize()
		{
			static const uint32 OPEN = GET_FUNC_PTR(DATA_FILES_OPEN);
			static const uint32 CLOSE = GET_FUNC_PTR(DATA_FILES_CLOSE);

			__asm {
				call	CLOSE
				call	OPEN
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

		static void ScenarioTagsLoadPreprocess(s_cache_header* cache_header)
		{
			// when a yelo made cache is loaded, this will be set to true, causing non-yelo-made 
			// caches to load the original data files. This isn't required as yelo bases 
			// its data files off the original ones, but I thought it would be good practice %-)
			static bool revert_file_names = false;

			if(cache_header->yelo.flags.uses_mod_data_files && cache_header->yelo.mod_name[0] != '\0')
			{
				const char (*bitmaps)[MAX_PATH];
				const char (*sounds) [MAX_PATH];
				const char (*locale) [MAX_PATH];
				FindSet(cache_header->yelo.mod_name,
					bitmaps, sounds, locale);

				DataFilesUseNewFiles(*bitmaps, *sounds, *locale);

				int16 original_index = Cache::CacheFileGlobals()->current_cache_files.index;
				Cache::CacheFileGlobals()->current_cache_files.index = NONE;
				DataFilesReInitialize();
				Cache::CacheFileGlobals()->current_cache_files.index = original_index;

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
#if PLATFORM_IS_USER
		struct s_yelo_settings {
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

			void InitializeMemoryOverrides()
			{
				if(UseMainmenuOverride())
				{
					for(int32 x = 0; x < NUMBEROF(K_UI_SCENARIO_NAME_REFERENCES); x++)
						*K_UI_SCENARIO_NAME_REFERENCES[x] = mainmenu_scenario_name;
				}
			}
		}g_yelo_settings;
#endif
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

		static s_original_multipler_map original_multiplayer_maps[] = {
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

		void Initialize()
		{
			MemoryUpgradesInitialize();
			CacheFormatPathHackInitialize();
#if PLATFORM_IS_USER
			g_yelo_settings.InitializeMemoryOverrides();
#endif

			Memory::WriteRelativeCall(MapListInitialize, GET_FUNC_VPTR(MULTIPLAYER_MAP_LIST_INITIALIZE_CALL));
		}

		void Dispose()
		{
			MemoryUpgradesDispose();
		}

		bool ReadHeader(cstring relative_map_name, s_cache_header& out_header)
		{
			bool result = false;

			string256 map_path;
			CacheFormatPathHack(map_path, "%s%s%s.map", RootDirectory(), "maps\\", relative_map_name);

			HANDLE f = CreateFileA(map_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
			if(f != INVALID_HANDLE_VALUE)
			{
				DWORD lpNumberOfBytesRead;
				if(ReadFile(f, &out_header, sizeof(out_header), &lpNumberOfBytesRead, NULL) != FALSE && lpNumberOfBytesRead == sizeof(out_header))
				{
					if( out_header.ValidSignatures() && 
						out_header.ValidFileSize(Enums::k_max_cache_size_upgrade) && 
						out_header.ValidName() && 
						out_header.version == s_cache_header::k_version && 
						out_header.yelo.IsValid())
						result = true;
				}

				CloseHandle(f);
			}

			return result;
		}

#if PLATFORM_IS_USER
		void LoadSettings(TiXmlElement* cf_element)
		{
			if(cf_element != NULL)
			{
				cstring mainmenu_override_name = cf_element->Attribute("mainmenuScenario");

				g_yelo_settings.InitializeMainmenuOverride(mainmenu_override_name);
			}
		}

		void SaveSettings(TiXmlElement* cf_element)
		{
			if(g_yelo_settings.UseMainmenuOverride())
				cf_element->SetAttribute("mainmenuScenario", g_yelo_settings.mainmenu_scenario_name);
		}
#endif
	};
};