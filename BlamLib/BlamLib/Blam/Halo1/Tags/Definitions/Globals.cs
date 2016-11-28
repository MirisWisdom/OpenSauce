/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo1.Tags
{
	#region globals_group
	[TI.TagGroup((int)TagGroups.Enumerated.matg, 3, 428)]
	public partial class globals_group : TI.Definition
	{
		// sound_block, field_block<TI.TagReference>

		// camera_block, field_block<TI.TagReference>

		#region player_control_block
		[TI.Definition(-1, 128)]
		public partial class player_control_block : TI.Definition
		{
			// look_function, field_block<TI.Real>

			public TI.Real MagnetismFriction, MagnetismAdhesion;
			public TI.Real InconsequentialTargetScale;
			public TI.Real LookAccelerationTime, LookAccelerationScale;
			public TI.Real LookPegThreshold;
			public TI.Real LookDefaultPitchRate, LookDefaultYawRate;
			public TI.Real LookAutolevelingScale;
			public TI.ShortInteger MinWeaponSwapTicks, MinAutolevelingTicks;
			public TI.Real MinAngleForVehicleFlipping;
			public TI.Block<field_block<TI.Real>> LookFunction;
		};
		#endregion

		#region difficulty_block
		[TI.Definition(-1, 644)]
		public partial class difficulty_block : TI.Definition
		{
			public TI.Real[] Health;
			public TI.Real[] RangedFire;
			public TI.Real[] Grenades;
			public TI.Real[] Placement;
		};
		#endregion

		#region grenades_block
		[TI.Definition(-1, 68)]
		public partial class grenades_block : TI.Definition
		{
			public TI.ShortInteger MaxCount;
			public TI.TagReference ThrowingEffect, HudInterface, Equipment, Projectile;
		};
		#endregion

		#region rasterizer_data_block
		[TI.Definition(-1, 428)]
		public partial class rasterizer_data_block : TI.Definition
		{
			public TI.TagReference DistanceAttenuation, VectorNormalization, AtmosphereicFogDensity,
				PlanarFogDensity, LinearCornerFade, ActiveCamoflageDistortion, Glow;

			public TI.TagReference Default2D, Default3D, DefaultCubeMap;

			public TI.TagReference Test0, Test1, Test2, Test3;

			public TI.TagReference VideoScanlineMap, VideoNoiseMap;

			public TI.Flags Flags;
			public TI.Real RefractionAmount, DistanceFalloff;
			public TI.RealColor TintColor;
			public TI.Real HyperStealthRefraction, HyperStealthDistanceFalloff;
			public TI.RealColor HyperStealthTintColor;

			public TI.TagReference DistanceAttenuation2D;
		};
		#endregion

		#region interface_tag_references_block
		[TI.Definition(-1, 304)]
		public partial class interface_tag_references_block : TI.Definition
		{
			public TI.TagReference[] Fields;
		};
		#endregion

		// cheat_weapons_block, field_block<TI.TagReference>

		// cheat_powerups_block, field_block<TI.TagReference>

		#region multiplayer_information_block
		[TI.Definition(-1, 160)]
		public partial class multiplayer_information_block : TI.Definition
		{
			// vehicles_block, field_block<TI.TagReference>

			// sounds_block, field_block<TI.TagReference>

			public TI.TagReference Flag, Unit;
			public TI.Block<field_block<TI.TagReference>> Vehicles;
			public TI.TagReference HillShader, FlagShader, Ball;
			public TI.Block<field_block<TI.TagReference>> Sounds;
		};
		#endregion

		#region player_information_block
		[TI.Definition(-1, 244)]
		public partial class player_information_block : TI.Definition
		{
			public TI.TagReference Unit;
			public TI.Real WalkingSpeed, DoubleSpeedMul, RunForward, RunBackward, RunSideways, RunAcceleration;
			public TI.Real SneakForward, SneakBackward, SneakSideways, SneakAcceleration, AirborneAcceleration;
			public TI.RealPoint3D GrenadeOrigin;
			public TI.Real StunMovementPenalty, StunTurningPenalty, StunJumpingPenalty;
			public TI.Real MinStunTime, MaxStunTime;
			public TI.RealBounds FirstPersionIdleTime;
			public TI.Real FirstPersionSkipFraction;
			public TI.TagReference CoopRespawnEffect;
		};
		#endregion

		#region first_person_interface_block
		[TI.Definition(-1, 192)]
		public partial class first_person_interface_block : TI.Definition
		{
			public TI.TagReference FirstPersionHands, BaseBitmap, ShieldMeter;
			public TI.Point2D ShieldMeterOrigin;
			public TI.TagReference BodyMeter;
			public TI.Point2D BodyMeterOrigin;
			public TI.TagReference NightVisionTurnOnEffect, NightVisionTurnOffEffect;
		};
		#endregion

		#region falling_damage_block
		[TI.Definition(-1, 152)]
		public partial class falling_damage_block : TI.Definition
		{
			public TI.RealBounds HarmfulFallingDist;
			public TI.TagReference FallingDamage;
			public TI.Real MaxFallingDist;
			public TI.TagReference DistDamage, VehicleEnvironmentCollisionDamage, VehicleKilledUnitDamageEffect, VehicleCollisionDamage, FlamingDeathDamage;
		};
		#endregion

		#region materials_block
		[TI.Definition(-1, 884)]
		public partial class materials_block : TI.Definition
		{
			#region breakable_surface_particle_effect_block
			[TI.Definition(-1, 128)]
			public partial class breakable_surface_particle_effect_block : TI.Definition
			{
				public TI.TagReference ParticleType;
				public TI.Flags Flags;
				public TI.Real Density;
				public TI.RealBounds VelocityScale;
				public TI.RealBounds AngularVelocity;
				public TI.RealBounds Radius;
				public TI.RealColor TintLowerBound, TintUpperBound;
			};
			#endregion

			public TI.Real GroundFrictionScale, GroundFrictionNormalK1Scale, GroundFrictionNormalK0Scale;
			public TI.Real GroundDepthScale, GroundDampFractionScale;

			public TI.Real MaxVitality;
			public TI.TagReference Effect, Sound;
			public TI.Block<breakable_surface_particle_effect_block> ParticleEffects;
			public TI.TagReference MeleeHitSound;
		};
		#endregion

		#region playlist_autogenerate_choice_block
		[TI.Definition(-1, 148)]
		public partial class playlist_autogenerate_choice_block : TI.Definition
		{
			public TI.String MapName, GameVariant;
			public TI.LongInteger MinExperience, MaxExperience;
			public TI.LongInteger MinPlayerCount, MaxPlayerCount;
			public TI.LongInteger Rating;
		};
		#endregion

		public TI.Block<field_block<TI.TagReference>> Sounds;
		public TI.Block<field_block<TI.TagReference>> Camera;
		public TI.Block<player_control_block> PlayerControl;
		public TI.Block<difficulty_block> Difficulty;
		public TI.Block<grenades_block> Grenades;
		public TI.Block<rasterizer_data_block> RasterizerData;
		public TI.Block<interface_tag_references_block> InterfaceBitmaps;
		public TI.Block<field_block<TI.TagReference>> WeaponList, CheatPowerups;
		public TI.Block<multiplayer_information_block> MultiplayerInformation;
		public TI.Block<player_information_block> PlayerInformation;
		public TI.Block<first_person_interface_block> FirstPersionInterface;
		public TI.Block<falling_damage_block> FallingDamage;
		public TI.Block<materials_block> Materials;
		public TI.Block<playlist_autogenerate_choice_block> PlayerlistMembers;
	};
	#endregion
}