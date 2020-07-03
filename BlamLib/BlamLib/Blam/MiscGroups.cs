/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using System.Collections.Generic;
using System.Text;
using BlamLib.TagInterface;

namespace BlamLib.Blam
{
	public static class MiscGroups
	{
		#region Functions
		/// <summary>
		/// Get a tag group definition from a game name and four character tag
		/// </summary>
		/// <param name="game"></param>
		/// <param name="group_tag"></param>
		/// <returns></returns>
		/// <remarks>
		/// Does not handle Struct group tags.
		/// 
		/// If <paramref name="game"/> equals <see cref="BlamVersion.Unknown"/>, <see cref="MiscGroups"/> is used for finding the TagGroup
		/// </remarks>
		/// <exception cref="Debug.Exceptions.UnreachableException">When <paramref name="game"/> is an unhandled game</exception>
		public static TagGroup TagGroupFrom(BlamVersion game, uint group_tag)
		{
			if (group_tag == uint.MaxValue) return TagGroup.Null;

			if ((game & BlamVersion.Halo1) != 0)			return Halo1.TagGroups.Groups.FindTagGroup(group_tag);
#if !NO_HALO2
			else if ((game & BlamVersion.Halo2) != 0)		return Halo2.TagGroups.Groups.FindTagGroup(group_tag);
#endif
#if !NO_HALO3
			else if ((game & BlamVersion.Halo3) != 0)		return Halo3.TagGroups.Groups.FindTagGroup(group_tag);
#endif
#if !NO_HALO_ODST
			else if ((game & BlamVersion.HaloOdst) != 0)	return HaloOdst.TagGroups.Groups.FindTagGroup(group_tag);
#endif
#if !NO_HALO_REACH
			else if ((game & BlamVersion.HaloReach) != 0)	return HaloReach.TagGroups.Groups.FindTagGroup(group_tag);
#endif
#if !NO_HALO4
			else if ((game & BlamVersion.Halo4) != 0)	return Halo4.TagGroups.Groups.FindTagGroup(group_tag);
#endif
			else if ((game & BlamVersion.Stubbs) != 0)		return Stubbs.TagGroups.Groups.FindTagGroup(group_tag);
			else if (game == BlamVersion.Unknown)			return MiscGroups.Groups.FindTagGroup(group_tag);

			throw new Debug.Exceptions.UnreachableException(game);
		}

		/// <summary>
		/// Get a <see cref="TagGroup"/> via a handle stored in a <see cref="DatumIndex"/> object which is a strong reference 
		/// and can be trusted to work even when streamed to a file and loaded at another time after the application 
		/// has reset
		/// </summary>
		/// <param name="group_handle">Strong reference to a <see cref="TagGroup"/></param>
		/// <returns></returns>
		public static TagGroup TagGroupFromHandle(DatumIndex group_handle)
		{
			if (group_handle == DatumIndex.Null) return TagGroup.Null;

			BlamVersion engine = (BlamVersion)group_handle.Salt;
			ushort index = group_handle.Index;
			bool is_struct = Util.Flags.Test(index, (ushort)0x8000);

			switch(engine)
			{
				case BlamVersion.Halo1:
					return Halo1.TagGroups.Groups[index];

#if !NO_HALO2
				case BlamVersion.Halo2:
					if (is_struct) return Halo2.StructGroups.Groups[index];
					return Halo2.TagGroups.Groups[index];
#endif

#if !NO_HALO3
				case BlamVersion.Halo3:
					if (is_struct) return Halo3.StructGroups.Groups[index];
					return Halo3.TagGroups.Groups[index];
#endif

#if !NO_HALO_ODST
				case BlamVersion.HaloOdst:
					// TODO: ummm, add the code for struct groups
					//if (is_struct) return HaloOdst.StructGroups.Groups[index];
					return HaloOdst.TagGroups.Groups[index];
#endif

#if !NO_HALO_REACH
				case BlamVersion.HaloReach:
					if (is_struct) return HaloReach.StructGroups.Groups[index];
					return HaloReach.TagGroups.Groups[index];
#endif

#if !NO_HALO4
				case BlamVersion.Halo4:
					if (is_struct) return Halo4.StructGroups.Groups[index];
					return Halo4.TagGroups.Groups[index];
#endif

				case BlamVersion.Stubbs:
					return Stubbs.TagGroups.Groups[index];


				case BlamVersion.Unknown:
					return MiscGroups.Groups[index];

				default:
					throw new Debug.Exceptions.UnreachableException(engine);
			}
		}

