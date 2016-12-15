/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/interface/terminal.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum hud_chat_type : long_enum
		{
			_hud_chat_type_none = CAST(long_enum,NONE),

			_hud_chat_type_all = 0,
			_hud_chat_type_team,
			_hud_chat_type_vehicle,
			_hud_chat_type_server,
			_hud_chat_type_info_msg, // used for messages pulled from Strings.dll. Convert 'hud_chat_network_data.message' to a long using 'wtol' to get the resource ID
		};
	};

	namespace GameUI
	{
		struct s_hud_chat_globals
		{
			bool active; PAD24;
			Enums::hud_chat_type chat_type;

			Console::s_terminal_state terminal_state;

			uint32 line_timers[8];
		}; BOOST_STATIC_ASSERT( sizeof(s_hud_chat_globals) == 0x1E8 );

		s_hud_chat_globals*		HudChatGlobals();
		int32 HudChatLineCount();
	};

	namespace blam
	{
		void PLATFORM_API hud_chat_to_network(int32 player_number, long_enum chat_type, wcstring text);

		void PLATFORM_API hud_chat_display_message(wcstring message);
	};
};