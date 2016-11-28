/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo1.Tags
{
	#region damage_effect
	partial class damage_effect_struct
	{
		public damage_effect_struct() : base(4)
		{
			Add(Radius = new TI.RealBounds());
			Add(CutoffScale = new TI.Real(TI.FieldType.RealFraction));
			Add(Flags = new TI.Flags()); // not exposed for continuous_damage_effect
			Add(new TI.Pad(20));
		}
	};
	#endregion

	#region damage_camera_effect
	partial class damage_camera_effect_struct
	{
		public damage_camera_effect_struct() : base(12)
		{
			Add(new TI.Pad(4 + 12));
			Add(ShakeDuration = new TI.Real());
			Add(ShakeFalloffFunction = new TI.Enum());
			Add(TI.Pad.Word);
			Add(RandomTranslation = new TI.Real());
			Add(RandomRotation = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(new TI.Pad(12));
			Add(WobbleFunction = new TI.Enum());
			Add(TI.Pad.Word);
			Add(WobblePeriod = new TI.Real());
			Add(WobbleWeight = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(new TI.Pad(4 + 20 + 8));
		}
	};
	#endregion

	#region damage_breaking_effect
	partial class damage_breaking_effect_struct
	{
		public damage_breaking_effect_struct() : base(9)
		{
			Add(new TI.Pad(112));
			Add(ForwardVelocity = new TI.Real());
			Add(ForwardRadius = new TI.Real());
			Add(ForwardExponent = new TI.Real());
			Add(new TI.Pad(12));
			Add(OutwardVelocity = new TI.Real());
			Add(OutwardRadius = new TI.Real());
			Add(OutwardExponent = new TI.Real());
			Add(new TI.Pad(12));
		}
	};
	#endregion

	#region damage_data
	partial class damage_data_struct
	{
		public damage_data_struct() : base(14)
		{
			Add(SideEffect = new TI.Enum());
			Add(Category = new TI.Enum());
			Add(Flags = new TI.Flags());
			Add(AOECoreRadius = new TI.Real());	// not exposed for continuous_damage_effect
			Add(DamageLowerBound = new TI.Real());
			Add(DamageUpperBound = new TI.RealBounds());
			Add(VehiclePassThroughPenalty = new TI.Real());
			Add(ActiveCamoDamage = new TI.Real()); // not exposed for continuous_damage_effect
			Add(Stun = new TI.Real());
			Add(MaxStun = new TI.Real());
			Add(StunTime = new TI.Real());
			Add(TI.Pad.DWord);
			Add(InstantaneousAcceleration = new TI.Real());
			Add(new TI.Pad(4 + 4)); // useless padding
		}
	};
	#endregion

	#region damage_modifiers
	partial class damage_modifiers_struct
	{
		public damage_modifiers_struct() : base(34)
		{
			DamageModifiers = new TI.Real[33];
			for (int x = 0; x < DamageModifiers.Length; x++)
				Add(DamageModifiers[x] = new TI.Real());
			Add(new TI.Pad(28));
		}
	};
	#endregion


	#region continuous_damage_effect
	public partial class continuous_damage_effect_group
	{
		public continuous_damage_effect_group() : base(7)
		{
			Add(DamageEffect = new TI.Struct<damage_effect_struct>(this));

			Add(LowFreq = new TI.Real(TI.FieldType.RealFraction));
			Add(HighFreq = new TI.Real(TI.FieldType.RealFraction));

			Add(DamageCameraEffect = new TI.Struct<damage_camera_effect_struct>(this));
			Add(new TI.Pad(damage_breaking_effect_struct.kSizeOf));
			Add(DamageData = new TI.Struct<damage_data_struct>(this));
			Add(DamageModifiers = new TI.Struct<damage_modifiers_struct>(this));
		}
	};
	#endregion

	#region damage_effect
	public partial class damage_effect_group
	{
		public damage_effect_group() : base(31)
		{
			Add(DamageEffect = new TI.Struct<damage_effect_struct>(this));

			Add(ScreenFlashType = new TI.Enum());
			Add(ScreenFlashPriority = new TI.Enum());
			Add(new TI.Pad(12));
			Add(ScreenFlashDuration = new TI.Real());
			Add(ScreenFlashFadeFunction = new TI.Enum());
			Add(new TI.Pad(2 + 8));
			Add(ScreenFlashMaxIntensity = new TI.Real(TI.FieldType.RealFraction));
			Add(new TI.Pad(4));
			Add(ScreenFlashColor = new TI.RealColor(BlamLib.TagInterface.FieldType.RealArgbColor));

			Add(RumbleLowFreq = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(RumbleLowDuration = new TI.Real());
			Add(RumbleLowFadeFunction = new TI.Enum());
			Add(new TI.Pad(2 + 8));

			Add(RumbleHighFreq = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(RumbleHighDuration = new TI.Real());
			Add(RumbleHighFadeFunction = new TI.Enum());
			Add(new TI.Pad(2 + 8 + 4 + 16));

			Add(CameraImpulseDuration = new TI.Real());
			Add(CameraImpulseFadeFunction = new TI.Enum());
			Add(new TI.Pad(2));
			Add(CameraImpulseRotation = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(CameraImpulsePushback = new TI.Real());
			Add(CameraImpulseJitter = new TI.RealBounds());
			Add(new TI.Pad(4 + 4));

			Add(PermCameraImpulseAngle = new TI.Real(BlamLib.TagInterface.FieldType.Angle));

			Add(DamageCameraEffect = new TI.Struct<damage_camera_effect_struct>(this));

			Add(Sound = new TI.TagReference(this, TagGroups.snd_));

			Add(DamageBreakingEffect = new TI.Struct<damage_breaking_effect_struct>(this));
			Add(DamageData = new TI.Struct<damage_data_struct>(this));
			Add(DamageModifiers = new TI.Struct<damage_modifiers_struct>(this));
		}
	};
	#endregion

	#region decal
	public partial class decal_group
	{
		public decal_group() : base(23)
		{
			Add(Flags = new TI.Flags(TI.FieldType.WordFlags));
			Add(Type = new TI.Enum());
			Add(Layer = new TI.Enum());
			Add(new TI.Pad(2));
			Add(NextDecalInChain = new TI.TagReference(this, TagGroups.deca));

			Add(Radius = new TI.RealBounds());
			Add(new TI.Pad(12));
			Add(Intensity = new TI.RealBounds(TI.FieldType.RealFractionBounds));
			Add(ColorLowerBounds = new TI.RealColor());
			Add(ColorUpperBounds = new TI.RealColor());
			Add(new TI.Pad(12));
			
			Add(AnimLoopFrame = new TI.ShortInteger());
			Add(AnimSpeed = new TI.ShortInteger());
			Add(new TI.Pad(28));
			Add(Lifetime = new TI.RealBounds());
			Add(DecayTime = new TI.RealBounds());
			Add(new TI.Pad(12 + 40 + 2 + 2));

			Add(FramebufferBlendFunction = new TI.Enum());
			Add(new TI.Pad(2 + 20));
			Add(Map = new TI.TagReference(this, TagGroups.bitm));
			Add(new TI.Pad(20));
			Add(MaxSpriteExtent = new TI.Real());
			Add(new TI.Pad(4 + 8));
		}
	};
	#endregion

	#region detail_object_collection
	public partial class detail_object_collection_group
	{
		#region detail_object_type_block
		public partial class detail_object_type_block
		{
			public detail_object_type_block() : base(14)
			{
				Add(Name = new TI.String());
				Add(SequenceIndex = new TI.ByteInteger());
				Add(TypeFlags = new TI.Flags(BlamLib.TagInterface.FieldType.ByteFlags));
				Add(new TI.Pad(2));
				Add(ColorOverrideFactor = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
				Add(new TI.Pad(8));
				Add(NearFadeDist = new TI.Real());
				Add(FarFadeDist = new TI.Real());
				Add(Size = new TI.Real());
				Add(new TI.Pad(4));
				Add(MinColor = new TI.RealColor());
				Add(MaxColor = new TI.RealColor());
				Add(AmbientColor = new TI.Color());
				Add(new TI.Pad(4));
			}
		};
		#endregion

		public detail_object_collection_group() : base(7)
		{
			Add(CollectionType = new TI.Enum());
			Add(new TI.Pad(2));
			Add(GlobalZOffset = new TI.Real());
			Add(new TI.Pad(44));
			Add(SpritePlate = new TI.TagReference(this, TagGroups.bitm));
			Add(Types = new TI.Block<detail_object_type_block>(this, 16));
			Add(new TI.Pad(48));
		}
	};
	#endregion

	#region effect
	public partial class effect_group
	{
		#region effect_event_block
		public partial class effect_event_block
		{
			#region effect_part_block
			public partial class effect_part_block
			{
				public effect_part_block() : base(14)
				{
					Add(CreatIn1 = new TI.Enum());
					Add(CreatIn2 = new TI.Enum());
					Add(Location = new TI.BlockIndex());
					Add(Flags = new TI.Flags(TI.FieldType.WordFlags));
					Add(new TI.Pad(16));
					Add(Type = new TI.TagReference(this));
					Add(new TI.Pad(24));
					Add(VelocityBounds = new TI.RealBounds());
					Add(VelocityConeAngle = new TI.Real(TI.FieldType.Angle));
					Add(AngularVelocityBounds = new TI.RealBounds(TI.FieldType.AngleBounds));
					Add(RadiusModifierBounds = new TI.RealBounds());
					Add(new TI.Pad(4));
					Add(ScaleModiferA = new TI.Flags());
					Add(ScaleModifierB = new TI.Flags());
				}
			};
			#endregion

			#region effect_particles_block
			public partial class effect_particles_block
			{
				public effect_particles_block() : base(27)
				{
					Add(CreatIn1 = new TI.Enum());
					Add(CreatIn2 = new TI.Enum());
					Add(Create = new TI.Enum());
					Add(new TI.Pad(2));
					Add(Location = new TI.BlockIndex());
					Add(new TI.Pad(2));
					Add(RelativeDirection = new TI.RealEulerAngles2D());
					Add(RelativeOffset = new TI.RealVector3D());
					Add(new TI.Pad(12 + 40));
					Add(ParticleType = new TI.TagReference(this, TagGroups.part));
					Add(Flags = new TI.Flags());
					Add(DistributionFunction = new TI.Enum());
					Add(new TI.Pad(2));
					Add(CreateCount = new TI.ShortIntegerBounds());
					Add(DistributionRadius = new TI.RealBounds());
					Add(new TI.Pad(12));
					Add(VelocityBounds = new TI.RealBounds());
					Add(VelocityConeAngle = new TI.Real(TI.FieldType.Angle));
					Add(AngularVelocityBounds = new TI.RealBounds(TI.FieldType.AngleBounds));
					Add(new TI.Pad(8));
					Add(Radius = new TI.RealBounds());
					Add(new TI.Pad(8));
					Add(TintLowerBound = new TI.RealColor(TI.FieldType.RealArgbColor));
					Add(TintUpperBound = new TI.RealColor(TI.FieldType.RealArgbColor));
					Add(new TI.Pad(16));
					Add(ScaleModiferA = new TI.Flags());
					Add(ScaleModifierB = new TI.Flags());
				}
			};
			#endregion

			public effect_event_block() : base(7)
			{
				Add(new TI.Pad(4));
				Add(SkipFraction = new TI.Real(TI.FieldType.RealFraction));
				Add(DelayBounds = new TI.RealBounds());
				Add(DurationBounds = new TI.RealBounds());
				Add(new TI.Pad(20));
				Add(Parts = new TI.Block<effect_part_block>(this, 32));
				Add(Particles = new TI.Block<effect_particles_block>(this, 32));
			}
		};
		#endregion

		public effect_group() : base(6)
		{
			Add(Flags = new TI.Flags());
			Add(LoopStartEvent = new TI.BlockIndex());
			Add(LoopStopEvent = new TI.BlockIndex());
			Add(new TI.Pad(32));
			Add(Locations = new TI.Block<field_block<TI.String>>(this, 32));
			Add(Events = new TI.Block<effect_event_block>(this, 32));
		}
	};
	#endregion

	#region fog
	public partial class fog_group
	{
		public fog_group() : base(33)
		{
			Add(Flags = new TI.Flags());
			Add(new TI.Pad(4 + 76 + 4));
			Add(MaxDensity = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(new TI.Pad(4));
			Add(OpaqueDist = new TI.Real());
			Add(new TI.Pad(4));
			Add(OpaqueDepth = new TI.Real());
			Add(new TI.Pad(8));
			Add(DistToWaterPlane = new TI.Real());

			Add(Color = new TI.RealColor());

			Add(ScreenLayerFlags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
			Add(LayerCount = new TI.ShortInteger());
			Add(DistGradient = new TI.RealBounds());
			Add(DensityGradient = new TI.RealBounds(BlamLib.TagInterface.FieldType.RealFractionBounds));
			Add(StartDistFromFogPlane = new TI.Real());
			Add(new TI.Pad(4));
			Add(ScreenLayerColor = new TI.Color(BlamLib.TagInterface.FieldType.RgbColor));
			Add(RotationMul = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(StrafingMul = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(ZoomMul = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(new TI.Pad(8));
			Add(MapScale = new TI.Real());
			Add(Map = new TI.TagReference(this, TagGroups.bitm));

			Add(AnimPeriod = new TI.Real());
			Add(new TI.Pad(4));
			Add(WindVelocity = new TI.RealBounds());
			Add(WindPeriod = new TI.RealBounds());
			Add(WindAccelerationWeight = new TI.Real());
			Add(WindPerpendicularWeight = new TI.Real());
			Add(new TI.Pad(8));
			Add(BackgroundSound = new TI.TagReference(this, TagGroups.snd_));
			Add(SoundEnvironment = new TI.TagReference(this, TagGroups.snde));
			Add(new TI.Pad(120));
		}
	};
	#endregion

	#region lens_flare
	public partial class lens_flare_group
	{
		#region lens_flare_reflection_block
		public partial class lens_flare_reflection_block
		{
			public lens_flare_reflection_block() : base(21)
			{
				Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
				Add(new TI.Pad(2));
				Add(BitmapIndex = new TI.ShortInteger());
				Add(new TI.Pad(2 + 20));
				Add(Position = new TI.Real());
				Add(RotationOffset = new TI.Real());
				Add(new TI.Pad(4));
				Add(Radius = new TI.RealBounds());
				Add(RadiusScaledBy = new TI.Enum());
				Add(new TI.Pad(2));
				Add(Brightness = new TI.RealBounds());
				Add(BrightnessScaledBy = new TI.Enum());
				Add(new TI.Pad(2));
				Add(TintColor = new TI.RealColor(BlamLib.TagInterface.FieldType.RealArgbColor));
				Add(ColorLowerBound = new TI.RealColor(BlamLib.TagInterface.FieldType.RealArgbColor));
				Add(ColorUpperBound = new TI.RealColor(BlamLib.TagInterface.FieldType.RealArgbColor));
				Add(AnimFlags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
				Add(AnimFunction = new TI.Enum());
				Add(AnimPeriod = new TI.Real());
				Add(AnimPhase = new TI.Real());
				Add(new TI.Pad(4));
			}
		};
		#endregion

		public lens_flare_group() : base(20)
		{
			Add(FalloffAngle = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(CutoffAngle = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(new TI.Pad(8));
			Add(OcclusionRadius = new TI.Real());
			Add(OcclusionOffsetDirection = new TI.Enum());
			Add(new TI.Pad(2));
			Add(NearFadeDist = new TI.Real());
			Add(FarFadeDist = new TI.Real());
			Add(Bitmap = new TI.TagReference(this, TagGroups.bitm));
			Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
			Add(new TI.Pad(2 + 76));
			Add(RotationFunction = new TI.Enum());
			Add(new TI.Pad(2));
			Add(RotationFunctionScale = new TI.Real());
			Add(new TI.Pad(24));
			Add(HorizScale = new TI.Real());
			Add(VertScale = new TI.Real());
			Add(new TI.Pad(28));
			Add(Reflections = new TI.Block<lens_flare_reflection_block>(this, 32));
			Add(new TI.Pad(32));
		}
	};
	#endregion

	#region material_effects
	public partial class material_effects_group
	{
		#region material_effect_block
		public partial class material_effect_block
		{
			#region material_effect_material_block
			public partial class material_effect_material_block
			{
				public material_effect_material_block() : base(3)
				{
					Add(Effect = new TI.TagReference(this, TagGroups.effe));
					Add(Sound = new TI.TagReference(this, TagGroups.snd_));
					Add(new TI.Pad(16));
				}
			};
			#endregion

			public material_effect_block() : base(2)
			{
				Add(Materials = new TI.Block<material_effect_material_block>(this, 33));
				Add(new TI.Pad(16));
			}
		};
		#endregion

		public material_effects_group() : base(2)
		{
			Add(Effects = new TI.Block<material_effect_block>(this, 13));
			Add(new TI.Pad(128));
		}
	};
	#endregion

	#region particle
	public partial class particle_group
	{
		public particle_group() : base(27)
		{
			Add(Flags = new TI.Flags());
			Add(Bitmap = new TI.TagReference(this, TagGroups.bitm));
			Add(Physics = new TI.TagReference(this, TagGroups.pphy));
			Add(Effects = new TI.TagReference(this, TagGroups.foot));
			Add(new TI.Pad(4));
			Add(Lifespan = new TI.RealBounds());
			Add(FadeInTime = new TI.Real());
			Add(FadeOutTime = new TI.Real());
			Add(CollisionEffect = new TI.TagReference(this)); // snd!,effe
			Add(DeathEffect = new TI.TagReference(this)); // snd!,effe
			Add(MinSize = new TI.Real());
			Add(new TI.Pad(8));
			Add(RadiusAnim = new TI.RealBounds());
			Add(new TI.Pad(4));
			Add(AnimRate = new TI.RealBounds());
			Add(ContactDeterioration = new TI.Real());
			Add(FadeStartSize = new TI.Real());
			Add(FadeEndSize = new TI.Real());
			Add(new TI.Pad(4));
			Add(FirstSeqIndex = new TI.ShortInteger());
			Add(InitialSeqCount = new TI.ShortInteger());
			Add(LoopSeqCount = new TI.ShortInteger());
			Add(FinalSeqCount = new TI.ShortInteger());
			Add(new TI.Pad(12));
			Add(Orientation = new TI.Enum());
			Add(new TI.Pad(2));

			Add(ShaderMap = new TI.Struct<shader_map_struct>(this));
		}
	};
	#endregion

	#region particle_system
	public partial class particle_system_group
	{
		#region particle_system_physics_constants_block
		public partial class particle_system_physics_constants_block
		{
			public particle_system_physics_constants_block() : base(1)
			{
				Add(/*The meaning of this constant depends on the selected physics creation/update function. = */ new TI.Real());
			}
		};
		#endregion

		#region particle_system_types_block
		public partial class particle_system_types_block // TODO: [postprocess]
		{
			#region particle_system_type_states_block
			public partial class particle_system_type_states_block // TODO: [postprocess]
			{
				public particle_system_type_states_block() : base(15)
				{
					Add(/*name = */ new TI.String());
					Add(/*duration bounds = */ new TI.RealBounds());
					Add(/*transition time bounds = */ new TI.RealBounds());
					Add(new TI.Pad(4));
					Add(/*scale multiplier = */ new TI.Real());
					Add(/*animation_rate_multiplier = */ new TI.Real());
					Add(/*rotation rate multiplier = */ new TI.Real());
					Add(/*color multiplier = */ new TI.RealColor(TI.FieldType.RealArgbColor));
					Add(/*radius multiplier = */ new TI.Real());
					Add(/*minimum particle count = */ new TI.Real());
					Add(/*particle creation rate = */ new TI.Real());
					Add(new TI.Pad(84));
					Add(/*particle creation physics = */ new TI.Enum());
					Add(/*particle update physics = */ new TI.Enum());
					Add(/*physics constants = */ new TI.Block<particle_system_physics_constants_block>(this, 16));
				}
			};
			#endregion

			#region particle_system_type_particle_states_block
			public partial class particle_system_type_particle_states_block // TODO: [postprocess]
			{
				public particle_system_type_particle_states_block() : base(16)
				{
					Add(/*name = */ new TI.String());
					Add(/*duration bounds = */ new TI.RealBounds());
					Add(/*transition time bounds = */ new TI.RealBounds());
					Add(/*bitmaps = */ new TI.TagReference(this, TagGroups.bitm));
					Add(/*sequence index = */ new TI.ShortInteger());
					Add(new TI.Pad(2 + 4));
					Add(/*scale = */ new TI.RealBounds());
					Add(/*animation rate = */ new TI.RealBounds());
					Add(/*rotation rate = */ new TI.RealBounds(TI.FieldType.AngleBounds));
					Add(/*color 1 = */ new TI.RealColor(TI.FieldType.RealArgbColor));
					Add(/*color 2 = */ new TI.RealColor(TI.FieldType.RealArgbColor));
					Add(/*radius multiplier = */ new TI.Real());
					Add(/*point physics = */ new TI.TagReference(this, TagGroups.pphy));
					Add(new TI.Pad(36));

					Add(/*shader map = */ new TI.Struct<shader_map_struct>(this));

					Add(/*physics constants = */ new TI.Block<particle_system_physics_constants_block>(this, 16));
				}
			};
			#endregion

			public particle_system_types_block() : base(14)
			{
				Add(/*name = */ new TI.String());
				Add(/*flags = */ new TI.Flags());
				Add(/*initial particle count = */ new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(/*complex sprite render modes = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*radius = */ new TI.Real());
				Add(new TI.Pad(36));
				Add(/*particle creation physics = */ new TI.Enum());
				Add(new TI.Pad(2));
				Add(/*physics flags = */ new TI.Flags());
				Add(/*physics constants = */ new TI.Block<particle_system_physics_constants_block>(this, 16));
				Add(/*states = */ new TI.Block<particle_system_type_states_block>(this, 8));
				Add(/*particle states = */ new TI.Block<particle_system_type_particle_states_block>(this, 8));
			}
		};
		#endregion

		public particle_system_group() : base(7)
		{
			Add(new TI.Pad(4 + 52));
			Add(/*point physics = */ new TI.TagReference(this, TagGroups.pphy));
			Add(/*system update physics = */ new TI.Enum());
			Add(new TI.Pad(2));
			Add(/*physics flags = */ new TI.Flags());
			Add(/*physics constants = */ new TI.Block<particle_system_physics_constants_block>(this, 16));
			Add(/*particle types = */ new TI.Block<particle_system_types_block>(this, 4));
		}
	};
	#endregion

	#region weather_particle_system
	public partial class weather_particle_system_group
	{
		#region weather_particle_type_block
		public partial class weather_particle_type_block
		{
			public weather_particle_type_block() : base(29)
			{
				Add(/*name = */ new TI.String());
				Add(/*flags = */ new TI.Flags());
				Add(/*fade-in start distance = */ new TI.Real()); // TODO: maybe make these real bounds fields
				Add(/*fade-in end distance = */ new TI.Real());
				Add(/*fade-out start distance = */ new TI.Real());
				Add(/*fade-out end distance = */ new TI.Real());
				Add(/*fade-in start height = */ new TI.Real());
				Add(/*fade-in end height = */ new TI.Real());
				Add(/*fade-out start height = */ new TI.Real());
				Add(/*fade-out end height = */ new TI.Real());
				Add(new TI.Pad(96));
				Add(/*particle count = */ new TI.RealBounds());
				Add(/*physics = */ new TI.TagReference(this, TagGroups.pphy));
				Add(new TI.Pad(16));
				Add(/*acceleration magnitude = */ new TI.RealBounds());
				Add(/*acceleration turning rate = */ new TI.Real(TI.FieldType.RealFraction));
				Add(/*acceleration change rate = */ new TI.Real());
				Add(new TI.Pad(32));
				Add(/*particle radius = */ new TI.RealBounds());
				Add(/*animation rate = */ new TI.RealBounds());
				Add(/*rotation rate = */ new TI.RealBounds(TI.FieldType.AngleBounds));
				Add(new TI.Pad(32));
				Add(/*color lower bound = */ new TI.RealColor(TI.FieldType.RealArgbColor));
				Add(/*color upper bound = */ new TI.RealColor(TI.FieldType.RealArgbColor));
				Add(new TI.Pad(64));
				Add(/*sprite bitmap = */ new TI.TagReference(this, TagGroups.bitm));
				Add(/*render mode = */ new TI.Enum());
				Add(/*render direction source = */ new TI.Enum());

				Add(/*shader map = */ new TI.Struct<shader_map_struct>(this));
			}
		};
		#endregion

		public weather_particle_system_group() : base(3)
		{
			Add(/*flags = */ new TI.Flags());
			Add(new TI.Pad(32));
			Add(/*particle types = */ new TI.Block<weather_particle_type_block>(this, 8));
		}
	};
	#endregion

	#region wind
	public partial class wind_group
	{
		public wind_group() : base(6)
		{
			Add(Velocity = new TI.RealBounds());
			Add(VariationArea = new TI.RealEulerAngles2D());
			Add(LocalVariationWeight = new TI.Real());
			Add(LocalVariationRate = new TI.Real());
			Add(Damping = new TI.Real());
			Add(new TI.Pad(36));
		}
	};
	#endregion
}