/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/interface/ui_widget_event_handler_functions.hpp>
#include <blamlib/Halo1/interface/ui_widget_game_data_input_functions.hpp>
#include <blamlib/Halo1/interface/ui_widget_text_search_and_replace_functions.hpp>

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum ui_widget_type : _enum
		{
			_ui_widget_type_container,
			_ui_widget_type_text_box,
			_ui_widget_type_spinner_list,
			_ui_widget_type_column_list,
			//////////////////////////////////////////////////////////////////////////
			// the following are not implemented
			_ui_widget_type_game_model,
			_ui_widget_type_movie,
			_ui_widget_type_custom,

			k_number_of_ui_widget_types,
		};
	};

	namespace TagGroups
	{
		struct ui_conditional_widget_reference
		{
			struct __flags
			{
				TAG_FLAG(load_if_event_handler_function_fails);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(long_flags) );

			TAG_FIELD(tag_reference, widget_tag, 'DeLa');
			TAG_FIELD(tag_string, name);
			TAG_FIELD(__flags, flags);
			TAG_FIELD(int16, custom_controller_index);
			
			PAD16;
			TAG_PAD(int32, 6);
		}; BOOST_STATIC_ASSERT( sizeof(ui_conditional_widget_reference) == 0x50 ); // max count: 32

		struct ui_child_widget_reference
		{
			struct __flags
			{
				TAG_FLAG(use_custom_controller_index);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(long_flags) );

			TAG_FIELD(tag_reference, widget_tag, 'DeLa');
			TAG_FIELD(tag_string, name);
			TAG_FIELD(__flags, flags);
			TAG_FIELD(int16, custom_controller_index);
			TAG_FIELD(int16, vertical_offset);
			TAG_FIELD(int16, horizontal_offset);
			
			PAD16;
			TAG_PAD(int32, 5);
		}; BOOST_STATIC_ASSERT( sizeof(ui_child_widget_reference) == 0x50 ); // max count: 32

		struct ui_widget_definition
		{
			enum { k_group_tag = 'DeLa' };

			struct __flags
			{
				TAG_FLAG(pass_unhandled_events_to_focused_child);
				TAG_FLAG(pause_game_time);
				TAG_FLAG(flash_background_bitmap);
				TAG_FLAG(dpad_up_down_tabs_thru_children);
				TAG_FLAG(dpad_left_right_tabs_thru_children);
				TAG_FLAG(dpad_up_down_tabs_thru_list_items);
				TAG_FLAG(dpad_left_right_tabs_thru_list_items);
				TAG_FLAG(dont_focus_a_specific_child_widget);
				TAG_FLAG(pass_unhandled_events_to_all_children);
				TAG_FLAG(render_regardless_of_controller_index);
				TAG_FLAG(pass_handled_events_to_all_children);
				TAG_FLAG(return_to_main_menu_if_no_history);
				TAG_FLAG(always_use_tag_controller_index);
				TAG_FLAG(always_use_nifty_render_fx);
				TAG_FLAG(don_t_push_history);
				TAG_FLAG(force_handle_mouse);
			}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(long_flags) );

			TAG_ENUM(widget_type, ui_widget_type);
			TAG_ENUM(controller_index, ui_controller_index);
			TAG_FIELD(tag_string, name);
			TAG_FIELD(rectangle2d, bounds);
			TAG_FIELD(__flags, flags);
			TAG_FIELD(int32, ms_to_auto_close, "<=0 to never auto-close");
			TAG_FIELD(int32, ms_auto_close_fade_time, "<= 0 for immediate close");
			TAG_FIELD(tag_reference, background_bitmap, "bitm");

			////////////////////////////////////////////////////////////////
			// game data input fxns
			// These functions use current game data to modify the appearance of
			// the widget. These functions are called every time the widget is
			// rendered.
			TAG_TBLOCK(game_data_inputs, game_data_input_reference);

			////////////////////////////////////////////////////////////////
			// event handlers
			// These allow actions to be tied to certain ui events.
			// The event handler is run every time the widget receives the specified event.
			// By default, the 'back' and 'B' buttons will take you to the previous screen.
			TAG_TBLOCK(event_handlers, ui_widget_event_handler_reference);

			////////////////////////////////////////////////////////////////
			// search-and-replace
			// These are used to run a search-and-replace on the specified
			// word in the text-box text, replacing all occurrences of the word
			// with the output of the replace-function. These are invoked each
			// time the text box is rendered (after any game data input functions
			// have been run). The searching is case-sensitive.
			TAG_TBLOCK(search_and_replace_functions, ui_widget_search_and_replace_reference);
			TAG_PAD(int32, 32);

			////////////////////////////////////////////////////////////////
			// text box
			// parameters specific to text box widgets
			// NOTE: the string list tag can also be used for lists whose items come from a string list tag
			struct {
				struct __flags
				{
					TAG_FLAG(editable);
					TAG_FLAG(password);
					TAG_FLAG(flashing);
					TAG_FLAG(dont_do_that_weird_focus_test);
				}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(long_flags) );

				TAG_FIELD(tag_reference, label_unicode_strings_list, 'ustr');
				TAG_FIELD(tag_reference, font, 'font');
				TAG_FIELD(real_argb_color, color);
				TAG_ENUM(justification);
				TAG_FIELD(__flags, flags);
				TAG_PAD(int32, 3);

				////////////////////////////////////////////////////////////////
				// more text box parameters
				TAG_FIELD(int16, string_list_index, "default is 0");
				TAG_FIELD(int16, horiz_offset, "offsets text position in its bounding area");
				TAG_FIELD(int16, vert_offset, "offsets the text position in its bounding area");
				TAG_PAD(int16, 13);

				PAD16; // because they didn't pad after justification
			}text_box;

			////////////////////////////////////////////////////////////////
			// list items
			// These options affect list items for both spinner and column lists
			// * child widgets are used to define the visible list items
			// * for lists with code-generated list items, the child widgets are used
			//   as templated for visible list item placement
			// IMPORTANT: for list widgets, the ONLY thing you can have as child widgets are the list item widgets!
			struct {
				struct __flags
				{
					TAG_FLAG(list_items_generated_in_code);
					TAG_FLAG(list_items_from_string_list_tag);
					TAG_FLAG(list_items_only_one_tooltip);
					TAG_FLAG(list_single_preview_no_scroll);
				}; BOOST_STATIC_ASSERT( sizeof(__flags) == sizeof(long_flags) );

				TAG_FIELD(__flags, flags);

				////////////////////////////////////////////////////////////////
				// spinner list
				// parameters specific to spinner list widgets
				// child widgets are the list items
				struct {
					TAG_FIELD(tag_reference, list_header_bitmap, 'bitm');
					TAG_FIELD(tag_reference, list_footer_bitmap, 'bitm');
					TAG_FIELD(rectangle2d, header_bounds);
					TAG_FIELD(rectangle2d, footer_bounds);
					TAG_PAD(int32, 8);
				}spinner;

				////////////////////////////////////////////////////////////////
				// column list
				// parameters specific to column list widgets
				// child widgets are the list items
				struct {
					TAG_FIELD(tag_reference, extended_description_widget, 'DeLa');
					TAG_PAD(int32, 8);
					TAG_PAD(int32, 64);
				}column;
			}list_items;

			////////////////////////////////////////////////////////////////
			// conditional widgets
			// use this to attach widgets that are loaded only
			// if some internal criteria is met while processing a widget event
			TAG_TBLOCK(conditional_widgets, ui_conditional_widget_reference);
			TAG_PAD(int32, 32);
			TAG_PAD(int32, 32);

			////////////////////////////////////////////////////////////////
			// child widgets
			// use this to attach widgets that are loaded as 'children'
			// of this widget (children are always loaded as part of the parent widget)
			TAG_TBLOCK(child_widgets, ui_child_widget_reference);
		}; BOOST_STATIC_ASSERT( sizeof(ui_widget_definition) == 0x3EC );
	};

	namespace blam
	{
		void PLATFORM_API ui_load_tags_for_scenario(datum_index scenario_index);
	};
};