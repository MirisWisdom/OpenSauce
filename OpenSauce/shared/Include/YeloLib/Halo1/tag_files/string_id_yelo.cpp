/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/tag_files/string_id_yelo.hpp>

#if PLATFORM_IS_EDITOR || defined(API_DEBUG)
	struct string_id_yelo_kv_pair
	{
		Yelo::string_id key;
		Yelo::cstring value;
	};
	#define __STRING_ID(value)				\
		{Yelo::_string_id::__STRING_ID_GENERATE_VALUE_NAME( value ), #value},

	static const string_id_yelo_kv_pair k_string_id_kv_pairs[] = {
		{Yelo::_string_id::_string_id_empty_string, ""},
		#include <YeloLib/Halo1/tag_files/string_ids.global.inl>
		// the 'map' set is defined in the cache file
		#include <YeloLib/Halo1/tag_files/string_ids.gui.inl>
		#include <YeloLib/Halo1/tag_files/string_ids.gui_alert.inl>
		#include <YeloLib/Halo1/tag_files/string_ids.gui_dialog.inl>
		#include <YeloLib/Halo1/tag_files/string_ids.properties.inl>
		#include <YeloLib/Halo1/tag_files/string_ids.components.inl>
		#include <YeloLib/Halo1/tag_files/string_ids.game_engine.inl>
		#include <YeloLib/Halo1/tag_files/string_ids.incident.inl>
		#include <YeloLib/Halo1/tag_files/string_ids.os_setting.inl>

		{Yelo::_string_id::_string_id_invalid, "NULL"}
	};

	#undef __STRING_ID
#endif

namespace Yelo
{
};