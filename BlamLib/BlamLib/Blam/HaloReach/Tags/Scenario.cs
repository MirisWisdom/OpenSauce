/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;
using H2 = BlamLib.Blam.Halo2.Tags;
using H3 = BlamLib.Blam.Halo3.Tags;

namespace BlamLib.Blam.HaloReach.Tags
{
	#region hs_scripts_block
	[TI.Definition(3, 24)]
	public class hs_scripts_block : Scripting.hs_scripts_block
	{
		public TI.Block<Blam.Halo3.Tags.hs_scripts_block.hs_scripts_parameters_block> Parameters;

		public hs_scripts_block() : base(5)
		{
			Add(Name = new TI.StringId());
			Add(ScriptType = new TI.Enum());
			Add(ReturnType = new TI.Enum());
			Add(RootExpressionIndex = new TI.LongInteger());
			Add(Parameters = new TI.Block<H3.hs_scripts_block.hs_scripts_parameters_block>(this, 0));
		}
	}
	#endregion

	#region cs_script_data_block
	[TI.Definition(1, 132)]
	public class cs_script_data_block : TI.Definition
	{
		#region cs_point_set_block
		[TI.Definition(3, 56)]
		public class cs_point_set_block : TI.Definition
		{
			#region cs_point_block
			[TI.Definition(3, 64)]
			public class cs_point_block : TI.Definition
			{
				public cs_point_block()
				{
					Add(/*name = */ new TI.String());
					Add(/*? = */ new TI.StringId());
					Add(/*position = */ new TI.RealPoint3D());
					Add(/*reference frame = */ new TI.ShortInteger());
					Add(TI.Pad.Word);
					Add(/*surface index = */ new TI.LongInteger());
					Add(/*facing direction = */ new TI.RealEulerAngles2D());
				}
			}
			#endregion

			public cs_point_set_block() : base(7)
			{
				Add(/*name = */ new TI.String());
				Add(/*points = */ new TI.Block<cs_point_block>(this, 20));
				Add(/*bsp index = */ new TI.BlockIndex()); // 1 scenario_structure_bsp_reference_block
				Add(/*manual reference frame = */ new TI.ShortInteger());
				Add(/*flags = */ new TI.Flags());
				Add(TI.BlockIndex.Word);
				Add(TI.Pad.Word); // TODO: I'm PRETTY sure this is just padding, only ever seen zeros
			}
		}
		#endregion

		public cs_script_data_block() : base(2)
		{
			Add(/*point sets = */ new TI.Block<cs_point_set_block>(this, 200));
			Add(new TI.Pad(120));
		}
	}
	#endregion

	#region scenario_cutscene_title_block
	[TI.Definition(3, 48)]
	public class scenario_cutscene_title_block : TI.Definition
	{
		public scenario_cutscene_title_block()
		{
			Add(/*name = */ new TI.StringId());
			Add(/*text bounds (on screen) = */ new TI.Rectangle2D());
			Add(new TI.UnknownPad(8)); // another Rect2D? If that's the case, this may actually be the on-screen field
			Add(/*justification = */ new TI.Enum());
			Add(/*font = */ new TI.Enum());
			Add(new TI.Enum());
			Add(TI.Pad.Word); // only ever seen this as zeros
			Add(/*text color = */ new TI.Color(TI.FieldType.RgbColor));
			Add(/*shadow color = */ new TI.Color(TI.FieldType.RgbColor));
			Add(/*fade in time [seconds] = */ new TI.Real());
			Add(/*up time [seconds] = */ new TI.Real());
			Add(/*fade out time [seconds] = */ new TI.Real());
		}
	}
	#endregion


