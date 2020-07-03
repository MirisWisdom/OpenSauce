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
	public partial class globals_group
	{
		#region player_control_block
		public partial class player_control_block
		{
			public player_control_block() : base(15)
			{
				Add(MagnetismFriction = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
				Add(MagnetismAdhesion = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
				Add(InconsequentialTargetScale = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
				Add(new TI.Pad(52));
				Add(LookAccelerationTime = new TI.Real());
				Add(LookAccelerationScale = new TI.Real());
				Add(LookPegThreshold = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
				Add(LookDefaultPitchRate = new TI.Real());
				Add(LookDefaultYawRate = new TI.Real());
				Add(LookAutolevelingScale = new TI.Real());
				Add(new TI.Pad(20));
				Add(MinWeaponSwapTicks = new TI.ShortInteger());
				Add(MinAutolevelingTicks = new TI.ShortInteger());
				Add(MinAngleForVehicleFlipping = new TI.Real(BlamLib.TagInterface.FieldType.Angle));
				Add(LookFunction = new TI.Block<field_block<TI.Real>>(this, 16));
			}
		};
		#endregion

		#region difficulty_block
		public partial class difficulty_block
		{
			public difficulty_block() : base(108)
			{
				Health = new TI.Real[36];
				for (int x = 0; x < Health.Length; x++)
					Add(Health[x] = new TI.Real());
				Add(new TI.Pad(16));

				RangedFire = new TI.Real[48];
				for (int x = 0; x < RangedFire.Length; x++)
					Add(RangedFire[x] = new TI.Real());
				Add(new TI.Pad(16));

				Grenades = new TI.Real[8];
				for (int x = 0; x < Grenades.Length; x++)
					Add(Grenades[x] = new TI.Real());
				Add(new TI.Pad(16 * 3));

				Placement = new TI.Real[12];
				for (int x = 0; x < Placement.Length; x++)
					Add(Placement[x] = new TI.Real());
				Add(new TI.Pad(16 * 4 + 84));
			}
		};
		#endregion

		#region grenades_block
		public partial class grenades_block
		{
			public grenades_block() : base(6)
			{
				Add(MaxCount = new TI.ShortInteger());
				Add(new TI.Pad(2));
				Add(ThrowingEffect = new TI.TagReference(this, TagGroups.effe));
				Add(HudInterface = new TI.TagReference(this, TagGroups.grhi));
				Add(Equipment = new TI.TagReference(this, TagGroups.eqip));
				Add(Projectile = new TI.TagReference(this, TagGroups.proj));
			}
		};
		#endregion

		#region rasterizer_data_block
		public partial class rasterizer_data_block
		{
			public rasterizer_data_block() : base(27)
			{
				Add(DistanceAttenuation = new TI.TagReference(this, TagGroups.bitm));
				Add(VectorNormalization = new TI.TagReference(this, TagGroups.bitm));
				Add(AtmosphereicFogDensity = new TI.TagReference(this, TagGroups.bitm));
				Add(PlanarFogDensity = new TI.TagReference(this, TagGroups.bitm));
				Add(LinearCornerFade = new TI.TagReference(this, TagGroups.bitm));
				Add(ActiveCamoflageDistortion = new TI.TagReference(this, TagGroups.bitm));
				Add(Glow = new TI.TagReference(this, TagGroups.bitm));
				Add(new TI.Pad(60));

				Add(Default2D = new TI.TagReference(this, TagGroups.bitm));
				Add(Default3D = new TI.TagReference(this, TagGroups.bitm));
				Add(DefaultCubeMap = new TI.TagReference(this, TagGroups.bitm));

				Add(Test0 = new TI.TagReference(this, TagGroups.bitm));
				Add(Test1 = new TI.TagReference(this, TagGroups.bitm));
				Add(Test2 = new TI.TagReference(this, TagGroups.bitm));
				Add(Test3 = new TI.TagReference(this, TagGroups.bitm));

				Add(VideoScanlineMap = new TI.TagReference(this, TagGroups.bitm));
				Add(VideoNoiseMap = new TI.TagReference(this, TagGroups.bitm));
				Add(new TI.Pad(52));

				Add(Flags = new TI.Flags(TI.FieldType.WordFlags));
				Add(new TI.Pad(2));
				Add(RefractionAmount = new TI.Real());
				Add(DistanceFalloff = new TI.Real());
				Add(TintColor = new TI.RealColor());
				Add(HyperStealthRefraction = new TI.Real());
				Add(HyperStealthDistanceFalloff = new TI.Real());
				Add(HyperStealthTintColor = new TI.RealColor());

				Add(DistanceAttenuation2D = new TI.TagReference(this, TagGroups.bitm));
			}
		};
		#endregion

		#region interface_tag_references_block
		public partial class interface_tag_references_block
		{
			public interface_tag_references_block() : base(17)
			{
				Fields = new TI.TagReference[16];
				for (int x = 0; x < Fields.Length; x++)
					Add(Fields[x] = new TI.TagReference(this));
				Add(new TI.Pad(48));

				Fields[0].ResetGroupTag(TagGroups.font);
				Fields[1].ResetGroupTag(TagGroups.font);
				Fields[2].ResetGroupTag(TagGroups.colo);
				Fields[3].ResetGroupTag(TagGroups.colo);
				Fields[4].ResetGroupTag(TagGroups.colo);
				Fields[5].ResetGroupTag(TagGroups.colo);
				Fields[6].ResetGroupTag(TagGroups.hudg);
				Fields[7].ResetGroupTag(TagGroups.bitm);
				Fields[8].ResetGroupTag(TagGroups.bitm);
				Fields[9].ResetGroupTag(TagGroups.bitm);
				Fields[10].ResetGroupTag(TagGroups.str_);
				Fields[11].ResetGroupTag(TagGroups.hud_);
				Fields[12].ResetGroupTag(TagGroups.bitm);
				Fields[13].ResetGroupTag(TagGroups.bitm);
				Fields[14].ResetGroupTag(TagGroups.bitm);
				Fields[15].ResetGroupTag(TagGroups.bitm);
			}
		};
		#endregion

		#region multiplayer_information_block
		public partial class multiplayer_information_block
		{
			public multiplayer_information_block() : base(8)
			{
				Add(Flag = new TI.TagReference(this, TagGroups.item));
				Add(Unit = new TI.TagReference(this, TagGroups.unit));
				Add(Vehicles = new TI.Block<field_block<TI.TagReference>>(this, 20));
				Add(HillShader = new TI.TagReference(this, TagGroups.shdr));
				Add(FlagShader = new TI.TagReference(this, TagGroups.shdr));
				Add(Ball = new TI.TagReference(this, TagGroups.item));
				Add(Sounds = new TI.Block<field_block<TI.TagReference>>(this, 60));
				Add(new TI.Pad(56));

				//Vehicles.Definition.Value.ResetReferenceGroupTag(TagGroups.vehi);
				//Sounds.Definition.Value.ResetReferenceGroupTag(TagGroups.snd_);
			}
		};
		#endregion

		#region player_information_block
		public partial class player_information_block
		{
			public player_information_block() : base(27)
			{
				Add(Unit = new TI.TagReference(this, TagGroups.unit));
				Add(new TI.Pad(28));
				Add(WalkingSpeed = new TI.Real());
				Add(DoubleSpeedMul = new TI.Real());
				Add(RunForward = new TI.Real());
				Add(RunBackward = new TI.Real());
				Add(RunSideways = new TI.Real());
				Add(RunAcceleration = new TI.Real());
				Add(SneakForward = new TI.Real());
				Add(SneakBackward = new TI.Real());
				Add(SneakSideways = new TI.Real());
				Add(SneakAcceleration = new TI.Real());
				Add(AirborneAcceleration = new TI.Real());
				Add(new TI.Pad(16));
				Add(GrenadeOrigin = new TI.RealPoint3D());
				Add(new TI.Pad(12));
				Add(StunMovementPenalty = new TI.Real());
				Add(StunTurningPenalty = new TI.Real());
				Add(StunJumpingPenalty = new TI.Real());
				Add(MinStunTime = new TI.Real());
				Add(MaxStunTime = new TI.Real());
				Add(new TI.Pad(8));
				Add(FirstPersionIdleTime = new TI.RealBounds());
				Add(FirstPersionSkipFraction = new TI.Real(BlamLib.TagInterface.FieldType.RealFraction));
				Add(new TI.Pad(16));
				Add(CoopRespawnEffect = new TI.TagReference(this, TagGroups.effe));
				Add(new TI.Pad(44));
			}
		};
		#endregion

		#region first_person_interface_block
		public partial class first_person_interface_block
		{
			public first_person_interface_block() : base(9)
			{
				Add(FirstPersionHands = new TI.TagReference(this, TagGroups.mod2));
				Add(BaseBitmap = new TI.TagReference(this, TagGroups.bitm));
				Add(ShieldMeter = new TI.TagReference(this, TagGroups.metr));
				Add(ShieldMeterOrigin = new TI.Point2D());
				Add(BodyMeter = new TI.TagReference(this, TagGroups.metr));
				Add(BodyMeterOrigin = new TI.Point2D());
				Add(NightVisionTurnOnEffect = new TI.TagReference(this, TagGroups.effe));
				Add(NightVisionTurnOffEffect = new TI.TagReference(this, TagGroups.effe));
				Add(new TI.Pad(88));
			}
		};
		#endregion

		#region falling_damage_block
		public partial class falling_damage_block
		{
			public falling_damage_block() : base(11)
			{
				Add(new TI.Pad(8));
				Add(HarmfulFallingDist = new TI.RealBounds());
				Add(FallingDamage = new TI.TagReference(this, TagGroups.jpt_));
				Add(new TI.Pad(8));
				Add(MaxFallingDist = new TI.Real());
				Add(DistDamage = new TI.TagReference(this, TagGroups.jpt_));
				Add(VehicleEnvironmentCollisionDamage = new TI.TagReference(this, TagGroups.jpt_));
				Add(VehicleKilledUnitDamageEffect = new TI.TagReference(this, TagGroups.jpt_));
				Add(VehicleCollisionDamage = new TI.TagReference(this, TagGroups.jpt_));
				Add(FlamingDeathDamage = new TI.TagReference(this, TagGroups.jpt_));
				Add(new TI.Pad(28));
			}
		};
		#endregion

		#region materials_block
		public partial class materials_block
		{
			#region breakable_surface_particle_effect_block
			public partial class breakable_surface_particle_effect_block
			{
				public breakable_surface_particle_effect_block() : base(12)
				{
					Add(ParticleType = new TI.TagReference(this, TagGroups.part));
					Add(Flags = new TI.Flags());
					Add(Density = new TI.Real());
					Add(VelocityScale = new TI.RealBounds());
					Add(new TI.Pad(4));
					Add(AngularVelocity = new TI.RealBounds(BlamLib.TagInterface.FieldType.AngleBounds));
					Add(new TI.Pad(8));
					Add(Radius = new TI.RealBounds());
					Add(new TI.Pad(8));
					Add(TintLowerBound = new TI.RealColor(BlamLib.TagInterface.FieldType.RealArgbColor));
					Add(TintUpperBound = new TI.RealColor(BlamLib.TagInterface.FieldType.RealArgbColor));
					Add(new TI.Pad(28));
				}
			};
			#endregion

			public materials_block() : base(15)
			{
				Add(new TI.Pad(100 + 48));
				Add(GroundFrictionScale = new TI.Real());
				Add(GroundFrictionNormalK1Scale = new TI.Real());
				Add(GroundFrictionNormalK0Scale = new TI.Real());
				Add(GroundDepthScale = new TI.Real());
				Add(GroundDampFractionScale = new TI.Real());
				Add(new TI.Pad(76 + 480));
				Add(MaxVitality = new TI.Real());
				Add(new TI.Pad(8 + 4));
				Add(Effect = new TI.TagReference(this, TagGroups.effe));
				Add(Sound = new TI.TagReference(this, TagGroups.snd_));
				Add(new TI.Pad(24));
				Add(ParticleEffects = new TI.Block<breakable_surface_particle_effect_block>(this, 8));
				Add(new TI.Pad(60));
				Add(MeleeHitSound = new TI.TagReference(this, TagGroups.snd_));
			}
		};
		#endregion

		#region playlist_autogenerate_choice_block
		public partial class playlist_autogenerate_choice_block
		{
			public playlist_autogenerate_choice_block() : base(8)
			{
				Add(MapName = new TI.String());
				Add(GameVariant = new TI.String());
				Add(MinExperience = new TI.LongInteger());
				Add(MaxExperience = new TI.LongInteger());
				Add(MinPlayerCount = new TI.LongInteger());
				Add(MaxPlayerCount = new TI.LongInteger());
				Add(Rating = new TI.LongInteger());
				Add(new TI.Pad(64));
			}
		};
		#endregion

		public globals_group() : base(16)
		{
			Add(new TI.Pad(248));
			Add(Sounds = new TI.Block<field_block<TI.TagReference>>(this, 2));
			Add(Camera = new TI.Block<field_block<TI.TagReference>>(this, 1));
			Add(PlayerControl = new TI.Block<player_control_block>(this, 1));
			Add(Difficulty = new TI.Block<difficulty_block>(this, 1));
			Add(Grenades = new TI.Block<grenades_block>(this, 2));
			Add(RasterizerData = new TI.Block<rasterizer_data_block>(this, 1));
			Add(InterfaceBitmaps = new TI.Block<interface_tag_references_block>(this, 1));
			Add(WeaponList = new TI.Block<field_block<TI.TagReference>>(this, 20));
			Add(CheatPowerups = new TI.Block<field_block<TI.TagReference>>(this, 20));
			Add(MultiplayerInformation = new TI.Block<multiplayer_information_block>(this, 1));
			Add(PlayerInformation = new TI.Block<player_information_block>(this, 1));
			Add(FirstPersionInterface = new TI.Block<first_person_interface_block>(this, 1));
			Add(FallingDamage = new TI.Block<falling_damage_block>(this, 1));
			Add(Materials = new TI.Block<materials_block>(this, 33));
			Add(PlayerlistMembers = new TI.Block<playlist_autogenerate_choice_block>(this, 20));

			//Sounds.Definition.Value.ResetReferenceGroupTag(TagGroups.snd_);
			//Camera.Definition.Value.ResetReferenceGroupTag(TagGroups.trak);
			//WeaponList.Definition.Value.ResetReferenceGroupTag(TagGroups.item);
			//CheatPowerups.Definition.Value.ResetReferenceGroupTag(TagGroups.eqip);
		}
	};
	#endregion
}