/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		struct scenario_object_name
		{
			TAG_FIELD(tag_string, name);
			_enum object_type;
			int16 scenario_object_index;
		}; BOOST_STATIC_ASSERT( sizeof(scenario_object_name) == 0x24 );

		struct scenario_device_group
		{
			tag_string name;
			real initial_value;
			long_flags flags;
			TAG_PAD(int32, 3);
		}; BOOST_STATIC_ASSERT( sizeof(scenario_device_group) == 0x34 );

		struct scenario_object_palette_entry
		{
			tag_reference object_definition;
			PAD_TYPE(tag_reference);
			PAD_TYPE(tag_reference);
		}; BOOST_STATIC_ASSERT( sizeof(scenario_object_palette_entry) == 0x30 );

		//////////////////////////////////////////////////////////////////////////
		// object
		struct s_scenario_object_header
		{
			int16 palette_index;
			int16 name_index;
		}; BOOST_STATIC_ASSERT( sizeof(s_scenario_object_header) == 0x4 );
		struct s_scenario_object_datum
		{
			word_flags placement_flags;
			int16 desired_permutation;
			real_point3d position;
			real_euler_angles3d rotation;
		}; BOOST_STATIC_ASSERT( sizeof(s_scenario_object_datum) == 0x1C );
		struct s_scenario_object_permutation
		{
			PAD128;	PAD64;	PAD64;
		}; BOOST_STATIC_ASSERT( sizeof(s_scenario_object_permutation) == 0x20 );

		struct s_scenario_scenery_datum
		{
			PAD64;
			s_scenario_object_permutation permutation;
		};

		struct s_scenario_sound_scenery_datum
		{
			PAD64;
		};

		//////////////////////////////////////////////////////////////////////////
		// units
		struct s_scenario_unit_datum
		{
			PAD64;
			s_scenario_object_permutation permutation;
			real body_vitality;
			long_flags flags;
			PAD64;
		};
		struct s_scenario_biped_datum
		{
			TAG_PAD(int32, 8);
		};
		struct s_scenario_vehicle_datum
		{
			TAG_PAD(int32, 8);
		};

		//////////////////////////////////////////////////////////////////////////
		// items
		struct s_scenario_weapon_datum
		{
			PAD64;
			s_scenario_object_permutation permutation;

			int16 rounds_left;
			int16 rounds_total;
			word_flags flags;
			PAD16;
			TAG_PAD(int32, 3);
		};
		struct s_scenario_equipment_datum
		{
			PAD16;
			word_flags flags;
			PAD32;
		};

		//////////////////////////////////////////////////////////////////////////
		// devices
		struct s_scenario_device_datum
		{
			PAD64;
			int16 power_group_index;
			int16 position_group_index;
			long_flags flags;
		};
		struct s_scenario_machine_datum
		{
			long_flags flags;
			TAG_PAD(int32, 3);
		};
		struct s_scenario_control_datum
		{
			long_flags flags;
			int16 custom_name_index;
			PAD16;
			PAD64;
		};
		struct s_scenario_light_fixture_datum
		{
			real_rgb_color color;
			real intensity;
			angle falloff, cutoff;
			PAD128;
		};


		struct s_scenario_object
		{
			s_scenario_object_header header;
			s_scenario_object_datum object;
		};
		struct s_scenario_device : public s_scenario_object
		{
			s_scenario_device_datum device;
		};
		struct s_scenario_unit : public s_scenario_object
		{
			s_scenario_unit_datum unit;
		};


		struct s_scenario_scenery : public s_scenario_object
		{
			s_scenario_scenery_datum scenery;
		}; BOOST_STATIC_ASSERT( sizeof(s_scenario_scenery) == 0x48 );

		//////////////////////////////////////////////////////////////////////////
		// units
		struct s_scenario_biped : public s_scenario_unit
		{
			s_scenario_biped_datum biped;
		}; BOOST_STATIC_ASSERT( sizeof(s_scenario_biped) == 0x78 );
		struct s_scenario_vehicle : public s_scenario_unit
		{
			s_scenario_vehicle_datum vehicle;
		}; BOOST_STATIC_ASSERT( sizeof(s_scenario_vehicle) == 0x78 );
		//////////////////////////////////////////////////////////////////////////
		// items
		struct s_scenario_equipment : public s_scenario_object
		{
			s_scenario_equipment_datum equipment;
		}; BOOST_STATIC_ASSERT( sizeof(s_scenario_equipment) == 0x28 );
		struct s_scenario_weapon : public s_scenario_object
		{
			s_scenario_weapon_datum weapon;
		}; BOOST_STATIC_ASSERT( sizeof(s_scenario_weapon) == 0x5C );
		//////////////////////////////////////////////////////////////////////////
		// devices
		struct s_scenario_machine : public s_scenario_device
		{
			s_scenario_machine_datum machine;
		}; BOOST_STATIC_ASSERT( sizeof(s_scenario_machine) == 0x40 );
		struct s_scenario_control : public s_scenario_device
		{
			s_scenario_control_datum control;
		}; BOOST_STATIC_ASSERT( sizeof(s_scenario_control) == 0x40 );
		struct s_scenario_light_fixture : public s_scenario_device
		{
			s_scenario_light_fixture_datum light_fixture;
		}; BOOST_STATIC_ASSERT( sizeof(s_scenario_light_fixture) == 0x58 );

		struct s_scenario_sound_scenery : public s_scenario_object
		{
			s_scenario_sound_scenery_datum sound_scenery;
		}; BOOST_STATIC_ASSERT( sizeof(s_scenario_sound_scenery) == 0x28 );
	};
};