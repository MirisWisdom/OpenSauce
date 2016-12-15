/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib.Managers
{
	/// <summary>
	/// Implemented by tag_database <see cref="TagInterface.Definition">Definitions</see>
	/// </summary>
	public interface ITagDatabase
	{
		/// <summary>
		/// Is the database void of tag entries?
		/// </summary>
		bool IsEmpty { get; }
	};

	/*
	 * I debated on creating a 'TagDatabaseItem' interface and then a single
	 * 'TagDatabase' class which used said interface for it's objects but
	 * the main concern was the fact that for cache items, their name must
	 * be polled via the owner cache object and thus a item interface wouldn't
	 * be all THAT helpful as a protected abstract method would have to be
	 * made which got the tag path of a 'TagDatabaseItem', resulting in casts
	 * and such.
	 * 
	 * In the end, both 'TagDatabase' and 'CacheTagDatabase' work off the same
	 * tag group definition and thus only need a UI to work off that, and not
	 * these actual database objects.
	 */

	public interface ITagDatabaseAddable
	{
		string GetTagName();

		char[] GetGroupTag();
	};

	/// <summary>
	/// Base interface for game specific Tag Databases
	/// </summary>
	/// <remarks>
	/// Database should be created in a sequential manner.
	/// 
	/// What this means is that the first "root" tag is added then 
	/// has it's child references added after it. After which, it 
	/// iterates over each of those child references and then treats 
	/// them as if they were a "root" tag, and so the process is repeated
	/// </remarks>
	public abstract class TagDatabase
	{
		// Two "hashes" are used when using a TagDatabase.
		// The first hash (internal) is an actual hash, built from the tag name and group tag's hash code
		// The second hash (public) is just the index of the database Entry item

		#region Hash Methods
		static long CalculateHashCode64(string tag_name, TagInterface.TagGroup group_tag)
		{
			long hash;

			hash = tag_name.GetHashCode();
			hash <<= 32;

			hash |= group_tag.ID;

			return hash;
		}

		static long CalculateHashCode64(ITagDatabaseAddable item)
		{
			long hash;

			hash = item.GetTagName().GetHashCode();
			hash <<= 32;

			hash |= TagInterface.TagGroup.ToUInt(item.GetGroupTag());

			return hash;
		}

		static long CalculateHashCode64(ITagDatabaseAddable item, ref HandleData data)
		{
			long hash;

			hash = (data.Name = item.GetTagName()).GetHashCode();
			hash <<= 32;

			hash |= TagInterface.TagGroup.ToUInt(data.GroupTag = item.GetGroupTag());

			return data.Hash = hash;
		}

		static void FromHashCode64(long hash, out int name_hash, out uint group_tag_hash)
		{
			name_hash = (int)((hash >> 32) & 0xFFFFFFFF);
			group_tag_hash = (uint)(hash & 0xFFFFFFFF);
		}
		#endregion


		#region Engine
		private BlamVersion engine;
		/// <summary>
		/// Engine this database is for
		/// </summary>
		public BlamVersion Engine	{ get { return engine; } }
		#endregion

		#region Root
		protected struct HandleData
		{
			public string Name;
			public char[] GroupTag;
			public long Hash;

			public HandleData(ITagDatabaseAddable item)
			{
				this.Name = null;
				this.GroupTag = null;
				Hash = -1;
				CalculateHashCode64(item, ref this);
			}

			public HandleData(string tag_name, TagInterface.TagGroup group_tag)
			{
				Hash = CalculateHashCode64(this.Name = tag_name, group_tag);
				GroupTag = group_tag.Tag;
			}
		};
		/*protected*/ HandleData rootData;

		/// <summary>
		/// Set the current "root" tag of the database
		/// </summary>
		/// <param name="root"></param>
		public void SetRoot(ITagDatabaseAddable root)							{ Add(rootData = new HandleData(root)); }
		public void SetRoot(string tag_name, TagInterface.TagGroup group_tag)	{ Add(rootData = new HandleData(tag_name, group_tag)); }
		#endregion

		/// <summary>
		/// Database tag definition
		/// </summary>
		public abstract TagInterface.Definition Definition { get; }


		#region Ctor
		/// <summary>
		/// Initialize the tag database without a root tag
		/// </summary>
		/// <param name="engine"></param>
		/// <remarks>Be sure to call <see cref="SetRoot(ITagDatabaseAddable)"/> or <see cref="SetRoot(string, TagInterface.TagGroup)"/> before adding tags</remarks>
		protected TagDatabase(BlamVersion engine) { this.engine = engine; }
		/// <summary>
		/// Initialize the tag database with a root tag
		/// </summary>
		/// <param name="engine"></param>
		/// <param name="root_tag"></param>
		protected TagDatabase(BlamVersion engine, ITagDatabaseAddable root_tag) : this(engine)
		{ SetRoot(root_tag); }
		protected TagDatabase(BlamVersion engine, string tag_name, TagInterface.TagGroup group_tag) : this(engine)
		{ SetRoot(tag_name, group_tag); }
		#endregion

		protected abstract int Add(HandleData reference);

		protected abstract int AddDependent(HandleData tag, HandleData dependent_tag);

		// I don't see any reasons for implementing Add variants for explicit
		// tag_name and group_tag values

		#region ITagDatabaseAddable
		/// <summary>
		/// Add a tag to the database
		/// </summary>
		/// <param name="tag"></param>
		/// <returns>Hash value for tag's database entry</returns>
		public int Add(ITagDatabaseAddable tag)
		{
			HandleData data = new HandleData(tag);
			return Add(data);
		}

		/// <summary>
		/// Add a tag to the database using the current root tag.
		/// </summary>
		/// <param name="dependent_tag">Tag which is directly referenced by the current root tag</param>
		/// <returns>Hash value for tag's database entry</returns>
		public int AddDependent(ITagDatabaseAddable dependent_tag)
		{
			HandleData data = new HandleData(dependent_tag);
			return AddDependent(rootData, data);
		}

		/// <summary>
		/// Add a tag to the database using <paramref name="tag"/> as the root tag
		/// </summary>
		/// <param name="tag">Tag to use as the root tag when adding <paramref name="dependent_tag"/> to the database</param>
		/// <param name="dependent_tag">Tag which is directly referenced by <paramref name="tag"/></param>
		/// <returns>Hash value for tag's database entry</returns>
		/// <remarks>
		/// Does not modify the current root tag.
		/// <paramref name="tag"/> must already already exist in the database
		/// </remarks>
		public int AddDependent(ITagDatabaseAddable tag, ITagDatabaseAddable dependent_tag)
		{
			HandleData root_data = new HandleData(tag);
			HandleData data = new HandleData(dependent_tag);
			return AddDependent(root_data, data);
		}
		#endregion
	};

	/// <summary>
	/// Base interface for game specific Tag Databases on cache files
	/// </summary>
	/// <remarks>Database should be created in a sequential manner.</remarks>
	public abstract class CacheTagDatabase
	{
		#region Root
		protected Blam.CacheIndex.Item root = null;
		/// <summary>
		/// Set the current "root" tag of the database
		/// </summary>
		/// <param name="root_tag"></param>
		public void SetRoot(Blam.CacheIndex.Item root_tag) { Add(root = root_tag); }
		#endregion

		/// <summary>
		/// Database tag definition
		/// </summary>
		public abstract TagInterface.Definition Definition { get; }

		/// <summary>
		/// Initialize the tag database without a root tag
		/// </summary>
		/// <remarks>Be sure to call <see cref="SetRoot"/> before adding tags</remarks>
		protected CacheTagDatabase() {}
		/// <summary>
		/// Initialize the tag database with a root tag
		/// </summary>
		/// <param name="root_tag"></param>
		protected CacheTagDatabase(Blam.CacheIndex.Item root_tag) { SetRoot(root_tag); }

		/// <summary>
		/// Add a tag to the database
		/// </summary>
		/// <param name="tag"></param>
		/// <returns>Hash value for tag's database entry</returns>
		public abstract int Add(Blam.CacheIndex.Item tag);

		/// <summary>
		/// Add a tag to the database using the current root tag.
		/// </summary>
		/// <param name="dependent_tag">Tag which is directly referenced by the current root tag</param>
		/// <returns>Hash value for tag's database entry</returns>
		public int AddDependent(Blam.CacheIndex.Item dependent_tag) { return AddDependent(root, dependent_tag); }

		/// <summary>
		/// Add a tag to the database using <paramref name="tag"/> as the root tag
		/// </summary>
		/// <param name="tag">Tag to use as the root tag when adding <paramref name="dependent_tag"/> to the database</param>
		/// <param name="dependent_tag">Tag which is directly referenced by <paramref name="tag"/></param>
		/// <returns>Hash value for tag's database entry</returns>
		/// <remarks>
		/// Does not modify the current root tag.
		/// <paramref name="tag"/> must already already exist in the database
		/// </remarks>
		public abstract int AddDependent(Blam.CacheIndex.Item tag, Blam.CacheIndex.Item dependent_tag);
	};

	/// <summary>
	/// Base interface for game specific Tag Database dealing with errors encountered 
	/// while working on tag files
	/// </summary>
	/// <remarks>Database should be created in a sequential manner.</remarks>
	public abstract class ErrorTagDatabase
	{
		/// <summary>
		/// Various flags that can be used to describe the kinds of errors encountered with tags
		/// </summary>
		/// <remarks>See <see cref="TagIndex"/>'s Sentinel constants</remarks>
		public enum ErrorFlags : ushort
		{
			/// <summary>
			/// No errors to report.
			/// 
			/// Use this on root tags which encounter problematic references
			/// </summary>
			None = 0,
			/// <summary>
			/// Reference was skipped due to certain conditions
			/// </summary>
			Skipped = 1 << 0,
			/// <summary>
			/// Reference data was found to be missing when an operation was trying to perform
			/// </summary>
			Missing = 1 << 1,
			/// <summary>
			/// Reference had invalid or unhandled versioning data
			/// </summary>
			InvalidVersion = 1 << 2,

			/// <summary>
			/// Reference failed to be operated on (ie, load)
			/// </summary>
			Failure = 1 << 3,
		};

		protected struct Entry : IEqualityComparer<Entry>
		{
			public static readonly IEqualityComparer<Entry> kEqualityComparer = new Entry();

			public string Path;
			public uint GroupTag;
			public Entry(string s, uint i) { Path = s; GroupTag = i; }

			#region IEqualityComparer<Entry> Members
			public bool Equals(Entry x, Entry y) { return x.GroupTag == y.GroupTag && x.Path == y.Path; }
			public int GetHashCode(Entry obj) { return obj.GetHashCode(); }
			#endregion

			public override bool Equals(object obj)
			{
				if (obj is Entry)
					return Equals(this, (Entry)obj);

				return false;
			}
			public override int GetHashCode()	{ return Path.GetHashCode() ^ unchecked((int)GroupTag); }

			public bool Equals(string path, uint group_tag) { return GroupTag == group_tag && Path == path; }
		};

		#region Root
		protected Entry root;
		/// <summary>
		/// Set the current "root" tag of the database
		/// </summary>
		/// <param name="path"></param>
		/// <param name="group_tag"></param>
		public void SetRoot(string path, uint group_tag) { root = new Entry(path, group_tag); Add(path, group_tag, ErrorFlags.None); }
		#endregion

		#region Engine
		private BlamVersion engine;
		/// <summary>
		/// Engine this database is for
		/// </summary>
		public BlamVersion Engine { get { return engine; } }
		#endregion

		/// <summary>
		/// Database tag definition
		/// </summary>
		public abstract TagInterface.Definition Definition { get; }

		protected ErrorTagDatabase(BlamVersion engine) { this.engine = engine; }

		/// <summary>
		/// Add a non-problematic tag to the database
		/// </summary>
		/// <param name="path">Tag definition's relative path</param>
		/// <param name="group_tag">Tag definition's group</param>
		/// <returns>Hash value for tag's database entry</returns>
		/// <remarks>
		/// Labels the tag definition as having no errors. Useful when we want to highlight the exact
		/// tag dependency graph used when encountering errors.
		/// </remarks>
		public int Add(string path, uint group_tag) { return Add(path, group_tag, ErrorFlags.None); }

		/// <summary>
		/// Add a problematic tag to the database
		/// </summary>
		/// <param name="path">Tag definition's relative path</param>
		/// <param name="group_tag">Tag definition's group</param>
		/// <param name="flags">Specific error flags for this tag</param>
		/// <returns>Hash value for tag's database entry</returns>
		public abstract int Add(string path, uint group_tag, ErrorFlags flags);

		/// <summary>
		/// Add a non-problematic tag reference to the database
		/// </summary>
		/// <param name="tr">Reference containing the tag definition information</param>
		/// <returns>Hash value for tag's database entry</returns>
		/// <remarks>
		/// Labels the tag definition as having no errors. Useful when we want to highlight the exact
		/// tag dependency graph used when encountering errors.
		/// </remarks>
		public int Add(TagInterface.TagReference tr) { return Add(tr, ErrorFlags.None); }

		/// <summary>
		/// Add a problematic tag reference to the database
		/// </summary>
		/// <param name="tr">Reference containing the tag definition information</param>
		/// <param name="flags">Specific error flags for this tag reference</param>
		/// <returns>Hash value for tag's database entry</returns>
		public int Add(TagInterface.TagReference tr, ErrorFlags flags)			{ return Add(tr.GetTagPath(), tr.GroupTagInt, flags); }

		/// <summary>
		/// Add a problematic tag to the database that is a dependent of <see cref="root"/>
		/// </summary>
		/// <param name="path">Tag definition's relative path</param>
		/// <param name="group_tag">Tag definition's group</param>
		/// <param name="flags">Specific error flags for this tag</param>
		/// <returns>Hash value for tag's database entry</returns>
		public abstract int AddDependent(string path, uint group_tag, ErrorFlags flags);

		/// <summary>
		/// Add a problematic tag reference to the database that is a dependent of <see cref="root"/>
		/// </summary>
		/// <param name="tr">Reference containing the tag definition information</param>
		/// <param name="flags">Specific error flags for this tag reference</param>
		/// <returns>Hash value for tag's database entry</returns>
		public int AddDependent(TagInterface.TagReference tr, ErrorFlags flags)	{ return AddDependent(tr.GetTagPath(), tr.GroupTagInt, flags); }

		/// <summary>
		/// Determine if a tag is present in the error database
		/// </summary>
		/// <param name="path">Tag's name</param>
		/// <param name="group_tag">Tag's group tag</param>
		/// <returns>True if the tag is present (is unbiased towards any flags, eg <see cref="ErrorFlags.None"/>)</returns>
		public abstract bool Contains(string path, uint group_tag);
	};
}