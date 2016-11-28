/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo1.Tags
{
	#region bitmap
	public partial class bitmap_group
	{
		#region bitmap_data_block
		public partial class bitmap_data_block
		{
			public override int GetDepth() { return Depth.Value; }
			public override short GetMipmapCount() { return MipmapCount.Value; }
		};
		#endregion
	};
	#endregion

	#region tag_database
	partial class tag_database_group
	{
		public bool IsEmpty { get { return Entries.Count == 0; } }
	};
	#endregion

	public class TagDatabase : Managers.TagDatabase
	{
		System.Collections.Generic.Dictionary<long, tag_database_entry_block> lookup = new System.Collections.Generic.Dictionary<long, tag_database_entry_block>();

		#region Definition
		tag_database_group db = new tag_database_group();
		/// <summary>
		/// Database tag definition
		/// </summary>
		public override TI.Definition Definition { get { return db; } }
		#endregion

		/// <summary>
		/// Get a entry in the database
		/// </summary>
		/// <param name="hash">Hash value for tag entry we wish to get</param>
		/// <returns></returns>
		public tag_database_entry_block this[int hash] { get { return db.Entries[hash]; } }

		/// <summary>
		/// Hack just for Stubbs
		/// </summary>
		/// <param name="engine"></param>
		internal TagDatabase(BlamVersion engine) : base(engine) {}

		public TagDatabase() : base(BlamVersion.Halo1) {}
		public TagDatabase(Managers.ITagDatabaseAddable root) : base(BlamVersion.Halo1, root) { }
		public TagDatabase(string tag_name, TagInterface.TagGroup group_tag) : base(BlamVersion.Halo1, tag_name, group_tag) { }

		protected override int Add(HandleData data)
		{
			int index = -1;

			tag_database_entry_block entry;
			if (!lookup.TryGetValue(data.Hash, out entry)) // hey, not in the database
			{
				index = db.Entries.Count;				// count will be the index of our new entry
				db.Entries.Add(out entry);				// add a new entry
				// set the name
				entry.Name.Value = new byte[data.Name.Length + 1];
				System.Text.Encoding.ASCII.GetBytes(data.Name, 0, data.Name.Length, entry.Name.Value, 0);
				entry.GroupTag.Value = data.GroupTag;	// set the group tag
				entry.HandleDataHigh.Value = (int)((data.Hash >> 32) & 0xFFFFFFFF);

				lookup.Add(data.Hash, entry);			// add entry to the dictionary
				return entry.Flags.Value = index;
			}

			return entry.Flags.Value;
		}

		protected override int AddDependent(HandleData tag, HandleData dependent_tag)
		{
			int index = Add(dependent_tag); // add and\or get the index of the child id entry

			var tag_entry = lookup[tag.Hash];
			var dtag_entry = lookup[dependent_tag.Hash];

			tag_database_entry_reference_block eref;
			dtag_entry.ReferenceIds.Add(out eref); // add a new entry reference to the dependent tag which points to this tag
			eref.EntryReference.Value = tag_entry.Flags.Value;

			tag_entry.ChildIds.Add(out eref); // add a new entry reference to this tag which points to the dependent tag

			return eref.EntryReference.Value = index;
		}
	};

	public class CacheTagDatabase : Managers.CacheTagDatabase
	{
		CacheFile cacheFile;

		System.Collections.Generic.Dictionary<DatumIndex, tag_database_entry_block> lookup = 
			new System.Collections.Generic.Dictionary<DatumIndex, tag_database_entry_block>(DatumIndex.kEqualityComparer);

		#region Definition
		tag_database_group db = new tag_database_group();
		/// <summary>
		/// Database tag definition
		/// </summary>
		public override TI.Definition Definition { get { return db; } }
		#endregion

		/// <summary>
		/// Get a entry in the database
		/// </summary>
		/// <param name="hash">Hash value for tag entry we wish to get</param>
		/// <returns></returns>
		public tag_database_entry_block this[int hash] { get { return db.Entries[hash]; } }

		public CacheTagDatabase(CacheFile cf) : base() { cacheFile = cf; }
		public CacheTagDatabase(CacheFile cf, Blam.CacheIndex.Item root_tag) : base(root_tag) { cacheFile = cf; }

		public override int Add(Blam.CacheIndex.Item tag)
		{
			int index = -1;

			tag_database_entry_block entry;
			if (!lookup.TryGetValue(tag.Datum, out entry)) // hey, not in the database
			{
				index = db.Entries.Count;				// count will be the index of our new entry
				db.Entries.Add(out entry);				// add a new entry
				// set the name
				string name = cacheFile.GetReferenceName(tag);
				entry.Name.Value = new byte[name.Length + 1];
				System.Text.Encoding.ASCII.GetBytes(name, 0, name.Length, entry.Name.Value, 0);
				entry.GroupTag.Value = tag.GroupTag.Tag;// set the group tag
				entry.HandleDataHigh.Value = name.GetHashCode();

				lookup.Add(tag.Datum, entry);			// add entry to the dictionary
				return entry.Flags.Value = index;
			}

			return entry.Flags.Value;
		}

		public override int AddDependent(Blam.CacheIndex.Item tag, Blam.CacheIndex.Item dependent_tag)
		{
			int index = Add(dependent_tag); // add and\or get the index of the child id entry

			var tag_entry = lookup[tag.Datum];
			var dtag_entry = lookup[dependent_tag.Datum];

			tag_database_entry_reference_block eref;
			dtag_entry.ReferenceIds.Add(out eref); // add a new entry reference to the dependent tag which points to this tag
			eref.EntryReference.Value = tag_entry.Flags.Value;

			tag_entry.ChildIds.Add(out eref); // add a new entry reference to this tag which points to the dependent tag

			return eref.EntryReference.Value = index;
		}
	};

	public class ErrorTagDatabase : Managers.ErrorTagDatabase
	{
		System.Collections.Generic.Dictionary<Entry, tag_database_entry_block> lookup = 
			new System.Collections.Generic.Dictionary<Entry, tag_database_entry_block>(Entry.kEqualityComparer);

		#region Definition
		tag_database_group db = new tag_database_group();
		/// <summary>
		/// Database tag definition
		/// </summary>
		public override TI.Definition Definition { get { return db; } }
		#endregion

		/// <summary>
		/// Get a entry in the database
		/// </summary>
		/// <param name="hash">Hash value for tag entry we wish to get</param>
		/// <returns></returns>
		public tag_database_entry_block this[int hash] { get { return db.Entries[hash]; } }

		/// <summary>
		/// Hack just for Stubbs
		/// </summary>
		/// <param name="engine"></param>
		internal ErrorTagDatabase(BlamVersion engine) : base(engine) { }

		public ErrorTagDatabase() : base(BlamVersion.Halo1) { }

		public override int Add(string path, uint group_tag, ErrorFlags flags)
		{
			int index = -1;

			Entry e = new Entry(path, group_tag);
			tag_database_entry_block entry;
			if (!lookup.TryGetValue(e, out entry)) // hey, not in the database
			{
				index = db.Entries.Count;	// count will be the index of our new entry
				index = (((ushort)flags) << 16) | (index & 0xFFFF);

				db.Entries.Add(out entry);	// add a new entry
				// set the name
				entry.Name.Value = new byte[e.Path.Length + 1];
				System.Text.Encoding.ASCII.GetBytes(e.Path, 0, e.Path.Length, entry.Name.Value, 0);
				entry.GroupTag.Value = TI.TagGroup.FromUInt(e.GroupTag, false); // set the group tag
				entry.HandleDataHigh.Value = e.Path.GetHashCode();

				lookup.Add(e, entry);		// add entry to the dictionary
				return (entry.Flags.Value = index) & 0xFFFF;
			}

			return entry.Flags.Value & 0xFFFF;
		}

		public override int AddDependent(string path, uint group_tag, ErrorFlags flags)
		{
			Entry e = new Entry(path, group_tag);
			int index = Add(path, group_tag, flags); // add and\or get the index of the child id entry

			// In error databases, we only track the problematic child ids, nothing more, nothing less
			tag_database_entry_reference_block eref;
			lookup[root].ChildIds.Add(out eref); // add a new entry reference

			eref.EntryReference.Value = index; // set the entry index

			return index;
		}

		public override bool Contains(string path, uint group_tag)
		{
			return lookup.ContainsKey(new Entry(path, group_tag));
		}
	};
}