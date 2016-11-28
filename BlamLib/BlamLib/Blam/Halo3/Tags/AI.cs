/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo3.Tags
{
	#region character
	[TI.TagGroup((int)TagGroups.Enumerated.char_, 2, 468)]
	public class character_group : TI.Definition
	{
		#region character_general_block
		[TI.Definition(2, 28)]
		public class character_general_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public character_general_block()
			{
				Add(/*general flags = */ new TI.Flags());
				Add(/*type = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*scariness = */ new TI.Real());
				Add(new TI.Pad(16));
			}
			#endregion
		}
		#endregion

		#region character_vitality_block
		[TI.Definition(2, 128)]
		public class character_vitality_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public character_vitality_block()
			{
				Add(/*vitality flags = */ new TI.Flags());
				Add(/*normal body vitality = */ new TI.Real());
				Add(/*normal shield vitality = */ new TI.Real());
				Add(/*legendary body vitality = */ new TI.Real());
				Add(/*legendary shield vitality = */ new TI.Real());
				Add(/*body recharge fraction = */ new TI.Real());
				Add(/*soft ping threshold (with shields) = */ new TI.Real());
				Add(/*soft ping threshold (no shields) = */ new TI.Real());
				Add(/*soft ping min interrupt time = */ new TI.Real());
				Add(/*hard ping threshold (with shields) = */ new TI.Real());
				Add(/*hard ping threshold (no shields) = */ new TI.Real());
				Add(/*hard ping min interrupt time = */ new TI.Real());
				Add(/*current damage decay delay = */ new TI.Real());
				Add(/*current damage decay time = */ new TI.Real());
				Add(/*recent damage decay delay = */ new TI.Real());
				Add(/*recent damage decay time = */ new TI.Real());
				Add(/*body recharge delay time = */ new TI.Real());
				Add(/*body recharge time = */ new TI.Real());
				Add(/*shield recharge delay time = */ new TI.Real());
				Add(/*shield recharge time = */ new TI.Real());
				Add(/*stun threshold = */ new TI.Real());
				Add(/*stun time bounds = */ new TI.RealBounds());
				Add(/*extended shield damage threshold = */ new TI.Real());
				Add(/*extended body damage threshold = */ new TI.Real());
				Add(/*suicide radius = */ new TI.Real());
				Add(new TI.Skip(8));

				// TODO: unknown
				Add(/*? = */ new TI.Real());
				Add(/*? = */ new TI.TagReference(this));
			}
			#endregion
		}
		#endregion

		#region character_placement_block
		[TI.Definition(1, 52)]
		public class character_placement_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public character_placement_block()
			{
				Add(new TI.Pad(4));
				Add(/*few upgrade chance (easy) = */ new TI.Real());
				Add(/*few upgrade chance (normal) = */ new TI.Real());
				Add(/*few upgrade chance (heroic) = */ new TI.Real());
				Add(/*few upgrade chance (legendary) = */ new TI.Real());
				Add(/*normal upgrade chance (easy) = */ new TI.Real());
				Add(/*normal upgrade chance (normal) = */ new TI.Real());
				Add(/*normal upgrade chance (heroic) = */ new TI.Real());
				Add(/*normal upgrade chance (legendary) = */ new TI.Real());
				Add(/*many upgrade chance (easy) = */ new TI.Real());
				Add(/*many upgrade chance (normal) = */ new TI.Real());
				Add(/*many upgrade chance (heroic) = */ new TI.Real());
				Add(/*many upgrade chance (legendary) = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region character_perception_block
		[TI.Definition(2, 44)]
		public class character_perception_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public character_perception_block()
			{
				Add(/*perception flags = */ new TI.Flags());
				Add(/*max vision distance = */ new TI.Real());
				Add(/*central vision angle = */ new TI.Real(TI.FieldType.Angle));
				Add(/*max vision angle = */ new TI.Real(TI.FieldType.Angle));
				Add(/*peripheral vision angle = */ new TI.Real(TI.FieldType.Angle));
				Add(/*peripheral distance = */ new TI.Real());
				Add(/*hearing distance = */ new TI.Real());
				Add(/*notice projectile chance = */ new TI.Real());
				// TODO: unknown
				Add(/*? = */ new TI.Real());
				Add(/*first ack. surprise distance = */ new TI.Real());
			}
			#endregion
		}
		#endregion

		#region character_look_block
		[TI.Definition(1, 80)]
		public class character_look_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public character_look_block()
			{
				Add(/*maximum aiming deviation = */ new TI.RealEulerAngles2D());
				Add(/*maximum looking deviation = */ new TI.RealEulerAngles2D());
				Add(new TI.Pad(16));
				Add(/*noncombat look delta L = */ new TI.Real(TI.FieldType.Angle));
				Add(/*noncombat look delta R = */ new TI.Real(TI.FieldType.Angle));
				Add(/*combat look delta L = */ new TI.Real(TI.FieldType.Angle));
				Add(/*combat look delta R = */ new TI.Real(TI.FieldType.Angle));
				Add(/*noncombat idle looking = */ new TI.RealBounds());
				Add(/*noncombat idle aiming = */ new TI.RealBounds());
				Add(/*combat idle looking = */ new TI.RealBounds());
				Add(/*combat idle aiming = */ new TI.RealBounds());
			}
			#endregion
		}
		#endregion

		#region character_movement_block
		[TI.Definition(2, 44)]
		public class character_movement_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public character_movement_block()
			{
				Add(/*movement flags = */ new TI.Flags());
				Add(/*pathfinding radius = */ new TI.Real());
				Add(/*destination radius = */ new TI.Real());
				Add(/*dive grenade chance = */ new TI.Real());
				Add(/*obstacle leap min size = */ new TI.Enum());
				Add(/*obstacle leap max size = */ new TI.Enum());
				Add(/*obstacle ignore size = */ new TI.Enum());
				Add(/*obstacle smashable size = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*jump height = */ new TI.Enum());
				Add(/*movement hints = */ new TI.Flags());
				Add(/*throttle scale = */ new TI.Real());

				// TODO: unknown
				Add(/*? = */new TI.Real());
				Add(/*? = */new TI.Real());
			}
			#endregion
		}
		#endregion

		#region character_ready_block
		[TI.Definition(1, 8)]
		public class character_ready_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public character_ready_block()
			{
				Add(/*ready time bounds = */ new TI.RealBounds());
			}
			#endregion
		}
		#endregion

		#region character_engage_block
		[TI.Definition(2, 40)]
		public class character_engage_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public character_engage_block()
			{
				Add(/*flags = */ new TI.Flags());
				// TODO: unknown
				Add(/*? = */ new TI.Real());
				Add(/*? = */ new TI.Real());
				Add(/*Crouch danger threshold = */ new TI.Real());
				Add(/*Stand danger threshold = */ new TI.Real());
				Add(/*Fight danger move threshold = */ new TI.Real());
				Add(/*? = */ new TI.Real());
				Add(/*? = */ new TI.TagReference(this));
			}
			#endregion
		}
		#endregion

		#region character_charge_block
		[TI.Definition(5, 124)]
		public class character_charge_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public character_charge_block()
			{
				Add(/*Charge flags = */ new TI.Flags());
				Add(/*melee consider range = */ new TI.Real());
				Add(/*melee chance = */ new TI.Real());
				Add(/*melee attack range = */ new TI.Real());
				Add(/*melee abort range = */ new TI.Real());
				Add(/*melee attack timeout = */ new TI.Real());
				Add(/*melee attack delay timer = */ new TI.Real());
				Add(/*melee leap range = */ new TI.RealBounds());
				Add(/*melee leap chance = */ new TI.Real());
				Add(/*ideal leap velocity = */ new TI.Real());
				Add(/*max leap velocity = */ new TI.Real());
				Add(/*melee leap ballistic = */ new TI.Real());
				Add(/*melee delay timer = */ new TI.Real());
				// TODO: unknown
				Add(new TI.UnknownPad(20));
				Add(/*berserk weapon = */ new TI.TagReference(this, TagGroups.weap));
				Add(new TI.UnknownPad(32));
			}
			#endregion
		}
		#endregion

		#region Fields
		public TI.Flags Flags;
		public TI.TagReference ParentCharacter;
		public TI.TagReference Unit;
		public TI.TagReference Creature;
		public TI.TagReference Style;
		public TI.TagReference MajorCharacter;

		public TI.Block<character_general_block> GeneralProperties;
		public TI.Block<character_vitality_block> VitalityProperties;
		public TI.Block<character_placement_block> PlacementProperties;
		public TI.Block<character_perception_block> PerceptionProperties;
		public TI.Block<character_look_block> LookProperties;
		public TI.Block<character_movement_block> MovementProperties;
		public TI.Block<character_ready_block> ReadyProperties;
		public TI.Block<character_engage_block> EngageProperties;
		#endregion

		#region Ctor
		public character_group()
		{
			Add(Flags = new TI.Flags());
			Add(ParentCharacter = new TI.TagReference(this, TagGroups.char_));
			Add(Unit = new TI.TagReference(this, TagGroups.unit));
			Add(Creature = new TI.TagReference(this, TagGroups.crea));
			Add(Style = new TI.TagReference(this, TagGroups.styl));
			Add(MajorCharacter = new TI.TagReference(this, TagGroups.char_));
			Add(TI.Pad.BlockHalo3);// tag block [0x?]
			Add(TI.Pad.BlockHalo3);// tag block [0x?] variants?
			Add(GeneralProperties = new TI.Block<character_general_block>(this, 1));
			Add(VitalityProperties = new TI.Block<character_vitality_block>(this, 1));
			Add(PlacementProperties = new TI.Block<character_placement_block>(this, 1));
			Add(PerceptionProperties = new TI.Block<character_perception_block>(this, 4));
			Add(LookProperties = new TI.Block<character_look_block>(this, 1));
			Add(MovementProperties = new TI.Block<character_movement_block>(this, 1));
			Add(ReadyProperties = new TI.Block<character_ready_block>(this, 3));
			Add(EngageProperties = new TI.Block<character_engage_block>(this, 3));
			Add(/*charge properties = */ new TI.Block<character_charge_block>(this, 3));

			// 0xF0
		}
		#endregion
	};
	#endregion

	#region style
	[TI.TagGroup((int)TagGroups.Enumerated.styl, 2, 92)]
	public class style_group : TI.Definition
	{
		#region special_movement_block
		[TI.Definition(1, 4)]
		public class special_movement_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public special_movement_block()
			{
				Add(/*Special movement 1 = */ new TI.Flags());
			}
			#endregion
		}
		#endregion

		#region behavior_names_block
		[TI.Definition(1, 32)]
		public class behavior_names_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public behavior_names_block()
			{
				Add(/*behavior name = */ new TI.String());
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public style_group()
		{
			Add(/*name = */ new TI.String());
			Add(new TI.Skip(8)); // enums
// 			Add(/*Combat status decay options = */ new TI.Enum());
// 			Add(new TI.Pad(2));
// 			Add(/*Attitude = */ new TI.Enum());
// 			Add(new TI.Pad(2));
// 			Add(/*engage attitude = */ new TI.Enum(TI.FieldType.ByteEnum));
// 			Add(/*evasion attitude = */ new TI.Enum(TI.FieldType.ByteEnum));
// 			Add(/*cover attitude = */ new TI.Enum(TI.FieldType.ByteEnum));
// 			Add(/*search attitude = */ new TI.Enum(TI.FieldType.ByteEnum));
// 			Add(/*presearch attitude = */ new TI.Enum(TI.FieldType.ByteEnum));
// 			Add(/*retreat attitude = */ new TI.Enum(TI.FieldType.ByteEnum));
// 			Add(/*charge attitude = */ new TI.Enum(TI.FieldType.ByteEnum));
// 			Add(/*ready attitude = */ new TI.Enum(TI.FieldType.ByteEnum));
// 			Add(/*idle attitude = */ new TI.Enum(TI.FieldType.ByteEnum));
// 			Add(/*weapon attitude = */ new TI.Enum(TI.FieldType.ByteEnum));
// 			Add(/*swarm attitude = */ new TI.Enum(TI.FieldType.ByteEnum));
// 			Add(new TI.Pad(1));

			Add(/*Style control = */ new TI.Flags());
			Add(/*Behaviors1 = */ new TI.Flags());
			Add(/*Behaviors2 = */ new TI.Flags());
			Add(/*Behaviors3 = */ new TI.Flags());
			Add(/*Behaviors4 = */ new TI.Flags());
			Add(/*Behaviors5 = */ new TI.Flags());
			Add(/*Behaviors6 = */ new TI.Flags());
			Add(/*Special movement = */ new TI.Block<special_movement_block>(this, 1));
			Add(/*Behavior list = */ new TI.Block<behavior_names_block>(this, 160));
		}
		#endregion
	};
	#endregion
}