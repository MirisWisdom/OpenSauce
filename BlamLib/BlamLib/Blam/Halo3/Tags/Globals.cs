/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo3.Tags
{
	[TI.Definition]
	public sealed class field_block<FieldType> : TI.field_block<FieldType> where FieldType : TI.Field, new() { };

	// TODO: ?????
	[TagInterface.Definition(1, 8)]
	public class predicted_resource_block : Cache.predicted_resource_block
	{
		public predicted_resource_block() : base(3)
		{
			Add(Type = new TI.Enum());
			Add(ResourceIndex = new TI.ShortInteger());
			Add(TagIndex = new TI.LongInteger());
		}
	};

	#region byte_block
	[TI.Definition(1, 1)]
	public class byte_block : TI.Definition
	{
		public byte_block() : base(1)
		{
			Add(/*Value = */ new TI.ByteInteger());
		}
	}
	#endregion

	#region global_geometry_material_block
	[TI.Definition(2, 36)]
	public class global_geometry_material_block : TI.Definition
	{
		#region global_geometry_material_property_block
		[TI.Definition(2, 12)]
		public class global_geometry_material_property_block : TI.Definition
		{
			#region Fields
			public TI.Enum Type;
			public TI.ShortInteger IntValue;
			public TI.Real RealValue;
			#endregion

			#region Ctor
			public global_geometry_material_property_block()
			{
				Add(Type = new TI.Enum());
				Add(IntValue = new TI.ShortInteger());
				Add(/*? = */ new TI.Skip(4)); // TODO
				Add(RealValue = new TI.Real());
			}
			#endregion
		}
		#endregion

		#region Fields
		public TI.TagReference Shader;
		public TI.Block<global_geometry_material_property_block> Properties;
		public TI.ByteInteger BreakableSurfaceIndex;
		#endregion

		#region Ctor
		public global_geometry_material_block()
		{
			Add(Shader = new TI.TagReference(this, TagGroups.rm__));
			Add(Properties = new TI.Block<global_geometry_material_property_block>(this, 16));
			Add(new TI.Pad(4)); // used for self indexing
			Add(BreakableSurfaceIndex = new TI.ByteInteger());
			Add(new TI.Pad(3));
		}
		#endregion
	}
	#endregion

	#region global_geometry_compression_info_block
	[TI.Definition(2, 44)]
	public class global_geometry_compression_info_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public global_geometry_compression_info_block()
		{
			// TODO
			Add(/*? = */ new TI.ShortInteger());
			Add(/*? = */ new TI.ShortInteger());
			Add(/*Position Bounds x = */ new TI.RealBounds());
			Add(/*Position Bounds y = */ new TI.RealBounds());
			Add(/*Position Bounds z = */ new TI.RealBounds());
			Add(/*Texcoord Bounds x = */ new TI.RealBounds());
			Add(/*Texcoord Bounds y = */ new TI.RealBounds());
		}
		#endregion
	}
	#endregion


	#region multiplayer_color_block
	[TI.Definition(1, 12)]
	public class multiplayer_color_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public multiplayer_color_block()
		{
			Add(/*color = */ new TI.RealColor());
		}
		#endregion
	}
	#endregion

	#region material_physics_properties_struct
	//physics properties
	[TI.Struct((int)StructGroups.Enumerated.mphp, 2, 16)]
	public class material_physics_properties_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public material_physics_properties_struct()
		{
			Add(new TI.Pad(4));
			Add(/*friction = */ new TI.Real());
			Add(/*restitution = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*density = */ new TI.Real());
		}
		#endregion
	}
	#endregion

	#region materials_sweeteners_struct
	//sweet, sweetening, sweeteners!
	[TI.Struct((int)StructGroups.Enumerated.msst, 2, 292)]
	public class materials_sweeteners_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public materials_sweeteners_struct()
		{
			Add(/* = */ new TI.TagReference(this));
			Add(/*sound sweetener (small) = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*sound sweetener (medium) = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*sound sweetener (large) = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*sound sweetener rolling = */ new TI.TagReference(this, TagGroups.lsnd));
			Add(/*sound sweetener grinding = */ new TI.TagReference(this, TagGroups.lsnd));
			Add(/*sound sweetener (melee) = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/* = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/* = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/* = */ new TI.TagReference(this));
			Add(/*effect sweetener (small) = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*effect sweetener (medium) = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*effect sweetener (large) = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*effect sweetener rolling = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*effect sweetener grinding = */ new TI.TagReference(this, TagGroups.effe));
			Add(/*effect sweetener (melee) = */ new TI.TagReference(this, TagGroups.effe));
			Add(/* = */ new TI.TagReference(this));
			Add(/* = */ new TI.TagReference(this));
			Add(/*sweetener inheritance flags = */ new TI.Flags());
		}
		#endregion
	}
	#endregion

	#region sounds_block
	[TI.Definition(1, 16)]
	public class sounds_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public sounds_block()
		{
			Add(/*sound = */ new TI.TagReference(this, TagGroups.snd_));
		}
		#endregion
	}
	#endregion

	#region sound_loopings_block
	[TI.Definition(1, 16)]
	public class sound_loopings_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public sound_loopings_block()
		{
			Add(/*sound = */ new TI.TagReference(this, TagGroups.lsnd));
		}
		#endregion
	}
	#endregion

	#region globals
	[TI.TagGroup((int)TagGroups.Enumerated.matg, 4, 1364)]
	public class globals_group : TI.Definition
	{
		#region havok_cleanup_resources_block
		[TI.Definition(1, 16)]
		public class havok_cleanup_resources_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public havok_cleanup_resources_block()
			{
				Add(/*object cleanup effect = */ new TI.TagReference(this, TagGroups.effe));
			}
			#endregion
		}
		#endregion

		#region sound_globals_block
		[TI.Definition(3, 80)]
		public class sound_globals_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public sound_globals_block()
			{
				Add(/*sound classes = */ new TI.TagReference(this, TagGroups.sncl));
				Add(/*sound effects = */ new TI.TagReference(this, TagGroups.sfx_));
				Add(/*sound mix = */ new TI.TagReference(this, TagGroups.snmx));
				Add(/*sound combat dialogue constants = */ new TI.TagReference(this, TagGroups.spk_));
				Add(/*sound global propagation = */ new TI.TagReference(this, TagGroups.sgp_));
			}
			#endregion
		}
		#endregion

		#region ai_globals_block
		[TI.Definition(1, 416)]
		public class ai_globals_block : TI.Definition
		{
			#region ai_globals_gravemind_block
			[TI.Definition(1, 12)]
			public class ai_globals_gravemind_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public ai_globals_gravemind_block()
				{
					Add(/*min retreat time = */ new TI.Real());
					Add(/*ideal retreat time = */ new TI.Real());
					Add(/*max retreat time = */ new TI.Real());
				}
				#endregion
			}
			#endregion

			#region ai_globals_style_block
			[TI.Definition(1, 12)]
			public class ai_globals_style_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public ai_globals_style_block()
				{
					Add(/*style = */ new TI.TagReference(this, TagGroups.styl));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public ai_globals_block()
			{
				Add(/*danger broadly facing = */ new TI.Real());
				Add(new TI.Pad(4));
				Add(/*danger shooting near = */ new TI.Real());
				Add(new TI.Pad(4));
				Add(/*danger shooting at = */ new TI.Real());
				Add(new TI.Pad(4));
				Add(/*danger extremely close = */ new TI.Real());
				Add(new TI.Pad(4));
				Add(/*danger shield damage = */ new TI.Real());
				Add(/*danger exetended shield damage = */ new TI.Real());
				Add(/*danger body damage = */ new TI.Real());
				Add(/*danger extended body damage = */ new TI.Real());
				Add(new TI.Pad(48));
				Add(/*global dialogue tag = */ new TI.TagReference(this, TagGroups.adlg));
				Add(/*default mission dialogue sound effect = */ new TI.StringId());
				Add(new TI.Pad(20));
				Add(/*jump down = */ new TI.Real());
				Add(/*jump step = */ new TI.Real());
				Add(/*jump crouch = */ new TI.Real());
				Add(/*jump stand = */ new TI.Real());
				Add(/*jump storey = */ new TI.Real());
				Add(/*jump tower = */ new TI.Real());
				Add(/*max jump down height down = */ new TI.Real());
				Add(/*max jump down height step = */ new TI.Real());
				Add(/*max jump down height crouch = */ new TI.Real());
				Add(/*max jump down height stand = */ new TI.Real());
				Add(/*max jump down height storey = */ new TI.Real());
				Add(/*max jump down height tower = */ new TI.Real());
				Add(/*hoist step = */ new TI.RealBounds());
				Add(/*hoist crouch = */ new TI.RealBounds());
				Add(/*hoist stand = */ new TI.RealBounds());
				Add(new TI.Pad(24));
				Add(/*vault step = */ new TI.RealBounds());
				Add(/*vault crouch = */ new TI.RealBounds());
				Add(new TI.Pad(48));
				Add(/*gravemind properties = */ new TI.Block<ai_globals_gravemind_block>(this, 1));
				Add(new TI.Pad(48));
				Add(/*scary target threhold = */ new TI.Real());
				Add(/*scary weapon threhold = */ new TI.Real());
				//Add(/*player scariness = */ new TI.Real());
				//Add(/*berserking actor scariness = */ new TI.Real());
				Add(new TI.Pad(10 * sizeof(float))); // can't pin-point where the above 2 values are in <-- here
				Add(/*styles = */ new TI.Block<ai_globals_style_block>(this, 0));
			}
			#endregion
		}
		#endregion

		#region game_globals_damage_block
		[TI.Definition(1, 12)]
		public class game_globals_damage_block : TI.Definition
		{
			#region damage_group_block
			[TI.Definition(1, 16)]
			public class damage_group_block : TI.Definition
			{
				#region armor_modifier_block
				[TI.Definition(1, 8)]
				public class armor_modifier_block : TI.Definition
				{
					#region Fields
					#endregion

					#region Ctor
					public armor_modifier_block()
					{
						Add(/*name = */ new TI.StringId());
						Add(/*damage multiplier = */ new TI.Real());
					}
					#endregion
				}
				#endregion

				#region Fields
				#endregion

				#region Ctor
				public damage_group_block()
				{
					Add(/*name = */ new TI.StringId());
					Add(/*armor modifiers = */ new TI.Block<armor_modifier_block>(this, 2147483647));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public game_globals_damage_block()
			{
				Add(/*damage groups = */ new TI.Block<damage_group_block>(this, 2147483647));
			}
			#endregion
		}
		#endregion

		#region sound_block
		[TI.Definition(1, 16)]
		public class sound_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public sound_block()
			{
				Add(/*sound (OBSOLETE) = */ new TI.TagReference(this));
			}
			#endregion
		}
		#endregion

		#region camera_block
		[TI.Definition(1, 160)]
		public class camera_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public camera_block()
			{
				Add(/*default unit camera track = */ new TI.TagReference(this, TagGroups.trak));
				//Add(/*default change pause = */ new TI.Real());
				//Add(/*first person change pause = */ new TI.Real());
				//Add(/*following camera change pause = */ new TI.Real());
				Add(new TI.Skip(36 * sizeof(float)));
			}
			#endregion
		}
		#endregion

		#region player_control_block
		[TI.Definition(2, 160)]
		public class player_control_block : TI.Definition
		{
			#region look_function_block
			[TI.Definition(1, 4)]
			public class look_function_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public look_function_block()
				{
					Add(/*scale = */ new TI.Real());
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public player_control_block()
			{
				//Add(/*magnetism friction = */ new TI.Real(TI.FieldType.RealFraction));
				//Add(/*magnetism adhesion = */ new TI.Real(TI.FieldType.RealFraction));
				//Add(/*inconsequential target scale = */ new TI.Real(TI.FieldType.RealFraction));
				Add(new TI.Skip(9 * sizeof(float)));
				Add(new TI.Pad(12));
				Add(/*crosshair location = */ new TI.RealPoint2D());
				Add(/*seconds to start = */ new TI.Real());
				Add(/*seconds to full speed = */ new TI.Real());
				Add(/*decay rate = */ new TI.Real());
				Add(/*full speed multiplier = */ new TI.Real());
				Add(/*pegged magnitude = */ new TI.Real());
				Add(/*pegged angular threshold = */ new TI.Real());
				Add(new TI.Pad(8));
				Add(/*look default pitch rate = */ new TI.Real());
				Add(/*look default yaw rate = */ new TI.Real());
				Add(/*look peg threshold [0,1] = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*look yaw acceleration time = */ new TI.Real());
				Add(/*look yaw acceleration scale = */ new TI.Real());
				Add(/*look pitch acceleration time = */ new TI.Real());
				Add(/*look pitch acceleration scale = */ new TI.Real());
				Add(/*look autolevelling scale = */ new TI.Real());
				Add(new TI.Pad(8));
				Add(/*gravity_scale = */ new TI.Real());
				Add(new TI.Pad(2));
				Add(/*minimum autolevelling ticks = */ new TI.ShortInteger());
				Add(/*minimum angle for vehicle flipping = */ new TI.Real(TI.FieldType.Angle));
				Add(/*look function = */ new TI.Block<look_function_block>(this, 16));
				Add(/*minimum action hold time = */ new TI.Real());
				Add(new TI.Skip(4)); // TODO: UNKNOWN FIELD
			}
			#endregion
		}
		#endregion

		#region difficulty_block
		[TI.Definition(1, 644)]
		public class difficulty_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public difficulty_block()
			{
				Add(/*easy enemy damage = */ new TI.Real());
				Add(/*normal enemy damage = */ new TI.Real());
				Add(/*hard enemy damage = */ new TI.Real());
				Add(/*imposs. enemy damage = */ new TI.Real());
				Add(/*easy enemy vitality = */ new TI.Real());
				Add(/*normal enemy vitality = */ new TI.Real());
				Add(/*hard enemy vitality = */ new TI.Real());
				Add(/*imposs. enemy vitality = */ new TI.Real());
				Add(/*easy enemy shield = */ new TI.Real());
				Add(/*normal enemy shield = */ new TI.Real());
				Add(/*hard enemy shield = */ new TI.Real());
				Add(/*imposs. enemy shield = */ new TI.Real());
				Add(/*easy enemy recharge = */ new TI.Real());
				Add(/*normal enemy recharge = */ new TI.Real());
				Add(/*hard enemy recharge = */ new TI.Real());
				Add(/*imposs. enemy recharge = */ new TI.Real());
				Add(/*easy friend damage = */ new TI.Real());
				Add(/*normal friend damage = */ new TI.Real());
				Add(/*hard friend damage = */ new TI.Real());
				Add(/*imposs. friend damage = */ new TI.Real());
				Add(/*easy friend vitality = */ new TI.Real());
				Add(/*normal friend vitality = */ new TI.Real());
				Add(/*hard friend vitality = */ new TI.Real());
				Add(/*imposs. friend vitality = */ new TI.Real());
				Add(/*easy friend shield = */ new TI.Real());
				Add(/*normal friend shield = */ new TI.Real());
				Add(/*hard friend shield = */ new TI.Real());
				Add(/*imposs. friend shield = */ new TI.Real());
				Add(/*easy friend recharge = */ new TI.Real());
				Add(/*normal friend recharge = */ new TI.Real());
				Add(/*hard friend recharge = */ new TI.Real());
				Add(/*imposs. friend recharge = */ new TI.Real());
				Add(/*easy infection forms = */ new TI.Real());
				Add(/*normal infection forms = */ new TI.Real());
				Add(/*hard infection forms = */ new TI.Real());
				Add(/*imposs. infection forms = */ new TI.Real());
				Add(new TI.Pad(16));
				Add(/*easy rate of fire = */ new TI.Real());
				Add(/*normal rate of fire = */ new TI.Real());
				Add(/*hard rate of fire = */ new TI.Real());
				Add(/*imposs. rate of fire = */ new TI.Real());
				Add(/*easy projectile error = */ new TI.Real());
				Add(/*normal projectile error = */ new TI.Real());
				Add(/*hard projectile error = */ new TI.Real());
				Add(/*imposs. projectile error = */ new TI.Real());
				Add(/*easy burst error = */ new TI.Real());
				Add(/*normal burst error = */ new TI.Real());
				Add(/*hard burst error = */ new TI.Real());
				Add(/*imposs. burst error = */ new TI.Real());
				Add(/*easy new target delay = */ new TI.Real());
				Add(/*normal new target delay = */ new TI.Real());
				Add(/*hard new target delay = */ new TI.Real());
				Add(/*imposs. new target delay = */ new TI.Real());
				Add(/*easy burst separation = */ new TI.Real());
				Add(/*normal burst separation = */ new TI.Real());
				Add(/*hard burst separation = */ new TI.Real());
				Add(/*imposs. burst separation = */ new TI.Real());
				Add(/*easy target tracking = */ new TI.Real());
				Add(/*normal target tracking = */ new TI.Real());
				Add(/*hard target tracking = */ new TI.Real());
				Add(/*imposs. target tracking = */ new TI.Real());
				Add(/*easy target leading = */ new TI.Real());
				Add(/*normal target leading = */ new TI.Real());
				Add(/*hard target leading = */ new TI.Real());
				Add(/*imposs. target leading = */ new TI.Real());
				Add(/*easy overcharge chance = */ new TI.Real());
				Add(/*normal overcharge chance = */ new TI.Real());
				Add(/*hard overcharge chance = */ new TI.Real());
				Add(/*imposs. overcharge chance = */ new TI.Real());
				Add(/*easy special fire delay = */ new TI.Real());
				Add(/*normal special fire delay = */ new TI.Real());
				Add(/*hard special fire delay = */ new TI.Real());
				Add(/*imposs. special fire delay = */ new TI.Real());
				Add(/*easy guidance vs player = */ new TI.Real());
				Add(/*normal guidance vs player = */ new TI.Real());
				Add(/*hard guidance vs player = */ new TI.Real());
				Add(/*imposs. guidance vs player = */ new TI.Real());
				Add(/*easy melee delay base = */ new TI.Real());
				Add(/*normal melee delay base = */ new TI.Real());
				Add(/*hard melee delay base = */ new TI.Real());
				Add(/*imposs. melee delay base = */ new TI.Real());
				Add(/*easy melee delay scale = */ new TI.Real());
				Add(/*normal melee delay scale = */ new TI.Real());
				Add(/*hard melee delay scale = */ new TI.Real());
				Add(/*imposs. melee delay scale = */ new TI.Real());
				Add(new TI.Pad(16));
				Add(/*easy grenade chance scale = */ new TI.Real());
				Add(/*normal grenade chance scale = */ new TI.Real());
				Add(/*hard grenade chance scale = */ new TI.Real());
				Add(/*imposs. grenade chance scale = */ new TI.Real());
				Add(/*easy grenade timer scale = */ new TI.Real());
				Add(/*normal grenade timer scale = */ new TI.Real());
				Add(/*hard grenade timer scale = */ new TI.Real());
				Add(/*imposs. grenade timer scale = */ new TI.Real());
				Add(new TI.Pad(16));
				Add(new TI.Pad(16));
				Add(new TI.Pad(16));
				Add(/*easy major upgrade (normal) = */ new TI.Real());
				Add(/*normal major upgrade (normal) = */ new TI.Real());
				Add(/*hard major upgrade (normal) = */ new TI.Real());
				Add(/*imposs. major upgrade (normal) = */ new TI.Real());
				Add(/*easy major upgrade (few) = */ new TI.Real());
				Add(/*normal major upgrade (few) = */ new TI.Real());
				Add(/*hard major upgrade (few) = */ new TI.Real());
				Add(/*imposs. major upgrade (few) = */ new TI.Real());
				Add(/*easy major upgrade (many) = */ new TI.Real());
				Add(/*normal major upgrade (many) = */ new TI.Real());
				Add(/*hard major upgrade (many) = */ new TI.Real());
				Add(/*imposs. major upgrade (many) = */ new TI.Real());
				Add(/*easy player vehicle ram chance = */ new TI.Real());
				Add(/*normal player vehicle ram chance = */ new TI.Real());
				Add(/*hard player vehicle ram chance = */ new TI.Real());
				Add(/*imposs. player vehicle ram chance = */ new TI.Real());
				Add(new TI.Pad(16));
				Add(new TI.Pad(16));
				Add(new TI.Pad(16));
				Add(new TI.Pad(84));
			}
			#endregion
		}
		#endregion

		#region grenades_block
		[TI.Definition(1, 68)]
		public class grenades_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public grenades_block()
			{
				Add(/*maximum count = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(/*throwing effect = */ new TI.TagReference(this, TagGroups.effe));
				Add(new TI.Pad(16));
				Add(/*equipment = */ new TI.TagReference(this, TagGroups.eqip));
				Add(/*projectile = */ new TI.TagReference(this, TagGroups.proj));
			}
			#endregion
		}
		#endregion

		#region interface_tag_references
		[TI.Definition(2, 288)]
		public class interface_tag_references : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public interface_tag_references()
			{
				Add(/*obsolete1 = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*obsolete2 = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*screen color table = */ new TI.TagReference(this, TagGroups.colo));
				Add(/*hud color table = */ new TI.TagReference(this, TagGroups.colo));
				Add(/*editor color table = */ new TI.TagReference(this, TagGroups.colo));
				Add(/*dialog color table = */ new TI.TagReference(this, TagGroups.colo));
				Add(/*hud globals = */ new TI.TagReference(this));
				Add(/*motion sensor sweep bitmap = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*motion sensor sweep bitmap mask = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*multiplayer hud bitmap = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/* = */ new TI.TagReference(this));
				Add(/*hud digits definition = */ new TI.TagReference(this));
				Add(/*motion sensor blip bitmap = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*interface noise = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*mainmenu ui globals = */ new TI.TagReference(this, TagGroups.wgtz));
				Add(/*singleplayer ui globals = */ new TI.TagReference(this, TagGroups.wgtz));
				Add(/*multiplayer ui globals = */ new TI.TagReference(this, TagGroups.wgtz));
				Add(/*chud globals = */ new TI.TagReference(this, TagGroups.chgd));
			}
			#endregion
		}
		#endregion

		#region cheat_weapons_block
		[TI.Definition(1, 16)]
		public class cheat_weapons_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public cheat_weapons_block()
			{
				Add(/*weapon = */ new TI.TagReference(this, TagGroups.item));
			}
			#endregion
		}
		#endregion

		#region cheat_powerups_block
		[TI.Definition(1, 16)]
		public class cheat_powerups_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public cheat_powerups_block()
			{
				Add(/*powerup = */ new TI.TagReference(this, TagGroups.eqip));
			}
			#endregion
		}
		#endregion

		#region player_information_block
		[TI.Definition(2, 192)]
		public class player_information_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public player_information_block()
			{
				Add(/*walking speed = */ new TI.Real());
				Add(/*run forward = */ new TI.Real());
				Add(/*run backward = */ new TI.Real());
				Add(/*run sideways = */ new TI.Real());
				Add(/*run acceleration = */ new TI.Real());
				Add(/*sneak forward = */ new TI.Real());
				Add(/*sneak backward = */ new TI.Real());
				Add(/*sneak sideways = */ new TI.Real());
				Add(/*sneak acceleration = */ new TI.Real());
				Add(/*airborne acceleration = */ new TI.Real());
				Add(/*grenade origin = */ new TI.RealPoint3D());
				Add(/*stun movement penalty = */ new TI.Real());
				Add(/*stun turning penalty = */ new TI.Real());
				Add(/*stun jumping penalty = */ new TI.Real());
				Add(/*minimum stun time = */ new TI.Real());
				Add(/*maximum stun time = */ new TI.Real());
				Add(/*first person idle time = */ new TI.RealBounds());
				Add(/*first person skip fraction = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*unused = */ new TI.TagReference(this));
				Add(/*unused = */ new TI.TagReference(this));
				Add(/*coop respawn effect = */ new TI.TagReference(this, TagGroups.effe));
				Add(/*binoculars zoom count = */ new TI.LongInteger());
				Add(/*binoculars zoom range = */ new TI.RealBounds());
				Add(/*flashlight on = */ new TI.TagReference(this, TagGroups.snd_));
				Add(/*flashlight off = */ new TI.TagReference(this, TagGroups.snd_));
				Add(/*? = */ new TI.TagReference(this, TagGroups.drdf));
			}
			#endregion
		}
		#endregion

		#region player_representation_block
		[TI.Definition(2, 84)]
		public class player_representation_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public player_representation_block()
			{
				Add(/*first person hands = */ new TI.TagReference(this, TagGroups.mode));
				Add(/*first person body = */ new TI.TagReference(this, TagGroups.mode));
				Add(/*third person unit = */ new TI.TagReference(this, TagGroups.unit));
				Add(/*third person variant = */ new TI.StringId());
				Add(/*binoculars zoom in sound = */ new TI.TagReference(this, TagGroups.snd_));
				Add(/*binoculars zoom out sound = */ new TI.TagReference(this, TagGroups.snd_));
			}
			#endregion
		}
		#endregion

		#region falling_damage_block
		[TI.Definition(2, 120)]
		public class falling_damage_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public falling_damage_block()
			{
				Add(/*harmful falling distance = */ new TI.RealBounds());
				Add(/*falling damage = */ new TI.TagReference(this, TagGroups.jpt_));
				Add(/*land soft damage = */ new TI.TagReference(this, TagGroups.jpt_));
				Add(/*land hard damage = */ new TI.TagReference(this, TagGroups.jpt_));
				Add(/*scripted damage = */ new TI.TagReference(this, TagGroups.jpt_));
				Add(/*maximum falling distance = */ new TI.Real());
				Add(/*distance damage = */ new TI.TagReference(this, TagGroups.jpt_));
				Add(new TI.Pad(3 * sizeof(float))); // these may be angles
			}
			#endregion
		}
		#endregion

		#region materials_block
		[TI.Definition(2, 368)]
		public class materials_block : TI.Definition
		{
			#region old_materials_block
			[TI.Definition(2, 92)]
			public class old_materials_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public old_materials_block()
				{
					Add(/*new material name = */ new TI.StringId());
					Add(/*new general material name = */ new TI.StringId());
					Add(/*? = */ new TI.ShortInteger()); // TODO: UNKNOWN FIELD
					Add(/*? = */ new TI.ShortInteger()); // TODO: UNKNOWN FIELD
					//Add(/*ground friction scale = */ new TI.Real());
					//Add(/*ground friction normal k1 scale = */ new TI.Real());
					//Add(/*ground friction normal k0 scale = */ new TI.Real());
					//Add(/*ground depth scale = */ new TI.Real());
					//Add(/*ground damp fraction scale = */ new TI.Real());
					Add(new TI.Skip(19 * sizeof(float))); // TODO: UNKNOWN FIELDS
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public materials_block()
			{
				Add(/*name = */ new TI.StringId());
				Add(/*parent name = */ new TI.StringId());
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(/*old material type = */ new TI.Enum());
				Add(/*general armor = */ new TI.StringId());
				Add(/*specific armor = */ new TI.StringId());
				Add(/*physics properties = */ new TI.Struct<material_physics_properties_struct>(this));
				Add(new TI.Pad(4));
				Add(/*sweeteners = */ new TI.Struct<materials_sweeteners_struct>(this));
				Add(/*material effects = */ new TI.TagReference(this, TagGroups.foot));
				Add(/*old materials = */ new TI.Block<old_materials_block>(this, 0));
			}
			#endregion
		}
		#endregion

		#region Fields
		public s_cache_language_pack
				English,
				Japanese,
				German,
				French,
				Spanish,
				Mexican,
				Italian,
				Korean,
				Chinese,
				Portuguese,
				Polish,
				Unknown;
		#endregion

		#region Ctor
		public globals_group()
		{
			Add(new TI.Pad(172));
			Add(/*language = */ new TI.Enum(TI.FieldType.LongEnum));
			Add(/*havok cleanup resources = */ new TI.Block<havok_cleanup_resources_block>(this, 1));
			Add(/*sound globals = */ new TI.Block<sound_globals_block>(this, 1));
			Add(/*ai globals = */ new TI.Block<ai_globals_block>(this, 1));
			Add(/*damage table = */ new TI.Block<game_globals_damage_block>(this, 2));
			Add(/* = */ new TI.Block<TI.g_null_block>(this, 0));
			Add(/*sounds = */ new TI.Block<sound_block>(this, 2));
			Add(/*camera = */ new TI.Block<camera_block>(this, 1));
			Add(/*player control = */ new TI.Block<player_control_block>(this, 1));
			Add(/*difficulty = */ new TI.Block<difficulty_block>(this, 1));
			Add(/*grenades = */ new TI.Block<grenades_block>(this, 4));
			Add(/*rasterizer data = */ new TI.Block<TI.g_null_block>(this, 0));
			Add(/*interface tags = */ new TI.Block<interface_tag_references>(this, 1));
			Add(/*@weapon list (update _weapon_list enum in game_globals.h) = */ new TI.Block<cheat_weapons_block>(this, 20));
			Add(/*@cheat powerups = */ new TI.Block<cheat_powerups_block>(this, 20));
			Add(/*@player information = */ new TI.Block<player_information_block>(this, 1));
			Add(/*@player representation = */ new TI.Block<player_representation_block>(this, 6));
			Add(/*falling damage = */ new TI.Block<falling_damage_block>(this, 1));
			Add(/*materials = */ new TI.Block<materials_block>(this, 256));
			Add(/*profile colors = */ new TI.Block<multiplayer_color_block>(this, 32));
			Add(/*multiplayer globals = */ new TI.TagReference(this, TagGroups.mulg));
			Add(TI.Pad.BlockHalo3); // [0x14]
			Add(TI.Pad.BlockHalo3); // [0x?]

			English = new s_cache_language_pack(this);
			Japanese = new s_cache_language_pack(this);
			German = new s_cache_language_pack(this);
			French = new s_cache_language_pack(this);
			Spanish = new s_cache_language_pack(this);
			Mexican = new s_cache_language_pack(this);
			Italian = new s_cache_language_pack(this);
			Korean = new s_cache_language_pack(this);
			Chinese = new s_cache_language_pack(this);
			Portuguese = new s_cache_language_pack(this);
			Polish = new s_cache_language_pack(this);
			Unknown = new s_cache_language_pack(this); // I've only seen this used in MP maps, but I've only checked a few, and it only has a few strings

			Add(/*rasterizer globals = */ new TI.TagReference(this, TagGroups.rasg));
			Add(/*default camera fx settings = */ new TI.TagReference(this, TagGroups.cfxs));
			Add(/*default wind = */ new TI.TagReference(this, TagGroups.wind));
			Add(/*default damage effect = */ new TI.TagReference(this, TagGroups.jpt_));
			Add(/*default collision damage = */ new TI.TagReference(this, TagGroups.cddf));
			Add(new TI.LongInteger()); // TODO: UNKNOWN FIELD
			Add(new TI.LongInteger()); // TODO: UNKNOWN FIELD, string id?
			Add(/*multiplayer globals = */ new TI.TagReference(this, TagGroups.effg));
		}
		#endregion

		internal s_cache_language_pack GetLanguage(LanguageType lang)
		{
			switch (lang)
			{
				case LanguageType.English:		return English;
				case LanguageType.Japanese:		return Japanese;
				case LanguageType.German:		return German;
				case LanguageType.French:		return French;
				case LanguageType.Spanish:		return Spanish;
				case LanguageType.Mexican:		return Mexican;
				case LanguageType.Italian:		return Italian;
				case LanguageType.Korean:		return Korean;
				case LanguageType.Chinese:		return Chinese;
				case LanguageType.Portuguese:	return Portuguese;
				case LanguageType.Polish:		return Polish;
				default:						throw new Debug.Exceptions.UnreachableException(lang);
			}
		}
	};
	#endregion

	#region sound_response_extra_sounds_struct
	[TI.Struct((int)StructGroups.Enumerated.masd_sound, 3, 192)]
	public class sound_response_extra_sounds_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public sound_response_extra_sounds_struct()
		{
			Add(/*japanese sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*german sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*french sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*spanish sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*mexican sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*italian sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*korean sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*chinese sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*portuguese sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*polish sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*? sound = */ new TI.TagReference(this, TagGroups.snd_));
		}
		#endregion
	}
	#endregion

	#region sound_response_definition_block
	[TI.Definition(1, 216)] // only the in-line struct changed we think, so this is still v1
	public class sound_response_definition_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public sound_response_definition_block()
		{
			Add(/*sound flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(/*english sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*extra sounds = */ new TI.Struct<sound_response_extra_sounds_struct>(this));
			Add(/*probability = */ new TI.Real());
		}
		#endregion
	}
	#endregion

	#region game_engine_general_event_block
	[TI.Definition(2, 260)]
	public class game_engine_general_event_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public game_engine_general_event_block()
		{
			Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(/*event = */ new TI.Enum());
			Add(/*audience = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(new TI.Pad(2));
			Add(/*? string = */ new TI.StringId());
			Add(/*display string = */ new TI.StringId());

			Add(/*required field = */ new TI.Enum());
			Add(/*excluded audience = */ new TI.Enum());
			Add(/*primary string = */ new TI.StringId());
			Add(/*primary string duration = */ new TI.LongInteger());
			Add(/*plural display string = */ new TI.StringId());
			Add(new TI.Pad(8)); // i THINK this is padding. in h2, it was 28...
			Add(/*sound delay (announcer only) = */ new TI.Real());
			Add(/*sound flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(/*sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(/*extra sounds = */ new TI.Struct<sound_response_extra_sounds_struct>(this));
			Add(new TI.Pad(4));
			// TODO: BLOCK STRUCTURE VERIFICATION
			// BUT, its never used...
			Add(/*sound permutations = */ new TI.Block<sound_response_definition_block>(this, 10));
		}
		#endregion
	}
	#endregion

	#region game_engine_slayer_event_block
	[TI.Definition(2, 260)]
	public class game_engine_slayer_event_block : game_engine_general_event_block
	{
		#region Fields
		#endregion

		#region Ctor
		public game_engine_slayer_event_block() : base() { }
		#endregion
	}
	#endregion

	#region game_engine_ctf_event_block
	[TI.Definition(2, 260)]
	public class game_engine_ctf_event_block : game_engine_general_event_block
	{
		#region Fields
		#endregion

		#region Ctor
		public game_engine_ctf_event_block() : base() { }
		#endregion
	}
	#endregion

	#region game_engine_oddball_event_block
	[TI.Definition(2, 260)]
	public class game_engine_oddball_event_block : game_engine_general_event_block
	{
		#region Fields
		#endregion

		#region Ctor
		public game_engine_oddball_event_block() : base() { }
		#endregion
	}
	#endregion

	#region game_engine_king_event_block
	[TI.Definition(2, 260)]
	public class game_engine_king_event_block : game_engine_general_event_block
	{
		#region Fields
		#endregion

		#region Ctor
		public game_engine_king_event_block() : base() { }
		#endregion
	}
	#endregion

	#region multiplayer_universal_block
	[TI.Definition(2, 180)]
	public class multiplayer_universal_block : TI.Definition
	{
		#region multiplayer_appearance_block
		[TI.Definition(1, 16)]
		public class multiplayer_appearance_block : TI.Definition
		{
			#region multiplayer_appearance_catagory_block
			[TI.Definition(1, 16)]
			public class multiplayer_appearance_catagory_block : TI.Definition
			{
				#region multiplayer_appearance_catagory_variant_block
				[TI.Definition(1, 28)]
				public class multiplayer_appearance_catagory_variant_block : TI.Definition
				{
					#region multiplayer_appearance_catagory_variant_id_block
					[TI.Definition(1, 8)]
					public class multiplayer_appearance_catagory_variant_id_block : TI.Definition
					{
						#region Fields
						#endregion

						#region Ctor
						public multiplayer_appearance_catagory_variant_id_block()
						{
							Add(/*catagory = */ new TI.StringId());
							Add(/*name = */ new TI.StringId());
						}
						#endregion
					};
					#endregion

					#region Fields
					#endregion

					#region Ctor
					public multiplayer_appearance_catagory_variant_block()
					{
						Add(/*name = */ new TI.StringId());
						Add(/*description string = */ new TI.StringId());
						Add(/*condition = */ new TI.StringId());
						Add(/*achievement condition = */ new TI.StringId());
						// I think there should only be one element of this
						Add(/*id = */ new TI.Block<multiplayer_appearance_catagory_variant_id_block>(this, 0));
					}
					#endregion
				};
				#endregion

				#region Fields
				#endregion

				#region Ctor
				public multiplayer_appearance_catagory_block()
				{
					Add(/*name = */ new TI.StringId());
					Add(/*variants = */ new TI.Block<multiplayer_appearance_catagory_variant_block>(this, 0));
				}
				#endregion
			};
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public multiplayer_appearance_block()
			{
				Add(/*model name = */ new TI.StringId());
				Add(/*catagories = */ new TI.Block<multiplayer_appearance_catagory_block>(this, 0));
			}
			#endregion
		};
		#endregion

		#region sandbox_weapon_block
		[TI.Definition(1, 24)]
		public class sandbox_weapon_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public sandbox_weapon_block()
			{
				Add(/*name = */ new TI.StringId());
				Add(/*default cost = */ new TI.Real());
				Add(/*weapon = */ new TI.TagReference(this, TagGroups.item));
			}
			#endregion
		};
		#endregion

		#region sandbox_vehicle_block
		[TI.Definition(1, 20)]
		public class sandbox_vehicle_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public sandbox_vehicle_block()
			{
				Add(/*name = */ new TI.StringId());
				Add(/*vehicle = */ new TI.TagReference(this, TagGroups.vehi));
			}
			#endregion
		};
		#endregion

		#region sandbox_equipment_block
		[TI.Definition(1, 20)]
		public class sandbox_equipment_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public sandbox_equipment_block()
			{
				Add(/*name = */ new TI.StringId());
				Add(/*equipment = */ new TI.TagReference(this, TagGroups.eqip));
			}
			#endregion
		};
		#endregion

		#region multiplayer_object_catagory_block
		[TI.Definition(1, 16)]
		public class multiplayer_object_catagory_block : TI.Definition
		{
			#region multiplayer_object_catagory_object_block
			[TI.Definition(1, 8)]
			public class multiplayer_object_catagory_object_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public multiplayer_object_catagory_object_block()
				{
					Add(/*name = */ new TI.StringId());
					// spawns [name] where [replaces] vehicle would go normally
					Add(/*replaces = */ new TI.StringId());
				}
				#endregion
			};
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public multiplayer_object_catagory_block()
			{
				Add(/*name = */ new TI.StringId());
				Add(/*allowed objects = */ new TI.Block<multiplayer_object_catagory_object_block>(this, 0));
			}
			#endregion
		};
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public multiplayer_universal_block()
		{
			Add(/*random player names = */ new TI.TagReference(this, TagGroups.unic));
			Add(/*team names = */ new TI.TagReference(this, TagGroups.unic));
			Add(/*team colors = */ new TI.Block<multiplayer_color_block>(this, 32));
			Add(/*appearances = */ new TI.Block<multiplayer_appearance_block>(this, 6));
			Add(/*multiplayer text = */ new TI.TagReference(this, TagGroups.unic));
			Add(/*sandbox ui text = */ new TI.TagReference(this, TagGroups.unic));
			Add(/*sandbox ui values = */ new TI.TagReference(this, TagGroups.jmrq));
			Add(/*sandbox weapons = */ new TI.Block<sandbox_weapon_block>(this, 0));
			Add(/*sandbox vehicles = */ new TI.Block<sandbox_vehicle_block>(this, 0));
			Add(/*sandbox equipment = */ new TI.Block<sandbox_equipment_block>(this, 0));
			Add(/*weapon catagories = */ new TI.Block<multiplayer_object_catagory_block>(this, 0));
			Add(/*vehicle catagories = */ new TI.Block<multiplayer_object_catagory_block>(this, 0));
			Add(/*game engine settings = */ new TI.TagReference(this, TagGroups.wezr));
		}
		#endregion
	}
	#endregion

	#region multiplayer_runtime_block
	[TI.Definition(3, 524)]
	public class multiplayer_runtime_block : TI.Definition
	{
		#region game_engine_flavor_event_block
		[TI.Definition(2, 260)]
		public class game_engine_flavor_event_block : game_engine_general_event_block
		{
			#region Fields
			#endregion

			#region Ctor
			public game_engine_flavor_event_block() : base() { }
			#endregion
		}
		#endregion

		#region game_engine_vip_event_block
		[TI.Definition(2, 260)]
		public class game_engine_vip_event_block : game_engine_general_event_block
		{
			#region Fields
			#endregion

			#region Ctor
			public game_engine_vip_event_block() : base() { }
			#endregion
		}
		#endregion

		#region game_engine_juggernaut_event_block
		[TI.Definition(2, 260)]
		public class game_engine_juggernaut_event_block : game_engine_general_event_block
		{
			#region Fields
			#endregion

			#region Ctor
			public game_engine_juggernaut_event_block() : base() { }
			#endregion
		}
		#endregion

		#region game_engine_territories_event_block
		[TI.Definition(2, 260)]
		public class game_engine_territories_event_block : game_engine_general_event_block
		{
			#region Fields
			#endregion

			#region Ctor
			public game_engine_territories_event_block() : base() { }
			#endregion
		}
		#endregion

		#region game_engine_assault_event_block
		[TI.Definition(2, 260)]
		public class game_engine_assault_event_block : game_engine_general_event_block
		{
			#region Fields
			#endregion

			#region Ctor
			public game_engine_assault_event_block() : base() { }
			#endregion
		}
		#endregion

		#region game_engine_infection_event_block
		[TI.Definition(2, 260)]
		public class game_engine_infection_event_block : game_engine_general_event_block
		{
			#region Fields
			#endregion

			#region Ctor
			public game_engine_infection_event_block() : base() { }
			#endregion
		}
		#endregion

		#region multiplayer_constants_block
		[TI.Definition(2, 540)]
		public class multiplayer_constants_block : TI.Definition
		{
			#region weapons_block
			[TI.Definition(1, 32)]
			public class weapons_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public weapons_block()
				{
					Add(/*weapon = */ new TI.TagReference(this, TagGroups.item));
					Add(new TI.Skip(16)); // all reals // danger values?
				}
				#endregion
			}
			#endregion

			#region vehicles_block
			[TI.Definition(1, 32)]
			public class vehicles_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public vehicles_block()
				{
					Add(/*vehicle = */ new TI.TagReference(this, TagGroups.vehi));
					Add(new TI.Skip(16)); // all reals // danger values?
				}
				#endregion
			}
			#endregion

			#region projectiles_block
			[TI.Definition(1, 28)]
			public class projectiles_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public projectiles_block()
				{
					Add(/*projectile = */ new TI.TagReference(this, TagGroups.proj));
					Add(new TI.Skip(12)); // all reals // danger values?
				}
				#endregion
			}
			#endregion

			#region powerup_block
			[TI.Definition(1, 20)]
			public class powerup_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public powerup_block()
				{
					Add(/*weapon = */ new TI.TagReference(this, TagGroups.eqip));
					Add(new TI.Skip(16)); // all reals // danger values?
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public multiplayer_constants_block()
			{
				Add(new TI.Skip(104)); // all reals
				Add(/*weapons = */ new TI.Block<weapons_block>(this, 20));
				Add(/*vehicles = */ new TI.Block<vehicles_block>(this, 20));
				Add(/*projectiles = */ new TI.Block<projectiles_block>(this, 20));
				Add(/*equipment = */ new TI.Block<powerup_block>(this, 20));
				Add(new TI.Skip(160)); // all reals

				Add(/*maximum random spawn bias = */ new TI.Real());
				Add(/*teleporter recharge time = */ new TI.Real());
				Add(/*grenade danger weight = */ new TI.Real());
				Add(/*grenade danger inner radius = */ new TI.Real());
				Add(/*grenade danger outer radius = */ new TI.Real());
				Add(/*grenade danger lead time = */ new TI.Real());
				Add(/*vehicle danger min speed = */ new TI.Real());
				Add(/*vehicle danger weight = */ new TI.Real());
				Add(/*vehicle danger radius = */ new TI.Real());
				Add(/*vehicle danger lead time = */ new TI.Real());
				Add(/*vehicle nearby player dist = */ new TI.Real());
				Add(/*hill bitmap = */ new TI.TagReference(this, TagGroups.bitm));
				Add(new TI.Skip(16)); // all reals //Add(/*flag reset stop distance = */ new TI.Real());
				Add(/*bomb explode effect = */ new TI.TagReference(this, TagGroups.effe));
				// TODO: VERIFY GROUP TAG
				Add(/*? = */ new TI.TagReference(this, TagGroups.effe));
				Add(/*bomb explode dmg effect = */ new TI.TagReference(this, TagGroups.jpt_));
				Add(/*bomb defuse effect = */ new TI.TagReference(this, TagGroups.effe));
				Add(/*cursor impact effect = */ new TI.TagReference(this, TagGroups.effe));
				Add(/*bomb defusal string = */ new TI.StringId());
				Add(/*blocked teleporter string = */ new TI.StringId());
				Add(new TI.Pad(4));
				Add(/*spawn_allowed_default_respawn_string string = */ new TI.StringId());
				Add(/*spawn_at_player_allowed_looking_at_self_string string = */ new TI.StringId());
				Add(/*spawn_at_player_allowed_looking_at_target_string string = */ new TI.StringId());
				Add(/*spawn_at_player_allowed_looking_at_potential_target_string string = */ new TI.StringId());
				Add(/*spawn_at_territory_allowed_looking_at_target_string string = */ new TI.StringId());
				Add(/*spawn_at_territory_allowed_looking_at_potential_target_string string = */ new TI.StringId());
				Add(/*player_out_of_lives_string string = */ new TI.StringId());
				Add(/*invalid_spawn_target_string string = */ new TI.StringId());
				Add(/*targetted_player_enemies_near_by_string string = */ new TI.StringId());
				Add(/*targetted_player_unfriendly_team_string string = */ new TI.StringId());
				Add(/*targetted_player_is_dead_string string = */ new TI.StringId());
				Add(/*targetted_player_in_combat_shields string = */ new TI.StringId());
				Add(/*targetted_player_too_far_from_owned_flag string = */ new TI.StringId());
				Add(/*no available netpoints string = */ new TI.StringId());
				Add(/*netpoint contested string = */ new TI.StringId());
			}
			#endregion
		}
		#endregion

		#region game_engine_status_response_block
		[TI.Definition(1, 36)]
		public class game_engine_status_response_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public game_engine_status_response_block()
			{
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2));
				Add(/*state = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*ffa message = */ new TI.StringId());
				Add(/*team message = */ new TI.StringId());
				Add(/* = */ new TI.TagReference(this));
				Add(new TI.Pad(4));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public multiplayer_runtime_block()
		{
			Add(/*editor unit = */ new TI.TagReference(this, TagGroups.unit));
			Add(/*editor gizmo = */ new TI.TagReference(this, TagGroups.scen));
			Add(/*flag = */ new TI.TagReference(this, TagGroups.item));
			Add(/*ball = */ new TI.TagReference(this, TagGroups.item));
			Add(/*da bomb = */ new TI.TagReference(this, TagGroups.item));
			Add(/*vip boundary = */ new TI.TagReference(this, TagGroups.bloc));
			Add(/*in game text = */ new TI.TagReference(this, TagGroups.unic));
			Add(/*sounds = */ new TI.Block<sounds_block>(this, 60));
			Add(/*looping sounds = */ new TI.Block<sound_loopings_block>(this, 0));
			Add(/*general events = */ new TI.Block<game_engine_general_event_block>(this, 128));
			Add(/*flavor events = */ new TI.Block<game_engine_flavor_event_block>(this, 128));
			Add(/*slayer events = */ new TI.Block<game_engine_slayer_event_block>(this, 128));
			Add(/*ctf events = */ new TI.Block<game_engine_ctf_event_block>(this, 128));
			Add(/*oddball events = */ new TI.Block<game_engine_oddball_event_block>(this, 128));
			Add(/*king events = */ new TI.Block<game_engine_king_event_block>(this, 128));
			Add(/*vip events = */ new TI.Block<game_engine_vip_event_block>(this, 128));
			Add(/*juggernaut events = */ new TI.Block<game_engine_juggernaut_event_block>(this, 128));
			Add(/*territories events = */ new TI.Block<game_engine_territories_event_block>(this, 128));
			Add(/*invasion events = */ new TI.Block<game_engine_assault_event_block>(this, 128));
			Add(/*infection events = */ new TI.Block<game_engine_infection_event_block>(this, 128));
			Add(/*? = */ new TI.LongInteger());
			Add(/*? = */ new TI.LongInteger());
			Add(/*multiplayer constants = */ new TI.Block<multiplayer_constants_block>(this, 1));
			Add(/*state responses = */ new TI.Block<game_engine_status_response_block>(this, 32));
			Add(/*scoreboard emblem bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*scoreboard dead emblem bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			// Never used, but I would think that thats what its for...
			Add(/*scoreboard emblem shader = */ new TI.TagReference(this, TagGroups.rmhg));
			Add(/*ctf intro = */ new TI.TagReference(this, TagGroups.chdt));
			Add(/*slayer intro = */ new TI.TagReference(this, TagGroups.chdt));
			Add(/*oddball intro = */ new TI.TagReference(this, TagGroups.chdt));
			Add(/*king intro = */ new TI.TagReference(this, TagGroups.chdt));
			Add(/*editor intro = */ new TI.TagReference(this, TagGroups.chdt));
			Add(/*vip intro = */ new TI.TagReference(this, TagGroups.chdt));
			Add(/*juggernaut intro = */ new TI.TagReference(this, TagGroups.chdt));
			Add(/*territories intro = */ new TI.TagReference(this, TagGroups.chdt));
			Add(/*invasion intro = */ new TI.TagReference(this, TagGroups.chdt));
			Add(/*infection intro = */ new TI.TagReference(this, TagGroups.chdt));

		}
		#endregion
	}
	#endregion

	#region multiplayer_globals
	[TI.TagGroup((int)TagGroups.Enumerated.mulg, 1, 24)]
	public class multiplayer_globals_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public multiplayer_globals_group()
		{
			Add(/*universal = */ new TI.Block<multiplayer_universal_block>(this, 1));
			Add(/*runtime = */ new TI.Block<multiplayer_runtime_block>(this, 1));
		}
		#endregion
	};
	#endregion

	#region game_engine_settings_definition
	[TI.TagGroup((int)TagGroups.Enumerated.wezr, 1, 136)]
	public class game_engine_settings_definition_group : TI.Definition
	{
		#region Fields
		#endregion

		public game_engine_settings_definition_group()
		{
			// tag block [0x48?] [base variant]
				// string id [name string]
				// string id [description string]
				// tag block [0x?]
				// tag block [0x?]
				// tag block [0x?]
				// tag block [0x?]
				// dword
				// short
				// byte
				// byte index
				// byte index
				// byte
				// word
				// word
				// word

			// dword
			// tag block [0x64]
				// string id [name]
				// tag block [0x8]
				// tag block [0x10]
				// tag block [0x4]
				// tag block [0x?]
				// tag block [0x?]
			// tag block [0x50] [slayer variants]
			// tag block [0x50] [oddball variants]
			// tag block [0x50] [ctf variants]
			// tag block [0x58] [assault variants]
			// tag block [0x5C] [infection variants]
			// tag block [0x4C] [king variants]
			// tag block [0x4C] [territories variants]
			// tag block [0x50] [juggernaut variants]
			// tag block [0x58] [vip variants]
			// tag block [0x50] [sandbox variants]
		}
	};
	#endregion
}