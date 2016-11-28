/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo1.Tags
{
	#region object
	[TI.TagGroup((int)TagGroups.Enumerated.obje, 1, object_group.k_object_size)]
	public partial class object_group : TI.Definition
	{
		internal const int k_object_size = 380;

		#region object_attachment_block
		[TI.Definition(-1, 72)]
		public partial class object_attachment_block : TI.Definition
		{
			public TI.TagReference Type;
			public TI.String Marker;
			public TI.Enum PrimaryScale, SecondaryScale, ChangeColor;
		};
		#endregion

		#region object_widget_block
		[TI.Definition(-1, 32)]
		public partial class object_widget_block : TI.Definition
		{
			public TI.TagReference Reference;
		};
		#endregion

		#region object_function_block
		[TI.Definition(-1, 360)]
		public partial class object_function_block : TI.Definition
		{
			public TI.Flags Flags;
			public TI.Real Period;
			public TI.Enum ScalePeriodBy;
			public TI.Enum Function;
			public TI.Enum ScaleFunctionBy;
			public TI.Enum WobbleFunction;
			public TI.Real WobblePeriod;
			public TI.Real WobbleMagnitude;
			public TI.Real SquareWaveThreshold;
			public TI.ShortInteger StepCount;
			public TI.Enum MapTo;
			public TI.ShortInteger SawtoothCount;
			public TI.Enum _Add;
			public TI.Enum ScaleResultBy;
			public TI.Enum BoundsMode;
			public TI.RealBounds Bounds;
			public TI.BlockIndex TurnoffWith;
			public TI.Real ScaleBy;
			public TI.String Usage;
		};
		#endregion

		#region object_change_colors_block
		[TI.Definition(-1, 44)]
		public partial class object_change_colors_block : TI.Definition
		{
			#region object_change_color_permutations_block
			[TI.Definition(-1, 28)]
			public partial class object_change_color_permutations_block : TI.Definition
			{
				public TI.Real Weight;
				public TI.RealColor ColorLowerBound, ColorUpperBound;
			};
			#endregion

			public TI.Enum DarkenBy, ScaleBy;
			public TI.Flags ScaleFlags;
			public TI.RealColor ColorLowerBound, ColorUpperBound;
			public TI.Block<object_change_color_permutations_block> Permutations;
		};
		#endregion

		public TI.Skip ObjectType;
		public TI.Flags ObjectFlags;
		public TI.Real BoundingRadius;
		public TI.RealPoint3D BoundingOffset, OriginOffset;
		public TI.Real AccelerationScale;
		public TI.TagReference Model, AnimationGraph, CollisionModel, Physics, ModifierShader, CreationEffect;
		public TI.Real RenderBoundingRadius;

		public TI.Enum Ain, Bin, Cin, Din;

		public TI.ShortInteger HudTextMessageIndex;
		public TI.ShortInteger ForcedShaderPermuationIndex;

		public TI.Block<object_attachment_block> Attachments;
		public TI.Block<object_widget_block> Widgets;
		public TI.Block<object_function_block> Functions;
		public TI.Block<object_change_colors_block> ChangeColors;
		public TI.Block<predicted_resource_block> PerdictedResources;
	};
	#endregion

	#region device
	[TI.TagGroup((int)TagGroups.Enumerated.devi, 1, /*object_group.k_object_size +*/ device_group.k_device_size, typeof(object_group))]
	public partial class device_group : object_group
	{
		internal const int k_device_size = 276;

		public TI.Flags DeviceFlags;
		public TI.Real PowerTransitionTime, PowerAccelerationTime;
		public TI.Real PositionTransitionTime, PositionAccelerationTime;
		public TI.Real DepoweredPositionTransitionTime, DepoweredPositionAccelerationTime;
		public TI.Enum AinDevice, BinDevice, CinDevice, DinDevice;
		public TI.TagReference Open, Close, Opened, Closed, Depowered, Repowered;
		public TI.Real DelayTime;
		public TI.TagReference DelayEffect;
		public TI.Real AutomaticActivationRadius;

		private TI.Skip PowerTransitionGameTime, PowerAccelerationGameTime;
		private TI.Skip PositionTransitionGameTime, PositionAccelerationGameTime;
		private TI.Skip DepoweredPositionTransitionGameTime, DepoweredPositionAccelerationGameTime;
		private TI.Skip DelayGameTime;
	};
	#endregion

	#region item
	[TI.TagGroup((int)TagGroups.Enumerated.item, 2, /*object_group.k_object_size +*/ item_group.k_item_size, typeof(object_group))]
	public partial class item_group : object_group
	{
		internal const int k_item_size = 396;

		public TI.Flags ItemFlags;
		
		public TI.ShortInteger MessageIndex;
		public TI.ShortInteger SortOrder;
		public TI.Real Scale;
		public TI.ShortInteger HudMessageValueScale;

		public TI.Enum AinDevice, BinDevice, CinDevice, DinDevice;

		public TI.TagReference MaterialEffects, CollisionSound;
		public TI.RealBounds DetonationDelay;
		public TI.TagReference DetonatingEffect, DetonationEffect;
	};
	#endregion

	#region unit
	[TI.TagGroup((int)TagGroups.Enumerated.unit, 2, /*object_group.k_object_size +*/ unit_group.k_unit_size, typeof(object_group))]
	public partial class unit_group : object_group
	{
		internal const int k_unit_size = 372;

		#region unit_camera_track_block
		[TI.Definition(-1, 28)]
		public partial class unit_camera_track_block : TI.Definition
		{
			public TI.TagReference Track;
		};
		#endregion

		#region unit_hud_reference_block
		[TI.Definition(-1, 48)]
		public partial class unit_hud_reference_block : TI.Definition
		{
			public TI.TagReference UnitHudInterface;
		};
		#endregion

		#region dialogue_variant_block
		[TI.Definition(-1, 24)]
		public partial class dialogue_variant_block : TI.Definition
		{
			public TI.ShortInteger VariantNumber;
			public TI.TagReference Dialogue;
		};
		#endregion

		#region powered_seat_block
		[TI.Definition(-1, 68)]
		public partial class powered_seat_block : TI.Definition
		{
			public TI.Real DriverPowerupTime, DriverPowerdownTime;
		};
		#endregion

		#region unit_weapon_block
		[TI.Definition(-1, 36)]
		public partial class unit_weapon_block : TI.Definition
		{
			public TI.TagReference Weapon;
		};
		#endregion

		#region unit_seat_block
		[TI.Definition(-1, 284)]
		public partial class unit_seat_block : TI.Definition
		{
			public TI.Flags Flags;
			public TI.String Label;
			public TI.String MarkerName;
			public TI.RealVector3D AccelerationScale;
			public TI.Real YawRate, PitchRate;
			public TI.String CameraMarkerName, CameraSugmergedMarkerName;
			public TI.Real PitchAutoLevel;
			public TI.RealBounds PitchRange;
			public TI.Block<unit_camera_track_block> CameraTracks;
			public TI.Block<unit_hud_reference_block> UnitHudInterface;
			public TI.ShortInteger HudTextMessageIndex;
			public TI.Real YawMin, YawMax;
			public TI.TagReference BuiltInGunner;
		};
		#endregion

		public TI.Flags UnitFlags;
		public TI.Enum DefaultTeam;
		public TI.Enum ConstantSoundVolume;
		public TI.Real RiderDamageFraction;
		public TI.TagReference IntegratedLightToggle;
		public TI.Enum AinUnit, BinUnit, CinUnit, DinUnit;

		public TI.Real CameraFieldOfView;
		public TI.Real CameraStiffness;
		public TI.String CameraMarkerName, CameraSubmergedMarkerName;
		public TI.Real PitchAutoLevel;
		public TI.RealBounds PitchRange;
		public TI.Block<unit_camera_track_block> CameraTracks;

		public TI.RealVector3D SeatAccelerationScale;

		public TI.Real SoftPingThreshold, SoftPingInterruptTime;
		public TI.Real HardPingThreshold, HardPingInterruptTime;
		public TI.Real HardDeathThreshold, FeignDeathThreshold, FeignDeathTime;
		public TI.Real DistOfEvadeAnim, DistOfDiveAnim;
		public TI.Real StunnedMovementThreshold;
		public TI.Real FeignDeathChance, FeignRepeatChance;

		public TI.TagReference SpawnedActor;
		public TI.ShortIntegerBounds SpawnedActorCount;
		public TI.Real SpawnedVelocity;
		public TI.Real AimingVelocityMax, AimingAccelerationMax;
		public TI.Real CasualAimingModifier;
		public TI.Real LookingVelocityMaximum, LookingAccelerationMax;
		public TI.Real AiVehicleRadius, AiDangerRadius;
		public TI.TagReference MeleeDamage;
		public TI.Enum MotionSensorBlipSize;
		public TI.Block<unit_hud_reference_block> NewHudInterfaces;
		public TI.Block<dialogue_variant_block> DialogueVariants;
		public TI.Real GrenadeVelocity;
		public TI.Enum GrenadeType;
		public TI.ShortInteger GrenadeCount;
		public TI.Block<powered_seat_block> PoweredSeats;
		public TI.Block<unit_weapon_block> Weapons;
		public TI.Block<unit_seat_block> Seats;
	};
	#endregion


	#region placeholder
	[TI.TagGroup((int)TagGroups.Enumerated.plac, 2, /*object_group.k_object_size +*/ 128, typeof(object_group))]
	public partial class placeholder_group : object_group
	{
		public placeholder_group() : base(1)
		{
			Add(new TI.Pad(128));
		}
	};
	#endregion

	#region projectile
	[TI.TagGroup((int)TagGroups.Enumerated.proj, 5, /*object_group.k_object_size +*/ 208, typeof(object_group))]
	public partial class projectile_group : object_group
	{
		#region projectile_material_response_block
		[TI.Definition(-1, 160)]
		public partial class projectile_material_response_block : TI.Definition
		{
			public TI.Flags Flags;

			public TI.Enum Response;
			public TI.TagReference Effect;

			public TI.Enum PotentialResponse;
			public TI.Flags PotentialFlags;
			public TI.Real SkipFraction;
			public TI.RealBounds Between;
			public TI.RealBounds And;
			public TI.TagReference PotentialEffect;

			public TI.Enum ScaleEffectsBy;
			public TI.Real AngularNoise;
			public TI.Real VelocityNoise;
			public TI.TagReference DetonationEffect;

			public TI.Real InitialFriction;
			public TI.Real MaxDist;

			public TI.Real ParallelFriction, PerpendicularFriction;
		};
		#endregion

		public TI.Flags Flags;
		public TI.Enum DetonationTimerStarts;
		public TI.Enum ImpactNoise;
		public TI.Enum AinProjectile, BinProjectile, CinProjectile, DinProjectile;
		public TI.TagReference SuperDetonation;
		public TI.Real AiPerceptionRadius;
		public TI.Real CollisionRadius;

		public TI.Real ArmingTime;
		public TI.Real DangerRadius;
		public TI.TagReference Effect;
		public TI.RealBounds Timer;
		public TI.Real MinVelocity, MaxRange;
		
		public TI.Real AirGravityScale;
		public TI.RealBounds AirDamageRange;
		public TI.Real WaterGravityScale;
		public TI.RealBounds WaterDamageRange;
		public TI.Real InitialVelocity, FinalVelocity;
		public TI.Real GuidedAngularVelocity;

		public TI.Enum DetonationNoise;
		public TI.TagReference DetonationStarted, FlybySound, AttachedDetonationDamage, ImpactDamage;
		public TI.Block<projectile_material_response_block> MaterialResponses;
	};
	#endregion

	#region scenery
	[TI.TagGroup((int)TagGroups.Enumerated.scen, 1, /*object_group.k_object_size +*/ 128, typeof(object_group))]
	public partial class scenery_group : object_group
	{
		public scenery_group() : base(1)
		{
			Add(new TI.Pad(128));
		}
	};
	#endregion

	#region sound_scenery
	[TI.TagGroup((int)TagGroups.Enumerated.ssce, 1, /*object_group.k_object_size +*/ 128, typeof(object_group))]
	public partial class sound_scenery_group : object_group
	{
		public sound_scenery_group() : base(1)
		{
			Add(new TI.Pad(128));
		}
	};
	#endregion


	#region device_control
	[TI.TagGroup((int)TagGroups.Enumerated.ctrl, 1, /*object_group.k_object_size + device_group.k_device_size +*/ 136, typeof(device_group))]
	public partial class device_control_group : device_group
	{
		public TI.Enum Type;
		public TI.Enum TriggersWhen;
		public TI.Real CallValue;
		public TI.TagReference On, Off, Deny;
	};
	#endregion

	#region device_light_fixture
	[TI.TagGroup((int)TagGroups.Enumerated.lifi, 1, /*object_group.k_object_size + device_group.k_device_size +*/ 64, typeof(device_group))]
	public partial class device_light_fixture_group : device_group
	{
		public device_light_fixture_group() : base(1)
		{
			Add(new TI.Pad(64));
		}
	};
	#endregion

	#region device_machine
	[TI.TagGroup((int)TagGroups.Enumerated.mach, 1, /*object_group.k_object_size + device_group.k_device_size +*/ 148, typeof(device_group))]
	public partial class device_machine_group : device_group
	{
		public TI.Enum Type;
		public TI.Flags Flags;
		public TI.Real DoorOpenTime;
		public TI.Enum CollisionResponse;
		public TI.ShortInteger ElevatorNode;
		
		private TI.Skip DoorOpenGameTime;
	};
	#endregion


	#region equipment
	[TI.TagGroup((int)TagGroups.Enumerated.eqip, 2, /*object_group.k_object_size + item_group.k_item_size +*/ 168, typeof(item_group))]
	public partial class equipment_group : item_group
	{
		public TI.Enum PowerupType, GrenadeType;
		public TI.Real PowerupTime;
		public TI.TagReference PickupSound;
	};
	#endregion

	#region garbage
	[TI.TagGroup((int)TagGroups.Enumerated.garb, 1, /*object_group.k_object_size + item_group.k_item_size +*/ 168, typeof(item_group))]
	public partial class garbage_group : item_group
	{
		public garbage_group() : base(1)
		{
			Add(new TI.Pad(168));
		}
	};
	#endregion

	#region weapon
	[TI.TagGroup((int)TagGroups.Enumerated.weap, 2, /*object_group.k_object_size + item_group.k_item_size +*/ 512, typeof(item_group))]
	public partial class weapon_group : item_group
	{
		#region magazines_block
		[TI.Definition(-1, 112)]
		public partial class magazines_block : TI.Definition
		{
			#region magazine_objects_block
			[TI.Definition(-1, 28)]
			public partial class magazine_objects_block : TI.Definition
			{
				public TI.ShortInteger Rounds;
				public TI.TagReference Equipment;
			};
			#endregion

			public TI.Flags Flags;
			public TI.ShortInteger RoundsRecharged, RoundsTotalInitial, RoundsTotalMax, RoundsLoadedMax;
			public TI.Real ReloadTime;
			public TI.ShortInteger RoundsReloaded;
			public TI.Real ChamberTime;
			public TI.TagReference ReloadingEffect, ChamberingEffect;
			public TI.Block<magazine_objects_block> Magazines;
		};
		#endregion

		#region triggers_block
		[TI.Definition(-1, 276)]
		public partial class triggers_block : TI.Definition
		{
			#region trigger_firing_effect_block
			[TI.Definition(-1, 132)]
			public partial class trigger_firing_effect_block : TI.Definition
			{
				public TI.ShortIntegerBounds ShotCount;
				public TI.TagReference FiringEffect, MisfireEffect, EmptyEffect, FiringDamage, MisfireDamage, EmtpyDamage;
			};
			#endregion

			public TI.Flags Flags;

			public TI.RealBounds RoundsPerSecond;
			public TI.Real FiringAccelerationTime, FiringDecelerationTime;
			public TI.Real BlurredRateOfFire;
			public TI.BlockIndex Magazine;
			public TI.ShortInteger RoundsPerShot, MinRoundsLoaded, RoundsBetweenTracers;
			public TI.Enum FiringNoise;

			public TI.RealBounds Error;
			public TI.Real ErrorAccelerationTime, ErrorDecelerationTime;

			public TI.Real ChargingTime, ChargedTime;
			public TI.Enum OverchargedAction;
			public TI.Real ChargedIllumination;
			public TI.Real SpewTime;
			public TI.TagReference ChargingEffect;

			public TI.Enum DistributionFunction;
			public TI.ShortInteger ProjectilesPerShot;
			public TI.Real DistributionAngle;
			public TI.Real MinError;
			public TI.RealBounds ErrorAngle;
			public TI.RealPoint3D FirstPersionOffset;
			public TI.TagReference Projectile;

			public TI.Real EjectionPortRecoveryTime, IlluminationRecoveryTime;
			public TI.Real HeatGeneratedPerRound, AgeGeneratedPerRound;
			public TI.Real OverloadTime;

			public TI.Block<trigger_firing_effect_block> FiringEffects;
		};
		#endregion

		public TI.Flags Flags;
		public TI.String Label;
		public TI.Enum SecondaryTriggerMode;
		public TI.ShortInteger MaxAltShotsLoaded;
		public TI.Enum AinWeapon, BinWeapon, CinWeapon, DinWeapon;
		public TI.Real ReadyTime;
		public TI.TagReference ReadyEffect;

		public TI.Real HeatRecoveryThreshold, OverheatedThreshold, HeatDetonationThreshold, HeatDetonationFraction, HeatLossPerSecond, HeatIllumination;
		public TI.TagReference Overheated, Detonation, PlayerMeleeDamage, PlayerMeleeResponse;

		public TI.TagReference ActorFiringParameters;

		public TI.Real NearRecticleRange, FarReticleRange, IntersectionRecticleRange;

		public TI.ShortInteger MagnificationLevel;
		public TI.RealBounds MagnificationRange;

		public TI.Real AutoaimAngle;
		public TI.Real AutoaimRange;
		public TI.Real MagnetismAngle;
		public TI.Real MagnetismRange;
		public TI.Real DeviationAngle;

		public TI.Enum MovementPenalized;
		public TI.Real ForwardMovementPenalty, SidewaysMovementPenalty;

		public TI.Real MinTargetRange, LookingTimeModifier;

		public TI.Real LightPowerOnTime, LightPowerOffTime;
		public TI.TagReference LightPowerOnEffect, LightPowerOffEffect;
		public TI.Real AgeHeatRecoveryPenalty, AgeRateOfFirePenalty;
		public TI.Real AgeMisfireStart, AgeMisfireChance;

		public TI.TagReference FirstPersonModel, FirstPersonAnimations;
		public TI.TagReference HudInterface, PickupSound, ZoomInSound, ZoomOutSound;
		public TI.Real ActiveCamoDing, ActiveCamoRegrowth;
		
		public TI.Enum WeaponType;
		public TI.Block<predicted_resource_block> PredictedResourcesWeapon;
		public TI.Block<magazines_block> Magazines;
		public TI.Block<triggers_block> Triggers;
	};
	#endregion


	#region biped
	[TI.TagGroup((int)TagGroups.Enumerated.bipd, 3, /*object_group.k_object_size + unit_group.k_unit_size +*/ 516, typeof(unit_group))]
	public partial class biped_group : unit_group
	{
		#region contact_point_block
		[TI.Definition(-1, 64)]
		public partial class contact_point_block : TI.Definition
		{
			public TI.String MarkerName;
		};
		#endregion

		public TI.Real MovingTurningSpeed;
		public TI.Flags Flags;
		public TI.Real StationaryTurningThreshold;
		public TI.Enum AinBiped, BinBiped, CinBiped, DinBiped;
		private TI.TagReference DontUse;

		public TI.Real BankAngle;
		public TI.Real BankApplyTime, BankDecayTime;
		public TI.Real PitchRatio;
		public TI.Real MaxVelocity, MaxSidestepVelocity;
		public TI.Real Acceleration, Deceleration;
		public TI.Real AngularVelocityMax, AngularAccelerationMax;
		public TI.Real CrouchVelocityModifier;

		public TI.Real MaxSlopeAngle;
		public TI.Real DownhillFalloffAngle, DownhillCutoffAngle;
		public TI.Real DownhillVelocityScale;
		public TI.Real UphillFalloffAngle, UphillCutoffAngle;
		public TI.Real UphillVelocityScale;
		
		public TI.TagReference Footsteps;

		public TI.Real JumpVelocity;

		public TI.Real MaxSoftLandingTime, MaxHardLandingTime;
		public TI.Real MinSoftLandingVelocity, MinHardLandingVelocity, MaxHardLandingVelocity;
		public TI.Real DeathHardLandingVelocity;

		public TI.Real StandingCameraHeight, CrouchingCameraHeight;
		public TI.Real CrouchTransitionTime;

		public TI.Real StandingCollisionHeight, CrouchingCollisionHeight;
		public TI.Real CollisionRadius;

		public TI.Real AutoaimWidth;

		public TI.Block<contact_point_block> ContactPoints;
	};
	#endregion

	#region vehicle
	[TI.TagGroup((int)TagGroups.Enumerated.vehi, 1, /*object_group.k_object_size + unit_group.k_unit_size +*/ 256, typeof(unit_group))]
	public partial class vehicle_group : unit_group
	{
		public TI.Flags Flags;
		public TI.Enum Type;
		public TI.Real MaxForwardSpeed, MaxReverseSpeed;
		public TI.Real SpeedAcceleration, SpeedDeceleration;
		public TI.Real MaxLeftTurn, MaxRightTurn;
		public TI.Real WheelCircumference;
		public TI.Real TurnRate, BlurSpeed;
		public TI.Enum AinVehicle, BinVehicle, CinVehicle, DinVehicle;
		public TI.Real MaxLeftSlide, MaxRightSlide;
		public TI.Real SlideAcceleration, SlideDecleration;
		public TI.Real MinFlippingAngularVelocity, MaxFlippingAngularVelocity;
		public TI.Real FixedGunYaw, FixedGunPitch;
		public TI.Real AiSideslipDist, AiDestRadius, AiAvoidanceDist, AiPathfindingRadius, AiChargeRepeatTimeout, AiStrafingAbortRange;
		public TI.RealBounds AiOverseeringBounds;
		public TI.Real AiSteeringMax;
		public TI.Real AiThrottleMax, AiMovePositionTime;
		public TI.TagReference SuspensionSound, CrashSound, MaterialEffects, Effect;
	};
	#endregion


	#region item_collection
	[TI.TagGroup((int)TagGroups.Enumerated.itmc, 0, 92)]
	public partial class item_collection_group : TI.Definition
	{
		#region item_permutation_block
		[TI.Definition(-1, 84)]
		public partial class item_permutation_block : TI.Definition
		{
			public TI.Real Weight;
			public TI.TagReference Item;
		};
		#endregion

		public TI.Block<item_permutation_block> ItemPermutations;
		public TI.ShortInteger SpawnTime;
	};
	#endregion
}