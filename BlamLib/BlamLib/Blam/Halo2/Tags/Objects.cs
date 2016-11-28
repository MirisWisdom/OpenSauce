/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo2.Tags
{
	#region biped_lock_on_data_struct
	[TI.Struct((int)StructGroups.Enumerated.blod, 2, 8)]
	public class biped_lock_on_data_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public biped_lock_on_data_struct() : base(2)
		{
			Add(/*flags = */ new TI.Flags());
			Add(/*lock on distance = */ new TI.Real());
		}
		#endregion
	}
	#endregion

	#region character_physics_ground_struct
	//character physics ground
	[TI.Struct((int)StructGroups.Enumerated.chgr, 1, 48)]
	public class character_physics_ground_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public character_physics_ground_struct() : base(11)
		{
			Add(/*maximum slope angle = */ new TI.Real(TI.FieldType.Angle));
			Add(/*downhill falloff angle = */ new TI.Real(TI.FieldType.Angle));
			Add(/*downhill cutoff angle = */ new TI.Real(TI.FieldType.Angle));
			Add(/*uphill falloff angle = */ new TI.Real(TI.FieldType.Angle));
			Add(/*uphill cutoff angle = */ new TI.Real(TI.FieldType.Angle));
			Add(new TI.UselessPad(16));
			Add(/*downhill velocity scale = */ new TI.Real());
			Add(/*uphill velocity scale = */ new TI.Real());
			Add(new TI.UselessPad(8));
			Add(new TI.Pad(20));
			Add(new TI.UselessPad(16));
		}
		#endregion
	}
	#endregion

	#region character_physics_flying_struct
	//character physics ground
	[TI.Struct((int)StructGroups.Enumerated.chfl, 1, 44)]
	public class character_physics_flying_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public character_physics_flying_struct() : base(12)
		{
			Add(/*bank angle = */ new TI.Real(TI.FieldType.Angle));
			Add(/*bank apply time = */ new TI.Real());
			Add(/*bank decay time = */ new TI.Real());
			Add(/*pitch ratio = */ new TI.Real());
			Add(/*max velocity = */ new TI.Real());
			Add(/*max sidestep velocity = */ new TI.Real());
			Add(/*acceleration = */ new TI.Real());
			Add(/*deceleration = */ new TI.Real());
			Add(/*angular velocity maximum = */ new TI.Real(TI.FieldType.Angle));
			Add(/*angular acceleration maximum = */ new TI.Real(TI.FieldType.Angle));
			Add(/*crouch velocity modifier = */ new TI.Real());
			Add(new TI.UselessPad(16));
		}
		#endregion
	}
	#endregion

	#region character_physics_dead_struct
	//character physics dead
	[TI.Struct((int)StructGroups.Enumerated.chdd, 1, 0)]
	public class character_physics_dead_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public character_physics_dead_struct() : base(1)
		{
			Add(new TI.UselessPad(16));
		}
		#endregion
	}
	#endregion

	#region character_physics_sentinel_struct
	//character physics sentinel
	[TI.Struct((int)StructGroups.Enumerated.chsn, 1, 0)]
	public class character_physics_sentinel_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public character_physics_sentinel_struct() : base(1)
		{
			Add(new TI.UselessPad(16));
		}
		#endregion
	}
	#endregion

	#region character_physics_struct
	//character physics
	[TI.Struct((int)StructGroups.Enumerated.chpy, 1, 160)]
	public class character_physics_struct : TI.Definition
	{
		#region spheres_block
		[TI.Definition(1, 128)]
		public class spheres_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public spheres_block() : base(27)
			{
				Add(/*name = */ new TI.StringId());
				Add(/*material = */ new TI.BlockIndex()); // 1 materials_block
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*relative mass scale = */ new TI.Real());
				Add(/*friction = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*restitution = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*volume  = */ new TI.Real());
				Add(/*mass = */ new TI.Real());
				Add(new TI.Skip(2));
				Add(/*phantom = */ new TI.BlockIndex()); // 1 phantoms_block
				Add(new TI.Skip(4));
				Add(/*size = */ new TI.ShortInteger());
				Add(/*count = */ new TI.ShortInteger());
				Add(new TI.Skip(4));
				Add(/*radius = */ new TI.Real());
				Add(new TI.Skip(4));
				Add(/*size = */ new TI.ShortInteger());
				Add(/*count = */ new TI.ShortInteger());
				Add(new TI.Skip(4 + 4));
				Add(/*rotation i = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*rotation j = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*rotation k = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*translation = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
			}
			#endregion
		}
		#endregion

		#region pills_block
		[TI.Definition(1, 80)]
		public class pills_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public pills_block() : base(19)
			{
				Add(/*name = */ new TI.StringId());
				Add(/*material = */ new TI.BlockIndex()); // 1 materials_block
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*relative mass scale = */ new TI.Real());
				Add(/*friction = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*restitution = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*volume  = */ new TI.Real());
				Add(/*mass = */ new TI.Real());
				Add(new TI.Skip(2));
				Add(/*phantom = */ new TI.BlockIndex()); // 1 phantoms_block
				Add(new TI.Skip(4));
				Add(/*size = */ new TI.ShortInteger());
				Add(/*count = */ new TI.ShortInteger());
				Add(new TI.Skip(4));
				Add(/*radius = */ new TI.Real());
				Add(/*bottom = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
				Add(/*top = */ new TI.RealVector3D());
				Add(new TI.Skip(4));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public character_physics_struct() : base(17)
		{
			Add(/*flags = */ new TI.Flags());
			Add(/*height standing = */ new TI.Real());
			Add(/*height crouching = */ new TI.Real());
			Add(/*radius = */ new TI.Real());
			Add(/*mass = */ new TI.Real());
			Add(/*living material name = */ new TI.StringId());
			Add(/*dead material name = */ new TI.StringId());
			Add(new TI.UselessPad(16));
			Add(new TI.Pad(4));
			Add(new TI.UselessPad(20));
			Add(/*dead sphere shapes = */ new TI.Block<spheres_block>(this, 1024));
			Add(/*pill shapes = */ new TI.Block<pills_block>(this, 1024));
			Add(/*sphere shapes = */ new TI.Block<spheres_block>(this, 1024));
			Add(/*ground physics = */ new TI.Struct<character_physics_ground_struct>(this));
			Add(/*flying physics = */ new TI.Struct<character_physics_flying_struct>(this));
			Add(/*dead physics = */ new TI.Struct<character_physics_dead_struct>(this));
			Add(/*sentinel physics = */ new TI.Struct<character_physics_sentinel_struct>(this));
		}
		#endregion
	}
	#endregion

	#region biped
	[TI.TagGroup((int)TagGroups.Enumerated.bipd, 3, 2, /*unit_group.UnitSize +*/ 336, typeof(unit_group))]
	public class biped_group : unit_group
	{
		#region contact_point_block
		[TI.Definition(1, 4)]
		public class contact_point_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public contact_point_block() : base(2)
			{
				Add(new TI.UselessPad(32));
				Add(/*marker name = */ new TI.StringId(true));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public biped_group() : base(35)
		{
			Add(/*moving turning speed = */ new TI.Real(TI.FieldType.Angle));
			Add(/*flags = */ new TI.Flags());
			Add(/*stationary turning threshold = */ new TI.Real(TI.FieldType.Angle));
			Add(new TI.UselessPad(16 + 32));
			Add(/*jump velocity = */ new TI.Real());
			Add(new TI.UselessPad(28));
			Add(/*maximum soft landing time = */ new TI.Real());
			Add(/*maximum hard landing time = */ new TI.Real());
			Add(/*minimum soft landing velocity = */ new TI.Real());
			Add(/*minimum hard landing velocity = */ new TI.Real());
			Add(/*maximum hard landing velocity = */ new TI.Real());
			Add(/*death hard landing velocity = */ new TI.Real());
			Add(new TI.UselessPad(16));
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
			Add(new TI.Pad(16));
			Add(new TI.UselessPad(12));
			Add(/*head shot acc scale = */ new TI.Real());
			Add(/*area damage effect = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*physics = */ new TI.Struct<character_physics_struct>(this));
			Add(/*contact points = */ new TI.Block<contact_point_block>(this, 3));
			Add(/*reanimation character = */ new TI.TagReference(this, TagGroups.char_));
			Add(/*death spawn character = */ new TI.TagReference(this, TagGroups.char_));
			Add(/*death spawn count = */ new TI.ShortInteger());
			Add(new TI.Pad(2));
		}
		#endregion
	};
	#endregion


	#region crate
	[TI.TagGroup((int)TagGroups.Enumerated.bloc, 1, /*object_group.ObjectSize +*/ 4, typeof(object_group))]
	public class crate_group : object_group
	{
		#region Fields
		#endregion

		#region Ctor
		public crate_group() : base(3)
		{
			Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(new TI.UselessPad(124));
		}
		#endregion
	};
	#endregion

	#region creature
	[TI.TagGroup((int)TagGroups.Enumerated.crea, 1, /*object_group.ObjectSize +*/ 224, typeof(object_group))]
	public class creature_group : object_group
	{
		#region Fields
		#endregion

		#region Ctor
		public creature_group() : base(14)
		{
			Add(/*flags = */ new TI.Flags());
			Add(/*default team = */ new TI.Enum());
			Add(/*motion sensor blip size = */ new TI.Enum());
			Add(/*turning velocity maximum = */ new TI.Real(TI.FieldType.Angle));
			Add(/*turning acceleration maximum = */ new TI.Real(TI.FieldType.Angle));
			Add(/*casual turning modifier = */ new TI.Real(TI.FieldType.RealFraction));
			Add(new TI.UselessPad(4));
			Add(/*autoaim width = */ new TI.Real());
			Add(/*physics = */ new TI.Struct<character_physics_struct>(this));
			Add(new TI.UselessPad(64));
			Add(/*impact damage = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*impact shield damage = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(new TI.UselessPad(32));
			Add(/*destroy after death time = */ new TI.RealBounds());
		}
		#endregion
	};
	#endregion


	#region device
	[TI.TagGroup((int)TagGroups.Enumerated.devi, 1, device_group.DeviceSize, typeof(object_group))]
	public class device_group : object_group
	{
		internal const int DeviceSize = 152 /*+ ObjectSize*/;
		internal const int DeviceFieldCount = 21;

		#region Fields
		#endregion

		#region Ctor
		public device_group() : this(0) {}
		protected device_group(int field_count) : base(field_count + DeviceFieldCount)
		{
			Add(/*flags = */ new TI.Flags());
			Add(/*power transition time = */ new TI.Real());
			Add(/*power acceleration time = */ new TI.Real());
			Add(/*position transition time = */ new TI.Real());
			Add(/*position acceleration time = */ new TI.Real());
			Add(/*depowered position transition time = */ new TI.Real());
			Add(/*depowered position acceleration time = */ new TI.Real());
			Add(/*lightmap flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(new TI.UselessPad(4));
			Add(/*open (up) = */ new TI.TagReference(this)); // snd!,effe,
			Add(/*close (down) = */ new TI.TagReference(this)); // snd!,effe,
			Add(/*opened = */ new TI.TagReference(this)); // snd!,effe,
			Add(/*closed = */ new TI.TagReference(this)); // snd!,effe,
			Add(/*depowered = */ new TI.TagReference(this)); // snd!,effe,
			Add(/*repowered = */ new TI.TagReference(this)); // snd!,effe,
			Add(/*delay time = */ new TI.Real());
			Add(new TI.UselessPad(8));
			Add(/*delay effect = */ new TI.TagReference(this)); // snd!,effe,
			Add(/*automatic activation radius = */ new TI.Real());
			Add(new TI.UselessPad(112));
		}
		#endregion

		/// <summary>
		/// This is a very dangerous method if used incorrectly.
		/// Will set all of this object's field to the FieldValue
		/// returned by <paramref name="other"/>'s fields.
		/// </summary>
		/// <param name="other"></param>
		internal void FromDevice(device_group other)
		{
			FromObject(other);
			for (int x = ObjectFieldCount; x < object_group.ObjectFieldCount + DeviceFieldCount; x++)
				this[x].FieldValue = other[x].FieldValue;
		}
	};
	#endregion

	#region device_control
	[TI.TagGroup((int)TagGroups.Enumerated.ctrl, 1, /*device_group.DeviceSize +*/ 60, typeof(device_group))]
	public class device_control_group : device_group
	{
		#region Fields
		#endregion

		#region Ctor
		public device_control_group() : base(8)
		{
			Add(/*type = */ new TI.Enum());
			Add(/*triggers when = */ new TI.Enum());
			Add(/*call value = */ new TI.Real());
			Add(/*action string = */ new TI.StringId());
			Add(new TI.UselessPad(76));
			Add(/*on = */ new TI.TagReference(this)); // snd!,effe,
			Add(/*off = */ new TI.TagReference(this)); // snd!,effe,
			Add(/*deny = */ new TI.TagReference(this)); // snd!,effe,
		}
		#endregion
	};
	#endregion

	#region device_light_fixture
	[TI.TagGroup((int)TagGroups.Enumerated.lifi, 1, /*device_group.DeviceSize +*/ 0, typeof(device_group))]
	public class device_light_fixture_group : device_group
	{
		#region Fields
		#endregion

		#region Ctor
		public device_light_fixture_group() : base(1)
		{
			Add(new TI.UselessPad(64));
		}
		#endregion
	};
	#endregion

	#region device_machine
	[TI.TagGroup((int)TagGroups.Enumerated.mach, 1, /*device_group.DeviceSize +*/ 24, typeof(device_group))]
	public class device_machine_group : device_group
	{
		#region Fields
		#endregion

		#region Ctor
		public device_machine_group() : base(10)
		{
			Add(/*type = */ new TI.Enum());
			Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*door open time = */ new TI.Real());
			Add(/*door occlusion bounds = */ new TI.RealBounds(TI.FieldType.RealFractionBounds));
			Add(new TI.UselessPad(72));
			Add(/*collision response = */ new TI.Enum());
			Add(/*elevator node = */ new TI.ShortInteger());
			Add(new TI.UselessPad(68));
			Add(/*pathfinding policy = */ new TI.Enum());
			Add(new TI.Pad(2));
		}
		#endregion
	};
	#endregion


	#region equipment
	[TI.TagGroup((int)TagGroups.Enumerated.eqip, 2, /*item_group.ItemSize +*/ 24, typeof(item_group))]
	public class equipment_group : item_group
	{
		#region Fields
		#endregion

		#region Ctor
		public equipment_group() : base(5)
		{
			Add(/*powerup type = */ new TI.Enum());
			Add(/*grenade type = */ new TI.Enum());
			Add(/*powerup time = */ new TI.Real());
			Add(/*pickup sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(new TI.UselessPad(144));
		}
		#endregion
	};
	#endregion

	#region garbage
	[TI.TagGroup((int)TagGroups.Enumerated.garb, 1, /*item_group.ItemSize +*/ 168, typeof(item_group))]
	public class garbage_group : item_group
	{
		#region Fields
		#endregion

		#region Ctor
		public garbage_group() : base(1)
		{
			Add(new TI.Pad(168));
		}
		#endregion
	};
	#endregion

	#region item
	[TI.TagGroup((int)TagGroups.Enumerated.item, 2, item_group.ItemSize, typeof(object_group))]
	public class item_group : object_group
	{
		internal const int ItemSize = 156 /*+ ObjectSize*/;
		internal const int ItemFieldCount = 24;

		#region predicted_bitmaps_block
		[TI.Definition(1, 16)]
		public class predicted_bitmaps_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public predicted_bitmaps_block() : base(1)
			{
				Add(/*bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public item_group() : this(0) { }
		public item_group(int field_count) : base(field_count + ItemFieldCount)
		{
			Add(/*flags = */ new TI.Flags());
			Add(/*OLD message index = */ new TI.ShortInteger());
			Add(/*sort order = */ new TI.ShortInteger());
			Add(/*multiplayer on-ground scale = */ new TI.Real());
			Add(/*campaign on-ground scale = */ new TI.Real());
			Add(/*pickup message = */ new TI.StringId());
			Add(/*swap message = */ new TI.StringId());
			Add(/*pickup or dual msg = */ new TI.StringId());
			Add(/*swap or dual msg = */ new TI.StringId());
			Add(/*dual-only msg = */ new TI.StringId());
			Add(/*picked up msg = */ new TI.StringId());
			Add(/*singluar quantity msg = */ new TI.StringId());
			Add(/*plural quantity msg = */ new TI.StringId());
			Add(/*switch-to msg = */ new TI.StringId());
			Add(/*switch-to from ai msg = */ new TI.StringId());
			Add(new TI.UselessPad(148));
			Add(/*UNUSED = */ new TI.TagReference(this, TagGroups.foot));
			Add(/*collision sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*predicted bitmaps = */ new TI.Block<predicted_bitmaps_block>(this, 8));
			Add(new TI.UselessPad(92));
			Add(/*detonation damage effect = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*detonation delay = */ new TI.RealBounds());
			Add(/*detonating effect = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*detonation effect = */ new TI.TagReference(this, TagGroups.effe));
		}
		#endregion

		/// <summary>
		/// This is a very dangerous method if used incorrectly.
		/// Will set all of this object's field to the FieldValue
		/// returned by <paramref name="other"/>'s fields.
		/// </summary>
		/// <param name="other"></param>
		internal void FromItem(item_group other)
		{
			FromObject(other);
			for (int x = ObjectFieldCount; x < object_group.ObjectFieldCount + ItemFieldCount; x++)
				this[x].FieldValue = other[x].FieldValue;
		}
	};
	#endregion

	#region item_collection
	[TI.TagGroup((int)TagGroups.Enumerated.itmc, 0, 16)]
	public class item_collection_group : TI.Definition
	{
		#region item_permutation
		[TI.Definition(1, 24)]
		public class item_permutation : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public item_permutation() : base(5)
			{
				Add(new TI.UselessPad(32));
				Add(/*weight = */ new TI.Real());
				Add(/*item = */ new TI.TagReference(this, TagGroups.item));
				Add(/*variant name = */ new TI.StringId());
				Add(new TI.UselessPad(28));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public item_collection_group() : base(4)
		{
			Add(/*item permutations = */ new TI.Block<item_permutation>(this, 32));
			Add(/*spawn time (in seconds, 0 = default) = */ new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(new TI.UselessPad(76));
		}
		#endregion
	};
	#endregion

	#region instantaneous_response_damage_effect_struct
	[TI.Struct((int)StructGroups.Enumerated.ires, 2, 16)]
	public class instantaneous_response_damage_effect_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public instantaneous_response_damage_effect_struct() : base(1)
		{
			Add(/*transition damage effect = */ new TI.TagReference(this, TagGroups.jpt_));
		}
		#endregion
	}
	#endregion

	#region instantaneous_response_damage_effect_marker_struct
	[TI.Struct((int)StructGroups.Enumerated.irem, 2, 4)]
	public class instantaneous_response_damage_effect_marker_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public instantaneous_response_damage_effect_marker_struct() : base(1)
		{
			Add(/*damage effect marker name = */ new TI.StringId());
		}
		#endregion
	}
	#endregion

	#region model_target_lock_on_data_struct
	[TI.Struct((int)StructGroups.Enumerated.MTLO, 1, 8)]
	public class model_target_lock_on_data_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public model_target_lock_on_data_struct() : base(2)
		{
			Add(/*flags = */ new TI.Flags());
			Add(/*lock on distance = */ new TI.Real());
		}
		#endregion
	}
	#endregion

	#region model
	[TI.TagGroup((int)TagGroups.Enumerated.hlmt, 1, 2, 348)]
	public class model_group : TI.Definition
	{
		#region model_variant_block
		[TI.Definition(1, 72)]
		public class model_variant_block : TI.Definition
		{
			#region model_variant_region_block
			[TI.Definition(1, 24)]
			public class model_variant_region_block : TI.Definition
			{
				#region model_variant_permutation_block
				[TI.Definition(1, 36)]
				public class model_variant_permutation_block : TI.Definition
				{
					#region model_variant_state_block
					[TI.Definition(2, 32)]
					public class model_variant_state_block : TI.Definition
					{
						#region Fields
						#endregion

						#region Ctor
						public model_variant_state_block() : base(7)
						{
							Add(/*permutation name = */ new TI.StringId());
							Add(new TI.Pad(1));
							Add(/*property flags = */ new TI.Flags(TI.FieldType.ByteFlags));
							Add(/*state = */ new TI.Enum());
							Add(/*looping effect = */ new TI.TagReference(this, TagGroups.effe));
							Add(/*looping effect marker name = */ new TI.StringId());
							Add(/*initial probability = */ new TI.Real(TI.FieldType.RealFraction));
						}
						#endregion
					}
					#endregion

					#region Fields
					#endregion

					#region Ctor
					public model_variant_permutation_block() : base(7)
					{
						Add(/*permutation name = */ new TI.StringId());
						Add(new TI.Pad(1));
						Add(/*flags = */ new TI.Flags(TI.FieldType.ByteFlags));
						Add(new TI.Pad(2));
						Add(/*probability = */ new TI.Real());
						Add(/*states = */ new TI.Block<model_variant_state_block>(this, 10));
						Add(new TI.Pad(5 + 7));
					}
					#endregion
				}
				#endregion

				#region Fields
				#endregion

				#region Ctor
				public model_variant_region_block() : base(6)
				{
					Add(/*region name = */ new TI.StringId());
					Add(new TI.Pad(1 + 1));
					Add(/*parent variant = */ new TI.BlockIndex()); // 1 model_variant_block
					Add(/*permutations = */ new TI.Block<model_variant_permutation_block>(this, 32));
					Add(/*sort order = */ new TI.Enum());
					Add(new TI.Pad(2));
				}
				#endregion
			}
			#endregion

			#region model_variant_object_block
			[TI.Definition(1, 24)]
			public class model_variant_object_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public model_variant_object_block() : base(3)
				{
					Add(/*parent marker = */ new TI.StringId());
					Add(/*child marker = */ new TI.StringId());
					Add(/*child object = */ new TI.TagReference(this, TagGroups.obje));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public model_variant_block() : base(7)
			{
				Add(/*name = */ new TI.StringId());
				Add(new TI.Pad(16));
				Add(/*regions = */ new TI.Block<model_variant_region_block>(this, 16));
				Add(/*objects = */ new TI.Block<model_variant_object_block>(this, 16));
				Add(new TI.Pad(8));
				Add(/*dialogue sound effect = */ new TI.StringId());
				Add(/*dialogue = */ new TI.TagReference(this, TagGroups.udlg));
			}
			#endregion
		}
		#endregion

		#region model_material_block
		[TI.Definition(1, 20)]
		public class model_material_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public model_material_block() : base(6)
			{
				Add(/*material name = */ new TI.StringId());
				Add(/*material type = */ new TI.Enum());
				Add(/*damage section = */ new TI.BlockIndex()); // 2
				Add(new TI.Pad(2 + 2));
				Add(/*global material name = */ new TI.StringId());
				Add(new TI.Pad(4));
			}
			#endregion
		}
		#endregion

		#region global_damage_info_block
		[TI.Definition(2, 320)]
		public class global_damage_info_block : TI.Definition
		{
			#region global_damage_section_block
			[TI.Definition(1, 68)]
			public class global_damage_section_block : TI.Definition
			{
				#region instantaneous_damage_repsonse_block
				[TI.Definition(1, 104)]
				public class instantaneous_damage_repsonse_block : TI.Definition
				{
					#region Fields
					#endregion

					#region Ctor
					public instantaneous_damage_repsonse_block() : base(19)
					{
						Add(/*response type = */ new TI.Enum());
						Add(/*constraint damage type = */ new TI.Enum());
						Add(/*flags = */ new TI.Flags());
						Add(/*damage threshold = */ new TI.Real());
						Add(/*transition effect = */ new TI.TagReference(this, TagGroups.effe));
						Add(/*damage effect = */ new TI.Struct<instantaneous_response_damage_effect_struct>(this));
						Add(/*region = */ new TI.StringId());
						Add(/*new state = */ new TI.Enum());
						Add(/*runtime region index = */ new TI.ShortInteger());
						Add(/*effect marker name = */ new TI.StringId());
						Add(/*damage effect marker = */ new TI.Struct<instantaneous_response_damage_effect_marker_struct>(this));
						Add(/*response delay = */ new TI.Real());
						Add(/*delay effect = */ new TI.TagReference(this, TagGroups.effe));
						Add(/*delay effect marker name = */ new TI.StringId());
						Add(/*constraint/group name = */ new TI.StringId());
						Add(/*ejecting seat label = */ new TI.StringId());
						Add(/*skip fraction = */ new TI.Real(TI.FieldType.RealFraction));
						Add(/*destroyed child object marker name = */ new TI.StringId());
						Add(/*total damage threshold = */ new TI.Real(TI.FieldType.RealFraction));
					}
					#endregion
				}
				#endregion

				#region Fields
				#endregion

				#region Ctor
				public global_damage_section_block() : base(11)
				{
					Add(/*name = */ new TI.StringId());
					Add(/*flags = */ new TI.Flags());
					Add(/*vitality percentage = */ new TI.Real(TI.FieldType.RealFraction));
					Add(/*instant responses = */ new TI.Block<instantaneous_damage_repsonse_block>(this, 16));
					Add(/* = */ new TI.Block<TI.g_null_block>(this, 0));
					Add(/* = */ new TI.Block<TI.g_null_block>(this, 0));
					Add(/*stun time = */ new TI.Real());
					Add(/*recharge time = */ new TI.Real());
					Add(new TI.Pad(4));
					Add(/*resurrection restored region name = */ new TI.StringId());
					Add(new TI.Pad(4));
				}
				#endregion
			}
			#endregion

			#region global_damage_nodes_block
			[TI.Definition(1, 16)]
			public class global_damage_nodes_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public global_damage_nodes_block() : base(1)
				{
					Add(new TI.Pad(2 + 2 + 12));
				}
				#endregion
			}
			#endregion

			#region damage_seat_info_block
			[TI.Definition(1, 20)]
			public class damage_seat_info_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public damage_seat_info_block() : base(5)
				{
					Add(/*seat label = */ new TI.StringId());
					Add(/*direct damage scale = */ new TI.Real(TI.FieldType.RealFraction));
					Add(/*damage transfer fall-off radius = */ new TI.Real());
					Add(/*maximum transfer damage scale = */ new TI.Real());
					Add(/*minimum transfer damage scale = */ new TI.Real());
				}
				#endregion
			}
			#endregion

			#region damage_constraint_info_block
			[TI.Definition(1, 20)]
			public class damage_constraint_info_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public damage_constraint_info_block() : base(5)
				{
					Add(/*physics model constraint name = */ new TI.StringId());
					Add(/*damage constraint name = */ new TI.StringId());
					Add(/*damage constraint group name = */ new TI.StringId());
					Add(/*group probability scale = */ new TI.Real());
					Add(new TI.Pad(4));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public global_damage_info_block() : base(31)
			{
				Add(/*flags = */ new TI.Flags());
				Add(/*global indirect material name = */ new TI.StringId());
				Add(/*indirect damage section = */ new TI.BlockIndex()); // 2 3¿Å|$Ùe?
				Add(new TI.Pad(2 + 4));
				Add(/*collision damage reporting type = */ new TI.Enum(TI.FieldType.ByteEnum));
				Add(/*response damage reporting type = */ new TI.Enum(TI.FieldType.ByteEnum));
				Add(new TI.Pad(2 + 20));
				Add(/*maximum vitality = */ new TI.Real());
				Add(/*minimum stun damage = */ new TI.Real());
				Add(/*stun time = */ new TI.Real());
				Add(/*recharge time = */ new TI.Real());
				Add(/*recharge fraction = */ new TI.Real(TI.FieldType.RealFraction));
				Add(new TI.Pad(64));
				Add(/*shield damaged first person shader = */ new TI.TagReference(this, TagGroups.shad));
				Add(/*shield damaged shader = */ new TI.TagReference(this, TagGroups.shad));
				Add(/*maximum shield vitality = */ new TI.Real());
				Add(/*global shield material name = */ new TI.StringId());
				Add(/*minimum stun damage = */ new TI.Real());
				Add(/*stun time = */ new TI.Real());
				Add(/*recharge time = */ new TI.Real());
				Add(/*shield damaged threshold = */ new TI.Real());
				Add(/*shield damaged effect = */ new TI.TagReference(this, TagGroups.effe));
				Add(/*shield depleted effect = */ new TI.TagReference(this, TagGroups.effe));
				Add(/*shield recharging effect = */ new TI.TagReference(this, TagGroups.effe));
				Add(/*damage sections = */ new TI.Block<global_damage_section_block>(this, 16));
				Add(/*nodes = */ new TI.Block<global_damage_nodes_block>(this, 255));
				Add(new TI.Pad(2 + 2 + 4 + 4));
				Add(/*damage seats = */ new TI.Block<damage_seat_info_block>(this, 16));
				Add(/*damage constraints = */ new TI.Block<damage_constraint_info_block>(this, 16));
				Add(/*overshield first person shader = */ new TI.TagReference(this, TagGroups.shad));
				Add(/*overshield shader = */ new TI.TagReference(this, TagGroups.shad));
			}
			#endregion
		}
		#endregion

		#region model_target_block
		[TI.Definition(1, 28)]
		public class model_target_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public model_target_block() : base(7)
			{
				Add(/*marker name = */ new TI.StringId());
				Add(/*size = */ new TI.Real());
				Add(/*cone angle = */ new TI.Real(TI.FieldType.Angle));
				Add(/*damage section = */ new TI.BlockIndex()); // 2
				Add(/*variant = */ new TI.BlockIndex()); // 1 model_variant_block
				Add(/*targeting relevance = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*lock-on data = */ new TI.Struct<model_target_lock_on_data_struct>(this));
			}
			#endregion
		}
		#endregion

		#region model_region_block
		[TI.Definition(1, 20)]
		public class model_region_block : TI.Definition
		{
			#region model_permutation_block
			[TI.Definition(1, 8)]
			public class model_permutation_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public model_permutation_block() : base(4)
				{
					Add(/*name = */ new TI.StringId());
					Add(/*flags = */ new TI.Flags(TI.FieldType.ByteFlags));
					Add(/*collision permutation index = */ new TI.ByteInteger());
					Add(new TI.Pad(2));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public model_region_block() : base(5)
			{
				Add(/*name = */ new TI.StringId());
				Add(/*collision region index = */ new TI.ByteInteger());
				Add(/*physics region index = */ new TI.ByteInteger());
				Add(new TI.Pad(2));
				Add(/*permutations = */ new TI.Block<model_permutation_block>(this, 32));
			}
			#endregion
		}
		#endregion

		#region model_node_block
		[TI.Definition(1, 92)]
		public class model_node_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public model_node_block() : base(12)
			{
				Add(/*name = */ new TI.StringId());
				Add(/*parent node = */ new TI.BlockIndex()); // 1 model_node_block
				Add(/*first child node = */ new TI.BlockIndex()); // 1 model_node_block
				Add(/*next sibling node = */ new TI.BlockIndex()); // 1 model_node_block
				Add(new TI.Pad(2));
				Add(/*default translation = */ new TI.RealPoint3D());
				Add(/*default rotation = */ new TI.RealQuaternion());
				Add(/*default inverse scale = */ new TI.Real());
				Add(/*default inverse forward = */ new TI.RealVector3D());
				Add(/*default inverse left = */ new TI.RealVector3D());
				Add(/*default inverse up = */ new TI.RealVector3D());
				Add(/*default inverse position = */ new TI.RealPoint3D());
			}
			#endregion
		}
		#endregion

		#region model_object_data_block
		[TI.Definition(1, 20)]
		public class model_object_data_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public model_object_data_block() : base(4)
			{
				Add(/*type = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*offset = */ new TI.RealPoint3D());
				Add(/*radius = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region global_scenario_load_parameters_block
		[TI.Definition(1, 68)]
		public class global_scenario_load_parameters_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public global_scenario_load_parameters_block() : base(3)
			{
				// Explanation here
				Add(/*scenario = */ new TI.TagReference(this, TagGroups.scnr));
				Add(/*parameters = */ new TI.Data(this));
				Add(new TI.Pad(32));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public model_group() : base(96)
		{
			Add(/*render model = */ new TI.TagReference(this, TagGroups.mode));
			Add(/*collision model = */ new TI.TagReference(this, TagGroups.coll));
			Add(/*animation = */ new TI.TagReference(this, TagGroups.jmad));
			Add(/*physics = */ new TI.TagReference(this, TagGroups.phys));
			Add(/*physics_model = */ new TI.TagReference(this, TagGroups.phmo));
			Add(/*disappear distance = */ new TI.Real());
			Add(/*begin fade distance = */ new TI.Real());
			Add(new TI.Pad(4));
			Add(/*reduce to L1 = */ new TI.Real());
			Add(/*reduce to L2 = */ new TI.Real());
			Add(/*reduce to L3 = */ new TI.Real());
			Add(/*reduce to L4 = */ new TI.Real());
			Add(/*reduce to L5 = */ new TI.Real());
			Add(new TI.Skip(4));
			Add(/*shadow fade distance = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(/*variants = */ new TI.Block<model_variant_block>(this, 64));
			Add(/*materials = */ new TI.Block<model_material_block>(this, 32));
			Add(/*new damage info = */ new TI.Block<global_damage_info_block>(this, 1));
			Add(/*targets = */ new TI.Block<model_target_block>(this, 32));
			Add(/* = */ new TI.Block<model_region_block>(this, 16));
			Add(/* = */ new TI.Block<model_node_block>(this, 255));
			Add(new TI.Pad(4));
			Add(/*model object data = */ new TI.Block<model_object_data_block>(this, 1));
			Add(/*default dialogue = */ new TI.TagReference(this, TagGroups.udlg));
			Add(/*UNUSED = */ new TI.TagReference(this, TagGroups.shad));
			Add(/*flags = */ new TI.Flags());
			Add(/*default dialogue effect = */ new TI.StringId());

			for (int x = 0; x < 32; x++)
				Add(/* = */ new TI.ByteInteger());

			for (int x = 0; x < 32; x++)
				Add(/* = */ new TI.ByteInteger());

			Add(/*runtime flags = */ new TI.Flags());
			Add(/*scenario load parameters = */ new TI.Block<global_scenario_load_parameters_block>(this, 32));
			Add(/*hologram shader = */ new TI.TagReference(this, TagGroups.shad));
			Add(/*hologram control function = */ new TI.StringId());
		}
		#endregion
	};
	#endregion

	#region object
	[TI.TagGroup((int)TagGroups.Enumerated.obje, 1, object_group.ObjectSize)]
	public class object_group : TI.Definition
	{
		internal const int ObjectSize = 256;
		internal const int ObjectFieldCount = 40;

		#region object_ai_properties_block
		[TI.Definition(1, 16)]
		public class object_ai_properties_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public object_ai_properties_block() : base(5)
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
		[TI.Definition(1, 36)]
		public class object_function_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public object_function_block() : base(7)
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
			public object_attachment_block() : base(8)
			{
				Add(/*type = */ new TI.TagReference(this)); // ligh,MGS2,tdtl,cont,effe,lsnd,lens,
				Add(/*marker = */ new TI.StringId(true));
				Add(new TI.UselessPad(4));
				Add(/*change color = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*primary scale = */ new TI.StringId());
				Add(/*secondary scale = */ new TI.StringId());
				Add(new TI.UselessPad(8));
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
			public object_widget_block() : base(2)
			{
				Add(/*type = */ new TI.TagReference(this)); // ant!,devo,whip,BooM,tdtl,
				Add(new TI.UselessPad(16));
			}
			#endregion
		}
		#endregion

		#region old_object_function_block
		[TI.Definition(1, 80)]
		public class old_object_function_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public old_object_function_block() : base(2)
			{
				Add(new TI.Pad(76));
				Add(/* = */ new TI.StringId(true));
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
				public object_change_color_initial_permutation() : base(4)
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
				public object_change_color_function() : base(6)
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
			public object_change_colors() : base(3)
			{
				Add(new TI.UselessPad(32));
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
		public TI.Block<old_object_function_block> OldFunctions;
		public TI.Block<object_change_colors> ChangeColors;
		public TI.Block<predicted_resource_block> PredictedResources;
		#endregion

		#region Ctor
		public object_group() : this(0) {}
		protected object_group(int field_count) : base(field_count + ObjectFieldCount)
		{
			Add(/*object type = */ new TI.Enum());
			Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*bounding radius = */ new TI.Real());
			Add(/*bounding offset = */ new TI.RealPoint3D());
			Add(new TI.UselessPad(12));
			Add(/*acceleration scale = */ new TI.Real());
			Add(/*lightmap shadow mode = */ new TI.Enum());
			Add(/*sweetener size = */ new TI.Enum(TI.FieldType.ByteEnum));
			Add(new TI.Pad(1 + 4));
			Add(new TI.UselessPad(32));
			Add(/*dynamic light sphere radius = */ new TI.Real());
			Add(/*dynamic light sphere offset = */ new TI.RealPoint3D());
			Add(/*default model variant = */ new TI.StringId());
			Add(/*model = */ new TI.TagReference(this, TagGroups.hlmt));
			Add(/*crate object = */ new TI.TagReference(this, TagGroups.bloc));
			Add(new TI.UselessPad(16));
			Add(/*modifier shader = */ new TI.TagReference(this, TagGroups.shad));
			Add(/*creation effect = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*material effects = */ new TI.TagReference(this, TagGroups.foot));
			Add(new TI.UselessPad(24));
			Add(AiProperties = new TI.Block<object_ai_properties_block>(this, 1));
			Add(new TI.UselessPad(24));
			Add(Functions = new TI.Block<object_function_block>(this, 256));
			Add(new TI.UselessPad(16));
			Add(/*Apply collision damage scale = */ new TI.Real());
			Add(/*min game acc (default) = */ new TI.Real());
			Add(/*max game acc (default) = */ new TI.Real());
			Add(/*min game scale (default) = */ new TI.Real());
			Add(/*max game scale (default) = */ new TI.Real());
			Add(/*min abs acc (default) = */ new TI.Real());
			Add(/*max abs acc (default) = */ new TI.Real());
			Add(/*min abs scale (default) = */ new TI.Real());
			Add(/*max abs scale (default) = */ new TI.Real());
			Add(/*hud text message index = */ new TI.ShortInteger());
			Add(new TI.Pad(2));
			Add(Attachments = new TI.Block<object_attachment_block>(this, 16));
			Add(Widgets = new TI.Block<object_widget_block>(this, 4));
			Add(OldFunctions = new TI.Block<old_object_function_block>(this, 4));
			Add(ChangeColors = new TI.Block<object_change_colors>(this, 4));
			Add(PredictedResources = new TI.Block<predicted_resource_block>(this, 2048));
		}
		#endregion

		internal override bool Reconstruct(BlamLib.Blam.CacheFile c)
		{
			PredictedResources.DeleteAll();

			return true;
		}

		/// <summary>
		/// This is a very dangerous method if used incorrectly.
		/// Will set all of this object's field to the FieldValue
		/// returned by <paramref name="other"/>'s fields.
		/// </summary>
		/// <param name="other"></param>
		internal void FromObject(object_group other)
		{
			for(int x = 0; x < ObjectFieldCount; x++)
				this[x].FieldValue = other[x].FieldValue;
		}
	};
	#endregion

	#region super_detonation_damage_struct
	//breakfast
	[TI.Struct((int)StructGroups.Enumerated.sd2s, 2, 16)]
	public class super_detonation_damage_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public super_detonation_damage_struct() : base(1)
		{
			Add(/*super detonation damage = */ new TI.TagReference(this, TagGroups.jpt_));
		}
		#endregion
	}
	#endregion

	#region angular_velocity_lower_bound_struct
	//breakfast
	[TI.Struct((int)StructGroups.Enumerated.avlb, 2, 4)]
	public class angular_velocity_lower_bound_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public angular_velocity_lower_bound_struct() : base(1)
		{
			Add(/*guided angular velocity (lower) = */ new TI.Real(TI.FieldType.Angle));
		}
		#endregion
	}
	#endregion

	#region projectile
	[TI.TagGroup((int)TagGroups.Enumerated.proj, 5, 2, /*object_group.ObjectSize +*/ 348, typeof(object_group))]
	public class projectile_group : object_group
	{
		#region projectile_material_response_block
		[TI.Definition(1, 112)]
		public class projectile_material_response_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public projectile_material_response_block() : base(23)
			{
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*response = */ new TI.Enum());
				Add(/*DO NOT USE (OLD effect) = */ new TI.TagReference(this, TagGroups.effe));
				Add(/*material name = */ new TI.StringId());
				Add(new TI.Skip(4));
				Add(new TI.UselessPad(8));
				Add(/*response = */ new TI.Enum());
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*chance fraction = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*between = */ new TI.RealBounds(TI.FieldType.AngleBounds));
				Add(/*and = */ new TI.RealBounds());
				Add(/*DO NOT USE (OLD effect) = */ new TI.TagReference(this, TagGroups.effe));
				Add(new TI.UselessPad(16));
				Add(/*scale effects by = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*angular noise = */ new TI.Real(TI.FieldType.Angle));
				Add(/*velocity noise = */ new TI.Real());
				Add(/*DO NOT USE (OLD detonation effect) = */ new TI.TagReference(this, TagGroups.effe));
				Add(new TI.UselessPad(24));
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
		public projectile_group() : base(46)
		{
			Add(/*flags = */ new TI.Flags());
			Add(/*detonation timer starts = */ new TI.Enum());
			Add(/*impact noise = */ new TI.Enum());
			Add(new TI.UselessPad(8));
			Add(/*AI perception radius = */ new TI.Real());
			Add(/*collision radius = */ new TI.Real());
			Add(/*arming time = */ new TI.Real());
			Add(/*danger radius = */ new TI.Real());
			Add(/*timer = */ new TI.RealBounds());
			Add(/*minimum velocity = */ new TI.Real());
			Add(/*maximum range = */ new TI.Real());
			Add(/*detonation noise = */ new TI.Enum());
			Add(/*super det. projectile count = */ new TI.ShortInteger());
			Add(/*detonation started = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*detonation effect (airborne) = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*detonation effect (ground) = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*detonation damage = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*attached detonation damage = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*super detonation = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*your momma! = */ new TI.Struct<super_detonation_damage_struct>(this));
			Add(/*detonation sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*damage reporting type = */ new TI.Enum(TI.FieldType.ByteEnum));
			Add(new TI.Pad(3));
			Add(/*super attached detonation damage = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(new TI.UselessPad(40));
			Add(/*material effect radius = */ new TI.Real());
			Add(/*flyby sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*impact effect = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*impact damage = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*boarding detonation time = */ new TI.Real());
			Add(/*boarding detonation damage = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*boarding attached detonation damage = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(new TI.UselessPad(28));
			Add(/*air gravity scale = */ new TI.Real());
			Add(/*air damage range = */ new TI.RealBounds());
			Add(/*water gravity scale = */ new TI.Real());
			Add(/*water damage range = */ new TI.RealBounds());
			Add(/*initial velocity = */ new TI.Real());
			Add(/*final velocity = */ new TI.Real());
			Add(/*blah = */ new TI.Struct<angular_velocity_lower_bound_struct>(this));
			Add(/*guided angular velocity (upper) = */ new TI.Real(TI.FieldType.Angle));
			Add(/*acceleration range = */ new TI.RealBounds());
			Add(new TI.Pad(4));
			Add(/*targeted leading fraction = */ new TI.Real(TI.FieldType.RealFraction));
			Add(new TI.UselessPad(48));
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
		public scenery_group() : base(5)
		{
			Add(/*pathfinding policy = */ new TI.Enum());
			Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*lightmapping policy = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(new TI.UselessPad(120));
		}
		#endregion
	};
	#endregion

	#region sound_scenery
	[TI.TagGroup((int)TagGroups.Enumerated.ssce, 1, /*object_group.ObjectSize +*/ 16, typeof(object_group))]
	public class sound_scenery_group : object_group
	{
		#region Fields
		#endregion

		#region Ctor
		public sound_scenery_group() : base(2)
		{
			Add(new TI.Pad(16));
			Add(new TI.UselessPad(112));
		}
		#endregion
	};
	#endregion

	#region unit_camera_struct
	//unit camera
	[TI.Struct((int)StructGroups.Enumerated.uncs, 1, 32)]
	public class unit_camera_struct : TI.Definition
	{
		#region unit_camera_track_block
		[TI.Definition(1, 16)]
		public class unit_camera_track_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public unit_camera_track_block() : base(2)
			{
				Add(/*track = */ new TI.TagReference(this, TagGroups.trak));
				Add(new TI.UselessPad(12));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public unit_camera_struct() : base(5)
		{
			Add(/*camera marker name = */ new TI.StringId(true));
			Add(/*camera submerged marker name = */ new TI.StringId(true));
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
		public unit_seat_acceleration_struct() : base(3)
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
		public unit_additional_node_names_struct() : base(1)
		{
			Add(/*preferred_gun_node = */ new TI.StringId());
		}
		#endregion
	}
	#endregion

	#region unit_boarding_melee_struct
	[TI.Struct((int)StructGroups.Enumerated.ubms, 2, 80)]
	public class unit_boarding_melee_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public unit_boarding_melee_struct() : base(5)
		{
			Add(/*boarding melee damage = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*boarding melee response = */ new TI.TagReference(this, TagGroups.jpt_));
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
		public unit_boost_struct() : base(5)
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
		public unit_lipsync_scales_struct() : base(2)
		{
			Add(/*attack weight = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*decay weight = */ new TI.Real(TI.FieldType.RealFraction));
		}
		#endregion
	}
	#endregion

	#region unit
	[TI.TagGroup((int)TagGroups.Enumerated.unit, 3, unit_group.UnitSize, typeof(object_group))]
	public class unit_group : object_group
	{
		internal const int UnitSize = 396 /*+ ObjectSize*/;
		internal const int UnitFieldCount = 52;

		#region unit_postures_block
		[TI.Definition(1, 16)]
		public class unit_postures_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public unit_postures_block() : base(3)
			{
				Add(/*name = */ new TI.StringId());
				Add(new TI.UselessPad(24));
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
			public unit_hud_reference_block() : base(3)
			{
				Add(new TI.UselessPad(16));
				Add(/*new unit hud interface = */ new TI.TagReference(this, TagGroups.nhdt));
				Add(new TI.UselessPad(16));
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
			public dialogue_variant_block() : base(4)
			{
				Add(/*variant number = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(new TI.UselessPad(4));
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
			public powered_seat_block() : base(4)
			{
				Add(new TI.UselessPad(4));
				Add(/*driver powerup time = */ new TI.Real());
				Add(/*driver powerdown time = */ new TI.Real());
				Add(new TI.UselessPad(56));
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
			public unit_weapon_block() : base(2)
			{
				Add(/*weapon = */ new TI.TagReference(this, TagGroups.weap));
				Add(new TI.UselessPad(20));
			}
			#endregion
		}
		#endregion

		#region unit_seat_block
		[TI.Definition(4, 192)]
		public class unit_seat_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public unit_seat_block() : base(36)
			{
				Add(/*flags = */ new TI.Flags());
				Add(/*label = */ new TI.StringId(true));
				Add(/*marker name = */ new TI.StringId(true));
				Add(/*entry marker(s) name = */ new TI.StringId());
				Add(/*boarding grenade marker = */ new TI.StringId());
				Add(/*boarding grenade string = */ new TI.StringId());
				Add(/*boarding melee string = */ new TI.StringId());
				Add(/*ping scale = */ new TI.Real(TI.FieldType.RealFraction));
				Add(new TI.UselessPad(8));
				Add(/*turnover time = */ new TI.Real());
				Add(/*acceleration = */ new TI.Struct<unit_seat_acceleration_struct>(this));
				Add(/*AI scariness = */ new TI.Real());
				Add(/*ai seat type = */ new TI.Enum());
				Add(/*boarding seat = */ new TI.BlockIndex()); // 1 unit_seat_block
				Add(/*listener interpolation factor = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*yaw rate bounds = */ new TI.RealBounds());
				Add(/*pitch rate bounds = */ new TI.RealBounds());
				Add(/*min speed reference = */ new TI.Real());
				Add(/*max speed reference = */ new TI.Real());
				Add(/*speed exponent = */ new TI.Real());
				Add(new TI.UselessPad(12));
				Add(/*unit camera = */ new TI.Struct<unit_camera_struct>(this));
				Add(/*unit hud interface = */ new TI.Block<unit_hud_reference_block>(this, 2));
				Add(/*enter seat string = */ new TI.StringId());
				Add(new TI.UselessPad(4));
				Add(/*yaw minimum = */ new TI.Real(TI.FieldType.Angle));
				Add(/*yaw maximum = */ new TI.Real(TI.FieldType.Angle));
				Add(/*built-in gunner = */ new TI.TagReference(this, TagGroups.char_));
				Add(new TI.UselessPad(20));
				Add(/*entry radius = */ new TI.Real());
				Add(/*entry marker cone angle = */ new TI.Real(TI.FieldType.Angle));
				Add(/*entry marker facing angle = */ new TI.Real(TI.FieldType.Angle));
				Add(/*maximum relative velocity = */ new TI.Real());
				Add(new TI.UselessPad(20));
				Add(/*invisible seat region = */ new TI.StringId());
				Add(/*runtime invisible seat region index = */ new TI.LongInteger());
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public unit_group() : this(0) {}
		protected unit_group(int field_count) : base(field_count + UnitFieldCount)
		{
			Add(/*flags = */ new TI.Flags());
			Add(/*default team = */ new TI.Enum());
			Add(/*constant sound volume = */ new TI.Enum());
			Add(new TI.UselessPad(4));
			Add(/*integrated light toggle = */ new TI.TagReference(this, TagGroups.effe));
			Add(new TI.UselessPad(8));
			Add(/*camera field of view = */ new TI.Real(TI.FieldType.Angle));
			Add(/*camera stiffness = */ new TI.Real());
			Add(/*unit camera = */ new TI.Struct<unit_camera_struct>(this));
			Add(/*acceleration = */ new TI.Struct<unit_seat_acceleration_struct>(this));
			Add(new TI.UselessPad(4));
			Add(/*soft ping threshold = */ new TI.Real());
			Add(/*soft ping interrupt time = */ new TI.Real());
			Add(/*hard ping threshold = */ new TI.Real());
			Add(/*hard ping interrupt time = */ new TI.Real());
			Add(/*hard death threshold = */ new TI.Real());
			Add(/*feign death threshold = */ new TI.Real());
			Add(/*feign death time = */ new TI.Real());
			Add(/*distance of evade anim = */ new TI.Real());
			Add(/*distance of dive anim = */ new TI.Real());
			Add(new TI.UselessPad(4));
			Add(/*stunned movement threshold = */ new TI.Real());
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
			Add(new TI.UselessPad(8));
			Add(/*melee damage = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*your momma = */ new TI.Struct<unit_boarding_melee_struct>(this));
			Add(/*motion sensor blip size = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(/*postures = */ new TI.Block<unit_postures_block>(this, 20));
			Add(/*NEW HUD INTERFACES = */ new TI.Block<unit_hud_reference_block>(this, 2));
			Add(/*dialogue variants = */ new TI.Block<dialogue_variant_block>(this, 16));
			Add(/*grenade velocity = */ new TI.Real());
			Add(/*grenade type = */ new TI.Enum());
			Add(/*grenade count = */ new TI.ShortInteger());
			Add(new TI.UselessPad(4));
			Add(/*powered seats = */ new TI.Block<powered_seat_block>(this, 2));
			Add(/*weapons = */ new TI.Block<unit_weapon_block>(this, 4));
			Add(/*seats = */ new TI.Block<unit_seat_block>(this, 32));
			Add(/*boost = */ new TI.Struct<unit_boost_struct>(this));
			Add(/*lipsync = */ new TI.Struct<unit_lipsync_scales_struct>(this));
		}
		#endregion

		/// <summary>
		/// This is a very dangerous method if used incorrectly.
		/// Will set all of this object's field to the FieldValue
		/// returned by <paramref name="other"/>'s fields.
		/// </summary>
		/// <param name="other"></param>
		internal void FromUnit(unit_group other)
		{
			FromObject(other);
			for (int x = ObjectFieldCount; x < object_group.ObjectFieldCount + UnitFieldCount; x++)
				this[x].FieldValue = other[x].FieldValue;
		}
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
		public torque_curve_struct() : base(7)
		{
			Add(/*min torque = */ new TI.Real());
			Add(/*max torque = */ new TI.Real());
			Add(/*peak torque scale = */ new TI.Real());
			Add(/*past peak torque exponent = */ new TI.Real());
			Add(/*torque at max angular velocity = */ new TI.Real());
			Add(/*torque at 2x max angular velocity = */ new TI.Real());
			Add(new TI.UselessPad(8));
		}
		#endregion
	}
	#endregion

	#region havok_vehicle_physics_struct
	//havok vehicle physics
	[TI.Struct((int)StructGroups.Enumerated.HVPH, 1, 96)]
	public class havok_vehicle_physics_struct : TI.Definition
	{
		#region anti_gravity_point_definition_block
		[TI.Definition(1, 76)]
		public class anti_gravity_point_definition_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public anti_gravity_point_definition_block() : base(16)
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
				Add(new TI.Pad(12 + 2 + 2));
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
			public friction_point_definition_block() : base(17)
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
			public vehicle_phantom_shape_block() : base(108)
			{
				Add(new TI.Skip(4));
				Add(/*size = */ new TI.ShortInteger());
				Add(/*count = */ new TI.ShortInteger());
				Add(new TI.Skip(4 + 4));
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
				Add(new TI.Skip(4 + 
					
					4));
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
				Add(new TI.Skip(4 + 
					
					4));
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
				Add(new TI.Skip(4 + 
					
					4));
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
		public havok_vehicle_physics_struct() : base(15)
		{
			Add(/*flags = */ new TI.Flags());
			Add(/*ground friction = */ new TI.Real());
			Add(/*ground depth = */ new TI.Real());
			Add(/*ground damp factor = */ new TI.Real());
			Add(/*ground moving friction = */ new TI.Real());
			Add(/*ground maximum slope 0 = */ new TI.Real());
			Add(/*ground maximum slope 1 = */ new TI.Real());
			Add(new TI.Pad(16));
			Add(/*anti_gravity_bank_lift = */ new TI.Real());
			Add(/*steering_bank_reaction_scale = */ new TI.Real());
			Add(/*gravity scale = */ new TI.Real());
			Add(/*radius = */ new TI.Real());
			Add(/*anti gravity points = */ new TI.Block<anti_gravity_point_definition_block>(this, 16));
			Add(/*friction points = */ new TI.Block<friction_point_definition_block>(this, 16));
			Add(/*shape phantom shape = */ new TI.Block<vehicle_phantom_shape_block>(this, 1));
		}
		#endregion
	}
	#endregion

	#region vehicle
	[TI.TagGroup((int)TagGroups.Enumerated.vehi, 1, /*unit_group.UnitSize +*/ 332, typeof(unit_group))]
	public class vehicle_group : unit_group
	{
		#region gear_block
		[TI.Definition(1, 68)]
		public class gear_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public gear_block() : base(8)
			{
				Add(/*loaded torque curve = */ new TI.Struct<torque_curve_struct>(this));
				Add(/*cruising torque curve = */ new TI.Struct<torque_curve_struct>(this));
				Add(/*min time to upshift = */ new TI.Real());
				Add(/*engine up-shift scale = */ new TI.Real());
				Add(/*gear ratio = */ new TI.Real());
				Add(/*min time to downshift = */ new TI.Real());
				Add(/*engine down-shift scale = */ new TI.Real());
				Add(new TI.UselessPad(12));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public vehicle_group() : base(48)
		{
			Add(/*flags = */ new TI.Flags());
			Add(/*type = */ new TI.Enum());
			Add(/*control = */ new TI.Enum());
			Add(/*maximum forward speed = */ new TI.Real());
			Add(/*maximum reverse speed = */ new TI.Real());
			Add(/*speed acceleration = */ new TI.Real());
			Add(/*speed deceleration = */ new TI.Real());
			Add(/*maximum left turn = */ new TI.Real());
			Add(/*maximum right turn (negative) = */ new TI.Real());
			Add(/*wheel circumference = */ new TI.Real());
			Add(/*turn rate = */ new TI.Real());
			Add(/*blur speed = */ new TI.Real());
			Add(/*specific type = */ new TI.Enum());
			Add(/*player training vehicle type = */ new TI.Enum());
			Add(/*flip message = */ new TI.StringId());
			Add(/*turn scale = */ new TI.Real());
			Add(/*speed turn penalty power (0.5 .. 2) = */ new TI.Real());
			Add(/*speed turn penalty (0 = none, 1 = can't turn at top speed) = */ new TI.Real());
			Add(/*maximum left slide = */ new TI.Real());
			Add(/*maximum right slide = */ new TI.Real());
			Add(/*slide acceleration = */ new TI.Real());
			Add(/*slide deceleration = */ new TI.Real());
			Add(/*minimum flipping angular velocity = */ new TI.Real());
			Add(/*maximum flipping angular velocity = */ new TI.Real());
			Add(/*vehicle size = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(new TI.UselessPad(20));
			Add(/*fixed gun yaw = */ new TI.Real());
			Add(/*fixed gun pitch = */ new TI.Real());
			Add(/*overdampen cusp angle = */ new TI.Real());
			Add(/*overdampen exponent = */ new TI.Real());
			Add(/*crouch transition time = */ new TI.Real());
			Add(new TI.Pad(4));
			Add(/*engine moment = */ new TI.Real());
			Add(/*engine max angular velocity = */ new TI.Real());
			Add(/*gears = */ new TI.Block<gear_block>(this, 16));
			Add(/*flying torque scale = */ new TI.Real());
			Add(/*seat enterance acceleration scale = */ new TI.Real());
			Add(/*seat exit accelersation scale = */ new TI.Real());
			Add(new TI.UselessPad(16));
			Add(/*air friction deceleration = */ new TI.Real());
			Add(/*thrust scale = */ new TI.Real());
			Add(/*suspension sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*crash sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*UNUSED = */ new TI.TagReference(this, TagGroups.foot));
			Add(/*special effect = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*unused effect = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*havok vehicle physics = */ new TI.Struct<havok_vehicle_physics_struct>(this));
		}
		#endregion
	};
	#endregion

	#region vehicle_collection
	[TI.TagGroup((int)TagGroups.Enumerated.vehc, 0, 16)]
	public class vehicle_collection_group : TI.Definition
	{
		#region vehicle_permutation
		[TI.Definition(1, 24)]
		public class vehicle_permutation : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public vehicle_permutation() : base(3)
			{
				Add(/*weight = */ new TI.Real());
				Add(/*vehicle = */ new TI.TagReference(this, TagGroups.vehi));
				Add(/*variant name = */ new TI.StringId());
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public vehicle_collection_group() : base(3)
		{
			Add(/*vehicle permutations = */ new TI.Block<vehicle_permutation>(this, 32));
			Add(/*spawn time (in seconds, 0 = default) = */ new TI.ShortInteger());
			Add(new TI.Pad(2));
		}
		#endregion
	};
	#endregion

	#region melee_aim_assist_struct
	[TI.Struct((int)StructGroups.Enumerated.masd_melee, 2, 20)]
	public class melee_aim_assist_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public melee_aim_assist_struct() : base(7)
		{
			Add(/*magnetism angle = */ new TI.Real(TI.FieldType.Angle));
			Add(/*magnetism range = */ new TI.Real());
			Add(new TI.UselessPad(8));
			Add(/*throttle magnitude = */ new TI.Real());
			Add(/*throttle minimum distance = */ new TI.Real());
			Add(/*throttle maximum adjustment angle = */ new TI.Real(TI.FieldType.Angle));
			Add(new TI.UselessPad(4));
		}
		#endregion
	}
	#endregion

	#region melee_damage_parameters_struct
	[TI.Struct((int)StructGroups.Enumerated.mdps, 2, 140)]
	public class melee_damage_parameters_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public melee_damage_parameters_struct() : base(10)
		{
			Add(/*damage pyramid angles = */ new TI.RealEulerAngles2D());
			Add(/*damage pyramid depth = */ new TI.Real());
			Add(/*1st hit melee damage = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*1st hit melee response = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*2nd hit melee damage = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*2nd hit melee response = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*3rd hit melee damage = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*3rd hit melee response = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*lunge melee damage = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*lunge melee response = */ new TI.TagReference(this, TagGroups.jpt_));
		}
		#endregion
	}
	#endregion

	#region aim_assist_struct
	[TI.Struct((int)StructGroups.Enumerated.easd, 1, 36)]
	public class aim_assist_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public aim_assist_struct() : base(6)
		{
			Add(/*autoaim angle = */ new TI.Real(TI.FieldType.Angle));
			Add(/*autoaim range = */ new TI.Real());
			Add(/*magnetism angle = */ new TI.Real(TI.FieldType.Angle));
			Add(/*magnetism range = */ new TI.Real());
			Add(/*deviation angle = */ new TI.Real(TI.FieldType.Angle));
			Add(new TI.Pad(4 + 12));
		}
		#endregion
	}
	#endregion

	#region weapon_tracking_struct
	[TI.Struct((int)StructGroups.Enumerated.wtsf, 2, 4)]
	public class weapon_tracking_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public weapon_tracking_struct() : base(2)
		{
			Add(/*tracking type = */ new TI.Enum());
			Add(new TI.Pad(2));
		}
		#endregion
	}
	#endregion

	#region weapon_shared_interface_struct
	[TI.Struct((int)StructGroups.Enumerated.wSiS, 1, 16)]
	public class weapon_shared_interface_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public weapon_shared_interface_struct() : base(1)
		{
			Add(new TI.Pad(16));
		}
		#endregion
	}
	#endregion

	#region weapon_interface_struct
	[TI.Struct((int)StructGroups.Enumerated.wItS, 1, 44)]
	public class weapon_interface_struct : TI.Definition
	{
		#region weapon_first_person_interface_block
		[TI.Definition(1, 32)]
		public class weapon_first_person_interface_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public weapon_first_person_interface_block() : base(2)
			{
				Add(/*first person model = */ new TI.TagReference(this, TagGroups.mode));
				Add(/*first person animations = */ new TI.TagReference(this, TagGroups.jmad));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public weapon_interface_struct() : base(3)
		{
			Add(/*shared interface = */ new TI.Struct<weapon_shared_interface_struct>(this));
			Add(/*first person = */ new TI.Block<weapon_first_person_interface_block>(this, 4));
			Add(/*new hud interface = */ new TI.TagReference(this, TagGroups.nhdt));
		}
		#endregion
	}
	#endregion

	#region weapon_trigger_autofire_struct
	[TI.Struct((int)StructGroups.Enumerated.wtas, 1, 12)]
	public class weapon_trigger_autofire_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public weapon_trigger_autofire_struct() : base(4)
		{
			Add(/*autofire time = */ new TI.Real());
			Add(/*autofire throw = */ new TI.Real());
			Add(/*secondary action = */ new TI.Enum());
			Add(/*primary action = */ new TI.Enum());
		}
		#endregion
	}
	#endregion

	#region weapon_trigger_charging_struct
	[TI.Struct((int)StructGroups.Enumerated.wtcs, 1, 52)]
	public class weapon_trigger_charging_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public weapon_trigger_charging_struct() : base(8)
		{
			Add(/*charging time = */ new TI.Real());
			Add(/*charged time = */ new TI.Real());
			Add(/*overcharged action = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(/*charged illumination = */ new TI.Real());
			Add(/*spew time = */ new TI.Real());
			Add(/*charging effect = */ new TI.TagReference(this)); // snd!,effe,
			Add(/*charging damage effect = */ new TI.TagReference(this, TagGroups.jpt_));
		}
		#endregion
	}
	#endregion

	#region weapon_barrel_damage_effect_struct
	[TI.Struct((int)StructGroups.Enumerated.wbde, 2, 16)]
	public class weapon_barrel_damage_effect_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public weapon_barrel_damage_effect_struct() : base(1)
		{
			Add(/*damage effect = */ new TI.TagReference(this, TagGroups.jpt_));
		}
		#endregion
	}
	#endregion

	#region weapon
	[TI.TagGroup((int)TagGroups.Enumerated.weap, 2, 4, /*item_group.ItemSize +*/ 716, typeof(item_group))]
	public class weapon_group : item_group
	{
		#region magazines
		[TI.Definition(2, 128)]
		public class magazines : TI.Definition
		{
			#region magazine_objects
			[TI.Definition(2, 20)]
			public class magazine_objects : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public magazine_objects() : base(3)
				{
					Add(/*rounds = */ new TI.ShortInteger());
					Add(new TI.Pad(2));
					Add(/*equipment = */ new TI.TagReference(this, TagGroups.eqip));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public magazines() : base(18)
			{
				Add(/*flags = */ new TI.Flags());
				Add(/*rounds recharged = */ new TI.ShortInteger());
				Add(/*rounds total initial = */ new TI.ShortInteger());
				Add(/*rounds total maximum = */ new TI.ShortInteger());
				Add(/*rounds loaded maximum = */ new TI.ShortInteger());
				Add(new TI.Pad(4));
				Add(new TI.UselessPad(4));
				Add(/*reload time = */ new TI.Real());
				Add(/*rounds reloaded = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(/*chamber time = */ new TI.Real());
				Add(new TI.Pad(8 + 16));
				Add(/*reloading effect = */ new TI.TagReference(this)); // snd!,effe,
				Add(/*reloading damage effect = */ new TI.TagReference(this, TagGroups.jpt_));
				Add(/*chambering effect = */ new TI.TagReference(this)); // snd!,effe,
				Add(/*chambering damage effect = */ new TI.TagReference(this, TagGroups.jpt_));
				Add(new TI.UselessPad(12));
				Add(/*magazines = */ new TI.Block<magazine_objects>(this, 8));
			}
			#endregion
		}
		#endregion

		#region weapon_triggers
		[TI.Definition(1, 80)]
		public class weapon_triggers : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public weapon_triggers() : base(10)
			{
				Add(/*flags = */ new TI.Flags());
				Add(/*input = */ new TI.Enum());
				Add(/*behavior = */ new TI.Enum());
				Add(/*primary barrel = */ new TI.BlockIndex()); // 1 weapon_barrels
				Add(/*secondary barrel = */ new TI.BlockIndex()); // 1 weapon_barrels
				Add(/*prediction = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(new TI.UselessPad(28));
				Add(/*autofire = */ new TI.Struct<weapon_trigger_autofire_struct>(this));
				Add(/*charging = */ new TI.Struct<weapon_trigger_charging_struct>(this));
			}
			#endregion
		}
		#endregion

		// triggers_block in Halo1
		#region weapon_barrels
		[TI.Definition(3, 256)]
		public class weapon_barrels : TI.Definition
		{
			// trigger_firing_effect_block in Halo1
			#region barrel_firing_effect_block
			[TI.Definition(2, 100)]
			public class barrel_firing_effect_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public barrel_firing_effect_block() : base(7)
				{
					Add(/*shot count = */ new TI.ShortIntegerBounds());
					Add(/*firing effect = */ new TI.TagReference(this)); // snd!,effe,
					Add(/*misfire effect = */ new TI.TagReference(this)); // snd!,effe,
					Add(/*empty effect = */ new TI.TagReference(this)); // snd!,effe,
					Add(/*firing damage = */ new TI.TagReference(this, TagGroups.jpt_));
					Add(/*misfire damage = */ new TI.TagReference(this, TagGroups.jpt_));
					Add(/*empty damage = */ new TI.TagReference(this, TagGroups.jpt_));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public weapon_barrels() : base(46)
			{
				Add(/*flags = */ new TI.Flags());
				Add(/*rounds per second = */ new TI.RealBounds());
				Add(/*acceleration time = */ new TI.Real());
				Add(/*deceleration time = */ new TI.Real());
				Add(/*barrel spin scale = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*blurred rate of fire = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*shots per fire = */ new TI.ShortIntegerBounds());
				Add(/*fire recovery time = */ new TI.Real());
				Add(/*soft recovery fraction = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*magazine = */ new TI.BlockIndex()); // 1 magazines
				Add(/*rounds per shot = */ new TI.ShortInteger());
				Add(/*minimum rounds loaded = */ new TI.ShortInteger());
				Add(/*rounds between tracers = */ new TI.ShortInteger());
				Add(/*optional barrel marker name = */ new TI.StringId());
				Add(/*prediction type = */ new TI.Enum());
				Add(/*firing noise = */ new TI.Enum());
				Add(/*acceleration time = */ new TI.Real());
				Add(/*deceleration time = */ new TI.Real());
				Add(/*damage error = */ new TI.RealBounds());
				Add(/*acceleration time = */ new TI.Real());
				Add(/*deceleration time = */ new TI.Real());
				Add(new TI.Pad(8));
				Add(/*minimum error = */ new TI.Real(TI.FieldType.Angle));
				Add(/*error angle = */ new TI.RealBounds(TI.FieldType.AngleBounds));
				Add(/*dual wield damage scale = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*distribution function = */ new TI.Enum());
				Add(/*projectiles per shot = */ new TI.ShortInteger());
				Add(/*distribution angle = */ new TI.Real());
				Add(/*minimum error = */ new TI.Real(TI.FieldType.Angle));
				Add(/*error angle = */ new TI.RealBounds(TI.FieldType.AngleBounds));
				Add(/*first person offset = */ new TI.RealPoint3D());
				Add(/*damage effect reporting type = */ new TI.Enum(TI.FieldType.ByteEnum));
				Add(new TI.Pad(3));
				Add(/*projectile = */ new TI.TagReference(this, TagGroups.proj));
				Add(/*eh = */ new TI.Struct<weapon_barrel_damage_effect_struct>(this));
				Add(/*ejection port recovery time = */ new TI.Real());
				Add(/*illumination recovery time = */ new TI.Real());
				Add(/*heat generated per round = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*age generated per round = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*overload time = */ new TI.Real());
				Add(/*angle change per shot = */ new TI.RealBounds(TI.FieldType.AngleBounds));
				Add(/*acceleration time = */ new TI.Real());
				Add(/*deceleration time = */ new TI.Real());
				Add(/*angle change function = */ new TI.Enum());
				Add(new TI.Pad(2 + 8 + 24));
				Add(/*firing effects = */ new TI.Block<barrel_firing_effect_block>(this, 3));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public weapon_group() : base(69)
		{
			Add(/*flags = */ new TI.Flags());
			Add(/* = */ new TI.StringId(true));
			Add(/*secondary trigger mode = */ new TI.Enum());
			Add(/*maximum alternate shots loaded = */ new TI.ShortInteger());
			Add(/*turn on time = */ new TI.Real());
			Add(/*ready time = */ new TI.Real());
			Add(/*ready effect = */ new TI.TagReference(this)); // snd!,effe,
			Add(/*ready damage effect = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*heat recovery threshold = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*overheated threshold = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*heat detonation threshold = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*heat detonation fraction = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*heat loss per second = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*heat illumination = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*overheated heat loss per second = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*overheated = */ new TI.TagReference(this)); // snd!,effe,
			Add(/*overheated damage effect = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*detonation = */ new TI.TagReference(this)); // snd!,effe,
			Add(/*detonation damage effect = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*player melee damage = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*player melee response = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*melee aim assist = */ new TI.Struct<melee_aim_assist_struct>(this));
			Add(/*melee damage parameters = */ new TI.Struct<melee_damage_parameters_struct>(this));
			Add(/*melee damage reporting type = */ new TI.Enum(TI.FieldType.ByteEnum));
			Add(new TI.Pad(1));
			Add(/*magnification levels = */ new TI.ShortInteger());
			Add(/*magnification range = */ new TI.RealBounds());
			Add(/*weapon aim assist = */ new TI.Struct<aim_assist_struct>(this));
			Add(/*movement penalized = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(/*forward movement penalty = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*sideways movement penalty = */ new TI.Real(TI.FieldType.RealFraction));
			Add(new TI.UselessPad(4));
			Add(/*AI scariness = */ new TI.Real());
			Add(/*weapon power-on time = */ new TI.Real());
			Add(/*weapon power-off time = */ new TI.Real());
			Add(/*weapon power-on effect = */ new TI.TagReference(this)); // snd!,effe,
			Add(/*weapon power-off effect = */ new TI.TagReference(this)); // snd!,effe,
			Add(/*age heat recovery penalty = */ new TI.Real());
			Add(/*age rate of fire penalty = */ new TI.Real());
			Add(/*age misfire start = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*age misfire chance = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*pickup sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*zoom-in sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*zoom-out sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*active camo ding = */ new TI.Real());
			Add(/*active camo regrowth rate = */ new TI.Real());
			Add(/*handle node = */ new TI.StringId());
			Add(/*weapon class = */ new TI.StringId());
			Add(/*weapon name = */ new TI.StringId());
			Add(/*multiplayer weapon type = */ new TI.Enum());
			Add(/*weapon type = */ new TI.Enum());
			Add(/*tracking = */ new TI.Struct<weapon_tracking_struct>(this));
			Add(/*player interface = */ new TI.Struct<weapon_interface_struct>(this));
			Add(/*predicted resources = */ new TI.Block<predicted_resource_block>(this, 2048));
			Add(/*magazines = */ new TI.Block<magazines>(this, 2));
			Add(/*new triggers = */ new TI.Block<weapon_triggers>(this, 2));
			Add(/*barrels = */ new TI.Block<weapon_barrels>(this, 2));
			Add(new TI.Pad(8));
			Add(new TI.UselessPad(16));
			Add(/*max movement acceleration = */ new TI.Real());
			Add(/*max movement velocity = */ new TI.Real());
			Add(/*max turning acceleration = */ new TI.Real());
			Add(/*max turning velocity = */ new TI.Real());
			Add(/*deployed vehicle = */ new TI.TagReference(this, TagGroups.vehi));
			Add(/*age effect = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*aged weapon = */ new TI.TagReference(this, TagGroups.weap));
			Add(/*first person weapon offset = */ new TI.RealVector3D());
			Add(/*first person scope size = */ new TI.RealVector2D());
		}
		#endregion
	};
	#endregion
}