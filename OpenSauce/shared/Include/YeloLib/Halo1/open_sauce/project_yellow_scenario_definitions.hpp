/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <YeloLib/tag_files/tag_groups_base_yelo.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum tag_postprocess_mode : byte_enum;

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

	namespace Flags
	{
		enum project_yellow_flags : word_flags
		{
			_project_yellow_dont_fix_ui_game_globals_bit,
			_project_yellow_game_updates_ignore_player_pvs_hack_bit,

			k_number_of_project_yellow_flags,

			_project_yellow_invalid_version_bit = 13,		// runtime. a yelo tag was found but it was an invalid version
			_project_yellow_null_definition_bit = 14,		// runtime. 'null_yelo' definition
			_project_yellow_cache_is_protected_bit = 15,	// runtime. cache was protected with HEK+

			_project_yellow__first_runtime_bit =
				_project_yellow_invalid_version_bit,
		};
		BOOST_STATIC_ASSERT(k_number_of_project_yellow_flags <= _project_yellow__first_runtime_bit);

		enum project_yellow_gameplay_flags : long_flags
		{
			// don't allow MTV settings for this map, even if the user enables it
			_project_yellow_gameplay_prohibit_multiteam_vehicles_bit,

			k_number_of_project_yellow_gameplay_flags
		};
	};

	namespace TagGroups
	{
		struct s_scripting_definitions;
		struct s_project_yellow_scripted_ui_widget;
		struct scenario;


		struct s_project_yellow_scenario_build_info
		{
			PAD16;
			TAG_ENUM(build_stage, Enums::production_build_stage);
			TAG_FIELD(uint32, revision);
			time_t timestamp;			BOOST_STATIC_ASSERT(sizeof(time_t) == 0x8);
			byte uuid_buffer[Enums::k_uuid_buffer_size];

			TAG_PAD(int32, 4); // 16

			bool HasUuid() const;
			void GenerateUuid();
		};

		//////////////////////////////////////////////////////////////////////////

		// yelo for scenarios
		struct project_yellow
		{
			enum { 
				k_group_tag = 'yelo',
				k_version = 2,
			};

			// internal name of the cache's Yelo tag when the user doesn't supply a definition
			static cstring k_default_name;

#if !PLATFORM_IS_EDITOR
			const 
#endif
				int16 version;
			TAG_FIELD(word_flags, flags, Flags::project_yellow_flags);


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
			struct _physics {
				TAG_FIELD(real, gravity_scale, "[0,2]", "amount to scale gravity ingame");
				TAG_FIELD(real, player_speed_scale, "[0,6]", "amount to scale the player speeds");

				TAG_PAD(int32, 5); // 20

				bool IsGravityScaleValid() const;
				void ResetGravityScale();

				bool IsPlayerSpeedScaleValid() const;
				void ResetPlayerSpeedScale();
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
				TAG_FIELD(long_flags, flags, Flags::project_yellow_gameplay_flags);

				TAG_PAD(int32, 5); // 20
			}gameplay;
			/* !-- Gameplay --! */


			/* !-- Scripting --! */
			TAG_TBLOCK(user_scripting, s_scripting_definitions); // 1
			/* !-- Scripting --! */


			TAG_PAD(int32, 23); // 92

			project_yellow(const bool invalid = false)
				: version(project_yellow::k_version)
			{
				flags = FLAG(Flags::_project_yellow_null_definition_bit);

				if(invalid)
				{
					SET_FLAG(flags, Flags::_project_yellow_invalid_version_bit, true);
				}

				yelo_globals.tag_index = datum_index::null;
				game_globals.tag_index = datum_index::null;
				explicit_references.tag_index = datum_index::null;
			}

			bool IsNull() const;
			bool IsCacheProtected() const;

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>
			/// 	Process a yelo scenario's globals data for the current operating mode (editing or cache building).
			/// </summary>
			///
			/// <param name="for_build_cache">	True if we're building a cache file, false if we're editing. </param>
			///
			/// <returns>	Returns the loaded yelo global's handle or datum_index::null. </returns>
			datum_index LoadYeloGlobals(const bool for_build_cache);

			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// <summary>	Loads the game_globals reference and renames it to K_GAME_GLOBALS_TAG_NAME. </summary>
			///
			/// <returns>	false if game_globals is NONE (ie, no override) or failed to load. </returns>
			bool LoadGameGlobalsOverride();

			static bool PLATFORM_API GroupPostprocess(datum_index tag_index, Enums::tag_postprocess_mode mode);
		};
	};
};