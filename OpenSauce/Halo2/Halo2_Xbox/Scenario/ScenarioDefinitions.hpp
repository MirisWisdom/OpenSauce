/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

#include <blamlib/Halo2/cache/predicted_resources.hpp>

#include "Game/GlobalDefinitions.hpp"
#include "Scenario/SkyDefinitions.hpp"
#include "Scenario/ObjectScenariioDefinitions.hpp"
#include "Scenario/AiScenarioDefinitions.hpp"
#include "Scenario/HsScenarioDefinitions.hpp"

namespace Yelo
{
	namespace Scenario
	{
		struct scenario_starting_profile
		{
			string_id Name;
			real_fraction StartingHealthDamage, StartingShieldDamage;
			struct s_weapon {
				TagReference<'weap'> Weapon;
				int16 RoundsLoaded, RoundsTotal;
			}Primary, Secondary;
			int16 StartingGrenadeCount[4];
		};

		struct scenario_player
		{
			real_point3d Position;
			angle Facing;
			_enum TeamDesignator;
			int16 BspIndex;
			_enum GameTypes[4];
			_enum SpawnTypes[4];

			string_id Unnamed[2];

			_enum CampaignPlayerType;
			PAD16; PAD32;
		};

		struct scenario_trigger_volume
		{
			string_id Name;
			int16 ObjectName;
			int16 _Skip; // I think this is post-processed to ObjectIndex
			string_id NodeName;
			real Data[6];
			real_point3d Position;
			real_point3d Extents;
			PAD32;
			int16 KillTriggerVolume;
			PAD16;
		};

		struct recorded_animation_definition
		{
			tag_string Name;
			byte Version;
			byte RawAnimationData;
			byte UnitControlDataVersion;
			PAD8;
			int16 LengthOfAnimation;
			PAD16;
			PAD32;
			tag_data EventStream;
		};

		struct scenario_netpoint
		{
			real_point3d Position;
			angle Facing;
			_enum Type;
			_enum TeamDesignator;
			int16 Identifier;
			word_flags Flags;

			string_id SpawnObjectName;
			string_id SpawnMarkerName;
		};

		struct scenario_netgame_equipment
		{
			long_flags Flags;
			_enum GameType[4];
			PAD16;
			int16 SpawnTime;
			int16 RespawnOnEmptyTime;
			_enum RespawnTimerStarts;
			byte_enum Classification;
			PAD24;
			TAG_PAD(int32, 10);
			real_euler_angles3d Orientation;
			tag_reference Collection; // itmc or vehc
		};

		struct scenario_starting_equipment
		{
			long Flags;
			_enum GameType[4];
			TAG_PAD(int32, 12);
			TagReference<'itmc'> ItemCollections[6];
			TAG_PAD(int32, 12);
		};

		struct scenario_bsp_switch_trigger_volume
		{
			int16 TriggerVolume;
			int16 SrcBsp, DstBsp;
			PAD16; PAD16; PAD16; PAD16;
		};

		struct scenario_decal
		{
			int16 DecalType;
			sbyte Yaw, Pitch;
			real_point3d Position;
		};

		struct scenario_detail_object_collection_palette_entry
		{
			TagReference<'dobc'> Definition;
			PAD64; PAD64; PAD64; PAD64;
		};


		struct scenario_cutscene_flag
		{
			PAD32;
			tag_string Name;
			real_point3d Position;
			real_euler_angles2d Facing;
		};

		struct scenario_cutscene_camera_point
		{
			word_flags Flags;
			_enum Type;
			tag_string Name;
			real_point3d Position;
			real_euler_angles3d Orientation;
			angle Unused;
		};

		struct s_scenario_cutscene_title
		{
			string_id Name;
			rectangle2d TextBounds;
			_enum Justification;
			_enum Font;
			rgb_color TextColor, ShadowColor;
			real FadeInTime, UpTime, FadeOutTime;
		};

		struct scenario_structure_bsp_reference
		{
			TAG_PAD(int32, 4);
			TagReference<'sbsp'> StructureBsp;
			TagReference<'ltmp'> Lightmap;
			PAD32;
			UNUSED_TYPE(real);
			PAD32;
			UNUSED_TYPE(real);
			UNUSED_TYPE(real);
			PAD64;
			word_flags Flags;
			PAD16;
			int16 DefaultSkyIndex; // scenario_sky_reference_block
			PAD16;
		};


