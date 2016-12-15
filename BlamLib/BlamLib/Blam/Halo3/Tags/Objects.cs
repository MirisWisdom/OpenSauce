/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo3.Tags
{
	#region object
	[TI.TagGroup((int)TagGroups.Enumerated.obje, 2, object_group.ObjectSize)]
	public class object_group : TI.Definition
	{
		internal const int ObjectSize = 248;
		protected static int ObjectFieldCount = 0;

		#region object_ai_properties_block
		[TI.Definition(1, 16)]
		public class object_ai_properties_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public object_ai_properties_block()
			{
				Add(/*ai flags = */ new TI.Flags());
				Add(/*ai type name = */ new TI.StringId());
				Add(new TI.Pad(4));
				Add(/*ai size = */ new TI.Enum());
				Add(/*leap jump speed = */ new TI.Enum());
			}
			#endregion
		}
		#endregion

		#region object_function_block
		[TI.Definition(1, 44)]
		public class object_function_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public object_function_block()
			{
				Add(/*flags = */ new TI.Flags());
				Add(/*import name = */ new TI.StringId());
				Add(/*export name = */ new TI.StringId());
				Add(/*turn off with = */ new TI.StringId());
				Add(/*min value = */ new TI.Real());
				Add(/*default function = */ new TI.Struct<mapping_function>(this));
				Add(/*scale by = */ new TI.StringId());
			}
			#endregion
		}
		#endregion

		#region object_attachment_block
		[TI.Definition(1, 32)]
		public class object_attachment_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public object_attachment_block()
			{
				Add(/*type = */ new TI.TagReference(this)); // ligh,MGS2,tdtl,cont,effe,lsnd,lens,
				Add(/*marker = */ new TI.StringId());
				Add(/*change color = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*primary scale = */ new TI.StringId());
				Add(/*secondary scale = */ new TI.StringId());
			}
			#endregion
		}
		#endregion

		#region object_widget_block
		[TI.Definition(1, 16)]
		public class object_widget_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public object_widget_block()
			{
				Add(/*type = */ new TI.TagReference(this)); // ant!,devo,whip,BooM,tdtl,
			}
			#endregion
		}
		#endregion

		#region object_change_colors
		[TI.Definition(1, 24)]
		public class object_change_colors : TI.Definition
		{
			#region object_change_color_initial_permutation
			[TI.Definition(1, 32)]
			public class object_change_color_initial_permutation : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public object_change_color_initial_permutation()
				{
					Add(/*weight = */ new TI.Real());
					Add(/*color lower bound = */ new TI.RealColor());
					Add(/*color upper bound = */ new TI.RealColor());
					Add(/*variant name = */ new TI.StringId());
				}
				#endregion
			}
			#endregion

			#region object_change_color_function
			[TI.Definition(1, 40)]
			public class object_change_color_function : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public object_change_color_function()
				{
					Add(new TI.Pad(4));
					Add(/*scale flags = */ new TI.Flags());
					Add(/*color lower bound = */ new TI.RealColor());
					Add(/*color upper bound = */ new TI.RealColor());
					Add(/*darken by = */ new TI.StringId());
					Add(/*scale by = */ new TI.StringId());
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public object_change_colors()
			{
				Add(/*initial permutations = */ new TI.Block<object_change_color_initial_permutation>(this, 32));
				Add(/*functions = */ new TI.Block<object_change_color_function>(this, 4));
			}
			#endregion
		}
		#endregion

		#region Fields
		public TI.Block<object_ai_properties_block> AiProperties;
		public TI.Block<object_function_block> Functions;
		public TI.Block<object_attachment_block> Attachments;
		public TI.Block<object_widget_block> Widgets;
		public TI.Block<object_change_colors> ChangeColors;
		public TI.Block<predicted_resource_block> PredictedResources;
		#endregion

		#region Ctor
		public object_group(int dummy) { }
		public object_group()
		{
			Add(/*object type = */ new TI.Enum());
			Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*bounding radius = */ new TI.Real());
			Add(/*bounding offset = */ new TI.RealPoint3D());
			Add(/*acceleration scale = */ new TI.Real());
			Add(/*lightmap shadow mode = */ new TI.Enum());
			Add(/*sweetener size = */ new TI.Enum(TI.FieldType.ByteEnum));
			Add(new TI.Pad(1));
			// TODO: postprocessed field
			Add(new TI.Pad(4));
			Add(/*dynamic light sphere radius = */ new TI.Real());
			Add(/*dynamic light sphere offset = */ new TI.RealPoint3D());
			Add(/*default model variant = */ new TI.StringId());
			Add(/*model = */ new TI.TagReference(this, TagGroups.hlmt));
			Add(/*crate object = */ new TI.TagReference(this, TagGroups.bloc));
			Add(/*collision damage = */ new TI.TagReference(this, TagGroups.cddf));
			Add(new TI.Pad(12));
			Add(/*creation effect = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*material effects = */ new TI.TagReference(this, TagGroups.foot));
			// TODO: unknown type
			Add(/*? = */ new TI.TagReference(this));
			Add(AiProperties = new TI.Block<object_ai_properties_block>(this, 1));
			Add(Functions = new TI.Block<object_function_block>(this, 256));
			Add(/*hud text message index = */ new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(Attachments = new TI.Block<object_attachment_block>(this, 16));
			Add(Widgets = new TI.Block<object_widget_block>(this, 4));
			Add(TI.Pad.BlockHalo3); // old_object_function_block
			Add(ChangeColors = new TI.Block<object_change_colors>(this, 4));
			Add(PredictedResources = new TI.Block<predicted_resource_block>(this, 2048));

			if (ObjectFieldCount == 0) ObjectFieldCount = this.Count;
		}
		#endregion
	};
	#endregion

	#region biped_lock_on_data_struct
	[TI.Struct((int)StructGroups.Enumerated.blod, 2, 8)]
	public class biped_lock_on_data_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public biped_lock_on_data_struct()
		{
			Add(/*flags = */ new TI.Flags());
			Add(/*lock on distance = */ new TI.Real());
		}
		#endregion
	}
	#endregion

	#region biped
	[TI.TagGroup((int)TagGroups.Enumerated.bipd, 3, 2, /*unit_group.UnitSize +*/ 468, typeof(unit_group))]
	public class biped_group : unit_group
	{
		#region contact_point_block
		[TI.Definition(1, 4)]
		public class contact_point_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public contact_point_block()
			{
				Add(/*marker name = */ new TI.StringId());
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public biped_group(int dummy) : base(dummy) { }
		public biped_group() : base()
		{
			Add(/*moving turning speed = */ new TI.Real(TI.FieldType.Angle));
			Add(/*flags = */ new TI.Flags());
			Add(/*stationary turning threshold = */ new TI.Real(TI.FieldType.Angle));
			Add(/*jump velocity = */ new TI.Real());
			Add(/*maximum soft landing time = */ new TI.Real());
			Add(/*maximum hard landing time = */ new TI.Real());
			Add(/*minimum soft landing velocity = */ new TI.Real());
			Add(/*minimum hard landing velocity = */ new TI.Real());
			Add(/*maximum hard landing velocity = */ new TI.Real());
			Add(/*death hard landing velocity = */ new TI.Real());
			Add(/*stun duration = */ new TI.Real());
			Add(/*standing camera height = */ new TI.Real());
			Add(/*crouching camera height = */ new TI.Real());
			Add(/*crouch transition time = */ new TI.Real());
			Add(/*camera interpolation start = */ new TI.Real(TI.FieldType.Angle));
			Add(/*camera interpolation end = */ new TI.Real(TI.FieldType.Angle));
			Add(/*camera forward movement scale = */ new TI.Real());
			Add(/*camera side movement scale = */ new TI.Real());
			Add(/*camera vertical movement scale = */ new TI.Real());
			Add(/*camera exclusion distance = */ new TI.Real());
			Add(/*autoaim width = */ new TI.Real());
			Add(/*lock-on data = */ new TI.Struct<biped_lock_on_data_struct>(this));
			// TODO: actually used
			Add(new TI.Pad(16));
			Add(/*head shot acc scale = */ new TI.Real());
			Add(/*area damage effect = */ new TI.TagReference(this, TagGroups.effe));

			//Add(/*physics = */ new TI.Struct<character_physics_struct>(this));
			Add(/*contact points = */ new TI.Block<contact_point_block>(this, 3));
			Add(/*reanimation character = */ new TI.TagReference(this, TagGroups.char_));
			Add(/*death spawn character = */ new TI.TagReference(this, TagGroups.char_));
			Add(/*death spawn count = */ new TI.ShortInteger());
			Add(new TI.Pad(2));
		}
		#endregion
	};
	#endregion

	#region projectile
	[TI.TagGroup((int)TagGroups.Enumerated.proj, 6, /*object_group.ObjectSize +*/ 428, typeof(object_group))]
	public class projectile_group : object_group
	{
		#region projectile_material_response_block
		[TI.Definition(1, 112)]
		public class projectile_material_response_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public projectile_material_response_block()
			{
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*response = */ new TI.Enum());
				Add(/*DO NOT USE (OLD effect) = */ new TI.TagReference(this, TagGroups.effe));
				Add(/*material name = */ new TI.StringId());
				Add(new TI.Skip(4));
				Add(/*response = */ new TI.Enum());
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*chance fraction = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*between = */ new TI.RealBounds(TI.FieldType.AngleBounds));
				Add(/*and = */ new TI.RealBounds());
				Add(/*DO NOT USE (OLD effect) = */ new TI.TagReference(this, TagGroups.effe));
				Add(/*scale effects by = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*angular noise = */ new TI.Real(TI.FieldType.Angle));
				Add(/*velocity noise = */ new TI.Real());
				Add(/*DO NOT USE (OLD detonation effect) = */ new TI.TagReference(this, TagGroups.effe));
				Add(/*initial friction = */ new TI.Real());
				Add(/*maximum distance = */ new TI.Real());
				Add(/*parallel friction = */ new TI.Real());
				Add(/*perpendicular friction = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public projectile_group(int dummy) : base(dummy) { }
		public projectile_group() : base()
		{
			Add(/*flags = */ new TI.Flags());
			Add(/*detonation timer starts = */ new TI.Enum());
			Add(/*impact noise = */ new TI.Enum());
			Add(/*AI perception radius = */ new TI.Real());
			Add(/*collision radius = */ new TI.Real());
			Add(/*arming time = */ new TI.Real());
			Add(/*danger radius = */ new TI.Real());
			Add(/*timer = */ new TI.RealBounds());
			Add(/*maximum range = */ new TI.Real());

			Add(/*detonation noise = */ new TI.Enum());
			Add(/*super det. projectile count = */ new TI.ShortInteger());
			Add(/*detonation started = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*detonation effect (airborne) = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*detonation effect (ground) = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*detonation damage = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*attached detonation damage = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*super detonation = */ new TI.TagReference(this, TagGroups.effe));
			//Add(/*your momma! = */ new TI.Struct<super_detonation_damage_struct>(this));
			Add(/*detonation sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*damage reporting type = */ new TI.Enum(TI.FieldType.ByteEnum));
			Add(new TI.Pad(3));
			Add(/*super attached detonation damage = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*material effect radius = */ new TI.Real());
			Add(/*flyby sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*impact effect = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*impact damage = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*boarding detonation time = */ new TI.Real());
			Add(/*boarding detonation damage = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*boarding attached detonation damage = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*air gravity scale = */ new TI.Real());
			Add(/*air damage range = */ new TI.RealBounds());
			Add(/*water gravity scale = */ new TI.Real());
			Add(/*water damage range = */ new TI.RealBounds());
			Add(/*initial velocity = */ new TI.Real());
			Add(/*final velocity = */ new TI.Real());
			//Add(/*blah = */ new TI.Struct<angular_velocity_lower_bound_struct>(this));
			Add(/*guided angular velocity (upper) = */ new TI.Real(TI.FieldType.Angle));
			Add(/*acceleration range = */ new TI.RealBounds());
			Add(new TI.Pad(4));
			Add(/*targeted leading fraction = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*material responses = */ new TI.Block<projectile_material_response_block>(this, 200));
		}
		#endregion
	};
	#endregion

	#region scenery
	[TI.TagGroup((int)TagGroups.Enumerated.scen, 1, 3, /*object_group.ObjectSize +*/ 8, typeof(object_group))]
	public class scenery_group : object_group
	{
		#region Fields
		#endregion

		#region Ctor
		public scenery_group(int dummy) : base(dummy) { }
		public scenery_group() : base()
		{
			Add(/*pathfinding policy = */ new TI.Enum());
			Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*lightmapping policy = */ new TI.Enum());
			Add(new TI.Pad(2));
		}
		#endregion
	};
	#endregion

	#region unit_camera_struct
	//unit camera
	[TI.Struct((int)StructGroups.Enumerated.uncs, 2, 36)]
	public class unit_camera_struct : TI.Definition
	{
		#region unit_camera_track_block
		[TI.Definition(1, 16)]
		public class unit_camera_track_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public unit_camera_track_block()
			{
				Add(/*track = */ new TI.TagReference(this, TagGroups.trak));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public unit_camera_struct()
		{
			// TODO: unknown
			Add(TI.Pad.DWord);
			Add(/*camera marker name = */ new TI.StringId());
			Add(/*camera submerged marker name = */ new TI.StringId());
			Add(/*pitch auto-level = */ new TI.Real(TI.FieldType.Angle));
			Add(/*pitch range = */ new TI.RealBounds(TI.FieldType.AngleBounds));
			Add(/*camera tracks = */ new TI.Block<unit_camera_track_block>(this, 2));
		}
		#endregion
	}
	#endregion

	#region unit_seat_acceleration_struct
	[TI.Struct((int)StructGroups.Enumerated.usas, 1, 20)]
	public class unit_seat_acceleration_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public unit_seat_acceleration_struct()
		{
			Add(/*acceleration range = */ new TI.RealVector3D());
			Add(/*accel action scale = */ new TI.Real());
			Add(/*accel attach scale = */ new TI.Real());
		}
		#endregion
	}
	#endregion

	#region unit_additional_node_names_struct
	[TI.Struct((int)StructGroups.Enumerated.uHnd, 2, 4)]
	public class unit_additional_node_names_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public unit_additional_node_names_struct()
		{
			Add(/*preferred_gun_node = */ new TI.StringId());
		}
		#endregion
	}
	#endregion

	#region unit_boarding_melee_struct
	[TI.Struct((int)StructGroups.Enumerated.ubms, 3, 112)]
	public class unit_boarding_melee_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public unit_boarding_melee_struct()
		{
			Add(/*boarding melee damage = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*boarding melee response = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*ejection melee damage = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*ejection melee response = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*landing melee damage = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*flurry melee damage = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*obstacle smash damage = */ new TI.TagReference(this, TagGroups.jpt_));
		}
		#endregion
	}
	#endregion

	#region unit_boost_struct
	[TI.Struct((int)StructGroups.Enumerated._1234, 1, 20)]
	public class unit_boost_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public unit_boost_struct()
		{
			Add(/*boost peak power = */ new TI.Real());
			Add(/*boost rise power = */ new TI.Real());
			Add(/*boost peak time = */ new TI.Real());
			Add(/*boost fall power = */ new TI.Real());
			Add(/*dead time = */ new TI.Real());
		}
		#endregion
	}
	#endregion

	#region unit_lipsync_scales_struct
	[TI.Struct((int)StructGroups.Enumerated.ulYc, 2, 8)]
	public class unit_lipsync_scales_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public unit_lipsync_scales_struct()
		{
			Add(/*attack weight = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*decay weight = */ new TI.Real(TI.FieldType.RealFraction));
		}
		#endregion
	}
	#endregion

	#region unit
	[TI.TagGroup((int)TagGroups.Enumerated.unit, 4, unit_group.UnitSize, typeof(object_group))]
	public class unit_group : object_group
	{
		internal const int UnitSize = 532 /*+ ObjectSize*/;
		protected static int UnitFieldCount = 0;

		#region unit_postures_block
		[TI.Definition(1, 16)]
		public class unit_postures_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public unit_postures_block()
			{
				Add(/*name = */ new TI.StringId());
				Add(/*pill offset = */ new TI.RealVector3D());
			}
			#endregion
		}
		#endregion

		#region unit_hud_reference_block
		[TI.Definition(1, 16)]
		public class unit_hud_reference_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public unit_hud_reference_block()
			{
				Add(/*unit hud interface = */ new TI.TagReference(this, TagGroups.chdt));
			}
			#endregion
		}
		#endregion

		#region dialogue_variant_block
		[TI.Definition(1, 20)]
		public class dialogue_variant_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public dialogue_variant_block()
			{
				Add(/*variant number = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(/*dialogue = */ new TI.TagReference(this, TagGroups.udlg));
			}
			#endregion
		}
		#endregion

		#region powered_seat_block
		[TI.Definition(1, 8)]
		public class powered_seat_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public powered_seat_block()
			{
				Add(/*driver powerup time = */ new TI.Real());
				Add(/*driver powerdown time = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region unit_weapon_block
		[TI.Definition(1, 16)]
		public class unit_weapon_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public unit_weapon_block()
			{
				Add(/*weapon = */ new TI.TagReference(this, TagGroups.weap));
			}
			#endregion
		}
		#endregion

		#region unit_seat_block
		[TI.Definition(5, 228)]
		public class unit_seat_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public unit_seat_block()
			{
				Add(/*flags = */ new TI.Flags());
				Add(/*label = */ new TI.StringId());
				Add(/*marker name = */ new TI.StringId());
				Add(/*entry marker(s) name = */ new TI.StringId());
				Add(/*boarding grenade marker = */ new TI.StringId());
				Add(/*boarding grenade string = */ new TI.StringId());
				Add(/*boarding melee string = */ new TI.StringId());
				Add(/*ping scale = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*turnover time = */ new TI.Real());
				Add(/*acceleration = */ new TI.Struct<unit_seat_acceleration_struct>(this));
				Add(/*AI scariness = */ new TI.Real());
				Add(/*ai seat type = */ new TI.Enum());
				Add(/*boarding seat = */ new TI.BlockIndex()); // 1 unit_seat_block
				Add(/*listener interpolation factor = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*yaw rate bounds = */ new TI.RealBounds());
				Add(/*pitch rate bounds = */ new TI.RealBounds());
				// TODO: unknown, idk if its a 'real' or not
				Add(new TI.Skip(4));
				Add(/*min speed reference = */ new TI.Real());
				Add(/*max speed reference = */ new TI.Real());
				Add(/*speed exponent = */ new TI.Real());
				Add(/*unit camera = */ new TI.Struct<unit_camera_struct>(this));
				Add(/*unit hud interface = */ new TI.Block<unit_hud_reference_block>(this, 2));
				Add(TI.Pad.BlockHalo3); // [0xC]
					// real
					// real
					// real
				Add(TI.Pad.BlockHalo3); // [0x?]
				Add(/*enter seat string = */ new TI.StringId());
				Add(/*yaw minimum = */ new TI.Real(TI.FieldType.Angle));
				Add(/*yaw maximum = */ new TI.Real(TI.FieldType.Angle));
				Add(/*built-in gunner = */ new TI.TagReference(this, TagGroups.char_));
				Add(/*entry radius = */ new TI.Real());
				Add(/*entry marker cone angle = */ new TI.Real(TI.FieldType.Angle));
				Add(/*entry marker facing angle = */ new TI.Real(TI.FieldType.Angle));
				Add(/*maximum relative velocity = */ new TI.Real());
				Add(/*invisible seat region = */ new TI.StringId());
				Add(/*runtime invisible seat region index = */ new TI.LongInteger());
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		protected unit_group(int dummy) : base(dummy) {}
		public unit_group() : base()
		{
			Add(/*flags = */ new TI.Flags());
			Add(/*default team = */ new TI.Enum());
			Add(/*constant sound volume = */ new TI.Enum());
			Add(TI.Pad.BlockHalo3); // tag block [0x10]
				// byte
				// byte
				// byte
				// byte (pad?)
				// short
				// real
				// real
			Add(/*integrated light toggle = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*camera field of view = */ new TI.Real(TI.FieldType.Angle));
			Add(/*camera stiffness = */ new TI.Real());
			Add(/*unit camera = */ new TI.Struct<unit_camera_struct>(this));
			Add(TI.Pad.BlockHalo3); // unknown [0xC]
			Add(TI.Pad.BlockHalo3); // tag block [0x?]
			Add(/*acceleration = */ new TI.Struct<unit_seat_acceleration_struct>(this));

			Add(/*soft ping threshold = */ new TI.Real());
			Add(/*soft ping interrupt time = */ new TI.Real());
			Add(/*hard ping threshold = */ new TI.Real());
			Add(/*hard ping interrupt time = */ new TI.Real());
			Add(/*hard death threshold = */ new TI.Real());
			Add(/*feign death threshold = */ new TI.Real());
			Add(/*feign death time = */ new TI.Real());
			Add(/*distance of evade anim = */ new TI.Real());
			Add(/*distance of dive anim = */ new TI.Real());
			// There is one less real field in this area of the definition
			// and I'm ASSUMING its this fucking field right herre
			//Add(/*stunned movement threshold = */ new TI.Real());
			Add(/*feign death chance = */ new TI.Real());
			Add(/*feign repeat chance = */ new TI.Real());

			Add(/*spawned turret character = */ new TI.TagReference(this, TagGroups.char_));
			Add(/*spawned actor count = */ new TI.ShortIntegerBounds());
			Add(/*spawned velocity = */ new TI.Real());
			Add(/*aiming velocity maximum = */ new TI.Real(TI.FieldType.Angle));
			Add(/*aiming acceleration maximum = */ new TI.Real(TI.FieldType.Angle));
			Add(/*casual aiming modifier = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*looking velocity maximum = */ new TI.Real(TI.FieldType.Angle));
			Add(/*looking acceleration maximum = */ new TI.Real(TI.FieldType.Angle));
			Add(/*right_hand_node = */ new TI.StringId());
			Add(/*left_hand_node = */ new TI.StringId());
			Add(/*more damn nodes = */ new TI.Struct<unit_additional_node_names_struct>(this));
			Add(/*melee damage = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*your momma = */ new TI.Struct<unit_boarding_melee_struct>(this));
			Add(/*motion sensor blip size = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(/*postures = */ new TI.Block<unit_postures_block>(this, 20));
			Add(/*HUD INTERFACES = */ new TI.Block<unit_hud_reference_block>(this, 2));
			Add(/*dialogue variants = */ new TI.Block<dialogue_variant_block>(this, 16));
			Add(/*grenade velocity = */ new TI.Real());
			Add(/*grenade type = */ new TI.Enum());
			Add(/*grenade count = */ new TI.ShortInteger());
			Add(/*powered seats = */ new TI.Block<powered_seat_block>(this, 2));
			Add(/*weapons = */ new TI.Block<unit_weapon_block>(this, 4));
			Add(/*seats = */ new TI.Block<unit_seat_block>(this, 32));
			// TODO: I'm sure emp has its own struct...
			Add(/*emp radius = */ new TI.Real());
			Add(/*emp drain effect = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*boost = */ new TI.Struct<unit_boost_struct>(this));
			Add(/*lipsync = */ new TI.Struct<unit_lipsync_scales_struct>(this));
			// TODO: eat shit fucker
			Add(/*? = */ new TI.TagReference(this));
			Add(/*? = */ new TI.TagReference(this));

			if (UnitFieldCount == 0) UnitFieldCount = this.Count - ObjectFieldCount;
		}
		#endregion
	};
	#endregion

	#region torque_curve_struct
	//power characteristics of engine
	[TI.Struct((int)StructGroups.Enumerated.trcv, 1, 24)]
	public class torque_curve_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public torque_curve_struct()
		{
			Add(/*min torque = */ new TI.Real());
			Add(/*max torque = */ new TI.Real());
			Add(/*peak torque scale = */ new TI.Real());
			Add(/*past peak torque exponent = */ new TI.Real());
			Add(/*torque at max angular velocity = */ new TI.Real());
			Add(/*torque at 2x max angular velocity = */ new TI.Real());
		}
		#endregion
	}
	#endregion

	#region havok_vehicle_physics_struct
	//havok vehicle physics
	[TI.Struct((int)StructGroups.Enumerated.HVPH, 2, -1)]
	public class havok_vehicle_physics_struct : TI.Definition
	{
		#region anti_gravity_point_definition_block
		[TI.Definition(1, 76)]
		public class anti_gravity_point_definition_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public anti_gravity_point_definition_block()
			{
				Add(/*marker name = */ new TI.StringId());
				Add(/*flags = */ new TI.Flags());
				Add(/*antigrav strength = */ new TI.Real());
				Add(/*antigrav offset = */ new TI.Real());
				Add(/*antigrav height = */ new TI.Real());
				Add(/*antigrav damp factor = */ new TI.Real());
				Add(/*antigrav normal k1 = */ new TI.Real());
				Add(/*antigrav normal k0 = */ new TI.Real());
				Add(/*radius = */ new TI.Real());
				Add(new TI.Pad(12));
				Add(new TI.Pad(2));
				Add(new TI.Pad(2));
				Add(/*damage source region name = */ new TI.StringId());
				Add(/*default state error = */ new TI.Real());
				Add(/*minor damage error = */ new TI.Real());
				Add(/*medium damage error = */ new TI.Real());
				Add(/*major damage error = */ new TI.Real());
				Add(/*destroyed state error = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region friction_point_definition_block
		[TI.Definition(1, 76)]
		public class friction_point_definition_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public friction_point_definition_block()
			{
				Add(/*marker name = */ new TI.StringId());
				Add(/*flags = */ new TI.Flags());
				Add(/*fraction of total mass = */ new TI.Real());
				Add(/*radius = */ new TI.Real());
				Add(/*damaged radius = */ new TI.Real());
				Add(/*friction type = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*moving friction velocity diff = */ new TI.Real());
				Add(/*e-brake moving friction = */ new TI.Real());
				Add(/*e-brake friction = */ new TI.Real());
				Add(/*e-brake moving friction vel diff = */ new TI.Real());
				Add(new TI.Pad(20));
				Add(/*collision global material name = */ new TI.StringId());
				Add(new TI.Pad(2));
				Add(/*model state destroyed = */ new TI.Enum());
				Add(/*region name = */ new TI.StringId());
				Add(new TI.Pad(4));
			}
			#endregion
		}
		#endregion

		#region vehicle_phantom_shape_block
		[TI.Definition(1, 672)]
		public class vehicle_phantom_shape_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public vehicle_phantom_shape_block()
			{
				Add(new TI.Skip(4));
				Add(/*size = */ new TI.ShortInteger());
				Add(/*count = */ new TI.ShortInteger());
				Add(new TI.Skip(4));
				Add(new TI.Skip(4));
				Add(/*child shapes size = */ new TI.LongInteger());
				Add(/*child shapes capacity = */ new TI.LongInteger());

				Add(/*shape type = */ new TI.Enum());
				Add(/*shape = */ new TI.BlockIndex()); // 2
				Add(/*collision filter = */ new TI.LongInteger());

				Add(/*shape type = */ new TI.Enum());
				Add(/*shape = */ new TI.BlockIndex()); // 2
				Add(/*collision filter = */ new TI.LongInteger());

				Add(/*shape type = */ new TI.Enum());
				Add(/*shape = */ new TI.BlockIndex()); // 2
				Add(/*collision filter = */ new TI.LongInteger());

				Add(/*shape type = */ new TI.Enum());
				Add(/*shape = */ new TI.BlockIndex()); // 2
				Add(/*collision filter = */ new TI.LongInteger());

				Add(/*multisphere count = */ new TI.LongInteger());
				Add(/*flags = */ new TI.Flags());
				Add(new TI.Pad(8));
				Add(/*x0 = */ new TI.Real());
				Add(/*x1 = */ new TI.Real());
				Add(/*y0 = */ new TI.Real());
				Add(/*y1 = */ new TI.Real());
				Add(/*z0 = */ new TI.Real());
				Add(/*z1 = */ new TI.Real());

				Add(new TI.Skip(4));
				Add(/*size = */ new TI.ShortInteger());
				Add(/*count = */ new TI.ShortInteger());
				Add(new TI.Skip(4));
				Add(/*num spheres = */ new TI.LongInteger());
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));

				Add(new TI.Skip(4));
				Add(/*size = */ new TI.ShortInteger());
				Add(/*count = */ new TI.ShortInteger());
				Add(new TI.Skip(4));
				Add(/*num spheres = */ new TI.LongInteger());
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));

				Add(new TI.Skip(4));
				Add(/*size = */ new TI.ShortInteger());
				Add(/*count = */ new TI.ShortInteger());
				Add(new TI.Skip(4));
				Add(/*num spheres = */ new TI.LongInteger());
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));

				Add(new TI.Skip(4));
				Add(/*size = */ new TI.ShortInteger());
				Add(/*count = */ new TI.ShortInteger());
				Add(new TI.Skip(4));
				Add(/*num spheres = */ new TI.LongInteger());
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*sphere = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public havok_vehicle_physics_struct()
		{
			// TODO: I REALLY don't know if these are for sure...
			Add(/*flags = */ new TI.Flags());
 			Add(/*ground friction = */ new TI.Real());
 			Add(/*ground depth = */ new TI.Real());
 			Add(/*ground damp factor = */ new TI.Real());
 			Add(/*ground moving friction = */ new TI.Real());
// 			Add(/*ground maximum slope 0 = */ new TI.Real());
// 			Add(/*ground maximum slope 1 = */ new TI.Real());
// 			Add(new TI.Pad(16));
			// real
			// real
			// real
			Add(/*anti_gravity_bank_lift = */ new TI.Real());
			Add(/*steering_bank_reaction_scale = */ new TI.Real());
			Add(/*gravity scale = */ new TI.Real());
			Add(/*radius = */ new TI.Real());
			Add(TI.Pad.BlockHalo3);
			Add(/*anti gravity points = */ new TI.Block<anti_gravity_point_definition_block>(this, 16));
			Add(/*friction points = */ new TI.Block<friction_point_definition_block>(this, 16));
			// TODO: havok structure is bound to have changed...
			Add(TI.Pad.BlockHalo3);//Add(/*shape phantom shape = */ new TI.Block<vehicle_phantom_shape_block>(this, 1));
		}
		#endregion
	}
	#endregion

	#region vehicle
	[TI.TagGroup((int)TagGroups.Enumerated.vehi, 2, /*unit_group.UnitSize +*/ 320, typeof(unit_group))]
	public class vehicle_group : unit_group
	{
		#region engine_block
		[TI.Definition(1, 64)]
		public class engine_block : TI.Definition
		{
			#region gear_block
			[TI.Definition(1, 68)]
			public class gear_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public gear_block()
				{
					Add(/*loaded torque curve = */ new TI.Struct<torque_curve_struct>(this));
					Add(/*cruising torque curve = */ new TI.Struct<torque_curve_struct>(this));
					Add(/*min time to upshift = */ new TI.Real());
					Add(/*engine up-shift scale = */ new TI.Real());
					Add(/*gear ratio = */ new TI.Real());
					Add(/*min time to downshift = */ new TI.Real());
					Add(/*engine down-shift scale = */ new TI.Real());
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public engine_block()
			{
				Add(/*? = */ new TI.Real());
				Add(/*? = */ new TI.Real());
				Add(/*? = */ new TI.Real());
				Add(/*? = */ new TI.Real());
				Add(/*? = */ new TI.Real());
				Add(/*engine moment = */ new TI.Real());
				Add(/*engine max angular velocity = */ new TI.Real());
				Add(/*gears = */ new TI.Block<gear_block>(this, 16));
				Add(/*change gear sound = */ new TI.TagReference(this, TagGroups.snd_));
				Add(/*? = */ new TI.Real());
				Add(/*? = */ new TI.Real());
			}
			#endregion
		};
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public vehicle_group(int dummy) : base(dummy) { }
		public vehicle_group() : base()
		{
			Add(/*flags = */ new TI.Flags());
			Add(new TI.Pad(12));
			Add(/*engine = */ new TI.Block<engine_block>(this, 1));
			Add(new TI.Pad(12));
			Add(TI.Pad.BlockHalo3); // [0x?]
			// unknown [0x48]
			Add(/*havok vehicle physics = */ new TI.Struct<havok_vehicle_physics_struct>(this));
			// byte (enum?)
			// byte (enum?)
			// word
			// real
			// real
			// real
			// real
			Add(/*seat entrance acceleration scale = */ new TI.Real());
			Add(/*seat exit acceleration scale = */ new TI.Real());
			Add(/*flip message = */ new TI.StringId());
			// TODO: the last two reference the global vehicle boost dmg response...
			Add(/*? = */ new TI.TagReference(this));
			Add(/*? = */ new TI.TagReference(this));
			Add(/*? = */ new TI.TagReference(this, TagGroups.drdf));
			Add(/*? = */ new TI.TagReference(this, TagGroups.drdf));
		}
		#endregion
	};
	#endregion
}