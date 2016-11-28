/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace blam
	{
		void PLATFORM_API hs_object_orient(datum_index object_index, int32 cutscene_flag_index, bool teleport, bool set_facing);


		// Spawn an effect tag on the specified object's marker
		void PLATFORM_API hs_effect_new_from_object_marker(datum_index effect_definition_index, datum_index object_index, cstring marker_name = "");
	};
};