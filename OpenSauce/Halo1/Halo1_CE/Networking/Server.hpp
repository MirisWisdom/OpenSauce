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
#pragma once

#include "Memory/Data.hpp"
#include "Memory/MemoryInterface.hpp"
#include "Game/GameEngine.hpp"

namespace Yelo
{
#if PLATFORM_IS_DEDI
	namespace Enums
	{
		enum server_event_type : _enum
		{
			// datetime string format: "%04d-%02d-%02d %02d:%02d:%02d"
			// <datetime>\t<event name>\t<log sprintf>\r\n

			// All indices are based starting from one (instead of zero)
			// So PlayerNumber and TeamIndex are both +1 when logged

			//OPEN
				//Log opened
				//Log continued from <old log file name>
			_server_event_type_log_open,
			//CHAT
				//GLOBAL \t<player name>\t<chat msg>
				//TEAM   \t<player name>\t<chat msg>
				//VEHICLE\t<player name>\t<chat msg>
				//SV_SAY \t<player name>\t<chat msg>
				//**SERVER**\tSV_SAY \t<chat msg>
			_server_event_type_chat,
			//NOTE
			_server_event_type_note,
			//CLOSE
			_server_event_type_log_close,
			//BAN
				//BANNED\t"<player name>"\tbans <ban count>\tcd key <cd hash>\tuntil <end datetime string>
				//BANNED\t"<player name>"\tbans <ban count>\tcd key <cd hash>\tuntil <INDEFINITE>
				//AUTOBAN\t"<player name>"\tcd key <cd hash>
				//UNBAN\t"<player name>"\tcd key <cd hash>
				//REJECT\t"<player name>"\tcd key <cd hash>
				//SV_KICK\t"<player name>"\tcd key <cd hash>
				//SV_BAN\t"<player name>"\tcd key <cd hash>
			_server_event_type_player_ban,
			//JOIN
				//SUCCESS\t"<player name>"\tplayer <player number>\tmachine <machine number>\t(<ip address>)\tcdkey <cd hash> // yes, 'cdkey'
				//FAILURE\t
					//BAD PASSWORD\t
					//GAME FULL\t
					//BAD CDKEY\t
					//BANNED\t
					//UNKNOWN\t
						//"<player name>"\tmachine <machine number>\t(<ip address>)\tcdkey <cd hash>
			_server_event_type_player_join,
			//QUIT
				//"<player name>"\tmachine <machine number>\t(<ip address>)\tcdkey <cd hash>
				//<No Player>\tmachine <machine number>\t(<ip address>) // no-player is not a param, but actual output
			_server_event_type_player_quit,
			//PCR_PLAYER
			_server_event_type_pcr_player,
			//PCR_TEAM
			_server_event_type_pcr_team,
			//TEAMS
				//JOIN\t"<player name>"\tplayer <player number>\tteam <team index>
				//LEAVE\t"<player name>"\tplayer <player number>\tteam <team index>
			_server_event_type_teams,
			//WARNING
			_server_event_type_warning,
			//KILL
			_server_event_type_kill,
			//SCORE
				//CTF\tplayer <player number>\t"<player name>"\tteam <team index>
			_server_event_type_score,
			//GRIEF
			_server_event_type_grief,
			//RCON
			_server_event_type_rcon,
			//GAMEINFO
			_server_event_type_game_info,

			_server_event_type,
		};
	};
#endif

	namespace Networking
	{
		// game-spy machine data
		struct gs_machine_data
		{
			int32 unknown1; // 0x0
			char cd_hash[32]; // 0x4
			void* unknown2; // 0x24
			uint32 skey; // 0x28, (GetTickCount ^ rand) & 0x3FFF
			struct { // 0x2C
				byte class_d;
				byte class_c;
				byte class_b;
				byte class_a;
			}ip;
			PAD32; // 0x30, GetTickCount
			PAD32; // 0x34
			PAD32; //int32 join_result; // 0x38, 1 = ?, 2 = error, 3 = ?; 1 & 3 seem to mean stuff is ok
			PAD32; // 0x3C
			PAD32; // 0x40, void* proc_unk
			PAD32; //char* join_errmsg; // 0x44

			// \auth\\pid\%d\ch\%s\resp\%s\ip\%d\skey\%dd
			PAD32; // 0x48, malloc'd char*
			PAD32; // 0x4C, char*'s length
		};

