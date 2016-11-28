/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <blamlib/Halo1/interface/hud_definitions.hpp>
#include <blamlib/Halo1/interface/hud_unit.hpp>

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	namespace Flags
	{
		enum auxilary_overlay_type : _enum
		{
			_auxilary_overlay_type_team_icon,

			_auxilary_overlay_type,
		};

		enum hud_use_on_map_type : _enum
		{
			_hud_use_on_map_type_any,
			_hud_use_on_map_type_solo,
			_hud_use_on_map_type_multiplayer,

			_hud_use_on_map_type,
		};

		enum unit_hud_sound_flags : long_flags
		{
			_unit_hud_sound_shield_recharging_bit,
			_unit_hud_sound_shield_damaged_bit,
			_unit_hud_sound_shield_low_bit,
			_unit_hud_sound_shield_empty_bit,
			_unit_hud_sound_health_low_bit,
			_unit_hud_sound_health_empty_bit,
			_unit_hud_sound_health_minor_damage_bit,
			_unit_hud_sound_health_major_damage_bit,
		};
	};

	namespace TagGroups
	{
		struct auxilary_overlay_definition : public s_hud_element_overlay
		{
			TAG_ENUM(type, Enums::auxilary_overlay_type);
			TAG_FIELD(Flags::hud_auxilary_overlay_flags, flags);
			TAG_PAD(int32, 6);
		}; BOOST_STATIC_ASSERT( sizeof(auxilary_overlay_definition) == 0x84 ); // max count: 16

		struct auxilary_meter_definition
		{
			TAG_ENUM(type, Enums::hud_auxilary_meter_type);
			PAD16;
			TAG_PAD(int32, 4);

			s_hud_element_overlay background;
			s_hud_element_meter meter;
			TAG_FIELD(real, minimum_fraction_cutoff);
			TAG_FIELD(Flags::hud_auxilary_panel_flags, flags);
			TAG_PAD(int32, 6);
			TAG_PAD(int32, 16);
		}; BOOST_STATIC_ASSERT( sizeof(auxilary_meter_definition) == 0x144 ); // max count: 16

		struct unit_hud_interface_definition
		{
			enum { k_group_tag = 'unhi' };

			s_hud_absolute_placement placement;

			s_hud_element_overlay hud_background;

			struct {
				s_hud_element_overlay background;

				struct {
					s_hud_element_meter element;
					TAG_FIELD(rgb_color, overcharge_minimum_color);
					TAG_FIELD(rgb_color, overcharge_maximum_color);
					TAG_FIELD(rgb_color, overcharge_flash_color);
					TAG_FIELD(rgb_color, overcharge_empty_color);
					TAG_PAD(int32, 4);
				}meter;
			}shield_panel;

			struct {
				s_hud_element_overlay background;

				struct {
					s_hud_element_meter element;
					TAG_FIELD(rgb_color, medium_health_left_color);
					TAG_FIELD(real, max_color_health_fraction_cutoff);
					TAG_FIELD(real, min_color_health_fraction_cutoff);
					TAG_PAD(int32, 5);
				}meter; // same size as shield_panel.meter, so this may be union'd
			}health_panel;

			struct {
				s_hud_element_overlay background;
				s_hud_element_overlay forground;
				TAG_PAD(int32, 8);

				s_hud_element center; // The blips use this as a reference point
			}motion_sensor;

			struct {
				s_hud_absolute_placement placement;

				TAG_TBLOCK(overlays, auxilary_overlay_definition);
			}auxilary_overlays;

			TAG_PAD(int32, 4);

			TAG_TBLOCK(sounds, sound_hud_element_definition);

			TAG_TBLOCK(meters, auxilary_meter_definition);

			TAG_PAD(int32, 89);
			TAG_PAD(int32, 12);
		}; BOOST_STATIC_ASSERT( sizeof(unit_hud_interface_definition) == 0x56C );
	};
};