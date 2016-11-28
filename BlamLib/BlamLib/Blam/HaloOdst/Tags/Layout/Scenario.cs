/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;
using H2 = BlamLib.Blam.Halo2.Tags;
using H3 = BlamLib.Blam.Halo3.Tags;

namespace BlamLib.Blam.HaloOdst.Tags
{
	#region scenario_arg_device_palette_block
	partial class scenario_arg_device_palette_block
	{
		public scenario_arg_device_palette_block()
		{
			Add(/*Name = */ new TI.TagReference(this, TagGroups.argd));
			Add(new TI.Pad(32));
		}
	};
	#endregion


	#region zone_block
	[TI.Definition(4, 60)]
	public class zone_block : TI.Definition
	{
		public zone_block() : base(4)
		{
			Add(/*name = */ new TI.String());
			Add(new TI.UnknownPad(4));
			Add(TI.UnknownPad.BlockHalo3);//Add(/*firing positions = */ new TI.Block<firing_positions_block>(this, 512));
			Add(TI.UnknownPad.BlockHalo3);//Add(/*areas = */ new TI.Block<areas_block>(this, 64));
		}
	}
	#endregion

	#region scenario
	partial class scenario_group
	{
		private void version_construct_add_unnamed_null_blocks()
		{
			#region g_null_block
			Add(/* = */ new TI.Block<TI.g_null_block>(this, 0));
			Add(/* = */ new TI.Block<TI.g_null_block>(this, 0));
			Add(/* = */ new TI.Block<TI.g_null_block>(this, 0));
			Add(/* = */ new TI.Block<TI.g_null_block>(this, 0));
			Add(/* = */ new TI.Block<TI.g_null_block>(this, 0));
			#endregion
		}
		private void version_construct_add_unnamed_array()
		{
			#region unknown
			Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger()); Add(/* = */ new TI.LongInteger());
			#endregion
		}
		public scenario_group()
		{
			Add(Type = new TI.Enum());
			Add(Flags = TI.Flags.Word);
			Add(/*map id = */ MapId.SkipField); // id used for mapinfo files
			Add(new TI.Skip(4));
			Add(SandboxBuget = new TI.Real());
			Add(StructureBsps = new TI.Block<H3.scenario_structure_bsp_reference_block>(this, 16));

			// ODST:
			Add(ScenarioPda = new BlamLib.TagInterface.TagReference(this, TagGroups.spda));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]

