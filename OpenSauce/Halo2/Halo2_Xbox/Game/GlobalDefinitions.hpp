/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Enums
	{
		enum material_type
		{
			_material_type_dirt,
			_material_type_sand,
			_material_type_stone,
			_material_type_snow,
			_material_type_wood,
			_material_type_metal_hollow,
			_material_type_metal_thin,
			_material_type_metal_thick,
			_material_type_rubber,
			_material_type_glass,
			_material_type_force_field,
			_material_type_grunt,
			_material_type_hunter_armor,
			_material_type_hunter_skin,
			_material_type_elite,
			_material_type_jackel,
			_material_type_jackel_energy_shield,
			_material_type_engineer_skin,
			_material_type_engineer_force_field,
			_material_type_flood_combat_form,
			_material_type_flood_carrier_form,
			_material_type_cyborg_armor,
			_material_type_cyborg_energy_shield,
			_material_type_human_armor,
			_material_type_human_skin,
			_material_type_sentinel,
			_material_type_monitor,
			_material_type_plastic,
			_material_type_water,
			_material_type_leaves,
			_material_type_elite_energy_shield,
			_material_type_ice,
			_material_type_hunter_shield,
			_material_type,
		};
	};

	namespace GameGlobals
	{
		struct s_game_globals_collision_damage
		{
			TagReference<'jpt!'> CollisionDamage;
			real_bounds GameAcceleration;
			real_bounds GameScale;
			real_bounds AbsoluteAcceleration;
			real_bounds AbsoluteScale;
			TAG_PAD(real, 8); // properly post-processed versions of the above fields
		};

		struct s_sound_globals_definition
		{
			TagReference<'sncl'> SoundClasses;
			TagReference<'sfx+'> SoundEffects;
			TagReference<'snmx'> SoundMix;
			TagReference<'spk!'> SoundCombatDialogueConstants;
			datum_index sound_gestalt; // ugh!
		};

		struct s_game_globals_camera
		{
			TagReference<'trak'> DefaultUnitCameraTrack;
			real DefaultChangePause, FirstPersonChangePause, FollowingCameraChangePause;
		};


		struct s_game_globals_grenade
		{
			int16 MaxCount;
			PAD16;
			TagReference<'effe'> ThrowingEffect;
			PAD64; PAD64; // future tag_reference
			TagReference<'eqip'> Equipment;
			TagReference<'proj'> Projectile;
		};


		struct s_campaign_runtime_level_definition
		{
			int32 CampaignId, MapId;
			long_string Path;
		};
		struct s_runtime_levels_definition
		{
			TagBlock<s_campaign_runtime_level_definition> CampaignLevels;
		};

		struct ui_level_runtime_data // pretty sure these are user_interface widget objects which are initialized at runtime
		{
			byte Skip1[576];
			byte Skip2[2304];
		};
		struct s_campaign_definition
		{
			int32 CampaignId;
			ui_level_runtime_data RuntimeData;
		};
		struct s_campaign_ui_level_definition
		{
			int32 CampaignId, MapId;
			TagReference<'bitm'> Bitmap;
			ui_level_runtime_data RuntimeData;
		};
		struct s_multiplayer_ui_level_definition
		{
			int32 MapId;
			TagReference<'bitm'> Bitmap;
			ui_level_runtime_data RuntimeData;

			long_string Path;
			int32 SortOrder;
			byte_flags Flags;
			PAD24;
			byte MaxTeams[16];
		};
		struct s_ui_levels_definition
		{
			TagBlock<s_campaign_definition> Campaigns;
			TagBlock<s_campaign_ui_level_definition> CampaignLevels;
			TagBlock<s_multiplayer_ui_level_definition> MultiplayerLevels;
		};



		struct group
		{
			TAG_PAD(int32, 43); // 0x0
			long_enum Language; // 0xB4
			TagBlock<TagReference<'effe'> > HavokCleanupResources; // 0xB0
			TagBlock<s_game_globals_collision_damage> CollisionDamage; // 0xB8
			TagBlock<s_sound_globals_definition> SoundGlobals; // 0xC0
			tag_block AiGlobals; // 0xC8, struct s_ai_globals_definition
			tag_block DamageTable; // 0xD0, struct s_damage_globals_definition
			UNUSED_TYPE(tag_block); // 0xD8
			UNUSED_TYPE(tag_block); // 0xE0, sounds (obsolete)
			TagBlock<s_game_globals_camera> Camera; // 0xE8
			tag_block PlayerControl; // 0xF0, struct s_game_globals_player_control
			tag_block Difficulty; // 0xF8, struct s_game_globals_difficulty_information
			TagBlock<s_game_globals_grenade> Grenades; // 0x100
			tag_block RastizerData; // 0x108, struct s_game_globals_rasterizer_data
			tag_block InterfaceTags; // 0x110, struct s_game_globals_interface_tag_references
			TAG_PAD(tag_block, 2); // 0x118
				// cheat_weapons, struct s_game_globals_tag_reference
				// cheat_powerups, struct s_game_globals_tag_reference
			tag_block MultplayerInformation; // 0x128, deprecated, struct s_game_globals_multiplayer_information
			tag_block PlayerInformation; // 0x130, struct s_game_globals_player_information
			tag_block PlayerRepresentation; // 0x138, struct s_game_globals_player_representation
			tag_block FallingDamage; // 0x140, struct s_game_globals_falling_damage
			tag_block OldMaterials; // 0x148, struct material_definition
			tag_block Materials; // 0x150, struct s_global_material_definition
			tag_block MultiplayerUi; // 0x158, struct s_game_globals_multiplayer_ui
			TagBlock<real_rgb_color> ProfileColors; // 0x160
			TagReference<'mulg'> MultiplayerGlobals; // 0x168
			TagBlock<s_runtime_levels_definition> RuntimeLevelData; // 0x170
			TagBlock<s_ui_levels_definition> UiLevelData; // 0x178
			TagReference<'gldf'> DefaultGlobalLighting; // 0x180

			struct s_language_pack {
				void* Index; // set when cache is loaded
				void* Table; // " "
				uint32 Count;
				uint32 Size;
				uint32 OffsetIndex;
				uint32 OffsetTable;
				BOOL Loaded; // this is a bool...
			}English, Japanese, Dutch, French, Spanish, Italian, Korean, Chinese, Portuguese; // 0x188
		}; BOOST_STATIC_ASSERT( sizeof(group) == 644 );
	};
};