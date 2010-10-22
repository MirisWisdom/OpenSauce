/*
    BlamLib: .NET SDK for the Blam Engine

    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using System.Collections.Generic;
using System.Text;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo3
{
	public static class BlamFile
	{
		#region Blam File Groups
		/// <summary>
		/// All tag groups in Halo 3 BLF files
		/// </summary>
		public static TagInterface.TagGroupCollection Groups = new TagInterface.TagGroupCollection(false,
			MiscGroups._blf, // 1
			MiscGroups._eof, // 1
			MiscGroups.chdr, // 9
			MiscGroups.athr, // 3
			MiscGroups.flmh, // A
			MiscGroups.flmd, // A
			MiscGroups.mpvr, // 3
			MiscGroups.mapv, // C
			MiscGroups.levl, // 3
			MiscGroups.cmpn, // 1

			MiscGroups.mapi, // 1
			MiscGroups.gvar, // A
			MiscGroups.mvar, // C
			MiscGroups.onfm, // 1
			MiscGroups.srid, // 2
			MiscGroups.scnd, // 1
			MiscGroups.netc, // 7D
			MiscGroups.mapm, // 1
			MiscGroups.fubh, // 1
			MiscGroups.funs, // 1
			MiscGroups.fupd, // 1
			MiscGroups.filq, // 1
			MiscGroups.furp, // 2
			MiscGroups.bhms, // 1
			MiscGroups.mmhs, // 3
			MiscGroups.motd, // 1
			MiscGroups._cmp, // 1
			MiscGroups.mmtp, // 1
			MiscGroups.mhcf, // B
			MiscGroups.mhdf, // 3
			MiscGroups.gset, // 6
			MiscGroups.mdsc
			);

		/// <summary>
		/// Constant indices to each tag group in <see cref="Groups"/>
		/// </summary>
		public enum Enumerated : int
		{
			_blf,
			_eof,
			chdr,
			athr,
			flmh,
			flmd,
			mpvr,
			mapv,
			levl,
			cmpn,

			mapi,
			gvar,
			mvar,
			onfm,
			srid,
			scnd,
			netc,
			mapm,
			fubh,
			funs,
			fupd,
			filq,
			furp,
			bhms,
			mmhs,
			motd,
			_cmp,
			mmtp,
			mhcf,
			mhdf,
			gset,
			mdsc,
		};

		internal static void InitializeBlfGroups()
		{
			MiscGroups._blf.EngineData = typeof(BLF);
			MiscGroups._eof.EngineData = typeof(EOF);
			MiscGroups.chdr.EngineData = typeof(CHDR);
			MiscGroups.athr.EngineData = typeof(ATHR);
			MiscGroups.flmh.EngineData = typeof(FLMH);
			MiscGroups.flmd.EngineData = typeof(FLMD);
			MiscGroups.mpvr.EngineData = typeof(MPVR);
			MiscGroups.mapv.EngineData = typeof(MAPV);
			MiscGroups.levl.EngineData = typeof(LEVL);
			MiscGroups.cmpn.EngineData = typeof(CMPN);

			MiscGroups.mapi.EngineData = typeof(MAPI);
			MiscGroups.gvar.EngineData = typeof(GVAR);
			MiscGroups.mvar.EngineData = typeof(MVAR);
			MiscGroups.onfm.EngineData = typeof(ONFM);
			MiscGroups.srid.EngineData = typeof(SRID);
			MiscGroups.scnd.EngineData = typeof(SCND);
			MiscGroups.netc.EngineData = typeof(NETC);
			MiscGroups.mapm.EngineData = typeof(MAPM);
			MiscGroups.fubh.EngineData = typeof(FUBH);
			MiscGroups.funs.EngineData = typeof(FUNS);
			MiscGroups.fupd.EngineData = typeof(FUPD);
			MiscGroups.filq.EngineData = typeof(FILQ);
			MiscGroups.furp.EngineData = typeof(FURP);
			MiscGroups.bhms.EngineData = typeof(BHMS);
			MiscGroups.mmhs.EngineData = typeof(MMHS);
			MiscGroups.motd.EngineData = typeof(MOTD);
			MiscGroups._cmp.EngineData = typeof(CMP);
			MiscGroups.mmtp.EngineData = typeof(MMTP);
			MiscGroups.mhcf.EngineData = typeof(MHCF);
			MiscGroups.mhdf.EngineData = typeof(MHDF);
			MiscGroups.gset.EngineData = typeof(GSET);
			MiscGroups.mdsc.EngineData = typeof(MDSC);
		}
		#endregion

		public enum ContentFileType : uint
		{
			[TI.Enum("Memory-Mapped IO File")]			GameVariant_MMIOF, // 3
			[TI.Enum("Game Variant - CTF")]				GameVariant_Ctf, // 4
			[TI.Enum("Game Variant - Slayer")]			GameVariant_Slayer, // 5
			[TI.Enum("Game Variant - Oddball")]			GameVariant_Oddball, // 6
			[TI.Enum("Game Variant - King of the Hill")]GameVariant_King, // 7
			[TI.Enum("Game Variant - Juggernaut")]		GameVariant_Juggernaut, // 8
			[TI.Enum("Game Variant - Territories")]		GameVariant_Territories, // 9
			[TI.Enum("Game Variant - Assault")]			GameVariant_Assault, // 11
			[TI.Enum("Game Variant - Infection")]		GameVariant_Infection, // 10
			[TI.Enum("Game Variant - VIP")]				GameVariant_Vip, // 1
			[TI.Enum("Map Variant")]					MapVariant, // 12
			[TI.Enum("Media - Film")]					Film, // 13
			[TI.Enum("Media - Snippet")]				Snippet, // 14
			[TI.Enum("Media - Screenshot")]				Screenshot,
		};

		public enum GameEngineType : int
		{
			[TI.Enum("None")]				None,			// 0x1B0
			[TI.Enum("Capture the Flag")]	Ctf,			// 0x1D8
			[TI.Enum("Slayer")]				Slayer,			// 0x1E0
			[TI.Enum("Oddball")]			Oddball,		// 0x1E0
			[TI.Enum("King")]				King,			// 0x1D8
			[TI.Enum("Sandbox")]			Sandbox,		// 0x1D0
			[TI.Enum("VIP")]				Vip,			// 0x218
			[TI.Enum("Juggernaut")]			Juggernaut,		// 0x1E0
			[TI.Enum("Territories")]		Territories,	// 0x1F0
			[TI.Enum("Assault")]			Assault,		// 0x200
			[TI.Enum("Infection")]			Infection,		// 0x230
		};

		public enum ObjectType
		{
			[TI.Enum("Unit - Biped")]				Biped,
			[TI.Enum("Unit - Vehicle")]				Vehicle,
			[TI.Enum("Item - Weapon")]				Weapon,
			[TI.Enum("Item - Equipment")]			Equipment,
			[TI.Enum("Device - Terminal")]			Terminal,
			[TI.Enum("Item - Projectile")]			Projectile,
			[TI.Enum("Object - Scenery")]			Scenery,
			[TI.Enum("Device - Machine")]			Machine,
			[TI.Enum("Device - Control")]			Control,
			[TI.Enum("Object - Sound Scenery")]		SoundScenery,
			[TI.Enum("Object - Crate")]				Crate,
			[TI.Enum("Object - Creature")]			Creature,
			[TI.Enum("Unit - Giant")]				Giant,
			[TI.Enum("Object - Effect Scenery")]	EffectScenery,
		};

		public enum ExecutableType : int
		{
			[TI.Enum("Tags")]		Tags,
			[TI.Enum("Symbols")]	Symbols,
			[TI.Enum("Debug")]		Debug,
			[TI.Enum("Profile")]	Profile,
			[TI.Enum("Play")]		Play,
			[TI.Enum("Release")]	Release,
		};

		public enum GameSimulation : byte
		{
			[TI.Enum("None")]				None,
			[TI.Enum("Local")]				Local,
			[TI.Enum("Synchronous-Client")]	SyncClient,
			[TI.Enum("Synchronous-Server")]	SyncServer,
			[TI.Enum("Distributed-Client")]	DistClient,
			[TI.Enum("Distributed-Server")]	DistServer,
		};

		public enum GamePlayback : short
		{
			[TI.Enum("None")]			None,
			[TI.Enum("Local")]			Local,
			[TI.Enum("Network-Server")]	NetworkServer,
			[TI.Enum("Network-Client")]	NetworkClient,
		};


		public class ContentHeader : IO.IStreamable // 0xF8
		{
			public const uint SizeOf = 0xF8;

			private ulong Unknown000; // MPVR's seems to always be zero'd
			public string Name;
			public string Description;
			public string Author;
			public ContentFileType FileType;
			private bool Unknown0BC;
			private ulong Unknown0C0;
			public ulong ContentLength;
			private ulong Unknown0D0;
			private int Unknown0D8;
			public Blam.MapId MapId;
			public int GameEngineType;
			public int CampaignDifficulty; // -1 if N/A
			public ushort HopperId;
			// PAD16
			public ulong GameId;

			#region IStreamable Members
			public void Read(BlamLib.IO.EndianReader s)
			{
				Unknown000 = s.ReadUInt64();
				Name = s.ReadUnicodeString(16);
				Description = s.ReadAsciiString(128);
				Author = s.ReadAsciiString(16);
				FileType = (ContentFileType)s.ReadUInt32();
				Unknown0BC = s.ReadInt32() > 0;
				Unknown0C0 = s.ReadUInt64();
				ContentLength = s.ReadUInt64();
				Unknown0D0 = s.ReadUInt64();
				Unknown0D8 = s.ReadInt32();
				MapId.Read(s);
				GameEngineType = s.ReadInt32();
				CampaignDifficulty = s.ReadInt32();
				HopperId = (ushort)((s.ReadUInt32() & 0xFFFF0000) >> 16);
				GameId = s.ReadUInt64();
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(Unknown000);
				s.WriteUnicodeString(Name, 16);
				s.Write(Description, 128);
				s.Write(Author, 16);
				s.Write((uint)FileType);
				s.Write(Unknown0BC ? 1 : 0);
				s.Write(Unknown0C0);
				s.Write(ContentLength);
				s.Write(Unknown0D0);
				s.Write(Unknown0D8);
				MapId.Write(s);
				s.Write(GameEngineType);
				s.Write(CampaignDifficulty);
				s.Write(((uint)HopperId) << 16);
				s.Write(GameId);
			}
			#endregion
		};

		public class LocalizedName32 : IO.IStreamable // 0x300
		{
			public const uint SizeOf = 0x300;

			public string[] Names = new string[12];

			#region IStreamable Members
			public void Read(BlamLib.IO.EndianReader s)
			{
				for (int x = 0; x < Names.Length; x++) Names[x] = s.ReadUnicodeString(32);
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				foreach (string str in Names) s.WriteUnicodeString(str, 32);
			}
			#endregion
		};

		public class LocalizedName64 : IO.IStreamable // 0x600
		{
			public const uint SizeOf = 0x600;

			public string[] Names = new string[12];

			#region IStreamable Members
			public void Read(BlamLib.IO.EndianReader s)
			{
				for (int x = 0; x < Names.Length; x++) Names[x] = s.ReadUnicodeString(64);
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				foreach (string str in Names) s.WriteUnicodeString(str, 64);
			}
			#endregion
		};

		public class LocalizedDescription : IO.IStreamable // 0xC00
		{
			public const uint SizeOf = 0xC00;

			public string[] Names = new string[12];

			#region IStreamable Members
			public void Read(BlamLib.IO.EndianReader s)
			{
				for (int x = 0; x < Names.Length; x++) Names[x] = s.ReadUnicodeString(128);
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				foreach (string str in Names) s.WriteUnicodeString(str, 128);
			}
			#endregion
		};

		public class GameEngineVariant : IO.IStreamable // 0x264
		{
			public const uint SizeOf = 0x264;

			public class PlayerTrait : IO.IStreamable // 0x1C
			{
				public const uint SizeOf = 0x1C;

				public struct ShieldVitality : IO.IStreamable // 0x8
				{
					public const uint SizeOf = 0x8;

					public byte DamageResistance;
					public byte ShieldRechargeRate;
					public byte Vampirism;
					public byte HeadshotImmunity;
					public byte ShieldMultiplier;

					#region IStreamable Members
					public void Read(BlamLib.IO.EndianReader s)
					{
						DamageResistance = s.ReadByte();
						ShieldRechargeRate = s.ReadByte();
						Vampirism = s.ReadByte();
						HeadshotImmunity = s.ReadByte();
						ShieldMultiplier = s.ReadByte();
						s.Seek(3, System.IO.SeekOrigin.Current);
					}

					public void Write(BlamLib.IO.EndianWriter s)
					{
						s.Write(DamageResistance);
						s.Write(ShieldRechargeRate);
						s.Write(Vampirism);
						s.Write(HeadshotImmunity);
						s.Write(ShieldMultiplier);
						s.Write(byte.MinValue);
						s.Write(ushort.MinValue);
					}
					#endregion
				}; public ShieldVitality _ShieldVitality = new ShieldVitality();

				public struct Weapons : IO.IStreamable // 0x8
				{
					public const uint SizeOf = 0x8;

					public byte InitialGrenadeCount;
					public byte InitialGrenadeType;
					public byte InitialPrimaryWeapon;
					public byte InitialSecondaryWeapon;
					public byte DamageModifier;
					public byte RechargingGrendes;
					public byte InfiniteAmmoSetting;
					public byte WeaponPickupAllowed;

					#region IStreamable Members
					public void Read(BlamLib.IO.EndianReader s)
					{
						InitialGrenadeCount = s.ReadByte();
						InitialGrenadeType = s.ReadByte();
						InitialPrimaryWeapon = s.ReadByte();
						InitialSecondaryWeapon = s.ReadByte();
						DamageModifier = s.ReadByte();
						RechargingGrendes = s.ReadByte();
						InfiniteAmmoSetting = s.ReadByte();
						WeaponPickupAllowed = s.ReadByte();
					}

					public void Write(BlamLib.IO.EndianWriter s)
					{
						s.Write(InitialGrenadeCount);
						s.Write(InitialGrenadeType);
						s.Write(InitialPrimaryWeapon);
						s.Write(InitialSecondaryWeapon);
						s.Write(DamageModifier);
						s.Write(RechargingGrendes);
						s.Write(InfiniteAmmoSetting);
						s.Write(WeaponPickupAllowed);
					}
					#endregion
				}; public Weapons _Weapons = new Weapons();

				public struct Movement : IO.IStreamable // 0x4
				{
					public const uint SizeOf = 0x4;

					public byte MovementSpeed;
					public byte Gravity;
					public byte VehicleUsage;

					#region IStreamable Members
					public void Read(BlamLib.IO.EndianReader s)
					{
						MovementSpeed = s.ReadByte();
						Gravity = s.ReadByte();
						VehicleUsage = s.ReadByte();
						s.Seek(1, System.IO.SeekOrigin.Current);
					}

					public void Write(BlamLib.IO.EndianWriter s)
					{
						s.Write(MovementSpeed);
						s.Write(Gravity);
						s.Write(VehicleUsage);
						s.Write(byte.MinValue);
					}
					#endregion
				}; public Movement _Movement = new Movement();

				public struct Apperance : IO.IStreamable // 0x4
				{
					public const uint SizeOf = 0x4;

					public byte ActiveCamo;
					public byte Waypoint;
					public byte Aura;
					public byte ForcedChangeColor;

					#region IStreamable Members
					public void Read(BlamLib.IO.EndianReader s)
					{
						ActiveCamo = s.ReadByte();
						Waypoint = s.ReadByte();
						Aura = s.ReadByte();
						ForcedChangeColor = s.ReadByte();
					}

					public void Write(BlamLib.IO.EndianWriter s)
					{
						s.Write(ActiveCamo);
						s.Write(Waypoint);
						s.Write(Aura);
						s.Write(ForcedChangeColor);
					}
					#endregion
				}; public Apperance _Apperance = new Apperance();

				public struct Sensors : IO.IStreamable // 0x4
				{
					public const uint SizeOf = 0x4;

					public short MotionTracker;
					public short MotionTrackerRange;

					#region IStreamable Members
					public void Read(BlamLib.IO.EndianReader s)
					{
						MotionTracker = s.ReadInt16();
						MotionTrackerRange = s.ReadInt16();
					}

					public void Write(BlamLib.IO.EndianWriter s)
					{
						s.Write(MotionTracker);
						s.Write(MotionTrackerRange);
					}
					#endregion
				}; public Sensors _Sensors = new Sensors();

				#region IStreamable Members
				public void Read(BlamLib.IO.EndianReader s)
				{
					_ShieldVitality.Read(s);
					_Weapons.Read(s);
					_Movement.Read(s);
					_Apperance.Read(s);
					_Sensors.Read(s);
				}

				public void Write(BlamLib.IO.EndianWriter s)
				{
					_ShieldVitality.Write(s);
					_Weapons.Write(s);
					_Movement.Write(s);
					_Apperance.Write(s);
					_Sensors.Write(s);
				}
				#endregion
			};

			public class MiscellaneousOptions : IO.IStreamable // 0x4
			{
				public const uint SizeOf = 0x4;

				public byte Flags;
				public byte RoundTimeLimitMinutes;
				public byte RoundLimit;
				public byte EarlyVictoryWinCount;

				#region IStreamable Members
				public void Read(BlamLib.IO.EndianReader s)
				{
					Flags = s.ReadByte();
					RoundTimeLimitMinutes = s.ReadByte();
					RoundLimit = s.ReadByte();
					EarlyVictoryWinCount = s.ReadByte();
				}

				public void Write(BlamLib.IO.EndianWriter s)
				{
					s.Write(Flags);
					s.Write(RoundTimeLimitMinutes);
					s.Write(RoundLimit);
					s.Write(EarlyVictoryWinCount);
				}
				#endregion
			};

			public class RespawnOptions : IO.IStreamable // 0x24
			{
				public const uint SizeOf = 0x24;

				public byte Flags;
				public byte LivesPerRound;
				public byte TeamLivesPerRound;
				public byte RespawnTime;
				public byte SuicideTime;
				public byte BetrayalTime;
				public byte RespawnGrowthTime;
				public byte PlayerTraitsDuration;
				public PlayerTrait RespawnTraits = new PlayerTrait();

				#region IStreamable Members
				public void Read(BlamLib.IO.EndianReader s)
				{
					Flags = s.ReadByte();
					LivesPerRound = s.ReadByte();
					TeamLivesPerRound = s.ReadByte();
					RespawnTime = s.ReadByte();
					SuicideTime = s.ReadByte();
					BetrayalTime = s.ReadByte();
					RespawnGrowthTime = s.ReadByte();
					PlayerTraitsDuration = s.ReadByte();
					RespawnTraits.Read(s);
				}

				public void Write(BlamLib.IO.EndianWriter s)
				{
					s.Write(Flags);
					s.Write(LivesPerRound);
					s.Write(TeamLivesPerRound);
					s.Write(RespawnTime);
					s.Write(SuicideTime);
					s.Write(BetrayalTime);
					s.Write(RespawnGrowthTime);
					s.Write(PlayerTraitsDuration);
					RespawnTraits.Write(s);
				}
				#endregion
			};

			public class MapOverrideOptions : IO.IStreamable // 0x78
			{
				public const uint SizeOf = 0x78;

				public uint Flags;
				public PlayerTrait BaseTraits = new PlayerTrait();
				public short WeaponSet;
				public short VehicleSet;
				public PlayerTrait RedPowerupTraits = new PlayerTrait();
				public PlayerTrait BluePowerupTraits = new PlayerTrait();
				public PlayerTrait YellowPowerupTraits = new PlayerTrait();

				#region IStreamable Members
				public void Read(BlamLib.IO.EndianReader s)
				{
					Flags = s.ReadUInt32();
					BaseTraits.Read(s);
					WeaponSet = s.ReadInt16();
					VehicleSet = s.ReadInt16();
					RedPowerupTraits.Read(s);
					BluePowerupTraits.Read(s);
					YellowPowerupTraits.Read(s);
				}

				public void Write(BlamLib.IO.EndianWriter s)
				{
					s.Write(Flags);
					BaseTraits.Write(s);
					s.Write(WeaponSet);
					s.Write(VehicleSet);
					RedPowerupTraits.Write(s);
					BluePowerupTraits.Write(s);
					YellowPowerupTraits.Write(s);
				}
				#endregion
			};

			#region Variants
			public abstract class Base : IO.IStreamable // 0x1B0
			{
				protected const uint kBaseSizeOf = 0xB0;

				public abstract uint Sizeof { get; }

				public MiscellaneousOptions MiscOptions = new MiscellaneousOptions();
				public RespawnOptions RespawnOptions = new RespawnOptions();
				public ushort SocialFlags;
				public short SocialTeamChanging;
				public MapOverrideOptions MapOverrides = new MapOverrideOptions();
				private ushort Unknown0A8;
				public short TeamScoringMethod;
				private ushort Unknown0AC;
				//PAD16

				#region IStreamable Members
				public virtual void Read(BlamLib.IO.EndianReader s)
				{
					MiscOptions.Read(s);
					RespawnOptions.Read(s);
					SocialFlags = s.ReadUInt16();
					SocialTeamChanging = s.ReadInt16();
					MapOverrides.Read(s);
					Unknown0A8 = s.ReadUInt16();
					TeamScoringMethod = s.ReadInt16();
					Unknown0AC = s.ReadUInt16();
					s.Seek(2, System.IO.SeekOrigin.Current);
				}

				public virtual void Write(BlamLib.IO.EndianWriter s)
				{
					MiscOptions.Write(s);
					RespawnOptions.Write(s);
					s.Write(SocialFlags);
					s.Write(SocialTeamChanging);
					MapOverrides.Write(s);
					s.Write(Unknown0A8);
					s.Write(TeamScoringMethod);
					s.Write(Unknown0AC);
					s.Write(ushort.MinValue);
				}
				#endregion
			};

			public class Ctf : Base // 0x1D8
			{
				public override uint Sizeof	{ get { return kBaseSizeOf + 0x28; } }

				public byte Flags;
				public byte HomeFlagWaypoint;
				public byte GameType;
				public byte Respawn;
				public short TouchReturnTime;
				public short SuddenDeathTime;
				public short ScoreToWin;
				public short FlagResetTime;
				public PlayerTrait CarrierTraits = new PlayerTrait();

				#region IStreamable Members
				public override void Read(BlamLib.IO.EndianReader s)
				{
					base.Read(s);

					Flags = s.ReadByte();
					HomeFlagWaypoint = s.ReadByte();
					GameType = s.ReadByte();
					Respawn = s.ReadByte();
					TouchReturnTime = s.ReadInt16();
					SuddenDeathTime = s.ReadInt16();
					ScoreToWin = s.ReadInt16();
					FlagResetTime = s.ReadInt16();
					CarrierTraits.Read(s);
				}

				public override void Write(BlamLib.IO.EndianWriter s)
				{
					base.Write(s);

					s.Write(Flags);
					s.Write(HomeFlagWaypoint);
					s.Write(GameType);
					s.Write(Respawn);
					s.Write(TouchReturnTime);
					s.Write(SuddenDeathTime);
					s.Write(ScoreToWin);
					s.Write(FlagResetTime);
					CarrierTraits.Write(s);
				}
				#endregion
			};

			public class Slayer : Base // 0x1DC
			{
				public override uint Sizeof	{ get { return kBaseSizeOf + 0x2C; } }

				public short ScoreToWin;
				public short KillPoints;
				public byte AssistPoints;
				public byte DeathPoints;
				public byte SuicidePoints;
				public byte BetrayalPoints;
				public byte LeaderKilledPoints;
				public byte EliminationPoints;
				public byte AssassinationPoints;
				public byte HeadshotPoints;
				public byte MeleePoints;
				public byte StickyPoints;
				public byte SplatterPoints;
				public byte KillingSpreePoints;
				public PlayerTrait LeaderTraits = new PlayerTrait();

				#region IStreamable Members
				public override void Read(BlamLib.IO.EndianReader s)
				{
					base.Read(s);

					ScoreToWin = s.ReadInt16();
					KillPoints = s.ReadInt16();
					AssistPoints = s.ReadByte();
					DeathPoints = s.ReadByte();
					SuicidePoints = s.ReadByte();
					BetrayalPoints = s.ReadByte();
					LeaderKilledPoints = s.ReadByte();
					EliminationPoints = s.ReadByte();
					AssassinationPoints = s.ReadByte();
					HeadshotPoints = s.ReadByte();
					MeleePoints = s.ReadByte();
					StickyPoints = s.ReadByte();
					SplatterPoints = s.ReadByte();
					KillingSpreePoints = s.ReadByte();
					LeaderTraits.Read(s);
				}

				public override void Write(BlamLib.IO.EndianWriter s)
				{
					base.Write(s);

					s.Write(ScoreToWin);
					s.Write(KillPoints);
					s.Write(AssistPoints);
					s.Write(DeathPoints);
					s.Write(SuicidePoints);
					s.Write(BetrayalPoints);
					s.Write(LeaderKilledPoints);
					s.Write(EliminationPoints);
					s.Write(AssassinationPoints);
					s.Write(HeadshotPoints);
					s.Write(MeleePoints);
					s.Write(StickyPoints);
					s.Write(SplatterPoints);
					s.Write(KillingSpreePoints);
					LeaderTraits.Write(s);
				}
				#endregion
			};

			public class Oddball : Base // 0x1DC
			{
				public override uint Sizeof	{ get { return kBaseSizeOf + 0x2C; } }

				public uint Flags;
				public short ScoreToWin;
				public short CarryingPoints;
				public byte KillPoints;
				public byte BallKillPoints;
				public byte CarrierKillPoints;
				public byte BallCount;
				public short BallSpawnDelay;
				public short BallInactiveRespawnDelay;
				public PlayerTrait CarrierTraits = new PlayerTrait();

				#region IStreamable Members
				public override void Read(BlamLib.IO.EndianReader s)
				{
					base.Read(s);

					Flags = s.ReadUInt32();
					ScoreToWin = s.ReadInt16();
					CarryingPoints = s.ReadInt16();
					KillPoints = s.ReadByte();
					BallKillPoints = s.ReadByte();
					CarrierKillPoints = s.ReadByte();
					BallCount = s.ReadByte();
					BallSpawnDelay = s.ReadInt16();
					BallInactiveRespawnDelay = s.ReadInt16();
					CarrierTraits.Read(s);
				}

				public override void Write(BlamLib.IO.EndianWriter s)
				{
					base.Write(s);

					s.Write(Flags);
					s.Write(ScoreToWin);
					s.Write(CarryingPoints);
					s.Write(KillPoints);
					s.Write(BallKillPoints);
					s.Write(CarrierKillPoints);
					s.Write(BallCount);
					s.Write(BallSpawnDelay);
					s.Write(BallInactiveRespawnDelay);
					CarrierTraits.Write(s);
				}
				#endregion
			};

			public class King : Base // 0x1D8
			{
				public override uint Sizeof	{ get { return kBaseSizeOf + 0x28; } }

				public uint Flags;
				public short ScoreToWin;
				public byte MovingHill;
				public byte MovingHillOrder;
				public byte InsideHillPoints;
				public byte OutsideHillPoints;
				public byte UncontestedHillBonus;
				public byte KillPoints;
				public PlayerTrait HillTraits = new PlayerTrait();

				#region IStreamable Members
				public override void Read(BlamLib.IO.EndianReader s)
				{
					base.Read(s);

					Flags = s.ReadUInt32();
					ScoreToWin = s.ReadInt16();
					MovingHill = s.ReadByte();
					MovingHillOrder = s.ReadByte();
					InsideHillPoints = s.ReadByte();
					OutsideHillPoints = s.ReadByte();
					UncontestedHillBonus = s.ReadByte();
					KillPoints = s.ReadByte();
					HillTraits.Read(s);
				}

				public override void Write(BlamLib.IO.EndianWriter s)
				{
					base.Write(s);

					s.Write(Flags);
					s.Write(ScoreToWin);
					s.Write(MovingHill);
					s.Write(MovingHillOrder);
					s.Write(InsideHillPoints);
					s.Write(OutsideHillPoints);
					s.Write(UncontestedHillBonus);
					s.Write(KillPoints);
					HillTraits.Write(s);
				}
				#endregion
			};

			public class Sandbox : Base // 0x1D0
			{
				public override uint Sizeof	{ get { return kBaseSizeOf + 0x20; } }

				public byte Flags;
				public byte EditMode;
				public short RespawnTime;
				public PlayerTrait EditorTraits = new PlayerTrait();

				#region IStreamable Members
				public override void Read(BlamLib.IO.EndianReader s)
				{
					base.Read(s);

					Flags = s.ReadByte();
					EditMode = s.ReadByte();
					RespawnTime = s.ReadInt16();
					EditorTraits.Read(s);
				}

				public override void Write(BlamLib.IO.EndianWriter s)
				{
					base.Write(s);

					s.Write(Flags);
					s.Write(EditMode);
					s.Write(RespawnTime);
					EditorTraits.Write(s);
				}
				#endregion
			};

			public class Vip : Base  // 0x216
			{
				public override uint Sizeof	{ get { return kBaseSizeOf + 0x66; } }

				public short ScoreToWin;
				public ushort Flags;
				public byte KillPoints;
				public byte TakedownPoints;
				public byte KillAsVipPoints;
				public byte VipDeathPoints;
				public byte DestinationArrivalPoints;
				public byte SuicidePoints;
				public byte BetrayalPoints;
				public byte VipSuicidePoints;
				public byte VipSelection;
				public byte ZoneMovement;
				public byte ZoneOrder;
				//PAD8
				public short InfluenceRadius;
				public PlayerTrait VipTraits = new PlayerTrait();
				public PlayerTrait ProximityTraits = new PlayerTrait();
				public PlayerTrait VipTeamTraits = new PlayerTrait();

				#region IStreamable Members
				public override void Read(BlamLib.IO.EndianReader s)
				{
					base.Read(s);

					ScoreToWin = s.ReadInt16();
					Flags = s.ReadUInt16();
					KillPoints = s.ReadByte();
					TakedownPoints = s.ReadByte();
					KillAsVipPoints = s.ReadByte();
					VipDeathPoints = s.ReadByte();
					DestinationArrivalPoints = s.ReadByte();
					SuicidePoints = s.ReadByte();
					BetrayalPoints = s.ReadByte();
					VipSuicidePoints = s.ReadByte();
					VipSelection = s.ReadByte();
					ZoneMovement = s.ReadByte();
					ZoneOrder = s.ReadByte();
					s.Seek(1, System.IO.SeekOrigin.Current);
					InfluenceRadius = s.ReadInt16();
					VipTraits.Read(s);
					ProximityTraits.Read(s);
					VipTeamTraits.Read(s);
				}

				public override void Write(BlamLib.IO.EndianWriter s)
				{
					base.Write(s);

					s.Write(ScoreToWin);
					s.Write(Flags);
					s.Write(KillPoints);
					s.Write(TakedownPoints);
					s.Write(KillAsVipPoints);
					s.Write(VipDeathPoints);
					s.Write(DestinationArrivalPoints);
					s.Write(SuicidePoints);
					s.Write(BetrayalPoints);
					s.Write(VipSuicidePoints);
					s.Write(VipSelection);
					s.Write(ZoneMovement);
					s.Write(ZoneOrder);
					s.Write(byte.MinValue);
					s.Write(InfluenceRadius);
					VipTraits.Write(s);
					ProximityTraits.Write(s);
					VipTeamTraits.Write(s);
				}
				#endregion
			};

			public class Juggernaut : Base // 0x1DC
			{
				public override uint Sizeof	{ get { return kBaseSizeOf + 0x2C; } }

				public short ScoreToWinRound;
				//PAD16
				public byte InitialJuggernaut;
				public byte NextJuggernaut;
				public byte Flags;
				public byte ZoneMovement;
				public byte ZoneOrder;
				public byte KillPoints;
				public byte JuggernautKillPoints;
				public byte KillAsJuggernautPoints;
				public byte DestinationArrivalPoints;
				public byte SuicidePoints;
				public byte BetrayalPoints;
				public byte JuggernautDelay;
				public PlayerTrait JuggernautTraits = new PlayerTrait();

				#region IStreamable Members
				public override void Read(BlamLib.IO.EndianReader s)
				{
					base.Read(s);

					ScoreToWinRound = s.ReadInt16();
					s.Seek(2, System.IO.SeekOrigin.Current);
					InitialJuggernaut = s.ReadByte();
					NextJuggernaut = s.ReadByte();
					Flags = s.ReadByte();
					ZoneMovement = s.ReadByte();
					ZoneOrder = s.ReadByte();
					KillPoints = s.ReadByte();
					JuggernautKillPoints = s.ReadByte();
					KillAsJuggernautPoints = s.ReadByte();
					DestinationArrivalPoints = s.ReadByte();
					SuicidePoints = s.ReadByte();
					BetrayalPoints = s.ReadByte();
					JuggernautDelay = s.ReadByte();
					JuggernautTraits.Read(s);
				}

				public override void Write(BlamLib.IO.EndianWriter s)
				{
					base.Write(s);

					s.Write(ScoreToWinRound);
					s.Write(ushort.MinValue);
					s.Write(InitialJuggernaut);
					s.Write(NextJuggernaut);
					s.Write(Flags);
					s.Write(ZoneMovement);
					s.Write(ZoneOrder);
					s.Write(KillPoints);
					s.Write(JuggernautKillPoints);
					s.Write(KillAsJuggernautPoints);
					s.Write(DestinationArrivalPoints);
					s.Write(SuicidePoints);
					s.Write(BetrayalPoints);
					s.Write(JuggernautDelay);
					JuggernautTraits.Write(s);
				}
				#endregion
			};

			public class Teritories : Base // 0x1F0
			{
				public override uint Sizeof	{ get { return kBaseSizeOf + 0x40; } }

				public ushort Flags;
				public short RepsawnOnCapture;
				public short CaptureTime;
				public short SuddenDeathTime;
				public PlayerTrait DefenderTraits = new PlayerTrait();
				public PlayerTrait AttackerTraits = new PlayerTrait();

				#region IStreamable Members
				public override void Read(BlamLib.IO.EndianReader s)
				{
					base.Read(s);

					Flags = s.ReadUInt16();
					RepsawnOnCapture = s.ReadInt16();
					CaptureTime = s.ReadInt16();
					SuddenDeathTime = s.ReadInt16();
					DefenderTraits.Read(s);
					AttackerTraits.Read(s);
				}

				public override void Write(BlamLib.IO.EndianWriter s)
				{
					base.Write(s);

					s.Write(Flags);
					s.Write(RepsawnOnCapture);
					s.Write(CaptureTime);
					s.Write(SuddenDeathTime);
					DefenderTraits.Write(s);
					AttackerTraits.Write(s);
				}
				#endregion
			};

			public class Assault : Base // 0x1FC
			{
				public override uint Sizeof	{ get { return kBaseSizeOf + 0x4C; } }

				public ushort Flags;
				public short Respawn;
				public short GameType;
				public short EnemyBombWaypoint;
				public short ScoreToWin;
				public short SuddenDeathTime;
				public short BombResetTime;
				public short BombArmingTime;
				public short BombDisarmingTime;
				public short BombFuseTime;
				public PlayerTrait CarrierTraits = new PlayerTrait();
				public PlayerTrait ArmingTraits = new PlayerTrait();

				#region IStreamable Members
				public override void Read(BlamLib.IO.EndianReader s)
				{
					base.Read(s);

					Flags = s.ReadUInt16();
					Respawn = s.ReadInt16();
					GameType = s.ReadInt16();
					EnemyBombWaypoint = s.ReadInt16();
					ScoreToWin = s.ReadInt16();
					SuddenDeathTime = s.ReadInt16();
					BombResetTime = s.ReadInt16();
					BombArmingTime = s.ReadInt16();
					BombDisarmingTime = s.ReadInt16();
					BombFuseTime = s.ReadInt16();
					CarrierTraits.Read(s);
					ArmingTraits.Read(s);
				}

				public override void Write(BlamLib.IO.EndianWriter s)
				{
					base.Write(s);

					s.Write(Flags);
					s.Write(Respawn);
					s.Write(GameType);
					s.Write(EnemyBombWaypoint);
					s.Write(ScoreToWin);
					s.Write(SuddenDeathTime);
					s.Write(BombResetTime);
					s.Write(BombArmingTime);
					s.Write(BombDisarmingTime);
					s.Write(BombFuseTime);
					CarrierTraits.Write(s);
					ArmingTraits.Write(s);
				}
				#endregion
			};

			public class Infection : Base // 0x22C
			{
				public override uint Sizeof	{ get { return kBaseSizeOf + 0x7C; } }

				public byte Flags;
				public byte SafeHavens;
				public byte NextZombie;
				public byte InitialZombieCount;
				public short SafeHavenMovementTime;
				public byte ZombieKillPoints;
				public byte InfectionPoints;
				public byte SafeHavenArrivalPoints;
				public byte SuicidePoints;
				public byte BetrayalPoints;
				public byte LastManBonusPoints;
				public PlayerTrait ZombieTraits = new PlayerTrait();
				public PlayerTrait AlphaTraits = new PlayerTrait();
				public PlayerTrait LastManTraits = new PlayerTrait();
				public PlayerTrait SafeHavenTraits = new PlayerTrait();

				#region IStreamable Members
				public override void Read(BlamLib.IO.EndianReader s)
				{
					base.Read(s);

					Flags = s.ReadByte();
					SafeHavens = s.ReadByte();
					NextZombie = s.ReadByte();
					InitialZombieCount = s.ReadByte();
					SafeHavenMovementTime = s.ReadInt16();
					ZombieKillPoints = s.ReadByte();
					InfectionPoints = s.ReadByte();
					SafeHavenArrivalPoints = s.ReadByte();
					SuicidePoints = s.ReadByte();
					BetrayalPoints = s.ReadByte();
					LastManBonusPoints = s.ReadByte();
					ZombieTraits.Read(s);
					AlphaTraits.Read(s);
					LastManTraits.Read(s);
					SafeHavenTraits.Read(s);
				}

				public override void Write(BlamLib.IO.EndianWriter s)
				{
					base.Write(s);

					s.Write(Flags);
					s.Write(SafeHavens);
					s.Write(NextZombie);
					s.Write(InitialZombieCount);
					s.Write(SafeHavenMovementTime);
					s.Write(ZombieKillPoints);
					s.Write(InfectionPoints);
					s.Write(SafeHavenArrivalPoints);
					s.Write(SuicidePoints);
					s.Write(BetrayalPoints);
					s.Write(LastManBonusPoints);
					ZombieTraits.Write(s);
					AlphaTraits.Write(s);
					LastManTraits.Write(s);
					SafeHavenTraits.Write(s);
				}
				#endregion
			};
			#endregion

			public GameEngineType GameEngineIndex;
			private uint _vtable;
			private int Unknown008;
			public ContentHeader Header = new ContentHeader();
			public Base VariantOptions; // 0x160

			#region IStreamable Members
			public void Read(BlamLib.IO.EndianReader s)
			{
				GameEngineIndex = (GameEngineType)s.ReadInt32();
				_vtable = s.ReadUInt32();
				Unknown008 = s.ReadInt32();
				Header.Read(s);

				switch(GameEngineIndex)
				{
					case GameEngineType.Ctf:			VariantOptions = new Ctf();			break;
					case GameEngineType.Slayer:			VariantOptions = new Slayer();		break;
					case GameEngineType.Oddball:		VariantOptions = new Oddball();		break;
					case GameEngineType.King:			VariantOptions = new King();		break;
					case GameEngineType.Sandbox:		VariantOptions = new Sandbox();		break;
					case GameEngineType.Vip:			VariantOptions = new Vip();			break;
					case GameEngineType.Juggernaut:		VariantOptions = new Juggernaut();	break;
					case GameEngineType.Territories:	VariantOptions = new Teritories();	break;
					case GameEngineType.Assault:		VariantOptions = new Assault();		break;
					case GameEngineType.Infection:		VariantOptions = new Infection();	break;
					default:							throw new Debug.Exceptions.UnreachableException();
				}

				VariantOptions.Read(s);

				s.Seek(0x160 - VariantOptions.Sizeof, System.IO.SeekOrigin.Current);
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write((int)GameEngineIndex);
				s.Write(_vtable);
				s.Write(Unknown008);
				Header.Write(s);
				VariantOptions.Write(s);
				s.Write(new byte[0x160 - VariantOptions.Sizeof]);
			}
			#endregion
		};

		public class MapVariant : IO.IStreamable // 0xE094
		{
			public const uint SizeOf = 0xE094;

			public class ObjectProperties : IO.IStreamable // 0x54
			{
				public const uint SizeOf = 0x54;

				public uint ObjectFlags;
				private uint Unknown004, Unknown008;
				public int BudgetIndex;
				public float X, Y, Z;
				public float Yaw, Pitch, Roll; // Up
				public float I, J, K; // Forward
				private uint Unknown034, Unknown038;
				public ushort GameEngineFlags;
				public byte Flags;
				public byte TeamAffiliation;
				public byte SharedStorage;
				public byte SpawnTime; // in seconds
				public byte CachedObjectType;
				public byte ShapeType;
				public float ShapeBoundaryWidthRadius;
				public float ShapeBoundaryBoxLength;
				public float ShapeBoundaryPositiveHeight;
				public float ShapeBoundaryNegativeHeight;

				#region IStreamable Members
				public void Read(BlamLib.IO.EndianReader s)
				{
					ObjectFlags = s.ReadUInt32();
					Unknown004 = s.ReadUInt32();
					Unknown008 = s.ReadUInt32();
					BudgetIndex = s.ReadInt32();
					X = s.ReadSingle(); Y = s.ReadSingle(); Z = s.ReadSingle();
					Yaw = s.ReadSingle(); Pitch = s.ReadSingle(); Roll = s.ReadSingle();
					I = s.ReadSingle(); J = s.ReadSingle(); K = s.ReadSingle();
					Unknown034 = s.ReadUInt32();
					Unknown038 = s.ReadUInt32();
					GameEngineFlags = s.ReadUInt16();
					Flags = s.ReadByte();
					TeamAffiliation = s.ReadByte();
					SharedStorage = s.ReadByte();
					SpawnTime = s.ReadByte();
					CachedObjectType = s.ReadByte();
					ShapeType = s.ReadByte();
					ShapeBoundaryWidthRadius = s.ReadSingle();
					ShapeBoundaryBoxLength = s.ReadSingle();
					ShapeBoundaryPositiveHeight = s.ReadSingle();
					ShapeBoundaryNegativeHeight = s.ReadSingle();
				}

				public void Write(BlamLib.IO.EndianWriter s)
				{
					s.Write(ObjectFlags);
					s.Write(Unknown004);
					s.Write(Unknown008);
					s.Write(BudgetIndex);
					s.Write(X); s.Write(Y); s.Write(Z);
					s.Write(Yaw); s.Write(Pitch); s.Write(Roll);
					s.Write(I); s.Write(J); s.Write(K);
					s.Write(Unknown034);
					s.Write(Unknown038);
					s.Write(GameEngineFlags);
					s.Write(Flags);
					s.Write(TeamAffiliation);
					s.Write(SharedStorage);
					s.Write(SpawnTime);
					s.Write(CachedObjectType);
					s.Write(ShapeType);
					s.Write(ShapeBoundaryWidthRadius);
					s.Write(ShapeBoundaryBoxLength);
					s.Write(ShapeBoundaryPositiveHeight);
					s.Write(ShapeBoundaryNegativeHeight);
				}
				#endregion
			};

			public class ObjectQuotas : IO.IStreamable // 0xC
			{
				public const uint SizeOf = 0xC;

				public Blam.DatumIndex TagIndex;
				public byte RuntimeMin, RuntimeMax;
				public byte PlacedCount, MaxCount;
				public float TotalCost;

				#region IStreamable Members
				public void Read(BlamLib.IO.EndianReader s)
				{
					TagIndex.Read(s);
					RuntimeMin = s.ReadByte(); RuntimeMax = s.ReadByte();
					PlacedCount = s.ReadByte(); MaxCount = s.ReadByte();
					TotalCost = s.ReadSingle();
				}

				public void Write(BlamLib.IO.EndianWriter s)
				{
					TagIndex.Write(s);
					s.Write(RuntimeMin); s.Write(RuntimeMax);
					s.Write(PlacedCount); s.Write(MaxCount);
					s.Write(TotalCost);
				}
				#endregion
			};

			public bool Valid;
			public ContentHeader Header = new ContentHeader();
			public short Version;
			public short NumberOfScenarioObjects;
			public short NumberOfVariantObjects;
			public short NumberOfPlaceableObjectQuotas;
			public int MapId;
			public float WorldBoundsXMin, WorldBoundsXMax;
			public float WorldBoundsYMin, WorldBoundsYMax;
			public float WorldBoundsZMin, WorldBoundsZMax;
			public int GameEngineSubtype;
			public float BudgetTotal, BudgetUsed;
			private uint Unknown5, Unknown6; // 6 seems to be constant by map
			public ObjectProperties[] Properties = new ObjectProperties[640];
			public short[] ObjectTypeMap = new short[14];
			public ObjectQuotas[] Quotas = new ObjectQuotas[256];

			#region IStreamable Members
			public void Read(BlamLib.IO.EndianReader s)
			{
				Valid = s.ReadInt32() > 0;
				Header.Read(s);
				Version = s.ReadInt16();
				NumberOfScenarioObjects = s.ReadInt16();
				NumberOfVariantObjects = s.ReadInt16();
				NumberOfPlaceableObjectQuotas = s.ReadInt16();
				MapId = s.ReadInt32();
				WorldBoundsXMin = s.ReadSingle(); WorldBoundsXMax = s.ReadSingle();
				WorldBoundsYMin = s.ReadSingle(); WorldBoundsYMax = s.ReadSingle();
				WorldBoundsZMin = s.ReadSingle(); WorldBoundsZMax = s.ReadSingle();
				GameEngineSubtype = s.ReadInt32();
				BudgetTotal = s.ReadSingle();
				BudgetUsed = s.ReadSingle();
				Unknown5 = s.ReadUInt32(); Unknown6 = s.ReadUInt32();
				for (int x = 0; x < Properties.Length; x++) (Properties[x] = new ObjectProperties()).Read(s);
				for (int x = 0; x < ObjectTypeMap.Length; x++) ObjectTypeMap[x] = s.ReadInt16();
				for (int x = 0; x < Quotas.Length; x++) (Quotas[x] = new ObjectQuotas()).Read(s);
			}

			public void Write(BlamLib.IO.EndianWriter s)
			{
				s.Write(Valid ? 1 : 0);
				Header.Write(s);
				s.Write(Version);
				s.Write(NumberOfScenarioObjects);
				s.Write(NumberOfVariantObjects);
				s.Write(NumberOfPlaceableObjectQuotas);
				s.Write(MapId);
				s.Write(WorldBoundsXMin); s.Write(WorldBoundsXMax);
				s.Write(WorldBoundsYMin); s.Write(WorldBoundsYMax);
				s.Write(WorldBoundsZMin); s.Write(WorldBoundsZMax);
				s.Write(GameEngineSubtype);
				s.Write(BudgetTotal);
				s.Write(BudgetUsed);
				s.Write(Unknown5); s.Write(Unknown6);
				for (int x = 0; x < Properties.Length; x++) Properties[x].Write(s);
				for (int x = 0; x < ObjectTypeMap.Length; x++) s.Write(ObjectTypeMap[x]);
				for (int x = 0; x < Quotas.Length; x++) Quotas[x].Write(s);
			}
			#endregion
		};


		public static List<Entry> ProcessBlamFile(string path)
		{
			List<Entry> entries = null;

			using(IO.EndianReader s = new BlamLib.IO.EndianReader(path, BlamLib.IO.EndianState.Big))
			{
				BLF head = new BLF();
				try { head.Read(s); }
				catch (Debug.ExceptionLog) { return null; }
				entries.Add(head);

				uint group_tag;
				TagInterface.TagGroup tag_group;
				Entry e;

				while((group_tag = unchecked((uint)s.Peek())) != MiscGroups._eof.ID)
				{
					tag_group = Groups.FindTagGroup(group_tag);
					Debug.Assert.If(tag_group, "Unhandled group @0x{0:X} ('{1}')", s.Position, new string(TagInterface.TagGroup.FromUInt(group_tag)));

					Type t = (Type)tag_group.EngineData;

					System.Reflection.ConstructorInfo ci = t.GetConstructor(new Type[0]);
					e = (Entry)ci.Invoke(null);

					e.Read(s);
					entries.Add(e);
				}
			}

			return entries;
		}

		// game state header
		// 0x8 map name
		// 0x128 map file checksum
		// 0x130 game options

		public abstract class Entry : IO.IStreamable
		{
			protected const uint kBaseSizeOf = 4 + 4 + 2 + 2;

			/// <summary>
			/// Group tag information for the file entry
			/// </summary>
			public abstract TagInterface.TagGroup GroupTag { get; }
			/// <summary>
			/// Size of the entry read from file
			/// </summary>
			protected uint BlockSize;
			/// <summary>
			/// Default entry size
			/// </summary>
			/// <remarks>Use <see cref="UInt32.MaxValue"/> for entries which don't have a constant size</remarks>
			public abstract uint SizeOf { get; }

			private short Unknown1, Unknown2;

			#region IStreamable Members
			public virtual void Read(BlamLib.IO.EndianReader s)
			{
				uint tag = s.ReadUInt32();// if (tag != GroupTag.ID) throw new Debug.ExceptionLog("blam file entry tag mismatch. {0} {1}", s.FileName, GroupTag.Name);
				if ((BlockSize = s.ReadUInt32()) != SizeOf && SizeOf != uint.MaxValue) throw new Debug.ExceptionLog("blam file entry size mismatch. {0} {1}: {2:X}", s.FileName, GroupTag.Name, BlockSize);
				Unknown1 = s.ReadInt16();
				Unknown2 = s.ReadInt16();
			}

			public virtual void Write(BlamLib.IO.EndianWriter s)
			{
				GroupTag.Write(s);
				s.Write(SizeOf);
				s.Write(Unknown1);
				s.Write(Unknown2);
			}

			/// <summary>
			/// For entries which aren't constant in size
			/// </summary>
			/// <param name="s"></param>
			/// <param name="size_override"></param>
			public void Write(BlamLib.IO.EndianWriter s, uint size_override)
			{
				GroupTag.Write(s);
				s.Write(size_override);
				s.Write(Unknown1);
				s.Write(Unknown2);
			}
			#endregion
		};

		public class BLF : Entry
		{
			private ushort EndianId = 0xFFFE; // big endian
			private string TypeString = string.Empty;

			#region Entry Members
			public const uint kSizeOf = 0x24;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups._blf; } }
			public override uint SizeOf { get { return kBaseSizeOf + kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);

				EndianId = s.ReadUInt16();
				TypeString = s.ReadTagString();
				s.Seek(2, System.IO.SeekOrigin.Current);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);

				s.Write(EndianId);
				s.Write(TypeString, false); // tag string
				s.Write(ushort.MinValue);
			}
			#endregion
		};

		public class EOF : Entry
		{
			public enum AuthenticationType : byte
			{
				None,
				Crc,
				Hash,
				Rsa,
			};

			public uint SizeOfFile;
			public AuthenticationType Authentication;

			#region Entry Members
			public const uint kSizeOf = uint.MaxValue;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups._eof; } }
			public override uint SizeOf { get { return kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);

				SizeOfFile = s.ReadUInt32();
				Authentication = (AuthenticationType)s.ReadByte();
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s, 0x11);

				s.Write(SizeOfFile);
				s.Write((byte)Authentication);
			}
			#endregion
		};

		public class CHDR : Entry
		{
			public ushort Build;
			public ushort MinorVersion; // map's minor version
			public ContentHeader Header = new ContentHeader();

			#region Entry Members
			public const uint kSizeOf = 2 + 2 + ContentHeader.SizeOf;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups.chdr; } }
			public override uint SizeOf { get { return kBaseSizeOf + kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);

				Build = s.ReadUInt16();
				MinorVersion = s.ReadUInt16();
				Header.Read(s);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);

				s.Write(Build);
				s.Write(MinorVersion);
				Header.Write(s);
			}
			#endregion
		};

		public class ATHR : Entry
		{
			private string Unknown000;
			private uint Unknown010;
			public int ExecutableVersion;
			public string BuildString;
			private string Unknown034;

			#region Entry Members
			public const uint kSizeOf = 0x44;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups.athr; } }
			public override uint SizeOf { get { return kBaseSizeOf + kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);

				Unknown000 = s.ReadAsciiString(16);
				Unknown010 = s.ReadUInt32();
				ExecutableVersion = s.ReadInt32();
				BuildString = s.ReadAsciiString(28);
				Unknown034 = s.ReadAsciiString(16);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);

				s.Write(Unknown000, 16);
				s.Write(Unknown010);
				s.Write(ExecutableVersion);
				s.Write(BuildString, 28);
				s.Write(Unknown034, 16);
			}
			#endregion
		};

		// Film Header
		public class FLMH : Entry
		{
			public class GamePlayerDataAppearance : IO.IStreamable // 0x20
			{
				public const uint SizeOf = 0x20;

				public bool Valid;
				public byte PrimaryColor;
				public byte SecondaryColor;
				public byte TertiaryColor;
				public byte PlayerModelChoice;
				public byte ForegroundEmblem;
				public byte BackgroundEmblem;
				public byte EmblemFlags;
				public byte EmblemPrimaryColor;
				public byte EmblemSecondaryColor;
				public byte EmblemBackgroundColor;
				public byte[][] ModelChoices = new byte[2][];
				public string ServiceTag;

				#region IStreamable Members
				public void Read(BlamLib.IO.EndianReader s)
				{
					Valid = s.ReadBool();
					PrimaryColor = s.ReadByte();
					SecondaryColor = s.ReadByte();
					TertiaryColor = s.ReadByte();
					PlayerModelChoice = s.ReadByte();
					ForegroundEmblem = s.ReadByte();
					BackgroundEmblem = s.ReadByte();
					EmblemFlags = s.ReadByte();
					EmblemPrimaryColor = s.ReadByte();
					EmblemSecondaryColor = s.ReadByte();
					EmblemBackgroundColor = s.ReadByte();
					ModelChoices[0] = s.ReadBytes(4);
					ModelChoices[1] = s.ReadBytes(4);
					ServiceTag = s.ReadUnicodeString(4);
					s.Seek(sizeof(ushort), System.IO.SeekOrigin.Current); // alignment
				}

				public void Write(BlamLib.IO.EndianWriter s)
				{
					s.Write(Valid);
					s.Write(PrimaryColor);
					s.Write(SecondaryColor);
					s.Write(TertiaryColor);
					s.Write(PlayerModelChoice);
					s.Write(ForegroundEmblem);
					s.Write(BackgroundEmblem);
					s.Write(EmblemFlags);
					s.Write(EmblemPrimaryColor);
					s.Write(EmblemSecondaryColor);
					s.Write(EmblemBackgroundColor);
					s.Write(ModelChoices[0], 4);
					s.Write(ModelChoices[1], 4);
					s.WriteUnicodeString(ServiceTag, 4);
					s.Write(ushort.MinValue); // alignment
				}
				#endregion
			};

			public class GamePlayerDataGlobalStats : IO.IStreamable // 0x10
			{
				public const uint SizeOf = 0x10;

				public bool Valid;
				public int ExperienceBase;
				private int ExperiencePenalty;
				public int HighestSkill;

				#region IStreamable Members
				public void Read(BlamLib.IO.EndianReader s)
				{
					Valid = s.ReadInt32() > 0;
					ExperienceBase = s.ReadInt32();
					ExperiencePenalty = s.ReadInt32();
					HighestSkill = s.ReadInt32();
				}

				public void Write(BlamLib.IO.EndianWriter s)
				{
					s.Write(Valid ? 1 : 0);
					s.Write(ExperienceBase);
					s.Write(ExperiencePenalty);
					s.Write(HighestSkill);
				}
				#endregion
			};

			public class GamePlayerDataDisplayedStats : IO.IStreamable // 0x2C
			{
				public const uint SizeOf = 0x2C;

				public bool Valid;
				public int RankedPlayed;
				public int RankedCompleted;
				public int RankedWon;
				public int UnrankedPlayed;
				public int UnrankedCompleted;
				public int UnrankedWon;
				public int CustomCompleted;
				public int CustomWon;

				// TODO: Not sure if these are actually part of this structure...
				private int Unknown024;
				private int Unknown028;

				#region IStreamable Members
				public void Read(BlamLib.IO.EndianReader s)
				{
					Valid = s.ReadInt32() > 0;
					RankedPlayed = s.ReadInt32();
					RankedCompleted = s.ReadInt32();
					RankedWon = s.ReadInt32();
					UnrankedPlayed = s.ReadInt32();
					UnrankedCompleted = s.ReadInt32();
					UnrankedWon = s.ReadInt32();
					CustomCompleted = s.ReadInt32();
					CustomWon = s.ReadInt32();
					Unknown024 = s.ReadInt32();
					Unknown028 = s.ReadInt32();
				}

				public void Write(BlamLib.IO.EndianWriter s)
				{
					s.Write(Valid ? 1 : 0);
					s.Write(RankedPlayed);
					s.Write(RankedCompleted);
					s.Write(RankedWon);
					s.Write(UnrankedPlayed);
					s.Write(UnrankedCompleted);
					s.Write(UnrankedWon);
					s.Write(CustomCompleted);
					s.Write(CustomWon);
					s.Write(Unknown024);
					s.Write(Unknown028);
				}
				#endregion
			};

			public class GamePlayerDataHopperData : IO.IStreamable // 0x1C
			{
				public const uint SizeOf = 0x1C;

				public bool Valid;
				public short HopperIdentifier;
				private int Unknown004;
				private int Unknown008;
				public int OldSkill;
				public int GamesPlayed;
				public int GamesCompleted;
				public int GamesWon;

				#region IStreamable Members
				public void Read(BlamLib.IO.EndianReader s)
				{
					Valid = s.ReadInt16() > 0;
					HopperIdentifier = s.ReadInt16();
					Unknown004 = s.ReadInt32();
					Unknown008 = s.ReadInt32();
					OldSkill = s.ReadInt32();
					GamesPlayed = s.ReadInt32();
					GamesCompleted = s.ReadInt32();
					GamesWon = s.ReadInt32();
				}

				public void Write(BlamLib.IO.EndianWriter s)
				{
					s.Write(ushort.MinValue + (Valid ? 1 : 0));
					s.Write(HopperIdentifier);
					s.Write(Unknown004);
					s.Write(Unknown008);
					s.Write(OldSkill);
					s.Write(GamesPlayed);
					s.Write(GamesCompleted);
					s.Write(GamesWon);
				}
				#endregion
			};

			public class GamePlayerData : IO.IStreamable // 0xC8
			{
				public const uint SizeOf = 0xC8;

				public string Name;
				public GamePlayerDataAppearance Appearance = new GamePlayerDataAppearance();
				private ulong Unknown040;
				private bool Unknown048; // is-silver-or-gold-live
				private bool Unknown049; // is-online-enabled
				private bool Unknown04A; // player-last-team-valid
				public byte LastTeam;
				private bool Unknown04C; // desires-veto
				private bool Unknown04D; // desires-rematch
				public byte HopperAccessFlags;
				private bool Unknown04F; // is-free-live-gold-account
				private bool Unknown050; // has-beta-permissions
				private bool Unknown051; // player-is-griefer, this may be a byte, just judged based on if its greater than zero
				private bool Unknown052;
				public byte CampaignHighestDifficulty;
				private int Unknown054;
				public int GamerRegion;
				public int GamerZone;
				public uint CheatFlags; // only one flag, so could use just as a boolean
				public uint BanFlags;
				public int RepeatedPlayCoefficient;
				private bool Unknown06C; // experience_growth_banned
				public GamePlayerDataGlobalStats GlobalStats = new GamePlayerDataGlobalStats();
				public GamePlayerDataDisplayedStats DisplayedStats = new GamePlayerDataDisplayedStats();
				public GamePlayerDataHopperData HopperData = new GamePlayerDataHopperData();

				#region IStreamable Members
				public void Read(BlamLib.IO.EndianReader s)
				{
					Name = s.ReadUnicodeString(10);
					Appearance.Read(s);
					Unknown040 = s.ReadUInt64();
					Unknown048 = s.ReadBool();
					Unknown049 = s.ReadBool();
					Unknown04A = s.ReadBool();
					LastTeam = s.ReadByte();
					Unknown04C = s.ReadBool();
					Unknown04D = s.ReadBool();
					HopperAccessFlags = s.ReadByte();
					Unknown04F = s.ReadBool();
					Unknown050 = s.ReadBool();
					Unknown051 = s.ReadBool();
					Unknown052 = s.ReadBool();
					CampaignHighestDifficulty = s.ReadByte();
					Unknown054 = s.ReadInt32();
					GamerRegion = s.ReadInt32();
					GamerZone = s.ReadInt32();
					CheatFlags = s.ReadUInt32();
					BanFlags = s.ReadUInt32();
					RepeatedPlayCoefficient = s.ReadInt32();
					Unknown06C = s.ReadInt32() > 0;
					GlobalStats.Read(s);
					DisplayedStats.Read(s);
					HopperData.Read(s);
				}

				public void Write(BlamLib.IO.EndianWriter s)
				{
					s.WriteUnicodeString(Name, 10);
					Appearance.Write(s);
					s.Write(Unknown040);
					s.Write(Unknown048);
					s.Write(Unknown049);
					s.Write(Unknown04A);
					s.Write(LastTeam);
					s.Write(Unknown04C);
					s.Write(Unknown04D);
					s.Write(HopperAccessFlags);
					s.Write(Unknown04F);
					s.Write(Unknown050);
					s.Write(Unknown051);
					s.Write(Unknown052);
					s.Write(CampaignHighestDifficulty);
					s.Write(Unknown054);
					s.Write(GamerRegion);
					s.Write(GamerZone);
					s.Write(CheatFlags);
					s.Write(BanFlags);
					s.Write(RepeatedPlayCoefficient);
					s.Write(Unknown06C ? 1 : 0);
					GlobalStats.Write(s);
					DisplayedStats.Write(s);
					HopperData.Write(s);
				}
				#endregion
			};

			public class GamePlayerMatchData : IO.IStreamable // 0x48
			{
				public const uint SizeOf = 0x48;

				public string Name;
				public int Team;
				public int AssignedTeam;

				public bool GlobalValid;
				public int GlobalExperience;
				public int GlobalRank;
				public int GlobalGrade;

				public bool HopperValid;
				public int HopperSkill;
				public int HopperSkillDisplay;
				public int HopperWeight;

				#region IStreamable Members
				public void Read(BlamLib.IO.EndianReader s)
				{
					Name = s.ReadUnicodeString(10);
					Team = s.ReadInt32();
					AssignedTeam = s.ReadInt32();
					
					GlobalValid = s.ReadInt32() > 0;
					GlobalExperience = s.ReadInt32();
					GlobalRank = s.ReadInt32();
					GlobalGrade = s.ReadInt32();

					HopperValid = s.ReadInt32() > 0;
					HopperSkill = s.ReadInt32();
					HopperSkillDisplay = s.ReadInt32();
					HopperWeight = s.ReadInt32();
				}

				public void Write(BlamLib.IO.EndianWriter s)
				{
					s.WriteUnicodeString(Name, 10);

					s.Write(GlobalValid ? 1 : 0);
					s.Write(GlobalExperience);
					s.Write(GlobalRank);
					s.Write(GlobalGrade);

					s.Write(HopperValid ? 1 : 0);
					s.Write(HopperSkill);
					s.Write(HopperSkillDisplay);
					s.Write(HopperWeight);
				}
				#endregion
			};

			public class GamePlayer : IO.IStreamable // 0x128
			{
				public const uint SizeOf = 0x128;

				public bool Valid;
				public bool LeftGame;
				public short UserIndex;
				public short ControllerIndex;
				//PAD16
				public byte[] MachineId; // [6]
				private ulong Unknown000E;
				//PAD16
				public GamePlayerData Data = new GamePlayerData();
				public GamePlayerMatchData MatchData = new GamePlayerMatchData();

				#region IStreamable Members
				public void Read(BlamLib.IO.EndianReader s)
				{
					Valid = s.ReadBool();
					LeftGame = s.ReadBool();
					UserIndex = s.ReadInt16();
					ControllerIndex = s.ReadInt16();
					s.Seek(2, System.IO.SeekOrigin.Current);
					MachineId = s.ReadBytes(6);
					Unknown000E = s.ReadUInt64();
					s.Seek(2, System.IO.SeekOrigin.Current);
					Data.Read(s);
					MatchData.Read(s);
				}

				public void Write(BlamLib.IO.EndianWriter s)
				{
					s.Write(Valid);
					s.Write(LeftGame);
					s.Write(UserIndex);
					s.Write(ControllerIndex);
					s.Write(ushort.MinValue);
					s.Write(MachineId, 6);
					s.Write(Unknown000E);
					s.Write(ushort.MinValue);
					Data.Write(s);
					MatchData.Write(s);
				}
				#endregion
			};

			public class GameInstance : IO.IStreamable // 0x12F0
			{
				public const uint SizeOf = 0x12F0;

				public bool InitialParticipantsExist;
				public uint MachineValidMask;
				public byte[][] MachineIds = new byte[16][];
				public GamePlayer[] Players = new GamePlayer[16];

				#region IStreamable Members
				public void Read(BlamLib.IO.EndianReader s)
				{
					InitialParticipantsExist = s.ReadInt32() > 0;
					MachineValidMask = s.ReadUInt32();
					for (int x = 0; x < MachineIds.Length; x++) MachineIds[x] = s.ReadBytes(6);
					for (int x = 0; x < Players.Length; x++) (Players[x] = new GamePlayer()).Read(s);
				}

				public void Write(BlamLib.IO.EndianWriter s)
				{
					s.Write(InitialParticipantsExist ? 1 : 0);
					s.Write(MachineValidMask);
					for (int x = 0; x < MachineIds.Length; x++) s.Write(MachineIds[x], 6);
					for (int x = 0; x < Players.Length; x++) Players[x].Write(s);
				}
				#endregion
			};

			public class GameOptions : IO.IStreamable // 0xF810, 1F348?
			{
				public const uint SizeOf = 0xF810;

				public int GameMode;					// 0x0
				public GameSimulation GameSimulation;	// 0x4
				public byte GameNetworkType;			// 0x5
				public short GameTickRate;				// 0x6
				private ulong GameInstance;				// 0x8
				private int Unknown010;					// 0x10
				public int Language;					// 0x14
				public int DeterminismVersion;			// 0x18
				public Blam.MapId MapId;				// 0x1C
				public string CachePath;				// 0x24, [260]
				public short InitialZoneSetIndex;		// 0x128
				private bool Unknown12A;				// 0x12A
				public byte DumpMachineIndex;			// 0x12B
				private bool Unknown12C;				// 0x12C
				private bool Unknown12D;				// 0x12D
				private bool Unknown12E;				// 0x12E
				//PAD8
				public GamePlayback GamePlayback;		// 0x130
				private bool Unknown132;				// 0x132
				//PAD8
				private int Unknown134;					// 0x134
				private int Unknown138;					// 0x138
				public short CampaignDifficulty;		// 0x13C
				public short CampaignInsertionPoint;	// 0x13E
				public short CampaignMetagameScoring;	// 0x140
				private bool Unknown142;				// 0x142
				private bool Unknown143;				// 0x143
				private int PrimarySkulls;				// 0x144
				private int SecondarySkulls;			// 0x148
				private byte[][] Unknown14C = new byte[4][]; // [30], 0x0 in each structure is a boolean marking if it is valid or not
				private bool Unknown1C4;				// 0x1C4
				//PAD24
				//PAD32?
				private byte[] Unknown1CC; // [92]
				public GameEngineVariant EngineVariant = new GameEngineVariant();	// 0x228
				//it looks like there should be a PAD32 here...
				public MapVariant MapVariant = new MapVariant();					// 0x490
				public GameInstance Game = new GameInstance();

				#region IStreamable Members
				public void Read(BlamLib.IO.EndianReader s)
				{
					GameMode = s.ReadInt32();
					GameSimulation = (GameSimulation)s.ReadByte();
					GameNetworkType = s.ReadByte();
					GameTickRate = s.ReadInt16();
					GameInstance = s.ReadUInt64();
					Unknown010 = s.ReadInt32();
					Language = s.ReadInt32();
					DeterminismVersion = s.ReadInt32();
					MapId.Read(s);
					CachePath = s.ReadAsciiString(260);
					InitialZoneSetIndex = s.ReadInt16();
					Unknown12A = s.ReadBool();
					DumpMachineIndex = s.ReadByte();
					Unknown12C = s.ReadBool();
					Unknown12D = s.ReadBool();
					Unknown12E = s.ReadBool();
					s.Seek(1, System.IO.SeekOrigin.Current);
					GamePlayback = (GamePlayback)s.ReadInt16();
					Unknown132 = s.ReadBool();
					s.Seek(1, System.IO.SeekOrigin.Current);
					Unknown134 = s.ReadInt32();
					Unknown138 = s.ReadInt32();
					CampaignDifficulty = s.ReadInt16();
					CampaignInsertionPoint = s.ReadInt16();
					CampaignMetagameScoring = s.ReadInt16();
					Unknown142 = s.ReadBool();
					Unknown143 = s.ReadBool();
					PrimarySkulls = s.ReadInt32();
					SecondarySkulls = s.ReadInt32();
					for (int x = 0; x < Unknown14C.Length; x++) Unknown14C[x] = s.ReadBytes(30);
					Unknown1C4 = s.ReadBool();
					s.Seek(3 + 4, System.IO.SeekOrigin.Current);
					Unknown1CC = s.ReadBytes(92);
					EngineVariant.Read(s);
					s.Seek(4, System.IO.SeekOrigin.Current);
					MapVariant.Read(s);
					Game.Read(s);
				}

				public void Write(BlamLib.IO.EndianWriter s)
				{
					s.Write(GameMode);
					s.Write((byte)GameSimulation);
					s.Write(GameNetworkType);
					s.Write(GameTickRate);
					s.Write(GameInstance);
					s.Write(Unknown010);
					s.Write(Language);
					s.Write(DeterminismVersion);
					MapId.Write(s);
					s.Write(CachePath, 260);
					s.Write(InitialZoneSetIndex);
					s.Write(Unknown12A);
					s.Write(DumpMachineIndex);
					s.Write(Unknown12C);
					s.Write(Unknown12D);
					s.Write(Unknown12E);
					s.Write(byte.MinValue);
					s.Write((short)GamePlayback);
					s.Write(Unknown132);
					s.Write(byte.MinValue);
					s.Write(Unknown134);
					s.Write(Unknown138);
					s.Write(CampaignDifficulty);
					s.Write(CampaignInsertionPoint);
					s.Write(CampaignMetagameScoring);
					s.Write(Unknown142);
					s.Write(Unknown143);
					s.Write(PrimarySkulls);
					s.Write(SecondarySkulls);
					for (int x = 0; x < Unknown14C.Length; x++) s.Write(Unknown14C[x], 30);
					s.Write(Unknown1C4);
					s.Write(new byte[3 + 4], 3 + 4);
					s.Write(Unknown1CC, 92);
					EngineVariant.Write(s);
					s.Write(uint.MinValue);
					MapVariant.Write(s);
					Game.Write(s);
				}
				#endregion
			};

			private int Unknown000;
			public string BuildString; // [32]
			public ExecutableType ExecutableType;
			public int ExecutableVersion;
			public int CompatibleVersion;
			public int Language;
			public int MapMinorVersion;
			private uint Unknown038; // byte value relating to the minor version stored in the first 8bits
			private uint Unknown03C;
			private uint Unknown040;
			public int SignatureLength;
			public byte[] Signature; // [60]
			private bool Unknown084;
			private bool Unknown085; // related to specifying film as a snippet
			private bool Unknown086; // related to specifying film as a snippet (and with a gamestate?)
			private bool Unknown087;
			private int Unknown088;
			public string SessionName; // [128]
			public GameOptions Options = new GameOptions();
			private uint UnknownF91C; // time_t structure
			public uint LengthInTicks;
			public uint SnippetStartTick;
			private byte[] UnknownF928; // [1336]

			#region Entry Members
			public const uint kSizeOf = 0xFE60;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups.flmh; } }
			public override uint SizeOf { get { return kBaseSizeOf + kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);

				Unknown000 = s.ReadInt32();
				BuildString = s.ReadTagString();
				ExecutableType = (ExecutableType)s.ReadInt32();
				ExecutableVersion = s.ReadInt32();
				CompatibleVersion = s.ReadInt32();
				Language = s.ReadInt32();
				MapMinorVersion = s.ReadInt32();
				Unknown038 = s.ReadUInt32();
				Unknown03C = s.ReadUInt32();
				Unknown040 = s.ReadUInt32();
				SignatureLength = s.ReadInt32();
				Signature = s.ReadBytes(60);
				Unknown084 = s.ReadBool();
				Unknown085 = s.ReadBool();
				Unknown086 = s.ReadBool();
				Unknown087 = s.ReadBool();
				Unknown088 = s.ReadInt32();
				SessionName = s.ReadAsciiString(128);
				Options.Read(s);
				UnknownF91C = s.ReadUInt32();
				LengthInTicks = s.ReadUInt32();
				SnippetStartTick = s.ReadUInt32();
				UnknownF928 = s.ReadBytes(1336);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);

				s.Write(Unknown000);
				s.Write(BuildString, false);
				s.Write((int)ExecutableType);
				s.Write(ExecutableVersion);
				s.Write(CompatibleVersion);
				s.Write(Language);
				s.Write(MapMinorVersion);
				s.Write(Unknown038);
				s.Write(Unknown03C);
				s.Write(Unknown040);
				s.Write(SignatureLength);
				s.Write(Signature, 60);
				s.Write(Unknown084);
				s.Write(Unknown085);
				s.Write(Unknown086);
				s.Write(Unknown087);
				s.Write(Unknown088);
				s.Write(SessionName, 128);
				Options.Write(s);
				s.Write(UnknownF91C);
				s.Write(LengthInTicks);
				s.Write(SnippetStartTick);
				s.Write(UnknownF928, 1336);
			}
			#endregion
		};

		// Film Data
		public class FLMD : Entry
		{
			public class Playload : IO.IStreamable
			{
				public int Size;
				public uint TickId;
				public byte[] Buffer;

				#region IStreamable Members
				public void Read(BlamLib.IO.EndianReader s)
				{
					Size = s.ReadInt32() - 4;
					TickId = s.ReadUInt32();
					Buffer = s.ReadBytes(Size);
				}

				public void Write(BlamLib.IO.EndianWriter s)
				{
					s.Write(Buffer.Length + 4);
					s.Write(TickId);
					s.Write(Buffer);
				}
				#endregion
			};

			public List<Playload> Playloads = new List<Playload>();

			#region Entry Members
			public const uint kSizeOf = uint.MaxValue;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups.flmd; } }
			public override uint SizeOf { get { return kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);

				// This calculation may not work in every case.
				// Noticed one film where there was yet 0x10000 bytes left before the EOF block
				uint end_pos = s.PositionUnsigned + (base.BlockSize - kBaseSizeOf);

				Playload pl;
				while(s.PositionUnsigned < end_pos)
				{
					(pl = new Playload()).Read(s);
					Playloads.Add(pl);
				}
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s, base.BlockSize);

				foreach(Playload pl in Playloads) pl.Write(s);
			}
			#endregion
		};

		// Multiplayer Variant
		public class MPVR : Entry
		{
			public GameEngineVariant EngineVariant = new GameEngineVariant();

			#region Entry Members
			public const uint kSizeOf = 0x264;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups.mpvr; } }
			public override uint SizeOf { get { return kBaseSizeOf + kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);

				EngineVariant.Read(s);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);

				EngineVariant.Write(s);
			}
			#endregion
		};

		// Map Variant
		public class MAPV : Entry
		{
			public MapVariant Variant = new MapVariant();

			#region Entry Members
			public const uint kSizeOf = 0xE094;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups.mapv; } }
			public override uint SizeOf { get { return kBaseSizeOf + kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);

				Variant.Read(s);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);

				Variant.Write(s);
			}
			#endregion
		};

		public class LEVL : Entry
		{
			public class CheckpointData : IO.IStreamable // 0xF08
			{
				public const uint SizeOf = 0xF08;

				private short Unknown000;
				private short Unknown002;
				private int Unknown004;
				public LocalizedName32 Names = new LocalizedName32();
				public LocalizedDescription Descriptions = new LocalizedDescription();

				#region IStreamable Members
				public void Read(BlamLib.IO.EndianReader s)
				{
					Unknown000 = s.ReadInt16();
					Unknown002 = s.ReadInt16();
					Unknown004 = s.ReadInt32();
					Names.Read(s);
					Descriptions.Read(s);
				}

				public void Write(BlamLib.IO.EndianWriter s)
				{
					s.Write(Unknown000);
					s.Write(Unknown002);
					s.Write(Unknown004);
					Names.Write(s);
					Descriptions.Write(s);
				}
				#endregion
			};

			public class SurvivalModeData : IO.IStreamable // 0xF10
			{
				public const uint SizeOf = 0xF10;

				private short Unknown000;
				private short Unknown002;
				private int Unknown004;
				private int Unknown008;
				public LocalizedName32 Names = new LocalizedName32();
				public LocalizedDescription Descriptions = new LocalizedDescription();

				#region IStreamable Members
				public void Read(BlamLib.IO.EndianReader s)
				{
					Unknown000 = s.ReadInt16();
					Unknown002 = s.ReadInt16();
					Unknown004 = s.ReadInt32();
					Unknown008 = s.ReadInt32();
					Names.Read(s);
					Descriptions.Read(s);
				}

				public void Write(BlamLib.IO.EndianWriter s)
				{
					s.Write(Unknown000);
					s.Write(Unknown002);
					s.Write(Unknown004);
					s.Write(Unknown008);
					Names.Write(s);
					Descriptions.Write(s);
				}
				#endregion
			};

			public int MapId;
			public int ScenarioType; // encoded in some kind of bit magic
			public LocalizedName32 Names = new LocalizedName32();
			public LocalizedDescription Descriptions = new LocalizedDescription();
			public string BlfName;
			public string FileName;
			public int MapIndex;
			public int Unknown1118;
			public byte Unknown111C;
			public byte Unknown111D;
			public byte[] MaxTeams; // max teams per game type
			public byte Unknown1129;
			public uint Unknown112C;
			public CheckpointData[] Checkpoints = new CheckpointData[4];
			// ODST:
			//public SurvivalModeData[] SurvivalModeMissions = new SurvivalModeData[5];

			#region Entry Members
			public const uint kSizeOf = 0x4D44; // ODST: 98B4

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups.levl; } }
			public override uint SizeOf { get { return kBaseSizeOf + kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);

				MapId = s.ReadInt32();
				ScenarioType = s.ReadInt32();
				Names.Read(s);
				Descriptions.Read(s);
				BlfName = s.ReadAsciiString(256);
				FileName = s.ReadAsciiString(256);
				MapIndex = s.ReadInt32();
				Unknown1118 = s.ReadInt32();
				Unknown111C = s.ReadByte();
				Unknown111D = s.ReadByte();
				MaxTeams = s.ReadBytes(11);
				Unknown1129 = s.ReadByte();
				s.Seek(2, System.IO.SeekOrigin.Current);
				Unknown112C = s.ReadUInt32();
				foreach (CheckpointData cp in Checkpoints) cp.Read(s);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);

				s.Write(MapId);
				s.Write(ScenarioType);
				Names.Write(s);
				Descriptions.Write(s);
				s.WriteUnicodeString(BlfName, 256);
				s.WriteUnicodeString(FileName, 256);
				s.Write(MapIndex);
				s.Write(Unknown1118);
				s.Write(Unknown111C);
				s.Write(Unknown111D);
				s.Write(MaxTeams);
				s.Write(Unknown1129);
				s.Write(ushort.MinValue);
				s.Write(Unknown112C);
				foreach (CheckpointData cp in Checkpoints) cp.Write(s);
			}
			#endregion
		};

		public class CMPN : Entry
		{
			public int CampaignId;
			public uint Unknown0004;
			public LocalizedName64 Names = new LocalizedName64();
			public LocalizedDescription Descriptions = new LocalizedDescription();
			public int[] LevelIds = new int[64];
			public int Unknown1308; // This is either unused, or part of level ids but as a terminator (meaning its always zero anyway)

			#region Entry Members
			public const uint kSizeOf = 0x130C;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups.cmpn; } }
			public override uint SizeOf { get { return kBaseSizeOf + kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);

				CampaignId = s.ReadInt32();
				Unknown0004 = s.ReadUInt32();
				Names.Read(s);
				Descriptions.Read(s);
				for (int x = 0; x < LevelIds.Length; x++) LevelIds[x] = s.ReadInt32();
				Unknown1308 = s.ReadInt32();
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);

				s.Write(CampaignId);
				s.Write(Unknown0004);
				Names.Write(s);
				Descriptions.Write(s);
				foreach (int level in LevelIds) s.Write(level);
				s.Write(Unknown1308);
			}
			#endregion
		};

		public class MAPI : Entry
		{
			#region Entry Members
			public const uint kSizeOf = uint.MaxValue;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups.mapi; } }
			public override uint SizeOf { get { return kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);
			}
			#endregion
		};

		public class GVAR : Entry
		{
			#region Entry Members
			public const uint kSizeOf = uint.MaxValue;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups.gvar; } }
			public override uint SizeOf { get { return kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);
			}
			#endregion
		};

		public class MVAR : Entry
		{
			#region Entry Members
			public const uint kSizeOf = uint.MaxValue;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups.mvar; } }
			public override uint SizeOf { get { return kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);
			}
			#endregion
		};

		public class ONFM : Entry
		{
			#region Entry Members
			public const uint kSizeOf = uint.MaxValue;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups.onfm; } }
			public override uint SizeOf { get { return kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);
			}
			#endregion
		};

		public class SRID : Entry
		{
			#region Entry Members
			public const uint kSizeOf = uint.MaxValue;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups.srid; } }
			public override uint SizeOf { get { return kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);
			}
			#endregion
		};

		public class SCND : Entry
		{
			#region Entry Members
			public const uint kSizeOf = uint.MaxValue;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups.scnd; } }
			public override uint SizeOf { get { return kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);
			}
			#endregion
		};

		public class NETC : Entry
		{
			#region Entry Members
			public const uint kSizeOf = uint.MaxValue;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups.netc; } }
			public override uint SizeOf { get { return kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);
			}
			#endregion
		};

		public class MAPM : Entry
		{
			#region Entry Members
			public const uint kSizeOf = uint.MaxValue;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups.mapm; } }
			public override uint SizeOf { get { return kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);
			}
			#endregion
		};

		// banhammer data
		public class FUBH : Entry
		{
			#region Entry Members
			public const uint kSizeOf = uint.MaxValue;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups.fubh; } }
			public override uint SizeOf { get { return kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);
			}
			#endregion
		};

		// network stats
		public class FUNS : Entry
		{
			#region Entry Members
			public const uint kSizeOf = uint.MaxValue;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups.funs; } }
			public override uint SizeOf { get { return kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);
			}
			#endregion
		};

		// upload related?
		public class FUPD : Entry
		{
			#region Entry Members
			public const uint kSizeOf = uint.MaxValue;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups.fupd; } }
			public override uint SizeOf { get { return kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);
			}
			#endregion
		};

		public class FILQ : Entry
		{
			#region Entry Members
			public const uint kSizeOf = uint.MaxValue;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups.filq; } }
			public override uint SizeOf { get { return kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);
			}
			#endregion
		};

		public class FURP : Entry
		{
			#region Entry Members
			public const uint kSizeOf = uint.MaxValue;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups.furp; } }
			public override uint SizeOf { get { return kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);
			}
			#endregion
		};

		public class BHMS : Entry
		{
			#region Entry Members
			public const uint kSizeOf = uint.MaxValue;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups.bhms; } }
			public override uint SizeOf { get { return kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);
			}
			#endregion
		};

		public class MMHS : Entry
		{
			#region Entry Members
			public const uint kSizeOf = uint.MaxValue;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups.mmhs; } }
			public override uint SizeOf { get { return kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);
			}
			#endregion
		};

		// message of the day
		public class MOTD : Entry
		{
			#region Entry Members
			public const uint kSizeOf = uint.MaxValue;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups.motd; } }
			public override uint SizeOf { get { return kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);
			}
			#endregion
		};

		// compressed
		public class CMP : Entry
		{
			#region Entry Members
			public const uint kSizeOf = uint.MaxValue;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups._cmp; } }
			public override uint SizeOf { get { return kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);
			}
			#endregion
		};

		// matchmaking tip
		public class MMTP : Entry
		{
			#region Entry Members
			public const uint kSizeOf = uint.MaxValue;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups._blf; } }
			public override uint SizeOf { get { return kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);
			}
			#endregion
		};

		public class MHCF : Entry
		{
			#region Entry Members
			public const uint kSizeOf = uint.MaxValue;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups._blf; } }
			public override uint SizeOf { get { return kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);
			}
			#endregion
		};

		public class MHDF : Entry
		{
			#region Entry Members
			public const uint kSizeOf = uint.MaxValue;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups._blf; } }
			public override uint SizeOf { get { return kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);
			}
			#endregion
		};

		public class GSET : Entry
		{
			#region Entry Members
			public const uint kSizeOf = uint.MaxValue;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups._blf; } }
			public override uint SizeOf { get { return kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);
			}
			#endregion
		};

		// minidump
		public class MDSC : Entry
		{
			#region Entry Members
			public const uint kSizeOf = uint.MaxValue;

			public override TagInterface.TagGroup GroupTag { get { return MiscGroups._blf; } }
			public override uint SizeOf { get { return kSizeOf; } }

			public override void Read(BlamLib.IO.EndianReader s)
			{
				base.Read(s);
			}

			public override void Write(BlamLib.IO.EndianWriter s)
			{
				base.Write(s);
			}
			#endregion
		};
	};
}