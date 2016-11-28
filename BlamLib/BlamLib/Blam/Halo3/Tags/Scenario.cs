/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;
using H2 = BlamLib.Blam.Halo2.Tags;

namespace BlamLib.Blam.Halo3.Tags
{
	#region scenario_structure_bsp_reference_block
	[TI.Definition(1, 108)]
	public class scenario_structure_bsp_reference_block : TI.Definition
	{
		#region Fields
		public TI.TagReference StructureBsp;
		public TI.TagReference Design;
		public TI.TagReference LightingInfo;
		public TI.TagReference Cubemap;
		public TI.TagReference Wind;
		#endregion

		#region Ctor
		public scenario_structure_bsp_reference_block()
		{
			Add(StructureBsp = new TI.TagReference(this, TagGroups.sbsp));
			Add(Design = new TI.TagReference(this, TagGroups.sddt));
			Add(LightingInfo = new TI.TagReference(this, TagGroups.stli));
			Add(new TI.Skip(
				4 + // may be a index
				16 +
				1 +
				1 + // index to 2nd tag block in scenario
				2 + // index
				4
				));
			Add(Cubemap = new TI.TagReference(this, TagGroups.bitm));
			Add(Wind = new TI.TagReference(this, TagGroups.wind));
			Add(new TI.Pad(4));
		}
		#endregion
	}
	#endregion

