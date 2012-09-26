/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once
#include "Memory/Data.hpp"

#include <blamlib/Halo1/game/game_allegiance.hpp>
#include <blamlib/Halo1/game/game_globals.hpp>
#include <blamlib/Halo1/game/game_time.hpp>
#include <blamlib/Halo1/game/main.hpp>
#include <blamlib/Halo1/physics/physics.hpp>
#include <blamlib/Halo1/physics/point_physics.hpp>
#include <blamlib/Halo1/saved_games/game_state.hpp>
#include <blamlib/Halo1/scenario/scenario.hpp>

#include <TagGroups/Halo1/global_definitions.hpp> // for game teams

namespace Yelo
{
	namespace TagGroups
	{
		struct bitmap_data;

		struct s_sound_permutation;

		struct scenario;

		struct s_game_globals;

		struct collision_bsp;
		struct structure_bsp;
	};
};

namespace Yelo
{
	namespace Enums
	{
		enum {
			// Default address of the game state in memory
			k_game_state_base_address = Enums::k_physical_memory_base_address,
			// Max amount of memory addressable by the game state. After this comes tag memory
			k_game_state_cpu_size = Enums::k_game_state_allocation_size,

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

#if !PLATFORM_IS_DEDI
		struct s_sound_cache_datum : Memory::s_datum_base
		{
			UNKNOWN_TYPE(bool);
			UNKNOWN_TYPE(bool);
			UNKNOWN_TYPE(byte); // haven't seen this used, don't know the exact type
			UNKNOWN_TYPE(byte);
			UNKNOWN_TYPE(bool);
			UNKNOWN_TYPE(byte); // haven't seen this used, don't know the exact type
			int16 cache_read_request_index;
			PAD16;
			TagGroups::s_sound_permutation* sound;
		}; BOOST_STATIC_ASSERT( sizeof(s_sound_cache_datum) == 0x10 );

		// TODO: this structure layout needs some fixin'
		struct s_texture_cache_datum : Memory::s_datum_base
		{
			int16 cache_read_request_index;
			UNKNOWN_TYPE(bool);
			UNKNOWN_TYPE(bool);
			PAD16;
			TagGroups::bitmap_data* bitmap;
			IDirect3DBaseTexture9* hardware_format; // the address of this field is returned by the texture request function
		}; BOOST_STATIC_ASSERT( sizeof(s_texture_cache_datum) == 0x10 );

		template<typename DatumT, uint32 DatumCount> struct s_resource_cache
		{
			Memory::DataArray<DatumT, DatumCount>* data;
			void* base_address;
			Memory::s_lruv_cache* lruv_cache;
			bool initialized;
			PAD24;
		};
#endif

		struct s_physical_memory_map_globals
		{
#if !PLATFORM_IS_DEDI
			s_resource_cache<s_sound_cache_datum, 512> pc_sound_cache;
			s_resource_cache<s_texture_cache_datum, 4096> pc_texture_cache;
#endif

			void* game_state_base_address;
			void* tag_cache_base_address;
			void* texture_cache_base_address;
			void* sound_cache_base_address;
		};
		s_physical_memory_map_globals* PhysicalMemoryMapGlobals();


		s_game_state_globals* GameStateGlobals();
		// Allocate an object of type [T] inside the game state memory and return its address.
		// Note: Also updates the game state's cpu allocation size by adding 'sizeof([T])'
		template<typename T>
		T* GameStateMalloc(size_t count = 1)
		{
			s_game_state_globals* gsg = GameStateGlobals();

			byte* base_addr = CAST_PTR(byte*, gsg->base_address) + gsg->cpu_allocation_size;
			const size_t size_of = sizeof(T) * count;

			// Debug check that we don't allocate more memory than the game state has available
			ASSERT((base_addr + size_of) <= PhysicalMemoryMapGlobals()->tag_cache_base_address, 
				"Bit off more game-state than the game could chew!");

			gsg->cpu_allocation_size += size_of;
#if 0 // TODO: If the allocation crc is updated, game states won't be loadable by stock games
			Memory::CRC(gsg->header->allocation_crc, &size_of, sizeof(size_of));
#endif

			return CAST_PTR(T*, base_addr);
		}


		s_game_globals*				GameGlobals();
		s_game_time_globals*		GameTimeGlobals();
		s_game_allegiance_globals*	GameAllegianceGlobals();
		Game::s_scenario_globals*	ScenarioGlobals();
		s_point_physics_globals*	PointPhysics();
		// Reference to the current platform's physics globals
		s_physics_globals*			Physics();



		// Scenario Tag data
		TagGroups::scenario*		Scenario();
		// Globals tag data
		TagGroups::s_game_globals*	GlobalGameGlobals();
		// Pointer to the current SBPS's bsp3d block
		TagGroups::collision_bsp*	Bsp3d();
		// Pointer to the current SBPS's collision bsp block
		TagGroups::collision_bsp*	CollisionBsp();
		// Pointer to the current SBSP's definition
		TagGroups::structure_bsp*	StructureBsp();
		
		// Index of the Scenario definition in the tag table
		datum_index	ScenarioIndex();
		// Index of the current SBSP in the Scenario's structure bsps block
		int16		StructureBspIndex();
		byte*		DeveloperMode();
		bool		DevmodeEnabled();

		// Are OS-modified game states in effect?
		// If so, these will render game saves incompatible with stock games
		bool		YeloGameStateEnabled();

		void Initialize();
		void Dispose();
		// Called only once: after all other game systems have been initialized at game startup.
		void PLATFORM_API InitializeForNewGameState();
		void PLATFORM_API InitializeForNewMap();
		void PLATFORM_API DisposeFromOldMap();
		// Don't call me unless your name is GameEngine!
		void Update(real delta_time);
		// Called in Initialize(). Don't call me from anywhere else for now.
		void InitializeForYeloGameState(bool enabled);

		// Called from ScriptingLibrary's Initialize (since the GameState is initialized before Scripting is)
		void InitializeScripting();
	};
};