		struct s_scenario_bsp_switch_transition_volume
		{
			int32 BspIndexKey; // properly a datum_index
			int16 TriggerVolumeIndex;
			PAD16;
		};

		struct s_scenario_level_data
		{
			TagReference<'unic'> LevelDescription;
			TagBlock<GameGlobals::s_campaign_ui_level_definition> CampaignLevelData;
			TagBlock<GameGlobals::s_multiplayer_ui_level_definition> Multiplayer;
		};



		struct group
		{
			tag_reference DontUse; // 0x0

			TagBlock<sky_reference_block> Skies; // 0x8

			_enum Type; // 0x10
			word_flags Flags; // 0x12

			PAD_TYPE(tag_block); // 0x14, child scenarios

			angle LocalNorth; // 0x1C

			TagBlock<TagGroups::predicted_resource> PredictedResources; // 0x20

			tag_block Functions; // 0x28
			tag_data EditorScenarioData; // 0x30
			tag_block Comments; // 0x38

			PAD_TYPE(tag_block); // 0x40, dont_use_me_scenario_environment_object_block

			TagBlock<object_names_block> ObjectNames; // 0x48

			TagBlock<s_scenario_scenery> Scenery; // 0x50
			TagBlock<object_palette_block> SceneryPalette; // 0x58

			TagBlock<s_scenario_biped> Bipeds; // 0x60
			TagBlock<object_palette_block> BipedPalette; // 0x68

			TagBlock<s_scenario_vehicle> Vehicles; // 0x70
			TagBlock<object_palette_block> VehiclePalette; // 0x78

			TagBlock<s_scenario_equipment> Equipment; // 0x80
			TagBlock<object_palette_block> EquipmentPalette; // 0x88

			TagBlock<s_scenario_weapon> Weapons; // 0x90
			TagBlock<object_palette_block> WeaponPalette; // 0x98

			TagBlock<scenario_device_group> DeviceGroups; // 0xA0

			TagBlock<s_scenario_machine> DeviceMachines; // 0xA8
			TagBlock<object_palette_block> DeviceMachinePalette; // 0xB0

			TagBlock<s_scenario_control> DeviceControls; // 0xB8
			TagBlock<object_palette_block> DeviceControlPalette; // 0xC0

			TagBlock<s_scenario_light_fixture> DeviceLightFixtures; // 0xC8
			TagBlock<object_palette_block> DeviceLightFixturePalette; // 0xD0

			TagBlock<s_scenario_sound_scenery> SoundScenery; // 0xD8
			TagBlock<object_palette_block> SoundSceneryPalette;  // 0xE0

			TagBlock<s_scenario_light> LightVolumes; // 0xE8
			TagBlock<object_palette_block> LightVolumesPalette; // 0xF0

			TagBlock<scenario_starting_profile> StartingProfiles; // 0xF8
			TagBlock<scenario_player> PlayerStartingLocations; // 0x100

			TagBlock<scenario_trigger_volume> TriggerVolumes; // 0x108

			TagBlock<recorded_animation_definition> RecordedAnimations; // 0x110

			TagBlock<scenario_netpoint> NetgameFlags; // 0x118
			TagBlock<scenario_netgame_equipment> NetgameEquipment; // 0x120

			TagBlock<scenario_starting_equipment> StartingEquipment; // 0x128

			TagBlock<scenario_bsp_switch_trigger_volume> BspSwitchTriggerVolumes; // 0x130

			TagBlock<scenario_decal> Decals; // 0x138
			TagBlock<tag_reference> DecalPalette; // 0x140

			TagBlock<scenario_detail_object_collection_palette_entry> DetailObjectCollectionPalette; // 0x148

			TagBlock<tag_reference> StylePalette; // 0x150
			TagBlock<squad_group_definition> SquadGroups; // 0x158
			TagBlock<squad_definition> Squads; // 0x160
			TagBlock<zone_definition> Zones; // 0x168
			TagBlock<ai_scene> MissionScenes; // 0x170

			TagBlock<TagReference<'char'>> CharacterPalette; // 0x178

