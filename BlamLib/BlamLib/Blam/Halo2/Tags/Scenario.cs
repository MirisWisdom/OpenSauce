/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo2.Tags
{
	#region multiplayer_scenario_description
	[TI.TagGroup((int)TagGroups.Enumerated.mply, 1, 12)]
	public class multiplayer_scenario_description_group : TI.Definition
	{
		#region scenario_description_block
		[TI.Definition(1, 68)]
		public class scenario_description_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_description_block() : base(4)
			{
				// Explanation here
				Add(/*descriptive bitmap = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*displayed map name = */ new TI.TagReference(this, TagGroups.unic));
				Add(/*scenario tag directory path = */ new TI.String());
				Add(new TI.Pad(4));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public multiplayer_scenario_description_group() : base(1)
		{
			Add(/*multiplayer scenarios = */ new TI.Block<scenario_description_block>(this, 32));
		}
		#endregion
	};
	#endregion

	#region scenario_sky_reference_block
	[TI.Definition(1, 16)]
	public class scenario_sky_reference_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_sky_reference_block() : base(1)
		{
			Add(/*Sky = */ new TI.TagReference(this, TagGroups.sky_));
		}
		#endregion
	}
	#endregion

	#region editor_comment_block
	[TI.Definition(1, 304)]
	public class editor_comment_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public editor_comment_block() : base(4)
		{
			Add(/*Position = */ new TI.RealPoint3D());
			Add(/*)Type = */ new TI.Enum(TI.FieldType.LongEnum));
			Add(/*Name = */ new TI.String());
			Add(/*Comment = */ new TI.String(TI.StringType.Ascii, 256));
		}
		#endregion
	}
	#endregion


	#region scenario objects
	#region dont_use_me_scenario_environment_object_block
	[TI.Definition(1, 64)]
	public class dont_use_me_scenario_environment_object_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public dont_use_me_scenario_environment_object_block() : base(7)
		{
			Add(/*BSP = */ new TI.BlockIndex()); // 1 scenario_structure_bsp_reference_block
			Add(/* = */ new TI.ShortInteger());
			Add(/*Unique ID = */ new TI.LongInteger());
			Add(new TI.Pad(4));
			Add(/*Object Definition Tag = */ new TI.Tag());
			Add(/*Object = */ new TI.LongInteger());
			Add(new TI.Pad(44));
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
		public scenario_object_names_block() : base(3)
		{
			Add(/*Name = */ new TI.String());
			Add(/* = */ new TI.BlockIndex()); // 1
			Add(/* = */ new TI.BlockIndex()); // 2
		}
		#endregion
	}
	#endregion

	#region scenario_object_id_struct
	[TI.Struct((int)StructGroups.Enumerated.obj_, 1, 8)]
	public class scenario_object_id_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_object_id_struct() : base(4)
		{
			Add(/*Unique ID = */ new TI.LongInteger());
			Add(/*Origin BSP Index = */ new TI.BlockIndex()); // 1 scenario_structure_bsp_reference_block
			Add(/*Type = */ new TI.Enum(TI.FieldType.ByteEnum));
			Add(/*Source = */ new TI.Enum(TI.FieldType.ByteEnum));
		}
		#endregion
	}
	#endregion

	#region scenario_object_datum_struct
	[TI.Struct((int)StructGroups.Enumerated.sobj, 2, 48)]
	public class scenario_object_datum_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_object_datum_struct() : base(10)
		{
			Add(/*Placement Flags = */ new TI.Flags());
			Add(/*Position = */ new TI.RealPoint3D());
			Add(/*Rotation = */ new TI.RealEulerAngles3D());
			Add(/*Scale = */ new TI.Real());
			Add(/*)Transform Flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*Manual BSP Flags = */ new TI.Flags(TI.FieldType.WordFlags)); // block flags
			Add(/*Object ID = */ new TI.Struct<scenario_object_id_struct>(this));
			Add(/*BSP Policy = */ new TI.Enum(TI.FieldType.ByteEnum));
			Add(new TI.Pad(1));
			Add(/*Editor Folder = */ new TI.BlockIndex()); // 1 g_scenario_editor_folder_block
		}
		#endregion
	}
	#endregion

	#region scenario_object_permutation_struct
	[TI.Struct((int)StructGroups.Enumerated.sper, 1, 24)]
	public class scenario_object_permutation_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_object_permutation_struct() : base(7)
		{
			Add(/*Variant Name = */ new TI.StringId());
			Add(/*Active Change Colors = */ new TI.Flags());
			Add(/*Primary Color = */ new TI.Color(TI.FieldType.RgbColor));
			Add(/*Secondary Color = */ new TI.Color(TI.FieldType.RgbColor));
			Add(/*Tertiary Color = */ new TI.Color(TI.FieldType.RgbColor));
			Add(/*Quaternary Color = */ new TI.Color(TI.FieldType.RgbColor));
			Add(new TI.UselessPad(16));
		}
		#endregion
	}
	#endregion

	#region scenario_object_palette_block
	[TI.Definition(1, 48)]
	public class scenario_object_palette_block : TI.Definition
	{
		#region Fields
		public TI.TagReference Name;
		#endregion

		public scenario_object_palette_block() : base(2)
		{
			Add(Name = new TI.TagReference(this, TagGroups.obje));
			Add(new TI.Pad(32));
		}
	};
	#endregion

	#region c_scenario_object
	public abstract class c_scenario_object : TI.Definition
	{
		public TI.BlockIndex Type, Name;
		public TI.Struct<scenario_object_datum_struct> ObjectData;

		/// <remarks>Only some object blocks use this</remarks>
		public TI.Struct<scenario_object_permutation_struct> PermutationData = null;

		protected c_scenario_object(int field_count) : base(field_count) { }
	};
	#endregion


	#region pathfinding_object_index_list_block
	[TI.Definition(1, 4)]
	public class pathfinding_object_index_list_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public pathfinding_object_index_list_block() : base(2)
		{
			Add(/*BSP Index = */ new TI.ShortInteger());
			Add(/*Pathfinding Object Index = */ new TI.ShortInteger());
		}
		#endregion
	}
	#endregion
	

	#region scenario_scenery_datum_struct_v4
	[TI.Struct((int)StructGroups.Enumerated.sct3, 1, 20)]
	public class scenario_scenery_datum_struct_v4 : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_scenery_datum_struct_v4() : base(5)
		{
			Add(/*Pathfinding Policy = */ new TI.Enum());
			Add(/*Lightmapping Policy = */ new TI.Enum());
			Add(/*Pathfinding References = */ new TI.Block<pathfinding_object_index_list_block>(this, 16));
			Add(new TI.Pad(2));
			Add(/*Valid Multiplayer Games = */ new TI.Flags(TI.FieldType.WordFlags));
		}
		#endregion
	}
	#endregion

	#region scenario_scenery_block
	[TI.Definition(5, 96)]
	public class scenario_scenery_block : c_scenario_object
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_scenery_block() : base(5)
		{
			Add(Type = new TI.BlockIndex()); // 1 scenario_scenery_palette_block
			Add(Name = new TI.BlockIndex()); // 1 scenario_object_names_block
			Add(ObjectData = new TI.Struct<scenario_object_datum_struct>(this));
			Add(PermutationData = new TI.Struct<scenario_object_permutation_struct>(this));
			Add(/*Scenery Data = */ new TI.Struct<scenario_scenery_datum_struct_v4>(this));
		}
		#endregion
	}
	#endregion


	#region scenario units
	#region scenario_unit_struct
	[TI.Struct((int)StructGroups.Enumerated.sunt, 1, 8)]
	public class scenario_unit_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_unit_struct() : base(2)
		{
			Add(/*Body Vitality = */ new TI.Real());
			Add(/*Flags = */ new TI.Flags());
		}
		#endregion
	}
	#endregion

	#region c_scenario_unit
	public abstract class c_scenario_unit : c_scenario_object
	{
		public TI.Struct<scenario_unit_struct> UnitData;

		protected c_scenario_unit(int field_count) : base(field_count) {}
	};
	#endregion

	#region scenario_biped_block
	[TI.Definition(3, 84)]
	public class scenario_biped_block : c_scenario_unit
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_biped_block() : base(5)
		{
			Add(Type = new TI.BlockIndex()); // 1 scenario_biped_palette_block
			Add(Name = new TI.BlockIndex()); // 1 scenario_object_names_block
			Add(ObjectData = new TI.Struct<scenario_object_datum_struct>(this));
			Add(PermutationData = new TI.Struct<scenario_object_permutation_struct>(this));
			Add(UnitData = new TI.Struct<scenario_unit_struct>(this));
		}
		#endregion
	}
	#endregion

	#region scenario_vehicle_block
	[TI.Definition(3, 84)]
	public class scenario_vehicle_block : c_scenario_unit
	{
		#region Ctor
		public scenario_vehicle_block() : base(5)
		{
			Add(Type = new TI.BlockIndex()); // 1 scenario_vehicle_palette_block
			Add(Name = new TI.BlockIndex()); // 1 scenario_object_names_block
			Add(ObjectData = new TI.Struct<scenario_object_datum_struct>(this));
			Add(PermutationData = new TI.Struct<scenario_object_permutation_struct>(this));
			Add(UnitData = new TI.Struct<scenario_unit_struct>(this));
		}
		#endregion
	}
	#endregion
	#endregion


	#region scenario items
	#region scenario_equipment_datum_struct
	[TI.Struct((int)StructGroups.Enumerated.seqt, 1, 4)]
	public class scenario_equipment_datum_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_equipment_datum_struct() : base(1)
		{
			Add(/*Equipment Flags = */ new TI.Flags());
		}
		#endregion
	}
	#endregion

	#region scenario_equipment_block
	[TI.Definition(3, 56)]
	public class scenario_equipment_block : c_scenario_object
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_equipment_block() : base(4)
		{
			Add(Type = new TI.BlockIndex()); // 1 scenario_equipment_palette_block
			Add(Name = new TI.BlockIndex()); // 1 scenario_object_names_block
			Add(ObjectData = new TI.Struct<scenario_object_datum_struct>(this));
			Add(/*Equipment Data = */ new TI.Struct<scenario_equipment_datum_struct>(this));
		}
		#endregion
	}
	#endregion

	#region scenario_weapon_datum_struct
	[TI.Struct((int)StructGroups.Enumerated.swpt, 1, 8)]
	public class scenario_weapon_datum_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_weapon_datum_struct() : base(3)
		{
			Add(/*Rounds Left = */ new TI.ShortInteger());
			Add(/*Rounds Loaded = */ new TI.ShortInteger());
			Add(/*Flags = */ new TI.Flags());
		}
		#endregion
	}
	#endregion

	#region scenario_weapon_block
	[TI.Definition(3, 84)]
	public class scenario_weapon_block : c_scenario_object
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_weapon_block() : base(5)
		{
			Add(Type = new TI.BlockIndex()); // 1 scenario_weapon_palette_block
			Add(Name = new TI.BlockIndex()); // 1 scenario_object_names_block
			Add(ObjectData = new TI.Struct<scenario_object_datum_struct>(this));
			Add(PermutationData = new TI.Struct<scenario_object_permutation_struct>(this));
			Add(/*Weapon Data = */ new TI.Struct<scenario_weapon_datum_struct>(this));
		}
		#endregion
	}
	#endregion
	#endregion


	#region scenario devices
	#region device_group_block
	[TI.Definition(1, 40)]
	public class device_group_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public device_group_block() : base(4)
		{
			Add(/*Name = */ new TI.String());
			Add(/*Initial Value = */ new TI.Real());
			Add(/*Flags = */ new TI.Flags());
			Add(new TI.UselessPad(12));
		}
		#endregion
	}
	#endregion

	#region scenario_device_struct
	[TI.Struct((int)StructGroups.Enumerated.sdvt, 1, 8)]
	public class scenario_device_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_device_struct() : base(3)
		{
			Add(/*Power Group = */ new TI.BlockIndex()); // 1 device_group_block
			Add(/*Position Group = */ new TI.BlockIndex()); // 1 device_group_block
			Add(/*Flags = */ new TI.Flags());
		}
		#endregion
	}
	#endregion

	#region scenario_machine_struct_v3
	[TI.Struct((int)StructGroups.Enumerated.smht, 1, 16)]
	public class scenario_machine_struct_v3 : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_machine_struct_v3() : base(2)
		{
			Add(/*Flags = */ new TI.Flags());
			Add(/*Pathfinding References = */ new TI.Block<pathfinding_object_index_list_block>(this, 16));
		}
		#endregion
	}
	#endregion

	#region c_scenario_device
	public abstract class c_scenario_device : c_scenario_object
	{
		public TI.Struct<scenario_device_struct> DeviceData;

		protected c_scenario_device(int field_count) : base(field_count) { }
	};
	#endregion

	#region scenario_machine_block
	[TI.Definition(4, 76)]
	public class scenario_machine_block : c_scenario_device
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_machine_block() : base(5)
		{
			Add(Type = new TI.BlockIndex()); // 1 scenario_machine_palette_block
			Add(Name = new TI.BlockIndex()); // 1 scenario_object_names_block
			Add(ObjectData = new TI.Struct<scenario_object_datum_struct>(this));
			Add(DeviceData = new TI.Struct<scenario_device_struct>(this));
			Add(/*Machine Data = */ new TI.Struct<scenario_machine_struct_v3>(this));
		}
		#endregion
	}
	#endregion

	#region scenario_control_struct
	[TI.Struct((int)StructGroups.Enumerated.sctt, 1, 8)]
	public class scenario_control_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_control_struct() : base(3)
		{
			Add(/*Flags = */ new TI.Flags());
			Add(/*DON'T TOUCH THIS = */ new TI.ShortInteger());
			Add(new TI.Pad(2));
		}
		#endregion
	}
	#endregion

	#region scenario_control_block
	[TI.Definition(3, 68)]
	public class scenario_control_block : c_scenario_device
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_control_block() : base(5)
		{
			Add(Type = new TI.BlockIndex()); // 1 scenario_control_palette_block
			Add(Name = new TI.BlockIndex()); // 1 scenario_object_names_block
			Add(ObjectData = new TI.Struct<scenario_object_datum_struct>(this));
			Add(DeviceData = new TI.Struct<scenario_device_struct>(this));
			Add(/*Control Data = */ new TI.Struct<scenario_control_struct>(this));
		}
		#endregion
	}
	#endregion

	#region scenario_light_fixture_struct
	[TI.Struct((int)StructGroups.Enumerated.slft, 1, 24)]
	public class scenario_light_fixture_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_light_fixture_struct() : base(4)
		{
			Add(/*Color = */ new TI.RealColor());
			Add(/*Intensity = */ new TI.Real());
			Add(/*Falloff Angle = */ new TI.Real(TI.FieldType.Angle));
			Add(/*Cutoff Angle = */ new TI.Real(TI.FieldType.Angle));
		}
		#endregion
	}
	#endregion

	#region scenario_light_fixture_block
	[TI.Definition(3, 84)]
	public class scenario_light_fixture_block : c_scenario_device
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_light_fixture_block() : base(5)
		{
			Add(Type = new TI.BlockIndex()); // 1 scenario_light_fixture_palette_block
			Add(Name = new TI.BlockIndex()); // 1 scenario_object_names_block
			Add(ObjectData = new TI.Struct<scenario_object_datum_struct>(this));
			Add(DeviceData = new TI.Struct<scenario_device_struct>(this));
			Add(/*light_fixture data = */ new TI.Struct<scenario_light_fixture_struct>(this));
		}
		#endregion
	}
	#endregion

	#region scenario_light_struct
	[TI.Struct((int)StructGroups.Enumerated.slit, 1, 48)]
	public class scenario_light_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_light_struct() : base(15)
		{
			Add(/*Type = */ new TI.Enum());
			Add(/*Flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*Lightmap Type = */ new TI.Enum());
			Add(/*Lightmap Flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*Lightmap Half Life = */ new TI.Real());
			Add(/*Lightmap Light Scale = */ new TI.Real());
			Add(new TI.UselessPad(116));
			Add(/*Target Point = */ new TI.RealPoint3D());
			Add(/*Width = */ new TI.Real());
			Add(/*Height Scale = */ new TI.Real());
			Add(/*Field of View = */ new TI.Real(TI.FieldType.Angle));
			Add(new TI.UselessPad(4));
			Add(/*Falloff Distance = */ new TI.Real());
			Add(/*Cutoff Distance = */ new TI.Real());
			Add(new TI.UselessPad(128));
		}
		#endregion
	}
	#endregion

	#region scenario_light_block
	[TI.Definition(3, 108)]
	public class scenario_light_block : c_scenario_device
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_light_block() : base(5)
		{
			Add(Type = new TI.BlockIndex()); // 1 scenario_light_palette_block
			Add(Name = new TI.BlockIndex()); // 1 scenario_object_names_block
			Add(ObjectData = new TI.Struct<scenario_object_datum_struct>(this));
			Add(DeviceData = new TI.Struct<scenario_device_struct>(this));
			Add(/*Light Data = */ new TI.Struct<scenario_light_struct>(this));
		}
		#endregion
	}
	#endregion
	#endregion


	#region sound_scenery_datum_struct
	[TI.Struct((int)StructGroups.Enumerated._sc_, 1, 28)]
	public class sound_scenery_datum_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public sound_scenery_datum_struct() : base(5)
		{
			Add(/*Volume Type = */ new TI.Enum(TI.FieldType.LongEnum));
			Add(/*Height = */ new TI.Real());
			Add(/*Override Distance Bounds = */ new TI.RealBounds());
			Add(/*Override Cone Angle Bounds = */ new TI.RealBounds(TI.FieldType.AngleBounds));
			Add(/*Override Outer Cone Gain = */ new TI.Real());
		}
		#endregion
	}
	#endregion

	#region scenario_sound_scenery_block
	[TI.Definition(3, 80)]
	public class scenario_sound_scenery_block : c_scenario_object
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_sound_scenery_block() : base(4)
		{
			Add(Type = new TI.BlockIndex()); // 1 scenario_sound_scenery_palette_block
			Add(Name = new TI.BlockIndex()); // 1 scenario_object_names_block
			Add(ObjectData = new TI.Struct<scenario_object_datum_struct>(this));
			Add(/*sound_scenery = */ new TI.Struct<sound_scenery_datum_struct>(this));
		}
		#endregion
	}
	#endregion
	#endregion


	#region scenario_trigger_volume_block
	[TI.Definition(2, 68)]
	public class scenario_trigger_volume_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_trigger_volume_block() : base(15)
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

			Add(/*Position = */ new TI.RealPoint3D());
			Add(/*Extents = */ new TI.RealPoint3D());
			Add(new TI.Pad(4));
			Add(/*Kill Trigger Volume = */ new TI.BlockIndex()); // 1 scenario_kill_trigger_volumes_block
			Add(new TI.Pad(2));
		}
		#endregion
	}
	#endregion

	#region recorded_animation_block
	[TI.Definition(1, 64)]
	public class recorded_animation_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public recorded_animation_block() : base(8)
		{
			Add(/*name = */ new TI.String());
			Add(/*version = */ new TI.ByteInteger());
			Add(/*raw animation data = */ new TI.ByteInteger());
			Add(/*unit control data version = */ new TI.ByteInteger());
			Add(new TI.Pad(1));
			Add(/*length of animation = */ new TI.ShortInteger());
			Add(new TI.Pad(2 + 4));
			Add(/*recorded animation event stream = */ new TI.Data(this));
		}
		#endregion
	}
	#endregion

	#region scenario_decals_block
	[TI.Definition(1, 16)]
	public class scenario_decals_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_decals_block() : base(4)
		{
			Add(/*Decal Type = */ new TI.BlockIndex()); // 1 scenario_decal_palette_block
			Add(/*Yaw[-127,127] = */ new TI.ByteInteger());
			Add(/*Pitch[-127,127] = */ new TI.ByteInteger());
			Add(/*Position = */ new TI.RealPoint3D());
		}
		#endregion
	}
	#endregion

	#region scenario_decal_palette_block
	[TI.Definition(1, 16)]
	public class scenario_decal_palette_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_decal_palette_block() : base(1)
		{
			Add(/*Reference = */ new TI.TagReference(this, TagGroups.deca));
		}
		#endregion
	}
	#endregion

	#region scenario_netgame_equipment_orientation_struct
	[TI.Struct((int)StructGroups.Enumerated.ntor, 1, 12)]
	public class scenario_netgame_equipment_orientation_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_netgame_equipment_orientation_struct() : base(1)
		{
			Add(/*Orientation = */ new TI.RealEulerAngles3D());
		}
		#endregion
	}
	#endregion


	#region style_palette_block
	[TI.Definition(1, 16)]
	public class style_palette_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public style_palette_block() : base(1)
		{
			Add(/*reference = */ new TI.TagReference(this, TagGroups.styl));
		}
		#endregion
	}
	#endregion

	#region squad_groups_block
	[TI.Definition(1, 36)]
	public class squad_groups_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public squad_groups_block() : base(5)
		{
			Add(/*name = */ new TI.String());
			Add(new TI.UselessPad(48));
			Add(/*parent = */ new TI.BlockIndex()); // 1 squad_groups_block
			Add(/*initial orders = */ new TI.BlockIndex()); // 1 orders_block
			Add(new TI.UselessPad(48));
		}
		#endregion
	}
	#endregion

	#region squads_block
	[TI.Definition(3, 120)]
	public class squads_block : TI.Definition
	{
		#region actor_starting_locations_block
		[TI.Definition(7, 100)]
		public class actor_starting_locations_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public actor_starting_locations_block() : base(22)
			{
				Add(/*name = */ new TI.StringId(true));
				Add(/*position = */ new TI.RealPoint3D());
				Add(/*reference frame = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(/*facing (yaw, pitch) = */ new TI.RealEulerAngles2D());
				Add(/*flags = */ new TI.Flags());
				Add(/*character type = */ new TI.BlockIndex()); // 1 character_palette_block
				Add(/*initial weapon = */ new TI.BlockIndex()); // 1 scenario_weapon_palette_block
				Add(/*initial secondary weapon = */ new TI.BlockIndex()); // 1 scenario_weapon_palette_block
				Add(new TI.Pad(2));
				Add(/*vehicle type = */ new TI.BlockIndex()); // 1 scenario_vehicle_palette_block
				Add(/*seat type = */ new TI.Enum());
				Add(/*grenade type = */ new TI.Enum());
				Add(/*swarm count = */ new TI.ShortInteger());
				Add(/*actor variant name = */ new TI.StringId());
				Add(/*vehicle variant name = */ new TI.StringId());
				Add(/*initial movement distance = */ new TI.Real());
				Add(/*emitter vehicle = */ new TI.BlockIndex()); // 1 scenario_vehicle_block
				Add(/*initial movement mode = */ new TI.Enum());
				Add(/*Placement script = */ new TI.String());
				Add(new TI.Skip(2));
				Add(new TI.Pad(2));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public squads_block() : base(24)
		{
			Add(/*name = */ new TI.String());
			Add(/*flags = */ new TI.Flags());
			Add(/*team = */ new TI.Enum());
			Add(/*parent = */ new TI.BlockIndex()); // 1 squad_groups_block
			Add(/*squad delay time = */ new TI.Real());
			Add(/*normal diff count = */ new TI.ShortInteger());
			Add(/*insane diff count = */ new TI.ShortInteger());
			Add(/*major upgrade = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(new TI.UselessPad(12));
			Add(/*vehicle type = */ new TI.BlockIndex()); // 1 scenario_vehicle_palette_block
			Add(/*character type = */ new TI.BlockIndex()); // 1 character_palette_block
			Add(/*initial zone = */ new TI.BlockIndex()); // 1 zone_block
			Add(new TI.Pad(2));
			Add(/*initial weapon = */ new TI.BlockIndex()); // 1 scenario_weapon_palette_block
			Add(/*initial secondary weapon = */ new TI.BlockIndex()); // 1 scenario_weapon_palette_block
			Add(/*grenade type = */ new TI.Enum());
			Add(/*initial order = */ new TI.BlockIndex()); // 1 orders_block
			Add(/*vehicle variant = */ new TI.StringId());
			Add(new TI.UselessPad(8));
			Add(/*starting locations = */ new TI.Block<actor_starting_locations_block>(this, 32));
			Add(/*Placement script = */ new TI.String());
			Add(new TI.Skip(2));
			Add(new TI.Pad(2));
		}
		#endregion
	}
	#endregion

	#region zone_block
	[TI.Definition(2, 64)]
	public class zone_block : TI.Definition
	{
		#region firing_positions_block
		[TI.Definition(4, 32)]
		public class firing_positions_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public firing_positions_block() : base(7)
			{
				Add(/*position (local) = */ new TI.RealPoint3D());
				Add(/*reference frame = */ new TI.ShortInteger());
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*area = */ new TI.BlockIndex()); // 1 areas_block
				Add(/*cluster index = */ new TI.ShortInteger());
				Add(new TI.Skip(4));
				Add(/*normal = */ new TI.RealEulerAngles2D());
			}
			#endregion
		}
		#endregion

		#region areas_block
		[TI.Definition(2, 140)]
		public class areas_block : TI.Definition
		{
			#region flight_reference_block
			[TI.Definition(1, 4)]
			public class flight_reference_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public flight_reference_block() : base(2)
				{
					Add(/*flight hint index = */ new TI.ShortInteger());
					Add(/*poit index = */ new TI.ShortInteger());
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public areas_block() : base(8)
			{
				Add(/*name = */ new TI.String());
				Add(/*area flags = */ new TI.Flags());
				Add(new TI.Skip(20));
				Add(new TI.Skip(4));
				Add(new TI.Pad(64));
				Add(/*manual reference frame = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(/*flight_hints = */ new TI.Block<flight_reference_block>(this, 10));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public zone_block() : base(7)
		{
			Add(/*name = */ new TI.String());
			Add(/*flags = */ new TI.Flags());
			Add(/*manual bsp = */ new TI.BlockIndex()); // 1 scenario_structure_bsp_reference_block
			Add(new TI.Pad(2));
			Add(new TI.UselessPad(24));
			Add(/*firing positions = */ new TI.Block<firing_positions_block>(this, 512));
			Add(/*areas = */ new TI.Block<areas_block>(this, 64));
		}
		#endregion
	}
	#endregion

	#region trigger_references
	[TI.Definition(1, 8)]
	public class trigger_references : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public trigger_references() : base(3)
		{
			Add(/*Trigger flags = */ new TI.Flags());
			Add(/*trigger = */ new TI.BlockIndex()); // 1 triggers_block
			Add(new TI.Pad(2));
		}
		#endregion
	}
	#endregion

	#region ai_scene_block
	[TI.Definition(1, 32)]
	public class ai_scene_block : TI.Definition
	{
		#region ai_scene_trigger_block
		[TI.Definition(1, 16)]
		public class ai_scene_trigger_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public ai_scene_trigger_block() : base(3)
			{
				Add(/*combination rule = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*triggers = */ new TI.Block<trigger_references>(this, 10));
			}
			#endregion
		}
		#endregion

		#region ai_scene_role_block
		[TI.Definition(1, 20)]
		public class ai_scene_role_block : TI.Definition
		{
			#region ai_scene_role_variants_block
			[TI.Definition(1, 4)]
			public class ai_scene_role_variants_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public ai_scene_role_variants_block() : base(1)
				{
					Add(/*variant designation = */ new TI.StringId());
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public ai_scene_role_block() : base(5)
			{
				Add(/*name = */ new TI.StringId());
				Add(/*group = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(new TI.UselessPad(36));
				Add(/*role variants = */ new TI.Block<ai_scene_role_variants_block>(this, 10));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public ai_scene_block() : base(5)
		{
			Add(/*name = */ new TI.StringId());
			Add(/*flags = */ new TI.Flags());
			Add(/*trigger conditions = */ new TI.Block<ai_scene_trigger_block>(this, 1));
			Add(new TI.UselessPad(32));
			Add(/*roles = */ new TI.Block<ai_scene_role_block>(this, 10));
		}
		#endregion
	}
	#endregion

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

	#region ai_animation_reference_block
	[TI.Definition(1, 60)]
	public class ai_animation_reference_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public ai_animation_reference_block() : base(3)
		{
			Add(/*animation name = */ new TI.String());
			Add(/*animation graph = */ new TI.TagReference(this, TagGroups.jmad));
			Add(new TI.Pad(12));
		}
		#endregion
	}
	#endregion

	#region ai_script_reference_block
	[TI.Definition(1, 40)]
	public class ai_script_reference_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public ai_script_reference_block() : base(2)
		{
			Add(/*script name = */ new TI.String());
			Add(new TI.Pad(8));
		}
		#endregion
	}
	#endregion

	#region ai_recording_reference_block
	[TI.Definition(1, 40)]
	public class ai_recording_reference_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public ai_recording_reference_block() : base(2)
		{
			Add(/*recording name = */ new TI.String());
			Add(new TI.Pad(8));
		}
		#endregion
	}
	#endregion

	#region ai_conversation_block
	[TI.Definition(1, 116)]
	public class ai_conversation_block : TI.Definition
	{
		#region ai_conversation_participant_block
		[TI.Definition(1, 84)]
		public class ai_conversation_participant_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public ai_conversation_participant_block() : base(6)
			{
				Add(new TI.Pad(8));
				Add(/*use this object = */ new TI.BlockIndex()); // 1 scenario_object_names_block
				Add(/*set new name = */ new TI.BlockIndex()); // 1 scenario_object_names_block
				Add(new TI.Pad(12 + 12));
				Add(/*encounter name = */ new TI.String());
				Add(new TI.Pad(4 + 12));
			}
			#endregion
		}
		#endregion

		#region ai_conversation_line_block
		[TI.Definition(1, 124)]
		public class ai_conversation_line_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public ai_conversation_line_block() : base(13)
			{
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*participant = */ new TI.BlockIndex()); // 1 ai_conversation_participant_block
				Add(/*addressee = */ new TI.Enum());
				Add(/*addressee participant = */ new TI.BlockIndex()); // 1 ai_conversation_participant_block
				Add(new TI.Pad(4));
				Add(/*line delay time = */ new TI.Real());
				Add(new TI.Pad(12));
				Add(/*variant 1 = */ new TI.TagReference(this, TagGroups.snd_));
				Add(/*variant 2 = */ new TI.TagReference(this, TagGroups.snd_));
				Add(/*variant 3 = */ new TI.TagReference(this, TagGroups.snd_));
				Add(/*variant 4 = */ new TI.TagReference(this, TagGroups.snd_));
				Add(/*variant 5 = */ new TI.TagReference(this, TagGroups.snd_));
				Add(/*variant 6 = */ new TI.TagReference(this, TagGroups.snd_));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public ai_conversation_block() : base(9)
		{
			Add(/*name = */ new TI.String());
			Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(/*trigger distance = */ new TI.Real());
			Add(/*run-to-player dist = */ new TI.Real());
			Add(new TI.Pad(36));
			Add(/*participants = */ new TI.Block<ai_conversation_participant_block>(this, 8));
			Add(/*lines = */ new TI.Block<ai_conversation_line_block>(this, 32));
			Add(/* = */ new TI.Block<TI.g_null_block>(this, 0));
		}
		#endregion
	}
	#endregion

	#region hs_scripts_block
	[TI.Definition(1, 40)]
	public class hs_scripts_block : Scripting.hs_scripts_block
	{
		#region Ctor
		public hs_scripts_block() : base(5)
		{
			Add(Name = new TI.String());
			Add(ScriptType = new TI.Enum());
			Add(ReturnType = new TI.Enum());
			Add(RootExpressionIndex = new TI.LongInteger());
			Add(new TI.UselessPad(52));
		}
		#endregion
	}
	#endregion

	#region hs_globals_block
	[TI.Definition(1, 40)]
	public class hs_globals_block : Scripting.hs_globals_block
	{
		public hs_globals_block() : base(6)
		{
			Add(Name = new TI.String());
			Add(Type = new TI.Enum());
			Add(new TI.Pad(2));
			Add(new TI.UselessPad(4));
			Add(InitExpressionIndex = new TI.LongInteger());
			Add(new TI.UselessPad(48));
		}
	}
	#endregion

	#region hs_references_block
	[TI.Definition(1, 16)]
	public class hs_references_block : TI.Definition
	{
		public hs_references_block() : base(2)
		{
			Add(new TI.UselessPad(24));
			Add(/*reference = */ new TI.TagReference(this));
		}
	}
	#endregion

	#region hs_source_files_block
	[TI.Definition(1, 52)]
	public class hs_source_files_block : TI.Definition
	{
		public hs_source_files_block() : base(2)
		{
			Add(/*name = */ new TI.String());
			Add(/*source = */ new TI.Data(this));
		}
	}
	#endregion

	#region cs_script_data_block
	[TI.Definition(1, 132)]
	public class cs_script_data_block : TI.Definition
	{
		#region cs_point_set_block
		[TI.Definition(2, 52)]
		public class cs_point_set_block : TI.Definition
		{
			#region cs_point_block
			[TI.Definition(2, 60)]
			public class cs_point_block : TI.Definition
			{
				public cs_point_block() : base(6)
				{
					Add(/*name = */ new TI.String());
					Add(/*position = */ new TI.RealPoint3D());
					Add(/*reference frame = */ new TI.ShortInteger());
					Add(TI.Pad.Word);
					Add(/*surface index = */ new TI.LongInteger());
					Add(/*facing direction = */ new TI.RealEulerAngles2D());
				}
			}
			#endregion

			public cs_point_set_block() : base(5)
			{
				Add(/*name = */ new TI.String());
				Add(/*points = */ new TI.Block<cs_point_block>(this, 20));
				Add(/*bsp index = */ new TI.BlockIndex()); // 1 scenario_structure_bsp_reference_block
				Add(/*manual reference frame = */ new TI.ShortInteger());
				Add(/*flags = */ new TI.Flags());
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

	#region scenario_cutscene_flag_block
	[TI.Definition(1, 56)]
	public class scenario_cutscene_flag_block : TI.Definition
	{
		public scenario_cutscene_flag_block() : base(5)
		{
			Add(new TI.Pad(4));
			Add(/*Name = */ new TI.String());
			Add(/*Position = */ new TI.RealPoint3D());
			Add(/*Facing = */ new TI.RealEulerAngles2D());
			Add(new TI.UselessPad(36));
		}
	}
	#endregion

	#region scenario_cutscene_camera_point_block
	[TI.Definition(1, 64)]
	public class scenario_cutscene_camera_point_block : TI.Definition
	{
		public scenario_cutscene_camera_point_block() : base(8)
		{
			Add(/*Flags = */ TI.Flags.Word);
			Add(/*Type = */ new TI.Enum());
			Add(/*Name = */ new TI.String());
			Add(new TI.UselessPad(4));
			Add(/*Position = */ new TI.RealPoint3D());
			Add(/*Orientation = */ new TI.RealEulerAngles3D());
			Add(/*)Unused = */ new TI.Real(TI.FieldType.Angle));
			Add(new TI.UselessPad(36));
		}
	}
	#endregion

	#region scenario_cutscene_title_block
	[TI.Definition(1, 36)]
	public class scenario_cutscene_title_block : TI.Definition
	{
		public scenario_cutscene_title_block() : base(9)
		{
			Add(/*name = */ new TI.StringId());
			Add(/*text bounds (on screen) = */ new TI.Rectangle2D());
			Add(/*justification = */ new TI.Enum());
			Add(/*font = */ new TI.Enum());
			Add(/*text color = */ new TI.Color(TI.FieldType.RgbColor));
			Add(/*shadow color = */ new TI.Color(TI.FieldType.RgbColor));
			Add(/*fade in time [seconds] = */ new TI.Real());
			Add(/*up time [seconds] = */ new TI.Real());
			Add(/*fade out time [seconds] = */ new TI.Real());
		}
	}
	#endregion


	#region scenario_structure_bsps_header
	[TI.Definition(2, scenario_structure_bsps_header.kSizeOf)]
	public class scenario_structure_bsps_header : TI.Definition
	{
		public const int kSizeOf = 16;

		#region Fields
		public TI.LongInteger Size;
		public TI.LongInteger PtrBsp;
		public TI.LongInteger PtrLightmap;
		public TI.Tag Signature;
		#endregion

		public scenario_structure_bsps_header()
		{
			Add(Size = new TI.LongInteger());
			Add(PtrBsp = new TI.LongInteger());
			Add(PtrLightmap = new TI.LongInteger());
			Add(Signature = new TI.Tag());
		}

		int CalculateBspSize()
		{
			uint bsp_address = (uint)PtrBsp.Value;
			uint ltmp_address = (uint)PtrLightmap.Value;

			return (int)(ltmp_address - bsp_address);
		}
		int CalculateLightmapSize()
		{
			uint total_size = (uint)Size.Value - kSizeOf;
			// we don't include the header size since we use the bsp address as our base address
			total_size -= kSizeOf;

			uint bsp_address = (uint)PtrBsp.Value;
			uint highest_address = bsp_address + total_size;

			uint ltmp_address = (uint)PtrLightmap.Value;
			return (int)(highest_address - ltmp_address);
		}

		/// <summary>
		/// Fixup the instance header of a BSP tag
		/// </summary>
		/// <param name="instance"></param>
		/// <param name="offset"></param>
		/// <returns>The BSP tag's address mask</returns>
		public uint FixupBspInstanceHeader(CacheItem instance, int offset)
		{
			instance.Address = PtrBsp;
			instance.Size = CalculateBspSize();
			instance.Offset = offset;
			instance.Location = CacheIndex.ItemLocation.Internal;

			uint bsp_address_mask = (uint)(instance.Address - instance.Offset);
			return bsp_address_mask;
		}
		public void FixupLightmapInstanceHeader(CacheItem instance, CacheItem owner_bsp)
		{
			instance.Address = this.PtrLightmap;
			instance.Size = CalculateLightmapSize();
			instance.Offset = owner_bsp.Offset + owner_bsp.Size;
			instance.Location = CacheIndex.ItemLocation.Internal;
		}
	};
	#endregion

	#region scenario_structure_bsp_reference_block
	[TI.Definition(2, scenario_structure_bsp_reference_block.kSizeOf)]
	public class scenario_structure_bsp_reference_block : TI.Definition
	{
		public const int kSizeOf = 84;
		/// <summary>
		/// Size of this block when targeting runtime (release) platforms
		/// </summary>
		public const int kRuntimeSizeOf = 68;

		#region Fields
		internal TI.LongInteger RuntimeOffset;
		internal TI.LongInteger RuntimeSize;
		internal TI.LongInteger RuntimeAddress;

		public TI.TagReference StructureBsp;
		public TI.TagReference Lightmap;
		#endregion

		#region Ctor
		public scenario_structure_bsp_reference_block() : base(16)
		{
			Add(RuntimeOffset = new TI.LongInteger());
			Add(RuntimeSize = new TI.LongInteger());
			Add(RuntimeAddress = new TI.LongInteger());
			Add(new TI.Pad(4));

			Add(StructureBsp = new TI.TagReference(this, TagGroups.sbsp));
			Add(Lightmap = new TI.TagReference(this, TagGroups.ltmp));
			Add(new TI.Pad(4));
			Add(/*UNUSED radiance est. search distance = */ new TI.Real());
			Add(new TI.Pad(4));
			Add(/*UNUSED luminels per world unit = */ new TI.Real());
			Add(/*UNUSED output white reference = */ new TI.Real());
			Add(new TI.Pad(8));
			Add(/*Flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(/*Default Sky = */ new TI.BlockIndex()); // 1 scenario_sky_reference_block
			Add(new TI.Pad(2));
		}
		#endregion
	}
	#endregion


	#region scenario_resources_block
	[TI.Definition(1, 36)]
	public class scenario_resources_block : TI.Definition
	{
		#region scenario_resource_reference_block
		[TI.Definition(1, 16)]
		public class scenario_resource_reference_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_resource_reference_block() : base(1)
			{
				Add(/*Reference = */ new TI.TagReference(this));
			}
			#endregion
		}
		#endregion

		#region scenario_hs_source_reference_block
		[TI.Definition(1, 16)]
		public class scenario_hs_source_reference_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_hs_source_reference_block() : base(1)
			{
				Add(/*Reference = */ new TI.TagReference(this, TagGroups.srhscf));
			}
			#endregion
		}
		#endregion

		#region scenario_ai_resource_reference_block
		[TI.Definition(1, 16)]
		public class scenario_ai_resource_reference_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_ai_resource_reference_block() : base(1)
			{
				Add(/*Reference = */ new TI.TagReference(this, TagGroups.srai));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public scenario_resources_block() : base(3)
		{
			Add(/*References = */ new TI.Block<scenario_resource_reference_block>(this, 16));
			Add(/*Script Source = */ new TI.Block<scenario_hs_source_reference_block>(this, 8));
			Add(/*AI Resources = */ new TI.Block<scenario_ai_resource_reference_block>(this, 2));
		}
		#endregion
	}
	#endregion

	#region hs_unit_seat_block
	[TI.Definition(1, 8)]
	public class hs_unit_seat_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public hs_unit_seat_block() : base(2)
		{
			Add(/* = */ new TI.LongInteger());
			Add(/* = */ new TI.LongInteger());
		}
		#endregion
	}
	#endregion

	#region scenario_kill_trigger_volumes_block
	[TI.Definition(1, 2)]
	public class scenario_kill_trigger_volumes_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_kill_trigger_volumes_block() : base(1)
		{
			Add(/*Trigger Volume = */ new TI.BlockIndex()); // 1 scenario_trigger_volume_block
		}
		#endregion
	}
	#endregion

	#region syntax_datum_block
	[TI.Definition(1, 20)]
	public class syntax_datum_block : Scripting.hs_syntax_datum_block
	{
		#region Ctor
		public syntax_datum_block() : base(7)
		{
			Add(DatumHeader = new TI.ShortInteger());
			Add(TypeUnion/*Script Index/Function Index/Constant Type Union*/= new TI.ShortInteger());
			Add(Type = new TI.ShortInteger());
			Add(Flags = new TI.ShortInteger());
			Add(NextNodeIndex = new TI.LongInteger());
			Add(Pointer = new TI.LongInteger());
			Add(Data = new TI.LongInteger());
		}
		#endregion
	}
	#endregion

	#region orders_block
	[TI.Definition(3, 144)]
	public class orders_block : TI.Definition
	{
		#region zone_set_block
		[TI.Definition(2, 8)]
		public class zone_set_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public zone_set_block() : base(4)
			{
				Add(/*area type = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*zone = */ new TI.BlockIndex()); // 1 zone_block
				Add(/*area = */ new TI.BlockIndex()); // 2
			}
			#endregion
		}
		#endregion

		#region secondary_zone_set_block
		[TI.Definition(2, 8)]
		public class secondary_zone_set_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public secondary_zone_set_block() : base(4)
			{
				Add(/*area type = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*zone = */ new TI.BlockIndex()); // 1 zone_block
				Add(/*area = */ new TI.BlockIndex()); // 2
			}
			#endregion
		}
		#endregion

		#region secondary_set_trigger_block
		[TI.Definition(1, 16)]
		public class secondary_set_trigger_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public secondary_set_trigger_block() : base(3)
			{
				Add(/*combination rule = */ new TI.Enum());
				Add(/*dialogue type = */ new TI.Enum());
				Add(/*triggers = */ new TI.Block<trigger_references>(this, 10));
			}
			#endregion
		}
		#endregion

		#region special_movement_block
		[TI.Definition(1, 4)]
		public class special_movement_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public special_movement_block() : base(1)
			{
				Add(/*Special movement 1 = */ new TI.Flags());
			}
			#endregion
		}
		#endregion

		#region order_ending_block
		[TI.Definition(1, 24)]
		public class order_ending_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public order_ending_block() : base(7)
			{
				Add(/*next order = */ new TI.BlockIndex()); // 1 orders_block
				Add(/*combination rule = */ new TI.Enum());
				Add(/*delay time = */ new TI.Real());
				Add(/*dialogue type = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(new TI.UselessPad(16));
				Add(/*triggers = */ new TI.Block<trigger_references>(this, 10));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public orders_block() : base(16)
		{
			Add(/*name = */ new TI.String());
			Add(/*Style = */ new TI.BlockIndex()); // 1 style_palette_block
			Add(new TI.Pad(2));
			Add(/*flags = */ new TI.Flags());
			Add(/*Force combat status = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(/*Entry Script = */ new TI.String());
			Add(new TI.Skip(2));
			Add(/*Follow squad = */ new TI.BlockIndex()); // 1 squads_block
			Add(/*follow radius = */ new TI.Real());
			Add(/*Primary area set = */ new TI.Block<zone_set_block>(this, 32));
			Add(/*Secondary area set = */ new TI.Block<secondary_zone_set_block>(this, 32));
			Add(/*Secondary set trigger = */ new TI.Block<secondary_set_trigger_block>(this, 1));
			Add(/*Special movement = */ new TI.Block<special_movement_block>(this, 1));
			Add(new TI.UselessPad(12));
			Add(/*Order endings = */ new TI.Block<order_ending_block>(this, 12));
		}
		#endregion
	}
	#endregion

	#region triggers_block
	[TI.Definition(1, 52)]
	public class triggers_block : TI.Definition
	{
		#region order_completion_condition
		[TI.Definition(1, 56)]
		public class order_completion_condition : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public order_completion_condition() : base(15)
			{
				Add(/*rule type = */ new TI.Enum());
				Add(/*squad = */ new TI.BlockIndex()); // 1 squads_block
				Add(/*squad group = */ new TI.BlockIndex()); // 1 squad_groups_block
				Add(/* = */ new TI.ShortInteger());
				Add(new TI.UselessPad(4 + 12));
				Add(/* = */ new TI.Real());
				Add(new TI.UselessPad(8));
				Add(/*trigger volume = */ new TI.BlockIndex()); // 1 scenario_trigger_volume_block
				Add(new TI.Pad(2));
				Add(new TI.UselessPad(8));
				Add(/*Exit condition script = */ new TI.String());
				Add(/* = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(new TI.UselessPad(36));
				Add(/*flags = */ new TI.Flags());
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public triggers_block() : base(6)
		{
			Add(/*name = */ new TI.String());
			Add(/*trigger flags = */ new TI.Flags());
			Add(/*combination rule = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(new TI.UselessPad(24));
			Add(/*conditions = */ new TI.Block<order_completion_condition>(this, 5));
		}
		#endregion
	}
	#endregion


	#region scenario_crate_block
	[TI.Definition(1, 76)]
	public class scenario_crate_block : c_scenario_object
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_crate_block() : base(4)
		{
			Add(Type = new TI.BlockIndex()); // 1 scenario_crate_palette_block
			Add(Name = new TI.BlockIndex()); // 1 scenario_object_names_block
			Add(ObjectData = new TI.Struct<scenario_object_datum_struct>(this));
			Add(PermutationData = new TI.Struct<scenario_object_permutation_struct>(this));
		}
		#endregion
	}
	#endregion

	#region scenario_creature_block
	[TI.Definition(1, 52)]
	public class scenario_creature_block : c_scenario_object
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_creature_block() : base(3)
		{
			Add(Type = new TI.BlockIndex()); // 1 scenario_creature_palette_block
			Add(Name = new TI.BlockIndex()); // 1 scenario_object_names_block
			Add(ObjectData = new TI.Struct<scenario_object_datum_struct>(this));
		}
		#endregion
	}
	#endregion


	#region scenario_decorator_set_palette_entry_block
	[TI.Definition(1, 16)]
	public class scenario_decorator_set_palette_entry_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_decorator_set_palette_entry_block() : base(1)
		{
			Add(/*Decorator Set = */ new TI.TagReference(this, TagGroups.DECR));
		}
		#endregion
	}
	#endregion

	#region scenario_bsp_switch_transition_volume_block
	[TI.Definition(1, 8)]
	public class scenario_bsp_switch_transition_volume_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_bsp_switch_transition_volume_block() : base(3)
		{
			Add(/*BSP Index Key = */ new TI.LongInteger());
			Add(/*Trigger Volume = */ new TI.BlockIndex()); // 1 scenario_trigger_volume_block
			Add(new TI.Pad(2));
		}
		#endregion
	}
	#endregion

	#region scenario_structure_bsp_spherical_harmonic_lighting_block
	[TI.Definition(1, 28)]
	public class scenario_structure_bsp_spherical_harmonic_lighting_block : TI.Definition
	{
		#region scenario_spherical_harmonic_lighting_point
		[TI.Definition(1, 12)]
		public class scenario_spherical_harmonic_lighting_point : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_spherical_harmonic_lighting_point() : base(1)
			{
				Add(/*Position = */ new TI.RealPoint3D());
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public scenario_structure_bsp_spherical_harmonic_lighting_block() : base(2)
		{
			Add(/*BSP = */ new TI.TagReference(this, TagGroups.sbsp));
			Add(/*Lighting Points = */ new TI.Block<scenario_spherical_harmonic_lighting_point>(this, 32768));
		}
		#endregion
	}
	#endregion

	#region g_scenario_editor_folder_block
	[TI.Definition(1, 260)]
	public class g_scenario_editor_folder_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public g_scenario_editor_folder_block() : base(2)
		{
			Add(/*parent folder = */ new TI.BlockIndex(TI.FieldType.LongBlockIndex)); // 1 g_scenario_editor_folder_block
			Add(/*name = */ new TI.String(TI.StringType.Ascii, 256));
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
		public static_spawn_zone_data_struct() : base(4)
		{
			Add(/*Name = */ new TI.StringId());
			Add(/*Relevant Team = */ new TI.Flags());
			Add(/*Relevant Games = */ new TI.Flags());
			Add(/*Flags = */ new TI.Flags());
		}
		#endregion
	}
	#endregion

	#region flock_definition_block
	[TI.Definition(1, 148)]
	public class flock_definition_block : TI.Definition
	{
		#region flock_source_block
		[TI.Definition(1, 28)]
		public class flock_source_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public flock_source_block() : base(4)
			{
				Add(/*position = */ new TI.RealVector3D());
				Add(/*starting yaw, pitch = */ new TI.RealEulerAngles2D());
				Add(/*radius = */ new TI.Real());
				Add(/*weight = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region flock_sink_block
		[TI.Definition(1, 16)]
		public class flock_sink_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public flock_sink_block() : base(2)
			{
				Add(/*position = */ new TI.RealVector3D());
				Add(/*radius = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public flock_definition_block() : base(35)
		{
			Add(/*bsp = */ new TI.BlockIndex()); // 1 scenario_structure_bsp_reference_block
			Add(new TI.Pad(2));
			Add(new TI.UselessPad(32));
			Add(/*bounding volume = */ new TI.BlockIndex()); // 1 scenario_trigger_volume_block
			Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*ecology margin = */ new TI.Real());
			Add(new TI.UselessPad(16));
			Add(/*sources = */ new TI.Block<flock_source_block>(this, 10));
			Add(/*sinks = */ new TI.Block<flock_sink_block>(this, 10));
			Add(/*production frequency = */ new TI.Real());
			Add(/*scale = */ new TI.RealBounds());
			Add(new TI.UselessPad(12));
			Add(/*creature = */ new TI.TagReference(this, TagGroups.crea));
			Add(/*boid count = */ new TI.ShortIntegerBounds());
			Add(new TI.UselessPad(24));
			Add(/*neighborhood radius = */ new TI.Real());
			Add(/*avoidance radius = */ new TI.Real());
			Add(/*forward scale = */ new TI.Real());
			Add(/*alignment scale = */ new TI.Real());
			Add(/*avoidance scale = */ new TI.Real());
			Add(/*leveling force scale = */ new TI.Real());
			Add(/*sink scale = */ new TI.Real());
			Add(/*perception angle = */ new TI.Real(TI.FieldType.Angle));
			Add(/*average throttle = */ new TI.Real());
			Add(/*maximum throttle = */ new TI.Real());
			Add(/*position scale = */ new TI.Real());
			Add(/*position min radius = */ new TI.Real());
			Add(/*position max radius = */ new TI.Real());
			Add(/*movement weight threshold = */ new TI.Real());
			Add(/*danger radius = */ new TI.Real());
			Add(/*danger scale = */ new TI.Real());
			Add(/*random offset scale = */ new TI.Real());
			Add(/*random offset period = */ new TI.RealBounds());
			Add(new TI.UselessPad(24 + 4));
			Add(/*flock name = */ new TI.StringId());
		}
		#endregion
	}
	#endregion


	#region structure_bsp_background_sound_palette_block
	[TI.Definition(1, 116)]
	public class structure_bsp_background_sound_palette_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public structure_bsp_background_sound_palette_block() : base(11)
		{
			Add(/*Name = */ new TI.String());
			Add(/*Background Sound = */ new TI.TagReference(this, TagGroups.lsnd));
			Add(/*Inside Cluster Sound = */ new TI.TagReference(this, TagGroups.lsnd));
			Add(new TI.Pad(20));
			Add(/*Cutoff Distance = */ new TI.Real());
			Add(/*Scale Flags = */ new TI.Flags());
			Add(/*Interior Scale = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*Portal Scale = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*Exterior Scale = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*Interpolation Speed = */ new TI.Real());
			Add(new TI.Pad(8));
		}
		#endregion
	}
	#endregion

	#region structure_bsp_sound_environment_palette_block
	[TI.Definition(1, 80)]
	public class structure_bsp_sound_environment_palette_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public structure_bsp_sound_environment_palette_block() : base(5)
		{
			Add(/*Name = */ new TI.String());
			Add(/*Sound Environment = */ new TI.TagReference(this, TagGroups.snde));
			Add(/*Cutoff Distance = */ new TI.Real());
			Add(/*Interpolation Speed = */ new TI.Real());
			Add(new TI.Pad(24));
		}
		#endregion
	}
	#endregion

	#region structure_bsp_weather_palette_block
	[TI.Definition(1, 152)]
	public class structure_bsp_weather_palette_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public structure_bsp_weather_palette_block() : base(8)
		{
			Add(/*Name = */ new TI.String());
			Add(/*Weather System = */ new TI.TagReference(this, TagGroups.weat));
			Add(new TI.Pad(2 + 2 + 32));
			Add(/*Wind = */ new TI.TagReference(this, TagGroups.wind));
			Add(/*Wind Direction = */ new TI.RealVector3D());
			Add(/*Wind Magnitude = */ new TI.Real());
			Add(new TI.Pad(4));
			Add(/*Wind Scale Function = */ new TI.String());
		}
		#endregion
	}
	#endregion

	#region scenario_cluster_data_block
	[TI.Definition(1, 80)]
	public class scenario_cluster_data_block : TI.Definition
	{
		#region scenario_cluster_background_sounds_block
		[TI.Definition(1, 4)]
		public class scenario_cluster_background_sounds_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_cluster_background_sounds_block() : base(2)
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
			public scenario_cluster_sound_environments_block() : base(2)
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
			public scenario_cluster_points_block() : base(1)
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
			public scenario_cluster_weather_properties_block() : base(2)
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
			public scenario_cluster_atmospheric_fog_properties_block() : base(2)
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
		public scenario_cluster_data_block() : base(7)
		{
			Add(/*BSP = */ new TI.TagReference(this, TagGroups.sbsp));
			Add(/*Background Sounds = */ new TI.Block<scenario_cluster_background_sounds_block>(this, 512));
			Add(/*Sound Environments = */ new TI.Block<scenario_cluster_sound_environments_block>(this, 512));
			Add(/*BSP Checksum = */ new TI.LongInteger());
			Add(/*Cluster Centroids = */ new TI.Block<scenario_cluster_points_block>(this, 512));
			Add(/*Weather Properties = */ new TI.Block<scenario_cluster_weather_properties_block>(this, 512));
			Add(/*Atmospheric Fog Properties = */ new TI.Block<scenario_cluster_atmospheric_fog_properties_block>(this, 512));
		}
		#endregion
	}
	#endregion

	#region scenario_atmospheric_fog_palette
	[TI.Definition(1, 256)]
	public class scenario_atmospheric_fog_palette : TI.Definition
	{
		#region scenario_atmospheric_fog_mixer_block
		[TI.Definition(1, 16)]
		public class scenario_atmospheric_fog_mixer_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_atmospheric_fog_mixer_block() : base(4)
			{
				Add(new TI.Pad(4));
				Add(/*Atmospheric Fog Source = */ new TI.StringId());
				Add(/*Interpolator = */ new TI.StringId());
				Add(new TI.Skip(2 + 2));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public scenario_atmospheric_fog_palette() : base(31)
		{
			Add(/*Name = */ new TI.StringId());
			Add(/*Color = */ new TI.RealColor());
			Add(/*Spread Distance = */ new TI.Real());
			Add(new TI.Pad(4));
			Add(/*Maximum Density = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*Start Distance = */ new TI.Real());
			Add(/*Opaque Distance = */ new TI.Real());
			Add(/*Color = */ new TI.RealColor());
			Add(new TI.Pad(4));
			Add(/*Maximum Density = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*Start Distance = */ new TI.Real());
			Add(/*Opaque Distance = */ new TI.Real());
			Add(new TI.Pad(4));
			Add(/*Planar Color = */ new TI.RealColor());
			Add(/*Planar Max Density = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*Planar Override Amount = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*Planar Min Distance Bias = */ new TI.Real());
			Add(new TI.Pad(44));
			Add(/*Patchy Color = */ new TI.RealColor());
			Add(new TI.Pad(12));
			Add(/*Patchy Density = */ new TI.RealBounds(TI.FieldType.RealFractionBounds));
			Add(/*Patchy Distance = */ new TI.RealBounds());
			Add(new TI.Pad(32));
			Add(/*Patchy Fog = */ new TI.TagReference(this, TagGroups.fpch));
			Add(/*Mixers = */ new TI.Block<scenario_atmospheric_fog_mixer_block>(this, 2));
			Add(/*Amount = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*Threshold = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*Brightness = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*Gamma Power = */ new TI.Real());
			Add(/*Camera Immersion Flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2));
		}
		#endregion
	}
	#endregion

	#region scenario
	/// <remarks>NONE of the child blocks have their upgrade functions written, but it really doesn't matter in our case...</remarks>
	[TI.TagGroup((int)TagGroups.Enumerated.scnr, 2, 3, 1476)]
	public partial class scenario_group : TI.Definition
	{
		#region scenario_child_scenario_block
		[TI.Definition(1, 32)]
		public class scenario_child_scenario_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_child_scenario_block() : base(2)
			{
				Add(/*Child Scenario = */ new TI.TagReference(this, TagGroups.scnr));
				Add(new TI.Pad(16));
			}
			#endregion
		}
		#endregion

		#region scenario_function_block
		[TI.Definition(1, 120)]
		public class scenario_function_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_function_block() : base(20)
			{
				Add(/*Flags = */ new TI.Flags());
				Add(/*Name = */ new TI.String());
				Add(/*Period = */ new TI.Real());
				Add(/*Scale Period By = */ new TI.BlockIndex()); // 1 scenario_function_block
				Add(/*Function = */ new TI.Enum());
				Add(/*Scale Function By = */ new TI.BlockIndex()); // 1 scenario_function_block
				Add(/*Wobble Function = */ new TI.Enum());
				Add(/*Wobble Period = */ new TI.Real());
				Add(/*Wobble Magnitude = */ new TI.Real());
				Add(/*Square Wave Threshold = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*Step Count = */ new TI.ShortInteger());
				Add(/*Map to = */ new TI.Enum());
				Add(/*Sawtooth Count = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(/*Scale Result by = */ new TI.BlockIndex()); // 1 scenario_function_block
				Add(/*Bounds Mode = */ new TI.Enum());
				Add(/*Bounds = */ new TI.RealBounds(TI.FieldType.RealFractionBounds));
				Add(new TI.Pad(4 + 2));
				Add(/*Turn Off with = */ new TI.BlockIndex()); // 1 scenario_function_block
				Add(new TI.Pad(16 + 16));
			}
			#endregion
		}
		#endregion

		#region scenario_profiles_block
		[TI.Definition(1, 84)]
		public class scenario_profiles_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_profiles_block() : base(14)
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
				Add(/*Starting <unknown> Grenade Count = */ new TI.ByteInteger());
				Add(/*Starting <unknown> Grenade Count = */ new TI.ByteInteger());
				Add(new TI.UselessPad(20));
			}
			#endregion
		}
		#endregion

		#region scenario_players_block
		[TI.Definition(1, 52)]
		public class scenario_players_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_players_block() : base(16)
			{
				Add(/*Position = */ new TI.RealPoint3D());
				Add(/*Facing = */ new TI.Real(TI.FieldType.Angle));
				Add(/*Team Designator = */ new TI.Enum());
				Add(/*BSP Index = */ new TI.ShortInteger());
				Add(/*Game Type 1 = */ new TI.Enum());
				Add(/*Game Type 2 = */ new TI.Enum());
				Add(/*Game Type 3 = */ new TI.Enum());
				Add(/*Game Type 4 = */ new TI.Enum());
				Add(/*Spawn Type 0 = */ new TI.Enum());
				Add(/*Spawn Type 1 = */ new TI.Enum());
				Add(/*Spawn Type 2 = */ new TI.Enum());
				Add(/*Spawn Type 3 = */ new TI.Enum());
				Add(/* = */ new TI.StringId());
				Add(/* = */ new TI.StringId());
				Add(/*Campaign Player Type = */ new TI.Enum());
				Add(new TI.Pad(6));
			}
			#endregion
		}
		#endregion

		#region scenario_netpoints_block
		[TI.Definition(2, 32)]
		public class scenario_netpoints_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_netpoints_block() : base(8)
			{
				Add(/*Position = */ new TI.RealPoint3D());
				Add(/*Facing = */ new TI.Real(TI.FieldType.Angle));
				Add(/*Type = */ new TI.Enum());
				Add(/*Team Designator = */ new TI.Enum());
				Add(/*Identifier = */ new TI.ShortInteger());
				Add(/*Flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/* = */ new TI.StringId());
				Add(/* = */ new TI.StringId());
			}
			#endregion
		}
		#endregion

		#region scenario_netgame_equipment_block
		[TI.Definition(1, 152)]
		public class scenario_netgame_equipment_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_netgame_equipment_block() : base(15)
			{
				Add(/*Flags = */ new TI.Flags());
				Add(/*Game Type 1 = */ new TI.Enum());
				Add(/*Game Type 2 = */ new TI.Enum());
				Add(/*Game Type 3 = */ new TI.Enum());
				Add(/*Game Type 4 = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*Spawn Time (in seconds, 0 = default) = */ new TI.ShortInteger());
				Add(/*Respawn on Empty Time = */ new TI.ShortInteger());
				Add(/*Respawn Timer Starts = */ new TI.Enum());
				Add(/*Classification = */ new TI.Enum(TI.FieldType.ByteEnum));
				Add(new TI.Pad(3 + 40));
				Add(/*Position = */ new TI.RealPoint3D());
				Add(/*Orientation = */ new TI.Struct<scenario_netgame_equipment_orientation_struct>(this));
				Add(/*Item/Vehicle Collection = */ new TI.TagReference(this)); // itmc,vehc,
				Add(new TI.Pad(48));
			}
			#endregion
		}
		#endregion

		#region scenario_starting_equipment_block
		[TI.Definition(1, 204)]
		public class scenario_starting_equipment_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_starting_equipment_block() : base(13)
			{
				Add(/*Flags = */ new TI.Flags());
				Add(/*Game Type 1 = */ new TI.Enum());
				Add(/*Game Type 2 = */ new TI.Enum());
				Add(/*Game Type 3 = */ new TI.Enum());
				Add(/*Game Type 4 = */ new TI.Enum());
				Add(new TI.Pad(48));
				Add(/*Item Collection 1 = */ new TI.TagReference(this, TagGroups.itmc));
				Add(/*Item Collection 2 = */ new TI.TagReference(this, TagGroups.itmc));
				Add(/*Item Collection 3 = */ new TI.TagReference(this, TagGroups.itmc));
				Add(/*Item Collection 4 = */ new TI.TagReference(this, TagGroups.itmc));
				Add(/*Item Collection 5 = */ new TI.TagReference(this, TagGroups.itmc));
				Add(/*Item Collection 6 = */ new TI.TagReference(this, TagGroups.itmc));
				Add(new TI.Pad(48));
			}
			#endregion
		}
		#endregion

		#region scenario_bsp_switch_trigger_volume_block
		[TI.Definition(1, 14)]
		public class scenario_bsp_switch_trigger_volume_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_bsp_switch_trigger_volume_block() : base(4)
			{
				Add(/*Trigger Volume = */ new TI.BlockIndex()); // 1 scenario_trigger_volume_block
				Add(/*Source = */ new TI.ShortInteger());
				Add(/*Destination = */ new TI.ShortInteger());
				Add(new TI.Pad(2 + 2 + 2 + 2));
			}
			#endregion
		}
		#endregion

		#region scenario_detail_object_collection_palette_block
		[TI.Definition(1, 48)]
		public class scenario_detail_object_collection_palette_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_detail_object_collection_palette_block() : base(2)
			{
				Add(/*Name = */ new TI.TagReference(this, TagGroups.dobc));
				Add(new TI.Pad(32));
			}
			#endregion
		}
		#endregion

		#region old_unused_strucure_physics_block
		[TI.Definition(1, 60)]
		public class old_unused_strucure_physics_block : TI.Definition
		{
			#region old_unused_object_identifiers_block
			[TI.Definition(1, 8)]
			public class old_unused_object_identifiers_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public old_unused_object_identifiers_block() : base(1)
				{
					Add(/*Object ID = */ new TI.Struct<scenario_object_id_struct>(this));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public old_unused_strucure_physics_block() : base(5)
			{
				Add(/*mopp code = */ new TI.Data(this));
				Add(/*Evironment Object Identifiers = */ new TI.Block<old_unused_object_identifiers_block>(this, 2048));
				Add(new TI.Pad(4));
				Add(/*mopp Bounds Min = */ new TI.RealPoint3D());
				Add(/*mopp Bounds Max = */ new TI.RealPoint3D());
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
				public dynamic_spawn_zone_overload_block() : base(5)
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
				#region Fields
				#endregion

				#region Ctor
				public static_spawn_zone_block() : base(7)
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
			public scenario_spawn_data_block() : base(7)
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

		#region scenario_planar_fog_palette
		[TI.Definition(1, 24)]
		public class scenario_planar_fog_palette : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_planar_fog_palette() : base(3)
			{
				Add(/*Name = */ new TI.StringId());
				Add(/*Planar Fog = */ new TI.TagReference(this, TagGroups.fog));
				Add(new TI.Pad(2 + 2));
			}
			#endregion
		}
		#endregion

		#region scenario_level_data_block
		[TI.Definition(1, 40)]
		public class scenario_level_data_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_level_data_block() : base(3)
			{
				Add(/*Level Description = */ new TI.TagReference(this, TagGroups.unic));
				Add(/*Campaign Level Data = */ new TI.Block<global_ui_campaign_level_block>(this, 20));
				Add(/*Multiplayer = */ new TI.Block<global_ui_multiplayer_level_block>(this, 50));
			}
			#endregion
		}
		#endregion

		#region ai_scenario_mission_dialogue_block
		[TI.Definition(1, 16)]
		public class ai_scenario_mission_dialogue_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public ai_scenario_mission_dialogue_block() : base(1)
			{
				Add(/*mission dialogue = */ new TI.TagReference(this, TagGroups.mdlg));
			}
			#endregion
		}
		#endregion

		#region scenario_interpolator_block
		[TI.Definition(1, 28)]
		public class scenario_interpolator_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_interpolator_block() : base(5)
			{
				Add(/*Name = */ new TI.StringId());
				Add(/*Accelerator Name = */ new TI.StringId());
				Add(/*Multiplier Name = */ new TI.StringId());
				Add(/*Function = */ new TI.Struct<scalar_function_struct>(this));
				Add(new TI.Skip(2 + 2));
			}
			#endregion
		}
		#endregion

		#region scenario_screen_effect_reference_block
		[TI.Definition(1, 44)]
		public class scenario_screen_effect_reference_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public scenario_screen_effect_reference_block() : base(5)
			{
				Add(new TI.Pad(16));
				Add(/*Screen Effect = */ new TI.TagReference(this, TagGroups.egor));
				Add(/*Primary Input = */ new TI.StringId());
				Add(/*Secondary Input = */ new TI.StringId());
				Add(new TI.Skip(2 + 2));
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
			public scenario_simulation_definition_table_block() : base(1)
			{
				Add(new TI.Skip(4));
			}
			#endregion
		}
		#endregion

		#region Fields
		#region misc
		public TI.TagReference DontUse;
		public TI.Block<scenario_sky_reference_block> Skies;
		public TI.Enum Type;
		public TI.Flags Flags;
		public TI.Block<scenario_child_scenario_block> ChildScenarios;
		public TI.Real LocalNorth;
		public TI.Block<predicted_resource_block> PredictedResources;
		public TI.Block<scenario_function_block> Functions;
		public TI.Data EditorScenarioData;
		public TI.Block<editor_comment_block> Comments;
		#endregion
		#region scenario objects
		public TI.Block<dont_use_me_scenario_environment_object_block> DontUseObjects;
		public TI.Block<scenario_object_names_block> ObjectNames;
		public TI.Block<scenario_scenery_block> Scenery;
		public TI.Block<scenario_object_palette_block> SceneryPalette;
		public TI.Block<scenario_biped_block> Bipeds;
		public TI.Block<scenario_object_palette_block> BipedsPalette;
		public TI.Block<scenario_vehicle_block> Vehicles;
		public TI.Block<scenario_object_palette_block> VehiclePalette;
		public TI.Block<scenario_equipment_block> Equipment;
		public TI.Block<scenario_object_palette_block> EquipmentPalette;
		public TI.Block<scenario_weapon_block> Weapons;
		public TI.Block<scenario_object_palette_block> WeaponPalette;
		public TI.Block<device_group_block> DeviceGroups;
		public TI.Block<scenario_machine_block> Machines;
		public TI.Block<scenario_object_palette_block> MachinePalette;
		public TI.Block<scenario_control_block> Controls;
		public TI.Block<scenario_object_palette_block> ControlPalette;
		public TI.Block<scenario_light_fixture_block> LightFixtures;
		public TI.Block<scenario_object_palette_block> LightFixturesPalette;
		public TI.Block<scenario_sound_scenery_block> SoundScenery;
		public TI.Block<scenario_object_palette_block> SoundSceneryPalette;
		public TI.Block<scenario_light_block> LightVolumes;
		public TI.Block<scenario_object_palette_block> LightVolumesPalette;
		#endregion
		public TI.Block<scenario_profiles_block> PlayerStartingProfile;
		public TI.Block<scenario_players_block> PlayerStartingLocations;
		public TI.Block<scenario_trigger_volume_block> KillTriggerVolumes;
		public TI.Block<recorded_animation_block> RecordedAnimations;
		public TI.Block<scenario_netpoints_block> NetgameFlags;
		public TI.Block<scenario_netgame_equipment_block> NetgameEquipment;
		public TI.Block<scenario_starting_equipment_block> StartingEquipment;
		public TI.Block<scenario_bsp_switch_trigger_volume_block> BspSwitchTriggerVolumes;
		public TI.Block<scenario_decals_block> Decals;
		public TI.Block<scenario_decal_palette_block> DecalsPalette;
		public TI.Block<scenario_detail_object_collection_palette_block> DetailObjectCollectionPalette;
		#region scenario ai
		public TI.Block<style_palette_block> StylePalette;
		public TI.Block<squad_groups_block> SquadGroups;
		public TI.Block<squads_block> Squads;
		public TI.Block<zone_block> Zones;
		public TI.Block<ai_scene_block> MissionScenes;
		public TI.Block<character_palette_block> CharacterPalette;
		public TI.Block<pathfinding_data_block> AIPathfindingData;
		public TI.Block<ai_animation_reference_block> AIAnimationReferences;
		public TI.Block<ai_script_reference_block> AIScriptReferences;
		public TI.Block<ai_recording_reference_block> AIRecordingReferences;
		public TI.Block<ai_conversation_block> AIConversations;
		#endregion
		#region scripting and cinematics
		public TI.Data HsSyntaxData;
		public TI.Data HsStringData;
		public TI.Block<hs_scripts_block> HsScripts;
		public TI.Block<hs_globals_block> HsGlobals;
		public TI.Block<hs_references_block> References;
		public TI.Block<hs_source_files_block> SourceFiles;
		public TI.Block<cs_script_data_block> ScriptingData;
		public TI.Block<scenario_cutscene_flag_block> CutsceneFlags;
		public TI.Block<scenario_cutscene_camera_point_block> CutsceneCameraPoints;
		public TI.Block<scenario_cutscene_title_block> CutsceneTitles;
		public TI.TagReference CustomObjectNames;
		public TI.TagReference ChapterTitleText;
		#endregion
		public TI.TagReference HUDMessages;
		public TI.Block<scenario_structure_bsp_reference_block> StructureBsps;
		public TI.Block<scenario_resources_block> ScenarioResources;
		TI.Block<old_unused_strucure_physics_block> ScenarioResourcesUnused;
		public TI.Block<hs_unit_seat_block> HsUnitSeats;
		public TI.Block<scenario_kill_trigger_volumes_block> ScenarioKillTriggers;
		public TI.Block<syntax_datum_block> HsScriptDatums;
		public TI.Block<orders_block> Orders;
		public TI.Block<triggers_block> Triggers;
		public TI.Block<structure_bsp_background_sound_palette_block> BackgroundSoundPalette;
		public TI.Block<structure_bsp_sound_environment_palette_block> SoundEnvironmentPalette;
		public TI.Block<structure_bsp_weather_palette_block> WeatherPalette;
		public TI.Block<scenario_cluster_data_block> ScenarioClusterData;
		public TI.Block<scenario_spawn_data_block> SpawnData;
		public TI.TagReference SoundEffectCollection;
		public TI.Block<scenario_crate_block> Crates;
		public TI.Block<scenario_object_palette_block> CratesPalette;
		public TI.TagReference GlobalLighting;
		public TI.Block<scenario_atmospheric_fog_palette> AtmosphericFogPalette;
		public TI.Block<scenario_planar_fog_palette> PlanarFogPalette;
		public TI.Block<flock_definition_block> Flocks;
		public TI.TagReference Subtitles;
		public TI.Block<decorator_placement_definition_block> Decorators;
		public TI.Block<scenario_creature_block> Creatures;
		public TI.Block<scenario_object_palette_block> CreaturesPalette;
		public TI.Block<scenario_decorator_set_palette_entry_block> DecoratorsPalette;
		public TI.Block<scenario_bsp_switch_transition_volume_block> BspTransitionVolumes;
		public TI.Block<scenario_structure_bsp_spherical_harmonic_lighting_block> StructureBspLighting;
		public TI.Block<g_scenario_editor_folder_block> EditorFolders;
		public TI.Block<scenario_level_data_block> LevelData;
		public TI.TagReference TerritoryLocationNames;
		public TI.Block<ai_scenario_mission_dialogue_block> MissionDialogue;
		public TI.TagReference Objectives;
		public TI.Block<scenario_interpolator_block> Interpolators;
		public TI.Block<hs_references_block> SharedReferences;
		public TI.Block<scenario_screen_effect_reference_block> ScreenEffectReferences;
		public TI.Block<scenario_simulation_definition_table_block> SimulationDefinitionTable;
		#endregion


		#region Upgrade
		private void version1_construct()
		{
			#region misc
			Add(DontUse = new TI.TagReference(this, TagGroups.sbsp));
			Add(/*Will not use.*/ new TI.TagReference(this, TagGroups.sbsp));
			Add(/*Cannot use.*/ new TI.TagReference(this, TagGroups.sky_));
			Add(Skies = new TI.Block<scenario_sky_reference_block>(this, 32));
			Add(Type = new TI.Enum());
			Add(Flags = new TI.Flags(TI.FieldType.WordFlags));
			Add(ChildScenarios = new TI.Block<scenario_child_scenario_block>(this, 16));
			Add(LocalNorth = new TI.Real(TI.FieldType.Angle));
			Add(new TI.Pad(20 + 136));
			Add(PredictedResources = new TI.Block<predicted_resource_block>(this, 2048));
			Add(Functions = new TI.Block<scenario_function_block>(this, 32));
			Add(EditorScenarioData = new TI.Data(this));
			Add(Comments = new TI.Block<editor_comment_block>(this, 65536));
			#endregion
			#region scenario objects
			Add(DontUseObjects = new TI.Block<dont_use_me_scenario_environment_object_block>(this, 4096));
			Add(new TI.Pad(212));
			version_construct_add_scenario_object_blocks();
			Add(new TI.Pad(60));
			#endregion
			Add(PlayerStartingProfile = new TI.Block<scenario_profiles_block>(this, 256));
			Add(PlayerStartingLocations = new TI.Block<scenario_players_block>(this, 256));
			Add(KillTriggerVolumes = new TI.Block<scenario_trigger_volume_block>(this, 256));
			Add(RecordedAnimations = new TI.Block<recorded_animation_block>(this, 1024));
			Add(NetgameFlags = new TI.Block<scenario_netpoints_block>(this, 200));
			Add(NetgameEquipment = new TI.Block<scenario_netgame_equipment_block>(this, 100));
			Add(StartingEquipment = new TI.Block<scenario_starting_equipment_block>(this, 200));
			Add(BspSwitchTriggerVolumes = new TI.Block<scenario_bsp_switch_trigger_volume_block>(this, 256));
			Add(Decals = new TI.Block<scenario_decals_block>(this, 65536));
			Add(DecalsPalette = new TI.Block<scenario_decal_palette_block>(this, 128));
			Add(DetailObjectCollectionPalette = new TI.Block<scenario_detail_object_collection_palette_block>(this, 32));
			Add(new TI.Pad(36));
			#region scenario ai
			Add(StylePalette = new TI.Block<style_palette_block>(this, 50));
			Add(SquadGroups = new TI.Block<squad_groups_block>(this, 100));
			Add(Squads = new TI.Block<squads_block>(this, 335));
			Add(Zones = new TI.Block<zone_block>(this, 128));
			Add(TI.Pad.BlockHalo1); // actor_palette_block
			//Add(MissionScenes = new TI.Block<ai_scene_block>(this, 100));
			Add(CharacterPalette = new TI.Block<character_palette_block>(this, 64));
			Add(AIPathfindingData = new TI.Block<pathfinding_data_block>(this, 16));
			Add(AIAnimationReferences = new TI.Block<ai_animation_reference_block>(this, 128));
			Add(AIScriptReferences = new TI.Block<ai_script_reference_block>(this, 128));
			Add(AIRecordingReferences = new TI.Block<ai_recording_reference_block>(this, 128));
			Add(AIConversations = new TI.Block<ai_conversation_block>(this, 128));
			#endregion
			#region scripting and cinematics
			Add(HsSyntaxData = new TI.Data(this));
			Add(HsStringData = new TI.Data(this));
			Add(HsScripts = new TI.Block<hs_scripts_block>(this, 1024));
			Add(HsGlobals = new TI.Block<hs_globals_block>(this, 256));
			Add(References = new TI.Block<hs_references_block>(this, 512));
			Add(SourceFiles = new TI.Block<hs_source_files_block>(this, 8));
			Add(ScriptingData = new TI.Block<cs_script_data_block>(this, 1));
			Add(new TI.Pad(12));
			Add(CutsceneFlags = new TI.Block<scenario_cutscene_flag_block>(this, 512));
			Add(CutsceneCameraPoints = new TI.Block<scenario_cutscene_camera_point_block>(this, 512));
			Add(CutsceneTitles = new TI.Block<scenario_cutscene_title_block>(this, 128));
			Add(new TI.Pad(108));
			Add(CustomObjectNames = new TI.TagReference(this, TagGroups.unic));
			//Add(ChapterTitleText = new TI.TagReference(this, TagGroups.unic));
			#endregion
			Add(/*In-Game Help Text*/ new TI.TagReference(this, TagGroups.unic));
			Add(HUDMessages = new TI.TagReference(this, TagGroups.hmt_));
			Add(StructureBsps = new TI.Block<scenario_structure_bsp_reference_block>(this, 16));

			//SceneryPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.scen);
			//BipedsPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.bipd);
			//VehiclePalette.Definition.Name.ResetReferenceGroupTag(TagGroups.vehi);
			//EquipmentPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.eqip);
			//WeaponPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.weap);
			//MachinePalette.Definition.Name.ResetReferenceGroupTag(TagGroups.mach);
			//ControlPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.ctrl);
			//LightFixturesPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.lifi);
		}

		internal override bool Upgrade()
		{
			TI.VersionCtorAttribute attr = base.VersionCtorAttributeUsed;
			if (attr.Major == 1)
			{
				switch (attr.Minor)
				{
					case 1428:
						Add(Interpolators = new TI.Block<scenario_interpolator_block>(this, 16));
						Add(SharedReferences = new TI.Block<hs_references_block>(this, 512));
						Add(ScreenEffectReferences = new TI.Block<scenario_screen_effect_reference_block>(this, 16));
						Add(SimulationDefinitionTable = new TI.Block<scenario_simulation_definition_table_block>(this, 512));
						break;

					case 1456:
						this.Clear();
						#region Old fields
						Add(
						#region misc
								DontUse
							,	Skies
							,	Type
							,	Flags
							,	ChildScenarios
							, LocalNorth
							, PredictedResources
							, Functions
							, EditorScenarioData
							, Comments
						#endregion
						#region scenario objects
							,	DontUseObjects
							,	ObjectNames
							,	Scenery
							,	SceneryPalette
							,	Bipeds
							,	BipedsPalette
							,	Vehicles
							,	VehiclePalette
							,	Equipment
							,	EquipmentPalette
							,	Weapons
							,	WeaponPalette
							,	DeviceGroups
							,	Machines
							,	MachinePalette
							,	Controls
							,	ControlPalette
							,	LightFixtures
							,	LightFixturesPalette
							,	SoundScenery
							,	SoundSceneryPalette
							,	LightVolumes
							,	LightVolumesPalette
						#endregion
							,	PlayerStartingProfile
							,	PlayerStartingLocations
							,	KillTriggerVolumes
							,	RecordedAnimations
							,	NetgameFlags
							,	NetgameEquipment
							,	StartingEquipment
							,	BspSwitchTriggerVolumes
							,	Decals
							,	DecalsPalette
							,	DetailObjectCollectionPalette
						#region scenario ai
							,	StylePalette
							,	SquadGroups
							,	Squads
							,	Zones
							,	MissionScenes = new TI.Block<ai_scene_block>(this, 100)
							,	CharacterPalette
							,	AIPathfindingData
							,	AIAnimationReferences
							,	AIScriptReferences
							,	AIRecordingReferences
							,	AIConversations
						#endregion
						#region scripting and cinematics
							,	HsSyntaxData
							,	HsStringData
							,	HsScripts
							,	HsGlobals
							,	References
							,	SourceFiles
							,	ScriptingData
							,	CutsceneFlags
							,	CutsceneCameraPoints
							,	CutsceneTitles
							,	CustomObjectNames
							,	ChapterTitleText = new TI.TagReference(this, TagGroups.unic)
						#endregion
							,	HUDMessages
							,	StructureBsps
						);
						#endregion
						#region New fields
						Add(ScenarioResources = new TI.Block<scenario_resources_block>(this, 1));
						Add(ScenarioResourcesUnused = new TI.Block<old_unused_strucure_physics_block>(this, 16));
						Add(HsUnitSeats = new TI.Block<hs_unit_seat_block>(this, 65536));
						Add(ScenarioKillTriggers = new TI.Block<scenario_kill_trigger_volumes_block>(this, 256));
						Add(HsScriptDatums = new TI.Block<syntax_datum_block>(this, 36864));
						Add(Orders = new TI.Block<orders_block>(this, 300));
						Add(Triggers = new TI.Block<triggers_block>(this, 256));
						Add(BackgroundSoundPalette = new TI.Block<structure_bsp_background_sound_palette_block>(this, 64));
						Add(SoundEnvironmentPalette = new TI.Block<structure_bsp_sound_environment_palette_block>(this, 64));
						Add(WeatherPalette = new TI.Block<structure_bsp_weather_palette_block>(this, 32));
						version_construct_add_unnamed_null_blocks();
						Add(ScenarioClusterData = new TI.Block<scenario_cluster_data_block>(this, 16));
						version_construct_add_unnamed_array();
						Add(SpawnData = new TI.Block<scenario_spawn_data_block>(this, 1));
						Add(SoundEffectCollection = new TI.TagReference(this, TagGroups.sfx_));
						Add(Crates = new TI.Block<scenario_crate_block>(this, 1024));
						Add(CratesPalette = new TI.Block<scenario_object_palette_block>(this, 256));
						Add(GlobalLighting = new TI.TagReference(this, TagGroups.gldf));
						Add(AtmosphericFogPalette = new TI.Block<scenario_atmospheric_fog_palette>(this, 127));
						Add(PlanarFogPalette = new TI.Block<scenario_planar_fog_palette>(this, 127));
						Add(Flocks = new TI.Block<flock_definition_block>(this, 20));
						Add(Subtitles = new TI.TagReference(this, TagGroups.unic));
						Add(Decorators = new TI.Block<decorator_placement_definition_block>(this, 1));
						Add(Creatures = new TI.Block<scenario_creature_block>(this, 128));
						Add(CreaturesPalette = new TI.Block<scenario_object_palette_block>(this, 256));
						Add(DecoratorsPalette = new TI.Block<scenario_decorator_set_palette_entry_block>(this, 32));
						Add(BspTransitionVolumes = new TI.Block<scenario_bsp_switch_transition_volume_block>(this, 256));
						Add(StructureBspLighting = new TI.Block<scenario_structure_bsp_spherical_harmonic_lighting_block>(this, 16));
						Add(EditorFolders = new TI.Block<g_scenario_editor_folder_block>(this, 32767));
						Add(LevelData = new TI.Block<scenario_level_data_block>(this, 1));
						Add(TerritoryLocationNames = new TI.TagReference(this, TagGroups.unic));
						Add(new TI.Pad(8));
						Add(MissionDialogue = new TI.Block<ai_scenario_mission_dialogue_block>(this, 1));
						Add(Objectives = new TI.TagReference(this, TagGroups.unic));
						Add(Interpolators = new TI.Block<scenario_interpolator_block>(this, 16));
						Add(SharedReferences = new TI.Block<hs_references_block>(this, 512));
						Add(ScreenEffectReferences = new TI.Block<scenario_screen_effect_reference_block>(this, 16));
						Add(SimulationDefinitionTable = new TI.Block<scenario_simulation_definition_table_block>(this, 512));
						#endregion

						//CratesPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.bloc);
						//CreaturesPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.crea);
						break;
				}
			}
			return true;
		}
		#endregion

		#region Ctor
		private void version_construct_add_scenario_object_blocks()
		{
			Add(ObjectNames = new TI.Block<scenario_object_names_block>(this, 640));
			Add(Scenery = new TI.Block<scenario_scenery_block>(this, 2000));
			Add(SceneryPalette = new TI.Block<scenario_object_palette_block>(this, 256));
			Add(Bipeds = new TI.Block<scenario_biped_block>(this, 128));
			Add(BipedsPalette = new TI.Block<scenario_object_palette_block>(this, 256));
			Add(Vehicles = new TI.Block<scenario_vehicle_block>(this, 256));
			Add(VehiclePalette = new TI.Block<scenario_object_palette_block>(this, 256));
			Add(Equipment = new TI.Block<scenario_equipment_block>(this, 256));
			Add(EquipmentPalette = new TI.Block<scenario_object_palette_block>(this, 256));
			Add(Weapons = new TI.Block<scenario_weapon_block>(this, 128));
			Add(WeaponPalette = new TI.Block<scenario_object_palette_block>(this, 256));
			Add(DeviceGroups = new TI.Block<device_group_block>(this, 128));
			Add(Machines = new TI.Block<scenario_machine_block>(this, 400));
			Add(MachinePalette = new TI.Block<scenario_object_palette_block>(this, 256));
			Add(Controls = new TI.Block<scenario_control_block>(this, 100));
			Add(ControlPalette = new TI.Block<scenario_object_palette_block>(this, 256));
			Add(LightFixtures = new TI.Block<scenario_light_fixture_block>(this, 500));
			Add(LightFixturesPalette = new TI.Block<scenario_object_palette_block>(this, 256));
			Add(SoundScenery = new TI.Block<scenario_sound_scenery_block>(this, 256));
			Add(SoundSceneryPalette = new TI.Block<scenario_object_palette_block>(this, 256));
			Add(LightVolumes = new TI.Block<scenario_light_block>(this, 500));
			Add(LightVolumesPalette = new TI.Block<scenario_object_palette_block>(this, 256));
		}
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

		public void version_construct(BlamVersion engine)
		{
			bool is_alpha = engine == BlamVersion.Halo2_Alpha;

			#region misc
			Add(DontUse = new TI.TagReference(this, TagGroups.sbsp));
			Add(Skies = new TI.Block<scenario_sky_reference_block>(this, 32));
			Add(Type = new TI.Enum());
			Add(Flags = new TI.Flags(TI.FieldType.WordFlags));
			Add(ChildScenarios = new TI.Block<scenario_child_scenario_block>(this, 16));
			Add(LocalNorth = new TI.Real(TI.FieldType.Angle));
			Add(PredictedResources = new TI.Block<predicted_resource_block>(this, 2048));
			Add(Functions = new TI.Block<scenario_function_block>(this, 32));
			Add(EditorScenarioData = new TI.Data(this));
			Add(Comments = new TI.Block<editor_comment_block>(this, 65536));
			#endregion
			#region scenario objects
			Add(DontUseObjects = new TI.Block<dont_use_me_scenario_environment_object_block>(this, 4096));
			version_construct_add_scenario_object_blocks();
			#endregion
			Add(PlayerStartingProfile = new TI.Block<scenario_profiles_block>(this, 256));
			Add(PlayerStartingLocations = new TI.Block<scenario_players_block>(this, 256));
			Add(KillTriggerVolumes = new TI.Block<scenario_trigger_volume_block>(this, 256));
			Add(RecordedAnimations = new TI.Block<recorded_animation_block>(this, 1024));
			Add(NetgameFlags = new TI.Block<scenario_netpoints_block>(this, 200));
			Add(NetgameEquipment = new TI.Block<scenario_netgame_equipment_block>(this, 100));
			Add(StartingEquipment = new TI.Block<scenario_starting_equipment_block>(this, 200));
			Add(BspSwitchTriggerVolumes = new TI.Block<scenario_bsp_switch_trigger_volume_block>(this, 256));
			Add(Decals = new TI.Block<scenario_decals_block>(this, 65536));
			Add(DecalsPalette = new TI.Block<scenario_decal_palette_block>(this, 128));
			Add(DetailObjectCollectionPalette = new TI.Block<scenario_detail_object_collection_palette_block>(this, 32));
			#region scenario ai
			Add(StylePalette = new TI.Block<style_palette_block>(this, 50));
			Add(SquadGroups = new TI.Block<squad_groups_block>(this, 100));
			Add(Squads = new TI.Block<squads_block>(this, 335));
			Add(Zones = new TI.Block<zone_block>(this, 128));
			Add(MissionScenes = new TI.Block<ai_scene_block>(this, 100));
			Add(CharacterPalette = new TI.Block<character_palette_block>(this, 64));
			Add(AIPathfindingData = new TI.Block<pathfinding_data_block>(this, 16));
			Add(AIAnimationReferences = new TI.Block<ai_animation_reference_block>(this, 128));
			Add(AIScriptReferences = new TI.Block<ai_script_reference_block>(this, 128));
			Add(AIRecordingReferences = new TI.Block<ai_recording_reference_block>(this, 128));
			Add(AIConversations = new TI.Block<ai_conversation_block>(this, 128));
			#endregion
			#region scripting and cinematics
			Add(HsSyntaxData = new TI.Data(this));
			Add(HsStringData = new TI.Data(this));
			Add(HsScripts = new TI.Block<hs_scripts_block>(this, 1024));
			Add(HsGlobals = new TI.Block<hs_globals_block>(this, 256));
			Add(References = new TI.Block<hs_references_block>(this, 512));
			Add(SourceFiles = new TI.Block<hs_source_files_block>(this, 8));
			Add(ScriptingData = new TI.Block<cs_script_data_block>(this, 1));
			Add(CutsceneFlags = new TI.Block<scenario_cutscene_flag_block>(this, 512));
			Add(CutsceneCameraPoints = new TI.Block<scenario_cutscene_camera_point_block>(this, 512));
			Add(CutsceneTitles = new TI.Block<scenario_cutscene_title_block>(this, 128));
			Add(CustomObjectNames = new TI.TagReference(this, TagGroups.unic));
			Add(ChapterTitleText = new TI.TagReference(this, TagGroups.unic));
			#endregion
			Add(HUDMessages = new TI.TagReference(this, TagGroups.hmt_));
			Add(StructureBsps = new TI.Block<scenario_structure_bsp_reference_block>(this, 16));
			Add(ScenarioResources = new TI.Block<scenario_resources_block>(this, 1));
			Add(ScenarioResourcesUnused = new TI.Block<old_unused_strucure_physics_block>(this, 16));
			Add(HsUnitSeats = new TI.Block<hs_unit_seat_block>(this, 65536));
			Add(ScenarioKillTriggers = new TI.Block<scenario_kill_trigger_volumes_block>(this, 256));
			Add(HsScriptDatums = new TI.Block<syntax_datum_block>(this, 36864));
			Add(Orders = new TI.Block<orders_block>(this, 300));
			Add(Triggers = new TI.Block<triggers_block>(this, 256));
			Add(BackgroundSoundPalette = new TI.Block<structure_bsp_background_sound_palette_block>(this, 64));
			Add(SoundEnvironmentPalette = new TI.Block<structure_bsp_sound_environment_palette_block>(this, 64));
			Add(WeatherPalette = new TI.Block<structure_bsp_weather_palette_block>(this, 32));
			version_construct_add_unnamed_null_blocks();
			Add(ScenarioClusterData = new TI.Block<scenario_cluster_data_block>(this, 16));
			version_construct_add_unnamed_array();
			Add(SpawnData = new TI.Block<scenario_spawn_data_block>(this, 1));
			Add(SoundEffectCollection = new TI.TagReference(this, TagGroups.sfx_));
			Add(Crates = new TI.Block<scenario_crate_block>(this, 1024));
			Add(CratesPalette = new TI.Block<scenario_object_palette_block>(this, 256));
			Add(GlobalLighting = new TI.TagReference(this, TagGroups.gldf));
			Add(AtmosphericFogPalette = new TI.Block<scenario_atmospheric_fog_palette>(this, 127));
			Add(PlanarFogPalette = new TI.Block<scenario_planar_fog_palette>(this, 127));
			Add(Flocks = new TI.Block<flock_definition_block>(this, 20));
			Add(Subtitles = new TI.TagReference(this, TagGroups.unic));
			Add(Decorators = new TI.Block<decorator_placement_definition_block>(this, 1));
			Add(Creatures = new TI.Block<scenario_creature_block>(this, 128));
			Add(CreaturesPalette = new TI.Block<scenario_object_palette_block>(this, 256));
			Add(DecoratorsPalette = new TI.Block<scenario_decorator_set_palette_entry_block>(this, 32));
			Add(BspTransitionVolumes = new TI.Block<scenario_bsp_switch_transition_volume_block>(this, 256));
			Add(StructureBspLighting = new TI.Block<scenario_structure_bsp_spherical_harmonic_lighting_block>(this, 16));
			Add(EditorFolders = new TI.Block<g_scenario_editor_folder_block>(this, 32767));
			Add(LevelData = new TI.Block<scenario_level_data_block>(this, 1));
			Add(TerritoryLocationNames = new TI.TagReference(this, TagGroups.unic));
			Add(new TI.Pad(8));
			Add(MissionDialogue = new TI.Block<ai_scenario_mission_dialogue_block>(this, 1));
			Add(Objectives = new TI.TagReference(this, TagGroups.unic));
			if (!is_alpha)
			{
				Add(Interpolators = new TI.Block<scenario_interpolator_block>(this, 16));
				Add(SharedReferences = new TI.Block<hs_references_block>(this, 512));
				Add(ScreenEffectReferences = new TI.Block<scenario_screen_effect_reference_block>(this, 16));
				Add(SimulationDefinitionTable = new TI.Block<scenario_simulation_definition_table_block>(this, 512));
			}

			//SceneryPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.scen);
			//BipedsPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.bipd);
			//VehiclePalette.Definition.Name.ResetReferenceGroupTag(TagGroups.vehi);
			//EquipmentPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.eqip);
			//WeaponPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.weap);
			//MachinePalette.Definition.Name.ResetReferenceGroupTag(TagGroups.mach);
			//ControlPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.ctrl);
			//LightFixturesPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.lifi);
			//LightVolumesPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.ligh);
			//CratesPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.bloc);
			//CreaturesPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.crea);
		}

		public scenario_group() : base(142) { version_construct(BlamVersion.Unknown); }

		[TI.VersionCtorHalo2(1, 1428, BlamVersion.Halo2_Alpha)]
		[TI.VersionCtorHalo2(1, 1456)]
		[TI.VersionCtorHalo2(2, 1476)]
		public scenario_group(int major, int minor)
		{
			if (major == 1)
			{
				switch (minor)
				{
					case 1428:
						version_construct(BlamVersion.Halo2_Alpha);
						break;
					case 1456:
						version1_construct();
						break;
				}
			}
			else if (major == 2)
			{
				switch (minor)
				{
					case 1476: // version 2 is the EXACT same as version 3
						break;
				}
			}
		}
		#endregion
	};
	#endregion

	#region scenario resources
	#region scenario_objects_resource
	public abstract class scenario_objects_resource_group : TI.Definition
	{
		#region Fields
		public TI.Block<scenario_object_names_block> Names;
		public TI.Block<scenario_object_palette_block> Palette;
		public TI.IBlock _Objects;
		#endregion

		protected scenario_objects_resource_group(int field_count) : base(field_count) {}
	};
	#endregion

	#region scenario_ai_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srai, 1, 228)]
	public class scenario_ai_resource_group : TI.Definition
	{
		#region Fields
		public TI.Block<style_palette_block> StylePalette;
		public TI.Block<squad_groups_block> SquadGroups;
		public TI.Block<squads_block> Squads;
		public TI.Block<zone_block> Zones;
		public TI.Block<character_palette_block> CharacterPalette;
		public TI.Block<ai_animation_reference_block> AIAnimationReferences;
		public TI.Block<ai_script_reference_block> AIScriptReferences;
		public TI.Block<ai_recording_reference_block> AIRecordingReferences;
		public TI.Block<ai_conversation_block> AIConversations;
		public TI.Block<cs_script_data_block> ScriptingData;
		public TI.Block<orders_block> Orders;
		public TI.Block<triggers_block> Triggers;
		public TI.Block<scenario_structure_bsp_reference_block> BSPReferences;
		public TI.Block<scenario_object_palette_block> WeaponReferences;
		public TI.Block<scenario_object_palette_block> VehicleReferences;
		public TI.Block<scenario_vehicle_block> VehicleDatumReferences;
		public TI.Block<ai_scene_block> MissionDialogueScenes;
		public TI.Block<flock_definition_block> Flocks;
		public TI.Block<scenario_trigger_volume_block> TriggerVolumeReferences;
		#endregion

		#region Ctor
		public scenario_ai_resource_group() : base(19)
		{
			Add(StylePalette = new TI.Block<style_palette_block>(this, 50));
			Add(SquadGroups = new TI.Block<squad_groups_block>(this, 100));
			Add(Squads = new TI.Block<squads_block>(this, 335));
			Add(Zones = new TI.Block<zone_block>(this, 128));
			Add(CharacterPalette = new TI.Block<character_palette_block>(this, 64));
			Add(AIAnimationReferences = new TI.Block<ai_animation_reference_block>(this, 128));
			Add(AIScriptReferences = new TI.Block<ai_script_reference_block>(this, 128));
			Add(AIRecordingReferences = new TI.Block<ai_recording_reference_block>(this, 128));
			Add(AIConversations = new TI.Block<ai_conversation_block>(this, 128));
			Add(ScriptingData = new TI.Block<cs_script_data_block>(this, 1));
			Add(Orders = new TI.Block<orders_block>(this, 300));
			Add(Triggers = new TI.Block<triggers_block>(this, 256));
			Add(BSPReferences = new TI.Block<scenario_structure_bsp_reference_block>(this, 16));
			Add(WeaponReferences = new TI.Block<scenario_object_palette_block>(this, 256));
			Add(VehicleReferences = new TI.Block<scenario_object_palette_block>(this, 256));
			Add(VehicleDatumReferences = new TI.Block<scenario_vehicle_block>(this, 256));
			Add(MissionDialogueScenes = new TI.Block<ai_scene_block>(this, 100));
			Add(Flocks = new TI.Block<flock_definition_block>(this, 20));
			Add(TriggerVolumeReferences = new TI.Block<scenario_trigger_volume_block>(this, 256));

			//WeaponReferences.Definition.Name.ResetReferenceGroupTag(TagGroups.weap);
			//VehicleReferences.Definition.Name.ResetReferenceGroupTag(TagGroups.vehi);
		}
		#endregion
	};
	#endregion

	#region scenario_bipeds_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srbipd, 1, 76)]
	public class scenario_bipeds_resource_group : scenario_objects_resource_group
	{
		#region Fields
		public TI.Block<scenario_biped_block> Objects;
		#endregion

		#region Ctor
		public scenario_bipeds_resource_group() : base(7)
		{
			Add(Names = new TI.Block<scenario_object_names_block>(this, 640));
			Add(/* = */ new TI.Block<dont_use_me_scenario_environment_object_block>(this, 4096));
			Add(/*Structure References = */ new TI.Block<scenario_structure_bsp_reference_block>(this, 16));
			Add(Palette = new TI.Block<scenario_object_palette_block>(this, 256));
			Add(Objects = new TI.Block<scenario_biped_block>(this, 128));
			Add(/*Next Object ID Salt = */ new TI.LongInteger());
			Add(/*Editor Folders = */ new TI.Block<g_scenario_editor_folder_block>(this, 32767));

			_Objects = Objects;
			//Palette.Definition.Name.ResetReferenceGroupTag(TagGroups.bipd);
		}
		#endregion
	};
	#endregion

	#region scenario_cinematics_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srcine, 1, 36)]
	public class scenario_cinematics_resource_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_cinematics_resource_group() : base(3)
		{
			Add(/*Flags = */ new TI.Block<scenario_cutscene_flag_block>(this, 512));
			Add(/*Camera Points = */ new TI.Block<scenario_cutscene_camera_point_block>(this, 512));
			Add(/*Recorded Animations = */ new TI.Block<recorded_animation_block>(this, 1024));
		}
		#endregion
	};
	#endregion

	#region scenario_cluster_data_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srclut, 1, 60)]
	public class scenario_cluster_data_resource_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_cluster_data_resource_group() : base(5)
		{
			Add(/*Cluster Data = */ new TI.Block<scenario_cluster_data_block>(this, 16));
			Add(/*Background Sound Palette = */ new TI.Block<structure_bsp_background_sound_palette_block>(this, 64));
			Add(/*Sound Environment Palette = */ new TI.Block<structure_bsp_sound_environment_palette_block>(this, 64));
			Add(/*Weather Palette = */ new TI.Block<structure_bsp_weather_palette_block>(this, 32));
			Add(/*Atmospheric Fog Palette = */ new TI.Block<scenario_atmospheric_fog_palette>(this, 127));
		}
		#endregion
	};
	#endregion

	#region scenario_comments_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srcmmt, 1, 12)]
	public class scenario_comments_resource_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_comments_resource_group() : base(1)
		{
			Add(/*Comments = */ new TI.Block<editor_comment_block>(this, 65536));
		}
		#endregion
	};
	#endregion

	#region scenario_creature_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srcrea, 1, 76)]
	public class scenario_creature_resource_group : scenario_objects_resource_group
	{
		#region Fields
		public TI.Block<scenario_creature_block> Objects;
		#endregion

		#region Ctor
		public scenario_creature_resource_group() : base(7)
		{
			Add(Names = new TI.Block<scenario_object_names_block>(this, 640));
			Add(/* = */ new TI.Block<dont_use_me_scenario_environment_object_block>(this, 4096));
			Add(/*Structure References = */ new TI.Block<scenario_structure_bsp_reference_block>(this, 16));
			Add(Palette = new TI.Block<scenario_object_palette_block>(this, 256));
			Add(Objects = new TI.Block<scenario_creature_block>(this, 128));
			Add(/*Next Object ID Salt = */ new TI.LongInteger());
			Add(/*Editor Folders = */ new TI.Block<g_scenario_editor_folder_block>(this, 32767));

			_Objects = Objects;
			//Palette.Definition.Name.ResetReferenceGroupTag(TagGroups.crea);
		}
		#endregion
	};
	#endregion

	#region scenario_decals_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srdeca, 1, 24)]
	public class scenario_decals_resource_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_decals_resource_group() : base(2)
		{
			Add(/*Palette = */ new TI.Block<scenario_decal_palette_block>(this, 128));
			Add(/*Decals = */ new TI.Block<scenario_decals_block>(this, 65536));
		}
		#endregion
	};
	#endregion

	#region scenario_decorators_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srdcrs, 1, 24)]
	public class scenario_decorators_resource_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_decorators_resource_group() : base(2)
		{
			Add(/*Decorator = */ new TI.Block<decorator_placement_definition_block>(this, 1));
			Add(/*Decorator Palette = */ new TI.Block<scenario_decorator_set_palette_entry_block>(this, 32));
		}
		#endregion
	};
	#endregion

	#region scenario_devices_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srdgrp, 1, 144)]
	public class scenario_devices_resource_group : TI.Definition
	{
		#region Fields
		public TI.Block<scenario_object_palette_block> MachinesPalette;
		public TI.Block<scenario_object_palette_block> ControlsPalette;
		public TI.Block<scenario_object_palette_block> LightFixturesPalette;
		#endregion

		#region Ctor
		public scenario_devices_resource_group() : base(14)
		{
			Add(/*Names = */ new TI.Block<scenario_object_names_block>(this, 640));
			Add(/* = */ new TI.Block<dont_use_me_scenario_environment_object_block>(this, 4096));
			Add(/*Structure References = */ new TI.Block<scenario_structure_bsp_reference_block>(this, 16));
			Add(/*Device Groups = */ new TI.Block<device_group_block>(this, 128));
			Add(/*Machines = */ new TI.Block<scenario_machine_block>(this, 400));
			Add(MachinesPalette = new TI.Block<scenario_object_palette_block>(this, 256));
			Add(/*Controls = */ new TI.Block<scenario_control_block>(this, 100));
			Add(ControlsPalette = new TI.Block<scenario_object_palette_block>(this, 256));
			Add(/*Light Fixtures = */ new TI.Block<scenario_light_fixture_block>(this, 500));
			Add(LightFixturesPalette = new TI.Block<scenario_object_palette_block>(this, 256));
			Add(/*next machine id salt = */ new TI.LongInteger());
			Add(/*Next Control ID Salt = */ new TI.LongInteger());
			Add(/*Next Light Fixture ID Salt = */ new TI.LongInteger());
			Add(/*Editor Folders = */ new TI.Block<g_scenario_editor_folder_block>(this, 32767));

			//MachinesPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.mach);
			//ControlsPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.ctrl);
			//LightFixturesPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.lifi);
		}
		#endregion
	};
	#endregion

	#region scenario_equipment_resource
	[TI.TagGroup((int)TagGroups.Enumerated.sreqip, 1, 76)]
	public class scenario_equipment_resource_group : scenario_objects_resource_group
	{
		#region Fields
		public TI.Block<scenario_equipment_block> Objects;
		#endregion

		#region Ctor
		public scenario_equipment_resource_group() : base(7)
		{
			Add(Names = new TI.Block<scenario_object_names_block>(this, 640));
			Add(/* = */ new TI.Block<dont_use_me_scenario_environment_object_block>(this, 4096));
			Add(/*Structure References = */ new TI.Block<scenario_structure_bsp_reference_block>(this, 16));
			Add(Palette = new TI.Block<scenario_object_palette_block>(this, 256));
			Add(Objects = new TI.Block<scenario_equipment_block>(this, 256));
			Add(/*Next Object ID Salt = */ new TI.LongInteger());
			Add(/*Editor Folders = */ new TI.Block<g_scenario_editor_folder_block>(this, 32767));

			_Objects = Objects;
			//Palette.Definition.Name.ResetReferenceGroupTag(TagGroups.eqip);
		}
		#endregion
	};
	#endregion

	#region scenario_hs_source_file
	[TI.TagGroup((int)TagGroups.Enumerated.srhscf, 1, 52)]
	public class scenario_hs_source_file_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_hs_source_file_group() : base(2)
		{
			Add(/*name = */ new TI.String());
			Add(/*source = */ new TI.Data(this));
		}
		#endregion
	};
	#endregion

	#region scenario_lights_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srligh, 1, 76)]
	public class scenario_lights_resource_group : scenario_objects_resource_group
	{
		#region Fields
		public TI.Block<scenario_light_block> Objects;
		#endregion

		#region Ctor
		public scenario_lights_resource_group() : base(7)
		{
			Add(Names = new TI.Block<scenario_object_names_block>(this, 640));
			Add(/* = */ new TI.Block<dont_use_me_scenario_environment_object_block>(this, 4096));
			Add(/*Structure References = */ new TI.Block<scenario_structure_bsp_reference_block>(this, 16));
			Add(Palette = new TI.Block<scenario_object_palette_block>(this, 256));
			Add(Objects = new TI.Block<scenario_light_block>(this, 500));
			Add(/*Next Object ID Salt = */ new TI.LongInteger());
			Add(/*Editor Folders = */ new TI.Block<g_scenario_editor_folder_block>(this, 32767));

			_Objects = Objects;
			//Palette.Definition.Name.ResetReferenceGroupTag(TagGroups.ligh);
		}
		#endregion
	};
	#endregion

	#region scenario_scenery_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srscen, 1, 104)]
	public class scenario_scenery_resource_group : TI.Definition
	{
		#region Fields
		public TI.Block<scenario_object_palette_block> SceneryPalette;
		public TI.Block<scenario_object_palette_block> CratePalette;
		#endregion

		#region Ctor
		public scenario_scenery_resource_group() : base(10)
		{
			Add(/*Names = */ new TI.Block<scenario_object_names_block>(this, 640));
			Add(/* = */ new TI.Block<dont_use_me_scenario_environment_object_block>(this, 4096));
			Add(/*Structure References = */ new TI.Block<scenario_structure_bsp_reference_block>(this, 16));
			Add(SceneryPalette = new TI.Block<scenario_object_palette_block>(this, 256));
			Add(/*Objects = */ new TI.Block<scenario_scenery_block>(this, 2000));
			Add(/*Next Scenery Object ID Salt = */ new TI.LongInteger());
			Add(CratePalette = new TI.Block<scenario_object_palette_block>(this, 256));
			Add(/*Objects = */ new TI.Block<scenario_crate_block>(this, 1024));
			Add(/*Next Block Object ID Salt = */ new TI.LongInteger());
			Add(/*Editor Folders = */ new TI.Block<g_scenario_editor_folder_block>(this, 32767));

			//SceneryPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.scen);
			//CratePalette.Definition.Name.ResetReferenceGroupTag(TagGroups.bloc);
		}
		#endregion
	};
	#endregion

	#region scenario_sound_scenery_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srssce, 1, 76)]
	public class scenario_sound_scenery_resource_group : scenario_objects_resource_group
	{
		#region Fields
		public TI.Block<scenario_sound_scenery_block> Objects;
		#endregion

		#region Ctor
		public scenario_sound_scenery_resource_group() : base(7)
		{
			Add(Names = new TI.Block<scenario_object_names_block>(this, 640));
			Add(/* = */ new TI.Block<dont_use_me_scenario_environment_object_block>(this, 4096));
			Add(/*Structure References = */ new TI.Block<scenario_structure_bsp_reference_block>(this, 16));
			Add(Palette = new TI.Block<scenario_object_palette_block>(this, 256));
			Add(Objects = new TI.Block<scenario_sound_scenery_block>(this, 256));
			Add(/*Next Object ID Salt = */ new TI.LongInteger());
			Add(/*Editor Folders = */ new TI.Block<g_scenario_editor_folder_block>(this, 32767));

			_Objects = Objects;
			//Palette.Definition.Name.ResetReferenceGroupTag(TagGroups.ssce);
		}
		#endregion
	};
	#endregion

	#region scenario_structure_lighting_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srsslt, 1, 12)]
	public class scenario_structure_lighting_resource_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_structure_lighting_resource_group() : base(1)
		{
			Add(/*Structure Lighting = */ new TI.Block<scenario_structure_bsp_spherical_harmonic_lighting_block>(this, 16));
		}
		#endregion
	};
	#endregion

	#region scenario_trigger_volumes_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srtrgr, 1, 24)]
	public class scenario_trigger_volumes_resource_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_trigger_volumes_resource_group() : base(2)
		{
			Add(/*Kill Trigger Volumes = */ new TI.Block<scenario_trigger_volume_block>(this, 256));
			Add(/*Object Names = */ new TI.Block<scenario_object_names_block>(this, 640));
		}
		#endregion
	};
	#endregion

	#region scenario_vehicles_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srvehi, 1, 76)]
	public class scenario_vehicles_resource_group : scenario_objects_resource_group
	{
		#region Fields
		public TI.Block<scenario_vehicle_block> Objects;
		#endregion

		#region Ctor
		public scenario_vehicles_resource_group() : base(7)
		{
			Add(Names = new TI.Block<scenario_object_names_block>(this, 640));
			Add(/* = */ new TI.Block<dont_use_me_scenario_environment_object_block>(this, 4096));
			Add(/*Structure References = */ new TI.Block<scenario_structure_bsp_reference_block>(this, 16));
			Add(Palette = new TI.Block<scenario_object_palette_block>(this, 256));
			Add(Objects = new TI.Block<scenario_vehicle_block>(this, 256));
			Add(/*Next Object ID Salt = */ new TI.LongInteger());
			Add(/*Editor Folders = */ new TI.Block<g_scenario_editor_folder_block>(this, 32767));

			_Objects = Objects;
			//Palette.Definition.Name.ResetReferenceGroupTag(TagGroups.vehi);
		}
		#endregion
	};
	#endregion

	#region scenario_weapons_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srweap, 1, 76)]
	public class scenario_weapons_resource_group : scenario_objects_resource_group
	{
		#region Fields
		public TI.Block<scenario_weapon_block> Objects;
		#endregion

		#region Ctor
		public scenario_weapons_resource_group() : base(7)
		{
			Add(Names = new TI.Block<scenario_object_names_block>(this, 640));
			Add(/* = */ new TI.Block<dont_use_me_scenario_environment_object_block>(this, 4096));
			Add(/*Structure References = */ new TI.Block<scenario_structure_bsp_reference_block>(this, 16));
			Add(Palette = new TI.Block<scenario_object_palette_block>(this, 256));
			Add(Objects = new TI.Block<scenario_weapon_block>(this, 128));
			Add(/*Next Object ID Salt = */ new TI.LongInteger());
			Add(/*Editor Folders = */ new TI.Block<g_scenario_editor_folder_block>(this, 32767));

			_Objects = Objects;
			//Palette.Definition.Name.ResetReferenceGroupTag(TagGroups.weap);
		}
		#endregion
	};
	#endregion
	#endregion
}