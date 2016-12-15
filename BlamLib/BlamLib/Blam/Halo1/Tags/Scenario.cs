/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo1.Tags
{
	#region scenario_structure_bsps_header
	[TI.Definition(1, scenario_structure_bsps_header.kSizeOf)]
	public class scenario_structure_bsps_header : TI.Definition
	{
		public const int kSizeOf = 24;
		#region Fields
		public TI.LongInteger PtrBsp;
		public TI.Tag Signature;
		#endregion

		public scenario_structure_bsps_header()
		{
			Add(PtrBsp = new TI.LongInteger());
			Add(new TI.Pad(16));
			Add(Signature = new TI.Tag());
		}
	};
	#endregion

	#region scenario_structure_bsps_block
	[TI.Definition(-1, scenario_structure_bsps_block.kSizeOf)]
	public class scenario_structure_bsps_block : TI.Definition
	{
		public const int kSizeOf = 32;
		#region Fields
		public TI.LongInteger Offset;
		public TI.LongInteger Size;
		public TI.Skip Data;
		public TI.TagReference StructureBsp;
		#endregion

		public scenario_structure_bsps_block() : base(4)
		{
			// Offset to this bsp's scenario_structure_bsps_header
			Add(Offset = new TI.LongInteger());
			Add(Size = new TI.LongInteger());
			Add(Data = new TI.Skip(4 // address
				+ 4)); // pad
			Add(StructureBsp = new TI.TagReference(this, TagGroups.sbsp));
		}
	};
	#endregion

	#region scenario resources
	// ALL scenario resource tag versions are '0' since halo 2's are all '1'

	#region scenario_objects_resource
	public abstract class scenario_objects_resource_group : TI.Definition
	{
		#region Fields
		public TI.Block<scenario_group.scenario_object_names_block> Names;
		public TI.Block<scenario_group.scenario_object_palette_block> Palette;
		public TI.IBlock _Objects;
		#endregion

		protected scenario_objects_resource_group(int field_count) : base(field_count) { }
	};
	#endregion

	#region scenario_scenery_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srscen, 0, 36)]
	public class scenario_scenery_resource_group : scenario_objects_resource_group
	{
		#region Fields
		public TI.Block<scenario_group.scenario_scenery_block> Objects;
		#endregion

		#region Ctor
		public scenario_scenery_resource_group() : base(3)
		{
			Add(Names = new TI.Block<scenario_group.scenario_object_names_block>(this, 512));
			Add(Objects = new TI.Block<scenario_group.scenario_scenery_block>(this, 2000));
			Add(Palette = new TI.Block<scenario_group.scenario_object_palette_block>(this, 100));

			_Objects = Objects;
			//Palette.Definition.Name.ResetReferenceGroupTag(TagGroups.scen);
		}
		#endregion
	};
	#endregion

	#region scenario_bipeds_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srbipd, 0, 36)]
	public class scenario_bipeds_resource_group : scenario_objects_resource_group
	{
		#region Fields
		public TI.Block<scenario_group.scenario_biped_block> Objects;
		#endregion

		#region Ctor
		public scenario_bipeds_resource_group() : base(3)
		{
			Add(Names = new TI.Block<scenario_group.scenario_object_names_block>(this, 512));
			Add(Objects = new TI.Block<scenario_group.scenario_biped_block>(this, 128));
			Add(Palette = new TI.Block<scenario_group.scenario_object_palette_block>(this, 100));

			_Objects = Objects;
			//Palette.Definition.Name.ResetReferenceGroupTag(TagGroups.bipd);
		}
		#endregion
	};
	#endregion

	#region scenario_vehicles_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srvehi, 0, 36)]
	public class scenario_vehicles_resource_group : scenario_objects_resource_group
	{
		#region Fields
		public TI.Block<scenario_group.scenario_vehicle_block> Objects;
		#endregion

		#region Ctor
		public scenario_vehicles_resource_group() : base(3)
		{
			Add(Names = new TI.Block<scenario_group.scenario_object_names_block>(this, 512));
			Add(Objects = new TI.Block<scenario_group.scenario_vehicle_block>(this, 80));
			Add(Palette = new TI.Block<scenario_group.scenario_object_palette_block>(this, 100));

			_Objects = Objects;
			//Palette.Definition.Name.ResetReferenceGroupTag(TagGroups.vehi);
		}
		#endregion
	};
	#endregion

	#region scenario_equipment_resource
	[TI.TagGroup((int)TagGroups.Enumerated.sreqip, 0, 36)]
	public class scenario_equipment_resource_group : scenario_objects_resource_group
	{
		#region Fields
		public TI.Block<scenario_group.scenario_equipment_block> Objects;
		#endregion

		#region Ctor
		public scenario_equipment_resource_group() : base(3)
		{
			Add(Names = new TI.Block<scenario_group.scenario_object_names_block>(this, 512));
			Add(Objects = new TI.Block<scenario_group.scenario_equipment_block>(this, 256));
			Add(Palette = new TI.Block<scenario_group.scenario_object_palette_block>(this, 100));

			_Objects = Objects;
			//Palette.Definition.Name.ResetReferenceGroupTag(TagGroups.eqip);
		}
		#endregion
	};
	#endregion

	#region scenario_weapons_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srweap, 0, 36)]
	public class scenario_weapons_resource_group : scenario_objects_resource_group
	{
		#region Fields
		public TI.Block<scenario_group.scenario_weapon_block> Objects;
		#endregion

		#region Ctor
		public scenario_weapons_resource_group() : base(3)
		{
			Add(Names = new TI.Block<scenario_group.scenario_object_names_block>(this, 512));
			Add(Objects = new TI.Block<scenario_group.scenario_weapon_block>(this, 128));
			Add(Palette = new TI.Block<scenario_group.scenario_object_palette_block>(this, 100));

			_Objects = Objects;
			//Palette.Definition.Name.ResetReferenceGroupTag(TagGroups.weap);
		}
		#endregion
	};
	#endregion

	#region scenario_sound_scenery_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srssce, 0, 36)]
	public class scenario_sound_scenery_resource_group : scenario_objects_resource_group
	{
		#region Fields
		public TI.Block<scenario_group.scenario_sound_scenery_block> Objects;
		#endregion

		#region Ctor
		public scenario_sound_scenery_resource_group() : base(3)
		{
			Add(Names = new TI.Block<scenario_group.scenario_object_names_block>(this, 512));
			Add(Objects = new TI.Block<scenario_group.scenario_sound_scenery_block>(this, 256));
			Add(Palette = new TI.Block<scenario_group.scenario_object_palette_block>(this, 100));

			_Objects = Objects;
			//Palette.Definition.Name.ResetReferenceGroupTag(TagGroups.ssce);
		}
		#endregion
	};
	#endregion

	#region scenario_devices_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srdgrp, 0, -1)]
	public class scenario_devices_resource_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_devices_resource_group()
		{
		}
		#endregion
	};
	#endregion

	#region scenario_decals_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srdeca, 0, 24)]
	public class scenario_decals_resource_group : TI.Definition
	{
		#region Fields
		public TI.Block<field_block<TI.TagReference>> Palette;
		public TI.Block<scenario_group.scenario_decals_block> Decals;
		#endregion

		#region Ctor
		public scenario_decals_resource_group() : base(2)
		{
			Add(Palette = new TI.Block<field_block<TI.TagReference>>(this, 128));
			Add(Decals = new TI.Block<scenario_group.scenario_decals_block>(this, 65536));

			//Palette.Definition.Value.ResetReferenceGroupTag(TagGroups.deca);
		}
		#endregion
	};
	#endregion

	#region scenario_cinematics_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srcine, 0, -1)]
	public class scenario_cinematics_resource_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_cinematics_resource_group()
		{
		}
		#endregion
	};
	#endregion

	#region scenario_trigger_volumes_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srtrgr, 0, -1)]
	public class scenario_trigger_volumes_resource_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_trigger_volumes_resource_group()
		{
		}
		#endregion
	};
	#endregion

	#region scenario_cluster_data_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srclut, 0, -1)]
	public class scenario_cluster_data_resource_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_cluster_data_resource_group()
		{
		}
		#endregion
	};
	#endregion

	#region scenario_hs_source_file
	[TI.TagGroup((int)TagGroups.Enumerated.srhscf, 0, -1)]
	public class scenario_hs_source_file_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_hs_source_file_group()
		{
		}
		#endregion
	};
	#endregion

	#region scenario_ai_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srai, 0, -1)]
	public class scenario_ai_resource_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_ai_resource_group()
		{
		}
		#endregion
	};
	#endregion

	#region scenario_comments_resource
	[TI.TagGroup((int)TagGroups.Enumerated.srcmmt, 0, 12)]
	public class scenario_comments_resource_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public scenario_comments_resource_group() : base(1)
		{
			Add(/*Comments = */ new TI.Block<scenario_group.editor_comment_block>(this, 1024));
		}
		#endregion
	};
	#endregion
	#endregion

	[TI.TagGroup((int)TagGroups.Enumerated.scnr, 2, 1456)]
	public class scenario_group : TI.Definition
	{
		// scenario_sky_reference_block, field_block<TI.TagReference>

		// scenario_child_scenario_block, field_block<TI.TagReference>

		#region scenario_child_scenario_block
		[TI.Definition(-1, 32)]
		public class scenario_child_scenario_block : TI.Definition
		{
			#region Fields
			public TI.TagReference Child;
			#endregion

			public scenario_child_scenario_block() : base(2)
			{
				Add(Child = new TI.TagReference(this, TagGroups.scnr));
				Add(TI.Pad.ReferenceHalo1);
			}
		};
		#endregion

		#region scenario_function_block
		[TI.Definition(-1, 120)]
		public class scenario_function_block : TI.Definition
		{
			#region Fields
			public TI.Flags Flags;
			public TI.String Name;
			public TI.Real Period;
			public TI.BlockIndex ScalePeriodBy;
			public TI.Enum Function;
			public TI.BlockIndex ScaleFunctionBy;
			public TI.Enum WobbleFunction;
			public TI.Real WobblePeriod;
			public TI.Real WobbleMagnitude;
			public TI.Real SquareWaveThreshold;
			public TI.ShortInteger StepCount;
			public TI.Enum MapTo;
			public TI.ShortInteger SawtoothCount;
			public TI.BlockIndex ScaleResultBy;
			public TI.Enum BoundsMode;
			public TI.Real Bounds;
			public TI.BlockIndex TurnoffWith;
			#endregion

			public scenario_function_block() : base(20)
			{
				Add(Flags = new TI.Flags());
				Add(Name = new TI.String());
				Add(Period = new TI.Real());
				Add(ScalePeriodBy = new TI.BlockIndex());
				Add(Function = new TI.Enum());
				Add(ScalePeriodBy = new TI.BlockIndex());
				Add(WobbleFunction = new TI.Enum());
				Add(WobblePeriod = new TI.Real());
				Add(WobbleMagnitude = new TI.Real());
				Add(SquareWaveThreshold = new TI.Real());
				Add(StepCount = new TI.ShortInteger());
				Add(MapTo = new TI.Enum());
				Add(SawtoothCount = new TI.ShortInteger());
				Add(TI.Pad.Word);
				Add(ScaleResultBy = new TI.BlockIndex());
				Add(BoundsMode = new TI.Enum());
				Add(Bounds = new TI.Real(BlamLib.TagInterface.FieldType.RealFractionBounds));
				Add(new TI.Pad(4 + 2));
				Add(TurnoffWith = new TI.BlockIndex());
				Add(new TI.Pad(16*2));
			}
		};
		#endregion

		#region editor_comment_block
		[TI.Definition(-1, 48)]
		public class editor_comment_block : TI.Definition
		{
			#region Fields
			public TI.RealPoint3D Position;
			public TI.Data Comment;
			#endregion

			public editor_comment_block() : base(3)
			{
				Add(Position = new TI.RealPoint3D());
				Add(TI.Pad.ReferenceHalo1);
				Add(Comment = new TI.Data(this));
			}
		};
		#endregion

		#region scenario_object_names_block
		[TI.Definition(-1, 36)]
		public class scenario_object_names_block : TI.Definition
		{
			#region Fields
			public TI.String Name;
			public TI.Skip Type;
			public TI.Skip Index;
			#endregion

			public scenario_object_names_block() : base(3)
			{
				Add(Name = new TI.String());
				Add(Type = new TI.Skip(2));
				Add(Index = new TI.Skip(2));
			}
		};
		#endregion

		#region scenario_object_block
		public abstract class scenario_object_block : TI.Definition
		{
			internal const int k_scenario_object_size = 32;

			#region Fields
			public TI.BlockIndex Type;
			public TI.BlockIndex Name;
			public TI.Flags NotPlaced;
			public TI.ShortInteger DesiredPermutation;
			public TI.RealPoint3D Position;
			public TI.RealEulerAngles3D Rotation;
			#endregion

			protected scenario_object_block(int field_count) : base(field_count + 6)
			{
				Add(Type = new TI.BlockIndex());
				Add(Name = new TI.BlockIndex());
				Add(NotPlaced = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
				Add(DesiredPermutation = new TI.ShortInteger());
				Add(Position = new TI.RealPoint3D());
				Add(Rotation = new TI.RealEulerAngles3D());
			}
		};
		#endregion

		#region scenario_object_palette_block
		[TI.Definition(-1, 48)]
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

		#region scenario_scenery_block
		[TI.Definition(-1, scenario_object_block.k_scenario_object_size + 40)]
		public class scenario_scenery_block : scenario_object_block
		{
			#region Fields
			#endregion

			public scenario_scenery_block() : base(1)
			{
				Add(new TI.Pad(8 + 16 + 8 + 8));
			}
		};
		#endregion

		#region scenario_unit_block
		public abstract class scenario_unit_block : scenario_object_block
		{
			internal const int k_scenario_unit_size = 56;

			#region Fields
			public TI.Real BodyVitality;
			public TI.Flags Flags;
			#endregion

			protected scenario_unit_block(int field_count) : base(field_count + 4)
			{
				Add(new TI.Pad(8 + 16 + 8 + 8));
				Add(BodyVitality = new TI.Real());
				Add(Flags = new TI.Flags());
				Add(new TI.Pad(8));
			}
		};
		#endregion

		#region scenario_biped_block
		[TI.Definition(-1, scenario_object_block.k_scenario_object_size + scenario_unit_block.k_scenario_unit_size + 32)]
		public class scenario_biped_block : scenario_unit_block
		{
			#region Fields
			#endregion

			public scenario_biped_block() : base(1)
			{
				Add(new TI.Pad(32));
			}
		};
		#endregion

		#region scenario_vehicle_block
		[TI.Definition(-1, scenario_object_block.k_scenario_object_size + scenario_unit_block.k_scenario_unit_size + 32)]
		public class scenario_vehicle_block : scenario_unit_block
		{
			#region Fields
			public TI.ByteInteger MultiplayerTeamIndex; // ByteInteger, and is for halopc only
			public TI.Flags MultiplayerSpawnFlags; // halopc only
			#endregion

			public scenario_vehicle_block() : base(4)
			{
				Add(MultiplayerTeamIndex = new TI.ByteInteger());
				Add(new TI.Pad(1));
				Add(MultiplayerSpawnFlags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
				Add(new TI.Pad(28));
			}
		};
		#endregion

		#region scenario_equipment_block
		[TI.Definition(-1, scenario_object_block.k_scenario_object_size + 8)]
		public class scenario_equipment_block : scenario_object_block
		{
			#region Fields
			public TI.Flags MiscFlags;
			#endregion

			public scenario_equipment_block() : base(3)
			{
				Add(new TI.Pad(2));
				Add(MiscFlags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
				Add(new TI.Pad(4));
			}
		};
		#endregion

		#region scenario_weapon_block
		[TI.Definition(-1, scenario_object_block.k_scenario_object_size + 60)]
		public class scenario_weapon_block : scenario_object_block
		{
			#region Fields
			public TI.ShortInteger RoundsLeft;
			public TI.ShortInteger RoundsLoaded;
			public TI.Flags Flags;
			#endregion

			public scenario_weapon_block() : base(5)
			{
				Add(new TI.Pad(8 + 16 + 8 + 8));
				Add(RoundsLeft = new TI.ShortInteger());
				Add(RoundsLoaded = new TI.ShortInteger());
				Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
				Add(new TI.Pad(2 + 12));
			}
		};
		#endregion

		#region device_group_block
		[TI.Definition(-1, 52)]
		public class device_group_block : TI.Definition
		{
			#region Fields
			public TI.String Name;
			public TI.Real InitialValue;
			public TI.Flags Flags;
			#endregion

			public device_group_block() : base(4)
			{
				Add(Name = new TI.String());
				Add(InitialValue = new TI.Real());
				Add(Flags = new TI.Flags());
				Add(new TI.Pad(12));
			}
		};
		#endregion

		#region scenario_device_block
		public abstract class scenario_device_block : scenario_object_block
		{
			internal const int k_scenario_device_size = 16;

			#region Fields
			public TI.BlockIndex PowerGroup;
			public TI.BlockIndex PositionGroup;
			public TI.Flags DeviceFlags;
			#endregion

			protected scenario_device_block(int field_count) : base(field_count + 4)
			{
				Add(new TI.Pad(8));
				Add(PowerGroup = new TI.BlockIndex());
				Add(PositionGroup = new TI.BlockIndex());
				Add(DeviceFlags = new TI.Flags());
			}
		};
		#endregion

		#region scenario_machine_block
		[TI.Definition(-1, scenario_object_block.k_scenario_object_size + scenario_device_block.k_scenario_device_size + 16)]
		public class scenario_machine_block : scenario_device_block
		{
			#region Fields
			public TI.Flags MachineFlags;
			#endregion

			public scenario_machine_block() : base(2)
			{
				Add(MachineFlags = new TI.Flags());
				Add(new TI.Pad(12));
			}
		};
		#endregion

		#region scenario_control_block
		[TI.Definition(-1, scenario_object_block.k_scenario_object_size + scenario_device_block.k_scenario_device_size + 16)]
		public class scenario_control_block : scenario_device_block
		{
			#region Fields
			public TI.Flags ControlFlags;
			public TI.ShortInteger CustomNameIndex; // can't touch this, dodododo, dah do dah do....
			#endregion

			public scenario_control_block() : base(3)
			{
				Add(ControlFlags = new TI.Flags());
				Add(CustomNameIndex = new TI.ShortInteger());
				Add(new TI.Pad(2 + 8));
			}
		};
		#endregion

		#region scenario_light_fixture_block
		[TI.Definition(-1, scenario_object_block.k_scenario_object_size + scenario_device_block.k_scenario_device_size + 40)]
		public class scenario_light_fixture_block : scenario_device_block
		{
			#region Fields
			public TI.RealColor Color;
			public TI.Real Intensity;
			public TI.Real FallOffAngle;
			public TI.Real CutoffAngle;
			#endregion

			public scenario_light_fixture_block() : base(5)
			{
				Add(Color = new TI.RealColor());
				Add(Intensity = new TI.Real());
				Add(FallOffAngle = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
				Add(CutoffAngle = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
				Add(new TI.Pad(16));
			}
		};
		#endregion

		#region scenario_sound_scenery_block
		[TI.Definition(-1, scenario_object_block.k_scenario_object_size + 8)]
		public class scenario_sound_scenery_block : scenario_object_block
		{
			#region Fields
			#endregion

			public scenario_sound_scenery_block() : base(1)
			{
				Add(new TI.Pad(8));
			}
		};
		#endregion

		#region scenario_profiles_block
		[TI.Definition(-1, 104)]
		public class scenario_profiles_block : TI.Definition
		{
			#region Fields
			public TI.String Name;
			public TI.Real StartingHealthMod;
			public TI.Real StartingShieldMod;
			public TI.TagReference PrimaryWeapon;
			public TI.ShortInteger PrimaryRoundsLoaded;
			public TI.ShortInteger PrimaryRoundsTotal;
			public TI.TagReference SecondaryWeapon;
			public TI.ShortInteger SecondaryRoundsLoaded;
			public TI.ShortInteger SecondaryRoundsTotal;
			public TI.ByteInteger StartingFrags;
			public TI.ByteInteger StartingPlasmas;
			public TI.ByteInteger StartingUnknownFrags;
			public TI.ByteInteger StartingUnknownPlasmas;
			#endregion

			public scenario_profiles_block() : base(14)
			{
				Add(Name = new TI.String());
				Add(StartingHealthMod = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
				Add(StartingShieldMod = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
				Add(PrimaryWeapon = new TI.TagReference(this, TagGroups.weap));
				Add(PrimaryRoundsLoaded = new TI.ShortInteger());
				Add(PrimaryRoundsTotal = new TI.ShortInteger());
				Add(SecondaryWeapon = new TI.TagReference(this, TagGroups.weap));
				Add(SecondaryRoundsLoaded = new TI.ShortInteger());
				Add(SecondaryRoundsTotal = new TI.ShortInteger());
				Add(StartingFrags = new TI.ByteInteger());
				Add(StartingPlasmas = new TI.ByteInteger());
				Add(StartingUnknownFrags = new TI.ByteInteger());
				Add(StartingUnknownPlasmas = new TI.ByteInteger());
				Add(new TI.Pad(20));
			}
		};
		#endregion

		#region scenario_players_block
		[TI.Definition(-1, 52)]
		public class scenario_players_block : TI.Definition
		{
			#region Fields
			public TI.RealPoint3D Position;
			public TI.Real Facing;
			public TI.ShortInteger TeamIndex;
			public TI.ShortInteger BspIndex;
			public TI.Enum Type0;
			public TI.Enum Type1;
			public TI.Enum Type2;
			public TI.Enum Type3;
			#endregion

			public scenario_players_block() : base(9)
			{
				Add(Position = new TI.RealPoint3D());
				Add(Facing = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
				Add(TeamIndex = new TI.ShortInteger());
				Add(BspIndex = new TI.ShortInteger());
				Add(Type0 = new TI.Enum());
				Add(Type1 = new TI.Enum());
				Add(Type2 = new TI.Enum());
				Add(Type3 = new TI.Enum());
				Add(new TI.Pad(24));
			}
		};
		#endregion

		#region scenario_trigger_volume_block
		[TI.Definition(-1, 96)]
		public class scenario_trigger_volume_block : TI.Definition
		{
			#region Fields
			public TI.Skip First;
			public TI.String Name;
			public TI.Real Field1;
			public TI.Real Field2;
			public TI.Real Field3;
			public TI.Real Field4;
			public TI.Real Field5;
			public TI.Real Field6;
			public TI.Real Field7;
			public TI.Real Field8;
			public TI.Real Field9;
			public TI.Real Field10;
			public TI.Real Field11;
			public TI.Real Field12;
			public TI.Real Field13;
			public TI.Real Field14;
			public TI.Real Field15;
			#endregion

			public scenario_trigger_volume_block() : base(17)
			{
				Add(First = new TI.Skip(4));
				Add(Name = new TI.String());
				Add(Field1 = new TI.Real());
				Add(Field2 = new TI.Real());
				Add(Field3 = new TI.Real());
				Add(Field4 = new TI.Real());
				Add(Field5 = new TI.Real());
				Add(Field6 = new TI.Real());
				Add(Field7 = new TI.Real());
				Add(Field8 = new TI.Real());
				Add(Field9 = new TI.Real());
				Add(Field10 = new TI.Real());
				Add(Field11 = new TI.Real());
				Add(Field12 = new TI.Real());
				Add(Field13 = new TI.Real());
				Add(Field14 = new TI.Real());
				Add(Field15 = new TI.Real());
			}
		};
		#endregion

		#region recorded_animation_block
		[TI.Definition(-1, 64)]
		public class recorded_animation_block : TI.Definition
		{
			#region Fields
			public TI.String Name;
			public TI.ByteInteger Version;
			public TI.ByteInteger RawAnimationData;
			public TI.ByteInteger UnitControlDataVersion;
			public TI.ShortInteger LengthOfAnimation;
			public TI.Data EventStream;
			#endregion

			public recorded_animation_block() : base(8)
			{
				Add(Name = new TI.String());
				Add(Version = new TI.ByteInteger());
				Add(RawAnimationData = new TI.ByteInteger());
				Add(UnitControlDataVersion = new TI.ByteInteger());
				Add(new TI.Pad(1));
				Add(LengthOfAnimation = new TI.ShortInteger());
				Add(new TI.Pad(2 + 4));
				Add(EventStream = new TI.Data(this));
			}
		};
		#endregion

		#region scenario_netgame_flags_block
		[TI.Definition(-1, 148)]
		public class scenario_netgame_flags_block : TI.Definition
		{
			#region Fields
			public TI.RealPoint3D Position;
			public TI.Real Facing;
			public TI.Enum Type;
			public TI.ShortInteger TeamIndex;
			public TI.TagReference WeaponGroup;
			#endregion

			public scenario_netgame_flags_block() : base(6)
			{
				Add(Position = new TI.RealPoint3D());
				Add(Facing = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
				Add(Type = new TI.Enum());
				Add(TeamIndex = new TI.ShortInteger());
				Add(WeaponGroup = new TI.TagReference(this, TagGroups.itmc));
				Add(new TI.Pad(112));
			}
		};
		#endregion

		#region scenario_netgame_equipment_block
		[TI.Definition(-1, 144)]
		public class scenario_netgame_equipment_block : TI.Definition
		{
			#region Fields
			public TI.Flags Flags;
			public TI.Enum Type0;
			public TI.Enum Type1;
			public TI.Enum Type2;
			public TI.Enum Type3;
			public TI.ShortInteger TeamIndex;
			public TI.ShortInteger SpawnTime;
			public TI.RealPoint3D Position;
			public TI.Real Facing;
			public TI.TagReference ItemCollection;
			#endregion

			public scenario_netgame_equipment_block() : base(12)
			{
				Add(Flags = new TI.Flags());
				Add(Type0 = new TI.Enum());
				Add(Type1 = new TI.Enum());
				Add(Type2 = new TI.Enum());
				Add(Type3 = new TI.Enum());
				Add(TeamIndex = new TI.ShortInteger());
				Add(SpawnTime = new TI.ShortInteger());
				Add(new TI.Pad(48));
				Add(Position = new TI.RealPoint3D());
				Add(Facing = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
				Add(ItemCollection = new TI.TagReference(this, TagGroups.itmc));
				Add(new TI.Pad(48));
			}
		};
		#endregion

		#region scenario_starting_equipment_block
		[TI.Definition(-1, 204)]
		public class scenario_starting_equipment_block : TI.Definition
		{
			#region Fields
			public TI.Flags Flags;
			public TI.Enum Type0;
			public TI.Enum Type1;
			public TI.Enum Type2;
			public TI.Enum Type3;
			public TI.TagReference ItemCollection1;
			public TI.TagReference ItemCollection2;
			public TI.TagReference ItemCollection3;
			public TI.TagReference ItemCollection4;
			public TI.TagReference ItemCollection5;
			public TI.TagReference ItemCollection6;
			#endregion

			public scenario_starting_equipment_block() : base(13)
			{
				Add(Flags = new TI.Flags());
				Add(Type0 = new TI.Enum());
				Add(Type1 = new TI.Enum());
				Add(Type2 = new TI.Enum());
				Add(Type3 = new TI.Enum());
				Add(new TI.Pad(48));
				Add(ItemCollection1 = new TI.TagReference(this, TagGroups.itmc));
				Add(ItemCollection2 = new TI.TagReference(this, TagGroups.itmc));
				Add(ItemCollection3 = new TI.TagReference(this, TagGroups.itmc));
				Add(ItemCollection4 = new TI.TagReference(this, TagGroups.itmc));
				Add(ItemCollection5 = new TI.TagReference(this, TagGroups.itmc));
				Add(ItemCollection6 = new TI.TagReference(this, TagGroups.itmc));
				Add(new TI.Pad(48));
			}
		};
		#endregion

		#region scenario_bsp_switch_trigger_volume_block
		[TI.Definition(-1, 8)]
		public class scenario_bsp_switch_trigger_volume_block : TI.Definition
		{
			#region Fields
			public TI.BlockIndex TriggerVolume;
			public TI.ShortInteger Source;
			public TI.ShortInteger Destination;
			#endregion

			public scenario_bsp_switch_trigger_volume_block() : base(4)
			{
				Add(TriggerVolume = new TI.BlockIndex());
				Add(Source = new TI.ShortInteger());
				Add(Destination = new TI.ShortInteger());
				Add(new TI.Pad(2));
			}
		};
		#endregion

		#region scenario_decals_block
		[TI.Definition(-1, 16)]
		public class scenario_decals_block : TI.Definition
		{
			#region Fields
			public TI.BlockIndex DecalType;
			public TI.ByteInteger Yaw, Pitch;
			public TI.RealPoint3D Position;
			#endregion

			public scenario_decals_block() : base(4)
			{
				Add(DecalType = new TI.BlockIndex());
				Add(Yaw = new TI.ByteInteger());
				Add(Pitch = new TI.ByteInteger());
				Add(Position = new TI.RealPoint3D());
			}
		};
		#endregion

		// scenario_decal_palette_block, field_block<TI.TagReference>

		#region scenario_detail_object_collection_palette_block
		[TI.Definition(-1, 48)]
		public class scenario_detail_object_collection_palette_block : TI.Definition
		{
			#region Fields
			public TI.TagReference Name;
			#endregion

			public scenario_detail_object_collection_palette_block() : base(2)
			{
				Add(Name = new TI.TagReference());
				Add(new TI.Pad(32));
			}
		}
		#endregion

		// actor_palette_block, field_block<TI.TagReference>

		#region encounter_block
		[TI.Definition(-1, 176)]
		public class encounter_block : TI.Definition
		{
			#region squads_block
			[TI.Definition(-1, 232)]
			public class squads_block : TI.Definition
			{
				#region move_positions_block
				[TI.Definition(-1, 80)]
				public class move_positions_block : TI.Definition
				{
					#region Fields
					public TI.RealPoint3D Position;
					public TI.Real Facing;
					public TI.Real Weight;
					public TI.RealBounds Time;
					public TI.BlockIndex Animation;
					public TI.ByteInteger SequenceID;
					public TI.LongInteger SurfaceIndex;
					#endregion

					public move_positions_block() : base(8)
					{
						Add(Position = new TI.RealPoint3D());
						Add(Facing = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
						Add(Weight = new TI.Real());
						Add(Time = new TI.RealBounds());
						Add(Animation = new TI.BlockIndex());
						Add(SequenceID = new TI.ByteInteger());
						Add(new TI.Pad(1 + 44));
						Add(SurfaceIndex = new TI.LongInteger());
					}
				};
				#endregion

				#region actor_starting_locations_block
				[TI.Definition(-1, 28)]
				public class actor_starting_locations_block : TI.Definition
				{
					#region Fields
					public TI.RealPoint3D Position;
					public TI.Real Facing;
					public TI.ByteInteger SequenceID;
					public TI.Flags Flags;
					public TI.Enum ReturnState;
					public TI.Enum InitialState;
					public TI.BlockIndex ActorType;
					public TI.BlockIndex CommandList;
					#endregion

					public actor_starting_locations_block() : base(9)
					{
						Add(Position = new TI.RealPoint3D());
						Add(Facing = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
						Add(new TI.Pad(2));
						Add(SequenceID = new TI.ByteInteger());
						Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.ByteFlags));
						Add(ReturnState = new TI.Enum());
						Add(InitialState = new TI.Enum());
						Add(ActorType = new TI.BlockIndex());
						Add(CommandList = new TI.BlockIndex());
					}
				};
				#endregion

				#region Fields
				public TI.String Name;
				public TI.BlockIndex ActorType;
				public TI.BlockIndex Platoon;
				public TI.Enum InitialState;
				public TI.Enum ReturnState;
				public TI.Flags Flags;
				public TI.Enum UniqueLeaderType;
				public TI.BlockIndex ManeuverToSquad;
				public TI.Real SquadDelayTimer;
				public TI.Flags Attacking;
				public TI.Flags AttackingSearch;
				public TI.Flags AttackingGuard;
				public TI.Flags Defending;
				public TI.Flags DefendingSearch;
				public TI.Flags DefendingGuard;
				public TI.Flags Pursuing;
				public TI.ShortInteger NormalDiffCount;
				public TI.ShortInteger InsaneDiffCount;
				public TI.Enum MajorUpgrade;
				public TI.ShortInteger RespawnMinActors, RespawnMaxActors, RespawnTotal;
				public TI.RealBounds RespawnDelay;
				public TI.Block<move_positions_block> MovePositions;
				public TI.Block<actor_starting_locations_block> StartingLocations;
				#endregion

				public squads_block() : base(31)
				{
					Add(Name = new TI.String());
					Add(ActorType = new TI.BlockIndex());
					Add(Platoon = new TI.BlockIndex());
					Add(InitialState = new TI.Enum());
					Add(ReturnState = new TI.Enum());
					Add(Flags = new TI.Flags());
					Add(UniqueLeaderType = new TI.Enum());
					Add(new TI.Pad(2 + 28 + 2));
					Add(ManeuverToSquad = new TI.BlockIndex());
					Add(SquadDelayTimer = new TI.Real());
					Add(Attacking = new TI.Flags());
					Add(AttackingSearch = new TI.Flags());
					Add(AttackingGuard = new TI.Flags());
					Add(Defending = new TI.Flags());
					Add(DefendingSearch = new TI.Flags());
					Add(DefendingGuard = new TI.Flags());
					Add(Pursuing = new TI.Flags());
					Add(new TI.Pad(4 + 8));
					Add(NormalDiffCount = new TI.ShortInteger());
					Add(InsaneDiffCount = new TI.ShortInteger());
					Add(MajorUpgrade = new TI.Enum());
					Add(new TI.Pad(2));
					Add(RespawnMinActors = new TI.ShortInteger());
					Add(RespawnMaxActors = new TI.ShortInteger());
					Add(RespawnTotal = new TI.ShortInteger());
					Add(new TI.Pad(2));
					Add(RespawnDelay = new TI.RealBounds());
					Add(new TI.Pad(48));
					Add(MovePositions = new TI.Block<move_positions_block>(this, 32));
					Add(StartingLocations = new TI.Block<actor_starting_locations_block>(this, 32));
					Add(new TI.Pad(12));
				}
			};
			#endregion

			#region platoons_block
			[TI.Definition(-1, 172)]
			public class platoons_block : TI.Definition
			{
				#region Fields
				public TI.String Name;
				public TI.Flags Flags;
				public TI.Enum ChangeStateWhen;
				public TI.BlockIndex HappensTo1;
				public TI.Enum ManeuverWhen;
				public TI.BlockIndex HappensTo2;
				#endregion

				public platoons_block() : base(9)
				{
					Add(Name = new TI.String());
					Add(Flags = new TI.Flags());
					Add(new TI.Pad(12));
					Add(ChangeStateWhen = new TI.Enum());
					Add(HappensTo1 = new TI.BlockIndex());
					Add(new TI.Pad(4 + 4));
					Add(ManeuverWhen = new TI.Enum());
					Add(HappensTo2 = new TI.BlockIndex());
					Add(new TI.Pad(4 + 4 + 64 + 36));
				}
			};
			#endregion

			#region firing_positions_block
			[TI.Definition(-1, 24)]
			public class firing_positions_block : TI.Definition
			{
				#region Fields
				public TI.RealPoint3D Position;
				public TI.Enum GroupIndex;
				#endregion

				public firing_positions_block() : base(3)
				{
					Add(Position = new TI.RealPoint3D());
					Add(GroupIndex = new TI.Enum());
					Add(new TI.Pad(10));
				}
			};
			#endregion

			#region Fields
			public TI.String Name;
			public TI.Flags Flags;
			public TI.Enum TeamIndex;
			public TI.Enum SearchBehavoir;
			public TI.ShortInteger ManualBspIndex;
			public TI.RealBounds RespawnDelay;
			public TI.Block<squads_block> Squads;
			public TI.Block<platoons_block> Platoons;
			public TI.Block<firing_positions_block> FiringPositions;
			public TI.Block<scenario_players_block> PlayerStartingLocations;
			#endregion

			public encounter_block() : base(12)
			{
				Add(Name = new TI.String());
				Add(Flags = new TI.Flags());
				Add(TeamIndex = new TI.Enum());
				Add(new TI.Skip(2));
				Add(SearchBehavoir = new TI.Enum());
				Add(ManualBspIndex = new TI.ShortInteger());
				Add(RespawnDelay = new TI.RealBounds());
				Add(new TI.Pad(76));
				Add(Squads = new TI.Block<squads_block>(this, 64));
				Add(Platoons = new TI.Block<platoons_block>(this, 32));
				Add(FiringPositions = new TI.Block<firing_positions_block>(this, 512));
				Add(PlayerStartingLocations = new TI.Block<scenario_players_block>(this, 256));
			}
		};
		#endregion

		#region ai_command_list_block
		[TI.Definition(-1, 96)]
		public class ai_command_list_block : TI.Definition
		{
			#region ai_command_block
			[TI.Definition(-1, 32)]
			public class ai_command_block : TI.Definition
			{
				#region Fields
				public TI.Enum AtomType;
				public TI.ShortInteger AtomModifier;
				public TI.Real Parameter1;
				public TI.Real Parameter2;
				public TI.BlockIndex Point1;
				public TI.BlockIndex Point2;
				public TI.BlockIndex Animation;
				public TI.BlockIndex Script;
				public TI.BlockIndex Recording;
				public TI.BlockIndex Command;
				public TI.BlockIndex ObjectName;
				#endregion

				public ai_command_block() : base(12)
				{
					Add(AtomType = new TI.Enum());
					Add(AtomModifier = new TI.ShortInteger());
					Add(Parameter1 = new TI.Real());
					Add(Parameter2 = new TI.Real());
					Add(Point1 = new TI.BlockIndex());
					Add(Point2 = new TI.BlockIndex());
					Add(Animation = new TI.BlockIndex());
					Add(Script = new TI.BlockIndex());
					Add(Recording = new TI.BlockIndex());
					Add(Command = new TI.BlockIndex());
					Add(ObjectName = new TI.BlockIndex());
					Add(new TI.Pad(6));
				}
			};
			#endregion

			#region ai_command_point_block
			[TI.Definition(-1, 20)]
			public class ai_command_point_block : TI.Definition
			{
				#region Fields
				public TI.RealPoint3D Position;
				#endregion

				public ai_command_point_block() : base(2)
				{
					Add(Position = new TI.RealPoint3D());
					Add(new TI.Pad(8));
				}
			};
			#endregion

			#region Fields
			public TI.String Name;
			public TI.Flags Flags;
			public TI.ShortInteger ManualBspIndex;
			public TI.Block<ai_command_block> Commands;
			public TI.Block<ai_command_point_block> Points;
			#endregion

			public ai_command_list_block() : base(8)
			{
				Add(Name = new TI.String());
				Add(Flags = new TI.Flags());
				Add(new TI.Pad(8));
				Add(ManualBspIndex = new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(Commands = new TI.Block<ai_command_block>(this, 64));
				Add(Points = new TI.Block<ai_command_point_block>(this, 64));
				Add(new TI.Pad(24));
			}
		};
		#endregion

		#region ai_animation_reference_block
		[TI.Definition(-1, 60)]
		public class ai_animation_reference_block : TI.Definition
		{
			#region Fields
			public TI.String Name;
			public TI.TagReference Animation;
			#endregion

			public ai_animation_reference_block() : base(3)
			{
				Add(Name = new TI.String());
				Add(Animation = new TI.TagReference(this, TagGroups.antr));
				Add(new TI.Pad(12));
			}
		};
		#endregion

		#region ai_script_reference_block
		[TI.Definition(-1, 40)]
		public class ai_script_reference_block : TI.Definition
		{
			#region Fields
			public TI.String Name;
			#endregion

			public ai_script_reference_block() : base(2)
			{
				Add(Name = new TI.String());
				Add(new TI.Pad(8));
			}
		};
		#endregion

		#region ai_recording_reference_block
		[TI.Definition(-1, 40)]
		public class ai_recording_reference_block : TI.Definition
		{
			#region Fields
			public TI.String Name;
			#endregion

			public ai_recording_reference_block() : base(2)
			{
				Add(Name = new TI.String());
				Add(new TI.Pad(8));
			}
		};
		#endregion

		#region ai_conversation_block
		[TI.Definition(-1, 116)]
		public class ai_conversation_block : TI.Definition
		{
			#region ai_conversation_participant_block
			[TI.Definition(-1, 84)]
			public class ai_conversation_participant_block : TI.Definition
			{
				#region Fields
				public TI.Flags Flags;
				public TI.Enum SelectionType;
				public TI.Enum ActorType;
				public TI.BlockIndex UseThisObject;
				public TI.BlockIndex SetNavName;
				public TI.String EncounterName;
				#endregion

				public ai_conversation_participant_block() : base(9)
				{
					Add(new TI.Pad(2));
					Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
					Add(SelectionType = new TI.Enum());
					Add(ActorType = new TI.Enum());
					Add(UseThisObject = new TI.BlockIndex());
					Add(SetNavName = new TI.BlockIndex());
					Add(new TI.Pad(12 + 12));
					Add(EncounterName = new TI.String());
					Add(new TI.Pad(4 + 12));
				}
			};
			#endregion

			#region ai_conversation_line_block
			[TI.Definition(-1, 124)]
			public class ai_conversation_line_block : TI.Definition
			{
				#region Fields
				public TI.Flags Flags;
				public TI.BlockIndex Participant;
				public TI.Enum Addressee;
				public TI.BlockIndex AddresseeParticipant;
				public TI.Real LineDelayTime;
				public TI.TagReference Variant1;
				public TI.TagReference Variant2;
				public TI.TagReference Variant3;
				public TI.TagReference Variant4;
				public TI.TagReference Variant5;
				public TI.TagReference Variant6;
				#endregion

				public ai_conversation_line_block() : base(13)
				{
					Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
					Add(Participant = new TI.BlockIndex());
					Add(Addressee = new TI.Enum());
					Add(AddresseeParticipant = new TI.BlockIndex());
					Add(TI.Pad.DWord);
					Add(LineDelayTime = new TI.Real());
					Add(new TI.Pad(12));
					Add(Variant1 = new TI.TagReference(this, TagGroups.snd_));
					Add(Variant2 = new TI.TagReference(this, TagGroups.snd_));
					Add(Variant3 = new TI.TagReference(this, TagGroups.snd_));
					Add(Variant4 = new TI.TagReference(this, TagGroups.snd_));
					Add(Variant5 = new TI.TagReference(this, TagGroups.snd_));
					Add(Variant6 = new TI.TagReference(this, TagGroups.snd_));
				}
			};
			#endregion

			#region Fields
			public TI.String Name;
			public TI.Flags Flags;
			public TI.Real TriggerDistance;
			public TI.Real RunToPlayerDist;
			public TI.Block<ai_conversation_participant_block> Participants;
			public TI.Block<ai_conversation_line_block> Lines;
			#endregion

			public ai_conversation_block() : base(9)
			{
				Add(Name = new TI.String());
				Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
				Add(new TI.Pad(2));
				Add(TriggerDistance = new TI.Real());
				Add(RunToPlayerDist = new TI.Real());
				Add(new TI.Pad(36));
				Add(Participants = new TI.Block<ai_conversation_participant_block>(this, 8));
				Add(Lines = new TI.Block<ai_conversation_line_block>(this, 32));
				Add(new TI.Pad(12));
			}
		};
		#endregion

		#region hs_scripts_block
		[TI.Definition(-1, 92)]
		public class hs_scripts_block : Scripting.hs_scripts_block
		{
			public hs_scripts_block() : base(5)
			{
				Add(Name = new TI.String());
				Add(ScriptType = new TI.Enum());
				Add(ReturnType = new TI.Enum());
				Add(RootExpressionIndex = new TI.LongInteger());
				Add(new TI.Pad(52));
			}
		};
		#endregion

		#region hs_globals_block
		[TI.Definition(-1, 92)]
		public class hs_globals_block : Scripting.hs_globals_block
		{
			public hs_globals_block() : base(5)
			{
				Add(Name = new TI.String());
				Add(Type = new TI.Enum());
				Add(new TI.Pad(2 + 4));
				Add(InitExpressionIndex = new TI.LongInteger());
				Add(new TI.Pad(48));
			}
		};
		#endregion

		#region hs_references_block
		[TI.Definition(-1, 40)]
		public class hs_references_block : TI.Definition
		{
			#region Fields
			public TI.TagReference Reference;
			#endregion

			public hs_references_block() : base(2)
			{
				Add(new TI.Pad(24));
				Add(Reference = new TI.TagReference(this));
			}
		};
		#endregion

		#region hs_source_files_block
		[TI.Definition(-1, 52)]
		public class hs_source_files_block : TI.Definition
		{
			#region Fields
			public TI.String Name;
			public TI.Data Source;
			#endregion

			public hs_source_files_block() : base(2)
			{
				Add(Name = new TI.String());
				Add(Source = new TI.Data(this));
			}
		};
		#endregion

		#region scenario_cutscene_flag_block
		[TI.Definition(-1, 92)]
		public class scenario_cutscene_flag_block : TI.Definition
		{
			#region Fields
			public TI.String Name;
			public TI.RealPoint3D Position;
			public TI.RealEulerAngles2D Facing;
			#endregion

			public scenario_cutscene_flag_block() : base(5)
			{
				Add(new TI.Pad(4));
				Add(Name = new TI.String());
				Add(Position = new TI.RealPoint3D());
				Add(Facing = new TI.RealEulerAngles2D());
				Add(new TI.Pad(36));
			}
		};
		#endregion

		#region scenario_cutscene_camera_point_block
		[TI.Definition(-1, 104)]
		public class scenario_cutscene_camera_point_block : TI.Definition
		{
			#region Fields
			public TI.String Name;
			public TI.RealPoint3D Position;
			public TI.RealEulerAngles3D Orientation;
			public TI.Real FieldOfView;
			#endregion

			public scenario_cutscene_camera_point_block() : base(7)
			{
				Add(new TI.Pad(4));
				Add(Name = new TI.String());
				Add(new TI.Pad(4));
				Add(Position = new TI.RealPoint3D());
				Add(Orientation = new TI.RealEulerAngles3D());
				Add(FieldOfView = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
				Add(new TI.Pad(36));
			}
		};
		#endregion

		#region scenario_cutscene_title_block
		[TI.Definition(-1, 96)]
		public class scenario_cutscene_title_block : TI.Definition
		{
			#region Fields
			public TI.String Name;
			public TI.Rectangle2D TextBounds;
			public TI.ShortInteger StringIndex;
			public TI.Enum TextStyle; // Undocumented field
			public TI.Enum Justification;
			public TI.Color TextColor;
			public TI.Color ShadowColor;
			public TI.Real FadeInTime;
			public TI.Real UpTime;
			public TI.Real FadeOutTIme;
			#endregion

			public scenario_cutscene_title_block() : base(14)
			{
				Add(new TI.Pad(4));
				Add(Name = new TI.String());
				Add(new TI.Pad(4));
				Add(TextBounds = new TI.Rectangle2D());
				Add(StringIndex = new TI.ShortInteger());
				// TODO: really an enum for text_style
				// 0 = plain
				// 1 = bold
				// 2 = italic
				// 3 = condense
				// 4 = underline
				Add(TextStyle = new TI.Enum());
				Add(Justification = new TI.Enum());
				Add(new TI.Pad(2 + 4));
				Add(TextColor = new TI.Color());
				Add(ShadowColor = new TI.Color());
				Add(FadeInTime = new TI.Real());
				Add(UpTime = new TI.Real());
				Add(FadeOutTIme = new TI.Real());
				Add(new TI.Pad(16));
			}
		};
		#endregion

		#region scenario_bsp_lightmap_set_block
		[TI.Definition(-1, 124)]
		public class scenario_bsp_lightmap_set_block : TI.Definition
		{
			#region Fields
			public TI.String Name;
			public TI.TagReference StandardLightmap;
			public TI.TagReference DirectionalLightmap1;
			public TI.TagReference DirectionalLightmap2;
			public TI.TagReference DirectionalLightmap3;
			#endregion

			public scenario_bsp_lightmap_set_block()
				: base(7)
			{
				Add(Name = new TI.String());
				Add(new TI.Pad(4));
				Add(StandardLightmap = new TI.TagReference(this, TagGroups.bitm));
				Add(DirectionalLightmap1 = new TI.TagReference(this, TagGroups.bitm));
				Add(DirectionalLightmap2 = new TI.TagReference(this, TagGroups.bitm));
				Add(DirectionalLightmap3 = new TI.TagReference(this, TagGroups.bitm));
				Add(new TI.Pad(4 * 6));
			}
		}
		#endregion

		#region scenario_bsp_sky_set_sky_block
		[TI.Definition(-1, 20)]
		public class scenario_bsp_sky_set_sky_block : TI.Definition
		{
			#region Fields
			public TI.BlockIndex SkyIndex;
			public TI.TagReference Sky;
			#endregion

			public scenario_bsp_sky_set_sky_block()
				: base(3)
			{
				Add(new TI.Pad(2));
				Add(SkyIndex = new TI.BlockIndex(TI.FieldType.ShortBlockIndex));
				Add(Sky = new TI.TagReference(this, TagGroups.sky_));
			}
		}
		#endregion

		#region scenario_bsp_sky_set_block
		[TI.Definition(-1, 44)]
		public class scenario_bsp_sky_set_block : TI.Definition
		{
			#region Fields
			public TI.String Name;
			public TI.Block<scenario_bsp_sky_set_sky_block> Skies;
			#endregion

			public scenario_bsp_sky_set_block()
				: base(2)
			{
				Add(Name = new TI.String());
				Add(Skies = new TI.Block<scenario_bsp_sky_set_sky_block>(this, 8));
			}
		}
		#endregion

		#region scenario_bsp_sky_set_block
		[TI.Definition(-1, 64)]
		public class scenario_bsp_modifier_block : TI.Definition
		{
			#region Fields
			public TI.BlockIndex BSPIndex;
			public TI.Block<scenario_bsp_lightmap_set_block> LightmapSets;
			public TI.Block<scenario_bsp_sky_set_block> SkySets;
			#endregion

			public scenario_bsp_modifier_block()
				: base(5)
			{
				Add(new TI.Pad(2));
				Add(BSPIndex = new TI.BlockIndex(TI.FieldType.ShortBlockIndex));
				Add(LightmapSets = new TI.Block<scenario_bsp_lightmap_set_block>(this, 64));
				Add(SkySets = new TI.Block<scenario_bsp_sky_set_block>(this, 64));
				Add(new TI.Pad(4 * 9));
			}
		}
		#endregion

		#region Fields
		private TI.TagReference DontUse;
		private TI.TagReference WontUse;
		private TI.TagReference CantUse;
		public TI.Block<field_block<TI.TagReference>> Skies;
		public TI.Enum Type;
		public TI.Flags Flags;
		public TI.Block<scenario_child_scenario_block> ChildScenarios;
		public TI.Real LocalNorth;
		public TI.Block<predicted_resource_block> PredictedResources;
		public TI.Block<scenario_function_block> Functions;
		public TI.Data EditorData;
		public TI.Block<editor_comment_block> Comments;

		public TI.Block<scenario_object_names_block> ObjectNames;
		public TI.Block<scenario_scenery_block> Scenery;
		public TI.Block<scenario_object_palette_block> SceneryPalette;
		public TI.Block<scenario_biped_block> Bipeds;
		public TI.Block<scenario_object_palette_block> BipedPalette;
		public TI.Block<scenario_vehicle_block> Vehicles;
		public TI.Block<scenario_object_palette_block> VehiclePalette;
		public TI.Block<scenario_equipment_block> Equipment;
		public TI.Block<scenario_object_palette_block> EquipmentPalette;
		public TI.Block<scenario_weapon_block> Weapons;
		public TI.Block<scenario_object_palette_block> WeaponsPalette;
		public TI.Block<device_group_block> DeviceGroups;
		public TI.Block<scenario_machine_block> Machines;
		public TI.Block<scenario_object_palette_block> MachinesPalette;
		public TI.Block<scenario_control_block> Controls;
		public TI.Block<scenario_object_palette_block> ControlsPalette;
		public TI.Block<scenario_light_fixture_block> LightFixtures;
		public TI.Block<scenario_object_palette_block> LightFixturesPalette;
		public TI.Block<scenario_sound_scenery_block> SoundScenery;
		public TI.Block<scenario_object_palette_block> SoundSceneryPalette;

		public TI.Block<scenario_profiles_block> PlayerStartingProfile;
		public TI.Block<scenario_players_block> PlayerStartingLocations;
		public TI.Block<scenario_trigger_volume_block> TriggerVolumes;
		public TI.Block<recorded_animation_block> RecordedAnimations;
		public TI.Block<scenario_netgame_flags_block> NetgameFlags;
		public TI.Block<scenario_netgame_equipment_block> NetgameEquipment;
		public TI.Block<scenario_starting_equipment_block> StartingEquipment;
		public TI.Block<scenario_bsp_switch_trigger_volume_block> BspSwitchTriggerVolumes;
		public TI.Block<scenario_decals_block> Decals;
		public TI.Block<field_block<TI.TagReference>> DecalPalette;
		public TI.Block<scenario_detail_object_collection_palette_block> DetailObjectCollectionPalette;

		public TI.Block<field_block<TI.TagReference>> ActorPalette;
		public TI.Block<encounter_block> Encounters;
		public TI.Block<ai_command_list_block> CommandLists;
		public TI.Block<ai_animation_reference_block> AiAnimationReferences;
		public TI.Block<ai_script_reference_block> AiScriptReferences;
		public TI.Block<ai_recording_reference_block> AiRecordingReferences;
		public TI.Block<ai_conversation_block> AiConverstaions;

		public TI.Data ScriptSyntaxData;
		public TI.Data ScriptStringData;
		public TI.Block<hs_scripts_block> Scripts;
		public TI.Block<hs_globals_block> Globals;
		public TI.Block<hs_references_block> References;
		public TI.Block<hs_source_files_block> SourceFiles;

		public TI.Block<scenario_cutscene_flag_block> CutsceneFlags;
		public TI.Block<scenario_cutscene_camera_point_block> CutsceneCameraPoints;
		public TI.Block<scenario_cutscene_title_block> CutsceneTitles;

		public TI.Block<scenario_bsp_modifier_block> BSPModifiers;

		public TI.TagReference CustomObjectNames;
		public TI.TagReference IngameHelpText;
		public TI.TagReference HudMessages;
		public TI.Block<scenario_structure_bsps_block> StructureBsps;
		#endregion

		public scenario_group() : base(69)
		{
			Add(DontUse = new TI.TagReference(this, TagGroups.yelo));
			Add(WontUse = new TI.TagReference(this, TagGroups.sbsp));
			Add(CantUse = new TI.TagReference(this, TagGroups.sky_));
			Add(Skies = new TI.Block<field_block<TI.TagReference>>(this, 8));
			Add(Type = new TI.Enum());
			Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
			Add(ChildScenarios = new TI.Block<scenario_child_scenario_block>(this, 32)); // child scenarios, tag block data wasn't removed in stubbs for some reason
			Add(LocalNorth = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(new TI.Pad(20 + 136));
			Add(PredictedResources = new TI.Block<predicted_resource_block>(this, 1024));
			Add(Functions = new TI.Block<scenario_function_block>(this, 32));
			Add(EditorData = new TI.Data(this));
			Add(Comments = new TI.Block<editor_comment_block>(this, 1024));
			Add(new TI.Pad(224));

			Add(ObjectNames = new TI.Block<scenario_object_names_block>(this, 512));
			Add(Scenery = new TI.Block<scenario_scenery_block>(this, 2000));
			Add(SceneryPalette = new TI.Block<scenario_object_palette_block>(this, 100));
			Add(Bipeds = new TI.Block<scenario_biped_block>(this, 128));
			Add(BipedPalette = new TI.Block<scenario_object_palette_block>(this, 100));
			Add(Vehicles = new TI.Block<scenario_vehicle_block>(this, 80));
			Add(VehiclePalette = new TI.Block<scenario_object_palette_block>(this, 100)); // silly bungie, why would u want to have up to 100 vehicle types to choose from when you could only possibly choose 80 of them?
			Add(Equipment = new TI.Block<scenario_equipment_block>(this, 256));
			Add(EquipmentPalette = new TI.Block<scenario_object_palette_block>(this, 100));
			Add(Weapons = new TI.Block<scenario_weapon_block>(this, 128));
			Add(WeaponsPalette = new TI.Block<scenario_object_palette_block>(this, 100));
			Add(DeviceGroups = new TI.Block<device_group_block>(this, 128));
			Add(Machines = new TI.Block<scenario_machine_block>(this, 400));
			Add(MachinesPalette = new TI.Block<scenario_object_palette_block>(this, 100));
			Add(Controls = new TI.Block<scenario_control_block>(this, 100));
			Add(ControlsPalette = new TI.Block<scenario_object_palette_block>(this, 100));
			Add(LightFixtures = new TI.Block<scenario_light_fixture_block>(this, 500));
			Add(LightFixturesPalette = new TI.Block<scenario_object_palette_block>(this, 100));
			Add(SoundScenery = new TI.Block<scenario_sound_scenery_block>(this, 256));
			Add(SoundSceneryPalette = new TI.Block<scenario_object_palette_block>(this, 100));
			Add(new TI.Pad(84));

			Add(PlayerStartingProfile = new TI.Block<scenario_profiles_block>(this, 256));
			Add(PlayerStartingLocations = new TI.Block<scenario_players_block>(this, 256));
			Add(TriggerVolumes = new TI.Block<scenario_trigger_volume_block>(this, 256));
			Add(RecordedAnimations = new TI.Block<recorded_animation_block>(this, 1024));
			Add(NetgameFlags = new TI.Block<scenario_netgame_flags_block>(this, 200));
			Add(NetgameEquipment = new TI.Block<scenario_netgame_equipment_block>(this, 200));
			Add(StartingEquipment = new TI.Block<scenario_starting_equipment_block>(this, 200));
			Add(BspSwitchTriggerVolumes = new TI.Block<scenario_bsp_switch_trigger_volume_block>(this, 256));
			Add(Decals = new TI.Block<scenario_decals_block>(this, 65536));
			Add(DecalPalette = new TI.Block<field_block<TI.TagReference>>(this, 128));
			Add(DetailObjectCollectionPalette = new TI.Block<scenario_detail_object_collection_palette_block>(this, 32));
			Add(new TI.Pad(84));

			Add(ActorPalette = new TI.Block<field_block<TI.TagReference>>(this, 64));
			Add(Encounters = new TI.Block<encounter_block>(this, 128));
			Add(CommandLists = new TI.Block<ai_command_list_block>(this, 256));
			Add(AiAnimationReferences = new TI.Block<ai_animation_reference_block>(this, 128));
			Add(AiScriptReferences = new TI.Block<ai_script_reference_block>(this, 128));
			Add(AiRecordingReferences = new TI.Block<ai_recording_reference_block>(this, 128));
			Add(AiConverstaions = new TI.Block<ai_conversation_block>(this, 128));

			Add(ScriptSyntaxData = new TI.Data(this, BlamLib.TagInterface.DataType.ScriptNode));
			Add(ScriptStringData = new TI.Data(this));
			Add(Scripts = new TI.Block<hs_scripts_block>(this, 512));
			Add(Globals = new TI.Block<hs_globals_block>(this, 128));
			Add(References = new TI.Block<hs_references_block>(this, 256));
			Add(SourceFiles = new TI.Block<hs_source_files_block>(this, 8));
			Add(new TI.Pad(24));

			Add(CutsceneFlags = new TI.Block<scenario_cutscene_flag_block>(this, 512));
			Add(CutsceneCameraPoints = new TI.Block<scenario_cutscene_camera_point_block>(this, 512));
			Add(CutsceneTitles = new TI.Block<scenario_cutscene_title_block>(this, 64));

			Add(BSPModifiers = new TI.Block<scenario_bsp_modifier_block>(this, 32));

			Add(new TI.Pad(96));

			Add(CustomObjectNames = new TI.TagReference(this, TagGroups.ustr));
			Add(IngameHelpText = new TI.TagReference(this, TagGroups.ustr));
			Add(HudMessages = new TI.TagReference(this, TagGroups.hmt_));
			Add(StructureBsps = new TI.Block<scenario_structure_bsps_block>(this, 16));

			//Skies.Definition.Value.ResetReferenceGroupTag(TagGroups.sky_);
			//SceneryPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.scen);
			//BipedPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.bipd);
			//VehiclePalette.Definition.Name.ResetReferenceGroupTag(TagGroups.vehi);
			//EquipmentPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.eqip);
			//WeaponsPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.weap);
			//MachinesPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.mach);
			//ControlsPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.ctrl);
			//LightFixturesPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.lifi);
			//SoundSceneryPalette.Definition.Name.ResetReferenceGroupTag(TagGroups.ssce);
			//DecalPalette.Definition.Value.ResetReferenceGroupTag(TagGroups.deca);
			//ActorPalette.Definition.Value.ResetReferenceGroupTag(TagGroups.actr);
			//DetailObjectCollectionPalette.ResetReferenceGroupTag(TagGroups.dobc);
		}
	};
}