			Add(/*DontUse = */new TI.TagReference(this, Halo3.TagGroups.sbsp)); // Its a tag reference here, but IDK if its for THIS exactly...
			Add(Skies = new TI.Block<H3.scenario_sky_reference_block>(this, 32));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x2C], see Halo3
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x64], see Halo3
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x24], see Halo3
			Add(TI.UnknownPad.BlockHalo3); // tag block [0xC], see Halo3

			// ODST:
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x4], campaign players
				// string id [player model name] // player #<element_index> uses this player model

			Add(new TI.UnknownPad(0x44)); // unknown [0x44]
			#region scenario objects
			Add(ObjectNames = new TI.Block<H3.scenario_object_names_block>(this, 0)); // I'm sure the max is now over 640...
			Add(TI.UnknownPad.BlockHalo3); // tag block [0xB4] Scenery
			Add(SceneryPalette = new TI.Block<H3.scenario_scenery_palette_block>(this, 0)); // I'm sure the max is now over 256...
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x74] Bipeds
			Add(BipedsPalette = new TI.Block<H3.scenario_biped_palette_block>(this, 0)); // ''
			Add(TI.UnknownPad.BlockHalo3); // tag block [0xA8] Vehicles
			Add(VehiclePalette = new TI.Block<H3.scenario_vehicle_palette_block>(this, 0)); // ''
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x8C] Equipment
			Add(EquipmentPalette = new TI.Block<H3.scenario_equipment_palette_block>(this, 0)); // ''
			Add(TI.UnknownPad.BlockHalo3); // tag block [0xA8] Weapons
			Add(WeaponPalette = new TI.Block<H3.scenario_weapon_palette_block>(this, 0)); // ''
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x28] Device groups
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x88] Machines
			Add(MachinePalette = new TI.Block<H3.scenario_machine_palette_block>(this, 0)); // ''
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x78] Terminals
			Add(TerminalPalette = new TI.Block<H3.scenario_terminal_palette_block>(this, 0)); // ''

			// ODST:
			Add(TI.UnknownPad.BlockHalo3); // tag block [0xB8] Arg Devices
			Add(ArgDevicesPalette = new TI.Block<scenario_arg_device_palette_block>(this, 0)); // ''

			Add(TI.UnknownPad.BlockHalo3); // tag block [0x7C] Controls
			Add(ControlPalette = new TI.Block<H3.scenario_control_palette_block>(this, 0)); // ''
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x70] Sound Scenery
			Add(SoundSceneryPalette = new TI.Block<H3.scenario_sound_scenery_palette_block>(this, 0)); // ''
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x84] Giants
			Add(GiantsPalette = new TI.Block<H3.scenario_giant_palette_block>(this, 0)); // ''
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x54] Effect Scenery
			Add(EffectSceneryPalette = new TI.Block<H3.scenario_effect_scenery_palette_block>(this, 0)); // ''
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x70] Light Volumes
			Add(LightVolumesPalette = new TI.Block<H3.scenario_light_palette_block>(this, 0)); // ''
			#endregion
			Add(/*Sandbox vehicles = */new TI.Block<H3.scenario_sandbox_vehicle_settings_block>(this, 0)); // 0x228
			Add(/*Sandbox weapons = */new TI.Block<H3.scenario_sandbox_weapon_settings_block>(this, 0));
			Add(/*Sandbox equipment = */new TI.Block<H3.scenario_sandbox_equipment_settings_block>(this, 0));
			Add(/*Sandbox scenery = */new TI.Block<H3.scenario_sandbox_scenery_settings_block>(this, 0));
			Add(/*Sandbox teleporters = */new TI.Block<H3.scenario_sandbox_teleporter_settings_block>(this, 0));
			Add(/*Sandbox netgame flags = */new TI.Block<H3.scenario_sandbox_netpoint_settings_block>(this, 0));
			Add(/*Sandbox spawns = */new TI.Block<H3.scenario_sandbox_spawn_settings_block>(this, 0));
			Add(TI.UnknownPad.BlockHalo3); // 0x27C tag block [0xC], see Halo3
			Add(/*PlayerStartingProfile = */new TI.Block<H3.scenario_group.scenario_profiles_block>(this, 256));
			Add(/*PlayerStartingLocations = */new TI.Block<H3.scenario_group.scenario_players_block>(this, 256));
			Add(/*KillTriggerVolumes = */new TI.Block<H3.scenario_trigger_volume_block>(this, 256)); // 0x260
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] // recorded animations?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x10], see Halo3
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x24] scenario_decal_systems_block 65536
			Add(/*DecalSystems = */new TI.Block<H3.scenario_decal_system_palette_block>(this, 128));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(/*SquadGroups = */new TI.Block<H3.squad_groups_block>(this, 100));
			Add(TI.UnknownPad.BlockHalo3); // squads, see Halo3
			Add(/*Zones = */new TI.Block<zone_block>(this, 128));

			// ODST: I think this new block was added AFTER what I'm thinking may be 'Mission Scenes'
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] // Mission Scenes?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]

			Add(/*CharacterPalette = */new TI.Block<H3.character_palette_block>(this, 64));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x6C], see Halo3
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?], see Halo3
			#region scripting and cinematics
			Add(HsStringData = new TI.Data(this)); // 0x42C
			Add(HsScripts = new TI.Block<H3.hs_scripts_block>(this, 1024));
			Add(HsGlobals = new TI.Block<H3.hs_globals_block>(this, 256));
			Add(References = new TI.Block<H3.hs_references_block>(this, 512));
			Add(SourceFiles = new TI.Block<H2.hs_source_files_block>(this, 8));
			Add(ScriptingData = new TI.Block<H2.cs_script_data_block>(this, 1));
			Add(CutsceneFlags = new TI.Block<H3.scenario_cutscene_flag_block>(this, 512));
			Add(CutsceneCameraPoints = new TI.Block<H3.scenario_cutscene_camera_point_block>(this, 512));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] Add(CutsceneTitles = new TI.Block<scenario_cutscene_title_block>(this, 128));
			Add(CustomObjectNames = new TI.TagReference(this, Halo3.TagGroups.unic));
			Add(ChapterTitleText = new TI.TagReference(this, Halo3.TagGroups.unic));
			Add(ScenarioResources = new TI.Block<H3.scenario_resources_block>(this, 1));
			Add(HsUnitSeats = new TI.Block<H3.hs_unit_seat_block>(this, 65536));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x2]
			Add(ScenarioKillTriggers = new TI.Block<H3.scenario_kill_trigger_volumes_block>(this, 256)); // 0x498
			Add(HsScriptDatums = new TI.Block<H3.syntax_datum_block>(this, 36864));
			#endregion
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] Orders
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] Triggers
			Add(/*BackgroundSoundPalette = */new TI.Block<H3.structure_bsp_background_sound_palette_block>(this, 64));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] SoundEnvironmentPalette
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] WeatherPalette
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			version_construct_add_unnamed_null_blocks();
			Add(/*ScenarioClusterData = */new TI.Block<H3.scenario_cluster_data_block>(this, 16));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			version_construct_add_unnamed_array();
			// 0x618


			// crate: 0x634, 0x640, 0xB0
			// creature: 0x674, 0x680, 0x54
		}
	};
	#endregion
};