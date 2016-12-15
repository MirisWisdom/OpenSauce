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
			_object_field_real_radius,
			_object_field_real_scale,
			_object_field_real_maximum_vitality, // Keep for backwards compatibility ONLY, value is incorrect
			_object_field_real_current_vitality, // Keep for backwards compatibility ONLY, value is incorrect
			_object_field_real_maximum_health,
			_object_field_real_maximum_shield,
			_object_field_real_current_health,
			_object_field_real_current_shield,

			_object_field_vector_position,
			_object_field_vector_transitional_velocity,
			_object_field_vector_forward,
			_object_field_vector_up,
			_object_field_vector_angular_velocity,
			_object_field_vector_center,

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

			_weapon_field_integer_magazine_state,
			_weapon_field_integer_magazine_reload,
			_weapon_field_integer_magazine_rounds,

			_weapon_tag_field_real_trigger_spew_time,
			_weapon_tag_field_real_trigger_rounds_per_second,

			_unit_field_object_index_throwing_grenade_projectile,
			_unit_field_object_index_weapon,
			_unit_field_object_index_equipment,
			_unit_field_object_index_40C, // TBD, when we verify the full extent of the field at 0x40C
			_unit_field_object_index_damage_responsible_flamer_object,
			_unit_field_object_index_recent_damage_unit,

			_unit_field_integer_ticks_until_flame_to_death,
			_unit_field_integer_throwing_grenade_state,
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
			_unit_field_integer_killing_spree_count,

			_unit_field_real_driver_power,
			_unit_field_real_gunner_power,
			_unit_field_real_integrated_light_power,
			_unit_field_real_integrated_light_toggle_power,
			_unit_field_real_integrated_night_vision_toggle_power,
			_unit_field_real_camo_power,
			_unit_field_real_full_spectrum_vision_power,
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
			_Inout_ TypeHolder& result);
		void ObjectDataSetRealByName(s_object_header_datum* header, 
			cstring data_name, cstring subdata_name,
			real data_value);

		
		void WeaponDataGetRealByName(s_weapon_datum* weapon, 
			cstring data_name,
			_Inout_ TypeHolder& result);
		void WeaponDataSetRealByName(s_weapon_datum* weapon, 
			cstring data_name,
			real data_value);

		void WeaponDataMagazineGetIntegerByName(s_weapon_datum* weapon, int32 magazine_index, 
			cstring data_name, cstring subdata_name, 
			_Inout_ TypeHolder& result);
		void WeaponDataMagazineSetIntegerByName(s_weapon_datum* weapon, int32 magazine_index, 
			cstring data_name, cstring subdata_name, 
			int32 data_value);

		void WeaponTagDataTriggerSetRealByName(s_weapon_datum* weapon, int32 trigger_index, 
			cstring data_name, cstring subdata_name, 
			real data_value);


		void UnitDataGetObjectIndexByName(s_unit_datum* unit, 
			cstring data_name,
			_Inout_ TypeHolder& result);

		void UnitDataGetIntegerByName(s_unit_datum* unit, 
			cstring data_name,
			_Inout_ TypeHolder& result);
		void UnitDataSetIntegerByName(s_unit_datum* unit, 
			cstring data_name,
			int32 data_value);
		void UnitDataGetRealByName(s_unit_datum* unit, 
			cstring data_name,
			_Inout_ TypeHolder& result);
		void UnitDataSetRealByName(s_unit_datum* unit, 
			cstring data_name,
			real data_value);
	};
};