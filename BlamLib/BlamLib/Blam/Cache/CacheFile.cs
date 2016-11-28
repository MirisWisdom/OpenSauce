/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib.Blam
{
	/// <summary>
	/// Types of cache files the engine uses
	/// </summary>
	public enum CacheType : int
	{
		/// <summary>
		/// Internal value, not a value the actual engine uses
		/// </summary>
		None = -1,
		/// <summary>
		/// Single player \ story-related scenario
		/// </summary>
		Campaign,
		/// <summary>
		/// Scenario that makes use of a engine variant, like CTF
		/// </summary>
		Multiplayer,
		/// <summary>
		/// Scenario that interfaces the user with settings and selecting a map to load to play.
		/// Starting in Halo2 it also holds shared UI resources
		/// </summary>
		MainMenu,
		/// <summary>
		/// Cache file that can be played (or stripped of run-time data, a la Halo 3+) and holds shared 
		/// resources used abroad
		/// </summary>
		Shared,
		/// <summary>
		/// Cache file that can be played (or stripped of run-time data, a la Halo 3+) and holds shared 
		/// resources used in campaign cache files
		/// </summary>
		SharedCampaign,

		Unknown5,
		Unknown6,
	};

	/// <summary>
	/// Functions implemented by a class that can be streamed to and from a cache file
	/// </summary>
	/// <see cref="BlamLib.TagInterface"/>
	public interface ICacheStreamable
	{
		/// <summary>
		/// Read the main body of the object
		/// </summary>
		/// <param name="c">cache to stream from</param>
		void Read(Blam.CacheFile c);
		/// <summary>
		/// Read the pointer data for the object
		/// </summary>
		/// <param name="c">cache to stream from</param>
		void ReadHeader(Blam.CacheFile c);

		/// <summary>
		/// Write the main body of this object
		/// </summary>
		/// <param name="c">cache to stream to</param>
		void Write(Blam.CacheFile c);
		/// <summary>
		/// Write the pointer data of this object
		/// </summary>
		/// <param name="c">cache to stream to</param>
		void WriteHeader(Blam.CacheFile c);
	};

	#region Header
	/// <summary>
	/// Base class for all Blam map header definitions
	/// </summary>
	public abstract class CacheHeader : IO.IStreamable
	{
		#region Version
		protected int version;
		/// <summary>
		/// This cache's version
		/// </summary>
		[System.ComponentModel.Category("Cache Header")]
		public int Version	{ get { return version; } }
		#endregion

		#region FileLength
		protected int fileLength;
		/// <summary>
		/// How long (in bytes) the cache file is (decompressed)
		/// </summary>
		[System.ComponentModel.Browsable(false)]
		public int FileLength	{ get { return fileLength; } }
		#endregion

		#region OffsetToIndex
		protected int offsetToIndex;
		/// <summary>
		/// Offset in the map to the map's tag index
		/// </summary>
		[System.ComponentModel.Category("Cache Header")]
		public int OffsetToIndex	{ get { return offsetToIndex; } }
		#endregion

		#region TagBufferSize
		protected int tagBufferSize;
		/// <summary>
		/// The total size of the tag data
		/// </summary>
		[System.ComponentModel.Category("Cache Header")]
		public int TagBufferSize	{ get { return tagBufferSize; } }
		#endregion

		#region Name
		protected string name;
		/// <summary>
		/// Name of this map
		/// </summary>
		[System.ComponentModel.Category("Cache Header")]
		public string Name	{ get { return name; } }
		#endregion

		#region Build
		protected string build;
		/// <summary>
		/// The build of the engine this cache
		/// file was created with
		/// </summary>
		/// <remarks>
		/// Halo1: ##.##.##.####
		/// Halo2: ##.##.##.#####
		/// Halo3: #####.YY.MM.DD.HHMM.[type]
		/// </remarks>
		[System.ComponentModel.Category("Cache Header")]
		public string Build	{ get { return build; } }
		#endregion

		#region CacheType
		protected CacheType cacheType;
		/// <summary>
		/// The map type of this map
		/// </summary>
		[System.ComponentModel.Category("Cache Header")]
		public CacheType CacheType	{ get { return cacheType; } }
		#endregion


		#region IStreamable Members
		/// <summary>
		/// Stream the header data from a buffer
		/// </summary>
		/// <param name="s"></param>
		public abstract void Read(BlamLib.IO.EndianReader s);
		/// <summary>
		/// Throws <see cref="NotImplementedException"/>
		/// </summary>
		/// <param name="s"></param>
		public virtual void Write(BlamLib.IO.EndianWriter s) { throw new NotImplementedException(); }
		#endregion
	};
	#endregion

	#region Index
	/// <summary>
	/// Base class for all Blam map tag header definitions
	/// </summary>
	public abstract class CacheIndex : IO.IStreamable, IEnumerable<CacheIndex.Item>
	{
		#region Item
		/// <summary>
		/// Specifies if an item itself or resource data
		/// is stored in the cache file or in a external (cache) file
		/// </summary>
		public enum ItemLocation : int
		{
			/// <summary>
			/// Internal to the map
			/// </summary>
			Internal,
			/// <summary>
			/// External, in another map or resource related file
			/// </summary>
			External,
			/// <summary>
			/// I have no fucking idea, go away
			/// </summary>
			Unknown,
		};

		/// <summary>
		/// Base class for all Blam map tag index item definitions
		/// </summary>
		public abstract class Item : IO.IStreamable
		{
			#region Datum
			protected DatumIndex datum;
			/// <summary>
			/// The tag's datum
			/// </summary>
			[System.ComponentModel.Category("Tag Instance")]
			[System.ComponentModel.ReadOnly(true)]
			public DatumIndex Datum
			{
				get { return datum; }
				internal set { datum = value; }
			}

			/// <summary>
			/// Returns the index of this item in the map's tag index
			/// </summary>
			[System.ComponentModel.Browsable(false)]
			public int Index { get { return datum.Index; } }
			#endregion

			/// <summary>
			/// Returns true if this item is a empty tag datum
			/// </summary>
			[System.ComponentModel.Browsable(false)]
			public bool IsEmpty { get { return datum.Handle == uint.MaxValue; } }
			/// <summary>
			/// Returns true if this item points to a null address
			/// </summary>
			[System.ComponentModel.Browsable(false)]
			public bool IsNull { get { return Address == 0; } }

			#region GroupTag
			public uint GroupTagInt;

			protected TagInterface.TagGroup groupTag;
			/// <summary>
			/// The four character code that designates
			/// the tag group this item holds in its data
			/// </summary>
			[System.ComponentModel.Category("Tag Instance")]
			[System.ComponentModel.ReadOnly(true)]
			public TagInterface.TagGroup GroupTag
			{
				get { return groupTag; }
				internal set { groupTag = value; }
			}
			#endregion

			#region TagNameOffset
			protected uint tagNameOffset;
			/// <summary>
			/// The offset to the tag name in the cache file's stream
			/// </summary>
			[System.ComponentModel.Category("Tag Instance")]
			[System.ComponentModel.ReadOnly(true)]
			public uint TagNameOffset
			{
				get { return tagNameOffset; }
				internal set { tagNameOffset = value; }
			}
			#endregion

			#region ReferenceName
			protected Blam.DatumIndex referenceName;
			/// <summary>
			/// Handle for the <see cref="Managers.ReferenceManager"/> in the owner <see cref="CacheFile"/> object 
			/// which stores all the reference names
			/// </summary>
			[System.ComponentModel.Browsable(false)]
			public Blam.DatumIndex ReferenceName
			{
				get { return referenceName; }
				internal set { referenceName = value; }
			}
			#endregion

			#region Address
			protected uint address;
			/// <summary>
			/// Address in memory where this tag is located
			/// </summary>
			[System.ComponentModel.Category("Tag Instance")]
			[System.ComponentModel.ReadOnly(true)]
			public uint Address
			{
				get { return address; }
				internal set { address = value; }
			}
			#endregion

			#region Size
			protected int size;
			/// <summary>
			/// Size of this item
			/// </summary>
			[System.ComponentModel.Browsable(false)]
			[System.ComponentModel.ReadOnly(true)]
			public int Size
			{
				get { return size; }
				internal set { size = value; }
			}
			#endregion

			#region Location
			protected ItemLocation location;
			/// <summary>
			/// In the map or not
			/// </summary>
			[System.ComponentModel.Category("Tag Instance")]
			[System.ComponentModel.ReadOnly(true)]
			public ItemLocation Location
			{
				get { return location; }
				internal set { location = value; }
			}

			/// <summary>
			/// Returns true if the tag has external data
			/// </summary>
			[System.ComponentModel.Browsable(false)]
			public bool HasExternalData { get { return (location == ItemLocation.External); } }
			#endregion

			#region Offset
			protected int offset;
			/// <summary>
			/// The item offset relative to the cache file
			/// </summary>
			[System.ComponentModel.Category("Tag Instance")]
			public int Offset
			{
				get { return offset; }
				internal set { offset = value; }
			}
			#endregion

			#region BspIndex
			protected int bspIndex;
			/// <summary>
			/// If this item is a structure bsp definition, then this is the index in the CacheIndex's BspTags array
			/// </summary>
			[System.ComponentModel.Browsable(false)]
			public int BspIndex
			{
				get { return bspIndex; }
				internal set { bspIndex = value; }
			}
			#endregion

			public override bool Equals(object obj)
			{
				if (obj is Item)
					return this.datum == (obj as Item).datum;

				return false;
			}
			public override int GetHashCode()	{ return datum; }

			/// <summary>
			/// Initialize the properties for a feign state
			/// </summary>
			/// <param name="prev_datum">Handle for the datum that precedes this one</param>
			/// <param name="group_tag"></param>
			internal virtual void InitializeForFeigning(DatumIndex prev_datum, TagInterface.TagGroup group_tag)
			{
				datum = new DatumIndex((ushort)(prev_datum.Index + 1), (short)(prev_datum.Salt + 1));
				GroupTagInt = group_tag.ID;
				groupTag = group_tag;

				tagNameOffset = address = uint.MaxValue;
				location = ItemLocation.Unknown;
				size = offset = bspIndex = -1;
			}

			/// <summary>
			/// Is this item actually fake?
			/// </summary>
			[System.ComponentModel.Browsable(false)]
			internal bool IsFeignItem { get {
				return datum != DatumIndex.Null &&
					tagNameOffset == uint.MaxValue && address == uint.MaxValue &&
					location == ItemLocation.Unknown;
			} }


			#region IStreamable Members
			/// <summary>
			/// Stream item data from a buffer
			/// </summary>
			/// <param name="s"></param>
			public abstract void Read(BlamLib.IO.EndianReader s);
			/// <summary>
			/// Stream item data to a buffer
			/// </summary>
			/// <param name="s"></param>
			public virtual void Write(BlamLib.IO.EndianWriter s) { throw new NotImplementedException(); }
			#endregion
		};

		struct ItemEnumerator : IEnumerator<Item>
		{
			CacheIndex index;
			int current;
			public ItemEnumerator(CacheIndex obj)
			{
				Debug.Assert.If(obj.Tags, "couldn't create index enumerator! {0}", "no tags loaded.");

				index = obj;
				current = -1;
			}

			#region IDisposable Members
			public void Dispose() {}
			#endregion

			#region IEnumerator<Item> Members
			public Item Current { get { return index.Tags[current]; } }
			#endregion

			#region IEnumerator Members
			object System.Collections.IEnumerator.Current { get { return index.Tags[current]; } }

			public bool MoveNext() { return ++current < index.Tags.Length; }

			public void Reset() { current = -1; }
			#endregion
		};
		#endregion


		#region Address
		protected uint address;
		/// <summary>
		/// The base address the tag data starts at, aka 'magic'
		/// </summary>
		[System.ComponentModel.Category("Tag Header")]
		public uint Address	{ get { return address; } }
		#endregion

		#region Scenario
		protected DatumIndex scenario;
		/// <summary>
		/// The handle to this cache file's scenario
		/// </summary>
		[System.ComponentModel.Category("Tag Header")]
		public DatumIndex Scenario	{ get { return scenario; } }
		#endregion

		#region TagCount
		protected int tagCount;
		/// <summary>
		/// The tag count for this index
		/// </summary>
		[System.ComponentModel.Category("Tag Header")]
		public int TagCount	{ get { return tagCount; } }
		#endregion

		#region TagsOffset
		protected uint tagsOffset;
		/// <summary>
		/// The offset to the tags
		/// </summary>
		[System.ComponentModel.Category("Tag Header")]
		public uint TagsOffset	{ get { return tagsOffset; } }
		#endregion

		#region (Abstract) IndexItems
		/// <summary>
		/// Returns the Index Items object used for this map
		/// </summary>
		[System.ComponentModel.Browsable(false)]
		public abstract Item[] Tags { get; }

		/// <summary>
		/// Don't call me directly. Call <see cref="CacheFile.AddFeignTagInstance"/>
		/// </summary>
		/// <param name="cf">Owner of this tag index</param>
		/// <param name="tag_name"></param>
		/// <param name="group_tag"></param>
		/// <returns></returns>
		internal virtual CacheIndex.Item AddFeignItem(CacheFile cf, string tag_name, TagInterface.TagGroup group_tag)
		{ return null; }
		#endregion

		#region BspCount
		protected int bspCount = 0;
		/// <summary>
		/// The # of structure bsps used to construct this map
		/// </summary>
		[System.ComponentModel.Category("Tag Header")]
		public int BspCount { get { return bspCount; } }
		#endregion

		#region BspTags
		protected Item[] bspTags = null;
		/// <summary>
		/// The structure bsps used to construct this map
		/// </summary>
		[System.ComponentModel.Browsable(false)]
		public Item[] BspTags { get { return bspTags; } }
		#endregion


		#region IStreamable Members
		/// <summary>
		/// Read the index data from a stream
		/// </summary>
		/// <param name="s"></param>
		public abstract void Read(BlamLib.IO.EndianReader s);
		/// <summary>
		/// Write the index data to a stream
		/// </summary>
		/// <param name="s"></param>
		public virtual void Write(BlamLib.IO.EndianWriter s) { throw new NotImplementedException(); }
		#endregion

		#region IEnumerable<CacheIndex.Item> Members
		/// <summary>
		/// Interface for enumerating through this index's items
		/// </summary>
		/// <returns></returns>
		public IEnumerator<CacheIndex.Item> GetEnumerator() { return new ItemEnumerator(this); }
		/// <summary>
		/// Interface for enumerating through this index's items
		/// </summary>
		/// <returns></returns>
		System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() { return new ItemEnumerator(this); }
		#endregion
	};
	#endregion

	#region Cache
	/// <summary>
	/// Base class for all Blam map interface definitions
	/// </summary>
	public abstract partial class CacheFile : IO.IStreamable, IDisposable
	{
		public static class TypeName
		{
			public const string kCampaign =			"campaign";
			public const string kMultiplayer =		"multiplayer";
			public const string kMainMenu =			"mainmenu";
			public const string kShared =			"shared";
			public const string kSharedCampaign =	"shared_campaign";
			public const string kUnknown5 =			"unknown5";
			public const string kUnknown6 =			"unknown6";

			/// <summary>
			/// Get the string identifier for the specified cache type
			/// </summary>
			/// <param name="t"></param>
			/// <returns>string identifier for cache type, or null if not defined</returns>
			public static string FromType(CacheType t)
			{
				switch(t)
				{
					case CacheType.Campaign:		return kCampaign;
					case CacheType.Multiplayer:		return kMultiplayer;
					case CacheType.MainMenu:		return kMainMenu;
					case CacheType.Shared:			return kShared;
					case CacheType.SharedCampaign:	return kSharedCampaign;
					case CacheType.Unknown5:		return kUnknown5;
					case CacheType.Unknown6:		return kUnknown6;
				}

				return null;
			}
		};

		/// <summary>
		/// Returns the header object used for this map
		/// </summary>
//		[System.ComponentModel.Browsable(false)]
		public abstract CacheHeader Header { get; }
		/// <summary>
		/// Get a unique name of this peticular cache file
		/// </summary>
		/// <returns></returns>
		/// <remarks>If an unique name is impossible to generate, justs concates the name from the header, an underscore, and the version string from the header</remarks>
		public virtual string GetUniqueName()
		{
			return string.Format("{0}_{1}", Header.Name, Header.Version);
		}

		/// <summary>
		/// Returns the index object used for this map
		/// </summary>
//		[System.ComponentModel.Browsable(false)]
		public abstract CacheIndex Index { get; }

		/// <summary>
		/// Add a feign (fake) tag instance to the tag index
		/// </summary>
		/// <param name="tag_name"></param>
		/// <param name="group_tag"></param>
		/// <returns>Null if engine doesn't implement support this functionality</returns>
		/// <remarks>
		/// Currently only used (and needed) for tag extraction cases where a tag reference may need 
		/// to be changed to a different reference (like an invalid tag so the tag is still usable) 
		/// that doesn't exist in the cache
		/// </remarks>
		internal CacheIndex.Item AddFeignTagInstance(string tag_name, TagInterface.TagGroup group_tag)
		{ return Index.AddFeignItem(this, tag_name, group_tag); }

		#region TagIndexManager
		protected Managers.CacheTagIndex tagIndexManager = null;
		/// <summary>Tag Index Manager for this cache file</summary>
		[System.ComponentModel.Browsable(false)]
		public Managers.CacheTagIndex TagIndexManager	{ get { return tagIndexManager; } }

		/// <summary>Override to provide custom implementations of the <see cref="Managers.CacheTagIndex"/></summary>
		/// <returns></returns>
		protected virtual Managers.CacheTagIndex NewTagIndexImplementation()
		{
			return new Managers.CacheTagIndex(this);
		}

		/// <summary>Initialize the <see cref="TagIndexManager"/> object for use</summary>
		/// <remarks>Recommended this is called AFTER <see cref="InitializeReferenceManager"/></remarks>
		protected internal void InitializeTagIndexManager()
		{
			tagIndexManager = NewTagIndexImplementation();

			Program.GetManager(engineVersion).AddTagIndex(this, tagIndexManager);
		}

		void CloseTagIndexManager()
		{
			if (tagIndexManager != null)
			{
				tagIndexManager.ExtractionDispose();

				Program.GetManager(engineVersion).CloseTagIndex(tagIndexManager.IndexId, true);
				tagIndexManager = null;
			}
		}
		#endregion

		#region References
		protected Managers.ReferenceManager refManager = null;
		/// <summary>
		/// Reference manager for this cache's tags
		/// </summary>
		[System.ComponentModel.Browsable(false)]
		public Managers.ReferenceManager References	{ get { return refManager; } }

		/// <summary>
		/// Initialize the <see cref="References"/> object for use
		/// </summary>
		/// <param name="name">Name to use for the reference manager</param>
		/// <remarks>Recommended this is called just before the index is read from the stream</remarks>
		protected internal void InitializeReferenceManager(string name)
		{
			refManager = new BlamLib.Managers.ReferenceManager(engineVersion, name, true);
		}

		/// <summary>
		/// Get the reference name's path value via a handle that is registered with <see cref="References"/>
		/// </summary>
		/// <param name="reference_name_handle"></param>
		/// <returns>The path string linked to <paramref name="reference_name_handle"/>, or null if retrieval fails</returns>
		public string GetReferenceName(Blam.DatumIndex reference_name_handle)
		{
			if (refManager == null || reference_name_handle == Blam.DatumIndex.Null) return null;

			return refManager[reference_name_handle];
		}

		/// <summary>
		/// Get <paramref name="tag_datum"/>'s reference path value
		/// </summary>
		/// <param name="tag_datum">A tag datum that exists in this cache instance</param>
		/// <returns>The path string linked to <paramref name="tag_datum"/>, or null if retrieval fails</returns>
		public string GetReferenceName(CacheIndex.Item tag_datum)
		{
			if (refManager == null || tag_datum == null) return null;

			return refManager[tag_datum.ReferenceName];
		}

		/// <summary>
		/// Get the tag definition's (whose handle equals <paramref name="tag_index"/>) name
		/// </summary>
		/// <param name="tag_datum">A tag index handle that references an existing datum in this cache instance</param>
		/// <param name="include_tag_group">True if the tag group should be appended to the result (ie, ".bitmap")</param>
		/// <returns>
		/// The path string linked to <paramref name="tag_index"/>'s tag definition, "empty" if the tag definition returns "IsEmpty", 
		/// or null if retrieval fails
		/// </returns>
		public string GetTagIndexName(DatumIndex tag_datum, bool include_tag_group)
		{
			if (refManager == null || tag_datum == DatumIndex.Null) return null;

			CacheIndex.Item i = Index.Tags[tag_datum.Index];
			
			if (i.IsEmpty) return "<empty>";

			string tag_name = refManager[i.ReferenceName];

			if(include_tag_group)
				return string.Format("{0}.{1}", tag_name, i.GroupTag.Name);

			return tag_name;
		}
		#endregion

		#region StringIdManager
		[System.ComponentModel.Browsable(false)]
		public Managers.StringIdManager StringIds { get; private set; }

		IO.EndianReader GetStringIdsIndicesBuffer(ICacheHeaderStringId sid_header)
		{
			int indicies_memory_size = sid_header.StringIdsCount * sizeof(int);

			InputStream.Seek(sid_header.StringIdIndicesOffset);
			byte[] input = InputStream.ReadBytes(indicies_memory_size);

			return new IO.EndianReader(input, engineVersion.GetCacheEndianState());
		}
		/// <summary></summary>
		/// <param name="sid_header"></param>
		/// <returns></returns>
		/// <remarks>This needs to be virtual as with the start of HaloReach, debug data is encrypted, so we have to override this behavior in its implmentation</remarks>
		protected virtual IO.EndianReader GetStringIdsBuffer(ICacheHeaderStringId sid_header)
		{
			InputStream.Seek(sid_header.StringIdsBufferOffset);
			byte[] input = InputStream.ReadBytes(sid_header.StringIdsBufferSize);

			return new IO.EndianReader(input, engineVersion.GetCacheEndianState());
		}

		/// <summary>
		/// Initialize the StringIdManager for this cache and load the dynamic id values
		/// </summary>
		/// <remarks>If this cache doesn't support string ids, this does nothing</remarks>
		protected void StringIdManagerInitializeAndRead()
		{
			// HACK:
			// TODO: We can't decrypt these yet!
			if (engineVersion == BlamVersion.HaloReach_Xbox)
				return;

			var sid_header = Header as ICacheHeaderStringId;
			if (sid_header == null) return; // This cache doesn't use string_ids, do nothing

			var gd = Program.GetManager(engineVersion);
			(gd as Managers.IStringIdController).StringIdCacheOpen(engineVersion);
			var static_collection = gd[engineVersion].GetResource<Managers.StringIdStaticCollection>(Managers.BlamDefinition.ResourceStringIds);

			StringIds = new Managers.StringIdManager(static_collection);

			using (var indices = GetStringIdsIndicesBuffer(sid_header))
			using (var buffer = GetStringIdsBuffer(sid_header))
			{
				StringIds.FromDebugStream(indices, buffer, true, sid_header.StringIdsCount, true);
			}

			StringIds.IsReadOnly = true;
		}

		void StringIdManagerDispose()
		{
			if (StringIds != null)
			{
				StringIds = null;

				var gd = Program.GetManager(engineVersion);
				(gd as Managers.IStringIdController).StringIdCacheClose(engineVersion);
			}
		}
		#endregion

		#region Flags
		protected Util.Flags flags = new Util.Flags(0);
		/// <summary>
		/// Special flags
		/// </summary>
		[System.ComponentModel.Browsable(false)]
		public Util.Flags Flags	{ get { return flags; } }

		/// <summary>
		/// The next available bit which can be used by inheriting 
		/// cache definitions for use in the <see cref="CacheFile.Flags"/>
		/// field
		/// </summary>
		public const int NextBitIndex = 0;
		#endregion

		#region EngineVersion
		protected BlamVersion engineVersion;
		/// <summary>
		/// Returns the engine that this cache is for
		/// </summary>
		[System.ComponentModel.Category("Cache")]
		[System.ComponentModel.ReadOnly(true)]
		public BlamVersion EngineVersion
		{
			get { return engineVersion; }
			internal set { engineVersion = value; }
		}
		#endregion

		#region CacheId
		protected Blam.DatumIndex cacheId = Blam.DatumIndex.Null;
		/// <summary>
		/// Identifier used in a <see cref="Managers.BlamDefinition"/>'s cache collection
		/// </summary>
		[System.ComponentModel.Browsable(false)]
		public Blam.DatumIndex CacheId
		{
			get { return cacheId; }
			internal set { cacheId = value; }
		}
		#endregion

		#region IO Streams
		public IO.EndianReader InputStream = null;
		public IO.EndianWriter OutputStream = null;
		#endregion

		#region IsLoaded
		protected bool isLoaded = false;
		/// <summary>
		/// Returns true if all the basic data for this cache file has been loaded
		/// </summary>
		[System.ComponentModel.Browsable(false)]
		public bool IsLoaded { get { return isLoaded; } }
		#endregion

		#region Address Mask
		/// <summary>
		/// True if map is in computer memory
		/// </summary>
		protected bool isMemoryMap = false;

		/// <summary>Aka, 'magic'</summary>
		protected uint addressMask;
		/// <summary>
		/// The value to subtract from addresses, to get the offset
		/// in the cache stream the address is referring to,
		/// but everyone knows it as 'magic'
		/// </summary>
		[System.ComponentModel.Category("Cache")]
		[System.ComponentModel.ReadOnly(true)]
		public uint AddressMask
		{
			get
			{
				if (isMemoryMap) return 0;

				if (useBspsIndex != -1)
					return bspAddressMasks[useBspsIndex];

				return addressMask;
			}
			set {
				InputStream.BaseAddress = addressMask = value;

				if(OutputStream != null)
					OutputStream.BaseAddress = value;
			}
		}

		protected List<uint> bspAddressMasks = new List<uint>(16);
		/// <summary>
		/// 16 unsigned integers that hold
		/// the address mask for each bsp
		/// </summary>
		/// <remarks>currently, the max bsp count hasn't gone above 16 in h1,2, and stubbs. It may in Halo3</remarks>
		[System.ComponentModel.Category("Cache")]
		[System.ComponentModel.ReadOnly(true)]
		public List<uint> BspAddressMasks { get { return bspAddressMasks; } }

		protected int useBspsIndex = -1;
		/// <summary>
		/// Changes the address mask ('magic') to use, when extracting a bsp
		/// </summary>
		/// <param name="bsp_index"></param>
		public void UseBspAddressMask(int bsp_index)
		{
			useBspsIndex = bsp_index;
			InputStream.BaseAddress = AddressMask;

			if (OutputStream != null)
				OutputStream.BaseAddress = AddressMask;
		}
		#endregion

		/// <summary>
		/// Is the map file at <paramref name="map_path"/> readonly?
		/// </summary>
		/// <param name="map_path"></param>
		/// <returns></returns>
		protected static bool CacheIsReadonly(string map_path)
		{
			var fa = System.IO.File.GetAttributes(map_path);

			return (fa & System.IO.FileAttributes.ReadOnly) != 0;
		}
		#region SharableReference
		/// <summary>
		/// Used for opening shared maps that the library already opens
		/// </summary>
		internal bool IsSharedReference = false;
		/// <summary>
		/// Determines if <paramref name="other"/> is a shared reference
		/// </summary>
		/// <param name="path"></param>
		/// <param name="other"></param>
		/// <returns></returns>
		internal static bool SharableReference(string path, CacheFile other)
		{
			return other != null && other.InputStream.FileName == path;
		}
		/// <summary>
		/// Share the cache information from <paramref name="src"/> with 
		/// <paramref name="dst"/>
		/// </summary>
		/// <param name="dst">Destination cache object</param>
		/// <param name="src">Source cache object with the information we want shared</param>
		internal protected static void ShareCacheStreams(CacheFile dst, CacheFile src)
		{
			dst.InputStream = src.InputStream;
			dst.OutputStream = src.OutputStream;
			dst.IsSharedReference = true;
		}
		#endregion


		/// <summary>
		/// Locate a tag index item based on a complete
		/// datum index held in a 32bit integer
		/// </summary>
		/// <param name="datum">integer holding the datum index</param>
		/// <remarks>Doesn't sanity check the datum, only gets the index</remarks>
		/// <returns>Tag index item associated with that datum index or null if its an invalid datum index</returns>
		public CacheIndex.Item LocateTagByDatum(int datum)
		{
			try { return Index.Tags[DatumIndex.ToIndex(datum)]; }
			catch (Exception) { return null; }
		}

		public bool TryAndFind(string name, TagInterface.TagGroup group, out CacheIndex.Item item)
		{
			item = null;

			DatumIndex reference_name;
			if(refManager.TryAndFind(group, name, out reference_name))
			{
				item = Array.Find(Index.Tags, ci => ci.ReferenceName == reference_name);

				if (item != null) return true;
			}

			return false;
		}

		#region Util
		/// <summary>
		/// Checks the sanity of a 32bit integer
		/// said to be holding a datum index
		/// </summary>
		/// <param name="datum">integer holding the datum index</param>
		/// <returns>True if its a valid datum index for that tag index</returns>
		public bool IsValidTagDatum(int datum)
		{
			short temp = (short)(datum >> 16);
			if (temp <= Index.Tags[Index.Tags.Length - 1].Datum.Salt &&
				temp >= Index.Tags[0].Datum.Salt)
			{
				temp = (short)(datum & 0xFFFF);
				if (temp <= Index.Tags[Index.Tags.Length - 1].Datum.Index &&
					temp >= Index.Tags[0].Datum.Index)
					return true;
			}
			return false;
		}

		/// <summary>
		/// Validates the cache's header for proper signatures
		/// </summary>
		/// <param name="s">cache stream</param>
		/// <param name="header_sizes">size of a single header</param>
		/// <remarks>Assumes the header is at the start of the stream</remarks>
		/// <returns>The header size entry that matches this cache or -1 if no matches</returns>
		internal static int ValidateHeader(IO.EndianReader s, params int[] header_sizes)
		{
			bool invalid = true;
			int result = -1;

			foreach (int header_size in header_sizes)
			{
				s.Seek(0);
				if (s.ReadTagUInt() == MiscGroups.head.ID && s.Length >= header_size) // it HAS to be more than the size of a header...
				{
					s.Seek(header_size - sizeof(uint));
					if (s.ReadTagUInt() == MiscGroups.foot.ID)
					{
						invalid = false;
						result = header_size;
					}
				}

				if(!invalid) break;
			}

			if (invalid) throw new InvalidCacheFileException(s.FileName);

			return result;
		}
		internal static int ValidateHeaderAdjustEndian(IO.EndianReader s, params int[] header_sizes)
		{
			bool invalid = true;
			int result = -1;

			foreach (int header_size in header_sizes)
			{
				if (s.Length < header_size) continue; // it HAS to be more than the size of a header...
				s.Seek(0);
				uint head = s.ReadTagUInt();

				if (head != MiscGroups.head.ID)
				{
					if (IO.ByteSwap.SwapUDWord(head) == MiscGroups.head.ID)
						s.State = s.State.Invert();
				}

				s.Seek(header_size - sizeof(uint));
				if (s.ReadTagUInt() == MiscGroups.foot.ID)
				{
					invalid = false;
					result = header_size;
				}

				if (!invalid) break;
			}

			if (invalid) throw new InvalidCacheFileException(s.FileName);

			return result;
		}
		#endregion

		/// <summary>
		/// Closes the map resources
		/// </summary>
		/// <remarks>Eg, tag manager, string id manager, IO streams, etc</remarks>
		public virtual void Close()
		{
			if (!CacheId.IsNull)
			{
				CloseTagIndexManager();

				refManager = null;

				StringIdManagerDispose();

				if (!IsSharedReference && InputStream != null) InputStream.Close();
				InputStream = null;

				if (!IsSharedReference && OutputStream != null) OutputStream.Close();
				OutputStream = null;

				CacheId = DatumIndex.Null;
			}
		}

		#region IStreamable Members
		/// <summary>
		/// Read the cache's data via the InputStream
		/// </summary>
		public void Read() { Read(InputStream); }

		internal virtual void ReadResourceCache() { throw new NotImplementedException(); }

		/// <summary>
		/// Read the cache's data from a stream
		/// </summary>
		/// <param name="s"></param>
		public abstract void Read(BlamLib.IO.EndianReader s);

		/// <summary>
		/// Write the cache's data to a stream
		/// </summary>
		/// <param name="s"></param>
		public virtual void Write(BlamLib.IO.EndianWriter s) { throw new NotImplementedException(); }
		#endregion

		#region IDisposable Members
		/// <summary>
		/// Calls the <see cref="Close"/> method
		/// </summary>
		public void Dispose()	{ this.Close(); }
		#endregion

		[System.Diagnostics.Conditional("DEBUG")]
		protected virtual void OutputExtraHeaderInfo(System.IO.StreamWriter s) { }

		[System.Diagnostics.Conditional("DEBUG")]
		protected virtual void OutputExtraTagIndexInfo(System.IO.StreamWriter s) { }

		[System.Diagnostics.Conditional("DEBUG")]
		protected virtual void OutputExtraTagInstanceInfo(System.IO.StreamWriter s) { }


		[System.Diagnostics.Conditional("DEBUG")]
		public static void OutputStringIds(CacheFile cache, string path, bool map_string_ids_only)
		{
			var sidm = cache.StringIds;

			using(System.IO.StreamWriter s = new System.IO.StreamWriter(path))
			{
				if (!map_string_ids_only)
				{
					switch (sidm.Definition.Description.Method)
					{
						case Blam.StringIdDesc.GenerateIdMethod.ByLength:
							foreach (var kv in sidm.StaticIdsContainer.StringIdsEnumerator())
								s.WriteLine("{0}\t{1}", kv.Key.ToUInt32().ToString("X8"), kv.Value);
							break;

						case Blam.StringIdDesc.GenerateIdMethod.BySet:
							foreach (var kv in sidm.StaticIdsContainer.StringIdsEnumerator())
								if (kv.Key.Set == 0)
									s.WriteLine("{0}\t{1}", kv.Key.ToUInt32().ToString("X8"), kv.Value);
							break;
					}
				}

				s.WriteLine("Cache String Ids:");

				foreach (var kv in sidm.DynamicIdsContainer.StringIdsEnumerator())
					s.WriteLine("\t{0}\t{1}", kv.Key.ToUInt32().ToString("X8"), kv.Value);
			}
		}

		[System.Diagnostics.Conditional("DEBUG")]
		static void OutputHeader(CacheHeader h, System.IO.StreamWriter s)
		{
			s.WriteLine("Name\t{0}", h.Name);
			s.WriteLine("Build\t{0}", h.Build);
			s.WriteLine("{0}\tVersion", h.Version.ToString("X8"));
			s.WriteLine("{0}\tOffset To Tag Buffer", h.OffsetToIndex.ToString("X8"));
			s.WriteLine("{0}\tTag Buffer Size", h.TagBufferSize.ToString("X8"));

			ICacheHeaderStringId chsi = h as ICacheHeaderStringId;
			if(chsi != null)
			{
				s.WriteLine("--String Ids--");
				s.WriteLine("{0}\tCount", chsi.StringIdsCount.ToString("X8"));
				s.WriteLine("{0}\tSize", chsi.StringIdsBufferSize.ToString("X8"));
				s.WriteLine("{0}\tOffset-Indices", chsi.StringIdIndicesOffset.ToString("X8"));
				s.WriteLine("{0}\tOffset-Buffer", chsi.StringIdsBufferOffset.ToString("X8"));
			}
		}

		[System.Diagnostics.Conditional("DEBUG")]
		public static void OutputTags(CacheFile cf, string path)
		{
			using (System.IO.StreamWriter s = new System.IO.StreamWriter(path))
			{
				s.WriteLine("Mask: {0}", cf.AddressMask.ToString("X8"));
				s.WriteLine();

				OutputHeader(cf.Header, s);
				cf.OutputExtraHeaderInfo(s);

				s.WriteLine();

				cf.OutputExtraTagIndexInfo(s);

				int x;
				CacheIndex.Item[] items = cf.Index.Tags;
				Dictionary<uint, List<CacheIndex.Item>> dic = new Dictionary<uint, List<CacheIndex.Item>>();

				s.WriteLine("Tag Instances:");
				x = 0;
				foreach(var item in items)
				{
					if(!item.IsEmpty)
					{
						s.WriteLine("\t0x{0}\t{1}\t{2}\t{3}\t{4}\t{5}", x.ToString("X"),
							item.Datum.Handle.ToString("X8"),
							item.Address.ToString("X8"),
							item.Offset.ToString("X8"),
							item.GroupTag.TagToString(),
							cf.GetReferenceName(item));

						List<CacheIndex.Item> list;
						if (!dic.TryGetValue(item.GroupTagInt, out list))
							dic.Add(item.GroupTagInt, list = new List<CacheIndex.Item>());
						list.Add(item);
					}
					x++;
				}

				s.WriteLine();
				cf.OutputExtraTagInstanceInfo(s);

				s.WriteLine();
				foreach(KeyValuePair<uint, List<CacheIndex.Item>> kv in dic)
				{
					s.WriteLine("Group:\t--{0}--", new string(TagInterface.TagGroup.FromUInt(kv.Key, false)));
					foreach(CacheIndex.Item ci in kv.Value)
					{
						s.WriteLine("\t{0}\t{1}\t{2}\t{3}\t{4}", ci.Datum.Handle.ToString("X8"),
							ci.Address.ToString("X8"),
							ci.Offset.ToString("X8"),
							ci.GroupTag.TagToString(),
							cf.GetReferenceName(ci));
					}
					s.WriteLine();
					s.WriteLine();
				}
			}
		}
	};
	#endregion

	/// <summary>
	/// Interface to a cache file's header that uses string ids
	/// </summary>
	public interface ICacheHeaderStringId
	{
		/// <summary>
		/// How many string ids there are
		/// </summary>
		int StringIdsCount { get; }
		/// <summary>
		/// Size of the character buffer that holds all the string ids
		/// </summary>
		int StringIdsBufferSize { get; }
		/// <summary>
		/// Offset to a table containing the character index (relative to <b>StringIdsBuffer</b>) 
		/// marking the start of each string id value
		/// </summary>
		int StringIdIndicesOffset { get; }
		/// <summary>
		/// Offset to the character buffer which containing all the string id values
		/// </summary>
		int StringIdsBufferOffset { get; }
	};


	/// <summary>
	/// Exception that can be thrown when a cache file interface 
	/// realizes that it is about to try and load a shared cache only 
	/// cache file (ie, its not a real cache file)
	/// </summary>
	public class SharedCacheAccessException : Debug.ExceptionLog
	{
		public SharedCacheAccessException() : base("Can't load resource-only cache files") { }
	};
	/// <summary>
	/// Exception that is thrown when reading a cache file's header which doesn't have valid data
	/// </summary>
	public class InvalidCacheFileException : Debug.ExceptionLog
	{
		/// <summary>
		/// Construct the exception
		/// </summary>
		/// <param name="filename">name of the file we were trying to read as a cache</param>
		public InvalidCacheFileException(string filename) : base("The file '{0}' is not a cache file or is corrupt.", filename) {}
	};
}