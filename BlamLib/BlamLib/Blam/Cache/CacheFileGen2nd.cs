/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System.Collections.Generic;

namespace BlamLib.Blam.Cache
{
	/// <summary>
	/// Cache Header interface for 2nd generation engines (namely for Xbox 1)
	/// </summary>
	public abstract class CacheHeaderGen2 : Blam.CacheHeader, ICacheHeaderStringId
	{
		#region SourceFile
		protected string sourceFile = "";
		/// <summary>
		/// Cache file path from the DVD
		/// </summary>
		public string SourceFile { get { return sourceFile; } }
		#endregion

		#region ICacheHeaderStringId
		protected int stringIdsCount;
		/// <summary>
		/// Total count of string ids
		/// </summary>
		public int StringIdsCount
		{
			get { return stringIdsCount; }
			set { stringIdsCount = value; }
		}

		protected int stringIdsBufferSize;
		/// <summary>
		/// Total size of the null terminated string id table
		/// </summary>
		public int StringIdsBufferSize
		{
			get { return stringIdsBufferSize; }
			set { stringIdsBufferSize = value; }
		}

		protected int stringIdIndicesOffset;
		/// <summary>
		/// Offsets of each string id in the null terminated string id table
		/// </summary>
		public int StringIdIndicesOffset
		{
			get { return stringIdIndicesOffset; }
			set { stringIdIndicesOffset = value; }
		}

		protected int stringIdsBufferOffset;
		/// <summary>
		/// Offset of the null terminated string id table
		/// </summary>
		public int StringIdsBufferOffset
		{
			get { return stringIdsBufferOffset; }
			set { stringIdsBufferOffset = value; }
		}
		#endregion

		#region ScenarioPath
		protected string scenarioPath;
		/// <summary>
		/// Tag path of the scenario
		/// </summary>
		public string ScenarioPath { get { return scenarioPath; } }
		#endregion

		#region NeedsShared
		protected bool needsShared;
		/// <summary>
		/// Cache requires shared cache files
		/// </summary>
		public bool NeedsShared	{ get { return needsShared; } }
		#endregion

		#region Tag Names
		protected int tagNamesCount;
		/// <summary>
		/// Number of tag names in the cache
		/// </summary>
		public int TagNamesCount
		{
			get { return tagNamesCount; }
			set { tagNamesCount = value; }
		}

		protected int tagNamesBufferOffset;
		/// <summary>
		/// Offset to the c-string buffer containing all the tag names
		/// </summary>
		public int TagNamesBufferOffset
		{
			get { return tagNamesBufferOffset; }
			set { tagNamesBufferOffset = value; }
		}

		protected int tagNamesBufferSize;
		/// <summary>
		/// Size of the c-string buffer which contains all the tag names
		/// </summary>
		public int TagNamesBufferSize
		{
			get { return tagNamesBufferSize; }
			set { tagNamesBufferSize = value; }
		}

		protected int tagNameIndicesOffset;
		/// <summary>
		/// Offset to a table containing the character index (relative to <b>TagNamesBuffer</b>) 
		/// marking the start of each tag name
		/// </summary>
		public int TagNameIndicesOffset
		{
			get { return tagNameIndicesOffset; }
			set { tagNameIndicesOffset = value; }
		}
		#endregion


		#region Checksum
		protected uint checksum;
		/// <summary>
		/// Cache's checksum (using a 32bit Xor algorithm, starting at after the cache header)
		/// </summary>
		public uint Checksum
		{
			get { return checksum; }
			set { checksum = value; }
		}
		#endregion
	};

	/// <summary>
	/// Cache Index interface for 2nd generation engines (namely for Xbox 1)
	/// </summary>
	public abstract class CacheIndexGen2 : Blam.CacheIndex
	{
		protected uint groupTagsOffset;
		protected uint groupTagsAddress;
		public uint GroupTagsAddress	{ get { return groupTagsAddress; } }

		protected int groupTagsCount;
		public int GroupTagsCount	{ get { return groupTagsCount; } }
	};


	public abstract class CacheItemGroupTagGen2 : IO.IStreamable
	{
		public TagInterface.TagGroup GroupTag1;
		public TagInterface.TagGroup GroupTag2;
		public TagInterface.TagGroup GroupTag3;

		public override string ToString()
		{
			return string.Format("{0}\t{1}\t{2}", 
				GroupTag1.TagToString(), 
				GroupTag2.TagToString(), 
				GroupTag3.TagToString()); }

		#region IStreamable Members
		protected void ReadGroupTags(Managers.BlamDefinition gd, IO.EndianReader s)
		{
			uint gt = s.ReadUInt32();
			if (gt != uint.MaxValue)	GroupTag1 = gd.TagGroupFind(TagInterface.TagGroup.FromUInt(gt));
			else						GroupTag1 = TagInterface.TagGroup.Null;

			gt = s.ReadUInt32();
			if (gt != uint.MaxValue)	GroupTag2 = gd.TagGroupFind(TagInterface.TagGroup.FromUInt(gt));
			else						GroupTag2 = TagInterface.TagGroup.Null;

			gt = s.ReadUInt32();
			if (gt != uint.MaxValue)	GroupTag3 = gd.TagGroupFind(TagInterface.TagGroup.FromUInt(gt));
			else						GroupTag3 = TagInterface.TagGroup.Null;
		}

		public abstract void Read(BlamLib.IO.EndianReader s);

		public virtual void Write(BlamLib.IO.EndianWriter s)
		{
			GroupTag1.Write(s);
			GroupTag2.Write(s);
			GroupTag3.Write(s);
		}
		#endregion
	};


	public abstract class CacheFileGen2 : Blam.CacheFile
	{
	};
};