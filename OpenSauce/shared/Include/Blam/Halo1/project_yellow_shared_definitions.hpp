/*
    Yelo: Open Sauce SDK

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include <TagGroups/Halo1/TagGroupDefinitions.hpp>
#include <Blam/Halo1/project_yellow_globals_shared_definitions.hpp>

namespace Yelo
{
	namespace TagEnums
	{
		enum biped_camera_type {
			_biped_camera_type_first_person,
			_biped_camera_type_third_person_fixed,
			_biped_camera_type_third_person_loose,
			_biped_camera_type_inverted,
			_biped_camera_type,
		};
	};

	namespace TagGroups
	{
		struct s_bsp_information_lightmap_set
		{
			TAG_FIELD(tag_string, name, "", "code name for this lightmap set");
			struct _flags {
				TAG_FLAG16(is_default);
			}flags;	BOOST_STATIC_ASSERT( sizeof(_flags) == sizeof(word_flags) );

			TAG_FIELD(tag_reference, definition, "bitm");
			TAG_FIELD(int16, sky_index);				// block_index to project_yellow_bsp_information->skies
			TAG_FIELD(int16, fog_index);				// block_index to project_yellow_bsp_information->bsp_fog
			TAG_FIELD(int16, weather_index);			// block_index to project_yellow_bsp_information->bsp_weather
			PAD16;

			TAG_PAD(tag_block, 2); // 24

			API_INLINE bool ContainsClusterModifications() const
			{
				return	sky_index != NONE || fog_index != NONE || weather_index != NONE;
			}
		};
		struct s_bsp_information_lightmap_set_reference
		{
			TAG_FIELD(int16, set_index); // block index to s_bsp_information_bsp_set->lightmap_sets
			PAD16;
			TAG_PAD(int32, 3); // 12
		};
		struct s_bsp_information_lightmap_set_group
		{
			TAG_FIELD(tag_string, name, "", "code name for this lightmap set group");
			TAG_TBLOCK(set_references, s_bsp_information_lightmap_set_reference); // 128
			TAG_PAD(int32, 8); // 32
		};
		struct s_bsp_information_bsp_set
		{
			TAG_FIELD(tag_reference, structure_bsp, "sbsp");
			TAG_TBLOCK(lightmap_sets, s_bsp_information_lightmap_set); // 128
			TAG_PAD(tag_block, 1); // 12, future s_bsp_information_lightmap_set_group
		};
		struct s_project_yellow_bsp_information
		{
			TAG_TBLOCK(skies, tag_reference);					// 64, all of these need to be in the exact same order as the blocks in the scenario \ bsp
 			TAG_TBLOCK(bsp_fog, tag_reference);
			TAG_TBLOCK(bsp_weather, tag_reference);

			TAG_PAD(tag_block, 2); // 24
			TAG_TBLOCK(bsp_sets, s_bsp_information_bsp_set); // 64
		};

		struct s_trigger_volume_cubemap
		{
			TAG_FIELD(tag_string, trigger_volume_name);
			TAG_FIELD(int16, cubemap_index);
			PAD16;
		}; BOOST_STATIC_ASSERT( sizeof(s_trigger_volume_cubemap) == 0x24 );

		// yelo for scenarios
		struct project_yellow
		{
#if !PLATFORM_IS_EDITOR
			const 
#endif
				int16 version;	enum { k_version = 2, k_group_tag = 'yelo' };
			struct _flags {
				TAG_FLAG16(unused); // current unused, so add a bit field so it pads to 2 bytes
				TAG_FLAG16(unused1);
				TAG_FLAG16(unused2);
				TAG_FLAG16(unused3);
				TAG_FLAG16(unused4);
				TAG_FLAG16(unused5);
				TAG_FLAG16(unused6);
				TAG_FLAG16(unused7);
				TAG_FLAG16(unused8);
				TAG_FLAG16(unused9);
				TAG_FLAG16(unused10);
				TAG_FLAG16(unused11);
				TAG_FLAG16(unused12);

				// Flags set at runtime
				TAG_FLAG16(invalid_version); // a yelo tag was found but it was an invalid version
				TAG_FLAG16(null_definition); // 'null_yelo' definition
				TAG_FLAG16(cache_is_protected); // cache was protected with HEK+
			}flags;	BOOST_STATIC_ASSERT( sizeof(_flags) == sizeof(word_flags) );


			/* !-- Misc --! */
			TAG_FIELD(tag_reference, yelo_globals, "gelo");
			TAG_FIELD(tag_reference, game_globals, "matg");
			TAG_FIELD(tag_reference, explicit_references, "tagc");

			TAG_PAD(int32, 10); // 40
			/* --- Misc --- */


			/* !-- UI/GUI --! */
			struct {
				struct _ui_flags {
					TAG_FLAG16(unused);
				}flags;	BOOST_STATIC_ASSERT( sizeof(_ui_flags) == sizeof(word_flags) );
				TAG_ENUM(biped_camera_type, biped_camera_type, "camera mode to use for players");

				TAG_PAD(tag_block, 2); // 24
			}ui;
			/* --- UI/GUI --- */


			/* !-- Physics --! */
			struct {
				TAG_PAD(int16, 2); // old scales, represented as 16 bit integers

				TAG_FIELD(real, gravity_scale, "[0,2]", "amount to scale gravity ingame");
				TAG_FIELD(real, player_speed_scale, "[0,6]", "amount to scale the player speeds");

				TAG_PAD(int32, 3); // 12

				API_INLINE bool IsGravityScaleValid() const	{ return gravity_scale >= 0.0f || gravity_scale <= 2.0f; }
				API_INLINE void ResetGravityScale()			{ gravity_scale = 1.0f; }

				API_INLINE bool IsPlayerSpeedScaleValid() const	{ return gravity_scale >= 0.0f || gravity_scale <= 6.0f; }
				API_INLINE void ResetPlayerSpeedScale()			{ player_speed_scale = 1.0f; }
			}physics;
			/* --- Physics --- */


			/* !-- Networking --! */
			struct {
				struct _networking_flags {
					TAG_FLAG(use_server_upgrades, "prone to cause unstable gameplay after a while");
					TAG_FLAG(use_client_upgrades);
				}flags;	BOOST_STATIC_ASSERT( sizeof(_networking_flags) == sizeof(long_flags) );

				TAG_PAD(int32, 5); // 20
			}networking;
			/* --- Networking --- */


			/* !-- Gameplay --! */
			struct {
				struct _gameplay_flags {
					TAG_FLAG(allow_multiteam_vehicles, "if not set, disallow is assumed");
				}flags;	BOOST_STATIC_ASSERT( sizeof(_gameplay_flags) == sizeof(long_flags) );

				TAG_PAD(int32, 5); // 20
			}gameplay;
			/* !-- Gameplay --! */


			/* !-- Scripting --! */
			TAG_TBLOCK(user_scripting, scripting_block); // 1
			/* !-- Scripting --! */

			/* !-- Bsp Enhancements --! */
			TAG_TBLOCK(bsp_information, s_project_yellow_bsp_information); // 1
			/* !-- Bsp Enhancements --! */


			TAG_PAD(int32, 20); // 80



			API_INLINE bool IsNull() const			{ return flags.null_definition_bit != false; }
			API_INLINE bool IsCacheProtected() const{ return flags.cache_is_protected_bit != false; }
		};
	};
};