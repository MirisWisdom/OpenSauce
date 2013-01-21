/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/units/unit_structures.hpp>

namespace Yelo
{
	namespace Objects
	{
		const size_t s_unit_data::k_offset_zoom_level = FIELD_OFFSET(s_unit_datum, unit.zoom_level);
		const size_t s_unit_data::k_offset_desired_zoom_level = FIELD_OFFSET(s_unit_datum, unit.desired_zoom_level);

		const size_t s_unit_data::k_offset_zoom_level_yelo = FIELD_OFFSET(s_unit_datum, unit.zoom_level_yelo);
		const size_t s_unit_data::k_offset_desired_zoom_level_yelo = FIELD_OFFSET(s_unit_datum, unit.desired_zoom_level_yelo);
	};
};