		/// <summary>
		/// Determines the <see cref="BlamVersion"/> from the provided four character code
		/// </summary>
		/// <param name="group_tag">game four character code</param>
		/// <returns></returns>
		/// <exception cref="Debug.Exceptions.UnreachableException"><paramref name="group_tag"/> doesn't match any expected groups</exception>
		public static BlamVersion VersionFromTag(char[] group_tag)
		{
			if		(TagGroup.Test(group_tag, (char[])blam))	return BlamVersion.Halo1;
			else if (TagGroup.Test(group_tag, (char[])blam2a))	return BlamVersion.Halo2;
			else if (TagGroup.Test(group_tag, (char[])blam2b))	return BlamVersion.Halo2;
			else if (TagGroup.Test(group_tag, (char[])blam2c))	return BlamVersion.Halo2;
			else if (TagGroup.Test(group_tag, (char[])blam2d))	return BlamVersion.Halo2;
			else if (TagGroup.Test(group_tag, (char[])blm2))	return BlamVersion.Halo2;
			else if (TagGroup.Test(group_tag, (char[])blm3))	return BlamVersion.Halo3;
			else if (TagGroup.Test(group_tag, (char[])stub))	return BlamVersion.Stubbs;
			else	throw new Debug.Exceptions.UnreachableException();
		}

		/// <summary>
		/// Determines the four character code to use for the provided engine version
		/// </summary>
		/// <param name="v">Blam engine version</param>
		/// <param name="internal_use">if true then it will use the internal group tags only</param>
		/// <returns></returns>
		/// <exception cref="Debug.Exceptions.UnreachableException"><paramref name="v"/> doesn't match any expected base game versions</exception>
		public static char[] VersionToTag(BlamVersion v, bool internal_use)
		{
			if		((v & BlamVersion.Halo1) != 0)					return blam.Tag;
			else if (internal_use && (v & BlamVersion.Halo2) != 0)	return blm2.Tag;
			else if (!internal_use && (v & BlamVersion.Halo2) != 0)	return blam2d.Tag;
			else if ((v & BlamVersion.Halo3) != 0)					return blm3.Tag;
			else if ((v & BlamVersion.Stubbs) != 0)					return stub.Tag;
			else	throw new Debug.Exceptions.UnreachableException(v);
		}

		/// <summary>
		/// Checks whether the tag supplied is a signature for the old
		/// tag format which used a different string id storage, useless
		/// padding, and in some cases was big endian.
		/// </summary>
		/// <param name="tag"></param>
		/// <returns></returns>
		internal static bool Halo2TagSignatureIsOldFormat(uint tag)
		{
			if		(tag == blam2a.ID)	return true;
			else if (tag == blam2b.ID)	return true;
			else if (tag == blam2c.ID)	return true;

			return false;
		}

		/// <summary>
		/// Checks whether the tag supplied is a signature for the old
		/// tag format which used a different fieldset header
		/// </summary>
		/// <param name="tag"></param>
		/// <returns></returns>
		internal static bool Halo2TagSignatureIsOldFieldsetHeaderFormat(uint tag)
		{
			if		(tag == blam2a.ID)	return true;

			return false;
		}

		/// <summary>
		/// Checks whether the tag supplied is a signature for the old
		/// tag format using the old string id storage
		/// </summary>
		/// <param name="tag"></param>
		/// <returns></returns>
		internal static bool Halo2TagSignatureIsOldStringId(uint tag)
		{
			if		(tag == blam2a.ID)	return true;
			else if	(tag == blam2b.ID)	return true;

			return false;
		}

		/// <summary>
		/// Checks whether the tag supplied is a signature for the old
		/// tag format using the useless padding fields
		/// </summary>
		/// <param name="tag"></param>
		/// <returns></returns>
		internal static bool Halo2TagSignatureUsesUselessPadding(uint tag)
		{
			if		(tag == blam2a.ID)	return true;
			else if	(tag == blam2b.ID)	return true;
			else if	(tag == blam2c.ID)	return true;

			return false;
		}
		#endregion

		#region Halo
		/// <summary>
		/// Blam Engine
		/// </summary>
		public static readonly TagGroup blam = new TagGroup("blam", "blam");

		#region Halo2
		/// <summary>
		/// Blam Engine, reversed
		/// </summary>
		public static readonly TagGroup malb = new TagGroup("malb", "blam");

		/// <summary>
		/// Blam Engine Halo 2
		/// </summary>
		/// <remarks>
		/// uses old format
		/// * old fieldset header
		/// * old string id
		/// * useless padding
		/// </remarks>
		public static readonly TagGroup blam2a = new TagGroup("ambl", "blam");

		/// <summary>
		/// Blam Engine Halo 2
		/// </summary>
		/// <remarks>
		/// uses old format
		/// * old string id
		/// * useless padding
		/// </remarks>
		public static readonly TagGroup blam2b = new TagGroup("LAMB", "blam");

		/// <summary>
		/// Blam Engine Halo 2
		/// </summary>
		/// <remarks>
		/// uses old format
		/// * useless padding
		/// </remarks>
		public static readonly TagGroup blam2c = new TagGroup("MLAB", "blam");

		/// <summary>
		/// Blam Engine Halo 2 (latest)
		/// </summary>
		public static readonly TagGroup blam2d = new TagGroup("BLM!", "blam");
		#endregion

