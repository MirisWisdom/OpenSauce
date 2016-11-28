/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace AI
	{
		struct s_ai_spatial_effect
		{
			int16 type;
			PAD16; // unknown field
			real_vector3d location;
			uint32 tick;
		}; BOOST_STATIC_ASSERT(sizeof(s_ai_spatial_effect) == 0x14);

		struct s_ai_vehicle_enterable_datum
		{
			datum_index vehicle_index;
			real distance;
			int16 teams_bitvector;
			int16 actor_types_bitvector;
			int16 actors_count;
			PAD16;
			datum_index actor_indices[6];
		}; BOOST_STATIC_ASSERT(sizeof(s_ai_vehicle_enterable_datum) == 0x28);

		struct s_ai_globals_data
		{
			bool ai_active;
			bool initialized;
			PAD8; // unknown field
			PAD8; // unknown field
			PAD16; // unknown field
			PAD16; // unknown field
			datum_index first_encounterless_actor_index;
			PAD32; // unknown field
			bool dialogue_triggers_enabled;
			PAD24;
			PAD(1, 8); // unknown field
			PAD(2, 8); // unknown field
			PAD(3, 8); // unknown field
			PAD16; // unknown field
			PAD16; // unknown field
			PAD(4, 256); // unknown field
			PAD16; // unknown field
			PAD16; // unknown field
			s_ai_spatial_effect spatial_effects[32];
			bool grenades_enabled;
			PAD8;
			int16 vehicles_enterable_count;
			s_ai_vehicle_enterable_datum vehicles_enterable[32];
			int16 pending_mounted_weapons_count;
			PAD16;
			datum_index pending_mounted_weapon_indices[8];
		}; BOOST_STATIC_ASSERT(sizeof(s_ai_globals_data) == 0x8DC);
	};
};