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
	[TI.Struct(-1, -1, 36)]
	public partial class damage_effect_struct : TI.Definition
	{
		public TI.RealBounds Radius;
		public TI.Real CutoffScale;
		/// <remarks>not exposed for continuous_damage_effect</remarks>
		public TI.Flags Flags;
	};
	#endregion

	#region damage_camera_effect
	[TI.Struct(-1, -1, 88)]
	public partial class damage_camera_effect_struct : TI.Definition
	{
		/// <remarks>not exposed for continuous_damage_effect</remarks>
		public TI.Real ShakeDuration;
		/// <remarks>not exposed for continuous_damage_effect</remarks>
		public TI.Enum ShakeFalloffFunction;
		public TI.Real RandomTranslation;
		public TI.Real RandomRotation;
		public TI.Enum WobbleFunction;
		public TI.Real WobblePeriod, WobbleWeight;
	};
	#endregion

	#region damage_breaking_effect
	[TI.Struct(-1, -1, damage_breaking_effect_struct.kSizeOf)]
	public partial class damage_breaking_effect_struct : TI.Definition
	{
		internal const int kSizeOf = 160;

		public TI.Real ForwardVelocity, ForwardRadius, ForwardExponent;
		public TI.Real OutwardVelocity, OutwardRadius, OutwardExponent;
	};
	#endregion

	#region damage_data
	[TI.Struct(-1, -1, 60)]
	public partial class damage_data_struct : TI.Definition
	{
		public TI.Enum SideEffect, Category;
		public TI.Flags Flags;
		/// <remarks>not exposed for continuous_damage_effect</remarks>
		public TI.Real AOECoreRadius;
		public TI.Real DamageLowerBound;
		public TI.RealBounds DamageUpperBound;
		public TI.Real VehiclePassThroughPenalty;
		/// <remarks>not exposed for continuous_damage_effect</remarks>
		public TI.Real ActiveCamoDamage;
		public TI.Real Stun, MaxStun, StunTime;
		public TI.Real InstantaneousAcceleration;
	};
	#endregion

	#region damage_modifiers
	[TI.Struct(-1, -1, 160)]
	public partial class damage_modifiers_struct : TI.Definition
	{
		public TI.Real[] DamageModifiers;
	};
	#endregion


	#region continuous_damage_effect
	[TI.TagGroup((int)TagGroups.Enumerated.cdmg, 1, 512)]
	public partial class continuous_damage_effect_group : TI.Definition
	{
		public TI.Struct<damage_effect_struct> DamageEffect;
		
		public TI.Real LowFreq, HighFreq;

		public TI.Struct<damage_camera_effect_struct> DamageCameraEffect;
		public TI.Struct<damage_data_struct> DamageData;
		public TI.Struct<damage_modifiers_struct> DamageModifiers;
	};
	#endregion

	#region damage_effect
	[TI.TagGroup((int)TagGroups.Enumerated.jpt_, 6, 672)]
	public partial class damage_effect_group : TI.Definition
	{
		public TI.Struct<damage_effect_struct> DamageEffect;

		public TI.Enum ScreenFlashType, ScreenFlashPriority;
		public TI.Real ScreenFlashDuration;
		public TI.Enum ScreenFlashFadeFunction;
		public TI.Real ScreenFlashMaxIntensity;
		public TI.RealColor ScreenFlashColor;

		public TI.Real RumbleLowFreq;
		public TI.Real RumbleLowDuration;
		public TI.Enum RumbleLowFadeFunction;

		public TI.Real RumbleHighFreq;
		public TI.Real RumbleHighDuration;
		public TI.Enum RumbleHighFadeFunction;

		public TI.Real CameraImpulseDuration;
		public TI.Enum CameraImpulseFadeFunction;
		public TI.Real CameraImpulseRotation;
		public TI.Real CameraImpulsePushback;
		public TI.RealBounds CameraImpulseJitter;

		public TI.Real PermCameraImpulseAngle;

		public TI.Struct<damage_camera_effect_struct> DamageCameraEffect;

		public TI.TagReference Sound;

		public TI.Struct<damage_breaking_effect_struct> DamageBreakingEffect;
		public TI.Struct<damage_data_struct> DamageData;
		public TI.Struct<damage_modifiers_struct> DamageModifiers;
	};
	#endregion

	#region decal
	[TI.TagGroup((int)TagGroups.Enumerated.deca, 1, 268)]
	public partial class decal_group : TI.Definition
	{
		public TI.Flags Flags;
		public TI.Enum Type;
		public TI.Enum Layer;
		public TI.TagReference NextDecalInChain;

		public TI.RealBounds Radius;
		public TI.RealBounds Intensity;
		public TI.RealColor ColorLowerBounds, ColorUpperBounds;

		public TI.ShortInteger AnimLoopFrame, AnimSpeed;
		public TI.RealBounds Lifetime, DecayTime;

		public TI.Enum FramebufferBlendFunction;
		public TI.TagReference Map;

		public TI.Real MaxSpriteExtent;
	};
	#endregion

	#region detail_object_collection
	[TI.TagGroup((int)TagGroups.Enumerated.dobc, 1, 128)]
	public partial class detail_object_collection_group : TI.Definition
	{
		#region detail_object_type_block
		[TI.Definition(-1, 96)]
		public partial class detail_object_type_block : TI.Definition
		{
			public TI.String Name;
			public TI.ByteInteger SequenceIndex;
			public TI.Flags TypeFlags;
			public TI.Real ColorOverrideFactor;
			public TI.Real NearFadeDist, FarFadeDist, Size;
			public TI.RealColor MinColor, MaxColor;
			public TI.Color AmbientColor;
		};
		#endregion

		public TI.Enum CollectionType;
		public TI.Real GlobalZOffset;
		public TI.TagReference SpritePlate;
		public TI.Block<detail_object_type_block> Types;
	};
	#endregion

	#region effect
	[TI.TagGroup((int)TagGroups.Enumerated.effe, 4, 64)]
	public partial class effect_group : TI.Definition
	{
		// effect_locations_block, field_block<TI.String>

		#region effect_event_block
		[TI.Definition(-1, 68)]
		public partial class effect_event_block : TI.Definition
		{
			#region effect_part_block
			[TI.Definition(-1, 104)]
			public partial class effect_part_block : TI.Definition
			{
				public TI.Enum CreatIn1, CreatIn2;
				public TI.BlockIndex Location;
				public TI.Flags Flags;
				public TI.TagReference Type;
				public TI.RealBounds VelocityBounds;
				public TI.Real VelocityConeAngle;
				public TI.RealBounds AngularVelocityBounds;
				public TI.RealBounds RadiusModifierBounds;
				public TI.Flags ScaleModiferA, ScaleModifierB;
			};
			#endregion

			#region effect_particles_block
			[TI.Definition(-1, 232)]
			public partial class effect_particles_block : TI.Definition
			{
				public TI.Enum CreatIn1, CreatIn2, Create;
				public TI.BlockIndex Location;
				public TI.RealEulerAngles2D RelativeDirection;
				public TI.RealVector3D RelativeOffset;
				public TI.TagReference ParticleType;
				public TI.Flags Flags;
				public TI.Enum DistributionFunction;
				public TI.ShortIntegerBounds CreateCount;
				public TI.RealBounds DistributionRadius;
				public TI.RealBounds VelocityBounds;
				public TI.Real VelocityConeAngle;
				public TI.RealBounds AngularVelocityBounds;
				public TI.RealBounds Radius;
				public TI.RealColor TintLowerBound, TintUpperBound;
				public TI.Flags ScaleModiferA, ScaleModifierB;
			};
			#endregion

			public TI.Real SkipFraction;
			public TI.RealBounds DelayBounds, DurationBounds;
			public TI.Block<effect_part_block> Parts;
			public TI.Block<effect_particles_block> Particles;
		};
		#endregion

		public TI.Flags Flags;
		public TI.BlockIndex LoopStartEvent, LoopStopEvent;
		public TI.Block<field_block<TI.String>> Locations;
		public TI.Block<effect_event_block> Events;
	};
	#endregion

	#region fog
	[TI.TagGroup((int)TagGroups.Enumerated.fog_, 1, 396)]
	public partial class fog_group : TI.Definition
	{
		public TI.Flags Flags;
		public TI.Real MaxDensity;
		public TI.Real OpaqueDist, OpaqueDepth;
		public TI.Real DistToWaterPlane;

		public TI.RealColor Color;

		public TI.Flags ScreenLayerFlags;
		public TI.ShortInteger LayerCount;
		public TI.RealBounds DistGradient;
		public TI.RealBounds DensityGradient;
		public TI.Real StartDistFromFogPlane;
		public TI.Color ScreenLayerColor;
		public TI.Real RotationMul, StrafingMul, ZoomMul;
		public TI.Real MapScale;
		public TI.TagReference Map;

		public TI.Real AnimPeriod;
		public TI.RealBounds WindVelocity, WindPeriod;
		public TI.Real WindAccelerationWeight, WindPerpendicularWeight;

		public TI.TagReference BackgroundSound, SoundEnvironment;
	};
	#endregion

	#region lens_flare
	[TI.TagGroup((int)TagGroups.Enumerated.lens, 2, 240)]
	public partial class lens_flare_group : TI.Definition
	{
		#region lens_flare_reflection_block
		[TI.Definition(-1, 128)]
		public partial class lens_flare_reflection_block : TI.Definition
		{
			public TI.Flags Flags;
			public TI.ShortInteger BitmapIndex;
			public TI.Real Position, RotationOffset;
			public TI.RealBounds Radius;
			public TI.Enum RadiusScaledBy;
			public TI.RealBounds Brightness;
			public TI.Enum BrightnessScaledBy;
			public TI.RealColor TintColor;
			public TI.RealColor ColorLowerBound, ColorUpperBound;
			public TI.Flags AnimFlags;
			public TI.Enum AnimFunction;
			public TI.Real AnimPeriod, AnimPhase;
		};
		#endregion

		public TI.Real FalloffAngle, CutoffAngle;
		public TI.Real OcclusionRadius;
		public TI.Enum OcclusionOffsetDirection;
		public TI.Real NearFadeDist, FarFadeDist;
		public TI.TagReference Bitmap;
		public TI.Flags Flags;
		public TI.Enum RotationFunction;
		public TI.Real RotationFunctionScale;
		public TI.Real HorizScale, VertScale;

		public TI.Block<lens_flare_reflection_block> Reflections;
	};
	#endregion

	#region material_effects
	[TI.TagGroup((int)TagGroups.Enumerated.foot, 1, 140)]
	public partial class material_effects_group : TI.Definition
	{
		#region material_effect_block
		[TI.Definition(-1, 28)]
		public partial class material_effect_block : TI.Definition
		{
			#region material_effect_material_block
			[TI.Definition(-1, 48)]
			public partial class material_effect_material_block : TI.Definition
			{
				public TI.TagReference Effect, Sound;
			};
			#endregion

			public TI.Block<material_effect_material_block> Materials;
		};
		#endregion

		public TI.Block<material_effect_block> Effects;
	};
	#endregion

	#region particle
	[TI.TagGroup((int)TagGroups.Enumerated.part, 2, 356)]
	public partial class particle_group : TI.Definition
	{
		public TI.Flags Flags;
		public TI.TagReference Bitmap, Physics, Effects;
		public TI.RealBounds Lifespan;
		public TI.Real FadeInTime, FadeOutTime;
		public TI.TagReference CollisionEffect, DeathEffect;
		public TI.Real MinSize;
		public TI.RealBounds RadiusAnim, AnimRate;
		public TI.Real ContactDeterioration, FadeStartSize, FadeEndSize;
		public TI.ShortInteger FirstSeqIndex, InitialSeqCount, LoopSeqCount, FinalSeqCount;
		public TI.Enum Orientation;

		public TI.Struct<shader_map_struct> ShaderMap;
	};
	#endregion

	#region particle_system
	[TI.TagGroup((int)TagGroups.Enumerated.pctl, 4, 104)]
	public partial class particle_system_group : TI.Definition
	{
		#region particle_system_physics_constants_block
		[TI.Definition(-1, 4)]
		public partial class particle_system_physics_constants_block : TI.Definition
		{
		};
		#endregion

		#region particle_system_types_block
		[TI.Definition(-1, 128)]
		public partial class particle_system_types_block : TI.Definition
		{
			#region particle_system_type_states_block
			[TI.Definition(-1, 192)]
			public partial class particle_system_type_states_block : TI.Definition
			{
			};
			#endregion

			#region particle_system_type_particle_states_block
			[TI.Definition(-1, 376)]
			public partial class particle_system_type_particle_states_block : TI.Definition
			{
			};
			#endregion
		};
		#endregion
	};
	#endregion

	#region weather_particle_system
	[TI.TagGroup((int)TagGroups.Enumerated.rain, 1, 48)]
	public partial class weather_particle_system_group : TI.Definition
	{
		#region weather_particle_type_block
		[TI.Definition(-1, 604)]
		public partial class weather_particle_type_block : TI.Definition
		{
		};
		#endregion
	};
	#endregion

	#region wind
	[TI.TagGroup((int)TagGroups.Enumerated.wind, 1, 64)]
	public partial class wind_group : TI.Definition
	{
		public TI.RealBounds Velocity;
		public TI.RealEulerAngles2D VariationArea;
		public TI.Real LocalVariationWeight, LocalVariationRate, Damping;
	};
	#endregion
}