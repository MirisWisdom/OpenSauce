/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib
{
	/// <summary>Extension class for the various Blam related enums</summary>
	public static class BlamExtensions
	{
		internal const string kDefaultGuidHalo1 = "{17ce36ab-9d9f-4abb-a8b8-c3ad82362ed8}";
		internal const string kDefaultGuidHalo2 = "{81a96c0f-53e7-402c-bba7-69f999fe982b}";
		internal const string kDefaultGuidHalo3 = "{175984d9-c972-44ff-9a27-08b9663b4ec1}";
		internal const string kDefaultGuidStubbs = "{0ffebbf1-2eff-429d-ba48-1ef7f40c4232}";
		internal const string kDefaultGuidHaloOdst = "{532b463e-7f32-42ce-93f6-a5f1874ab007}";
		internal const string kDefaultGuidHaloReach = "{9755fe07-04b1-4bb9-a896-766ba111bb15}";
		internal const string kDefaultGuidHalo4 = "{D49D8313-E598-4c74-8C94-11C321D0D1CB}";

		public static bool HasFlag(this BlamVersion ver, BlamVersion flag)
		{
			return (ver & flag) == flag;
		}

		#region Conversion Utils
		/// <summary>BlamVersion to BlamBuild</summary>
		/// <param name="ver">game engine version</param>
		/// <returns></returns>
		public static BlamBuild ToBuild(this BlamVersion ver)		{ return ((BlamBuild)ver) & BlamBuild.kAll; }

		/// <summary>BlamVersion to BlamPlatform</summary>
		/// <param name="ver">game engine version</param>
		/// <returns></returns>
		public static BlamPlatform ToPlatform(this BlamVersion ver)	{ return ((BlamPlatform)ver) & BlamPlatform.kAll; }

		/// <summary>BlamVersion from BlamBuild</summary>
		/// <param name="build">game engine</param>
		/// <returns></returns>
		public static BlamVersion ToVersion(this BlamBuild build)	{ return (BlamVersion)build; }

		/// <summary>BlamVersion from BlamBuild and BlamPlatform</summary>
		/// <param name="build">game engine</param>
		/// <param name="platform">game engine platform</param>
		/// <returns></returns>
		public static BlamVersion Version(BlamBuild build, BlamPlatform platform)
		{
			return ((BlamVersion)build) | ((BlamVersion)platform);
		}
		#endregion

		#region IO Utils
		/// <summary>Returns true if the supplied blam engine makes use of string ids</summary>
		/// <param name="version">game engine version</param>
		/// <returns></returns>
		public static bool UsesStringIds(this BlamVersion version)
		{
			return	((version & BlamVersion.Halo1) == 0) &&
					((version & BlamVersion.Stubbs) == 0);
		}

		public static bool SupportsTagVersioning(this BlamVersion version)
		{
			return
				version == BlamVersion.Halo2_Alpha ||
				version == BlamVersion.Halo2_Epsilon ||
				version == BlamVersion.Halo2_Xbox ||
				version == BlamVersion.Halo3_Beta;
			// TODO: ODST? Reach?
		}

		/// <summary>
		/// Get the EndianState used for cache files in a specific engine
		/// </summary>
		/// <param name="version"></param>
		/// <returns></returns>
		public static IO.EndianState GetCacheEndianState(this BlamVersion version)
		{
			if (version.IsXbox360())
				// TODO: Do we need to use Big for Halo Mac too? Stubbs Mac appears to be Little...
				return BlamLib.IO.EndianState.Big;

			return BlamLib.IO.EndianState.Little;
		}
		#endregion

		#region Platform Utils
		/// <summary>
		/// Returns true if the supplied blam engine makes use of field set version 
		/// headers in the tag file streams
		/// </summary>
		/// <param name="version">game engine version</param>
		/// <returns></returns>
		public static bool UsesFieldSetVersionHeader(this BlamVersion version)
		{
			return	((version & BlamVersion.Halo1) == 0) &&
					((version & BlamVersion.Stubbs) == 0);
		}
		
		/// <summary>Returns true if the engine version is for any of the xbox systems</summary>
		/// <param name="version">game engine version</param>
		/// <returns></returns>
		public static bool IsXbox(this BlamVersion version)
		{
			return ((version & BlamVersion.Xbox) != 0);
		}

		/// <summary>Returns true if the engine version is for the xbox 1</summary>
		/// <param name="version">game engine version</param>
		/// <returns></returns>
		public static bool IsXbox1(this BlamVersion version)
		{
			return
				((version & BlamVersion.Xbox) != 0) && (
					((version & BlamVersion.Halo1) != 0) ||
					((version & BlamVersion.Halo2) != 0)
				);
		}

		/// <summary>Returns true if the engine version is for the xbox 360</summary>
		/// <param name="version">game engine version</param>
		/// <returns></returns>
		public static bool IsXbox360(this BlamVersion version)
		{
			return
				((version & BlamVersion.Xbox) != 0) && (
					((version & BlamVersion.Halo3) != 0) ||
					((version & BlamVersion.HaloOdst) != 0) ||
					((version & BlamVersion.HaloReach) != 0) ||
					((version & BlamVersion.Halo4) != 0)
				);
		}

		/// <summary>Returns true if the engine version is for anything other than a game console</summary>
		/// <param name="version">game engine version</param>
		/// <returns></returns>
		/// <remarks>Returns true if its for the mac as well</remarks>
		public static bool IsPc(this BlamVersion version)
		{
			return
				((version & BlamVersion.PC) != 0) ||
				((version & BlamVersion.Mac) != 0);
		}

		/// <summary>
		/// Returns true if the engine version is equal to one of the "extended" engine values
		/// </summary>
		/// <param name="version">game engine version</param>
		/// <returns></returns>
		public static bool IsSpecial(this BlamVersion version)
		{
			return
				((version & BlamVersion.Beta) != 0) ||
				((version & BlamVersion.Epsilon) != 0) ||
				((version & BlamVersion.Extended) != 0);
		}
		#endregion

		#region Tag Group Collections
		/// <summary>
		/// Determines the collection of tag groups to use for the provided engine version
		/// </summary>
		/// <param name="v">Blam engine version</param>
		/// <returns></returns>
		/// <exception cref="Debug.ExceptionLog"><paramref name="v"/> doesn't match any expected base game versions</exception>
		public static TagInterface.TagGroupCollection VersionToTagCollection(this BlamVersion v)
		{
			if ((v & BlamVersion.Halo1) != 0)			return Blam.Halo1.TagGroups.Groups;
#if !NO_HALO2
			else if ((v & BlamVersion.Halo2) != 0)		return Blam.Halo2.TagGroups.Groups;
#endif
#if !NO_HALO3
			else if ((v & BlamVersion.Halo3) != 0)		return Blam.Halo3.TagGroups.Groups;
#endif
#if !NO_HALO_ODST
			else if ((v & BlamVersion.HaloOdst) != 0)	return Blam.HaloOdst.TagGroups.Groups;
#endif
#if !NO_HALO_REACH
			else if ((v & BlamVersion.HaloReach) != 0)	return Blam.HaloReach.TagGroups.Groups;
#endif
#if !NO_HALO4
			else if ((v & BlamVersion.Halo4) != 0)		return Blam.Halo4.TagGroups.Groups;
#endif
			else if ((v & BlamVersion.Stubbs) != 0)		return Blam.Stubbs.TagGroups.Groups;
			else	throw new Debug.Exceptions.UnreachableException(v);
		}

		/// <summary>
		/// Determines the collection of struct groups to use for the provided engine version
		/// </summary>
		/// <param name="version"></param>
		/// <returns></returns>
		public static TagInterface.TagGroupCollection VersionToStructCollection(this BlamVersion version)
		{
#if !NO_HALO2
			if ((version & BlamVersion.Halo2) != 0)		return Blam.Halo2.StructGroups.Groups;
			else
#endif
#if !NO_HALO3
			if ((version & BlamVersion.Halo3) != 0)		return Blam.Halo3.StructGroups.Groups;
			else
#endif
#if !NO_HALO_ODST
			if ((version & BlamVersion.HaloOdst) != 0)	return Blam.Halo3.StructGroups.Groups; // TODO: make ODST group
			else
#endif
#if !NO_HALO_REACH
			if ((version & BlamVersion.HaloReach) != 0)	return Blam.HaloReach.StructGroups.Groups;
			else
#endif
#if !NO_HALO4
			if ((version & BlamVersion.Halo4) != 0)		return Blam.Halo4.StructGroups.Groups;
			else
#endif
			throw new Debug.Exceptions.UnreachableException(version);
		}
		#endregion

		#region Streaming Extensions
		/// <summary>
		/// Extension method for <see cref="BlamBuild"/> for streaming an instance from a stream
		/// </summary>
		/// <param name="build"></param>
		/// <param name="s"></param>
		public static void Read(this BlamBuild build, IO.EndianReader s)		{ build = (BlamBuild)s.ReadUInt16(); }
		/// <summary>
		/// Extension method for <see cref="BlamBuild"/> for streaming an instance to a stream
		/// </summary>
		/// <param name="build"></param>
		/// <param name="s"></param>
		public static void Write(this BlamBuild build, IO.EndianWriter s)		{ s.Write((ushort)build); }

		/// <summary>
		/// Extension method for <see cref="BlamPlatform"/> for streaming an instance to a stream
		/// </summary>
		/// <param name="platform"></param>
		/// <param name="s"></param>
		public static void Read(this BlamPlatform platform, IO.EndianReader s)	{ platform = (BlamPlatform)s.ReadUInt16(); }
		/// <summary>
		/// Extension method for <see cref="BlamPlatform"/> for streaming an instance to a stream
		/// </summary>
		/// <param name="platform"></param>
		/// <param name="s"></param>
		public static void Write(this BlamPlatform platform, IO.EndianWriter s)	{ s.Write((ushort)platform); }

		/// <summary>
		/// Extension method for <see cref="BlamVersion"/> for streaming an instance from a stream
		/// </summary>
		/// <param name="version"></param>
		/// <param name="s"></param>
		public static void Read(this BlamVersion version, IO.EndianReader s)	{ version = (BlamVersion)s.ReadUInt16(); }
		/// <summary>
		/// Extension method for <see cref="BlamVersion"/> for streaming an instance to a stream
		/// </summary>
		/// <param name="version"></param>
		/// <param name="s"></param>
		public static void Write(this BlamVersion version, IO.EndianWriter s)	{ s.Write((ushort)version); }
		#endregion
	};

	/// <summary>The various builds of the blam engine</summary>
	[Flags]
	public enum BlamBuild : ushort
	{
		/// <summary>Unknown Engine</summary>
		Unknown = 0,

		/// <summary>Halo 1 engine</summary>
		Halo1 = 256,
		/// <summary>Stubbs the Zombie engine</summary>
		Stubbs = 512,
		/// <summary>Halo 2 engine</summary>
		Halo2 = 1024,
		/// <summary>Halo 3 engine</summary>
		Halo3 = 2048,
		/// <summary>Halo 3: ODST engine</summary>
		HaloOdst = 4096,
		/// <summary>Halo Reach engine</summary>
		HaloReach = 8192,
		/// <summary>Halo 4 engine</summary>
		Halo4 = 16384,

		kAll = Halo1 | Stubbs | Halo2 | Halo3 | HaloOdst | HaloReach | Halo4
	};

	/// <summary>The various platforms for the blam engine</summary>
	[Flags]
	public enum BlamPlatform : ushort
	{
		/// <summary>Unknown platform</summary>
		Unknown = 0,

		/// <summary>Xbox 1 or 360 platform</summary>
		Xbox = 1,
		/// <summary>PC platform</summary>
		PC = 2,
		/// <summary>Macintosh platform</summary>
		Mac = 4,

		// 8 - could use this for PS3 later or another build id

		Beta = 16,
		Epsilon = 32,
		/// <summary>Mainly a 'hack' for HaloCE</summary>
		Extended = 64,
		/// <summary>Mainly a 'hack' for Halo Anniversary</summary>
		Remake = 128,

		kAll = Xbox | PC | Mac |
			Beta | Epsilon | Extended
	};

	/// <summary>The various versions of the blam engine</summary>
	/// <remarks>
	/// This is internally treated as a <see cref="UInt16"/> because the original tag file format had an 
	/// unused 16-bit field. Since BlamLib interops with official (Halo1, Halo2) file formats, it uses this 
	/// field to store the blam-version of the tag data. However, BlamLib never depends on the tag header data 
	/// expect what the official tag file format guarantees. The extra information written to tag files is 
	/// kept in for future uses in other tools.
	/// 
	/// Looking back, I should have made this enumeration independent from the old tag file formats used in 
	/// the released tools. I should have made a specific ID which I stored in the 16-bit field that I could 
	/// translate back into a more robust blam-version-id.
	/// 
	/// Even this description system with bit-wise encodings isn't exactly the best method for representing the 
	/// many variants of Blam. If I were to redo this library, I probably would go with a custom structure that 
	/// still allowed me to do simple comparisons and strong platform information.
	/// </remarks>
	[Flags]
	public enum BlamVersion : ushort
	{
		/// <summary>Unknown Version</summary>
		Unknown = 0,

		#region Base - Platform (see BlamPlatform)
		Xbox =	BlamPlatform.Xbox,
		PC =	BlamPlatform.PC,
		Mac =	BlamPlatform.Mac,
		#endregion

		// 8 - could be used for either Platform or Type (more likely to get used for a Type)

		#region Base - Type (see BlamPlatform)
		Beta =		BlamPlatform.Beta,
		Epsilon =	BlamPlatform.Epsilon,
		Extended =	BlamPlatform.Extended,
		Remake =	BlamPlatform.Remake,
		#endregion

		#region Base - Game (see BlamBuild)
		/// <summary>Halo 1 base version</summary>
		Halo1 =		BlamBuild.Halo1,
		/// <summary>Stubbs the Zombie base version</summary>
		Stubbs =	BlamBuild.Stubbs,
		/// <summary>Halo 2 base version</summary>
		Halo2 =		BlamBuild.Halo2,
		/// <summary>Halo 3 base version</summary>
		Halo3 =		BlamBuild.Halo3,
		/// <summary>Halo 3: ODST base version</summary>
		HaloOdst =	BlamBuild.HaloOdst,
		/// <summary>Halo Reach base version</summary>
		HaloReach =	BlamBuild.HaloReach,
		/// <summary>Halo 4 base version</summary>
		Halo4 =		BlamBuild.Halo4,
		#endregion

		// 32768 - could be used for potential future Blam engines


		#region Halo 1
		/// <summary>Halo 1 (Xbox 1)</summary>
		Halo1_Xbox =	Halo1 | Xbox,
		/// <summary>Halo 1 (Xbox 360) Anniversary</summary>
		Halo1_XboxX =	Halo1 | Xbox | Remake,
		/// <summary>Halo 1 (PC)</summary>
		Halo1_PC =		Halo1 | PC,
		/// <summary>Halo 1 (PC) Custom Edition</summary>
		Halo1_CE =		Halo1 | PC | Extended,
		/// <summary>Halo 1 (PC) Anniversary</summary>
		Halo1_PCX =		Halo1 | PC | Remake,
		/// <summary>Halo 1 (Mac)</summary>
		Halo1_Mac =		Halo1 | Mac,

		/// <summary>Halo 1 (All)</summary>
		kHalo1 = Halo1_Xbox | Halo1_XboxX | Halo1_PC | Halo1_CE | Halo1_PCX | Halo1_Mac,
		#endregion

		#region Stubbs
		/// <summary>Stubbs the Zombie (Xbox 1)</summary>
		Stubbs_Xbox =	Stubbs | Xbox,
		/// <summary>Stubbs the Zombie (PC)</summary>
		Stubbs_PC =		Stubbs | PC,
		/// <summary>Stubbs the Zombie (Mac)</summary>
		Stubbs_Mac =	Stubbs | Mac,

		/// <summary>Stubbs the Zombie (All)</summary>
		kStubbs = Stubbs_Xbox | Stubbs_PC | Stubbs_Mac,
		#endregion

		#region Halo2
		/// <summary>Halo 2 (Xbox 1)</summary>
		Halo2_Xbox =	Halo2 | Xbox,
		/// <summary>Halo 2 (Xbox 1) Alpha</summary>
		Halo2_Alpha =	Halo2 | Xbox | Beta,
		/// <summary>Halo 2 (Xbox 1) Epsilon</summary>
		/// <remarks>Pre-Cert</remarks>
		Halo2_Epsilon = Halo2 | Xbox | Epsilon,
		/// <summary>Halo 2 (PC) Vista</summary>
		Halo2_PC =		Halo2 | PC,

		/// <summary>Halo 2 (All)</summary>
		/// <remarks>Intentionally doesn't include <see cref="Halo2_Epsilon"/></remarks>
		kHalo2 = Halo2_Alpha | Halo2_Xbox | Halo2_PC,
		#endregion

		#region Halo3
		/// <summary>Halo 3 (Xbox 360)</summary>
		Halo3_Xbox =	Halo3 | Xbox,
		/// <summary>Halo 3 (Xbox 360) Beta</summary>
		Halo3_Beta =	Halo3 | Xbox | Beta,
		/// <summary>Halo 3 (Xbox 360) Epsilon</summary>
		Halo3_Epsilon =	Halo3 | Xbox | Epsilon,
//		/// <summary>Halo 3 (PC)</summary>
//		Halo3_PC =		Halo3 | PC,

//		/// <summary>Halo 3 (All)</summary>
//		/// <remarks>Intentionally doesn't include <see cref="Halo3_Epsilon"/></remarks>
//		kHalo3 = Halo3_Beta | Halo3_Xbox /*| Halo3_PC*/,
		#endregion

		#region HaloOdst
		/// <summary>Halo 3: ODST (Xbox 360)</summary>
		HaloOdst_Xbox =	HaloOdst | Xbox,
//		/// <summary>Halo 3: ODST (PC)</summary>
//		HaloOdst_PC =	HaloOdst | PC,

//		/// <summary>Halo 3: ODST (All)</summary>
//		kHaloOdst = HaloOdst_Xbox | HaloOdst_PC,
		#endregion

		#region HaloReach
		/// <summary>Halo: Reach (Xbox 360)</summary>
		HaloReach_Xbox = HaloReach | Xbox,
		/// <summary>Halo: Reach (Xbox 360) Beta</summary>
		HaloReach_Beta = HaloReach | Xbox | Beta,
//		/// <summary>Halo: Reach (PC)</summary>
//		HaloReach_PC = HaloReach | PC,

//		/// <summary>Halo: Reach (All)</summary>
//		kHaloReach = HaloReach_Beta | HaloReach_Xbox | HaloReach_PC,
		#endregion

		#region Halo4
		/// <summary>Halo 4 (Xbox 360)</summary>
		Halo4_Xbox = Halo4 | Xbox,
//		/// <summary>Halo 4 (PC)</summary>
//		Halo4_PC = Halo4 | PC,

//		/// <summary>Halo: Reach (All)</summary>
//		kHalo4 = Halo4_Xbox | Halo4_PC,
		#endregion

		/// <summary>All supported base games</summary>
		kAllEngines = Halo1 | Stubbs | Halo2 | Halo3 | HaloOdst | HaloReach,
		/// <summary>All supported versions</summary>
		kAll = kHalo1 | kStubbs | kHalo2 |
			/*kHalo3*/Halo3_Beta | Halo3_Xbox | // TODO: If Halo3 ever gets a PC version, uncomment and use kHalo3
			/*kHaloOdst*/HaloOdst_Xbox | // TODO: If ODST ever gets a PC version, uncomment and use kHaloOdst
			/*kHaloReach*/HaloReach_Beta | HaloReach_Xbox | // TODO: If Reach ever gets a PC version, uncomment and use kHaloReach
			/*kHalo4*/Halo4_Xbox, // TODO: If Halo4 ever gets a PC version, uncomment and use kHalo3
	};

	/// <summary>Exception thrown when the library encounters a mismatch in expected engine versions</summary>
	/// <remarks>
	/// Example: If we try loading a file meant for Halo 2, but we're doing it under Halo 1, this should be thrown
	/// </remarks>
	public class InvalidBlamVersionException : Debug.ExceptionLog
	{
		/// <summary>Invalid engine exception details</summary>
		/// <param name="expected">The engine build we were expecting</param>
		/// <param name="got">The engine build we actually got</param>
		/// <remarks>Meant for general engine conflictions</remarks>
		public InvalidBlamVersionException(BlamBuild expected, BlamBuild got) : base(null, "Invalid engine build")
		{
			Debug.LogFile.WriteLine(
				"Invalid engine build: expected '{0}' but got '{1}'",
				expected, got);
		}
		/// <summary>Invalid engine exception details</summary>
		/// <param name="expected">The engine platform we were expecting</param>
		/// <param name="got">The engine platform we actually got</param>
		public InvalidBlamVersionException(BlamPlatform expected, BlamPlatform got) : base(null, "Invalid engine platform")
		{
			Debug.LogFile.WriteLine(
				"Invalid platform: expected '{0}' but got '{1}'",
				expected, got);
		}
		/// <summary>Invalid engine exception details</summary>
		/// <param name="expected">The engine we were expecting</param>
		/// <param name="got">The engine we actually got</param>
		/// <remarks>Meant for specific engine conflicts (eg, <see cref="BlamVersion.Halo1_Xbox"/> <see cref="BlamVersion.Halo1_PC"/>)</remarks>
		public InvalidBlamVersionException(BlamVersion expected, BlamVersion got) : base(null, "Invalid engine")
		{
			Debug.LogFile.WriteLine(
				"Invalid engine: expected '{0}' but got '{1}'",
				expected, got);
		}
	};
}