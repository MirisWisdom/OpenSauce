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
	[TI.Definition(1, 48)]
	public sealed partial class scenario_arg_device_palette_block : TI.Definition
	{
		#region Fields
		#endregion
	}
	#endregion

	#region scenario
	[TI.TagGroup((int)TagGroups.Enumerated.scnr, 4, -1)]
	public sealed partial class scenario_group : TI.Definition
	{
		#region Fields
		public TI.Enum Type;
		public TI.Flags Flags;
		public TI.Real SandboxBuget;
		public TI.Block<H3.scenario_structure_bsp_reference_block> StructureBsps;
		public TI.Block<H3.scenario_sky_reference_block> Skies;
		public TI.TagReference ScenarioPda;

		#region scenario objects
		public TI.Block<H3.scenario_object_names_block> ObjectNames;
		//public TI.Block<H3.scenario_scenery_block> Scenery;
		public TI.Block<H3.scenario_scenery_palette_block> SceneryPalette;
		//public TI.Block<H3.scenario_biped_block> Bipeds;
		public TI.Block<H3.scenario_biped_palette_block> BipedsPalette;
		//public TI.Block<H3.scenario_vehicle_block> Vehicles;
		public TI.Block<H3.scenario_vehicle_palette_block> VehiclePalette;
		//public TI.Block<H3.scenario_equipment_block> Equipment;
		public TI.Block<H3.scenario_equipment_palette_block> EquipmentPalette;
		//public TI.Block<H3.scenario_weapon_block> Weapons;
		public TI.Block<H3.scenario_weapon_palette_block> WeaponPalette;
		//public TI.Block<H3.device_group_block> DeviceGroups;
		//public TI.Block<H3.scenario_machine_block> Machines;
		public TI.Block<H3.scenario_machine_palette_block> MachinePalette;
		//public TI.Block<H3.scenario_terminal_block> Terminals;
		public TI.Block<H3.scenario_terminal_palette_block> TerminalPalette;

		//public TI.Block<scenario_arg_device_block> ArgDevices;
		public TI.Block<scenario_arg_device_palette_block> ArgDevicesPalette;

		//public TI.Block<H3.scenario_control_block> Controls;
		public TI.Block<H3.scenario_control_palette_block> ControlPalette;
		//public TI.Block<H3.scenario_sound_scenery_block> SoundScenery;
		public TI.Block<H3.scenario_sound_scenery_palette_block> SoundSceneryPalette;
		//public TI.Block<H3.scenario_giant_block> Giants;
		public TI.Block<H3.scenario_giant_palette_block> GiantsPalette;
		//public TI.Block<H3.scenario_effect_scenery_block> EffectScenery;
		public TI.Block<H3.scenario_effect_scenery_palette_block> EffectSceneryPalette;
		//public TI.Block<H3.scenario_light_block> LightVolumes;
		public TI.Block<H3.scenario_light_palette_block> LightVolumesPalette;
		#endregion

		#region scripting and cinematics
		public TI.Data HsStringData;
		public TI.Block<H3.hs_scripts_block> HsScripts;
		public TI.Block<H3.hs_globals_block> HsGlobals;
		public TI.Block<H3.hs_references_block> References;
		public TI.Block<H2.hs_source_files_block> SourceFiles;
		public TI.Block<H2.cs_script_data_block> ScriptingData;
		public TI.Block<H3.scenario_cutscene_flag_block> CutsceneFlags;
		public TI.Block<H3.scenario_cutscene_camera_point_block> CutsceneCameraPoints;
		//public TI.Block<H3.scenario_cutscene_title_block> CutsceneTitles;
		public TI.TagReference CustomObjectNames;
		public TI.TagReference ChapterTitleText;
		#endregion
		public TI.Block<H3.scenario_resources_block> ScenarioResources;
		public TI.Block<H3.hs_unit_seat_block> HsUnitSeats;
		public TI.Block<H3.scenario_kill_trigger_volumes_block> ScenarioKillTriggers;
		public TI.Block<H3.syntax_datum_block> HsScriptDatums;

		#endregion
	};
	#endregion
};