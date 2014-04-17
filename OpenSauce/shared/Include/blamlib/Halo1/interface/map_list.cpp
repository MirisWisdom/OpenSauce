/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/interface/map_list.hpp>

namespace Yelo
{
	namespace Interface
	{
	};

	namespace blam
	{
#if !PLATFORM_IS_EDITOR
		cstring map_list_ignored_map_names[10+1+3] = {
			"a10", "a30", "a50",
			"b30", "b40",
			"c10", "c20", "c40",
			"d20", "d40",
			"ui",
			"bitmaps",
			"sounds",
			"loc",
		};

		const Interface::s_map_list_map_info k_original_multiplayer_maps[19] = {
			{0,  "beavercreek", true},
			{1,  "sidewinder", true},
			{2,  "damnation", true},
			{3,  "ratrace", true},
			{4,  "prisoner", true},
			{5,  "hangemhigh", true},
			{6,  "chillout", true},
			{7,  "carousel", true},
			{8,  "boardingaction", true},
			{9,  "bloodgulch", true},
			{10, "wizard", true},
			{11, "putput", true},
			{12, "longest", true},

			{13, "icefields"},
			{14, "deathisland"},
			{15, "dangercanyon"},
			{16, "infinity"},
			{17, "timberland"},
			{18, "gephyrophobia"},
		};
#endif
	};
};