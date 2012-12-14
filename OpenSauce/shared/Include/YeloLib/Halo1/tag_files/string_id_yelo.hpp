/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/tag_files/tag_groups_base.hpp>

#define STRING_ID(set, value)	\
	_string_id::_##set##_string_##value

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_string_id_yelo_length = k_max_tag_name_length
		};
	};
	typedef char string_id_yelo_value[Yelo::Enums::k_string_id_yelo_length+1];

	typedef tag_reference_name_reference string_id_yelo_value_reference;

	struct string_id_yelo
	{
		enum { k_signature = 'sidy' };

		tag signature;
		string_id_yelo_value_reference value;
		int32 value_length;
		datum_index tag_index;

		string_id id;

		static void format_string(cstring string);
		static char* get_string(string_id id, __out string_id_yelo_value value);
		static cstring get_string(string_id id);
	}; BOOST_STATIC_ASSERT( sizeof(string_id_yelo) == 0x14 );
#define pad_string_id_yelo PAD_TYPE(tag_reference); PAD_TYPE(string_id);

	namespace TagGroups
	{
		struct s_string_id_yelo_definition
		{
			enum { k_group_tag = string_id_yelo::k_signature };

			tag_data documentation;
			PAD_TYPE(tag_data);
		};
	};

	namespace _string_id
	{
		enum {
			k_index_bit_count = 19,
			k_id_set_bit_count = 6,

			k_index_bit_mask = (1<<k_index_bit_count) - 1,
			k_id_bit_mask = (1<<k_id_set_bit_count) - 1,

			k_index_bit_shift = 0,
			k_id_bit_shift = k_index_bit_count,

			k_maximum_set_values = (1 << k_index_bit_count) >> 7,
			k_maximum_set_storage = k_maximum_set_values * (Enums::k_string_id_length+1),

			_string_id_invalid = NONE,
		};

		enum {
			_set_global,
			// instead of appending the map's string ids to the 'global' set, we set aside an entire set
			_set_map,
			_set_gui,
			_set_gui_alert,
			_set_gui_dialog,
			_set_properties,
			_set_components,
			_set_game_engine,
			_set_incident,
			_set_os_settings,
			_set_unused10,
			_set_unused11,
			_set_unused12,
			_set_unused13,
			_set_unused14,
			_set_unused15,
			_set_unused16,
			_set_unused17,
			_set_unused18,
			_set_unused19,

			k_number_of_sets,

			k_last_valid_set = _set_unused10 - 1,
		};
//////////////////////////////////////////////////////////////////////////
// generator macros
#define __STRING_ID_GENERATE_VALUE_NAME(value)			\
	BOOST_JOIN(											\
		BOOST_JOIN(_,__STRING_ID_GENERATE_SET_NAME) ,	\
		BOOST_JOIN(_string_, value )					\
		)
#define __STRING_ID_GENERATE_COUNTER_NEXT		(__COUNTER__ - __ENUM_COUNTER__ - 1)
#define __STRING_ID_GENERATE(value)													\
	(																							\
		(BOOST_JOIN(_set_,__STRING_ID_GENERATE_SET_NAME)&	k_id_bit_mask)		<< k_id_bit_shift		|	\
		(__STRING_ID_GENERATE_COUNTER_NEXT				&	k_index_bit_mask)	<< k_index_bit_shift		\
	)
#define __STRING_ID(value)								\
	__STRING_ID_GENERATE_VALUE_NAME( value ) = __STRING_ID_GENERATE( value ),
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// global
		enum {	__ENUM_COUNTER__global = __COUNTER__ };
#undef		__ENUM_COUNTER__
#define		__ENUM_COUNTER__ __ENUM_COUNTER__global
		enum {
			_string_id_empty_string = __STRING_ID_GENERATE_COUNTER_NEXT,
#include <YeloLib/Halo1/tag_files/string_ids.global.inl>
			k_number_of_global_strings = __STRING_ID_GENERATE_COUNTER_NEXT,
		};
		BOOST_STATIC_ASSERT( _string_id_empty_string == 0 );


//////////////////////////////////////////////////////////////////////////
// NO MAP STRINGS DEFINED IN CODE!


//////////////////////////////////////////////////////////////////////////
// gui
		enum {	__ENUM_COUNTER__gui = __COUNTER__ };
#undef		__ENUM_COUNTER__
#define		__ENUM_COUNTER__ __ENUM_COUNTER__gui
		enum {
#include	<YeloLib/Halo1/tag_files/string_ids.gui.inl>
			k_number_of_gui_strings = __STRING_ID_GENERATE_COUNTER_NEXT,
		};


//////////////////////////////////////////////////////////////////////////
// gui_alert
		enum {	__ENUM_COUNTER__gui_alert = __COUNTER__ };
#undef		__ENUM_COUNTER__
#define		__ENUM_COUNTER__ __ENUM_COUNTER__gui_alert
		enum {
#include	<YeloLib/Halo1/tag_files/string_ids.gui_alert.inl>
			k_number_of_gui_alert_strings = __STRING_ID_GENERATE_COUNTER_NEXT,
		};


//////////////////////////////////////////////////////////////////////////
// gui_dialog
		enum {	__ENUM_COUNTER__gui_dialog = __COUNTER__ };
#undef		__ENUM_COUNTER__
#define		__ENUM_COUNTER__ __ENUM_COUNTER__gui_dialog
		enum {
#include	<YeloLib/Halo1/tag_files/string_ids.gui_dialog.inl>
			k_number_of_gui_dialog_strings = __STRING_ID_GENERATE_COUNTER_NEXT,
		};


//////////////////////////////////////////////////////////////////////////
// properties
		enum {	__ENUM_COUNTER__properties = __COUNTER__ };
#undef		__ENUM_COUNTER__
#define		__ENUM_COUNTER__ __ENUM_COUNTER__properties
		enum {
#include	<YeloLib/Halo1/tag_files/string_ids.properties.inl>
			k_number_of_properties_strings = __STRING_ID_GENERATE_COUNTER_NEXT,
		};


//////////////////////////////////////////////////////////////////////////
// components
		enum {	__ENUM_COUNTER__components = __COUNTER__ };
#undef		__ENUM_COUNTER__
#define		__ENUM_COUNTER__ __ENUM_COUNTER__components
		enum {
#include	<YeloLib/Halo1/tag_files/string_ids.components.inl>
			k_number_of_components_strings = __STRING_ID_GENERATE_COUNTER_NEXT,
		};


//////////////////////////////////////////////////////////////////////////
// game_engine
		enum {	__ENUM_COUNTER__game_engine = __COUNTER__ };
#undef		__ENUM_COUNTER__
#define		__ENUM_COUNTER__ __ENUM_COUNTER__game_engine
		enum {
#include	<YeloLib/Halo1/tag_files/string_ids.game_engine.inl>
			k_number_of_game_engine_strings = __STRING_ID_GENERATE_COUNTER_NEXT,
		};


//////////////////////////////////////////////////////////////////////////
// incident
		enum {	__ENUM_COUNTER__incident = __COUNTER__ };
#undef		__ENUM_COUNTER__
#define		__ENUM_COUNTER__ __ENUM_COUNTER__incident
		enum {
#include	<YeloLib/Halo1/tag_files/string_ids.incident.inl>
			k_number_of_incident_strings = __STRING_ID_GENERATE_COUNTER_NEXT,
		};


//////////////////////////////////////////////////////////////////////////
// os_setting
		enum {	__ENUM_COUNTER__os_setting = __COUNTER__ };
#undef		__ENUM_COUNTER__
#define		__ENUM_COUNTER__ __ENUM_COUNTER__os_setting
		enum {
#include	<YeloLib/Halo1/tag_files/string_ids.os_setting.inl>
			k_number_of_os_setting_strings = __STRING_ID_GENERATE_COUNTER_NEXT,
		};

#undef __STRING_ID_GENERATE_SET_NAME
#undef __STRING_ID_GENERATE_COUNTER_NEXT
#undef __STRING_ID_GENERATE
#undef __STRING_ID
	};
};