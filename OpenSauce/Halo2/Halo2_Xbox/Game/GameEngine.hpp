/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum game_engine
		{
			_game_engine_none,
			_game_engine_campaign = _game_engine_none,
			_game_engine_ctf,
			_game_engine_slayer,
			_game_engine_oddball,
			_game_engine_koth,

			// deprecated
			_game_engine_race,
			// deprecated
			_game_engine_headhunter,

			_game_engine_juggernaut,
			_game_engine_territories,
			_game_engine_assualt,
			_game_engine,
		};
	};

	namespace GameEngine
	{
		struct c_definition
		{
			void* get_type; // 0x0
			void* initialize; // 0x4
			void* UNKNOWN(2); // 0x8
			void* UNKNOWN(3); // 0xC
			void* UNKNOWN(4); // 0x10
			void* UNKNOWN(5); // 0x14
			void* UNKNOWN(6); // 0x18
			void* UNKNOWN(7); // 0x1C
			void* UNKNOWN(8); // 0x20
			void* UNKNOWN(9); // 0x24
			void* UNKNOWN(10); // 0x28
			void* UNKNOWN(11); // 0x2C
			void* UNKNOWN(12); // 0x30
			void* UNKNOWN(13); // 0x34
			void* UNKNOWN(14); // 0x38
			void* UNKNOWN(15); // 0x3C
			void* UNKNOWN(16); // 0x40
			void* UNKNOWN(17); // 0x44
			void* UNKNOWN(18); // 0x48
			void* UNKNOWN(19); // 0x4C
			void* UNKNOWN(20); // 0x50
			void* UNKNOWN(21); // 0x54
			void* UNKNOWN(22); // 0x58
			void* UNKNOWN(23); // 0x5C
			void* UNKNOWN(24); // 0x60
			void* UNKNOWN(25); // 0x64
			void* UNKNOWN(26); // 0x68
			void* UNKNOWN(27); // 0x6C
			void* UNKNOWN(28); // 0x70
			void* UNKNOWN(29); // 0x74
			void* UNKNOWN(30); // 0x78
			void* UNKNOWN(31); // 0x7C
			void* UNKNOWN(32); // 0x80
			void* UNKNOWN(33); // 0x84
			void* UNKNOWN(34); // 0x88
			void* UNKNOWN(35); // 0x8C
			void* UNKNOWN(36); // 0x90
			void* UNKNOWN(37); // 0x94
			void* UNKNOWN(38); // 0x98
			void* UNKNOWN(39); // 0x9C
			void* UNKNOWN(40); // 0xA0
			void* UNKNOWN(41); // 0xA4
			void* UNKNOWN(42); // 0xA8
			void* UNKNOWN(43); // 0xAC
			void* UNKNOWN(44); // 0xB0
			void* UNKNOWN(45); // 0xB4
			void* UNKNOWN(46); // 0xB8
			void* UNKNOWN(47); // 0xBC
			void* UNKNOWN(48); // 0xC0
			void* UNKNOWN(49); // 0xC4
			void* UNKNOWN(50); // 0xC8
		};

		struct s_slayer_engine_state_data : TStructImpl(36)
		{
			TStructGetPtrImpl(word_flags, ValidTeams, 0x0);
			//TStructGetPtrImpl(int16, , 0x2);
			TStructGetPtrImpl(word_flags, InitialTeams, 0x4);
			TStructGetPtrImpl(word_flags, ActiveTeams, 0x6);
			TStructGetPtrImpl(word_flags, EverActiveTeams, 0x8);
			TStructGetPtrImpl(int16, TeamDesignatorToTeamIndex, 0xA); // 8 shorts
			TStructGetPtrImpl(int16, RoundIndex, 0x1E);
		};
	};

	namespace GameState
	{
		struct s_ctf_globals // 0x208
		{
		};

		struct s_slayer_globals // 0xC
		{
		};

		struct s_oddball_globals // 0x1C
		{
		};

		struct s_koth_globals // 0x1DC
		{
			// real_rgb @ 0x1AC, 0x1B8, 0x1C4, 0x1D0
		};

		struct s_juggernaut_globals // 0x
		{
		};

		struct s_territories_globals // 0x118
		{
		};

		struct s_game_engine_globals : TStructImpl(3228)
		{
			// k_game_engine_timer_count = 3
			// int16 valid_team_designators; // 0x4
			// int16 valid_teams; // 0x6
			// int16 allowable_team_designators; // 0x8
			// int16 initial_teams; // 0xA
			// 0x254 byte struct @ 0x304
			// 0x320 byte struct @ 0x7DC. datum_index[200] for each net-game equipment's object_index

			struct s_multiplayer_weapon // [128]
			{
			};

			TStructGetPtrImpl(s_slayer_globals, Slayer,				PLATFORM_VALUE(0xFC, 0xFC, 0x78));
			TStructGetPtrImpl(s_ctf_globals, Ctf,					PLATFORM_VALUE(0xFC, 0xFC, 0x78));
			TStructGetPtrImpl(s_oddball_globals, Oddball,			PLATFORM_VALUE(0xFC, 0xFC, 0x78));
			TStructGetPtrImpl(s_koth_globals, Koth,					PLATFORM_VALUE(0xFC, 0xFC, 0x78));
			TStructGetPtrImpl(s_juggernaut_globals, Juggernaut,		PLATFORM_VALUE(0xFC, 0xFC, 0x78));
			TStructGetPtrImpl(s_territories_globals, Territories,	PLATFORM_VALUE(0xFC, 0xFC, 0x78));

			TStructGetPtrImpl(int32, EngineIndex,					PLATFORM_VALUE(0xC14, 0xC14, 0xC4C));
		};

		s_game_engine_globals* _GameEngine();
	};
};