			tag_block AiPathfindingData; // 0x180
			tag_block AiAnimationReferences; // 0x18C
			tag_block AiScriptReferences; // 0x198
			tag_block AiRecordingReferences; // 0x1A4
			tag_block AiConversations; // 0x1B0
			PAD_TYPE(tag_data); // 0x1BC, old script syntax data
			TagData<char> ScriptStringData; // 0x1D0
			TagBlock<hs_script> Scripts; // 0x1D4
			TagBlock<hs_global_internal> Globals; // 0x1E0
			TagBlock<tag_reference> References; // 0x1EC
			tag_block SourceFiles; // 0x1F8 // isn't used in cache files

			// TODO: all offsets after here need to be updated

			TagBlock<cs_script_data> ScriptingData; // 0x1D8

			TagBlock<scenario_cutscene_flag> CutsceneFlags; // 0x1E0
			TagBlock<scenario_cutscene_camera_point> CutsceneCameraPoints; // 0x1E8
			TagBlock<s_scenario_cutscene_title> CutsceneTitles; // 0x1F0

			TagReference<'unic'> CustomObjectNames; // 0x1F8
			TagReference<'unic'> IngameHelpText; // 0x200
			TagReference<'hmt '> HudMessages; // 0x208

			TagBlock<scenario_structure_bsp_reference> StructureBsps; // 0x210

			UNUSED_TYPE(tag_block); // 0x218, scenario_resources_block
			UNUSED_TYPE(tag_block); // 0x220, old_unused_strucure_physics_block
			TagBlock<s_hs_unit_seat_mapping> HsUnitSeats; // 0x228

			TagBlock<int16> KillVolumes; // 0x230, each element is a index to a scenario_trigger_volume

			TagBlock<Scripting::hs_syntax_node> HsSyntaxDatums; // 0x238

			tag_block AiOrders; // 0x240, struct orders_definition
			tag_block AiFunctions; // 0x248, struct trigger_definition

			tag_block BackgroundSoundPalette; // 0x250 stolen from halo1 bsp
			tag_block SoundEnvironmentPalette; // 0x258 stolen from halo1 bsp
			tag_block WeatherPalette; // 0x260 stolen from halo1 bsp

 			TAG_PAD(tag_block, 5); // 0x268
			tag_block ScenarioClusterData; // 0x290, struct s_scenario_cluster_data

			int32 UnknownArray[32]; // 0x298

			tag_block SpawnData; // 0x318, struct s_scenario_spawn_data
			TagReference<'sfx+'> SoundEffectCollection; // 0x320 unused

			TagBlock<s_scenario_crate> Crate; // 0x328
			TagBlock<object_palette_block> CratePalette; // 0x330
			TagReference<'gldf'> GlobalLighting; // 0x338
			tag_block AtmosphericFogPalette; // 0x340, struct s_scenario_atmospheric_fog_palette_entry
			tag_block PlanarFogPalette; // 0x348, struct s_scenario_planar_fog_palette_entry
			tag_block Flocks; // 0x350, struct flock_definition
			TagReference<'unic'> Subtitles; // 0x358

 			tag_block Decorators; // 0x360, class c_decorator_placement_definition
			TagBlock<s_scenario_creature> Creatures;
			TagBlock<object_palette_block> CreaturePalette;
			TagBlock<TagReference<'DECR'> > DecoratorsPalette; // 0x378
			TagBlock<s_scenario_bsp_switch_transition_volume> BspTransitionVolumes; // 0x380
			tag_block StructureBspLighting; // 0x388, struct s_scenario_structure_bsp_spherical_harmonic_lighting
			tag_block EditorFolders; // 0x390, struct s_scenario_editor_folder
			TagBlock<s_scenario_level_data> LevelData; // 0x398
			TagReference<'unic'> TerritoryLocationNames; // 0x3A0
			PAD64; // 0x3A8
			TagBlock<TagReference<'mdlg'> > MissionDialogue;// 0x3B0
			TagReference<'unic'> Objectives; // 0x3B8
			tag_block Interpolators; // 0x3C0 struct s_scenario_interpolator
			TagBlock<tag_reference> SharedReferences; // 0x3C8, struct hs_tag_reference
			tag_block ScreenEffectReferences; // 0x3D0, struct s_scenario_screen_effect_reference
 			TagBlock<datum_index> SimulationDefinitionTable; // 0x3D8, struct s_scenario_simulation_definition_table_element
		}; BOOST_STATIC_ASSERT( sizeof(group) == 992 );
	};
};