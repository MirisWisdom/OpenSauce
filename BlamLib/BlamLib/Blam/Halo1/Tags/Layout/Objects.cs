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
	public partial class object_group
	{
		#region object_attachment_block
		public partial class object_attachment_block
		{
			public object_attachment_block() : base(6)
			{
				Add(Type = new TI.TagReference(this));
				Add(Marker = new TI.String());
				Add(PrimaryScale = new TI.Enum());
				Add(SecondaryScale = new TI.Enum());
				Add(ChangeColor = new TI.Enum());
				Add(new TI.Pad(2 + 16));
			}
		};
		#endregion

		#region object_widget_block
		public partial class object_widget_block
		{
			public object_widget_block() : base(2)
			{
				Add(Reference = new TI.TagReference(this));
				Add(new TI.Pad(16));
			}
		};
		#endregion

		#region object_function_block
		public partial class object_function_block
		{
			public object_function_block() : base(21)
			{
				Add(Flags = new TI.Flags());
				Add(Period = new TI.Real());
				Add(ScalePeriodBy = new TI.Enum());
				Add(Function = new TI.Enum());
				Add(ScaleFunctionBy = new TI.Enum());
				Add(WobbleFunction = new TI.Enum());
				Add(WobblePeriod = new TI.Real());
				Add(WobbleMagnitude = new TI.Real());
				Add(SquareWaveThreshold = new TI.Real(TI.FieldType.RealFraction));
				Add(StepCount = new TI.ShortInteger());
				Add(MapTo = new TI.Enum());
				Add(SawtoothCount = new TI.ShortInteger());
				Add(_Add = new TI.Enum());
				Add(ScaleResultBy = new TI.Enum());
				Add(BoundsMode = new TI.Enum());
				Add(Bounds = new TI.RealBounds(BlamLib.TagInterface.FieldType.RealFractionBounds));
				Add(new TI.Pad(4 + 2));
				Add(TurnoffWith = new TI.BlockIndex());
				Add(ScaleBy = new TI.Real());
				Add(new TI.Pad(252 + 16));
				Add(Usage = new TI.String());
			}
		};
		#endregion

		#region object_change_colors_block
		public partial class object_change_colors_block
		{
			#region object_change_color_permutations_block
			public partial class object_change_color_permutations_block
			{
				public object_change_color_permutations_block() : base(3)
				{
					Add(Weight = new TI.Real());
					Add(ColorLowerBound = new TI.RealColor());
					Add(ColorUpperBound = new TI.RealColor());
				}
			};
			#endregion

			public object_change_colors_block() : base(6)
			{
				Add(DarkenBy = new TI.Enum());
				Add(ScaleBy = new TI.Enum());
				Add(ScaleFlags = new TI.Flags());
				Add(ColorLowerBound = new TI.RealColor());
				Add(ColorUpperBound = new TI.RealColor());
				Add(Permutations = new TI.Block<object_change_color_permutations_block>(this, 8));
			}
		};
		#endregion

		public object_group() : this(0) {}
		protected object_group(int field_count) : base(field_count + 28)
		{
			Add(ObjectType = new TI.Skip(2));
			Add(ObjectFlags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
			Add(BoundingRadius = new TI.Real());
			Add(BoundingOffset = new TI.RealPoint3D());
			Add(OriginOffset = new TI.RealPoint3D());
			Add(AccelerationScale = new TI.Real());
			Add(new TI.Pad(4));
			Add(Model = new TI.TagReference(this, TagGroups.mode));
			Add(AnimationGraph = new TI.TagReference(this, TagGroups.antr));
			Add(new TI.Pad(40));
			Add(CollisionModel = new TI.TagReference(this, TagGroups.coll));
			Add(Physics = new TI.TagReference(this, TagGroups.phys));
			Add(ModifierShader = new TI.TagReference(this, TagGroups.shdr));
			Add(CreationEffect = new TI.TagReference(this, TagGroups.effe));
			Add(new TI.Pad(84));
			Add(RenderBoundingRadius = new TI.Real());

			Add(Ain = new TI.Enum());
			Add(Bin = new TI.Enum());
			Add(Cin = new TI.Enum());
			Add(Din = new TI.Enum());
			Add(new TI.Pad(44));
			Add(HudTextMessageIndex = new TI.ShortInteger());
			Add(ForcedShaderPermuationIndex = new TI.ShortInteger());
			Add(Attachments = new TI.Block<object_attachment_block>(this, 8));
			Add(Widgets = new TI.Block<object_widget_block>(this, 4));
			Add(Functions = new TI.Block<object_function_block>(this, 4));
			Add(ChangeColors = new TI.Block<object_change_colors_block>(this, 4));
			Add(PerdictedResources = new TI.Block<predicted_resource_block>(this, 1024));
		}
	};
	#endregion

	#region device
	public partial class device_group
	{
		public device_group() : this(0) {}
		protected device_group(int field_count) : base(field_count + 29)
		{
			Add(DeviceFlags = new TI.Flags());
			Add(PowerTransitionTime = new TI.Real()); // 0x180
			Add(PowerAccelerationTime = new TI.Real()); // 0x184
			Add(PositionTransitionTime = new TI.Real()); // 0x188
			Add(PositionAccelerationTime = new TI.Real()); // 0x18C
			Add(DepoweredPositionTransitionTime = new TI.Real()); // 0x190
			Add(DepoweredPositionAccelerationTime = new TI.Real()); // 0x194
			Add(AinDevice = new TI.Enum());
			Add(BinDevice = new TI.Enum());
			Add(CinDevice = new TI.Enum());
			Add(DinDevice = new TI.Enum());
			Add(Open = new TI.TagReference(this));
			Add(Close = new TI.TagReference(this));
			Add(Opened = new TI.TagReference(this));
			Add(Closed = new TI.TagReference(this));
			Add(Depowered = new TI.TagReference(this));
			Add(Repowered = new TI.TagReference(this));
			Add(DelayTime = new TI.Real());
			Add(new TI.Pad(8));
			Add(DelayEffect = new TI.TagReference(this));
			Add(AutomaticActivationRadius = new TI.Real());
			Add(new TI.Pad(84));

			Add(PowerAccelerationGameTime = new TI.Skip(4)); // 0x274
			Add(PowerTransitionGameTime = new TI.Skip(4)); // 0x278
			Add(DepoweredPositionAccelerationGameTime = new TI.Skip(4)); // 0x27C
			Add(DepoweredPositionTransitionGameTime = new TI.Skip(4)); // 0x280
			Add(PositionAccelerationGameTime = new TI.Skip(4)); // 0x284
			Add(PositionTransitionGameTime = new TI.Skip(4)); // 0x288
			Add(DelayGameTime = new TI.Skip(4)); // 0x28C
		}
	};
	#endregion

	#region item
	public partial class item_group
	{
		public item_group() : this(0) {}
		protected item_group(int field_count) : base(field_count + 17)
		{
			Add(ItemFlags = new TI.Flags());

			Add(MessageIndex = new TI.ShortInteger());
			Add(SortOrder = new TI.ShortInteger());
			Add(Scale = new TI.Real());
			Add(HudMessageValueScale = new TI.ShortInteger());
			Add(new TI.Pad(2 + 16));
			
			Add(AinDevice = new TI.Enum());
			Add(BinDevice = new TI.Enum());
			Add(CinDevice = new TI.Enum());
			Add(DinDevice = new TI.Enum());
			Add(new TI.Pad(164));

			Add(MaterialEffects = new TI.TagReference(this, TagGroups.foot));
			Add(CollisionSound = new TI.TagReference(this, TagGroups.snd_));
			Add(new TI.Pad(120));
			Add(DetonationDelay = new TI.RealBounds());
			Add(DetonatingEffect = new TI.TagReference(this, TagGroups.effe));
			Add(DetonationEffect = new TI.TagReference(this, TagGroups.effe));
		}
	};
	#endregion

	#region unit
	public partial class unit_group
	{
		#region unit_camera_track_block
		public partial class unit_camera_track_block
		{
			public unit_camera_track_block() : base(2)
			{
				Add(Track = new TI.TagReference(this, TagGroups.trak));
				Add(new TI.Pad(12));
			}
		};
		#endregion

		#region unit_hud_reference_block
		public partial class unit_hud_reference_block
		{
			public unit_hud_reference_block() : base(2)
			{
				Add(UnitHudInterface = new TI.TagReference(this, TagGroups.unhi));
				Add(new TI.Pad(32));
			}
		};
		#endregion

		#region dialogue_variant_block
		public partial class dialogue_variant_block
		{
			public dialogue_variant_block() : base(3)
			{
				Add(VariantNumber = new TI.ShortInteger());
				Add(new TI.Pad(2 + 4));
				Add(Dialogue = new TI.TagReference(this, TagGroups.udlg));
			}
		};
		#endregion

		#region powered_seat_block
		public partial class powered_seat_block
		{
			public powered_seat_block() : base(4)
			{
				Add(new TI.Pad(4));
				Add(DriverPowerupTime = new TI.Real());
				Add(DriverPowerdownTime = new TI.Real());
				Add(new TI.Pad(56));
			}
		};
		#endregion

		#region unit_weapon_block
		public partial class unit_weapon_block
		{
			public unit_weapon_block() : base(2)
			{
				Add(Weapon = new TI.TagReference(this, TagGroups.weap));
				Add(new TI.Pad(20));
			}
		};
		#endregion

		#region unit_seat_block
		public partial class unit_seat_block
		{
			public unit_seat_block() : base(21)
			{
				Add(Flags = new TI.Flags());
				Add(Label = new TI.String());
				Add(MarkerName = new TI.String());
				Add(new TI.Pad(32));
				Add(AccelerationScale = new TI.RealVector3D());
				Add(new TI.Pad(12));
				Add(YawRate = new TI.Real());
				Add(PitchRate = new TI.Real());
				Add(CameraMarkerName = new TI.String());
				Add(CameraSugmergedMarkerName = new TI.String());
				Add(PitchAutoLevel = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
				Add(PitchRange = new TI.RealBounds(BlamLib.TagInterface.FieldType.AngleBounds));
				Add(CameraTracks = new TI.Block<unit_camera_track_block>(this, 2));
				Add(UnitHudInterface = new TI.Block<unit_hud_reference_block>(this, 2));
				Add(new TI.Pad(4));
				Add(HudTextMessageIndex = new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(YawMin = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
				Add(YawMax = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
				Add(BuiltInGunner = new TI.TagReference(this, TagGroups.actv));
				Add(new TI.Pad(20));
			}
		};
		#endregion

		public unit_group() : this(0) {}
		protected unit_group(int field_count) : base(field_count + 54)
		{
			Add(UnitFlags = new TI.Flags());
			Add(DefaultTeam = new TI.Enum());
			Add(ConstantSoundVolume = new TI.Enum());
			Add(RiderDamageFraction = new TI.Real());
			Add(IntegratedLightToggle = new TI.TagReference(this, TagGroups.effe));
			Add(AinUnit = new TI.Enum());
			Add(BinUnit = new TI.Enum());
			Add(CinUnit = new TI.Enum());
			Add(DinUnit = new TI.Enum());
			Add(CameraFieldOfView = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(CameraStiffness = new TI.Real());
			Add(CameraMarkerName = new TI.String());
			Add(CameraSubmergedMarkerName = new TI.String());
			Add(PitchAutoLevel = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(PitchRange = new TI.RealBounds(BlamLib.TagInterface.FieldType.AngleBounds));
			Add(CameraTracks = new TI.Block<unit_camera_track_block>(this, 2));
			Add(SeatAccelerationScale = new TI.RealVector3D());
			Add(new TI.Pad(12));
			Add(SoftPingThreshold = new TI.Real());
			Add(SoftPingInterruptTime = new TI.Real());
			Add(HardPingThreshold = new TI.Real());
			Add(HardPingInterruptTime = new TI.Real());
			Add(HardDeathThreshold = new TI.Real());
			Add(FeignDeathThreshold = new TI.Real());
			Add(FeignDeathTime = new TI.Real());
			Add(DistOfEvadeAnim = new TI.Real());
			Add(DistOfDiveAnim = new TI.Real());
			Add(new TI.Pad(4));
			Add(StunnedMovementThreshold = new TI.Real());
			Add(FeignDeathChance = new TI.Real());
			Add(FeignRepeatChance = new TI.Real());
			Add(SpawnedActor = new TI.TagReference(this, TagGroups.actv));
			Add(SpawnedActorCount = new TI.ShortIntegerBounds());
			Add(SpawnedVelocity = new TI.Real());
			Add(AimingVelocityMax = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(AimingAccelerationMax = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(CasualAimingModifier = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(LookingVelocityMaximum = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(LookingAccelerationMax = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(new TI.Pad(8));
			Add(AiVehicleRadius = new TI.Real());
			Add(AiDangerRadius = new TI.Real());
			Add(MeleeDamage = new TI.TagReference(this, TagGroups.jpt_));
			Add(MotionSensorBlipSize = new TI.Enum());
			Add(new TI.Pad(2 + 12));
			Add(NewHudInterfaces = new TI.Block<unit_hud_reference_block>(this, 2));
			Add(DialogueVariants = new TI.Block<dialogue_variant_block>(this, 16));
			Add(GrenadeVelocity = new TI.Real());
			Add(GrenadeType = new TI.Enum());
			Add(GrenadeCount = new TI.ShortInteger());
			Add(new TI.Pad(4));
			Add(PoweredSeats = new TI.Block<powered_seat_block>(this, 2));
			Add(Weapons = new TI.Block<unit_weapon_block>(this, 4));
			Add(Seats = new TI.Block<unit_seat_block>(this, 16));
		}
	};
	#endregion


	#region projectile
	public partial class projectile_group
	{
		#region projectile_material_response_block
		public partial class projectile_material_response_block
		{
			public projectile_material_response_block() : base(21)
			{
				Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));

				Add(Response = new TI.Enum());
				Add(Effect = new TI.TagReference(this, TagGroups.effe));
				Add(new TI.Pad(16));

				Add(PotentialResponse = new TI.Enum());
				Add(PotentialFlags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
				Add(SkipFraction = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
				Add(Between = new TI.RealBounds(BlamLib.TagInterface.FieldType.AngleBounds));
				Add(And = new TI.RealBounds());
				Add(PotentialEffect = new TI.TagReference(this, TagGroups.effe));
				Add(new TI.Pad(16));

				Add(ScaleEffectsBy = new TI.Enum());
				Add(TI.Pad.Word);
				Add(AngularNoise = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
				Add(VelocityNoise = new TI.Real());
				Add(DetonationEffect = new TI.TagReference(this, TagGroups.effe));
				Add(new TI.Pad(24));

				Add(InitialFriction = new TI.Real());
				Add(MaxDist = new TI.Real());

				Add(ParallelFriction = new TI.Real());
				Add(PerpendicularFriction = new TI.Real());
			}
		};
		#endregion

		public projectile_group() : base(31)
		{
			Add(Flags = new TI.Flags());
			Add(DetonationTimerStarts = new TI.Enum());
			Add(ImpactNoise = new TI.Enum());
			Add(AinProjectile = new TI.Enum());
			Add(BinProjectile = new TI.Enum());
			Add(CinProjectile = new TI.Enum());
			Add(DinProjectile = new TI.Enum());
			Add(SuperDetonation = new TI.TagReference(this, TagGroups.effe));
			Add(AiPerceptionRadius = new TI.Real());
			Add(CollisionRadius = new TI.Real());

			Add(ArmingTime = new TI.Real());
			Add(DangerRadius = new TI.Real());
			Add(Effect = new TI.TagReference(this, TagGroups.effe));
			Add(Timer = new TI.RealBounds());
			Add(MinVelocity = new TI.Real());
			Add(MaxRange = new TI.Real());

			Add(AirGravityScale = new TI.Real());
			Add(AirDamageRange = new TI.RealBounds());
			Add(WaterGravityScale = new TI.Real());
			Add(WaterDamageRange = new TI.RealBounds());
			Add(InitialVelocity = new TI.Real());
			Add(FinalVelocity = new TI.Real());
			Add(GuidedAngularVelocity = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			
			Add(DetonationNoise = new TI.Enum());
			Add(new TI.Pad(2));
			Add(DetonationStarted = new TI.TagReference(this, TagGroups.effe));
			Add(FlybySound = new TI.TagReference(this, TagGroups.snd_));
			Add(AttachedDetonationDamage = new TI.TagReference(this, TagGroups.jpt_));
			Add(ImpactDamage = new TI.TagReference(this, TagGroups.jpt_));
			Add(new TI.Pad(12));
			Add(MaterialResponses = new TI.Block<projectile_material_response_block>(this, 33));
		}
	};
	#endregion


	#region device_control
	public partial class device_control_group
	{
		public device_control_group() : base(7)
		{
			Add(Type = new TI.Enum());
			Add(TriggersWhen = new TI.Enum());
			Add(CallValue = new TI.Real());
			Add(new TI.Pad(80));
			Add(On = new TI.TagReference(this));
			Add(Off = new TI.TagReference(this));
			Add(Deny = new TI.TagReference(this));
		}
	};
	#endregion

	#region device_machine
	public partial class device_machine_group
	{
		public device_machine_group() : base(8)
		{
			Add(Type = new TI.Enum());
			Add(Flags = new TI.Flags(BlamLib.TagInterface.FieldType.WordFlags));
			Add(DoorOpenTime = new TI.Real());
			Add(new TI.Pad(80));
			Add(CollisionResponse = new TI.Enum());
			Add(ElevatorNode = new TI.ShortInteger());
			Add(new TI.Pad(52));
			Add(DoorOpenGameTime = new TI.Skip(4));
		}
	};
	#endregion


	#region equipment
	public partial class equipment_group
	{
		public equipment_group() : base(5)
		{
			Add(PowerupType = new TI.Enum());
			Add(GrenadeType = new TI.Enum());
			Add(PowerupTime = new TI.Real());
			Add(PickupSound = new TI.TagReference(this, TagGroups.snd_));
			Add(new TI.Pad(144)); // probably padding for 9 tag references
		}
	};
	#endregion

	#region weapon
	public partial class weapon_group
	{
		#region magazines_block
		public partial class magazines_block
		{
			#region magazine_objects_block
			public partial class magazine_objects_block
			{
				public magazine_objects_block() : base(3)
				{
					Add(Rounds = new TI.ShortInteger());
					Add(new TI.Pad(10));
					Add(Equipment = new TI.TagReference(this, TagGroups.eqip));
				}
			};
			#endregion

			public magazines_block() : base(15)
			{
				Add(Flags = new TI.Flags());
				Add(RoundsRecharged = new TI.ShortInteger());
				Add(RoundsTotalInitial = new TI.ShortInteger());
				Add(RoundsTotalMax = new TI.ShortInteger());
				Add(RoundsLoadedMax = new TI.ShortInteger());
				Add(new TI.Pad(8));
				Add(ReloadTime = new TI.Real());
				Add(RoundsReloaded = new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(ChamberTime = new TI.Real());
				Add(new TI.Pad(8 + 16));
				Add(ReloadingEffect = new TI.TagReference(this));
				Add(ChamberingEffect = new TI.TagReference(this));
				Add(new TI.Pad(12));
				Add(Magazines = new TI.Block<magazine_objects_block>(this, 8));
			}
		};
		#endregion

		#region triggers_block
		public partial class triggers_block
		{
			#region trigger_firing_effect_block
			public partial class trigger_firing_effect_block
			{
				public trigger_firing_effect_block() : base(8)
				{
					Add(ShotCount = new TI.ShortIntegerBounds());
					Add(new TI.Pad(32));
					Add(FiringEffect = new TI.TagReference(this));
					Add(MisfireEffect = new TI.TagReference(this));
					Add(EmptyEffect = new TI.TagReference(this));
					Add(FiringDamage = new TI.TagReference(this, TagGroups.jpt_));
					Add(MisfireDamage = new TI.TagReference(this, TagGroups.jpt_));
					Add(EmtpyDamage = new TI.TagReference(this, TagGroups.jpt_));
				}
			};
			#endregion

			public triggers_block() : base(41)
			{
				Add(Flags = new TI.Flags());

				Add(RoundsPerSecond = new TI.RealBounds());
				Add(FiringAccelerationTime = new TI.Real());
				Add(FiringDecelerationTime = new TI.Real());
				Add(BlurredRateOfFire = new TI.Real());
				Add(new TI.Pad(8));
				Add(Magazine = new TI.BlockIndex());
				Add(RoundsPerShot = new TI.ShortInteger());
				Add(MinRoundsLoaded = new TI.ShortInteger());
				Add(RoundsBetweenTracers = new TI.ShortInteger());
				Add(new TI.Pad(6));
				Add(FiringNoise = new TI.Enum());

				Add(Error = new TI.RealBounds());
				Add(ErrorAccelerationTime = new TI.Real());
				Add(ErrorDecelerationTime = new TI.Real());
				Add(new TI.Pad(8));

				Add(ChargingTime = new TI.Real());
				Add(ChargedTime = new TI.Real());
				Add(OverchargedAction = new TI.Enum());
				Add(new TI.Pad(2));
				Add(ChargedIllumination = new TI.Real());
				Add(SpewTime = new TI.Real());
				Add(ChargingEffect = new TI.TagReference(this));

				Add(DistributionFunction = new TI.Enum());
				Add(ProjectilesPerShot = new TI.ShortInteger());
				Add(DistributionAngle = new TI.Real());
				Add(new TI.Pad(4));
				Add(MinError = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
				Add(ErrorAngle = new TI.RealBounds(BlamLib.TagInterface.FieldType.AngleBounds));
				Add(FirstPersionOffset = new TI.RealPoint3D());
				Add(new TI.Pad(4));
				Add(Projectile = new TI.TagReference(this, TagGroups.proj));

				Add(EjectionPortRecoveryTime = new TI.Real());
				Add(IlluminationRecoveryTime = new TI.Real());
				Add(new TI.Pad(12));
				Add(HeatGeneratedPerRound = new TI.Real());
				Add(AgeGeneratedPerRound = new TI.Real());
				Add(new TI.Pad(4));
				Add(OverloadTime = new TI.Real());
				Add(new TI.Pad(8 + 32 + 24));
				
				Add(FiringEffects = new TI.Block<trigger_firing_effect_block>(this, 8));
			}
		};
		#endregion

		public weapon_group() : base(67)
		{
			Add(Flags = new TI.Flags());
			Add(Label = new TI.String());
			Add(SecondaryTriggerMode = new TI.Enum());
			Add(MaxAltShotsLoaded = new TI.ShortInteger());
			Add(AinWeapon = new TI.Enum());
			Add(BinWeapon = new TI.Enum());
			Add(CinWeapon = new TI.Enum());
			Add(DinWeapon = new TI.Enum());
			Add(ReadyTime = new TI.Real());
			Add(ReadyEffect = new TI.TagReference(this));
			Add(HeatRecoveryThreshold = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(OverheatedThreshold = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(HeatDetonationThreshold = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(HeatDetonationFraction = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(HeatLossPerSecond = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(HeatIllumination = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(new TI.Pad(16));
			Add(Overheated = new TI.TagReference(this));
			Add(Detonation = new TI.TagReference(this));
			Add(PlayerMeleeDamage = new TI.TagReference(this, TagGroups.jpt_));
			Add(PlayerMeleeResponse = new TI.TagReference(this, TagGroups.jpt_));
			Add(new TI.Pad(8));

			Add(ActorFiringParameters = new TI.TagReference(this, TagGroups.actv));

			Add(NearRecticleRange = new TI.Real());
			Add(FarReticleRange = new TI.Real());
			Add(IntersectionRecticleRange = new TI.Real());
			
			Add(new TI.Pad(2));
			Add(MagnificationLevel = new TI.ShortInteger());
			Add(MagnificationRange = new TI.RealBounds());

			Add(AutoaimAngle = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(AutoaimRange = new TI.Real());
			Add(MagnetismAngle = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(MagnetismRange = new TI.Real());
			Add(DeviationAngle = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(new TI.Pad(4));

			Add(MovementPenalized = new TI.Enum());
			Add(new TI.Pad(2));
			Add(ForwardMovementPenalty = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(SidewaysMovementPenalty = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(new TI.Pad(4));

			Add(MinTargetRange = new TI.Real());
			Add(LookingTimeModifier = new TI.Real());
			Add(new TI.Pad(4));

			Add(LightPowerOnTime = new TI.Real());
			Add(LightPowerOffTime = new TI.Real());
			Add(LightPowerOnEffect = new TI.TagReference(this));
			Add(LightPowerOffEffect = new TI.TagReference(this));
			Add(AgeHeatRecoveryPenalty = new TI.Real());
			Add(AgeRateOfFirePenalty = new TI.Real());
			Add(AgeMisfireStart = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(AgeMisfireChance = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
			Add(new TI.Pad(12));

			Add(FirstPersonModel = new TI.TagReference(this, TagGroups.mode));
			Add(FirstPersonAnimations = new TI.TagReference(this, TagGroups.ant_));
			Add(new TI.Pad(4));
			Add(HudInterface = new TI.TagReference(this, TagGroups.wphi));
			Add(PickupSound = new TI.TagReference(this, TagGroups.snd_));
			Add(ZoomInSound = new TI.TagReference(this, TagGroups.snd_));
			Add(ZoomOutSound = new TI.TagReference(this, TagGroups.snd_));
			Add(new TI.Pad(12));
			Add(ActiveCamoDing = new TI.Real());
			Add(ActiveCamoRegrowth = new TI.Real());
			Add(new TI.Pad(12 + 2));

			Add(WeaponType = new TI.Enum());
			Add(PredictedResourcesWeapon = new TI.Block<predicted_resource_block>(this, 1024));
			Add(Magazines = new TI.Block<magazines_block>(this, 2));
			Add(Triggers = new TI.Block<triggers_block>(this, 2));
		}
	};
	#endregion


	#region biped
	public partial class biped_group
	{
		#region contact_point_block
		public partial class contact_point_block
		{
			public contact_point_block() : base(2)
			{
				Add(new TI.Pad(32));
				Add(MarkerName = new TI.String());
			}
		};
		#endregion

		public biped_group() : base(51)
		{
			Add(MovingTurningSpeed = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(Flags = new TI.Flags());
			Add(StationaryTurningThreshold = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(new TI.Pad(16));
			Add(AinBiped = new TI.Enum());
			Add(BinBiped = new TI.Enum());
			Add(CinBiped = new TI.Enum());
			Add(DinBiped = new TI.Enum());
			Add(DontUse = new TI.TagReference(this, TagGroups.jpt_));

			Add(BankAngle = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(BankApplyTime = new TI.Real());
			Add(BankDecayTime = new TI.Real());
			Add(PitchRatio = new TI.Real());
			Add(MaxVelocity = new TI.Real());
			Add(MaxSidestepVelocity = new TI.Real());
			Add(Acceleration = new TI.Real());
			Add(Deceleration = new TI.Real());
			Add(AngularVelocityMax = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(AngularAccelerationMax = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(CrouchVelocityModifier = new TI.Real());
			Add(new TI.Pad(8));

			Add(MaxSlopeAngle = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(DownhillFalloffAngle = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(DownhillCutoffAngle = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(DownhillVelocityScale = new TI.Real());
			Add(UphillFalloffAngle = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(UphillCutoffAngle = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(UphillVelocityScale = new TI.Real());
			Add(new TI.Pad(24));
			Add(Footsteps = new TI.TagReference(this, TagGroups.foot));
			Add(new TI.Pad(24));

			Add(JumpVelocity = new TI.Real());
			Add(new TI.Pad(28));
			Add(MaxSoftLandingTime = new TI.Real());
			Add(MaxHardLandingTime = new TI.Real());
			Add(MinSoftLandingVelocity = new TI.Real());
			Add(MinHardLandingVelocity = new TI.Real());
			Add(MaxHardLandingVelocity = new TI.Real());
			Add(DeathHardLandingVelocity = new TI.Real());
			Add(new TI.Pad(20));

			Add(StandingCameraHeight = new TI.Real());
			Add(CrouchingCameraHeight = new TI.Real());
			Add(CrouchTransitionTime = new TI.Real());
			Add(new TI.Pad(24));
			Add(StandingCollisionHeight = new TI.Real());
			Add(CrouchingCollisionHeight = new TI.Real());
			Add(CollisionRadius = new TI.Real());
			Add(new TI.Pad(40));
			Add(AutoaimWidth = new TI.Real());
			Add(new TI.Pad(140));

			Add(ContactPoints = new TI.Block<contact_point_block>(this, 2));
		}
	};
	#endregion

	#region vehicle
	public partial class vehicle_group
	{
		public vehicle_group() : base(42)
		{
			Add(Flags = new TI.Flags());
			Add(Type = new TI.Enum());
			Add(new TI.Pad(2));
			Add(MaxForwardSpeed = new TI.Real());
			Add(MaxReverseSpeed = new TI.Real());
			Add(SpeedAcceleration = new TI.Real());
			Add(SpeedDeceleration = new TI.Real());
			Add(MaxLeftTurn = new TI.Real());
			Add(MaxRightTurn = new TI.Real());
			Add(WheelCircumference = new TI.Real());
			Add(TurnRate = new TI.Real());
			Add(BlurSpeed = new TI.Real());
			Add(AinVehicle = new TI.Enum());
			Add(BinVehicle = new TI.Enum());
			Add(CinVehicle = new TI.Enum());
			Add(DinVehicle = new TI.Enum());
			Add(new TI.Pad(12));
			Add(MaxLeftSlide = new TI.Real());
			Add(MaxRightSlide = new TI.Real());
			Add(SlideAcceleration = new TI.Real());
			Add(SlideDecleration = new TI.Real());
			Add(MinFlippingAngularVelocity = new TI.Real());
			Add(MaxFlippingAngularVelocity = new TI.Real());
			Add(new TI.Pad(24));
			Add(FixedGunYaw = new TI.Real());
			Add(FixedGunPitch = new TI.Real());
			Add(new TI.Pad(24));
			Add(AiSideslipDist = new TI.Real());
			Add(AiDestRadius = new TI.Real());
			Add(AiAvoidanceDist = new TI.Real());
			Add(AiPathfindingRadius = new TI.Real());
			Add(AiChargeRepeatTimeout = new TI.Real());
			Add(AiStrafingAbortRange = new TI.Real());
			Add(AiOverseeringBounds = new TI.RealBounds(BlamLib.TagInterface.FieldType.AngleBounds));
			Add(AiSteeringMax = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
			Add(AiThrottleMax = new TI.Real());
			Add(AiMovePositionTime = new TI.Real());
			Add(new TI.Pad(4));
			Add(SuspensionSound = new TI.TagReference(this, TagGroups.snd_));
			Add(CrashSound = new TI.TagReference(this, TagGroups.snd_));
			Add(MaterialEffects = new TI.TagReference(this, TagGroups.foot));
			Add(Effect = new TI.TagReference(this, TagGroups.effe));
		}
	};
	#endregion


	#region item_collection
	public partial class item_collection_group
	{
		#region item_permutation_block
		public partial class item_permutation_block
		{
			public item_permutation_block() : base(4)
			{
				Add(new TI.Pad(32));
				Add(Weight = new TI.Real());
				Add(Item = new TI.TagReference(this, TagGroups.item));
				Add(new TI.Pad(32));
			}
		};
		#endregion

		public item_collection_group() : base(3)
		{
			Add(ItemPermutations = new TI.Block<item_permutation_block>(this, 32));
			Add(SpawnTime = new TI.ShortInteger());
			Add(new TI.Pad(2 + 76));
		}
	};
	#endregion
}