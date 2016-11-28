/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace UIWidgets
	{
		// TODO: move to ui_widget_structures.hpp
		struct s_ui_widget_instance
		{
			datum_index definition_index;		// 0x0
			cstring name;						// 0x4
			int16 local_player_index;			// 0x8
			point2d position;					// 0xA
			_enum widget_type;					// 0xE
			bool visible;						// 0x10
			bool render_regardless_of_controller_index; // 0x11
			UNKNOWN_TYPE(bool);					// 0x12
			bool pauses_game_time;				// 0x13
			bool deleted;						// 0x14
			UNKNOWN_TYPE(bool);					// 0x15
			UNKNOWN_TYPE(bool);					// 0x16
			PAD8;
			DWORD creator_process_start_time;	// 0x18
			DWORD ms_to_auto_close;				// 0x1C
			DWORD ms_auto_close_fade_time;		// 0x20
			real alpha_modifier;				// 0x24
			s_ui_widget_instance* previous;		// 0x28
			s_ui_widget_instance* next;			// 0x2C
			s_ui_widget_instance* parent;		// 0x30
			s_ui_widget_instance* first_child;	// 0x34
			s_ui_widget_instance* focused_child;// 0x38
			union {								// 0x3C
				struct {
					wchar_t* buffer;
					int16 string_index;			// 0x40
					PAD16;
				}textbox;

				struct {
					UNKNOWN_TYPE(int16);		// 0x3C seems to be a copy of element index
					UNKNOWN_TYPE(int16);		// 0x3E 

					int16 element_index;		// 0x40
					UNKNOWN_TYPE(int16);		// 0x42 goto state (negative is prev, positive is next)
					void* elements;				// 0x44
					uint16 element_count;		// 0x48
					PAD16;
				}list;
			};
			s_ui_widget_instance* extended_desc_widget; // 0x4C
			void* user_data;					// 0x50
			UNKNOWN_TYPE(int16);				// 0x54 list related
			PAD16;

			UNKNOWN_TYPE(int16);				// 0x58
			UNKNOWN_TYPE(int16);				// 0x5A
			UNKNOWN_TYPE(int16);				// 0x5C
			UNKNOWN_TYPE(int16);				// 0x5E
		}; BOOST_STATIC_ASSERT( sizeof(s_ui_widget_instance) == 0x60 );
	};

	namespace blam
	{
		UIWidgets::s_ui_widget_instance* PLATFORM_API ui_widget_load_by_name_or_tag(
			cstring name,
			datum_index ui_widget_definition_index,
			UIWidgets::s_ui_widget_instance* parent = nullptr,
			// don't set ctrl'er index unless you specify the last three params as well
			int16 controller_index = NONE, 
			datum_index topmost_widget_definition_index = datum_index::null,
			datum_index parent_widget_definition_index = datum_index::null,
			int16 child_index_from_parent = NONE);
	};
};