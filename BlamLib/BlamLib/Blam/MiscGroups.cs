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

		#region Halo3
		/// <summary>
		/// 'Blam File'
		/// </summary>
		public static readonly TagGroup _blf = new TagGroup("_blf", "blam_file");

		/// <summary>
		/// End of file
		/// </summary>
		/// <remarks>
		/// tag signature
		/// long size_of
		/// short
		/// short
		/// long size_of_file
		/// byte[] align_to_16b
		/// </remarks>
		public static readonly TagGroup _eof = new TagGroup("_eof", "end_of_file");

		/// <summary>
		/// Content Header
		/// </summary>
		public static readonly TagGroup chdr = new TagGroup("chdr", "content_header");

		/// <summary>
		/// Content Author (Engine related, not user)
		/// </summary>
		public static readonly TagGroup athr = new TagGroup("athr", "content_author");

		/// <summary>
		/// File Header
		/// </summary>
		public static readonly TagGroup flmh = new TagGroup("flmh", "film_header");

		/// <summary>
		/// File Data
		/// </summary>
		public static readonly TagGroup flmd = new TagGroup("flmd", "film_data");

		/// <summary>
		/// s_blffile_game_variant
		/// </summary>
		public static readonly TagGroup mpvr = new TagGroup("mpvr", "s_blffile_game_variant");

		/// <summary>
		/// s_blffile_map_variant
		/// </summary>
		public static readonly TagGroup mapv = new TagGroup("mapv", "s_blffile_map_variant");

		/// <summary>
		/// 
		/// </summary>
		public static readonly TagGroup levl = new TagGroup("levl", "level_info");

		/// <summary>
		/// 
		/// </summary>
		public static readonly TagGroup cmpn = new TagGroup("cmpn", "campaign_info");

		/// <summary>
		/// 
		/// </summary>
		public static readonly TagGroup mapi = new TagGroup("mapi", "map_image");

		/// <summary>
		/// 
		/// </summary>
		public static readonly TagGroup gvar = new TagGroup("gvar", ""); // game variant

		/// <summary>
		/// 
		/// </summary>
		public static readonly TagGroup mvar = new TagGroup("mvar", ""); // map variant

		/// <summary>
		/// 
		/// </summary>
		public static readonly TagGroup onfm = new TagGroup("onfm", "");

		/// <summary>
		/// 
		/// </summary>
		public static readonly TagGroup srid = new TagGroup("srid", "");

		/// <summary>
		/// screen-shot internal data
		/// </summary>
		public static readonly TagGroup scnd = new TagGroup("scnd", "");

		/// <summary>
		/// 
		/// </summary>
		public static readonly TagGroup netc = new TagGroup("netc", "");

		/// <summary>
		/// 
		/// </summary>
		public static readonly TagGroup mapm = new TagGroup("mapm", "");

		/// <summary>
		/// (file upload) ban-hammer data
		/// </summary>
		public static readonly TagGroup fubh = new TagGroup("fubh", "");

		/// <summary>
		/// (file upload) network statistics
		/// </summary>
		public static readonly TagGroup funs = new TagGroup("funs", "");

		/// <summary>
		/// (file upload) hopper directory
		/// </summary>
		public static readonly TagGroup fupd = new TagGroup("fupd", "");

		/// <summary>
		/// file queue
		/// </summary>
		/// <remarks>for user auto downloads</remarks>
		public static readonly TagGroup filq = new TagGroup("filq", "");

		/// <summary>
		/// (file upload) repeated play
		/// </summary>
		public static readonly TagGroup furp = new TagGroup("furp", "");

		/// <summary>
		/// ban-hammer message
		/// </summary>
		public static readonly TagGroup bhms = new TagGroup("bhms", "");

		/// <summary>
		/// matchmaking hopper statistics
		/// </summary>
		public static readonly TagGroup mmhs = new TagGroup("mmhs", "");

		/// <summary>
		/// message of the day
		/// </summary>
		public static readonly TagGroup motd = new TagGroup("motd", "");

		/// <summary>
		/// compressed data
		/// </summary>
		public static readonly TagGroup _cmp = new TagGroup("_cmp", "");

		/// <summary>
		/// matchmaking tips
		/// </summary>
		public static readonly TagGroup mmtp = new TagGroup("mmtp", "");

		/// <summary>
		/// matchmaking hopper config file?
		/// </summary>
		public static readonly TagGroup mhcf = new TagGroup("mhcf", "");

		/// <summary>
		/// matchmaking hopper descriptions file?
		/// </summary>
		public static readonly TagGroup mhdf = new TagGroup("mhdf", "");

		/// <summary>
		/// 
		/// </summary>
		public static readonly TagGroup gset = new TagGroup("gset", "");

		/// <summary>
		/// 
		/// </summary>
		public static readonly TagGroup mdsc = new TagGroup("mdsc", ""); // something to do with a minidump?


		/// <summary>
		/// begin network web event
		/// </summary>
		public static readonly TagGroup bnwe = new TagGroup("bnwe", "");

		/// <summary>
		/// end network web event
		/// </summary>
		public static readonly TagGroup enwe = new TagGroup("enwe", "");

		/// <summary>
		/// local cheater
		/// </summary>
		public static readonly TagGroup lche = new TagGroup("lche", "");

		/// <summary>
		/// remote cheater
		/// </summary>
		public static readonly TagGroup rche = new TagGroup("rche", "");


		/// <summary>
		/// 
		/// </summary>
		/// <remarks>Used in both campaign and multiplayer data uploads</remarks>
		public static readonly TagGroup glcp = new TagGroup("glcp", "");

		/// <summary>
		/// 
		/// </summary>
		/// <remarks>Used in both campaign and multiplayer data uploads
		/// h3:   0x11
		/// odst: 0x18
		/// </remarks>
		public static readonly TagGroup clif = new TagGroup("clif", "");


		/// <summary>
		/// multiplayer (game data?)
		/// </summary>
		public static readonly TagGroup mpgd = new TagGroup("mpgd", "");

		/// <summary>
		/// multiplayer (match options?)
		/// </summary>
		public static readonly TagGroup mpmo = new TagGroup("mpmo", "");

		/// <summary>
		/// multiplayer (play list?)
		/// </summary>
		public static readonly TagGroup mppl = new TagGroup("mppl", "");

		/// <summary>
		/// multiplayer (?)
		/// </summary>
		public static readonly TagGroup mptm = new TagGroup("mptm", "");

		/// <summary>
		/// multiplayer (?)
		/// </summary>
		public static readonly TagGroup mpma = new TagGroup("mpma", "");

		/// <summary>
		/// multiplayer (stats?)
		/// </summary>
		public static readonly TagGroup mps1 = new TagGroup("mps1", "");

		/// <summary>
		/// multiplayer (stats?)
		/// </summary>
		public static readonly TagGroup mps2 = new TagGroup("mps2", "");

		/// <summary>
		/// multiplayer (stats?)
		/// </summary>
		public static readonly TagGroup mps3 = new TagGroup("mps3", "");

		/// <summary>
		/// 
		/// </summary>
		public static readonly TagGroup _par = new TagGroup("_par", "");

		/// <summary>
		/// multiplayer (?)
		/// </summary>
		public static readonly TagGroup mpev = new TagGroup("mpev", "");


		/// <summary>
		/// game options
		/// </summary>
		/// <remarks>
		/// h3:   0xF820
		/// odst: 0xEFD8
		/// </remarks>
		public static readonly TagGroup gmop = new TagGroup("gmop", "");

		/// <summary>
		/// campaign (?)
		/// </summary>
		/// <remarks>
		/// h3:   0x2448
		/// odst: 0x19D58 // "campaign meta-game globals" game state = 0x19D48
		/// </remarks>
		public static readonly TagGroup cmrp = new TagGroup("cmrp", "");

		/// <summary>
		/// campaign (?)
		/// </summary>
		/// <remarks>
		/// h3:   0x78
		/// odst: 0x2128
		/// </remarks>
		public static readonly TagGroup cmrs = new TagGroup("cmrs", "");

		/// <summary>
		/// campaign (survival?)
		/// </summary>
		/// <remarks>
		/// odst: 0x398 // "survival mode globals" game state = 0x3B0
		/// </remarks>
		public static readonly TagGroup cmsu = new TagGroup("cmsu", "");

		/// <summary>
		/// campaign (?)
		/// </summary>
		/// <remarks>
		/// odst: 0x120
		/// </remarks>
		public static readonly TagGroup chrt = new TagGroup("chrt", "");

		/// <summary>
		/// campaign (?)
		/// </summary>
		/// <remarks>
		/// odst: 0x11
		/// </remarks>
		public static readonly TagGroup cmmm = new TagGroup("cmmm", "");


		/// <summary>
		/// match quality data
		/// </summary>
		public static readonly TagGroup mqdt = new TagGroup("mqdt", ""); // atlas match qual
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
			pool,
			#endregion

			#region Halo3
			_blf,
			_eof,
			chdr,
			athr,
			flmh,
			flmd,
			mpvr,
			mapv,
			mapi,
			levl,
			cmpn,
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
			mdsc
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

			#region Halo3
			/// <summary></summary>
			_blf,
			/// <summary></summary>
			_eof,
			/// <summary></summary>
			chdr,
			/// <summary></summary>
			athr,
			/// <summary></summary>
			flmh,
			/// <summary></summary>
			flmd,
			/// <summary></summary>
			mpvr,
			/// <summary></summary>
			mapv,
			/// <summary></summary>
			mapi,
			/// <summary></summary>
			levl,
			/// <summary></summary>
			cmpn,
			/// <summary></summary>
			gvar,
			/// <summary></summary>
			mvar,
			/// <summary></summary>
			onfm,
			/// <summary></summary>
			srid,
			/// <summary></summary>
			scnd,
			/// <summary></summary>
			netc,
			/// <summary></summary>
			mapm,
			/// <summary></summary>
			fubh,
			/// <summary></summary>
			funs,
			/// <summary></summary>
			fupd,
			/// <summary></summary>
			filq,
			/// <summary></summary>
			furp,
			/// <summary></summary>
			bhms,
			/// <summary></summary>
			mmhs,
			/// <summary></summary>
			motd,
			/// <summary></summary>
			_cmp,
			/// <summary></summary>
			mmtp,
			/// <summary></summary>
			mhcf,
			/// <summary></summary>
			mhdf,
			/// <summary></summary>
			gset,
			/// <summary></summary>
			mdsc,
			#endregion
		};
		#endregion

		#region Static Ctor
		static MiscGroups()
		{
			for (int x = 0; x < Groups.Count; x++)
				Groups[x].InitializeHandle(BlamVersion.Unknown, x, false);

#if !NO_HALO3
			Halo3.BlamFile.InitializeBlfGroups();
#endif
		}
		#endregion
	};
}