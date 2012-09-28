/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <TagGroups/Halo1/TagGroupDefinitions.hpp>
#include <Blam/Halo1/project_yellow_globals_shared_definitions.hpp>

namespace Yelo
{
	namespace TagEnums
	{
		enum production_build_stage
		{
			_production_build_stage_ship,
			_production_build_stage_alpha,
			_production_build_stage_beta,
			_production_build_stage_delta,
			_production_build_stage_epsilon,
			_production_build_stage_release,
		};
	};

	namespace TagGroups
	{
		struct s_project_yellow_scenario_build_info
		{
			PAD16;
			TAG_ENUM(build_stage, TagEnums::production_build_stage);
			TAG_FIELD(uint32, revision);
			time_t timestamp;			BOOST_STATIC_ASSERT(sizeof(time_t) == 0x8);

			TAG_PAD(int32, 8); // 32
		};

		//////////////////////////////////////////////////////////////////////////
		// scenario information
		struct s_scenario_faux_zone_sky
		{
			TAG_FIELD(tag_string, name, "", "code name for this sky");
			PAD16;
			// Index of the scenario sky this will replace
			TAG_FIELD(int16, scenario_sky_to_replace); // block into to the 'skies' block
			TAG_FIELD(tag_reference, sky, 'sky ');

			bool IsValid() const { return scenario_sky_to_replace != NONE && !sky.tag_index.IsNull(); }
		};

		struct s_scenario_faux_zone_lightmap_set
		{
			TAG_FIELD(tag_reference, definition, 'bitm');
			TAG_PAD(tag_reference, 3); // 48
		};
		struct s_scenario_faux_zone_set_variant
		{
			TAG_FIELD(tag_string, name, "", "code name for this set variant");
			struct _flags {
				TAG_FLAG16(unused);
			}flags;	BOOST_STATIC_ASSERT( sizeof(_flags) == sizeof(word_flags) );

			TAG_FIELD(int16, lightmap_index,	s_scenario_faux_zone_lightmap_set);
			TAG_FIELD(int16, sky_index,			s_scenario_faux_zone_sky);

			PAD16;
			PAD32;

			// Does this variant have an actual changes to the game/tag state?
			API_INLINE bool HasGameStateChanges() const
			{
				return lightmap_index != NONE || sky_index != NONE;
			}
		};
		struct s_scenario_faux_zone_set
		{
			TAG_FIELD(tag_string, name, "", "code name for this zone set");
			TAG_FIELD(tag_reference, structure_bsp, 'sbsp');
			TAG_TBLOCK(lightmaps, s_scenario_faux_zone_lightmap_set); // 128
			TAG_TBLOCK(variants, s_scenario_faux_zone_set_variant); // 128
			TAG_PAD(tag_block, 2); // 24
		};
		struct s_project_yellow_scenario_information
		{
			// Skies that the scenario uses. Doesn't have to match the ordering of the scenario's block
			TAG_TBLOCK(skies, tag_reference); // 'sky ', k_maximum_skies_per_scenario
			TAG_PAD(tag_block, 1); // 12
			TAG_TBLOCK(zone_skies, s_scenario_faux_zone_sky); // 64
			TAG_TBLOCK(zones, s_scenario_faux_zone_set); // 64
			TAG_PAD(tag_block, 3); // 36
		};
		//////////////////////////////////////////////////////////////////////////

		// yelo for scenarios
		struct project_yellow
		{
#if !PLATFORM_IS_EDITOR
			const 
#endif
				int16 version;	enum { k_version = 2, k_group_tag = 'yelo' };
			struct _flags {
				TAG_FLAG16(unused); // currently unused, so add a bit field so it pads to 2 bytes
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
			TAG_TBLOCK(build_info, s_project_yellow_scenario_build_info); // 1

			TAG_PAD(int32, 10); // 40
			/* --- Misc --- */


			/* !-- UI/GUI --! */
			struct {
				TAG_TBLOCK(scripted_widgets, s_project_yellow_scripted_ui_widget); // 128

				TAG_PAD(int32, 4); // 16
			}ui;
			/* --- UI/GUI --- */


			/* !-- Physics --! */
			struct {
				TAG_FIELD(real, gravity_scale, "[0,2]", "amount to scale gravity ingame");
				TAG_FIELD(real, player_speed_scale, "[0,6]", "amount to scale the player speeds");

				TAG_PAD(int32, 5); // 20

				API_INLINE bool IsGravityScaleValid() const	{ return gravity_scale >= 0.0f || gravity_scale <= 2.0f; }
				API_INLINE void ResetGravityScale()			{ gravity_scale = 1.0f; }

				API_INLINE bool IsPlayerSpeedScaleValid() const	{ return gravity_scale >= 0.0f || gravity_scale <= 6.0f; }
				API_INLINE void ResetPlayerSpeedScale()			{ player_speed_scale = 1.0f; }
			}physics;
			/* --- Physics --- */


			/* !-- Netgame --! */
			struct {
				struct _networking_flags {
					TAG_FLAG(unused);
				}flags;	BOOST_STATIC_ASSERT( sizeof(_networking_flags) == sizeof(long_flags) );

				TAG_PAD(int32, 5); // 20
			}networking;
			/* --- Netgame --- */


			/* !-- Gameplay --! */
			struct {
				struct _gameplay_flags {
					TAG_FLAG(prohibit_multiteam_vehicles, "don't allow MTV settings for this map, even if the user enables it");
				}flags;	BOOST_STATIC_ASSERT( sizeof(_gameplay_flags) == sizeof(long_flags) );

				TAG_PAD(int32, 5); // 20
			}gameplay;
			/* !-- Gameplay --! */


			/* !-- Scripting --! */
			TAG_TBLOCK(user_scripting, scripting_block); // 1
			/* !-- Scripting --! */

			/* !-- Scenario Enhancements --! */
			TAG_TBLOCK(scenario, s_project_yellow_scenario_information); // 1
			/* !-- Scenario Enhancements --! */


			TAG_PAD(int32, 20); // 80



			API_INLINE bool IsNull() const			{ return flags.null_definition_bit != false; }
			API_INLINE bool IsCacheProtected() const{ return flags.cache_is_protected_bit != false; }
		};
	};
};