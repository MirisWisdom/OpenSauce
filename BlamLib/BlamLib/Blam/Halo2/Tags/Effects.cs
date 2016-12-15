/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo2.Tags
{
	#region particle_system_definition_block_new
	[TI.Definition(2, 68)]
	public class particle_system_definition_block_new : TI.Definition
	{
		#region particle_property_scalar_struct_new
		[TI.Struct((int)StructGroups.Enumerated.PRPS, 1, 20)]
		public class particle_property_scalar_struct_new : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public particle_property_scalar_struct_new() : base(5)
			{
				Add(/*Input Variable = */ new TI.Enum());
				Add(/*Range Variable = */ new TI.Enum());
				Add(/*Output Modifier = */ new TI.Enum());
				Add(/*Output Modifier Input = */ new TI.Enum());
				Add(/*Mapping = */ new TI.Struct<mapping_function>(this));
			}
			#endregion
		}
		#endregion

		#region particle_property_color_struct_new
		[TI.Struct((int)StructGroups.Enumerated.PRPC, 1, 20)]
		public class particle_property_color_struct_new : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public particle_property_color_struct_new() : base(5)
			{
				Add(/*Input Variable = */ new TI.Enum());
				Add(/*Range Variable = */ new TI.Enum());
				Add(/*Output Modifier = */ new TI.Enum());
				Add(/*Output Modifier Input = */ new TI.Enum());
				Add(/*Mapping = */ new TI.Struct<mapping_function>(this));
			}
			#endregion
		}
		#endregion

		#region particle_system_emitter_definition_block
		[TI.Definition(1, 228)]
		public class particle_system_emitter_definition_block : TI.Definition
		{
			#region Fields
			public TI.RealPoint3D TranslationalOffset;
			public TI.RealEulerAngles2D RelativeDirection;
			#endregion

			#region Ctor
			public particle_system_emitter_definition_block() : base(14)
			{
				Add(/*particle physics = */ new TI.TagReference(this, TagGroups.pmov));
				Add(/*particle emission rate = */ new TI.Struct<particle_property_scalar_struct_new>(this));
				Add(/*particle lifespan = */ new TI.Struct<particle_property_scalar_struct_new>(this));
				Add(/*particle velocity = */ new TI.Struct<particle_property_scalar_struct_new>(this));
				Add(/*particle angular velocity = */ new TI.Struct<particle_property_scalar_struct_new>(this));
				Add(/*particle size = */ new TI.Struct<particle_property_scalar_struct_new>(this));
				Add(/*particle tint = */ new TI.Struct<particle_property_color_struct_new>(this));
				Add(/*particle alpha = */ new TI.Struct<particle_property_scalar_struct_new>(this));
				Add(/*emission shape = */ new TI.Enum(TI.FieldType.LongEnum));
				Add(/*emission radius = */ new TI.Struct<particle_property_scalar_struct_new>(this));
				Add(/*emission angle = */ new TI.Struct<particle_property_scalar_struct_new>(this));
				Add(TranslationalOffset = new TI.RealPoint3D());
				Add(RelativeDirection = new TI.RealEulerAngles2D());
				Add(new TI.Pad(8));
			}
			#endregion
		}
		#endregion

		#region Fields
		public TI.Block<particle_system_emitter_definition_block> Emitters;
		#endregion

		#region Ctor
		public particle_system_definition_block_new() : base(15)
		{
			Add(/*particle = */ new TI.TagReference(this)); // prt3,PRTM,
			Add(/*location = */ new TI.BlockIndex(TI.FieldType.LongBlockIndex)); // 1 effect_locations_block
			Add(/*coordinate system = */ new TI.Enum());
			Add(/*environment = */ new TI.Enum());
			Add(/*disposition = */ new TI.Enum());
			Add(/*camera mode = */ new TI.Enum());
			Add(/*sort bias = */ new TI.ShortInteger());
			Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*LOD in distance = */ new TI.Real());
			Add(/*LOD feather in delta = */ new TI.Real());
			Add(new TI.Skip(4));
			Add(/*LOD out distance = */ new TI.Real());
			Add(/*LOD feather out delta = */ new TI.Real());
			Add(new TI.Skip(4));
			Add(Emitters = new TI.Block<particle_system_emitter_definition_block>(this, 8));
		}
		#endregion
	}
	#endregion

	#region colony
	[TI.TagGroup((int)TagGroups.Enumerated.coln, 4, 76)]
	public class colony_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public colony_group() : base(7)
		{
			Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2 + 4));
			Add(/*radius = */ new TI.RealBounds());
			Add(new TI.Pad(12));
			Add(/*debug color = */ new TI.RealColor(TI.FieldType.RealArgbColor));
			Add(/*base map = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*detail map = */ new TI.TagReference(this, TagGroups.bitm));
		}
		#endregion
	};
	#endregion

	#region damage_outer_cone_angle_struct
	[TI.Struct((int)StructGroups.Enumerated.masd_damage, 2, 4)]
	public class damage_outer_cone_angle_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public damage_outer_cone_angle_struct() : base(1)
		{
			Add(/*dmg outer cone angle = */ new TI.Real(TI.FieldType.Angle));
		}
		#endregion
	}
	#endregion

	#region screen_flash_definition_struct
	[TI.Struct((int)StructGroups.Enumerated.SFDS, 1, 32)]
	public class screen_flash_definition_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public screen_flash_definition_struct() : base(10)
		{
			Add(/*type = */ new TI.Enum());
			Add(/*priority = */ new TI.Enum());
			Add(new TI.UselessPad(12));
			Add(/*duration = */ new TI.Real());
			Add(/*fade function = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(new TI.UselessPad(8));
			Add(/*maximum intensity = */ new TI.Real(TI.FieldType.RealFraction));
			Add(new TI.UselessPad(4));
			Add(/*color = */ new TI.RealColor(TI.FieldType.RealArgbColor));
		}
		#endregion
	}
	#endregion

	#region vibration_frequency_definition_struct
	[TI.Struct((int)StructGroups.Enumerated.RFDS, 1, 16)]
	public class vibration_frequency_definition_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public vibration_frequency_definition_struct() : base(3)
		{
			Add(/*duration = */ new TI.Real());
			Add(/*dirty whore = */ new TI.Struct<mapping_function>(this));
			Add(new TI.UselessPad(16));
		}
		#endregion
	}
	#endregion

	#region vibration_definition_struct
	[TI.Struct((int)StructGroups.Enumerated.RBDS, 1, 32)]
	public class vibration_definition_struct : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public vibration_definition_struct() : base(3)
		{
			Add(/*low frequency vibration = */ new TI.Struct<vibration_frequency_definition_struct>(this));
			Add(/*high frequency vibration = */ new TI.Struct<vibration_frequency_definition_struct>(this));
			Add(new TI.UselessPad(16));
		}
		#endregion
	}
	#endregion

	#region damage_effect_sound_effect_definition
	[TI.Struct((int)StructGroups.Enumerated.dsfx, 1, 20)]
	public class damage_effect_sound_effect_definition : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public damage_effect_sound_effect_definition() : base(3)
		{
			Add(/*effect name = */ new TI.StringId());
			Add(/*duration = */ new TI.Real());
			Add(/*effect scale function = */ new TI.Struct<mapping_function>(this));
		}
		#endregion
	}
	#endregion

	#region damage_effect
	[TI.TagGroup((int)TagGroups.Enumerated.jpt_, 6, 2, 212)]
	public class damage_effect_group : TI.Definition
	{
		#region damage_effect_player_response_block
		[TI.Definition(1, 88)]
		public class damage_effect_player_response_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public damage_effect_player_response_block() : base(6)
			{
				Add(/*response type = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*screen flash = */ new TI.Struct<screen_flash_definition_struct>(this));
				Add(/*vibration = */ new TI.Struct<vibration_definition_struct>(this));
				Add(/*sound effect = */ new TI.Struct<damage_effect_sound_effect_definition>(this));
				Add(new TI.UselessPad(24));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public damage_effect_group() : base(57)
		{
			Add(/*radius = */ new TI.RealBounds());
			Add(/*cutoff scale = */ new TI.Real(TI.FieldType.RealFraction));
			Add(/*flags = */ new TI.Flags());
			Add(/*side effect = */ new TI.Enum());
			Add(/*category = */ new TI.Enum());
			Add(/*flags = */ new TI.Flags());
			Add(/*AOE core radius = */ new TI.Real());
			Add(/*damage lower bound = */ new TI.Real());
			Add(/*damage upper bound = */ new TI.RealBounds());
			Add(/*dmg inner cone angle = */ new TI.Real(TI.FieldType.Angle));
			Add(/*blah = */ new TI.Struct<damage_outer_cone_angle_struct>(this));
			Add(/*active camouflage damage = */ new TI.Real());
			Add(/*stun = */ new TI.Real());
			Add(/*maximum stun = */ new TI.Real());
			Add(/*stun time = */ new TI.Real());
			Add(new TI.UselessPad(4));
			Add(/*instantaneous acceleration = */ new TI.Real());
			Add(new TI.UselessPad(4 + 4));
			Add(/*rider direct damage scale = */ new TI.Real());
			Add(/*rider maximum transfer damage scale = */ new TI.Real());
			Add(/*rider minimum transfer damage scale = */ new TI.Real());
			Add(new TI.UselessPad(140));
			Add(/*general_damage = */ new TI.StringId());
			Add(/*specific_damage = */ new TI.StringId());
			Add(/*AI stun radius = */ new TI.Real());
			Add(/*AI stun bounds = */ new TI.RealBounds());
			Add(/*shake radius = */ new TI.Real());
			Add(/*EMP radius = */ new TI.Real());
			Add(/*player responses = */ new TI.Block<damage_effect_player_response_block>(this, 2));
			Add(/*duration = */ new TI.Real());
			Add(/*fade function = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(/*rotation = */ new TI.Real(TI.FieldType.Angle));
			Add(/*pushback = */ new TI.Real());
			Add(/*jitter = */ new TI.RealBounds());
			Add(new TI.UselessPad(4 + 24));
			Add(/*duration = */ new TI.Real());
			Add(/*falloff function = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(/*random translation = */ new TI.Real());
			Add(/*random rotation = */ new TI.Real(TI.FieldType.Angle));
			Add(new TI.UselessPad(12));
			Add(/*wobble function = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(/*wobble function period = */ new TI.Real());
			Add(/*wobble weight = */ new TI.Real(TI.FieldType.RealFraction));
			Add(new TI.UselessPad(4 + 20 + 8));
			Add(/*sound = */ new TI.TagReference(this, TagGroups.snd_));
			Add(new TI.UselessPad(112));
			Add(/*forward velocity = */ new TI.Real());
			Add(/*forward radius = */ new TI.Real());
			Add(/*forward exponent = */ new TI.Real());
			Add(new TI.UselessPad(12));
			Add(/*outward velocity = */ new TI.Real());
			Add(/*outward radius = */ new TI.Real());
			Add(/*outward exponent = */ new TI.Real());
			Add(new TI.UselessPad(12));
		}
		#endregion
	};
	#endregion

	#region decal
	[TI.TagGroup((int)TagGroups.Enumerated.deca, 1, 188)]
	public class decal_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public decal_group() : base(20)
		{
			Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(/*type = */ new TI.Enum());
			Add(/*layer = */ new TI.Enum());
			Add(/*max overlapping count = */ new TI.ShortInteger());
			Add(/*next decal in chain = */ new TI.TagReference(this, TagGroups.deca));
			Add(/*radius = */ new TI.RealBounds());
			Add(/*radius overlap rejection = */ new TI.Real());
			Add(new TI.UselessPad(16));
			Add(/*color lower bounds = */ new TI.RealColor());
			Add(/*color upper bounds = */ new TI.RealColor());
			Add(new TI.UselessPad(12 + 4 + 28));
			Add(/*lifetime = */ new TI.RealBounds());
			Add(/*decay time = */ new TI.RealBounds());
			Add(new TI.UselessPad(12));
			Add(new TI.Pad(40 + 2 + 2 + 2 + 2 + 20));
			Add(/*bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Pad(20));
			Add(/*maximum sprite extent = */ new TI.Real());
			Add(new TI.Pad(4));
			Add(new TI.UselessPad(8));
		}
		#endregion
	};
	#endregion

	#region effect
	[TI.TagGroup((int)TagGroups.Enumerated.effe, 4, 64)]
	public class effect_group : TI.Definition
	{
		#region effect_locations_block
		[TI.Definition(1, 4)]
		public class effect_locations_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public effect_locations_block() : base(1)
			{
				Add(/*marker name = */ new TI.StringId(true));
			}
			#endregion
		}
		#endregion

		#region effect_event_block
		[TI.Definition(1, 72)]
		public class effect_event_block : TI.Definition
		{
			#region effect_part_block
			[TI.Definition(1, 64)]
			public class effect_part_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public effect_part_block() : base(12)
				{
					Add(/*create in = */ new TI.Enum());
					Add(/*create in = */ new TI.Enum());
					Add(/*location = */ new TI.BlockIndex()); // 1 effect_locations_block
					Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
					Add(new TI.Pad(4));
					Add(/*type = */ new TI.TagReference(this)); // jpt!,obje,snd!,deca,coln,ligh,MGS2,tdtl,lens,
					Add(/*velocity bounds = */ new TI.RealBounds());
					Add(/*velocity cone angle = */ new TI.Real(TI.FieldType.Angle));
					Add(/*angular velocity bounds = */ new TI.RealBounds(TI.FieldType.AngleBounds));
					Add(/*radius modifier bounds = */ new TI.RealBounds());
					Add(/*A scales values = */ new TI.Flags());
					Add(/*B scales values = */ new TI.Flags());
				}
				#endregion
			}
			#endregion

			#region beam_block
			[TI.Definition(1, 92)]
			public class beam_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public beam_block() : base(9)
				{
					Add(/*shader = */ new TI.TagReference(this, TagGroups.shad));
					Add(/*location = */ new TI.BlockIndex()); // 1 effect_locations_block
					Add(new TI.Pad(2));
					Add(/*color = */ new TI.Struct<color_function_struct>(this));
					Add(/*alpha = */ new TI.Struct<scalar_function_struct>(this));
					Add(/*width = */ new TI.Struct<scalar_function_struct>(this));
					Add(/*length = */ new TI.Struct<scalar_function_struct>(this));
					Add(/*yaw = */ new TI.Struct<scalar_function_struct>(this));
					Add(/*pitch = */ new TI.Struct<scalar_function_struct>(this));
				}
				#endregion
			}
			#endregion

			#region effect_accelerations_block
			[TI.Definition(1, 20)]
			public class effect_accelerations_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public effect_accelerations_block() : base(7)
				{
					Add(/*create in = */ new TI.Enum());
					Add(/*create in = */ new TI.Enum());
					Add(/*location = */ new TI.BlockIndex()); // 1 effect_locations_block
					Add(new TI.Pad(2));
					Add(/*acceleration = */ new TI.Real());
					Add(/*inner cone angle = */ new TI.Real());
					Add(/*outer cone angle = */ new TI.Real());
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public effect_event_block() : base(9)
			{
				Add(/*flags = */ new TI.Flags());
				Add(/*skip fraction = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*delay bounds = */ new TI.RealBounds());
				Add(/*duration bounds = */ new TI.RealBounds());
				Add(/*parts = */ new TI.Block<effect_part_block>(this, 32));
				Add(new TI.UselessPad(12));
				Add(/*beams = */ new TI.Block<beam_block>(this, 1024));
				Add(/*accelerations = */ new TI.Block<effect_accelerations_block>(this, 32));
				Add(/*particle systems = */ new TI.Block<particle_system_definition_block_new>(this, 32));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public effect_group() : base(12)
		{
			Add(/*flags = */ new TI.Flags());
			Add(/*loop start event = */ new TI.BlockIndex()); // 1 effect_event_block
			Add(new TI.Skip(2));
			Add(new TI.Pad(4));
			Add(/*locations = */ new TI.Block<effect_locations_block>(this, 32));
			Add(/*events = */ new TI.Block<effect_event_block>(this, 32));
			Add(new TI.UselessPad(12));
			Add(/*looping sound = */ new TI.TagReference(this, TagGroups.lsnd));
			Add(/*location = */ new TI.BlockIndex()); // 1 effect_locations_block
			Add(new TI.Skip(2));
			Add(/*always play distance = */ new TI.Real());
			Add(/*never play distance = */ new TI.Real());
		}
		#endregion
	};
	#endregion

	#region lens_flare
	[TI.TagGroup((int)TagGroups.Enumerated.lens, 2, 124)]
	public class lens_flare_group : TI.Definition
	{
		#region lens_flare_reflection_block
		[TI.Definition(1, 48)]
		public class lens_flare_reflection_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public lens_flare_reflection_block() : base(15)
			{
				Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2));
				Add(/*bitmap index = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(new TI.UselessPad(20));
				Add(/*position = */ new TI.Real());
				Add(/*rotation offset = */ new TI.Real());
				Add(new TI.UselessPad(4));
				Add(/*radius = */ new TI.RealBounds());
				Add(new TI.UselessPad(4));
				Add(/*brightness = */ new TI.RealBounds(TI.FieldType.RealFractionBounds));
				Add(new TI.UselessPad(4));
				Add(/*modulation factor = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*color = */ new TI.RealColor());
				Add(new TI.UselessPad(48));
			}
			#endregion
		}
		#endregion

		#region lens_flare_scalar_animation_block
		[TI.Definition(1, 12)]
		public class lens_flare_scalar_animation_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public lens_flare_scalar_animation_block() : base(1)
			{
				Add(/*function = */ new TI.Struct<scalar_function_struct>(this));
			}
			#endregion
		}
		#endregion

		#region lens_flare_color_animation_block
		[TI.Definition(1, 12)]
		public class lens_flare_color_animation_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public lens_flare_color_animation_block() : base(1)
			{
				Add(/*function = */ new TI.Struct<color_function_struct>(this));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public lens_flare_group() : base(27)
		{
			Add(/*falloff angle = */ new TI.Real(TI.FieldType.Angle));
			Add(/*cutoff angle = */ new TI.Real(TI.FieldType.Angle));
			Add(new TI.Skip(4 + 4));
			Add(/*occlusion radius = */ new TI.Real());
			Add(/*occlusion offset direction = */ new TI.Enum());
			Add(/*occlusion inner radius scale = */ new TI.Enum());
			Add(/*near fade distance = */ new TI.Real());
			Add(/*far fade distance = */ new TI.Real());
			Add(/*bitmap = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Skip(2));
			Add(new TI.UselessPad(76));
			Add(/*rotation function = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(/*rotation function scale = */ new TI.Real(TI.FieldType.Angle));
			Add(new TI.UselessPad(24));
			Add(/*corona scale = */ new TI.RealVector2D());
			Add(/*falloff function = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(new TI.UselessPad(24));
			Add(/*reflections = */ new TI.Block<lens_flare_reflection_block>(this, 32));
			Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
			Add(new TI.Pad(2));
			Add(/*brightness = */ new TI.Block<lens_flare_scalar_animation_block>(this, 1));
			Add(/*color = */ new TI.Block<lens_flare_color_animation_block>(this, 1));
			Add(/*rotation = */ new TI.Block<lens_flare_scalar_animation_block>(this, 1));
			Add(new TI.UselessPad(4));
		}
		#endregion
	};
	#endregion

	#region light
	[TI.TagGroup((int)TagGroups.Enumerated.ligh, 4, 272)]
	public class light_group : TI.Definition
	{
		#region light_brightness_animation_block
		[TI.Definition(1, 12)]
		public class light_brightness_animation_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public light_brightness_animation_block() : base(1)
			{
				Add(/*function = */ new TI.Struct<mapping_function>(this));
			}
			#endregion
		}
		#endregion

		#region light_color_animation_block
		[TI.Definition(1, 12)]
		public class light_color_animation_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public light_color_animation_block() : base(1)
			{
				Add(/*function = */ new TI.Struct<mapping_function>(this));
			}
			#endregion
		}
		#endregion

		#region light_gel_animation_block
		[TI.Definition(1, 24)]
		public class light_gel_animation_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public light_gel_animation_block() : base(2)
			{
				Add(/*dx = */ new TI.Struct<mapping_function>(this));
				Add(/*dy = */ new TI.Struct<mapping_function>(this));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public light_group() : base(62)
		{
			Add(/*flags = */ new TI.Flags());
			Add(new TI.UselessPad(16));
			Add(/*type = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(/*size modifer = */ new TI.RealBounds());
			Add(/*shadow quality bias = */ new TI.Real());
			Add(/*shadow tap bias = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(new TI.UselessPad(24));
			Add(/*radius = */ new TI.Real());
			Add(/*specular radius = */ new TI.Real());
			Add(new TI.UselessPad(32));
			Add(/*near width = */ new TI.Real());
			Add(/*height stretch = */ new TI.Real());
			Add(/*field of view = */ new TI.Real());
			Add(/*falloff distance = */ new TI.Real());
			Add(/*cutoff distance = */ new TI.Real());
			Add(new TI.UselessPad(4));
			Add(/*interpolation flags = */ new TI.Flags());
			Add(/*bloom bounds = */ new TI.RealBounds());
			Add(/*specular lower bound = */ new TI.RealColor());
			Add(/*specular upper bound = */ new TI.RealColor());
			Add(/*diffuse lower bound = */ new TI.RealColor());
			Add(new TI.UselessPad(4));
			Add(/*diffuse upper bound = */ new TI.RealColor());
			Add(/*brightness bounds = */ new TI.RealBounds());
			Add(new TI.UselessPad(4));
			Add(/*gel map = */ new TI.TagReference(this, TagGroups.bitm));
			Add(/*specular mask = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(new TI.UselessPad(12));
			Add(new TI.Pad(4));
			Add(new TI.UselessPad(80 + 12 + 12 + 12 + 16));
			Add(/*falloff function = */ new TI.Enum());
			Add(/*diffuse contrast = */ new TI.Enum());
			Add(/*specular contrast = */ new TI.Enum());
			Add(/*falloff geometry = */ new TI.Enum());
			Add(new TI.UselessPad(8));
			Add(/*lens flare = */ new TI.TagReference(this, TagGroups.lens));
			Add(/*bounding radius = */ new TI.Real());
			Add(/*light volume = */ new TI.TagReference(this, TagGroups.MGS2));
			Add(new TI.UselessPad(8));
			Add(/*default lightmap setting = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(/*lightmap half life = */ new TI.Real());
			Add(/*lightmap light scale = */ new TI.Real());
			Add(new TI.UselessPad(20));
			Add(/*duration = */ new TI.Real());
			Add(new TI.Pad(2));
			Add(/*falloff function = */ new TI.Enum());
			Add(new TI.UselessPad(8));
			Add(/*illumination fade = */ new TI.Enum());
			Add(/*shadow fade = */ new TI.Enum());
			Add(/*specular fade = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(new TI.UselessPad(8));
			Add(/*flags = */ new TI.Flags());
			Add(/*brightness animation = */ new TI.Block<light_brightness_animation_block>(this, 1));
			Add(/*color animation = */ new TI.Block<light_color_animation_block>(this, 1));
			Add(/*gel animation = */ new TI.Block<light_gel_animation_block>(this, 1));
			Add(new TI.UselessPad(72));
			Add(/*shader = */ new TI.TagReference(this, TagGroups.shad));
		}
		#endregion
	};
	#endregion

	#region light_volume
	[TI.TagGroup((int)TagGroups.Enumerated.MGS2, 1, 20)]
	public class light_volume_group : TI.Definition
	{
		#region light_volume_volume_block
		[TI.Definition(1, 188)]
		public class light_volume_volume_block : TI.Definition
		{
			#region light_volume_aspect_block
			[TI.Definition(1, 36)]
			public class light_volume_aspect_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public light_volume_aspect_block() : base(5)
				{
					Add(/*along axis = */ new TI.Struct<scalar_function_struct>(this));
					Add(/*away from axis = */ new TI.Struct<scalar_function_struct>(this));
					Add(/*parallel scale = */ new TI.Real());
					Add(/*parallel threshold angle = */ new TI.Real(TI.FieldType.Angle));
					Add(/*parallel exponent = */ new TI.Real());
				}
				#endregion
			}
			#endregion

			#region light_volume_runtime_offset_block
			[TI.Definition(1, 8)]
			public class light_volume_runtime_offset_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public light_volume_runtime_offset_block() : base(1)
				{
					Add(/* = */ new TI.RealVector2D());
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public light_volume_volume_block() : base(25)
			{
				Add(/*flags = */ new TI.Flags());
				Add(new TI.UselessPad(16));
				Add(/*bitmap = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*sprite count = */ new TI.LongInteger());
				Add(new TI.UselessPad(32));
				Add(/*offset function = */ new TI.Struct<scalar_function_struct>(this));
				Add(/*radius function = */ new TI.Struct<scalar_function_struct>(this));
				Add(/*brightness function = */ new TI.Struct<scalar_function_struct>(this));
				Add(/*color function = */ new TI.Struct<color_function_struct>(this));
				Add(new TI.UselessPad(64));
				Add(/*facing function = */ new TI.Struct<scalar_function_struct>(this));
				Add(new TI.UselessPad(64));
				Add(/*aspect = */ new TI.Block<light_volume_aspect_block>(this, 1));
				Add(new TI.UselessPad(64));
				Add(/*radius frac min = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*DEPRECATED!! x-step exponent = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*DEPRECATED!! x-buffer length = */ new TI.LongInteger());
				Add(/*x-buffer spacing = */ new TI.LongInteger());
				Add(/*x-buffer min iterations = */ new TI.LongInteger());
				Add(/*x-buffer max iterations = */ new TI.LongInteger());
				Add(/*x-delta max error = */ new TI.Real(TI.FieldType.RealFraction));
				Add(new TI.UselessPad(48));
				Add(new TI.Skip(4));
				Add(/* = */ new TI.Block<light_volume_runtime_offset_block>(this, 256));
				Add(new TI.Skip(48));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public light_volume_group() : base(5)
		{
			// Explanation here
			Add(new TI.UselessPad(64));
			Add(/*falloff distance from camera = */ new TI.Real());
			Add(/*cutoff distance from camera = */ new TI.Real());
			Add(new TI.UselessPad(32));
			Add(/*volumes = */ new TI.Block<light_volume_volume_block>(this, 16));
		}
		#endregion
	};
	#endregion

	#region particle_property_color_struct_new
	[TI.Struct((int)StructGroups.Enumerated.PRPC, 1, 20)]
	public class particle_property_color_struct_new : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public particle_property_color_struct_new() : base(5)
		{
			Add(/*Input Variable = */ new TI.Enum());
			Add(/*Range Variable = */ new TI.Enum());
			Add(/*Output Modifier = */ new TI.Enum());
			Add(/*Output Modifier Input = */ new TI.Enum());
			Add(/*Mapping = */ new TI.Struct<mapping_function>(this));
		}
		#endregion
	}
	#endregion

	#region particle_property_scalar_struct_new
	[TI.Struct((int)StructGroups.Enumerated.PRPS, 1, 20)]
	public class particle_property_scalar_struct_new : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public particle_property_scalar_struct_new() : base(5)
		{
			Add(/*Input Variable = */ new TI.Enum());
			Add(/*Range Variable = */ new TI.Enum());
			Add(/*Output Modifier = */ new TI.Enum());
			Add(/*Output Modifier Input = */ new TI.Enum());
			Add(/*Mapping = */ new TI.Struct<mapping_function>(this));
		}
		#endregion
	}
	#endregion

	#region effect_locations_block
	[TI.Definition(1, 4)]
	public class effect_locations_block : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public effect_locations_block() : base(1)
		{
			Add(/*marker name = */ new TI.StringId());
		}
		#endregion
	}
	#endregion

	#region particle
	[TI.TagGroup((int)TagGroups.Enumerated.prt3, 1, 248)]
	public class particle_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public particle_group() : base(20)
		{
			Add(/*flags = */ new TI.Flags());
			Add(/*particle billboard style = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(/*first sequence index = */ new TI.ShortInteger());
			Add(/*sequence count = */ new TI.ShortInteger());

			// Custom: hide
			Add(/*shader template = */ new TI.TagReference(this, TagGroups.stem));
			Add(/*shader parameters = */ new TI.Block<global_shader_parameter_block>(this, 64));
			// Custom: edih

			Add(/*color = */ new TI.Struct<particle_property_color_struct_new>(this));
			Add(/*alpha = */ new TI.Struct<particle_property_scalar_struct_new>(this));
			Add(/*scale = */ new TI.Struct<particle_property_scalar_struct_new>(this));
			Add(/*rotation = */ new TI.Struct<particle_property_scalar_struct_new>(this));
			Add(/*frame index = */ new TI.Struct<particle_property_scalar_struct_new>(this));
			Add(/*collision effect = */ new TI.TagReference(this)); // effe,snd!,foot,
			Add(/*death effect = */ new TI.TagReference(this)); // effe,snd!,foot,
			Add(/*locations = */ new TI.Block<effect_locations_block>(this, 32));
			Add(/*attached particle systems = */ new TI.Block<particle_system_definition_block_new>(this, 32));
			Add(/* = */ new TI.Block<shader_postprocess_definition_new_block>(this, 1));
			Add(new TI.Pad(8));
			Add(new TI.Pad(16));
			Add(new TI.Pad(16));
		}
		#endregion
	};
	#endregion

	#region particle_physics
	[TI.TagGroup((int)TagGroups.Enumerated.pmov, 1, 32)]
	public class particle_physics_group : TI.Definition
	{
		#region particle_controller
		[TI.Definition(1, 24)]
		public class particle_controller : TI.Definition
		{
			#region particle_controller_parameters
			[TI.Definition(1, 24)]
			public class particle_controller_parameters : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public particle_controller_parameters() : base(2)
				{
					Add(/*parameter id = */ new TI.LongInteger());
					Add(/*property = */ new TI.Struct<particle_property_scalar_struct_new>(this));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public particle_controller() : base(4)
			{
				Add(/*type = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*parameters = */ new TI.Block<particle_controller_parameters>(this, 9));
				Add(new TI.Pad(8));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public particle_physics_group() : base(3)
		{
			Add(/*template = */ new TI.TagReference(this, TagGroups.pmov));
			Add(/*flags = */ new TI.Flags());
			Add(/*movements = */ new TI.Block<particle_controller>(this, 4));
		}
		#endregion
	};
	#endregion

	#region screen_effect
	[TI.TagGroup((int)TagGroups.Enumerated.egor, 4, 156)]
	public class screen_effect_group : TI.Definition
	{
		#region rasterizer_screen_effect_pass_reference_block
		[TI.Definition(1, 192)]
		public class rasterizer_screen_effect_pass_reference_block : TI.Definition
		{
			#region rasterizer_screen_effect_texcoord_generation_advanced_control_block
			[TI.Definition(1, 72)]
			public class rasterizer_screen_effect_texcoord_generation_advanced_control_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public rasterizer_screen_effect_texcoord_generation_advanced_control_block() : base(8)
				{
					Add(/*stage 0 flags = */ new TI.Flags(TI.FieldType.WordFlags));
					Add(/*stage 1 flags = */ new TI.Flags(TI.FieldType.WordFlags));
					Add(/*stage 2 flags = */ new TI.Flags(TI.FieldType.WordFlags));
					Add(/*stage 3 flags = */ new TI.Flags(TI.FieldType.WordFlags));
					Add(/*stage 0 offset = */ new TI.RealPlane3D());
					Add(/*stage 1 offset = */ new TI.RealPlane3D());
					Add(/*stage 2 offset = */ new TI.RealPlane3D());
					Add(/*stage 3 offset = */ new TI.RealPlane3D());
				}
				#endregion
			}
			#endregion

			#region rasterizer_screen_effect_convolution_block
			[TI.Definition(1, 92)]
			public class rasterizer_screen_effect_convolution_block : TI.Definition
			{
				#region Fields
				#endregion

				#region Ctor
				public rasterizer_screen_effect_convolution_block() : base(8)
				{
					// Explanation here
					Add(/*flags = */ new TI.Flags(TI.FieldType.WordFlags));
					Add(new TI.Pad(2 + 64));
					Add(/*convolution amount = */ new TI.Real());
					Add(/*filter scale = */ new TI.Real());
					Add(/*filter box factor = */ new TI.Real(TI.FieldType.RealFraction));
					Add(/*zoom falloff radius = */ new TI.Real());
					Add(/*zoom cutoff radius = */ new TI.Real());
					Add(/*resolution scale = */ new TI.Real(TI.FieldType.RealFraction));
				}
				#endregion
			}
			#endregion

			#region Fields
			#endregion

			#region Ctor
			public rasterizer_screen_effect_pass_reference_block() : base(16)
			{
				Add(/*explanation = */ new TI.Data(this));
				Add(/*layer pass index = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(/*primary=0 and secondary=0 = */ new TI.ByteInteger());
				Add(/*primary>0 and secondary=0 = */ new TI.ByteInteger());
				Add(/*primary=0 and secondary>0 = */ new TI.ByteInteger());
				Add(/*primary>0 and secondary>0 = */ new TI.ByteInteger());
				Add(new TI.Pad(64));
				Add(/*stage 0 mode = */ new TI.Enum());
				Add(/*stage 1 mode = */ new TI.Enum());
				Add(/*stage 2 mode = */ new TI.Enum());
				Add(/*stage 3 mode = */ new TI.Enum());
				Add(/*advanced control = */ new TI.Block<rasterizer_screen_effect_texcoord_generation_advanced_control_block>(this, 1));
				Add(/*target = */ new TI.Enum());
				Add(new TI.Pad(2 + 64));
				Add(/*convolution = */ new TI.Block<rasterizer_screen_effect_convolution_block>(this, 2));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public screen_effect_group() : base(4)
		{
			// Explanation here
			Add(new TI.Pad(64));
			Add(/*shader = */ new TI.TagReference(this, TagGroups.shad));
			Add(new TI.Pad(64));
			Add(/*pass references = */ new TI.Block<rasterizer_screen_effect_pass_reference_block>(this, 8));
		}
		#endregion
	};
	#endregion

	#region global_wind_model_struct
	[TI.Struct((int)StructGroups.Enumerated.WNDM, 1, 160)]
	public class global_wind_model_struct : TI.Definition
	{
		#region gloal_wind_primitives_block
		[TI.Definition(1, 24)]
		public class gloal_wind_primitives_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public gloal_wind_primitives_block() : base(5)
			{
				Add(/*position = */ new TI.RealVector3D());
				Add(/*radius = */ new TI.Real());
				Add(/*strength = */ new TI.Real());
				Add(/*wind primitive type = */ new TI.Enum());
				Add(new TI.Pad(2));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public global_wind_model_struct() : base(15)
		{
			Add(/*wind tiling scale = */ new TI.Real());
			Add(/*wind primary heading/pitch/strength = */ new TI.RealVector3D());
			Add(/*primary rate of change = */ new TI.Real());
			Add(/*primary min strength = */ new TI.Real());
			Add(new TI.Pad(4 + 4 + 12));
			Add(/*wind gusting heading/pitch/strength = */ new TI.RealVector3D());
			Add(/*gust diretional rate of change = */ new TI.Real());
			Add(/*gust strength rate of change = */ new TI.Real());
			Add(/*gust cone angle = */ new TI.Real());
			Add(new TI.Pad(4 + 4 + 12 + 12 + 12 + 12));
			Add(/*turbulance rate of change = */ new TI.Real());
			Add(/*turbulence_scale x, y, z = */ new TI.RealVector3D());
			Add(/*gravity constant = */ new TI.Real());
			Add(/*wind_pirmitives = */ new TI.Block<gloal_wind_primitives_block>(this, 128));
			Add(new TI.Pad(4));
		}
		#endregion
	}
	#endregion

	#region weather_system
	[TI.TagGroup((int)TagGroups.Enumerated.weat, 1, 188)]
	public partial class weather_system_group : TI.Definition
	{
		#region global_particle_system_lite_block
		[TI.Definition(1, 156)]
		public partial class global_particle_system_lite_block : TI.Definition
		{
			#region particle_system_lite_data_block
			[TI.Definition(1, 56)]
			public partial class particle_system_lite_data_block : TI.Definition
			{
				#region particles_render_data_block
				[TI.Definition(1, 20)]
				public class particles_render_data_block : TI.Definition
				{
					#region Fields
					#endregion

					#region Ctor
					public particles_render_data_block() : base(5)
					{
						Add(/*position.x = */ new TI.Real());
						Add(/*position.y = */ new TI.Real());
						Add(/*position.z = */ new TI.Real());
						Add(/*size = */ new TI.Real());
						Add(/*color = */ new TI.Color(TI.FieldType.RgbColor));
					}
					#endregion
				}
				#endregion

				#region particles_update_data_block
				[TI.Definition(1, 32)]
				public class particles_update_data_block : TI.Definition
				{
					#region Fields
					#endregion

					#region Ctor
					public particles_update_data_block() : base(6)
					{
						Add(/*velocity.x = */ new TI.Real());
						Add(/*velocity.y = */ new TI.Real());
						Add(/*velocity.z = */ new TI.Real());
						Add(new TI.Pad(12));
						Add(/*mass = */ new TI.Real());
						Add(/*creation time stamp = */ new TI.Real());
					}
					#endregion
				}
				#endregion

				public TI.Block<particles_render_data_block> ParticlesRenderData;
				public TI.Block<particles_update_data_block> ParticlesOtherData;

				public particle_system_lite_data_block() : base(3)
				{
					Add(ParticlesRenderData = new TI.Block<particles_render_data_block>(this, 4096));
					Add(ParticlesOtherData = new TI.Block<particles_update_data_block>(this, 4096));
					Add(new TI.Pad(32));
				}
			}
			#endregion

			#region Fields
			public TI.Struct<geometry_block_info_struct> GeometryBlockInfo;
			public TI.Block<particle_system_lite_data_block> ParticleSystemData;
			#endregion

			#region Ctor
			public global_particle_system_lite_block() : base(22)
			{
				Add(/*sprites = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*view box width = */ new TI.Real());
				Add(/*view box height = */ new TI.Real());
				Add(/*view box depth = */ new TI.Real());
				Add(/*exclusion radius = */ new TI.Real());
				Add(/*max velocity = */ new TI.Real());
				Add(/*min mass = */ new TI.Real());
				Add(/*max mass = */ new TI.Real());
				Add(/*min size = */ new TI.Real());
				Add(/*max size = */ new TI.Real());
				Add(/*maximum number of particles = */ new TI.LongInteger());
				Add(/*initial velocity = */ new TI.RealVector3D());
				Add(/*bitmap animation speed = */ new TI.Real());
				Add(GeometryBlockInfo = new TI.Struct<geometry_block_info_struct>(this));
				Add(ParticleSystemData = new TI.Block<particle_system_lite_data_block>(this, 1));
				Add(/*type = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*mininum opacity = */ new TI.Real());
				Add(/*maxinum opacity = */ new TI.Real());
				Add(/*rain streak scale = */ new TI.Real());
				Add(/*rain line width = */ new TI.Real());
				Add(new TI.Pad(4 + 4 + 4));
			}
			#endregion
		}
		#endregion

		#region global_weather_background_plate_block
		[TI.Definition(1, 960)]
		public class global_weather_background_plate_block : TI.Definition
		{
			#region Fields
			#endregion

			#region Ctor
			public global_weather_background_plate_block() : base(30)
			{
				Add(/*texture 0 = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*texture 1 = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*texture 2 = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*plate positions 0 = */ new TI.Real());
				Add(/*plate positions 1 = */ new TI.Real());
				Add(/*plate positions 2 = */ new TI.Real());
				Add(/*move speed 0 = */ new TI.RealVector3D());
				Add(/*move speed 1 = */ new TI.RealVector3D());
				Add(/*move speed 2 = */ new TI.RealVector3D());
				Add(/*texture scale 0 = */ new TI.Real());
				Add(/*texture scale 1 = */ new TI.Real());
				Add(/*texture scale 2 = */ new TI.Real());
				Add(/*jitter 0 = */ new TI.RealVector3D());
				Add(/*jitter 1 = */ new TI.RealVector3D());
				Add(/*jitter 2 = */ new TI.RealVector3D());
				Add(/*plate z near = */ new TI.Real());
				Add(/*plate z far = */ new TI.Real());
				Add(/*depth blend z near = */ new TI.Real());
				Add(/*depth blend z far = */ new TI.Real());
				Add(/*opacity 0 = */ new TI.Real());
				Add(/*opacity 1 = */ new TI.Real());
				Add(/*opacity 2 = */ new TI.Real());
				Add(/*flags = */ new TI.Flags());
				Add(/*tint color0 = */ new TI.RealColor());
				Add(/*tint color1 = */ new TI.RealColor());
				Add(/*tint color2 = */ new TI.RealColor());
				Add(/*mass 1 = */ new TI.Real());
				Add(/*mass 2 = */ new TI.Real());
				Add(/*mass 3 = */ new TI.Real());
				Add(new TI.Pad(736));
			}
			#endregion
		}
		#endregion

		#region Fields
		#endregion

		#region Ctor
		public weather_system_group() : base(4)
		{
			Add(/*particle system = */ new TI.Block<global_particle_system_lite_block>(this, 1));
			Add(/*background plates = */ new TI.Block<global_weather_background_plate_block>(this, 3));
			Add(/*wind model = */ new TI.Struct<global_wind_model_struct>(this));
			Add(/*fade radius = */ new TI.Real());
		}
		#endregion
	};
	#endregion

	#region wind
	[TI.TagGroup((int)TagGroups.Enumerated.wind, 1, 64)]
	public class wind_group : TI.Definition
	{
		#region Fields
		#endregion

		#region Ctor
		public wind_group() : base(6)
		{
			Add(/*velocity = */ new TI.RealBounds());
			Add(/*variation area = */ new TI.RealEulerAngles2D());
			Add(/*local variation weight = */ new TI.Real());
			Add(/*local variation rate = */ new TI.Real());
			Add(/*damping = */ new TI.Real());
			Add(new TI.Pad(36));
		}
		#endregion
	};
	#endregion
};