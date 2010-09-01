/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
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
#include "TagGroups/CacheFiles.hpp"

#include <Blam/Halo1/BlamMemoryUpgrades.hpp>
#include "Memory/MemoryInterface.hpp"
#include "Common/YeloSettings.hpp"
#include <Blam/Halo1/BlamDataFileUpgrades.hpp>

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
			static uint32 OPEN = GET_FUNC_PTR(DATA_FILES_OPEN);
			static uint32 CLOSE = GET_FUNC_PTR(DATA_FILES_CLOSE);

			__asm {
				push	ecx
				call	CLOSE
				call	OPEN
				pop		ecx
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
			// when a yelo made cache is loaded, this will be set
			// to true, causing non-yelo made caches to load the 
			// original data files. This isn't required as yelo 
			// bases it's data files off the original ones, but 
			// I thought it would be good practice %-)
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

		// v1.08: 0x629B78, 0x59F518 - original mp map table
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

		t_multiplayer_map_data* MultiplayerMaps()		PTR_IMP_GET2(multiplayer_maps);
		s_cache_file_globals* CacheFileGlobals()		PTR_IMP_GET2(cache_file_globals);

		void Initialize()
		{
			MemoryUpgradesInitialize();
		}

		void Dispose()
		{
			MemoryUpgradesDispose();
		}

		API_FUNC_NAKED void MultiplayerMapsReIntialize()
		{
			static uint32 Initialize = GET_FUNC_PTR(MULTIPLAYER_MAP_LIST_INITIALIZE);
			static uint32 Dispose = GET_FUNC_PTR(MULTIPLAYER_MAP_LIST_DISPOSE);

			__asm {
				call	Dispose
				call	Initialize
				retn
			}
		}
	};
};