	#region scenario_sky_reference_block
	[TI.Definition(1, 20)]
	public class scenario_sky_reference_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_sky_reference_block()
		{
			Add(/*Sky = */ new TI.TagReference(this, TagGroups.scen));
			Add(new TI.BlockIndex());
			Add(new TI.BlockIndex());
		}
		#endregion
	}
	#endregion

	#region scenario_object_names_block
	[TI.Definition(1, 36)]
	public class scenario_object_names_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_object_names_block()
		{
			Add(/*Name = */ new TI.String());
			Add(/*Type = */ new TI.BlockIndex()); // 1
			Add(/*Index = */ new TI.BlockIndex()); // 2
		}
		#endregion
	}
	#endregion



	#region scenario_scenery_palette_block
	[TI.Definition(1, 48)]
	public class scenario_scenery_palette_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_scenery_palette_block()
		{
			Add(/*Name = */ new TI.TagReference(this, TagGroups.scen));
			Add(new TI.Pad(32));
		}
		#endregion
	}
	#endregion

	#region scenario_biped_palette_block
	[TI.Definition(1, 48)]
	public class scenario_biped_palette_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_biped_palette_block()
		{
			Add(/*Name = */ new TI.TagReference(this, TagGroups.bipd));
			Add(new TI.Pad(32));
		}
		#endregion
	}
	#endregion

	#region scenario_vehicle_palette_block
	[TI.Definition(1, 48)]
	public class scenario_vehicle_palette_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_vehicle_palette_block()
		{
			Add(/*Name = */ new TI.TagReference(this, TagGroups.vehi));
			Add(new TI.Pad(32));
		}
		#endregion
	}
	#endregion

	#region scenario_equipment_palette_block
	[TI.Definition(1, 48)]
	public class scenario_equipment_palette_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_equipment_palette_block()
		{
			Add(/*Name = */ new TI.TagReference(this, TagGroups.eqip));
			Add(new TI.Pad(32));
		}
		#endregion
	}
	#endregion

	#region scenario_weapon_palette_block
	[TI.Definition(1, 48)]
	public class scenario_weapon_palette_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_weapon_palette_block()
		{
			Add(/*Name = */ new TI.TagReference(this, TagGroups.weap));
			Add(new TI.Pad(32));
		}
		#endregion
	}
	#endregion

	#region scenario_machine_palette_block
	[TI.Definition(1, 48)]
	public class scenario_machine_palette_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_machine_palette_block()
		{
			Add(/*Name = */ new TI.TagReference(this, TagGroups.mach));
			Add(new TI.Pad(32));
		}
		#endregion
	}
	#endregion

	#region scenario_terminal_palette_block
	[TI.Definition(1, 48)]
	public class scenario_terminal_palette_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_terminal_palette_block()
		{
			Add(/*Name = */ new TI.TagReference(this, TagGroups.term));
			Add(new TI.Pad(32));
		}
		#endregion
	}
	#endregion

	#region scenario_control_palette_block
	[TI.Definition(1, 48)]
	public class scenario_control_palette_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_control_palette_block()
		{
			Add(/*Name = */ new TI.TagReference(this, TagGroups.ctrl));
			Add(new TI.Pad(32));
		}
		#endregion
	}
	#endregion

	#region scenario_sound_scenery_palette_block
	[TI.Definition(1, 48)]
	public class scenario_sound_scenery_palette_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_sound_scenery_palette_block()
		{
			Add(/*Name = */ new TI.TagReference(this, TagGroups.ssce));
			Add(new TI.Pad(32));
		}
		#endregion
	}
	#endregion

	#region scenario_giant_palette_block
	[TI.Definition(1, 48)]
	public class scenario_giant_palette_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_giant_palette_block()
		{
			Add(/*Name = */ new TI.TagReference(this, TagGroups.gint));
			Add(new TI.Pad(32));
		}
		#endregion
	}
	#endregion

	#region scenario_effect_scenery_palette_block
	[TI.Definition(1, 48)]
	public class scenario_effect_scenery_palette_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_effect_scenery_palette_block()
		{
			Add(/*Name = */ new TI.TagReference(this, TagGroups.efsc));
			Add(new TI.Pad(32));
		}
		#endregion
	}
	#endregion

	#region scenario_light_palette_block
	[TI.Definition(1, 48)]
	public class scenario_light_palette_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_light_palette_block()
		{
			Add(/*Name = */ new TI.TagReference(this, TagGroups.ligh));
			Add(new TI.Pad(32));
		}
		#endregion
	}
	#endregion


	#region scenario_sandbox_vehicle_settings_block
	[TI.Definition(1, 48)]
	public class scenario_sandbox_vehicle_settings_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_sandbox_vehicle_settings_block()
		{
			Add(/*Type = */ new TI.TagReference(this, TagGroups.unit));
			Add(/*Name = */ new TI.StringId());
			Add(/*Max Count = */ new TI.LongInteger());
			Add(/*Cost = */ new TI.Real());
			Add(new TI.Pad(20));
		}
		#endregion
	}
	#endregion

	#region scenario_sandbox_weapon_settings_block
	[TI.Definition(1, 48)]
	public class scenario_sandbox_weapon_settings_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_sandbox_weapon_settings_block()
		{
			Add(/*Type = */ new TI.TagReference(this, TagGroups.obje));
			Add(/*Name = */ new TI.StringId());
			Add(/*Max Count = */ new TI.LongInteger());
			Add(/*Cost = */ new TI.Real());
			Add(new TI.Pad(20));
		}
		#endregion
	}
	#endregion

	#region scenario_sandbox_equipment_settings_block
	[TI.Definition(1, 48)]
	public class scenario_sandbox_equipment_settings_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_sandbox_equipment_settings_block()
		{
			Add(/*Type = */ new TI.TagReference(this, TagGroups.item));
			Add(/*Name = */ new TI.StringId());
			Add(/*Max Count = */ new TI.LongInteger());
			Add(/*Cost = */ new TI.Real());
			Add(new TI.Pad(20));
		}
		#endregion
	}
	#endregion

	#region scenario_sandbox_scenery_settings_block
	[TI.Definition(1, 48)]
	public class scenario_sandbox_scenery_settings_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_sandbox_scenery_settings_block()
		{
			Add(/*Type = */ new TI.TagReference(this, TagGroups.obje));
			Add(/*Name = */ new TI.StringId());
			Add(/*Max Count = */ new TI.LongInteger());
			Add(/*Cost = */ new TI.Real());
			Add(new TI.Pad(20));
		}
		#endregion
	}
	#endregion

	#region scenario_sandbox_teleporter_settings_block
	[TI.Definition(1, 48)]
	public class scenario_sandbox_teleporter_settings_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_sandbox_teleporter_settings_block()
		{
			Add(/*Type = */ new TI.TagReference(this, TagGroups.obje));
			Add(/*Name = */ new TI.StringId());
			Add(/*Max Count = */ new TI.LongInteger());
			Add(/*Cost = */ new TI.Real());
			Add(new TI.Pad(20));
		}
		#endregion
	}
	#endregion

	#region scenario_sandbox_netpoint_settings_block
	[TI.Definition(1, 48)]
	public class scenario_sandbox_netpoint_settings_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_sandbox_netpoint_settings_block()
		{
			Add(/*Type = */ new TI.TagReference(this, TagGroups.obje));
			Add(/*Name = */ new TI.StringId());
			Add(/*Max Count = */ new TI.LongInteger());
			Add(/*Cost = */ new TI.Real());
			Add(new TI.Pad(20));
		}
		#endregion
	}
	#endregion

	#region scenario_sandbox_spawn_settings_block
	[TI.Definition(1, 48)]
	public class scenario_sandbox_spawn_settings_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_sandbox_spawn_settings_block()
		{
			Add(/*Type = */ new TI.TagReference(this, TagGroups.obje));
			Add(/*Name = */ new TI.StringId());
			Add(/*Max Count = */ new TI.LongInteger());
			Add(/*Cost = */ new TI.Real());
			Add(new TI.Pad(20));
		}
		#endregion
	}
	#endregion


	#region scenario_trigger_volume_block
	[TI.Definition(2, 124)] // used to have documented as 68
	public class scenario_trigger_volume_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_trigger_volume_block()
		{
			Add(/*Name = */ new TI.StringId());
			Add(/*Object Name = */ new TI.BlockIndex()); // 1 scenario_object_names_block
			Add(new TI.Skip(2));
			Add(/*Node Name = */ new TI.StringId());

			Add(/* = */ new TI.Real());
			Add(/* = */ new TI.Real());
			Add(/* = */ new TI.Real());
			Add(/* = */ new TI.Real());
			Add(/* = */ new TI.Real());
			Add(/* = */ new TI.Real());

			//Add(/*Position = */ new TI.RealPoint3D());
			Add(/* = */ new TI.Real());
			Add(/* = */ new TI.Real());
			Add(/* = */ new TI.Real());

			//Add(/*Extents = */ new TI.RealPoint3D());
			Add(/* = */ new TI.Real());
			Add(/* = */ new TI.Real());
			Add(/* = */ new TI.Real());
			Add(/* = */ new TI.Real());
			Add(new TI.UnknownPad(4));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x14]
				// real
				// real
				// real
				// real?
				// real?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x80]
				// real[32]
			Add(/* = */ new TI.Real());
			Add(/* = */ new TI.Real());
			Add(/* = */ new TI.Real());
			Add(/* = */ new TI.Real());
			Add(/* = */ new TI.Real());
			Add(/* = */ new TI.Real());

			Add(new TI.Real()); // Halo2 had this has Pad(4)
			Add(/*Kill Trigger Volume = */ new TI.BlockIndex()); // 1 scenario_kill_trigger_volumes_block
			Add(new TI.ShortInteger()); // Halo2 had this has Pad(2)
		}
		#endregion
	}
	#endregion


	#region scenario_decal_system_palette_block
	[TI.Definition(1, 16)]
	public class scenario_decal_system_palette_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_decal_system_palette_block()
		{
			Add(/*Reference = */ new TI.TagReference(this, TagGroups.decs));
		}
		#endregion
	}
	#endregion


	#region squad_groups_block
	[TI.Definition(2, 40)]
	public class squad_groups_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public squad_groups_block() : base(5)
		{
			Add(/*name = */ new TI.String());
			Add(/*parent = */ new TI.BlockIndex()); // 1 squad_groups_block
			Add(/*? = */ new TI.BlockIndex());
			Add(TI.UnknownPad.Word);
			Add(/*initial orders? = */ new TI.BlockIndex()); // 1 orders_block
		}
		#endregion
	}
	#endregion

	// TODO:
	#region squads_block
	[TI.Definition(4, 104)]
	public class squads_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public squads_block() : base(24)
		{
// 			Add(/*name = */ new TI.String());
// 			Add(/*flags = */ new TI.Flags());
// 			Add(/*team = */ new TI.Enum());
// 			Add(/*parent = */ new TI.BlockIndex()); // 1 squad_groups_block
// 			Add(/*squad delay time = */ new TI.Real());
// 			Add(/*normal diff count = */ new TI.ShortInteger());
// 			Add(/*insane diff count = */ new TI.ShortInteger());
// 			Add(/*major upgrade = */ new TI.Enum());
// 			Add(new TI.Pad(2));
// 			Add(new TI.UselessPad(12));
// 			Add(/*vehicle type = */ new TI.BlockIndex()); // 1 scenario_vehicle_palette_block
// 			Add(/*character type = */ new TI.BlockIndex()); // 1 character_palette_block
// 			Add(/*initial zone = */ new TI.BlockIndex()); // 1 zone_block
// 			Add(new TI.Pad(2));
// 			Add(/*initial weapon = */ new TI.BlockIndex()); // 1 scenario_weapon_palette_block
// 			Add(/*initial secondary weapon = */ new TI.BlockIndex()); // 1 scenario_weapon_palette_block
// 			Add(/*grenade type = */ new TI.Enum());
// 			Add(/*initial order = */ new TI.BlockIndex()); // 1 orders_block
// 			Add(/*vehicle variant = */ new TI.StringId());
// 			Add(new TI.UselessPad(8));
// 			Add(/*starting locations = */ new TI.Block<actor_starting_locations_block>(this, 32));
// 			Add(/*Placement script = */ new TI.String());
// 			Add(new TI.Skip(2));
// 			Add(new TI.Pad(2));
		}
		#endregion
	}
	#endregion

	#region zone_block
	[TI.Definition(3, 64)]
	public class zone_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public zone_block() : base(6)
		{
			Add(/*name = */ new TI.String());
			Add(/*flags = */ new TI.Flags());
			Add(/*manual bsp = */ new TI.BlockIndex()); // 1 scenario_structure_bsp_reference_block
			Add(new TI.Pad(2));
			Add(TI.UnknownPad.BlockHalo3);//Add(/*firing positions = */ new TI.Block<firing_positions_block>(this, 512));
			Add(TI.UnknownPad.BlockHalo3);//Add(/*areas = */ new TI.Block<areas_block>(this, 64));
		}
		#endregion
	}
	#endregion

	// ai_scene_block?

	#region character_palette_block
	[TI.Definition(1, 16)]
	public class character_palette_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public character_palette_block() : base(1)
		{
			Add(/*reference = */ new TI.TagReference(this, TagGroups.char_));
		}
		#endregion
	}
	#endregion


	#region hs_scripts_block
	[TI.Definition(2, 52)]
	public class hs_scripts_block : Scripting.hs_scripts_block
	{
		#region hs_scripts_parameters_block
		[TI.Definition(1, 36)]
		public class hs_scripts_parameters_block : hs_scripts_parameters_base
		{
			public hs_scripts_parameters_block() : base(3)
			{
				Add(Name = new TI.String());
				Add(Type =  new TI.Enum());
				Add(TI.Pad.Word);
			}
		}
		#endregion

		public TI.Block<hs_scripts_parameters_block> Parameters;

		public hs_scripts_block() : base(5)
		{
			Add(Name = new TI.String());
			Add(ScriptType = new TI.Enum());
			Add(ReturnType = new TI.Enum());
			Add(RootExpressionIndex = new TI.LongInteger());
			Add(Parameters = new TI.Block<hs_scripts_parameters_block>(this, 0));
		}

		public override BlamLib.TagInterface.IBlock GetParametersBlock() { return Parameters; }
	}
	#endregion

	#region hs_globals_block
	[TI.Definition(1, 40)]
	public class hs_globals_block : Scripting.hs_globals_block
	{
		public hs_globals_block() : base(4)
		{
			Add(Name = new TI.String());
			Add(Type = new TI.Enum());
			Add(TI.Pad.Word);
			Add(InitExpressionIndex = new TI.LongInteger());
		}
	}
	#endregion

	#region hs_references_block
	[TI.Definition(1, 16)]
	public class hs_references_block : TI.Definition
	{
		public hs_references_block() : base(1)
		{
			Add(/*reference = */ new TI.TagReference(this));
		}
	}
	#endregion

	#region scenario_cutscene_flag_block
	[TI.Definition(2, 28)]
	public class scenario_cutscene_flag_block : TI.Definition
	{
		public scenario_cutscene_flag_block() : base(4)
		{
			Add(new TI.Pad(4));
			Add(/*Name = */ new TI.StringId());
			Add(/*Position = */ new TI.RealPoint3D());
			Add(/*Facing = */ new TI.RealEulerAngles2D());
		}
	}
	#endregion

	#region scenario_cutscene_camera_point_block
	[TI.Definition(2, 64)]
	public class scenario_cutscene_camera_point_block : TI.Definition
	{
		public scenario_cutscene_camera_point_block() : base(8)
		{
			Add(/*Flags = */ TI.Flags.Word);
			Add(/*Type = */ new TI.Enum());
			Add(/*Name = */ new TI.String());
			Add(TI.Pad.DWord); // I've only ever seen this as zero
			Add(/*Position = */ new TI.RealPoint3D());
			Add(/*Orientation = */ new TI.RealEulerAngles3D());
		}
	}
	#endregion

	#region scenario_cutscene_title_block
	[TI.Definition(2, 40)]
	public class scenario_cutscene_title_block : TI.Definition
	{
		public scenario_cutscene_title_block()
		{
			Add(/*name = */ new TI.StringId());
			Add(/*text bounds (on screen) = */ new TI.Rectangle2D());
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


	#region scenario_resources_block
	[TI.Definition(2, 36)]
	public class scenario_resources_block : TI.Definition
	{
		#region scenario_resource_reference_block
		[TI.Definition(1, 16)]
		public class scenario_resource_reference_block : TI.Definition
		{
			public scenario_resource_reference_block()
			{
				Add(/*Reference = */ new TI.TagReference(this));
			}
		}
		#endregion

		#region scenario_hs_source_reference_block
		[TI.Definition(1, 16)]
		public class scenario_hs_source_reference_block : TI.Definition
		{
			public scenario_hs_source_reference_block()
			{
				Add(/*Reference = */ new TI.TagReference(this, TagGroups.srhscf));
			}
		}
		#endregion

		#region scenario_ai_resource_reference_block
		[TI.Definition(1, 16)]
		public class scenario_ai_resource_reference_block : TI.Definition
		{
			public scenario_ai_resource_reference_block()
			{
				Add(/*Reference = */ new TI.TagReference(this, TagGroups.srai));
			}
		}
		#endregion

		public scenario_resources_block()
		{
			Add(TI.Pad.DWord);
			Add(/*Script Source = */ new TI.Block<scenario_hs_source_reference_block>(this, 8));
			Add(/*References = */ new TI.Block<scenario_resource_reference_block>(this, 16));
			Add(/*AI Resources = */ new TI.Block<scenario_ai_resource_reference_block>(this, 2));
		}
	}
	#endregion

	#region hs_unit_seat_block
	[TI.Definition(1, 8)]
	public class hs_unit_seat_block : TI.Definition
	{
		public hs_unit_seat_block()
		{
			Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger());
		}
	}
	#endregion

	#region scenario_kill_trigger_volumes_block
	[TI.Definition(1, 2)]
	public class scenario_kill_trigger_volumes_block : TI.Definition
	{
		public scenario_kill_trigger_volumes_block()
		{
			Add(/*Trigger Volume = */ new TI.BlockIndex()); // 1 scenario_trigger_volume_block
		}
	}
	#endregion

	#region syntax_datum_block
	[TI.Definition(1, 24)]
	public class syntax_datum_block : Scripting.hs_syntax_datum_block
	{
		#region Ctor
		public syntax_datum_block() : base(8)
		{
			Add(DatumHeader = new TI.ShortInteger());
			Add(TypeUnion/*Script Index/Function Index/Constant Type Union*/= new TI.ShortInteger());
			Add(Type = new TI.ShortInteger());
			Add(Flags = new TI.ShortInteger());
			Add(NextNodeIndex = new TI.LongInteger());
			Add(Pointer = new TI.LongInteger());
			Add(Data = new TI.LongInteger());
			Add(LineNumber = new TI.ShortInteger());
			Add(LineNumberPadding = new TI.ShortInteger());
		}
		#endregion
	}
	#endregion


	#region structure_bsp_background_sound_palette_block
	[TI.Definition(2, 84)]
	public class structure_bsp_background_sound_palette_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public structure_bsp_background_sound_palette_block()
		{
			Add(/*Name = */ new TI.String());
			Add(/*Sound Environment = */ new TI.TagReference(this, TagGroups.snde));
			Add(/*Cutoff Distance = */ new TI.Real());
			Add(/*Interpolation Speed = */ new TI.Real());

			Add(/*Background Sound = */ new TI.TagReference(this, TagGroups.lsnd));
			Add(/*Inside Cluster Sound = */ new TI.TagReference(this, TagGroups.lsnd));
			Add(/*Cutoff Distance = */ new TI.Real());
			Add(/*Scale Flags = */ new TI.Flags());
			Add(/*Interior Scale = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*Portal Scale = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*Exterior Scale = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*Interpolation Speed = */ new TI.Real());
		}
		#endregion
	}
	#endregion

	#region scenario_cluster_data_block
	[TI.Definition(2, 80)]
	public class scenario_cluster_data_block : TI.Definition
	{
		#region scenario_cluster_background_sounds_block
		[TI.Definition(1, 4)]
		public class scenario_cluster_background_sounds_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_cluster_background_sounds_block()
			{
				Add(/*Type = */ new TI.BlockIndex()); // 1 structure_bsp_background_sound_palette_block
				Add(new TI.Pad(2));
			}
			#endregion
		}
		#endregion

		#region scenario_cluster_sound_environments_block
		[TI.Definition(1, 4)]
		public class scenario_cluster_sound_environments_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_cluster_sound_environments_block()
			{
				Add(/*Type = */ new TI.BlockIndex()); // 1 structure_bsp_sound_environment_palette_block
				Add(new TI.Pad(2));
			}
			#endregion
		}
		#endregion

		#region scenario_cluster_points_block
		[TI.Definition(1, 12)]
		public class scenario_cluster_points_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_cluster_points_block()
			{
				Add(/*Centroid = */ new TI.RealPoint3D());
			}
			#endregion
		}
		#endregion

		#region scenario_cluster_weather_properties_block
		[TI.Definition(1, 4)]
		public class scenario_cluster_weather_properties_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_cluster_weather_properties_block()
			{
				Add(/*Type = */ new TI.BlockIndex()); // 1 structure_bsp_weather_palette_block
				Add(new TI.Pad(2));
			}
			#endregion
		}
		#endregion

		#region scenario_cluster_atmospheric_fog_properties_block
		[TI.Definition(1, 4)]
		public class scenario_cluster_atmospheric_fog_properties_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_cluster_atmospheric_fog_properties_block()
			{
				Add(/*Type = */ new TI.BlockIndex()); // 1 scenario_atmospheric_fog_palette
				Add(new TI.Pad(2));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public scenario_cluster_data_block()
		{
			Add(/*BSP = */ new TI.TagReference(this, TagGroups.sbsp));
			Add(/*Background Sounds = */ new TI.Block<scenario_cluster_background_sounds_block>(this, 512));
			Add(/*Sound Environments = */ new TI.Block<scenario_cluster_sound_environments_block>(this, 512));
			Add(/*Cluster Centroids = */ new TI.Block<scenario_cluster_points_block>(this, 512));
			Add(/*BSP Checksum = */ new TI.LongInteger());
			Add(/*Weather Properties = */ new TI.Block<scenario_cluster_weather_properties_block>(this, 512));
			Add(/*Atmospheric Fog Properties = */ new TI.Block<scenario_cluster_atmospheric_fog_properties_block>(this, 512));
		}
		#endregion
	}
	#endregion

	#region static_spawn_zone_data_struct
	[TI.Struct((int)StructGroups.Enumerated.sszd, 1, 16)]
	public class static_spawn_zone_data_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public static_spawn_zone_data_struct()
		{
			Add(/*Name = */ new TI.StringId());
			Add(/*Relevant Team = */ new TI.Flags());
			Add(/*Relevant Games = */ new TI.Flags());
			Add(/*Flags = */ new TI.Flags());
		}
		#endregion
	}
	#endregion

	#region scenario_crate_palette_block
	[TI.Definition(1, 48)]
	public class scenario_crate_palette_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_crate_palette_block()
		{
			Add(/*Name = */ new TI.TagReference(this, TagGroups.bloc));
			Add(new TI.Pad(32));
		}
		#endregion
	}
	#endregion

	#region scenario_creature_palette_block
	[TI.Definition(1, 48)]
	public class scenario_creature_palette_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_creature_palette_block()
		{
			Add(/*Name = */ new TI.TagReference(this, TagGroups.crea));
			Add(new TI.Pad(32));
		}
		#endregion
	}
	#endregion

	#region g_scenario_editor_folder_block
	[TI.Definition(1, 260)]
	public class g_scenario_editor_folder_block : TI.Definition
	{
		public TI.BlockIndex ParentFolder;
		public TI.String Name;

		public g_scenario_editor_folder_block()
		{
			Add(ParentFolder = TI.BlockIndex.Long); // 1 g_scenario_editor_folder_block
			Add(Name = TI.String.LongString);
		}
	}
	#endregion

	#region scenario
	[TI.TagGroup((int)TagGroups.Enumerated.scnr, 3, 1976)]
	public class scenario_group : TI.Definition
	{
		#region scenario_profiles_block
		[TI.Definition(1, 84)]
		public class scenario_profiles_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_profiles_block()
			{
				Add(/*Name = */ new TI.String());
				Add(/*Starting Health Damage = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*Starting Shield Damage = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*Primary Weapon = */ new TI.TagReference(this, TagGroups.weap));
				Add(/*Rounds Loaded = */ new TI.ShortInteger());
				Add(/*Rounds Total = */ new TI.ShortInteger());
				Add(/*Secondary Weapon = */ new TI.TagReference(this, TagGroups.weap));
				Add(/*Rounds Loaded = */ new TI.ShortInteger());
				Add(/*Rounds Total = */ new TI.ShortInteger());
				Add(/*Starting Fragmentation Grenade Count = */ new TI.ByteInteger());
				Add(/*Starting Plasma Grenade Count = */ new TI.ByteInteger());
				Add(/*Starting Spike Grenade Count = */ new TI.ByteInteger());
				Add(/*Starting Flame Grenade Count = */ new TI.ByteInteger());
			}
			#endregion
		}
		#endregion

		#region scenario_players_block
		[TI.Definition(2, 24)]
		public class scenario_players_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_players_block()
			{
				Add(/*Position = */ new TI.RealPoint3D());
				Add(/*Facing = */ new TI.Real(TI.FieldType.Angle));
				Add(/*Team Designator = */ new TI.Enum());
				Add(/*BSP Index = */ new TI.ShortInteger());
				Add(/*Game Type = */ new TI.Enum());
				Add(/*Spawn Type = */ new TI.Enum());
			}
			#endregion
		}
		#endregion

		#region scenario_spawn_data_block
		[TI.Definition(1, 108)]
		public class scenario_spawn_data_block : TI.Definition
		{
			#region dynamic_spawn_zone_overload_block
			[TI.Definition(1, 16)]
			public class dynamic_spawn_zone_overload_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public dynamic_spawn_zone_overload_block()
				{
					Add(/*Overload Type = */ new TI.Enum());
					Add(new TI.Pad(2));
					Add(/*Inner Radius = */ new TI.Real());
					Add(/*Outer Radius = */ new TI.Real());
					Add(/*Weight = */ new TI.Real());
				}
				#endregion
			}
			#endregion

			#region static_spawn_zone_block
			[TI.Definition(1, 48)]
			public class static_spawn_zone_block : TI.Definition
			{
				#region dynamic_spawn_zone_overload_block
				[TI.Definition(1, 16)]
				public class dynamic_spawn_zone_overload_block : TI.Definition
				{
					#region Fields
					#endregion

					#region Ctor
					public dynamic_spawn_zone_overload_block()
					{
						Add(/*Overload Type = */ new TI.Enum());
						Add(new TI.Pad(2));
						Add(/*Inner Radius = */ new TI.Real());
						Add(/*Outer Radius = */ new TI.Real());
						Add(/*Weight = */ new TI.Real());
					}
					#endregion
				}
				#endregion

				#region Fields
				#endregion

				#region Ctor
				public static_spawn_zone_block()
				{
					Add(/*Data = */ new TI.Struct<static_spawn_zone_data_struct>(this));
					Add(/*Position = */ new TI.RealPoint3D());
					Add(/*Lower Height = */ new TI.Real());
					Add(/*Upper Height = */ new TI.Real());
					Add(/*Inner Radius = */ new TI.Real());
					Add(/*Outer Radius = */ new TI.Real());
					Add(/*Weight = */ new TI.Real());
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public scenario_spawn_data_block()
			{
				Add(/*Dynamic Spawn Lower Height = */ new TI.Real());
				Add(/*Dynamic Spawn Upper Height = */ new TI.Real());
				Add(/*Game Object Reset Height = */ new TI.Real());
				Add(new TI.Pad(60));
				Add(/*Dynamic Spawn Overloads = */ new TI.Block<dynamic_spawn_zone_overload_block>(this, 32));
				Add(/*Static Respawn Zones = */ new TI.Block<static_spawn_zone_block>(this, 128));
				Add(/*Static Initial Spawn Zones = */ new TI.Block<static_spawn_zone_block>(this, 128));
			}
			#endregion
		}
		#endregion

		#region scenario_simulation_definition_table_block
		[TI.Definition(1, 4)]
		public class scenario_simulation_definition_table_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_simulation_definition_table_block()
			{
				Add(new TI.Skip(4));
			}
			#endregion
		}
		#endregion

		#region Fields
		public TI.Enum Type;
		public TI.Flags Flags;
		//public TI.Real LocalNorth;
		public TI.Real SandboxBuget;
		public TI.Block<scenario_structure_bsp_reference_block> StructureBsps;
		public TI.Block<scenario_sky_reference_block> Skies;

		#region scenario objects
		public TI.Block<scenario_object_names_block> ObjectNames;
		//public TI.Block<scenario_scenery_block> Scenery;
		public TI.Block<scenario_scenery_palette_block> SceneryPalette;
		//public TI.Block<scenario_biped_block> Bipeds;
		public TI.Block<scenario_biped_palette_block> BipedsPalette;
		//public TI.Block<scenario_vehicle_block> Vehicles;
		public TI.Block<scenario_vehicle_palette_block> VehiclePalette;
		//public TI.Block<scenario_equipment_block> Equipment;
		public TI.Block<scenario_equipment_palette_block> EquipmentPalette;
		//public TI.Block<scenario_weapon_block> Weapons;
		public TI.Block<scenario_weapon_palette_block> WeaponPalette;
		//public TI.Block<device_group_block> DeviceGroups;
		//public TI.Block<scenario_machine_block> Machines;
		public TI.Block<scenario_machine_palette_block> MachinePalette;
		//public TI.Block<scenario_terminal_block> Terminals;
		public TI.Block<scenario_terminal_palette_block> TerminalPalette;
		//public TI.Block<scenario_control_block> Controls;
		public TI.Block<scenario_control_palette_block> ControlPalette;
		//public TI.Block<scenario_sound_scenery_block> SoundScenery;
		public TI.Block<scenario_sound_scenery_palette_block> SoundSceneryPalette;
		//public TI.Block<scenario_giant_block> Giants;
		public TI.Block<scenario_giant_palette_block> GiantsPalette;
		//public TI.Block<scenario_effect_scenery_block> EffectScenery;
		public TI.Block<scenario_effect_scenery_palette_block> EffectSceneryPalette;
		//public TI.Block<scenario_light_block> LightVolumes;
		public TI.Block<scenario_light_palette_block> LightVolumesPalette;
		#endregion

		#region scripting and cinematics
		public TI.Data HsStringData;
		public TI.Block<hs_scripts_block> HsScripts;
		public TI.Block<hs_globals_block> HsGlobals;
		public TI.Block<hs_references_block> References;
		public TI.Block<H2.hs_source_files_block> SourceFiles;
		public TI.Block<H2.cs_script_data_block> ScriptingData;
		public TI.Block<scenario_cutscene_flag_block> CutsceneFlags;
		public TI.Block<scenario_cutscene_camera_point_block> CutsceneCameraPoints;
		public TI.Block<scenario_cutscene_title_block> CutsceneTitles;
		public TI.TagReference CustomObjectNames;
		public TI.TagReference ChapterTitleText;
		#endregion
		public TI.Block<scenario_resources_block> ScenarioResources;
		public TI.Block<hs_unit_seat_block> HsUnitSeats;
		public TI.Block<scenario_kill_trigger_volumes_block> ScenarioKillTriggers;
		public TI.Block<syntax_datum_block> HsScriptDatums;
		#endregion

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
			Add(StructureBsps = new TI.Block<scenario_structure_bsp_reference_block>(this, 16));
			Add(/*DontUse = */new TI.TagReference(this, TagGroups.sbsp)); // Its a tag reference here, but IDK if its for THIS exactly...
			Add(Skies = new TI.Block<scenario_sky_reference_block>(this, 32));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x2C]
				// long
				// long
				// tag block [0x4]
				// tag block [0x54]
					// tag block [0xC]
						// tag block [0x20]
							// tag block [0x4]
							// long
							// long
					// tag block [0xC]
						// tag block [0x20]
							// tag block [0x4]
							// long
							// long
					// tag block [0x1] // most likely a bitvector...
					// tag block [0x1] // most likely a bitvector...
					// tag block [0x?] // may be an unused tag block
					// tag block [0x4]
					// tag block [0xC]
						// unknown [0xC]
				// tag block [0x18]
					// unknown [0x18]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x64] zone sets
				// unknown [0x28]
				// tag block [0x?]
				// tag block [0x?]
				// tag block [0x?]
				// tag block [0x?]
				// tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x24] zone set related
				// string id [name]
				// dword
				// dword (only seen as zero)
				// dword
				// dword
				// dword
				// dword
				// long block index? (only seen as NONE)
				// long block index? (probably to the tag block before this one)
			Add(TI.UnknownPad.BlockHalo3); // tag block [0xC] zone set related
				// string id [name]
				// dword (seems to equal 1<<element_index)
				// dword
			Add(new TI.UnknownPad(0x44)); // unknown [0x44]
			#region scenario objects
			Add(ObjectNames = new TI.Block<scenario_object_names_block>(this, 0)); // I'm sure the max is now over 640...
			Add(TI.UnknownPad.BlockHalo3); // tag block [0xB4] Scenery
			Add(SceneryPalette = new TI.Block<scenario_scenery_palette_block>(this, 0)); // I'm sure the max is now over 256...
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x74] Bipeds
			Add(BipedsPalette = new TI.Block<scenario_biped_palette_block>(this, 0)); // ''
			Add(TI.UnknownPad.BlockHalo3); // tag block [0xA8] Vehicles
			Add(VehiclePalette = new TI.Block<scenario_vehicle_palette_block>(this, 0)); // ''
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x8C] Equipment
			Add(EquipmentPalette = new TI.Block<scenario_equipment_palette_block>(this, 0)); // ''
			Add(TI.UnknownPad.BlockHalo3); // tag block [0xA8] Weapons
			Add(WeaponPalette = new TI.Block<scenario_weapon_palette_block>(this, 0)); // ''
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x28] Device groups
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x70?] Machines
			Add(MachinePalette = new TI.Block<scenario_machine_palette_block>(this, 0)); // ''
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] Terminals
			Add(TerminalPalette = new TI.Block<scenario_terminal_palette_block>(this, 0)); // ''
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x64?] Controls
			Add(ControlPalette = new TI.Block<scenario_control_palette_block>(this, 0)); // ''
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x70] Sound Scenery
			Add(SoundSceneryPalette = new TI.Block<scenario_sound_scenery_palette_block>(this, 0)); // ''
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] Giants
			Add(GiantsPalette = new TI.Block<scenario_giant_palette_block>(this, 0)); // ''
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] Effect Scenery
			Add(EffectSceneryPalette = new TI.Block<scenario_effect_scenery_palette_block>(this, 0)); // ''
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x70] Light Volumes
			Add(LightVolumesPalette = new TI.Block<scenario_light_palette_block>(this, 0)); // ''
			#endregion
			Add(/*Sandbox vehicles = */new TI.Block<scenario_sandbox_vehicle_settings_block>(this, 0)); // 0x1E8
			Add(/*Sandbox weapons = */new TI.Block<scenario_sandbox_weapon_settings_block>(this, 0));
			Add(/*Sandbox equipment = */new TI.Block<scenario_sandbox_equipment_settings_block>(this, 0));
			Add(/*Sandbox scenery = */new TI.Block<scenario_sandbox_scenery_settings_block>(this, 0));
			Add(/*Sandbox teleporters = */new TI.Block<scenario_sandbox_teleporter_settings_block>(this, 0));
			Add(/*Sandbox netgame flags = */new TI.Block<scenario_sandbox_netpoint_settings_block>(this, 0));
			Add(/*Sandbox spawns = */new TI.Block<scenario_sandbox_spawn_settings_block>(this, 0));
			Add(TI.UnknownPad.BlockHalo3); // 0x23C tag block [0xC], camera related
				// word
				// word?
				// string id
				// word
				// word?
			Add(/*PlayerStartingProfile = */new TI.Block<scenario_profiles_block>(this, 256));
			Add(/*PlayerStartingLocations = */new TI.Block<scenario_players_block>(this, 256));
			Add(/*KillTriggerVolumes = */new TI.Block<scenario_trigger_volume_block>(this, 256)); // 0x260
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x10], trigger volume related, zone set related
				// word
				// short block index
				// word
				// short block index
				// word
				// short block index
				// word
				// short block index
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
			Add(/*DecalSystems = */new TI.Block<scenario_decal_system_palette_block>(this, 128));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(/*SquadGroups = */new TI.Block<squad_groups_block>(this, 100));
			Add(TI.UnknownPad.BlockHalo3); // Add(Squads = new TI.Block<squads_block>(this, 335));
			Add(/*Zones = */new TI.Block<zone_block>(this, 128));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] // Mission Scenes?
			Add(/*CharacterPalette = */new TI.Block<character_palette_block>(this, 64));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x6C]
				// tag block [0x?]
				// tag block [0x?]
				// tag block [0x?]
				// tag block [0x?]
				// tag block [0x?]
				// tag block [0x?]
				// tag block [0x?]
				// tag block [0x?]
				// tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
				// unknown [0x54]
				// tag block [0x?]
				// tag block [0xC]
			#region scripting and cinematics
			Add(HsStringData = new TI.Data(this)); // 0x3E0, NOTE: memory region 3
			Add(HsScripts = new TI.Block<hs_scripts_block>(this, 1024));
			Add(HsGlobals = new TI.Block<hs_globals_block>(this, 256));
			Add(References = new TI.Block<hs_references_block>(this, 512));
			Add(SourceFiles = new TI.Block<H2.hs_source_files_block>(this, 8));
			Add(ScriptingData = new TI.Block<H2.cs_script_data_block>(this, 1));
			Add(CutsceneFlags = new TI.Block<scenario_cutscene_flag_block>(this, 512));
			Add(CutsceneCameraPoints = new TI.Block<scenario_cutscene_camera_point_block>(this, 512));
			Add(CutsceneTitles = new TI.Block<scenario_cutscene_title_block>(this, 128));
			Add(CustomObjectNames = new TI.TagReference(this, TagGroups.unic));
			Add(ChapterTitleText = new TI.TagReference(this, TagGroups.unic));
			Add(ScenarioResources = new TI.Block<scenario_resources_block>(this, 1));
			Add(HsUnitSeats = new TI.Block<hs_unit_seat_block>(this, 65536));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x2]
			Add(ScenarioKillTriggers = new TI.Block<scenario_kill_trigger_volumes_block>(this, 256)); // 0x498
			Add(HsScriptDatums = new TI.Block<syntax_datum_block>(this, 36864));
			#endregion
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] Orders
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] Triggers
			Add(/*BackgroundSoundPalette = */new TI.Block<structure_bsp_background_sound_palette_block>(this, 64));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] SoundEnvironmentPalette
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] WeatherPalette
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			version_construct_add_unnamed_null_blocks();
			Add(/*ScenarioClusterData = */new TI.Block<scenario_cluster_data_block>(this, 16));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			version_construct_add_unnamed_array();
			Add(/*SpawnData = */new TI.Block<scenario_spawn_data_block>(this, 1));
			Add(/*SoundEffectCollection = */new TI.TagReference(this, TagGroups.sfx_));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0xB0] Crates
			Add(/*CratesPalette = */new TI.Block<scenario_crate_palette_block>(this, 256));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x10] Flocks palette
				// tag reference
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x48] Flocks
			Add(/*Subtitles = */new TI.TagReference(this, TagGroups.unic));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] Creatures
			Add(/*CreaturesPalette = */new TI.Block<scenario_creature_palette_block>(this, 256));
			Add(/*EditorFolders = */new TI.Block<g_scenario_editor_folder_block>(this, 32767));
			Add(/*TerritoryLocationNames = */new TI.TagReference(this, TagGroups.unic));
			Add(new TI.Pad(8));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] MissionDialogue
			Add(/*Objectives = */new TI.TagReference(this, TagGroups.unic));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] Interpolators
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] SharedReferences
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] ScreenEffectReferences
			Add(/*SimulationDefinitionTable = */new TI.Block<scenario_simulation_definition_table_block>(this, 0/*512*/)); // 0x6A4, 512 is no longer the maximum (i'm willing to bet 

			// everything after was added during halo3
			Add(/*Camere fx settings = */new TI.TagReference(this, TagGroups.cfxs));
			Add(/*? = */new TI.TagReference(this, TagGroups.sefc));
			Add(/*sky parameters= */new TI.TagReference(this, TagGroups.skya));
			Add(/*GlobalLighting = */new TI.TagReference(this, TagGroups.chmt));
			Add(/*lightmap = */new TI.TagReference(this, TagGroups.sLdT));
			Add(/*performance throttles = */new TI.TagReference(this, TagGroups.perf));
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x10]
				// real point 3d
				// short (index?) // or string id...
				// short (index?)
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?] (maybe 0x10)
				// real point 3d
				// string id
			Add(TI.UnknownPad.BlockHalo3); // tag block [0x?]?
		}
	};
	#endregion
}