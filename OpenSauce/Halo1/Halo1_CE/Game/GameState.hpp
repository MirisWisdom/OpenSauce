/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once
#include "Memory/Data.hpp"

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


		struct s_main_globals
		{
			UNKNOWN_TYPE(uint32); // time related
			PAD32;
			LARGE_INTEGER performance_counter;
			UNKNOWN_TYPE(bool);
			bool is_taking_screenshot;
			PAD16;
			real delta_time;
			_enum game_connection;

			struct s_screenshot {
				int16 counter;
				void* movie; // screenshot bitmap
				PAD32; PAD32;
				int32 movie_frame_index;
				UNKNOWN_TYPE(real);
			}screenshot;

			struct s_map {
				bool reset_map;
				bool switch_to_campaign;
				bool revert_map;
				bool skip_cinematic;
				bool save_map;
				bool save_map_nonsafe;
				bool save_map_with_timeout;
				bool is_saving_map;
				int32 saving_map_timeout_countdown;
				int32 saving_map_timeout_timer;
				UNKNOWN_TYPE(int32);
				UNKNOWN_TYPE(int16);
				bool won_map;
				bool lost_map;
				bool respawn_coop_players;

				struct s_core {
					bool save;
					bool load;
					bool load_at_startup;
				}core;

				int16 switch_to_structure_bsp; // if not NONE, switches to the scenario's bsp by index
				bool main_menu_scenario_loaded;
				bool main_menu_scenario_load;
			}map;

			UNUSED_TYPE(bool);
			UNKNOWN_TYPE(bool);
			UNKNOWN_TYPE(bool);
			bool quit;
			UNKNOWN_TYPE(int32);
			UNKNOWN_TYPE(int32);
			UNKNOWN_TYPE(int32);
			bool set_game_connection_to_film_playback;
			bool time_is_stopped;
			bool start_time;
			UNUSED_TYPE(bool);

			bool skip_frames;
			PAD8;
			int16 skip_frames_count;

			int16 lost_map_count;
			int16 respawn_count;

			UNKNOWN_TYPE(bool);
			PAD24;
			UNKNOWN_TYPE(bool);
			char scenario_tag_path[256];
			char multiplayer_map_name[256];
			char queued_map[256];

			UNKNOWN_TYPE(bool);
			UNKNOWN_TYPE(tag_string);
			PAD(0, 8+1); // char[8+1]
			PAD8;
			PAD32;

			void QuitToMainMenu()
			{
				map.switch_to_structure_bsp = NONE;
				map.save_map = false;
				map.main_menu_scenario_load = true;
			}
		}; BOOST_STATIC_ASSERT( sizeof(s_main_globals) == 0x3A0 );
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


		struct s_header_data
		{
			uint32 allocation_crc;
			char level[256];
			tag_string version;
			int16 player_spawn_count;
			_enum game_difficulty;
			uint32 cache_crc;
			byte _unk3[32];
		}; BOOST_STATIC_ASSERT( sizeof(s_header_data) == 0x14C );
		struct s_game_state_globals
		{
			void* base_address;
			uint32 cpu_allocation_size;
			PAD32; // unused
			uint32 crc;
			bool locked;
			bool saved;
			PAD16;
			uint32 revert_time;
			s_header_data* header;

			// etc... there are more fields to this but I could give a flying fuck less about them


			// Allocate an object of type [T] inside the game state memory and return its address.
			// Note: Also updates the game state's cpu allocation size by adding 'sizeof([T])'
			template<typename T>
			T* Malloc(size_t count = 1)
			{
				byte* base_addr = CAST_PTR(byte*, base_address) + cpu_allocation_size;
				const size_t size_of = sizeof(T) * count;

				// Debug check that we don't allocate more memory than the game state has available
				ASSERT((base_addr + size_of) <= PhysicalMemoryMapGlobals()->tag_cache_base_address, 
					"Bit off more game-state than the game could chew!");

				cpu_allocation_size += size_of;
#if 0 // TODO: If the allocation crc is updated, game states won't be loadable by stock games
				Memory::CRC(header->allocation_crc, &size_of, sizeof(size_of));
#endif

				return CAST_PTR(T*, base_addr);
			}
		};
		s_game_state_globals* GameStateGlobals();


		struct s_game_options
		{
			UNKNOWN_TYPE(int16);
			UNKNOWN_TYPE(int16); // ?, not sure about this field
			UNKNOWN_TYPE(int16); // ?, not sure about this field
			_enum difficulty_level;
			int32 random_seed;
			char map_name[0x7F+1];
			byte unknown[0x7F+1]; // pretty sure this is just an unused string
		}; BOOST_STATIC_ASSERT( sizeof(s_game_options) == 0x10C );

		struct s_game_globals
		{
			bool map_loaded;
			bool active;
			bool players_are_double_speed;
			UNKNOWN_TYPE(bool);
			UNKNOWN_TYPE(real);

			s_game_options options;
		}; BOOST_STATIC_ASSERT( sizeof(s_game_globals) == 0x114 );
		s_game_globals*				GameGlobals();

		struct s_game_time_globals
		{
			bool initialized;		// 0x0
			bool active;			// 0x1
			bool paused;			// 0x2
			PAD8;
			UNKNOWN_TYPE(int16);	// 0x4
			UNKNOWN_TYPE(int16);	// 0x6
			UNKNOWN_TYPE(int16);	// 0x8
			PAD16;
			uint32 local_time;		// 0xC, game time
			uint32 elapsed_time;	// 0x10
			uint32 server_time;		// 0x14
			real game_speed;		// 0x18
			real leftover_time_sec;	// 0x1C
		};
		s_game_time_globals*		GameTimeGlobals();

		struct s_game_allegiance_globals
		{
			struct s_allegiance
			{
				Enums::global_game_team this_team;
				Enums::global_game_team other_team;
				int16 threshold;
				UNKNOWN_TYPE(int16);	// 0x6
				UNKNOWN_TYPE(bool);		// 0x8
				UNKNOWN_TYPE(bool);		// 0x9
				bool is_broken;			// 0xA
				UNKNOWN_TYPE(bool);		// 0xB
				UNKNOWN_TYPE(bool);		// 0xC
				PAD8;
				int16 incidents_count;	// 0xE
				UNKNOWN_TYPE(int16);	// 0x10
			}; BOOST_STATIC_ASSERT( sizeof(s_allegiance) == 0x12 );

			int16 current_incidents;
			s_allegiance allegiances[8];
			PAD16;
			long_flags ally_mapping_flags[ BIT_VECTOR_SIZE_IN_DWORDS(Enums::_global_game_team * Enums::_global_game_team) ]; // 0x94
			long_flags enemy_mapping_flags[ BIT_VECTOR_SIZE_IN_DWORDS(Enums::_global_game_team * Enums::_global_game_team) ]; // 0xA4
		}; BOOST_STATIC_ASSERT( sizeof(s_game_allegiance_globals) == 0xB4 );
		s_game_allegiance_globals*	GameAllegianceGlobals();

		struct s_scenario_globals
		{
			int16 current_structure_bsp_index;
			PAD16;
			byte UNKNOWN(0)[0x2C][1]; // only one element on pc, so related to each local player then something about the fog around them?
			bool UNKNOWN(1);
			PAD24;
			byte sound_environment[0x48]; // if I gave a fuck about defining the tag structure, I'd use it here
		}; BOOST_STATIC_ASSERT( sizeof(s_scenario_globals) == 0x7C );
		s_scenario_globals*			ScenarioGlobals();


		struct s_point_physics_globals
		{
		private:
			real water_mass;
			real air_mass;

			static const real DensityToMass(real density) { return density * 118613.34f; }
		public:

			void SetWaterMass(real density)	{ water_mass = DensityToMass(density); }
			void SetAirMass(real density)	{ air_mass = DensityToMass(density); }
		};
		s_point_physics_globals*	PointPhysics();

		struct s_physics_globals
		{
			real gravity;
		private:
			real water_density;
			real air_density;
		public:

			// Halo's normal gravity value constant
			static const real	GravityConstant()		{ return 3.5651792e-3f; }
			static const real	WaterDensityConstant()	{ return 1.0f; }
			static const real	AirDensityConstant()	{ return 0.0011f; }

			void SetGravityScale(real scale)
			{
				gravity = GravityConstant() * scale;
			}
			void SetWaterDensity(real density)
			{
				PointPhysics()->SetWaterMass(water_density = density);
			}
			void SetAirDensity(real density)
			{
				PointPhysics()->SetAirMass(air_density = density);
			}

			void Reset()
			{
				gravity = GravityConstant();
				SetWaterDensity(WaterDensityConstant());
				SetAirDensity(AirDensityConstant());
			}
		}; BOOST_STATIC_ASSERT( sizeof(s_physics_globals) == 0xC );
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