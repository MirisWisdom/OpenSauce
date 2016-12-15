/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/game/game_engine.hpp>
#include <blamlib/Halo1/objects/vehicle_remapper.hpp>
#include <blamlib/Halo1/saved_games/game_variant.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum game_engine_custom : long_enum
		{
			_game_engine_custom1 = k_number_of_game_engines,
			_game_engine_custom2,
			_game_engine_custom3,
			_game_engine_custom4,
			_game_engine_custom5,
			_game_engine_custom,
		};

		enum game_trait_custom : long_enum
		{
			_game_custom_trait1 = _game_trait+1,
			_game_custom_trait2,
			_game_custom_trait3,
			_game_custom_trait4,
			_game_custom_trait5,
			_game_custom_trait,
		};

		enum game_engine_mode : long_enum
		{
			_game_engine_mode_active,
			_game_engine_mode_postgame_delay,
			_game_engine_mode_postgame_rasterize_delay,
			_game_engine_mode_3,
		};
	};

	namespace GameEngine
	{
		s_ctf_globals* CtfGlobals();
		s_slayer_globals* SlayerGlobals();
		s_oddball_globals* OddballGlobals();
		s_king_globals* KingGlobals();
		s_race_globals* RaceGlobals();

		s_game_variant* GlobalVariant();
		//s_vehicle_remapper
		// v1.08: 0x7FB300


		// List of old game engines which can be used
		const game_engine_definition* const* Definitions();
		// Current game engine being used
		const game_engine_definition* Current();

		// List of original and custom made game engines
		const game_engine_definition* const* NewDefinitions();
		// How many total game engine definitions there are (that are valid and not null)
		int32 NewDefinitionsCount();

		// Register a engine definition so that it may be used by the game code
		void RegisterNewGameEngine(game_engine_definition* engine);

		void Initialize();
		void Dispose();
		void PLATFORM_API InitializeForNewMap();
		// Game components updated before the game_enigne:
		//	* game allegiance
		//	* ai
		//	* players (if we're not the server)
		//	* effects
		//	* FP weapons
		// Everything else is updated after the game_engine (scripting, objects, etc).
		// Our update is performed AFTER the engine's update is done.
		void PLATFORM_API Update();
	};
};