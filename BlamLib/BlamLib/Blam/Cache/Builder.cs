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
	/// Sections in the cache which we use to store various streams in
	/// </summary>
	public enum CacheSectionType
	{
		/// <summary>
		/// The Debug section, used for writing debug related resources 
		/// to the cache file (ie, string_ids)
		/// </summary>
		Debug,
		/// <summary>
		/// The Resource section, used for storing pageable data that is 
		/// only needed during certain situations in the scenario
		/// </summary>
		Resource,
		/// <summary>
		/// The Tag section, used for storing various tag data
		/// </summary>
		Tag,
		/// <summary>
		/// The Locale section, used for storing localization text data
		/// </summary>
		/// <remarks>AKA: language pack</remarks>
		Localization,

		kMax,
	};
}

namespace BlamLib.Blam.Cache
{
	/// <summary>
	/// Various streams we can write to when building a cache
	/// </summary>
	public enum StreamType
	{
		/// <summary>
		/// The DebugStream, used for writing debug related resources 
		/// to the cache file (ie, string_ids)
		/// </summary>
		Debug,

		/// <summary>
		/// The OutputStream, used for writing resources to the
		/// cache file
		/// </summary>
		Output,

		/// <summary>
		/// The LocalizationStream, used for writing localization resources 
		/// to the cache file
		/// </summary>
		Localization,

		/// <summary>
		/// The BspsStream, used for writing structure bsp tag data
		/// to the cache file bsp buffer
		/// </summary>
		Bsps,

		/// <summary>
		/// The TagStream, used for writing tag data to the cache
		/// file buffer
		/// </summary>
		Tag,
	};

	/// <summary>
	/// Cache File builder base class
	/// </summary>
	public abstract class BuilderBase : IDisposable
	{
		#region Engine
		protected BlamVersion engineVersion;
		/// <summary>
		/// Returns the engine version this
		/// cache is for
		/// </summary>
		public BlamVersion EngineVersion { get { return engineVersion; } }
		#endregion

		#region BuilderId
		protected Blam.DatumIndex builderId = Blam.DatumIndex.Null;
		/// <summary>
		/// Handle for this builder to <see cref="Managers.BlamDefinition"/>'s management
		/// </summary>
		public Blam.DatumIndex BuilderId
		{
			get { return builderId; }
			internal set { builderId = value; }
		}
		#endregion

		#region Tags
		protected List<BuilderItem> tags = new List<BuilderItem>(128);
		/// <summary>
		/// Returns the Index Items object used for this map
		/// </summary>
		public List<BuilderItem> Tags { get { return tags; } }

		/// <summary>
		/// Adds a new tag to the list
		/// </summary>
		/// <remarks>Item will only have its index value set</remarks>
		/// <returns>Reference to the new BuilderItem</returns>
		public abstract BuilderItem AddTag();

		/// <summary>
		/// Checks to see if <paramref name="file"/> tag is loaded already
		/// </summary>
		/// <param name="file">tag file name (relative to tags directory)</param>
		/// <param name="filter">group tag to use as a filter</param>
		/// <returns>BuilderItem representing <paramref name="file"/></returns>
		public BuilderItem TagIsLoaded(string file, TagInterface.TagGroup filter)
		{
			//foreach (BuilderItem t in tags)
			//	if (t.FileName == file && filter.Test(t.GroupTag))
			//		return t;

			return null;
		}
		#endregion

		#region StreamMode
		protected StreamType streamMode = StreamType.Tag;
		/// <summary>
		/// Stream that <see cref="CurrentStream"/> will return
		/// </summary>
		public StreamType StreamMode
		{
			get { return streamMode; }
			set { streamMode = value; }
		}
		#endregion

		#region Streams
		public IO.EndianReader InputStream = null;

		/// <summary>
		/// Stream that will be attached to the file
		/// that acts as the final cache
		/// </summary>
		protected IO.EndianWriter CacheStream = null;

		/// <summary>
		/// File stream used for writing resources to
		/// </summary>
		public IO.EndianWriter OutputStream = null;

