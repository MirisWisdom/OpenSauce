/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/interface/hud_definitions.hpp>

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum weapon_hud_state
		{
			_weapon_hud_state_total_ammo,
			_weapon_hud_state_loaded_ammo,
			_weapon_hud_state_heat,
			_weapon_hud_state_age,
			_weapon_hud_state_secondary_weapon_total_ammo,
			_weapon_hud_state_secondary_weapon_loaded_ammo,
			_weapon_hud_state_dist_to_target,
			_weapon_hud_state_elevation_to_target,

			k_number_of_weapon_hud_states,
		};

		enum weapon_crosshair_type
		{
			_weapon_crosshair_type_aim,
			_weapon_crosshair_type_zoom,
			_weapon_crosshair_type_charge,
			_weapon_crosshair_type_should_reload,
			_weapon_crosshair_type_flash_heat,
			_weapon_crosshair_type_flash_total_ammo,
			_weapon_crosshair_type_flash_battery,
			_weapon_crosshair_type_reload_or_overheat,
			_weapon_crosshair_type_flash_when_firing_and_no_ammo,
			_weapon_crosshair_type_flash_when_firing_and_no_grenade,
			_weapon_crosshair_type_low_ammo_and_none_left_to_reload,
			_weapon_crosshair_type_should_reload_secondary_trigger,
			_weapon_crosshair_type_flash_secondary_total_ammo,
			_weapon_crosshair_type_flash_secondary_reload,
			_weapon_crosshair_type_flash_when_firing_secondary_trigger_with_no_ammo,
			_weapon_crosshair_type_low_secondary_ammo_and_none_left_to_reload,
			_weapon_crosshair_type_primary_trigger_ready,
			_weapon_crosshair_type_secondary_trigger_ready,
			_weapon_crosshair_type_flash_when_firing_with_depleted_battery,

			k_number_of_weapon_crosshair_types,
		};
	};

	namespace Flags
	{
		enum
		{
			_weapon_hud_element_runtime_invalid_bit, // set by postprocessing code
		};

		enum
		{
			_weapon_hud_overlay_type_show_on_flashing_bit,
			_weapon_hud_overlay_type_show_on_empty_bit,
			_weapon_hud_overlay_type_show_on_reload_bit, // or overheating
			_weapon_hud_overlay_type_show_on_default_bit,
			_weapon_hud_overlay_type_show_always_bit,
		};
		enum
		{
			_weapon_hud_overlay_flashes_when_active_bit,
			_weapon_hud_overlay_invalid_bit, // not exposed. set during postprocess, when overlay references invalid sequence
		};

		enum grenade_hud_sound_flags : long_flags
		{
			_grenade_hud_sound_low_grenade_count_bit,
			_grenade_hud_sound_no_grenades_left_bit,
			_grenade_hud_sound_throw_on_no_grenades_bit,
		};
	};

	namespace TagGroups
	{
		//////////////////////////////////////////////////////////////////////////
		// weapon hud interface
		struct weapon_hud_element
		{
			TAG_ENUM(state_attached_to, Enums::weapon_hud_state);
			byte_flags runtime_flags;
			PAD8;
			TAG_ENUM(can_use_on_map_type, Enums::hud_use_on_map_type); // actually treated as byte_flags
			PAD16;
			TAG_PAD(int32, 7);
		}; BOOST_STATIC_ASSERT( sizeof(weapon_hud_element) == 0x24 );

		struct weapon_hud_static_element : public weapon_hud_element
		{
			s_hud_element_overlay overlay;
			TAG_PAD(int32, 10);
		}; BOOST_STATIC_ASSERT( sizeof(weapon_hud_static_element) == 0xB4 );

		struct weapon_hud_meter_element : public weapon_hud_element
		{
			s_hud_element_meter element;
			TAG_PAD(int32, 10);
		}; BOOST_STATIC_ASSERT( sizeof(weapon_hud_meter_element) == 0xB4 );

		struct weapon_hud_number_element : public weapon_hud_element
		{
			s_hud_element_number element;
			TAG_FIELD(word_flags, weapon_flags);
			PAD16;
			TAG_PAD(int32, 9);
		}; BOOST_STATIC_ASSERT( sizeof(weapon_hud_number_element) == 0xA0 );

		struct weapon_hud_crosshair_item
		{
			s_hud_element element;
			s_hud_color_flash flash;
			TAG_FIELD(int16, frame_rate);
			TAG_FIELD(int16, sequence_index);
			TAG_FIELD(long_flags, flags);
			TAG_PAD(int32, 8);
		}; BOOST_STATIC_ASSERT( sizeof(weapon_hud_crosshair_item) == 0x6C );
		struct weapon_hud_crosshairs_element
		{
			TAG_ENUM(crosshair_type, Enums::weapon_crosshair_type);
			byte_flags runtime_flags;
			PAD8;
			TAG_ENUM(can_use_on_map_type, Enums::hud_use_on_map_type); // actually byte_enum
			PAD16;
			TAG_PAD(int32, 7);
			TAG_FIELD(tag_reference, crosshair_bitmap, 'bitm');
			TAG_TBLOCK(crosshair_overlays, weapon_hud_crosshair_item);
			TAG_PAD(int32, 10);
		}; BOOST_STATIC_ASSERT( sizeof(weapon_hud_crosshairs_element) == 0x68 );

		struct weapon_hud_overlay_item
		{
			s_hud_element element;
			s_hud_color_flash flash;
			TAG_FIELD(int16, frame_rate);
			PAD16;
			TAG_FIELD(int16, sequence_index);
			TAG_FIELD(word_flags, type);
			TAG_FIELD(long_flags, flags);
			TAG_PAD(tag_reference, 1);
			TAG_PAD(int32, 10);
		}; BOOST_STATIC_ASSERT( sizeof(weapon_hud_overlay_item) == 0x88 );
		struct weapon_hud_overaly
		{
			TAG_FIELD(tag_reference, overlay_bitmap, 'bitm');
			TAG_TBLOCK(overlays, weapon_hud_overlay_item);
		};
		struct weapon_hud_overlays_element : public weapon_hud_element
		{
			weapon_hud_overaly overlay;
			TAG_PAD(int32, 10);
		}; BOOST_STATIC_ASSERT( sizeof(weapon_hud_overlays_element) == 0x68 );

		struct weapon_hud_messaging_information
		{
			TAG_FIELD(int16, sequence_index);
			TAG_FIELD(int16, width_offset);
			TAG_FIELD(point2d, offset_from_reference_corner);
			TAG_FIELD(argb_color, override_icon_color);
			TAG_FIELD(sbyte, frame_rate);
			TAG_FIELD(byte_flags, flags);
			TAG_FIELD(int16, text_index);
			TAG_PAD(int32, 12);
		};

		struct weapon_hud_interface_definition
		{
			enum { k_group_tag = 'wphi' };

			TAG_FIELD(tag_reference, child_hud, 'wphi');
			struct {
				TAG_FIELD(word_flags, flags);
				PAD16;
				TAG_FIELD(int16, inventory_ammo);
				TAG_FIELD(int16, loaded_ammo);
				TAG_FIELD(int16, heat);
				TAG_FIELD(int16, age);
				TAG_PAD(int32, 8);
			}flash_cutoffs;

			s_hud_absolute_placement placement;

			TAG_TBLOCK(static_elements, weapon_hud_static_element);
			TAG_TBLOCK(meter_elements, weapon_hud_meter_element);
			TAG_TBLOCK(number_elements, weapon_hud_number_element);
			TAG_TBLOCK(crosshairs, weapon_hud_crosshairs_element);
			TAG_TBLOCK(overlay_elements, weapon_hud_overlays_element);
			long_flags runtime_used_crosshairs[BIT_VECTOR_SIZE_IN_DWORDS(Enums::k_number_of_weapon_crosshair_types)];
			TAG_PAD(tag_block, 1);
			TAG_TBLOCK(screen_effect, hud_screen_effect_definition);
			TAG_PAD(tag_block, 11);
			weapon_hud_messaging_information messaging_information;
		}; BOOST_STATIC_ASSERT( sizeof(weapon_hud_interface_definition) == 0x17C );


		//////////////////////////////////////////////////////////////////////////
		// grenade hud interface
		struct grenade_hud_interface_definition
		{
			enum { k_group_tag = 'grhi' };
			
			s_hud_absolute_placement placement;
			s_hud_element_overlay hud_background;
			s_hud_element_overlay total_grenades_background;
			struct {
				s_hud_element_number element;
				TAG_FIELD(int16, flash_cutoff);
				PAD16;
			}total_grenades_numbers;
			weapon_hud_overaly total_grenade_overlays;
			TAG_TBLOCK(warning_sounds, sound_hud_element_definition);
			TAG_PAD(int32, 17);
			weapon_hud_messaging_information messaging_information;
		}; BOOST_STATIC_ASSERT( sizeof(grenade_hud_interface_definition) == 0x1F8 );
	};
};