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

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_hs_thread_stack_size = 0x200,
		};

		enum playback_event_type {
			_playback_event_type,

			//NUMBER_OF_UNIT_ANIMATION_STATES = 7
			//NUMBER_OF_UNIT_AIMING_SPEEDS = 2
			//NUMBER_OF_UNIT_CONTROL_FLAGS = 15
			//MAXIMUM_WEAPONS_PER_UNIT = 4
			//NUMBER_OF_UNIT_GRENADE_TYPES = 2

			//1 animation_state_event_data		: byte
			//1 aiming_speed_event_data			: byte
			//2 control_flags_event_data		: short
			//2 weapon_index_event_data			: short
			//8 throttle_event_data				: long, long
			//2 vector_char_difference_data		: byte, byte
			//4 vector_short_difference_data	: short, short

			//4 animation_event_v1				: short, short
				//6 animation_state_set_event_v1	: byte
				//6 aiming_speed_set_event_v1		: byte
				//6 control_flags_set_event_v1		: short
				//6 weapon_index_set_event_v1		: short
				//C throttle_set_event_v1			: long, long
				//10 multi_vector_set_event_v1		: long, long, long
				//C angle_vector_set_event_v1		: long, long

			//unit_control_v1
				//0  byte
				//1  byte
				//2  word
				//4  word
				//-1 word
				//C  real_vector2d
				//1C real_vector3d
				//28 real_vector3d
				//34 real_vector3d
			//unit_control_v2
				//18 long 
			//unit_control_v3
				//6  word
			//unit_control_v4
				//8  word

			// limited by 'NUMBEROF(apply_funcs)' (0x5C)
			//_playback_animation_state_set,8
			//_playback_aiming_speed_set,C
			//_playback_control_flags_set,10
			//_playback_weapon_index_set,14
			//_playback_throttle_set,18

			//NUMBER_OF_CONTROL_VECTORS = 8
			//_playback_vector_char_difference_set,1C
			//_playback_vector_short_difference_set,3C


			//,0
			//,1
			//_playback_v1_animation_state_set,2
			//_playback_v1_aiming_speed_set,3
			//_playback_v1_control_flags_set,4
			//_playback_v1_weapon_index_set,5
			//_playback_v1_throttle_set,6
			//,7
			//,8
			//_playback_v1_facing_vector_set,9
			//_playback_v1_aiming_vector_set,A
			//_playback_v1_looking_vector_set,B
			//_playback_v1_facing_aiming_vector_set,C
			//_playback_v1_facing_aiming_looking_vector_set,F
			//_playback_v1_facing_angles_set,10
			//_playback_v1_facing_aiming_looking_angles_set,16
		};

		enum hs_thread_type : _enum
		{
			_hs_thread_type_script,
			_hs_thread_type_global_initialize, // script global initializer thunk
			_hs_thread_type_console_command,
			_hs_thread_type,
		};
	};

	namespace Scripting
	{
		//////////////////////////////////////////////////////////////////////////
		// recorded animations
		struct s_unit_control_data
		{
			byte animation_state;
			byte aiming_speed;
			word_flags control_flags;
			int16 weapon_index, grenade_index;
			int16 zoom_index;
			PAD16;
			real_vector3d throttle;
			real primary_trigger;
			real_vector3d facing_vector, aiming_vector, looking_vector;
		};

		struct s_recorded_animations_datum : Memory::s_datum_base_aligned
		{
			datum_index unit_index;
			uint16 time_left;
			word_flags flags;
			uint32 relative_ticks;
			void* event_stream;
			s_unit_control_data control_data;

			// unknown
			PAD32; PAD32; PAD32;
			PAD16; PAD16;
		}; BOOST_STATIC_ASSERT( sizeof(s_recorded_animations_datum) == 0x64 );
		typedef Memory::DataArray<s_recorded_animations_datum, 64> t_recorded_animations_data;
		t_recorded_animations_data*		RecordedAnimations();
		//////////////////////////////////////////////////////////////////////////

		struct hs_syntax_node : Memory::s_datum_base
		{
			union {
				int16 constant_type;
				int16 function_index;
			};
			int16 type;
			union {
				uint16 flags;
				int16 pointer_type;
			};
			datum_index next_expression;
			int32 pointer;
			union {
				void* address;

				struct {
					bool _bool;
					real _real;
					int16 _short;
					int32 _long;
					datum_index _datum;
				}Value;
			};
		};
		typedef Memory::DataArray<hs_syntax_node, 19001> t_hs_syntax_data;
		t_hs_syntax_data*				HSSyntax();


		struct s_object_list_header_datum : Memory::s_datum_base_aligned
		{
			int16 reference_count;	// how many references there are to this list
			int16 count;			// how many objects are in the list
			datum_index first;		// handle to the first list object reference
		};
		typedef Memory::DataArray<s_object_list_header_datum, 48> t_object_list_header_data;
		t_object_list_header_data*		ObjectListHeader();


		struct s_list_object_reference_datum : Memory::s_datum_base_aligned
		{
			datum_index object_index;	// handle to the object datum
			datum_index next_reference;	// handle to the next list object reference in the list
		};
		typedef Memory::DataArray<s_list_object_reference_datum, 128> t_list_object_reference_data;
		t_list_object_reference_data*	ListObjectReference();


		struct s_hs_thread_datum : Memory::s_datum_base
		{
			struct stack_frame
			{
				stack_frame* previous;
				datum_index source;
				void* result_reference;
				uint16 size;
				uint16 offset;
			};

			Enums::hs_thread_type type;
			word_flags flags;
			int16 type_index; // or NONE if type doesn't require an index
			uint32 sleep_until;
			UNKNOWN_TYPE(uint32); // game time related
			stack_frame* stack_data;
			TypeHolder result;

			byte stack_buffer[Enums::k_hs_thread_stack_size];
		};
		typedef Memory::DataArray<s_hs_thread_datum, 256> t_hs_thread_data;
		t_hs_thread_data*				HSThreads();


		struct s_hs_globals_datum : Memory::s_datum_base_aligned
		{
			union {
				void* address;

				struct {
					bool _bool;
					int16 _short;
					int32 _long;
					real _real;
					datum_index _datum;
					char* _string;
				}Value;
			};
		};
		typedef Memory::DataArray<s_hs_globals_datum, 1024> t_hs_globals_data;
		t_hs_globals_data*				HSGlobals();


		void Initialize();
		void Dispose();

		void PLATFORM_API Update();

		datum_index ObjectListNew();
	};
};