		/// <summary>
		/// Buffer used to store debug related data
		/// </summary>
		public IO.EndianWriter DebugStream = null;
		/// <summary>
		/// Buffer used to store localization data
		/// </summary>
		public IO.EndianWriter LocalizationStream = null;
		/// <summary>
		/// Buffer used to store structure bsp tag data
		/// </summary>
		public IO.EndianWriter BspsStream = null;
		/// <summary>
		/// Buffer used to store tag data
		/// </summary>
		public IO.EndianWriter TagStream = null;
		/// <summary>
		/// Current stream being written to
		/// </summary>
		public IO.EndianWriter CurrentStream
		{
			get
			{
				switch (streamMode)
				{
					case StreamType.Debug:			return DebugStream;
					case StreamType.Output:			return OutputStream;
					case StreamType.Localization:	return LocalizationStream;
					case StreamType.Bsps:			return BspsStream;
					case StreamType.Tag:			return TagStream;
					default:						throw new Debug.Exceptions.UnreachableException();
				}
			}
		}
		#endregion

		protected BuilderBase(BlamVersion engine) { engineVersion = engine; }

		public void AddResource(byte[] buffer, out int offset)
		{
			offset = OutputStream.Position;
			OutputStream.Write(buffer, buffer.Length);
		}

		#region IDisposable Members
		public virtual void Dispose()
		{
		}
		#endregion
	};

	/// <summary>
	/// Cache file builder base tag index class
	/// </summary>
	public abstract class BuilderTagIndexBase : Managers.ITagIndex, IEnumerable<BuilderItem>
	{
		#region Unimplemented Managers.ITagIndex methods
		/// <summary>
		/// Does nothing
		/// </summary>
		/// <param name="tag_index"></param>
		/// <returns>null</returns>
		/// <see cref="BlamLib.Managers.DataArray{T}"/>
		Managers.TagManager Managers.ITagIndex.this[Blam.DatumIndex tag_index] { get { return null; } }

		#region IndexId
		/// <summary>
		/// Handle for this index to <see cref="Managers.BlamDefinition"/>'s management
		/// </summary>
		/// <remarks>Always returs <see cref="Blam.DatumIndex.Null"/></remarks>
		Blam.DatumIndex Managers.ITagIndex.IndexId { get { return Blam.DatumIndex.Null; } }
		#endregion

		#region Ignore List
		/// <summary>
		/// Get the ignore tag group list for this tag index
		/// </summary>
		/// <remarks>Returns <c>null</c></remarks>
		TagInterface.TagGroupCollection Managers.ITagIndex.IgnoreList { get { return null; } }
		#endregion

		/// <summary>
		/// Does nothing
		/// </summary>
		/// <param name="name"></param>
		/// <param name="tag_group"></param>
		/// <returns><see cref="Blam.DatumIndex.Null"/></returns>
		Blam.DatumIndex Managers.ITagIndex.Open(string name, TagInterface.TagGroup tag_group) { return DatumIndex.Null; }

		/// <summary>
		/// Does nothing.
		/// </summary>
		/// <param name="tag_index"></param>
		void Managers.ITagIndex.UnloadWithDependents(Blam.DatumIndex tag_index) { }

		/// <summary>
		/// Does nothing.
		/// </summary>
		/// <param name="tag_index"></param>
		void Managers.ITagIndex.UnloadForceWithDependents(Blam.DatumIndex tag_index) { }
		#endregion

		#region DataArray
		//protected Managers.DataArray<BuilderItem> Array = new Managers.DataArray<BuilderItem>("builder tag instances");
		Managers.IDataArray dataArray;

		internal protected void DataArraySet(Managers.IDataArray value) { dataArray = value; }
		#endregion

		#region Engine
		protected BlamVersion engine;
		/// <summary>
		/// Engine version this tag index is for
		/// </summary>
		public BlamVersion Engine { get { return engine; } }
		#endregion

		#region SourceIndex
		protected Managers.ITagIndex sourceIndex;
		/// <summary>
		/// Tag index which we're building a cache file from
		/// </summary>
		public Managers.ITagIndex SourceIndex	{ get { return sourceIndex; } }
		#endregion

		#region References
		protected Managers.ReferenceManager refManager;
		/// <summary>
		/// Reference manger for this index
		/// </summary>
		public Managers.ReferenceManager References { get { return refManager; } }
		#endregion

		#region StringIdManager
		Managers.StringIdManager stringIdManager = null;
		/// <summary>
		/// String ids manager for this index
		/// </summary>
		public Managers.StringIdManager StringIds { get { return stringIdManager; } }

