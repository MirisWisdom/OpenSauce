/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum object_field_index : _enum {
			_object_field_real_function_out,

			_object_field_vector_position,
			_object_field_vector_transitional_velocity,
			_object_field_vector_forward,
			_object_field_vector_up,
			_object_field_vector_angular_velocity,

			_unit_field_vector_desired_facing,
			_unit_field_vector_desired_aiming,
			_unit_field_vector_aiming,
			_unit_field_vector_aiming_velocity,
			_unit_field_vector_looking,
			_unit_field_vector_looking_angles,
			_unit_field_vector_looking_velocity,

			_weapon_field_real_heat,
			_weapon_field_real_age,
			_weapon_field_real_light_power,

			_weapon_tag_field_real_trigger_spew_time,
			_weapon_tag_field_real_trigger_rounds_per_second,

			_unit_field_object_index_weapon,
			_unit_field_object_index_recent_damage_unit,

			_unit_field_integer_total_grenade_count_frag,
			_unit_field_integer_total_grenade_count_plasma,
			_unit_field_integer_total_grenade_count_custom2,
			_unit_field_integer_total_grenade_count_custom3,
			_unit_field_integer_current_grenade_index,
			_unit_field_integer_zoom_level,
			_unit_field_integer_desired_zoom_level,

			_unit_field_integer_vehicle_seat_index,
			_unit_field_integer_current_weapon_index,
			_unit_field_integer_feign_death_timer,

			_unit_field_real_camo_power,
			_unit_field_real_driver_power,
			_unit_field_real_gunner_power,
		};
	};

	namespace Objects
	{
		struct s_object_header_datum;

		struct s_weapon_datum;
		struct s_unit_datum;

		void InitializeObjectFieldDefinitions();

		real_vector3d* ObjectDataGetVectorByName(s_object_header_datum* header, 
			cstring data_name);

		void ObjectDataGetRealByName(s_object_header_datum* header, 
			cstring data_name, cstring subdata_name,
			__inout TypeHolder& result);
		void ObjectDataSetRealByName(s_object_header_datum* header, 
			cstring data_name, cstring subdata_name,
			real data_value);

		
		void WeaponDataGetRealByName(s_weapon_datum* weapon, 
			cstring data_name,
			__inout TypeHolder& result);
		void WeaponDataSetRealByName(s_weapon_datum* weapon, 
			cstring data_name,
			real data_value);

		void WeaponTagDataTriggerSetRealByName(s_weapon_datum* weapon, int32 trigger_index, 
			cstring data_name, cstring subdata_name, 
			real data_value);


		void UnitDataGetObjectIndexByName(s_unit_datum* unit, 
			cstring data_name,
			__inout TypeHolder& result);

		void UnitDataGetIntegerByName(s_unit_datum* unit, 
			cstring data_name,
			__inout TypeHolder& result);
		void UnitDataSetIntegerByName(s_unit_datum* unit, 
			cstring data_name,
			int32 data_value);
		void UnitDataGetRealByName(s_unit_datum* unit, 
			cstring data_name,
			__inout TypeHolder& result);
		void UnitDataSetRealByName(s_unit_datum* unit, 
			cstring data_name,
			real data_value);
	};
};