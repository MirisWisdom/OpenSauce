/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo1.Tags
{
	#region actor_group
	[TI.TagGroup((int)TagGroups.Enumerated.actr, 2, 1272)]
	public sealed partial class actor_group : TI.Definition
	{
		#region Fields
		public TI.Flags Flags;
		public TI.Flags MoreFlags;
		public TI.Enum Type;

		#region perception
		public TI.Real MaxVisionDist;
		public TI.Real CentralVisionAngle;
		public TI.Real MaxVisionAngle;
		public TI.Real PeripheralVisionAngle;
		public TI.Real PeripheralDist;
		public TI.RealVector3D StandingGunOffset;
		public TI.RealVector3D CrouchingGunOffset;
		public TI.Real HearingDist;
		public TI.Real NoticeProjectileChance;
		public TI.Real NoticeVehicleChance;
		public TI.Real CombatPerceptionTime;
		public TI.Real GuardPerceptionTime;
		public TI.Real NonCombatPerceptionTime;
		#endregion

		#region movement
		public TI.Real DiveIntoCoverChance;
		public TI.Real EmergeFromCoverChance;
		public TI.Real DiveFromGrenadeChance;
		public TI.Real PathfindingRadius;
		public TI.Real GlassIgnoranceChance;
		public TI.Real StationaryMovementDist;
		public TI.Real FreeFlyingSidestep;
		public TI.Real BeginMovingAngle;
		#endregion

		#region looking
		public TI.RealEulerAngles2D MaxAimingDeviation, MaxLookingDeviation;
		public TI.Real NonCombatLookDeltaL, NonCombatLookDeltaR;
		public TI.Real CombatLookDeltaL, CombatLookDeltaR;
		public TI.RealEulerAngles2D IdleAimingRange, IdleLookingRange;
		public TI.RealBounds EventLookTimeModifier;
		public TI.RealBounds NonCombatIdleFacing, NonCombatIdleAiming, NonCombatIdleLooking;
		public TI.RealBounds GuardIdleFacing, GuardIdleAiming, GuardIdleLooking;
		public TI.RealBounds CombatIdleFacing, CombatIdleAiming, CombatIdleLooking;
		#endregion

		private TI.TagReference DoNotUseWeapon, DoNotUseProjectile;

		#region unopposable
		public TI.Enum UnreachableDangerTrigger, VehicleDangerTrigger, PlayerDangerTrigger;
		public TI.RealBounds DangerTriggerTime;
		public TI.ShortInteger FriendsKilledTrigger, FriendsRetreatingTrigger;
		public TI.RealBounds RetreatTime;
		#endregion

		public TI.RealBounds CoveringTime;
		public TI.Real FriendKilledPanicChance;
		public TI.Enum LeaderType;
		public TI.Real LeaderKilledPanicChance;
		public TI.Real PanicDamageThreshold;
		public TI.Real SurpriseDist;

		public TI.RealBounds HideBehindCoverTime;
		public TI.Real HideTagerNotVisibleTime;
		public TI.Real HideShieldFraction;
		public TI.Real AttackShieldFraction;
		public TI.Real PursueShieldFraction;
		public TI.Enum DefensiveCrouchType;
		public TI.Real AttackingCrouchThreshold;
		public TI.Real DefendingCrouchThreshold;
		public TI.Real MinStandTime;
		public TI.Real MinCrouchTime;
		public TI.Real DefendingHideTime;
		public TI.Real AttackingEvasionThreshold, DefendingEvasionThreshold;
		public TI.Real EvasionSeekCoverChance;
		public TI.Real EvasionDelayTime;
		public TI.Real MaxSeekCoverDist;
		public TI.Real CoverDamageThreshold;
		public TI.Real StalkingDiscoveryTime;
		public TI.Real StalkingMaxDist;
		public TI.Real StationaryFacingAngle;
		public TI.Real ChangeFacingStandTime;

		public TI.RealBounds UncoverDelayTimer;
		public TI.RealBounds TargetSearchTime;
		public TI.RealBounds PursuitPositionTime;
		public TI.ShortInteger NumPositionsCoord, NumPositionsNormal;

		public TI.Real MeleeAttackDelay, MeleeFudgeFactor, MeleeChargeTime;
		public TI.RealBounds MeleeLeapRange;
		public TI.Real MeleeLeapVelocity, MeleeLeapChance, MeleeLeapBallistic;
		public TI.Real BerserkDamageAmount, BerserkDamageThreshold;
		public TI.Real BerserkProximity;
		public TI.Real SuicideSensingDist;
		public TI.Real BerserkGrenadeChance;

		public TI.RealBounds GuardPositionTIme, CombatPositionTime;
		public TI.Real OldPositionAvoidDist, FriendAvoidDist;

		public TI.RealBounds NoncombatIdleSpeechTime, CombatIdleSpechTime;

		private TI.TagReference DoNotUseMajorUpgrade;
		#endregion
	};
	#endregion

	#region actor_variant_group
	[TI.TagGroup((int)TagGroups.Enumerated.actv, 1, 568)]
	public sealed partial class actor_variant_group : TI.Definition
	{
		#region actor_variant_change_colors_block
		[TI.Definition(-1, 32)]
		public sealed partial class actor_variant_change_colors_block : TI.Definition
		{
			#region Fields
			public TI.RealColor ColorLowerBound, ColorUpperBound;
			#endregion
		};
		#endregion

		#region Fields
		public TI.Flags Flags;
		public TI.TagReference ActorDefinition;
		public TI.TagReference Unit;
		public TI.TagReference MajorVariant;

		public TI.Enum MovementType;
		public TI.Real InitialCrouchChance;
		public TI.RealBounds CrouchTime, RunTime;

		public TI.TagReference Weapon;
		public TI.Real MaxFiringDist;
		public TI.Real RateOfFire;
		public TI.Real ProjectileError;
		public TI.RealBounds FirstBurstDelayTime;
		public TI.Real NewTargetFiringPatternTime;
		public TI.Real SurpriseDelayTime, SurpriseFireWildyTime;
		public TI.Real DeathFireWildlyChance, DeathFireWildlyTime;
		public TI.RealBounds DesiredCombatRange;
		public TI.RealVector3D CustomStandGunOffset, CustomCrouchGunOffset;
		public TI.Real TargetTracking, TargetLeading;
		public TI.Real WeaponDamageModifier;
		public TI.Real DamagePerSecond;

		public TI.Real BurstOriginRadius;
		public TI.Real BurstOriginAngle;
		public TI.RealBounds BurstReturnLength;
		public TI.Real BurstReturnAngle;
		public TI.RealBounds BurstDuration, BurstSepartion;
		public TI.Real BurstAngularVelocity;
		public TI.Real SpecialDamageModifier;
		public TI.Real SpecialProjectileError;

		public TI.Real NewTargetBurstDuration, NewTargetBurstSeperation, NewTargetRateOfFire, NewTargetProjectileError;
		public TI.Real MovingBurstDuration, MovingBurstSeperation, MovingRateOfFire, MovingProjectileError;
		public TI.Real BerserkBurstDuration, BerserkBurstSeperation, BerserkRateOfFire, BerserkProjectileError;

		public TI.Real SuperBallisticRange, BombardmentRange, ModifiedVisionRange;
		public TI.Enum SpecialFireMode, SpecialFireSituation;
		public TI.Real SpecialFireChance, SpecialFireDelay;

		public TI.Real MeleeRange, MeleeAbortRange;
		public TI.RealBounds BerserkFiringRange;
		public TI.Real BerserkMeleeRange, BerserkMeleeAbortRange;

		public TI.Enum GrenadeType, TrajectoryType, GrenadeStimulus;
		public TI.ShortInteger MinEnemyCount;
		public TI.Real EnemyRadius;
		public TI.Real GrenadeVelocity;
		public TI.RealBounds GrenadeRanges;
		public TI.Real CollateralDamageRadius;
		public TI.Real GrenadeChance;
		public TI.Real GrenadeCheckTime;
		public TI.Real EncounterGrenadeTimeout;

		public TI.TagReference Equipment;
		public TI.ShortIntegerBounds GrenadeCount;
		public TI.Real DontDropGrenadesChance;
		public TI.RealBounds DropWeaponLoaded;
		public TI.ShortIntegerBounds DropWeaponAmmo;

		public TI.Real BodyVitality, ShieldVitality;
		public TI.Real ShieldSappingRadius;
		public TI.ShortInteger ForcedShaderPermutation;

		public TI.Block<actor_variant_change_colors_block> ChangeColors;
		#endregion
	};
	#endregion
}