		void StringTableInitialize()
		{
			var gd = Program.GetManager(engine);
			(gd as Managers.IStringIdController).StringIdCacheOpen(engine);
			var static_collection = gd[engine].GetResource<Managers.StringIdStaticCollection>(Managers.BlamDefinition.ResourceStringIds);

			stringIdManager = new Managers.StringIdManager(static_collection);
		}

		void StringTableDispose()
		{
			if (stringIdManager != null)
			{
				stringIdManager = null;

				var gd = Program.GetManager(engine);
				(gd as Managers.IStringIdController).StringIdCacheClose(engine);
			}
		}
		#endregion

		#region Ctor
		/// <summary>
		/// Create a new builder tag index
		/// </summary>
		/// <param name="version">Engine version for the tags</param>
		/// <param name="source_index"></param>
		/// <remarks>
		/// Must call <see cref="DataArraySet"/> in inheriting object's ctor
		/// </remarks>
		protected BuilderTagIndexBase(BlamVersion version, Managers.ITagIndex source_index)
		{
			Debug.Assert.If(source_index, "Couldn't create a builder tag index! Require a valid source index.");
			Debug.Assert.If(source_index.Engine == version, "Couldn't create a builder tag index! Engine mismatch: {0:X} != {1:X}.", source_index.Engine, version);
			engine = version;
			sourceIndex = source_index;

			refManager = new Managers.ReferenceManager(version, string.Format("[BuilderTagIndex {0}]", version.ToString("X")), false);

			if (engine.UsesStringIds())
				StringTableInitialize();
		}

		public void Dispose()
		{
			UnloadAll();

			if (engine.UsesStringIds())
				StringTableDispose();
		}
		#endregion

		#region Add
		/// <summary>
		/// Create a new <see cref="BuilderItem"/> object from a tag that exists in the <see cref="SourceIndex"/>
		/// </summary>
		/// <param name="source_tag">Tag that exists in <see cref="SourceIndex"/></param>
		/// <returns></returns>
		protected abstract BuilderItem BuildFromSource(Managers.TagManager source_tag);

		public DatumIndex Add(DatumIndex source_index_tag_index)
		{
			Managers.TagManager tagman = sourceIndex[source_index_tag_index];
			return BuildFromSource(tagman).Datum;
		}
		#endregion

		#region Remove
		/// <summary>
		/// Updates the reference count for the tag, unloading the tag if there are no more references
		/// </summary>
		/// <param name="tag_index">index of the tag to unload</param>
		/// <returns>true if no longer in the tag index</returns>
		public bool Unload(Blam.DatumIndex tag_index)
		{
			//BuilderItem tm = this[tag_index];
			bool removed = dataArray.Remove(tag_index);
			//if(removed)
			//	refManager.Remove(tm.ReferenceName);

			return removed;
		}

		/// <summary>
		/// Removes the tag from the index completely, disregarding
		/// any other cases (IE, multiple references to the tag)
		/// </summary>
		/// <param name="tag_index">index of the tag to force the unload upon</param>
		public void UnloadForce(Blam.DatumIndex tag_index)
		{
			//BuilderItem tm = this[tag_index];
			//refManager.Delete(tm.ReferenceName);
			dataArray.Close(tag_index);
		}

		/// <summary>
		/// Removes all tags from the index completely, disregarding
		/// any other cases (IE, multiple references to the tag)
		/// </summary>
		public void UnloadAll()
		{
			sourceIndex = null;
			dataArray.CloseAll();
			refManager.Clear();
		}
		#endregion

		#region Util
		#endregion

		#region IEnumerable<BuilderItem> Members
		/// <summary>
		/// Does nothing
		/// </summary>
		/// <returns><c>null</c></returns>
		IEnumerator<BuilderItem> System.Collections.Generic.IEnumerable<BuilderItem>.GetEnumerator() { return null; }
		System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() { return dataArray.GetEnumerator(); }
		#endregion
	};

	/// <summary>
	/// Base class for tag index datums
	/// </summary>
	public abstract class BuilderItem : ICacheObject, Managers.IReferenceMangerObject, IDisposable
	{
		#region Datum
		protected DatumIndex datum = DatumIndex.Null;
		/// <summary>
		/// The tag's datum
		/// </summary>
		public DatumIndex Datum
		{
			get { return datum; }
			internal set { datum = value; }
		}

		/// <summary>
		/// Returns the index of this item in the cache's tag index
		/// </summary>
		public int Index { get { return datum.Index; } }
		#endregion

