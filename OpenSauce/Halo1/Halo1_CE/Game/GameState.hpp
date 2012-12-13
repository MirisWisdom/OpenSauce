/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include "Memory/MemoryInterface.hpp" // for CRC

#include <blamlib/Halo1/cache/physical_memory_map.hpp>
#include <blamlib/Halo1/game/game_allegiance.hpp>
#include <blamlib/Halo1/game/game_globals.hpp>
#include <blamlib/Halo1/game/game_time.hpp>
#include <blamlib/Halo1/main/main.hpp>
#include <blamlib/Halo1/physics/physics.hpp>
#include <blamlib/Halo1/physics/point_physics.hpp>
#include <blamlib/Halo1/saved_games/game_state.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		struct s_game_globals;
	};
};

namespace Yelo
{
	namespace Enums
	{
		enum {
			// How much additional memory, if any, we allocate for the objects pool
			k_game_state_allocation_size_object_memory_pool_upgrade = 0x10000,
			// 0x42974 bytes available in game state allocation for Clients.
			// Dedis should have a little bit more (since they don't alloc a few render based things) 
			// but to keep things simple we'll limit both sides of the spectrum to the same size.
			// UPDATE: 0x50A78 bytes for Dedi (stock only allocates 0x3EF588 bytes)
			// NOTE: Basically, the game doesn't fully use the amount it sets aside for game state data 
			// (ie, objects, players, etc), so we're able to steal some of this memory for our own use. 
			// Why? Because the game state is saved to file when the player saves the game or dumps a core. 
			// So if there's a project component which requires serialization, we'd want to allocate its 
			// memory in the game state, not anywhere else.
			k_game_state_allocation_maximum_size_for_yelo = 0x42970
				- k_game_state_allocation_size_object_memory_pool_upgrade,

			// Default address of the tag cache in memory (comes right after the game state memory by default)
			k_tag_cache_base_address = k_game_state_base_address + k_game_state_cpu_size,
		};
		BOOST_STATIC_ASSERT( k_game_state_allocation_maximum_size_for_yelo >= 0 );

		enum {
			// How many values we allow in the runtime data game state for each type (ie, integers, real, etc)
			k_runtime_data_max_values_count = 64,
		};

		enum {
			// This is a partially made up constant, though when dev-mode is >= this, it outputs messages to the console
			k_developer_mode_level_debug_output = 4,
		};
	};

	namespace GameState
	{
		void WriteEvent(cstring str = "", bool write_time_stamp = true);


		s_main_globals* MainGlobals();
		s_physical_memory_map_globals* PhysicalMemoryMapGlobals();


		s_game_state_globals* GameStateGlobals();
		// Allocate an object of type [T] inside the game state memory and return its address.
		// Note: Also updates the game state's cpu allocation size by adding 'sizeof([T])'
		template<typename T>
		T* GameStateMalloc(const bool k_update_allocation_crc = true, const size_t count = 1)
		{
			s_game_state_globals* gsg = GameStateGlobals();

			byte* base_addr = CAST_PTR(byte*, gsg->base_address) + gsg->cpu_allocation_size;
			const size_t size_of = sizeof(T) * count;

			// Debug check that we don't allocate more memory than the game state has available
			ASSERT((base_addr + size_of) <= PhysicalMemoryMapGlobals()->tag_cache_base_address, 
				"Bit off more game-state than the game could chew!");

			gsg->cpu_allocation_size += size_of;
			// If the allocation crc is updated, game states won't be loadable by stock games
			if(k_update_allocation_crc)
				Memory::CRC(gsg->header->allocation_crc, &size_of, sizeof(size_of));

			return CAST_PTR(T*, base_addr);
		}


		s_game_globals*				GameGlobals();
		s_game_time_globals*		GameTimeGlobals();
		s_game_allegiance_globals*	GameAllegianceGlobals();
		s_point_physics_globals*	PointPhysics();
		// Reference to the current platform's physics globals
		s_physics_globals*			Physics();



		// Globals tag data
		TagGroups::s_game_globals*	GlobalGameGlobals();
		
		byte*		DeveloperMode();
		bool		DevmodeEnabled();

		// Are OS-modified game states in effect?
		// If so, these will render game saves incompatible with stock games
		bool		YeloGameStateEnabled();

		void Initialize();
		void Dispose();
		// Don't call me unless your name is GameEngine!
		void Update(real delta_time);

		void PLATFORM_API InitializeForNewMap();
		void PLATFORM_API DisposeFromOldMap();
		
		// Called only once: after all other game systems have been initialized at game startup.
		void PLATFORM_API InitializeForNewGameState();
		// Called in Initialize(). Don't call me from anywhere else for now.
		void InitializeForYeloGameState(bool enabled);
		// These handlers are called by internal game state procs code. Don't touch.
		void PLATFORM_API HandleBeforeSaveLifeCycle();
		void PLATFORM_API HandleBeforeLoadLifeCycle();
		void PLATFORM_API HandleAfterLoadLifeCycle();

		// Called from ScriptingLibrary's Initialize (since the GameState is initialized before Scripting is)
		void InitializeScripting();
	};
};