		/// <summary>
		/// Blam Engine Version 2 (Halo 2)
		/// </summary>
		public static readonly TagGroup blm2 = new TagGroup("blm2", "blam2");

		/// <summary>
		/// Blam Engine Version 3 (Halo 3)
		/// </summary>
		public static readonly TagGroup blm3 = new TagGroup("blm3", "blam3");
		#endregion

		/// <summary>
		/// Blam Engine (Stubbs The Zombie)
		/// </summary>
		public static readonly TagGroup stub = new TagGroup("stub", "stubbs");

		#region Signatures
		/// <summary>
		/// Halo2 Resource Header
		/// </summary>
		public static readonly TagGroup blkh = new TagGroup("blkh", "resource_header");

		/// <summary>
		/// Halo2 Resource Data
		/// </summary>
		public static readonly TagGroup rsrc = new TagGroup("rsrc", "resource_data");

		/// <summary>
		/// Halo2 Resource Footer
		/// </summary>
		public static readonly TagGroup blkf = new TagGroup("blkf", "resource_footer");

		/// <summary>
		/// Block Header
		/// </summary>
		public static readonly TagGroup head = new TagGroup("head", "block_header");

		/// <summary>
		/// Block Trailer
		/// </summary>
		public static readonly TagGroup tail = new TagGroup("tail", "block_trailer");

		/// <summary>
		/// Footer
		/// </summary>
		public static readonly TagGroup foot = new TagGroup("foot", "footer");

		/// <summary>
		/// Tag Block Field
		/// </summary>
		public static readonly TagGroup tbfd = new TagGroup("tbfd", "tag_block_field_set");

		/// <summary>
		/// Tag Struct Field
		/// </summary>
		public static readonly TagGroup tsfd = new TagGroup("tsfd", "tag_struct_field_set");

		/// <summary>
		/// Byte Swap definition
		/// </summary>
		public static readonly TagGroup bysw = new TagGroup("bysw", "byte_swap");

		/// <summary>
		/// Cache resource database header
		/// </summary>
		public static readonly TagGroup crdb = new TagGroup("crdb", "cache_resource_database");
		#endregion

		#region Game State
		/// <summary>
		/// Data Array
		/// </summary>
		public static readonly TagGroup data = new TagGroup("d@t@", "data_array");

		/// <summary>
		/// Memory Pool
		/// </summary>
		public static readonly TagGroup pool = new TagGroup("pool", "memory_pool");
		#endregion

		#region Tags Group Collection
		/// <summary>
		/// All tag groups in MiscGroups
		/// </summary>
		public static TagGroupCollection Groups = new TagGroupCollection(false,
			blam,
			malb,

			blam2a,
			blam2b,
			blam2c,
			blam2d,

			blm2,
			blm3,

			stub,

			#region Signatures
			blkh,
			rsrc,
			blkf,
			head,
			tail,
			foot,
			tbfd,
			tsfd,
			bysw,
			crdb,
			#endregion

			#region GameState
			data,
			pool
			#endregion
			);

		/// <summary>
		/// Constant indices to each tag group in <see cref="Groups"/>
		/// </summary>
		public enum Enumerated : int
		{
			/// <summary>Blam Engine</summary>
			blam,
			/// <summary>Blam Engine, reversed</summary>
			malb,

			/// <summary>Blam Engine Halo 2</summary>
			/// <remarks>uses old format</remarks>
			blam2a,
			/// <summary>Blam Engine Halo 2</summary>
			/// <remarks>uses old format</remarks>
			blam2b,
			/// <summary>Blam Engine Halo 2</summary>
			/// <remarks>uses old format</remarks>
			blam2c,
			/// <summary>Blam Engine Halo 2 (latest)</summary>
			blam2d,

			/// <summary>Blam Engine Version 2 (Halo 2)</summary>
			blm2,
			/// <summary>Blam Engine Version 3 (Halo 3)</summary>
			blm3,

			/// <summary>Blam Engine (Stubbs The Zombie)</summary>
			stub,

			#region Signatures
			/// <summary>Halo2 Resource Header</summary>
			blkh,
			/// <summary>Halo2 Resource Data</summary>
			rsrc,
			/// <summary>Halo2 Resource Footer</summary>
			blkf,
			/// <summary>Block Header</summary>
			head,
			/// <summary>Block Trailer</summary>
			tail,
			/// <summary>Footer</summary>
			foot,
			/// <summary>Tag Block Field</summary>
			tbfd,
			/// <summary>Tag Struct Field</summary>
			tsfd,
			/// <summary>Byte Swap definition</summary>
			bysw,
			/// <summary>Cache resource database header</summary>
			crdb,
			#endregion

			#region GameState
			/// <summary>Data Array</summary>
			data,
			/// <summary>Memory Pool</summary>
			pool,
			#endregion
		};
		#endregion

		#region Static Ctor
		static MiscGroups()
		{
			for (int x = 0; x < Groups.Count; x++)
				Groups[x].InitializeHandle(BlamVersion.Unknown, x, false);
		}
		#endregion
	};
}