		#region SourceIndexDatum
		protected DatumIndex sourceIndexDatum = DatumIndex.Null;
		/// <summary>
		/// The tag's datum in the <see cref="BuilderTagIndexBase.SourceIndex"/> in the <see cref="BuilderTagIndexBase"/> owner
		/// </summary>
		public DatumIndex SourceIndexDatum	{ get { return sourceIndexDatum; } }
		#endregion

		#region GroupTag
		protected TagInterface.TagGroup groupTag = TagInterface.TagGroup.Null;
		/// <summary>
		/// The four character code that designates
		/// the tag group this item holds in its data
		/// </summary>
		public TagInterface.TagGroup GroupTag	{ get { return groupTag; } }
		#endregion

		#region ReferenceName
		protected Blam.DatumIndex referenceName = DatumIndex.Null;
		/// <summary>
		/// Handle for the <see cref="Managers.ReferenceManager"/> in the owner <see cref="BuilderTagIndexBase"/> object 
		/// which stores all the reference names
		/// </summary>
		public Blam.DatumIndex ReferenceName
		{
			get { return referenceName; }
			internal set { referenceName = value; }
		}
		#endregion

		#region FileNameAddress
		protected uint fileNameAddress = uint.MaxValue;
		/// <summary>
		/// The CString file name address in the tag stream, or offset 
		/// if the file names are stored in debug sections
		/// </summary>
		public uint FileNameAddress
		{
			get { return fileNameAddress; }
			set { fileNameAddress = value; }
		}
		#endregion

		#region Address
		protected uint address = uint.MaxValue;
		/// <summary>
		/// Address in memory where this tag is located
		/// </summary>
		public uint Address
		{
			get { return address; }
			set { address = value; }
		}
		#endregion

		#region Size
		protected int size = 0;
		/// <summary>
		/// Size of this item
		/// </summary>
		public int Size
		{
			get { return size; }
			set { size = value; }
		}
		#endregion

		#region Offset
		protected int offset = -1;
		/// <summary>
		/// The item offset in the Builder's <see cref="BuilderBase.TagStream"/>
		/// </summary>
		public int Offset
		{
			get { return offset; }
			set { offset = value; }
		}
		#endregion

		/// <summary>Only for satisfying <see cref="BlamLib.Managers.DataArray{T}"/>'s template constraints</summary>
		internal protected BuilderItem() {}

		protected BuilderItem(BuilderTagIndexBase owner, Managers.TagManager source)
		{
			this.sourceIndexDatum = source.TagIndex;
			this.groupTag = source.GroupTag;
			this.referenceName = Managers.ReferenceManager.CopyHandle(/*this,*/ owner.References, owner.SourceIndex.References, source.ReferenceName);
		}

		#region ICacheObject Members
		public abstract void PreProcess(BuilderBase owner);

		public abstract bool Build(BuilderBase owner);

		public abstract void PostProcess(BuilderBase owner);
		#endregion

		#region IDisposable Members
		public virtual void Dispose()	{}
		#endregion

		#region IReferenceMangerObject Members
		DatumIndex BlamLib.Managers.IReferenceMangerObject.ReferenceId								{ get { return referenceName; } }

		DatumIndex BlamLib.Managers.IReferenceMangerObject.ParentReferenceId						{ get { return DatumIndex.Null; } }

		IEnumerable<DatumIndex> BlamLib.Managers.IReferenceMangerObject.GetReferenceIdEnumerator()	{ yield break; }

		bool BlamLib.Managers.IReferenceMangerObject.UpdateReferenceId(BlamLib.Managers.ReferenceManager manager, DatumIndex new_datum)
		{
			this.referenceName = new_datum;

			return true;
		}
		#endregion
	};

	/// <summary>
	/// Implement in objects that need special code for being put into a cache file
	/// </summary>
	public interface ICacheObject
	{
		/// <summary>
		/// Prepare any resources needed before building
		/// </summary>
		/// <param name="owner"></param>
		void PreProcess(BuilderBase owner);

		/// <summary>
		/// Build object into cache
		/// </summary>
		/// <param name="owner"></param>
		/// <returns></returns>
		bool Build(BuilderBase owner);

		/// <summary>
		/// Perform last minute operations for this object before completion
		/// </summary>
		/// <param name="owner"></param>
		void PostProcess(BuilderBase owner);
	};
}