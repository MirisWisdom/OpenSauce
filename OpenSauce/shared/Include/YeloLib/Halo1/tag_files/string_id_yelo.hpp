/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	struct tag_field;
	struct tag_reference_definition;

	namespace Enums
	{
		enum {
			k_string_id_yelo_length = k_max_tag_name_length
		};
	};
	typedef char string_id_yelo_value[Yelo::Enums::k_string_id_yelo_length+1];

	typedef tag_reference_name_reference string_id_yelo_value_reference;

	// TODO: actually, we should just postprocess cache-bound data so the runtime value doesn't contain the tag_reference fields
	// just like in H2 where they don't include the debug data (eg, definition) pointers in things like tag_block, etc
	struct string_id_yelo
	{
		enum {
			k_signature = 'sidy',
			// how many bytes of string_id_yelo are debug data (ie, for tags builds) only
			k_debug_data_size = sizeof(tag_reference),
		};

#if TRUE//PLATFORM_IS_EDITOR // TODO: uncomment this when we finish 'runtime field size' support in the tag system
		tag signature;
		string_id_yelo_value_reference value;
		int32 value_length;
		datum_index tag_index;
#endif

		string_id id;

		static char* GetString(string_id id, __out string_id_yelo_value value);
		static cstring GetString(string_id id);

		static void FormatString(char* string);
#if PLATFORM_IS_EDITOR
		// Get the start of a string_id value that is pretending to be a tag name
		static char* GetStringStart(tag_reference_name_reference name);

		static tag_reference_definition* global_reference_definition;
#endif
	};
#if TRUE//PLATFORM_IS_EDITOR // TODO: uncomment this when we finish 'runtime field size' support in the tag system
	BOOST_STATIC_ASSERT( sizeof(string_id_yelo) == 0x14 );
	#define pad_string_id_yelo PAD_TYPE(tag_reference); PAD_TYPE(string_id);
#else
	BOOST_STATIC_ASSERT( sizeof(string_id_yelo) == 0x4 );
	#define pad_string_id_yelo PAD_TYPE(string_id);
#endif

	namespace TagGroups
	{
		struct s_string_id_yelo_definition
		{
			enum { k_group_tag = string_id_yelo::k_signature };

			tag_data documentation;
			PAD_TYPE(tag_data);
		};

#if PLATFORM_IS_EDITOR
		bool TagFieldIsStringId(const tag_field* field);
		// Is the field a tag_string field to be treated as a string_id?
		// TODO: when we encounter tag_strings which contain invalid string_id characters, we'll need to warn the user
		bool TagFieldIsOldStringId(const tag_field* field);
#endif
	};

	namespace _string_id
	{
		enum {
			k_index_bit_count = 19,
			k_id_set_bit_count = 6,

			k_index_bit_mask = MASK(k_index_bit_count),
			k_id_bit_mask = MASK(k_id_set_bit_count),

			k_index_bit_shift = 0,
			k_id_bit_shift = k_index_bit_count,

			k_maximum_set_values = (1 << k_index_bit_count) >> 7,
			k_maximum_set_storage = k_maximum_set_values * (Enums::k_string_id_length+1),

			_string_id_invalid = NONE,
		};

		enum {
			_set_global,
			// instead of appending the cache's string ids to the 'global' set, we set aside an entire set
			_set_cache,
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
	};
};