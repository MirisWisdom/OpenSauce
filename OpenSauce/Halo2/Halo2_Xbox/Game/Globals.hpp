/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

#include <blamlib/Halo2/memory/data.hpp>

#include "Game/GlobalDefinitions.hpp"
#include "Scenario/ScenarioDefinitions.hpp"

namespace Yelo
{
	namespace Enums
	{
		enum game_simulation
		{
			_game_simulation_none,
			_game_simulation_local,
			_game_simulation_synchronous_client,
			_game_simulation_synchronous_server,
			_game_simulation_film_playback,
			_game_simulation_distributed_client,
			_game_simulation_distributed_server,
			k_game_simulation_count
		};
	};

	namespace GameState
	{
		struct s_game_options : TStructImpl(4376)
		{
		};

		struct s_header : TStructImpl(4744)
		{
			// options start at 0x130
		};
		s_header* _Header();

		struct s_random_math_globals
		{
			uint32 UNKNOWN(0);
			uint32 UNKNOWN(1);
		};
		s_random_math_globals* _RandomGlobals();

		struct s_game_globals : TStructImpl(4608)
		{
			TStructGetPtrImpl(bool, Initializing, 0x0);
			TStructGetPtrImpl(bool, MapActive, 0x1);
			TStructGetPtrImpl(int16, ActiveStructureBspIndex, 0x2);
			TStructGetPtrImpl(_enum, SimulationMode, 0xC); // [Enums::game_simulation]

			TStructGetPtrImpl(long_enum, ScenarioType, 0x8 + 0x0); // options start here, sizeof(0xA50) in alpha
			TStructGetPtrImpl(string256, ScenarioPath, 0x8 + 0x1C);
			TStructGetPtrImpl(void*, GameVariant, 0x8 + 0x130);

			TStructGetPtrImpl(bool, IsPlayTest, 0x130);
			TStructGetPtrImpl(bool, IsCooperative, 0x134);
			TStructGetPtrImpl(bool, GameInProgress, 0xA58);
			TStructGetPtrImpl(bool, Reverted, 0x1121);
			TStructGetPtrImpl(uint32, RevertTime, 0x1124);

			// short machine_door_portal_indices_count; // 0xC28
		};
		s_game_globals* _GameGlobals();

		struct s_game_time_globals
		{
			bool Initialized;
			bool Paused;
			uint16 TickRate; // FPS
			real Time;
			uint32 Elapsed; // in seconds, * 30 or MaxFPS for the frames elapsed
			real GameSpeed;
			PAD(0, sizeof(real)*5);
			// 0x10 lone real
			// 0x14, 0x18, 0x1C, seem to be xyz or something
			// 0x20 lone real
		};
		s_game_time_globals* _GameTimeGlobals();

		struct s_level_handle_datum
		{
			PAD32;
			PAD32;
		};
		typedef Memory::DataArray<s_level_handle_datum, 20> t_campaign_level_data;
		t_campaign_level_data* _CampaignLevelHandles();
		typedef Memory::DataArray<s_level_handle_datum, 50> t_multiplayer_level_data;
		t_multiplayer_level_data* _MultiplayerLevelHandles();


		GameGlobals::group* _Globals();

		Scenario::group* _Scenario();
	};
};