		struct gs_machine_data_list_entry
		{
			gs_machine_data* machine;
			gs_machine_data_list_entry* next;
			gs_machine_data_list_entry* prev;
		};

		struct gs_machine_data_list
		{
			int32 game_pid;
			gs_machine_data_list_entry entries;
		};


		// If this is a server, returns all the machines connected to this machine
		gs_machine_data_list* GsMachines(); // [4]


		// Password for this machine
		wcstring ServerPassword();
		// Remote console password for this machine
		cstring ServerRconPassword();
		// Time limit for this machine
		int32 ServerTimelimit();

		// Port used to host a server
		int32 ConnectionPort();
		// Port used to connect to the server
		int32 ConnectionClientPort();

		gs_machine_data* GamespyGetMachine(int32 machine_id);
	};

	namespace Server
	{
		struct s_banlist_entry
		{
			wchar_t player_name[12+1];	// 0x0
			char cd_hash[32];			// 0x1A
			UNKNOWN_TYPE(bool);			// 0x3A
			PAD8;
			int16 ban_count;			// 0x3C
			bool banned_to_infinity;	// 0x3E
			PAD8;
			__time32_t ban_end_time;	// 0x40

			typedef Memory::DynamicArray<s_banlist_entry> array_t;
		}; BOOST_STATIC_ASSERT( sizeof(s_banlist_entry) == 0x44 );

		struct s_mapcycle_entry
		{
			cstring map_name;		// Both of these are GlobalAlloc'd
			wcstring variant_name;
		};

		struct s_network_sv_file_globals
		{
			Memory::DynamicArray<s_banlist_entry> banlist;
			Memory::DynamicArray<s_mapcycle_entry> mapcycles; // defined in 'client' builds, but no code which references it
			PAD32;
#if PLATFORM_IS_DEDI
			bool log_enabled; // this isn't padded...

		private:
			byte log_file_reference[0x10C];

		public:
			bool logging_enabled;
			PAD16;

			uint32 log_rotation_threshold;

		private:
			byte event_enable_logging[Enums::_server_event_type];
			byte event_enable_echoing[Enums::_server_event_type];
			PAD32;
#endif
		private:
			char banlist_file[0x108];
		}; BOOST_STATIC_ASSERT( sizeof(s_network_sv_file_globals) == PLATFORM_VALUE(0x124, 0x25C) );


		struct s_network_sv_mapcycle_entry
		{
			s_mapcycle_entry definition;
			PAD32; // ? if this is anything, it isn't a pointer, that I can be sure of
			GameEngine::s_game_variant variant;
		};

		struct s_network_sv_mapcycle_globals
		{
			Memory::GbxArray<s_network_sv_mapcycle_entry> loaded_mapcycles;
			int32 current_cycle;

			// dedi only: 4CF2C0. loads the map-cycle definitions
		};


		struct s_network_sv_globals
		{
			bool initialized;
			PAD24;
			uint32 last_status_display_time; // game_time
			wchar_t server_password[8+1];
			bool single_flag_force_reset;
			PAD8;
			char banlist_path[MAX_PATH];
#if PLATFORM_IS_DEDI
			HANDLE banlist_mutex;
#endif
			long_enum friendly_fire_type;
			char rcon_password[12];
#if PLATFORM_IS_DEDI
			struct {
				uint32 last_time; // (last rotation time) game_time
				uint32 next_time; // (next rotation time) game_time
			}log_rotation;
#else
			PAD32;
#endif
			struct {
				char file_name[256];
				char text[256];
			}motd;
		}; BOOST_STATIC_ASSERT( sizeof(s_network_sv_globals) == PLATFORM_VALUE(0x334,0x33C) );


		s_network_sv_file_globals* NetworkSvFileGlobals();

		s_network_sv_mapcycle_globals* NetworkSvMapcycleGlobals();

		s_network_sv_globals* NetworkSvGlobals();


		void Initialize();
		void Dispose();


#if PLATFORM_IS_DEDI
		int32 ServerInstance();


		//////////////////////////////////////////////////////////////////////////
		// Logging State: 'true' if the [event_type] is output to the log file
		// Echoing State: 'true' if the [event_type] is echo'd to the console window of the server

		bool EventTypeLoggingStateGet(Enums::server_event_type event_type);
		void EventTypeLoggingStateSet(Enums::server_event_type event_type, bool state);
		bool EventTypeEchoingStateGet(Enums::server_event_type event_type);
		void EventTypeEchoingStateSet(Enums::server_event_type event_type, bool state);
#endif
	};
};