	#region hs_unit_seat_block
	[TI.Definition(2, 12)]
	public class hs_unit_seat_block : TI.Definition
	{
		public hs_unit_seat_block()
		{
			Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger());
		}
	}
	#endregion

	#region scenario
	[TI.TagGroup((int)TagGroups.Enumerated.scnr, 5, 2156)]
	public class scenario_group : TI.Definition
	{
		#region Fields
		public TI.Enum Type;
		public TI.Flags Flags;

		#region scripting and cinematics
		public TI.Data HsStringData;
		public TI.Block<hs_scripts_block> HsScripts;
		public TI.Block<H3.hs_globals_block> HsGlobals;
		#endregion
		#endregion

		void version_construct_add_unnamed_array()
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
			Add(TI.UnknownPad.DWord);
			Add(/*map id = */ MapId.SkipField); // id used for mapinfo files
			Add(new TI.Skip(4));
			Add(new TI.UnknownPad(4 + 4));
			Add(/*LocalNorth =*/ new TI.Real(TI.FieldType.Angle));
			Add(new TI.UnknownPad(20));
			Add(/*SandboxBuget =*/ new TI.Real());
			Add(TI.UnknownPad.DWord);
			Add(/*performance throttle = */new TI.TagReference(this, TagGroups.gptd));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0xAC] StructureBsps
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x20] structure designs
				// tag reference [sddt]
				// tag reference [????]
			Add(TI.UnknownPad.ReferenceHalo3);
			Add(TI.UnknownPad.ReferenceHalo3);
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x30] Skies
				// tag reference [scen] (sky)
				// unknown 0xC bytes (tag block?)
				// tag reference [????]
				// short
				// short

			// TOOD: finish up from here
			Add(new TI.UnknownPad(0x3A0));

			#region scripting and cinematics
			Add(HsStringData = new TI.Data(this)); // 0x430, NOTE: stored in memory region 3
			Add(HsScripts = new TI.Block<hs_scripts_block>(this, 1024));
			Add(HsGlobals = new TI.Block<H3.hs_globals_block>(this, 256));
			Add(/*References =*/ new TI.Block<H3.hs_references_block>(this, 512));
			Add(/*SourceFiles =*/ new TI.Block<H2.hs_source_files_block>(this, 8));
			Add(/*ScriptingData =*/ new TI.Block<cs_script_data_block>(this, 1));
			Add(/*CutsceneFlags =*/ new TI.Block<H3.scenario_cutscene_flag_block>(this, 512));
			Add(/*CutsceneCameraPoints =*/ new TI.Block<H3.scenario_cutscene_camera_point_block>(this, 512));
			Add(/*CutsceneTitles =*/ new TI.Block<scenario_cutscene_title_block>(this, 128));
			// 0x4A4
			Add(/*CustomObjectNames =*/ new TI.TagReference(this, TagGroups.unic));
			Add(/*ChapterTitleText =*/ new TI.TagReference(this, TagGroups.unic));
			Add(/*ScenarioResources =*/ new TI.Block<H3.scenario_resources_block>(this, 1));
			Add(/*HsUnitSeats =*/ new TI.Block<hs_unit_seat_block>(this, 65536));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x4]
				// short
				// short
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			// 0x4F4
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x30] this is physics related ata
				// dword
				// tag block H3.mopp_code_block
				// dword
				// tag_reference?
				// real?
				// real
				// real
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(/*HsScriptDatums =*/ new TI.Block<H3.syntax_datum_block>(this, 0));
			#endregion
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] Orders
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] Triggers
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] BackgroundSoundPalette
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] SoundEnvironmentPalette
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] WeatherPalette
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x54] ScenarioClusterData
				// this is scenario_cluster_data_block but with an extra dword of data right after the tag reference
			version_construct_add_unnamed_array();
			Add(TI.UnknownPad.BlockHalo3); // SpawnData?
			// 0x604
			Add(/*SoundEffectCollection = */new TI.TagReference(this, TagGroups.sfx_));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0xD8] Crates
			Add(/*CratesPalette = */new TI.Block<H3.scenario_crate_palette_block>(this, 256));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x10] Flocks palette
				// tag reference
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] Flocks
			Add(/*Subtitles = */new TI.TagReference(this, TagGroups.unic));

			// Creatures & CreaturesPalette should be in one of these blocks
			Add(TI.UnknownPad.BlockHalo3);
			Add(TI.UnknownPad.BlockHalo3);
			Add(TI.UnknownPad.BlockHalo3);
			Add(/*EditorFolders = */new TI.Block<H3.g_scenario_editor_folder_block>(this, 32767));
			Add(/*TerritoryLocationNames = */new TI.TagReference(this, TagGroups.unic));
			Add(new TI.Pad(8));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] MissionDialogue
			Add(/*Objectives = */new TI.TagReference(this, TagGroups.unic));
			Add(/*? = */new TI.TagReference(this, TagGroups.sirp));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(/*Camere fx settings = */new TI.TagReference(this, TagGroups.cfxs));
			Add(/*? = */new TI.TagReference(this, TagGroups.sefc));
			Add(/*? = */new TI.TagReference(this, TagGroups.ssao));
			Add(/*sky parameters= */new TI.TagReference(this, TagGroups.skya));
			Add(/*atmosphere globals = */new TI.TagReference(this, TagGroups.atgf));
			Add(/*GlobalLighting = */new TI.TagReference(this, TagGroups.chmt));
			Add(/*lightmap = */new TI.TagReference(this, TagGroups.sLdT));
			Add(/*performance throttles = */new TI.TagReference(this, TagGroups.perf));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.ReferenceHalo3);
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.ReferenceHalo3);
			Add(TI.UnknownPad.ReferenceHalo3);
			Add(TI.UnknownPad.ReferenceHalo3);
		}
	};
	#endregion
}