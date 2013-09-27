/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/tag_files/string_id_yelo.hpp>

#include <blamlib/Halo1/tag_files/tag_groups_structures.hpp>

#if PLATFORM_IS_EDITOR || defined(API_DEBUG)
	struct string_id_yelo_kv_pair
	{
		Yelo::string_id key;
		Yelo::cstring value;
	};
	#define STRING_ID_YELO_KV_PAIRS_ARRAY_NAME(set_name)	BOOST_JOIN(k_string_id_kv_pairs_,set_name)
	#define STRING_ID_YELO_KV_PAIRS_ARRAY_STATIC_ASSERT_LENGTH(set_name)	\
		BOOST_STATIC_ASSERT( BOOST_JOIN(BOOST_JOIN(Yelo::_string_id::k_number_of_,set_name), _strings) == NUMBEROF( STRING_ID_YELO_KV_PAIRS_ARRAY_NAME(set_name) )-1 ); 
		// minus 1 to account for the terminator pair
		// we use a terminator so empty sets don't generate compiler errors

	#define __STRING_ID(value)									\
		{Yelo::_string_id::__STRING_ID_GENERATE_VALUE_NAME( value ), #value},

	#define __STRING_ID_SET_BEGIN()								\
		static const string_id_yelo_kv_pair STRING_ID_YELO_KV_PAIRS_ARRAY_NAME(__STRING_ID_GENERATE_SET_NAME)[] = {
	#define __STRING_ID_SET_END()								\
			{Yelo::_string_id::_string_id_invalid, "NULL"}		\
		}; STRING_ID_YELO_KV_PAIRS_ARRAY_STATIC_ASSERT_LENGTH( __STRING_ID_GENERATE_SET_NAME );
	#define __STRING_ID_EMPTY_STRING_HACK()	{Yelo::_string_id::_string_id_empty_string, ""},

	#include <YeloLib/Halo1/tag_files/string_ids.global.inl>
	#include <YeloLib/Halo1/tag_files/string_ids.gui.inl>
	#include <YeloLib/Halo1/tag_files/string_ids.gui_alert.inl>
	#include <YeloLib/Halo1/tag_files/string_ids.gui_dialog.inl>
	#include <YeloLib/Halo1/tag_files/string_ids.properties.inl>
	#include <YeloLib/Halo1/tag_files/string_ids.components.inl>
	#include <YeloLib/Halo1/tag_files/string_ids.game_engine.inl>
	#include <YeloLib/Halo1/tag_files/string_ids.incident.inl>
	#include <YeloLib/Halo1/tag_files/string_ids.os_setting.inl>
	static const string_id_yelo_kv_pair* const k_string_id_kv_pairs[] = {
		STRING_ID_YELO_KV_PAIRS_ARRAY_NAME(global),
		nullptr, // the 'cache' set is defined in the cache file
		STRING_ID_YELO_KV_PAIRS_ARRAY_NAME(gui),
		STRING_ID_YELO_KV_PAIRS_ARRAY_NAME(gui_alert),
		STRING_ID_YELO_KV_PAIRS_ARRAY_NAME(gui_dialog),
		STRING_ID_YELO_KV_PAIRS_ARRAY_NAME(properties),
		STRING_ID_YELO_KV_PAIRS_ARRAY_NAME(components),
		STRING_ID_YELO_KV_PAIRS_ARRAY_NAME(game_engine),
		STRING_ID_YELO_KV_PAIRS_ARRAY_NAME(incident),
		STRING_ID_YELO_KV_PAIRS_ARRAY_NAME(os_setting),

		nullptr, nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr, nullptr,
	}; BOOST_STATIC_ASSERT( NUMBEROF(k_string_id_kv_pairs) == Yelo::_string_id::k_number_of_sets );

	#undef __STRING_ID_GENERATE_SET_NAME

	#undef STRING_ID_YELO_KV_PAIRS_ARRAY_NAME
	#undef STRING_ID_YELO_KV_PAIRS_ARRAY_STATIC_ASSERT_LENGTH
	#undef __STRING_ID
	#undef __STRING_ID_SET_BEGIN
	#undef __STRING_ID_SET_END
	#undef __STRING_ID_EMPTY_STRING_HACK
#endif

namespace Yelo
{
	void string_id_yelo::format_string(char* string)
	{
		for(char* c = string; *c != '\0'; ++c)
		{
			if(*c >= 'A' || *c <= 'Z')
				*c = tolower(*c);

			if(*c == ' ' || *c == '-')
				*c = '_';
		}
	}

#if PLATFORM_IS_EDITOR
	char* string_id_yelo::get_string_start(tag_reference_name_reference name)
	{
		// we only consider the characters after the last directory separator 
		char* last_slash = strrchr(name, '\\');

		return last_slash != nullptr ? last_slash+1 : name;
	}
#endif

	char* string_id_yelo::get_string(string_id id, __out string_id_yelo_value value)
	{
		assert(false); // TODO
		return nullptr;
	}

	cstring string_id_yelo::get_string(string_id id)
	{
		assert(false); // TODO
		return nullptr;
	}

	namespace TagGroups
	{
		bool TagFieldIsStringId(const tag_field* field)
		{
			// NOTE: feign string_id fields should have the _tag_reference_non_resolving_bit set
			// in their tag_reference_definition
			return	field->type == Enums::_field_tag_reference && 
					field->Definition<tag_reference_definition>()->group_tag == s_string_id_yelo_definition::k_group_tag;
		}

		bool TagFieldIsOldStringId(const tag_field* field)
		{
			return	field->type == Enums::_field_string &&
					field->DefinitionCast<tag>() == s_string_id_yelo_definition::k_group_tag;
		}
	}
};