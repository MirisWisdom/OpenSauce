/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/Halo1/tag_files/string_id_yelo.hpp>

#define STRING_ID(set, value)	\
	_string_id::_##set##_string_##value

namespace Yelo
{
	namespace _string_id
	{
//////////////////////////////////////////////////////////////////////////
// generator macros
// Returns the current generated set's (enum) member name by value name
#define __STRING_ID_GENERATE_VALUE_NAME(value)			\
	BOOST_JOIN(											\
		BOOST_JOIN(_,__STRING_ID_GENERATE_SET_NAME) ,	\
		BOOST_JOIN(_string_, value )					\
		)

// Returns the name of the current generated set's base counter
#define __STRING_ID_GENERATE_COUNTER_NAME		BOOST_JOIN(__, BOOST_JOIN(__STRING_ID_GENERATE_SET_NAME, __ENUM_COUNTER__ ))
// Returns the next counter value, ie index, for the generated set
#define __STRING_ID_GENERATE_COUNTER_NEXT		(__COUNTER__ - __STRING_ID_GENERATE_COUNTER_NAME - 1)
// Generate a string_id by value name
#define __STRING_ID_GENERATE(value)																			\
	(																										\
		(BOOST_JOIN(_set_,__STRING_ID_GENERATE_SET_NAME)&	k_id_bit_mask)		<< k_id_bit_shift		|	\
		(__STRING_ID_GENERATE_COUNTER_NEXT				&	k_index_bit_mask)	<< k_index_bit_shift		\
	)
// Generate a string_id definition by value name
#define __STRING_ID(value)								\
	__STRING_ID_GENERATE_VALUE_NAME( value ) = __STRING_ID_GENERATE( value ),
// Start a string_id set definition, declaring/defining any needed boilerplate code
#define __STRING_ID_SET_BEGIN()							\
	enum { __STRING_ID_GENERATE_COUNTER_NAME = __COUNTER__,
// End a string_id set definition, declaring/defining any needed boilerplate code
#define __STRING_ID_SET_END()																								\
		BOOST_JOIN(BOOST_JOIN(k_number_of_,__STRING_ID_GENERATE_SET_NAME),_strings) = __STRING_ID_GENERATE_COUNTER_NEXT,	\
	};
// _string_id_empty_string definition hack for the 'global' set
#define __STRING_ID_EMPTY_STRING_HACK()	_string_id_empty_string = __STRING_ID_GENERATE_COUNTER_NEXT,
//////////////////////////////////////////////////////////////////////////


#include <YeloLib/Halo1/tag_files/string_ids.global.inl>
		BOOST_STATIC_ASSERT( _string_id_empty_string == 0 );
// NO CACHE STRINGS DEFINED IN CODE!
#include <YeloLib/Halo1/tag_files/string_ids.gui.inl>
#include <YeloLib/Halo1/tag_files/string_ids.gui_alert.inl>
#include <YeloLib/Halo1/tag_files/string_ids.gui_dialog.inl>
#include <YeloLib/Halo1/tag_files/string_ids.properties.inl>
#include <YeloLib/Halo1/tag_files/string_ids.components.inl>
#include <YeloLib/Halo1/tag_files/string_ids.game_engine.inl>
#include <YeloLib/Halo1/tag_files/string_ids.incident.inl>
#include <YeloLib/Halo1/tag_files/string_ids.os_setting.inl>

#undef __STRING_ID_GENERATE_SET_NAME

#undef __STRING_ID_GENERATE_COUNTER_NAME
#undef __STRING_ID_GENERATE_COUNTER_NEXT
#undef __STRING_ID_GENERATE
#undef __STRING_ID

#undef __STRING_ID_SET_BEGIN
#undef __STRING_ID_SET_END
#undef __STRING_ID_EMPTY_STRING_HACK
	};
};