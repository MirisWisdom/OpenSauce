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
	public partial class actor_group
	{
		public actor_group() : base(124)
		{
			Add(Flags = new TI.Flags());
			Add(MoreFlags = new TI.Flags());
			Add(new TI.Pad(12));
			Add(Type = new TI.Enum());
			Add(new TI.Pad(2));

			#region perception
			Add(MaxVisionDist = new TI.Real());
			Add(CentralVisionAngle = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(MaxVisionAngle = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(new TI.Pad(4));
			Add(PeripheralVisionAngle = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(PeripheralDist = new TI.Real());
			Add(new TI.Pad(4));
			Add(StandingGunOffset = new TI.RealVector3D());
			Add(CrouchingGunOffset = new TI.RealVector3D());
			Add(HearingDist = new TI.Real());
			Add(NoticeProjectileChance = new TI.Real());
			Add(NoticeVehicleChance = new TI.Real());
			Add(new TI.Pad(8));
			Add(CombatPerceptionTime = new TI.Real());
			Add(GuardPerceptionTime = new TI.Real());
			Add(NonCombatPerceptionTime = new TI.Real());
			Add(new TI.Pad(12 + 8));
			#endregion

			#region movement
			Add(DiveIntoCoverChance = new TI.Real());
			Add(EmergeFromCoverChance = new TI.Real());
			Add(DiveFromGrenadeChance = new TI.Real());
			Add(PathfindingRadius = new TI.Real());
			Add(GlassIgnoranceChance = new TI.Real());
			Add(StationaryMovementDist = new TI.Real());
			Add(FreeFlyingSidestep = new TI.Real());
			Add(BeginMovingAngle = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(new TI.Pad(4));
			#endregion

			#region looking
			Add(MaxAimingDeviation = new TI.RealEulerAngles2D());
			Add(MaxLookingDeviation = new TI.RealEulerAngles2D());
			Add(NonCombatLookDeltaL = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(NonCombatLookDeltaR = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(CombatLookDeltaL = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(CombatLookDeltaR = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(IdleAimingRange = new TI.RealEulerAngles2D());
			Add(IdleLookingRange = new TI.RealEulerAngles2D());
			Add(EventLookTimeModifier = new TI.RealBounds());
			Add(NonCombatIdleFacing = new TI.RealBounds());
			Add(NonCombatIdleAiming = new TI.RealBounds());
			Add(NonCombatIdleLooking = new TI.RealBounds());
			Add(GuardIdleFacing = new TI.RealBounds());
			Add(GuardIdleAiming = new TI.RealBounds());
			Add(GuardIdleLooking = new TI.RealBounds());
			Add(CombatIdleFacing = new TI.RealBounds());
			Add(CombatIdleAiming = new TI.RealBounds());
			Add(CombatIdleLooking = new TI.RealBounds());
			Add(new TI.Pad(8 + 16));
			#endregion

			Add(DoNotUseWeapon = new TI.TagReference(this, TagGroups.weap));
			Add(new TI.Pad(268));
			Add(DoNotUseProjectile = new TI.TagReference(this, TagGroups.proj));

			#region unopposable
			Add(UnreachableDangerTrigger = new TI.Enum());
			Add(VehicleDangerTrigger = new TI.Enum());
			Add(PlayerDangerTrigger = new TI.Enum());
			Add(new TI.Pad(2));
			Add(DangerTriggerTime = new TI.RealBounds());
			Add(FriendsKilledTrigger = new TI.ShortInteger());
			Add(FriendsRetreatingTrigger = new TI.ShortInteger());
			Add(new TI.Pad(12));
			Add(RetreatTime = new TI.RealBounds());
			Add(new TI.Pad(8));
			#endregion

			#region panic
			Add(CoveringTime = new TI.RealBounds());
			Add(FriendKilledPanicChance = new TI.Real());
			Add(LeaderType = new TI.Enum());
			Add(new TI.Pad(2));
			Add(LeaderKilledPanicChance = new TI.Real());
			Add(PanicDamageThreshold = new TI.Real());
			Add(SurpriseDist = new TI.Real());
			Add(new TI.Pad(28));
			#endregion

			#region defensive
			Add(HideBehindCoverTime = new TI.RealBounds());
			Add(HideTagerNotVisibleTime = new TI.Real());
			Add(HideShieldFraction = new TI.Real());
			Add(AttackShieldFraction = new TI.Real());
			Add(PursueShieldFraction = new TI.Real());
			Add(new TI.Pad(16));
			Add(DefensiveCrouchType = new TI.Enum());
			Add(new TI.Pad(2));
			Add(AttackingCrouchThreshold = new TI.Real());
			Add(DefendingCrouchThreshold = new TI.Real());
			Add(MinStandTime = new TI.Real());
			Add(MinCrouchTime = new TI.Real());
			Add(DefendingHideTime = new TI.Real());
			Add(AttackingEvasionThreshold = new TI.Real());
			Add(DefendingEvasionThreshold = new TI.Real());
			Add(EvasionSeekCoverChance = new TI.Real());
			Add(EvasionDelayTime = new TI.Real());
			Add(MaxSeekCoverDist = new TI.Real());
			Add(CoverDamageThreshold = new TI.Real());
			Add(StalkingDiscoveryTime = new TI.Real());
			Add(StalkingMaxDist = new TI.Real());
			Add(StationaryFacingAngle = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(ChangeFacingStandTime = new TI.Real());
			Add(new TI.Pad(4));
			#endregion

			#region pursuit
			Add(UncoverDelayTimer = new TI.RealBounds());
			Add(TargetSearchTime = new TI.RealBounds());
			Add(PursuitPositionTime = new TI.RealBounds());
			Add(NumPositionsCoord = new TI.ShortInteger());
			Add(NumPositionsNormal = new TI.ShortInteger());
			Add(new TI.Pad(32));
			#endregion

			#region berserk
			Add(MeleeAttackDelay = new TI.Real());
			Add(MeleeFudgeFactor = new TI.Real());
			Add(MeleeChargeTime = new TI.Real());
			Add(MeleeLeapRange = new TI.RealBounds());
			Add(MeleeLeapVelocity = new TI.Real());
			Add(MeleeLeapChance = new TI.Real());
			Add(MeleeLeapBallistic = new TI.Real());
			Add(BerserkDamageAmount = new TI.Real());
			Add(BerserkDamageThreshold = new TI.Real());
			Add(BerserkProximity = new TI.Real());
			Add(SuicideSensingDist = new TI.Real());
			Add(BerserkGrenadeChance = new TI.Real());
			Add(new TI.Pad(12));
			#endregion

			#region firing positions
			Add(GuardPositionTIme = new TI.RealBounds());
			Add(CombatPositionTime = new TI.RealBounds());
			Add(OldPositionAvoidDist = new TI.Real());
			Add(FriendAvoidDist = new TI.Real());
			Add(new TI.Pad(40));
			#endregion

			#region communication
			Add(NoncombatIdleSpeechTime = new TI.RealBounds());
			Add(CombatIdleSpechTime = new TI.RealBounds());
			Add(new TI.Pad(48 + 128));
			#endregion

			Add(DoNotUseMajorUpgrade = new TI.TagReference(this, TagGroups.actr));
			Add(new TI.Pad(48));
		}
	};
	#endregion

	#region actor_variant_group
	public partial class actor_variant_group
	{
		public partial class actor_variant_change_colors_block
		{
			public actor_variant_change_colors_block() : base(3)
			{
				Add(ColorLowerBound = new TI.RealColor());
				Add(ColorUpperBound = new TI.RealColor());
				Add(new TI.Pad(8));
			}
		};

		public actor_variant_group() : base(90)
		{
			Add(Flags = new TI.Flags());
			Add(ActorDefinition = new TI.TagReference(this, TagGroups.actr));
			Add(Unit = new TI.TagReference(this, TagGroups.unit));
			Add(MajorVariant = new TI.TagReference(this, TagGroups.actv));
			Add(new TI.Pad(24));
			
			#region movement switching
			Add(MovementType = new TI.Enum());
			Add(new TI.Pad(2));
			Add(InitialCrouchChance = new TI.Real());
			Add(CrouchTime = new TI.RealBounds());
			Add(RunTime = new TI.RealBounds());
			#endregion

			#region ranged combat
			Add(Weapon = new TI.TagReference(this, TagGroups.weap));
			Add(MaxFiringDist = new TI.Real());
			Add(RateOfFire = new TI.Real());
			Add(ProjectileError = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(FirstBurstDelayTime = new TI.RealBounds());
			Add(NewTargetFiringPatternTime = new TI.Real());
			Add(SurpriseDelayTime = new TI.Real());
			Add(SurpriseFireWildyTime = new TI.Real());
			Add(DeathFireWildlyChance = new TI.Real());
			Add(DeathFireWildlyTime = new TI.Real());
			Add(DesiredCombatRange = new TI.RealBounds());
			Add(CustomStandGunOffset = new TI.RealVector3D());
			Add(CustomCrouchGunOffset = new TI.RealVector3D());
			Add(TargetTracking = new TI.Real());
			Add(TargetLeading = new TI.Real());
			Add(WeaponDamageModifier = new TI.Real());
			Add(DamagePerSecond = new TI.Real());
			#endregion

			#region burst geometry
			Add(BurstOriginRadius = new TI.Real());
			Add(BurstOriginAngle = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(BurstReturnLength = new TI.RealBounds());
			Add(BurstReturnAngle = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(BurstDuration = new TI.RealBounds());
			Add(BurstSepartion = new TI.RealBounds());
			Add(BurstAngularVelocity = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(new TI.Pad(4));
			Add(SpecialDamageModifier = new TI.Real());
			Add(SpecialProjectileError = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			#endregion

			#region firing patterns
			Add(NewTargetBurstDuration = new TI.Real());
			Add(NewTargetBurstSeperation = new TI.Real());
			Add(NewTargetRateOfFire = new TI.Real());
			Add(NewTargetProjectileError = new TI.Real());
			Add(new TI.Pad(8));
			Add(MovingBurstDuration = new TI.Real());
			Add(MovingBurstSeperation = new TI.Real());
			Add(MovingRateOfFire = new TI.Real());
			Add(MovingProjectileError = new TI.Real());
			Add(new TI.Pad(8));
			Add(BerserkBurstDuration = new TI.Real());
			Add(BerserkBurstSeperation = new TI.Real());
			Add(BerserkRateOfFire = new TI.Real());
			Add(BerserkProjectileError = new TI.Real());
			Add(new TI.Pad(8));
			#endregion

			#region special-case firing properties
			Add(SuperBallisticRange = new TI.Real());
			Add(BombardmentRange = new TI.Real());
			Add(ModifiedVisionRange = new TI.Real());
			Add(SpecialFireMode = new TI.Enum());
			Add(SpecialFireSituation = new TI.Enum());
			Add(SpecialFireChance = new TI.Real());
			Add(SpecialFireDelay = new TI.Real());
			#endregion

			#region berserking and melee
			Add(MeleeRange = new TI.Real());
			Add(MeleeAbortRange = new TI.Real());
			Add(BerserkFiringRange = new TI.RealBounds());
			Add(BerserkMeleeRange = new TI.Real());
			Add(BerserkMeleeAbortRange = new TI.Real());
			Add(new TI.Pad(8));
			#endregion

			#region grenades
			Add(GrenadeType = new TI.Enum());
			Add(TrajectoryType = new TI.Enum());
			Add(GrenadeStimulus = new TI.Enum());
			Add(MinEnemyCount = new TI.ShortInteger());
			Add(EnemyRadius = new TI.Real());
			Add(new TI.Pad(4));
			Add(GrenadeVelocity = new TI.Real());
			Add(GrenadeRanges = new TI.RealBounds());
			Add(CollateralDamageRadius = new TI.Real());
			Add(GrenadeChance = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(GrenadeCheckTime = new TI.Real());
			Add(EncounterGrenadeTimeout = new TI.Real());
			Add(new TI.Pad(20));
			#endregion

			#region items
			Add(Equipment = new TI.TagReference(this, TagGroups.eqip));
			Add(GrenadeCount = new TI.ShortIntegerBounds());
			Add(DontDropGrenadesChance = new TI.Real());
			Add(DropWeaponLoaded = new TI.RealBounds());
			Add(DropWeaponAmmo = new TI.ShortIntegerBounds());
			Add(new TI.Pad(12 + 16));
			#endregion

			#region unit
			Add(BodyVitality = new TI.Real());
			Add(ShieldVitality = new TI.Real());
			Add(ShieldSappingRadius = new TI.Real());
			Add(ForcedShaderPermutation = new TI.ShortInteger());
			Add(new TI.Pad(2 + 16 + 12));
			#endregion

			Add(ChangeColors = new TI.Block<actor_variant_change_colors_block>(this, 4));
		}
	};
	#endregion
}