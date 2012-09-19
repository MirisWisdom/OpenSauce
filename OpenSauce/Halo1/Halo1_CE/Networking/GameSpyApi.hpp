/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum gamespy_qr_field
		{
			_gamespy_qr_field_reserved,
			_gamespy_qr_field_hostname,
			_gamespy_qr_field_gamename,
			_gamespy_qr_field_gamever,
			_gamespy_qr_field_hostport,
			_gamespy_qr_field_mapname,
			_gamespy_qr_field_gametype,
			_gamespy_qr_field_gamevariant,
			_gamespy_qr_field_numplayers,
			_gamespy_qr_field_numteams,
			_gamespy_qr_field_maxplayers,
			_gamespy_qr_field_gamemode,
			_gamespy_qr_field_teamplay,
			_gamespy_qr_field_fraglimit,
			_gamespy_qr_field_team_fraglimit,
			_gamespy_qr_field_timeelapsed,
			_gamespy_qr_field_timelimit,
			_gamespy_qr_field_roundtime,
			_gamespy_qr_field_roundelapsed,
			_gamespy_qr_field_password,
			_gamespy_qr_field_groupid,
			_gamespy_qr_field_player,
			_gamespy_qr_field_score,
			_gamespy_qr_field_skill,
			_gamespy_qr_field_ping,
			_gamespy_qr_field_team,
			_gamespy_qr_field_deaths,
			_gamespy_qr_field_pid,
			_gamespy_qr_field_team_t,
			_gamespy_qr_field_score_t,
			_gamespy_qr_field_reserved1,
			_gamespy_qr_field_reserved2,
			_gamespy_qr_field_reserved3,
			_gamespy_qr_field_reserved4,
			_gamespy_qr_field_reserved5,
			_gamespy_qr_field_reserved6,
			_gamespy_qr_field_reserved7,
			_gamespy_qr_field_reserved8,
			_gamespy_qr_field_reserved9,
			_gamespy_qr_field_reserved10,
			_gamespy_qr_field_reserved11,
			_gamespy_qr_field_reserved12,
			_gamespy_qr_field_reserved13,
			_gamespy_qr_field_reserved14,
			_gamespy_qr_field_reserved15,
			_gamespy_qr_field_reserved16,
			_gamespy_qr_field_reserved17,
			_gamespy_qr_field_reserved18,
			_gamespy_qr_field_reserved19,
			_gamespy_qr_field_reserved20,
			_gamespy_qr_field_reserved21,

			_gamespy_qr_field_dedicated,
			_gamespy_qr_field_player_flags,
			_gamespy_qr_field_game_flags,
			_gamespy_qr_field_game_classic,

			_gamespy_qr_field,
			_gamespy_qr_field_max_registered_keys = 254,
		}; BOOST_STATIC_ASSERT( _gamespy_qr_field <= _gamespy_qr_field_max_registered_keys );

		enum gamespy_connection_state : long_enum
		{
			GTI2AwaitingServerChallenge,
			GTI2AwaitingAcceptance,
			GTI2AwaitingClientChallenge,
			GTI2AwaitingClientResponse,
			GTI2AwaitingAcceptReject,
			GTI2Connected,
			GTI2Closing,
			GTI2Closed,
		};

		enum gamespy_connection_result : long_enum
		{
			GT2Success,
			GT2OutOfMemory,
			GT2Rejected,
			GT2NetworkError,
			GT2AddressError,
			GT2DuplicateAddress,
			GT2TimedOut,
			GT2NegotiationError,
			GT2InvalidConnection,
			GT2InvalidMessage,
			GT2SendFailed
		};

		enum game_build_number_index
		{
			_game_build_number_index_100,
			_game_build_number_index_107,
			_game_build_number_index_108,
			_game_build_number_index_109,

			k_max_game_build_number_index,
		};
	};

	namespace Networking
	{
		struct s_gamespy_buffer // GTI2Buffer, gt\gt2Main.h
		{
			byte* buffer;
			uint32 buffer_size;
			uint32 length;
		};

		struct s_gamespy_socket // GTI2Socket, gt\gt2Main.h
		{
			SOCKET socket;
			in_addr address;
			int16 port; PAD16;
			void* connections;
			void* closedConnections;
			BOOL close;
			BOOL error;
			void* connectAttemptCallback;
			void* socketErrorCallback;
			void* sendDumpCallback;
			void* receiveDumpCallback;
			void* unrecongizedMessageCallback;
			void* user_data; // 0x30, engine treats this as s_transport_endpoint*
			int32 incomingBufferSize;
			int32 outgoingBufferSize;
			int32 protocolOffset;
			UNKNOWN_TYPE(int32); // 0x40, I believe I saw some code treat this as a s_transport_endpoint* ...
			BOOL broadcastEnabled;
		}; BOOST_STATIC_ASSERT( sizeof(s_gamespy_socket) == 0x48 );
		struct s_gamespy_connection // GTI2Connection, gt\gt2Main.h
		{
			in_addr address;
			int16 port; PAD16;

			s_gamespy_socket* socket;
			Enums::gamespy_connection_state state;
			BOOL initiated;
			BOOL freeAtAcceptReject;
			Enums::gamespy_connection_result connectionResult;

			uint32 startTime, timeout;

			PAD(0, sizeof(int32)*8);

			s_gamespy_buffer incomingBuffer, outgoingBuffer;
			void* incomingBufferMessages;
			void* outgoingBufferMessages;

			uint16 serialNumber, expectedSerialNumber;

			char response[32];

			uint32 lastSend;
			uint32 challengeTime;

			PAD(1, sizeof(int32)*4);
			void* sendFilters;
			void* receiveFilters;
			PAD(2, sizeof(byte)*4);
			PAD(3, sizeof(byte)*28);
			PAD(4, sizeof(char)*32);
			PAD(5, sizeof(char)*32);
			PAD(6, sizeof(char)*32);
			PAD(7, sizeof(char)*20);
			PAD(8, sizeof(char)*16);
			UNKNOWN_TYPE(int32);
		}; BOOST_STATIC_ASSERT( sizeof(s_gamespy_connection) == 0x150 );

		struct s_gamespy_qr_data // (query/response) qr2_implementation_s, qr2\qr2.h
		{
			SOCKET heartbeat_socket;
			char game_name[64];
			char priv_key[64];
			char runtime_key[4];
			void* proc_server_key;
			void* proc_player_key;
			void* proc_team_key;
			void* proc_key_list;
			void* proc_player_team_count;
			void* proc_adderror;
			void* nn_callback;
			void* cm_callback;
			uint32 last_heartbeat_time;
			uint32 last_keepalive_time;
			long_enum listed_state;
			BOOL is_public;
			int32 query_port;
			int32 read_socket;
			int32 nat_negotiate;
			sockaddr_in heartbeat_addr;
			void* proc_process_cdkey; // void (PLATFORM_API*)(char* buffer, size_t buffer_size, sockaddr* src_addr)
			int32 client_msg_keys[10];
			int32 client_msg_key_index;
			void* user_data;
		}; BOOST_STATIC_ASSERT( sizeof(s_gamespy_qr_data) == 0x108 );


		struct s_gamespy_client_node // gsnode_s, gcdkey\gcdkeys.c
		{
			struct s_gamespy_client* client;
			s_gamespy_client_node* next, * prev;
		};
		struct s_gamespy_client // gsclient_s, gcdkey\gcdkeys.c
		{
			int32 id;					// 0x0
			char cd_hash[33];			// 0x4
			PAD24;
			uint32 skey;				// 0x28, (GetTickCount ^ rand) & 0x3FFF
			int32 ip;
			uint32 sent_req_time;		// 0x30, GetTickCount
			int32 number_of_retries;	// 0x34
			long_enum state;			// 0x38, 0 = sent request, 1 = ok, 2 = not ok, 3 = done;
			PAD32;						// 0x3C, void* proc_unk
			void* authenticate_proc;	// 0x40
			char* errmsg;				// 0x44
			// \auth\\pid\%d\ch\%s\resp\%s\ip\%d\skey\%dd
			char* req_str;				// 0x48, malloc'd char*
			uint32 req_str_length;		// 0x4C
		}; BOOST_STATIC_ASSERT( sizeof(s_gamespy_client) == 0x50 );

		struct s_gamespy_product // gsproduct_s, gcdkey\gcdkeys.c
		{
			int32 game_pid;
			s_gamespy_client_node clients;
		};

		// see "serverbrowsing\sb_internal.h" for these
		struct s_gamespy_server;
		struct s_gamespy_server_browser;

		struct s_gamespy_config
		{
			int32 game_pid;
			bool is_public_server;
			PAD24;
			int32 check_for_updates_status;
		};
		struct s_gamespy_globals
		{
			char game_name[8];
			char game_priv_key[8];
			PAD128; // just another (unused) copy of the priv key
			int32 server_port;

			struct {
				bool initialized;
				PAD24;
				void* mutex_reference;
				void* thread_reference;
				bool update_check_complete;
				PAD24;
				PAD32; // unused space...
				char proxy_server[256];
				bool proxy_server_initialized;
				PAD24;

				int32 file_id;
				char download_url[256];
				char version_name[64];
			}game_patch;

			struct {
				long_enum state;
				bool server_is_exiting;
				PAD24;
				s_gamespy_qr_data* obj;
				uint32 last_state_change_heartbeat_time;
				char temp_key_buffer[256];
			}qr2;
		}; BOOST_STATIC_ASSERT( sizeof(s_gamespy_globals) == 0x390 );
		struct s_gamespy_server_browser_globals
		{
			struct {
				s_gamespy_server* server;
				bool has_password;
				PAD24;
				wchar_t password[10]; // password provided by the user
			}selected_server;

			s_gamespy_server_browser* server_browser;
			bool initialized;
			PAD24;
			int32 total_number_of_players; // total number of players on all servers in the current game version
			UNKNOWN_TYPE(int32);
			UNKNOWN_TYPE(int32);
			UNKNOWN_TYPE(int32);
			UNKNOWN_TYPE(byte); // bool
			UNKNOWN_TYPE(byte); // bool
			PAD16;
			UNKNOWN_TYPE(int32);
			byte unknown_shit[10]; // this isn't a buffer but I didn't want to declare each unknown byte or bool...
			PAD16; PAD32;
			wchar_t unknown[256];
			UNKNOWN_TYPE(int32); // probably some kind of state value
			UNKNOWN_TYPE(uint32); // flags
			UNKNOWN_TYPE(int32);
			int32 server_browser_update_error;
			struct {
				void* thread_mutex;
				void* thread;
				PAD32;
			}query_server_list;
			UNKNOWN_TYPE(byte); // bool
			PAD24;
			UNKNOWN_TYPE(uint32); // time
			struct {
				s_gamespy_server** list;
				int32 count;
			}server_list;
			PAD32; // set to zero, but never used

			// NOTE: this structure seems to have one less DWORD in it in dedi builds. 
			// However, this structure shouldn't even be accessed in dedi builds so I'm not researching further!
		}; BOOST_STATIC_ASSERT( sizeof(s_gamespy_server_browser_globals) == 0x27C );


		extern cstring g_game_build_numbers[Enums::k_max_game_build_number_index];

		s_gamespy_socket* GsSocket();
		s_gamespy_socket* GsLoopbackSocket();
		s_gamespy_config* GsConfig();
		s_gamespy_globals* GsGlobals();
#if PLATFORM_IS_USER
		s_gamespy_server_browser_globals* GsServerBrowserGlobals();
#endif

		// If this is a server, returns all the machines connected to this machine on a specific pid
		s_gamespy_product* GsProducts(); // [4]

		s_gamespy_client* GsGetClient(int32 client_id);


		// If [and_game_build] is true, it will also change the GameState::GameBuildStrings
		bool ChangeAdvertisedGameVersion(cstring version_str, bool and_game_build);
	};
};