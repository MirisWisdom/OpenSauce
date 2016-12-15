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
	/// Interface for objects which are compatible for addition to a <see cref="ReferenceManager"/> instance
	/// </summary>
	/// <seealso cref="TagInterface.TagReference"/>
	public interface IReferenceMangerObject
	{
		/// <summary>
		/// The reference id of this object
		/// </summary>
		Blam.DatumIndex ReferenceId { get; }

		/// <summary>
		/// If the implementing class has an owner object, and it has an associated reference id, 
		/// this returns that value
		/// </summary>
		Blam.DatumIndex ParentReferenceId { get; }

		/// <summary>
		/// If the reference object must contain multiple reference ids, it must implement this 
		/// method to expose those ids to the system
		/// </summary>
		/// <returns></returns>
		/// <remarks><see cref="ReferenceId"/> shouldn't occur in the enumeration</remarks>
		IEnumerable<Blam.DatumIndex> GetReferenceIdEnumerator();

		/// <summary>
		/// Update this object's reference (or one of them if multiple are contained) value with a new one
		/// </summary>
		/// <param name="manager">Manager making this call to update</param>
		/// <param name="new_datum">New reference value</param>
		/// <returns>True if update was successful, false otherwise.</returns>
		bool UpdateReferenceId(ReferenceManager manager, Blam.DatumIndex new_datum);
	};

	/// <summary>
	/// Class for managing operations associated with a pair consisting of a <see cref="BlamLib.TagInterface.TagGroup"/> 
	/// and a string. The pair make up a 'reference name' handle. The handle is the group_tag value and the tag name value 
	/// of a tag reference.
	/// </summary>
	/// <remarks>A tag name is really just the tag's path (without an extension)</remarks>
	[IO.Class((int)IO.TagGroups.Enumerated.ReferenceManagerFile, 1)]
	public sealed class ReferenceManager : IO.FileManageable, IEnumerable<Blam.DatumIndex>, ICloneable
	{
		#region Name
		/// <summary>
		/// Max characters we use when streaming <see cref="Name"/>
		/// </summary>
		const int kIONameLength = 64;

		string name;
		/// <summary>
		/// Name of this manager
		/// </summary>
		public string Name	{ get { return name; } }
		#endregion

		#region Engine
		BlamVersion engine;
		/// <summary>
		/// Which engine this manager is for
		/// </summary>
		public BlamVersion Engine	{ get { return engine; } }
		#endregion

		#region IsReadOnly
		bool isReadOnly;
		/// <summary>
		/// Is this reference manger read only?
		/// </summary>
		/// <remarks>If it is read only, we can't perform any renaming, but we can add\remove</remarks>
		public bool IsReadOnly	{ get { return isReadOnly; } }
		#endregion

		#region implementation fields
		// The way this works is that we are provided a list of group tags to work with.
		// Then we keep track of tag names in a list.
		// There can be multiple instances of the same tag name, BUT there 
		// can be only one group tag matched to each one of those names.
		//
		// In the case of adding, we'll check to see if a group tag \ tag name
		// pair exists, and return that. Else we create a new pair and return that.
		//
		// In the case of removing a tag from our tracking, it only removes
		// the path matched with the specified tag group when calling Remove.
		//
		// In the case of renaming a reference, we'll just create a new handle that uses
		// the new name's index, while removing the old name from the list, then loop 
		// through our reference tracker and propagate our update to the tracked child objects

		bool customGroupTags = false;
		/// <summary>
		/// List of supported tag groups for our references
		/// </summary>
		TagInterface.TagGroupCollection groupTags;
		/// <summary>
		/// List of tag names
		/// </summary>
		/// <remarks>These do not include file extensions</remarks>
		List<string> referencePaths = new List<string>();
		/// <summary>
		/// Indexes to <see cref="referencePaths"/> which are unused
		/// </summary>
		List<int> unusedPaths = new List<int>();

		/// <summary>
		/// List of references used by this manager
		/// </summary>
		List<Blam.DatumIndex> references = new List<BlamLib.Blam.DatumIndex>();
		/// <summary>
		/// Hashtable for lists of objects that link to references kept by this manager
		/// </summary>
		/// <remarks>
		/// TKey:	reference-name datum
		/// TValue:	List of objects which reference the associated TKey
		/// </remarks>
		Dictionary<Blam.DatumIndex, List<IReferenceMangerObject>> referencers = 
			new Dictionary<BlamLib.Blam.DatumIndex, List<IReferenceMangerObject>>(Blam.DatumIndex.kEqualityComparer);


		/// <summary>
		/// Get the list of supported group tags for use in this reference manager
		/// </summary>
		/// <returns></returns>
		public TagInterface.TagGroupCollection GetGroupTags()
		{
			return groupTags;
		}
		#endregion

		#region Count
		/// <summary>
		/// How many references this manager contains
		/// </summary>
		public int Count	{ get { return references.Count; } }
		#endregion

		#region Ctor
		/// <summary>
		/// Create reference manager
		/// </summary>
		/// <remarks>Used for reading a manager from a file</remarks>
		internal ReferenceManager() : base() {}

		/// <summary>
		/// Construct an empty reference manager
		/// </summary>
		/// <param name="engine">Target engine this manager is for</param>
		/// <param name="manager_name">Name of this manager</param>
		/// <param name="is_read_only">Value indicating whether or not we can perform any renaming operations</param>
		public ReferenceManager(BlamVersion engine, string manager_name, bool is_read_only) : base()
		{
			this.engine = engine;
			this.name = manager_name;
			this.isReadOnly = is_read_only;

			groupTags = engine.VersionToTagCollection();
		}

		/// <summary>
		/// Construct an empty reference manager, with explicit supported group tags
		/// </summary>
		/// <param name="engine">Target engine this manager is for</param>
		/// <param name="manager_name">Name of this manager</param>
		/// <param name="is_read_only">Value indicating whether or not we can perform any renaming operations</param>
		/// <param name="group_tags">Supported group tags</param>
		public ReferenceManager(BlamVersion engine, string manager_name, bool is_read_only, params TagInterface.TagGroup[] group_tags) : base()
		{
			this.engine = engine;
			this.name = manager_name;
			this.isReadOnly = is_read_only;

			customGroupTags = true;
			// we don't want to sort, in case the user is using a pre-defined group list
			// and not actually a params listing of group tags
			groupTags = new BlamLib.TagInterface.TagGroupCollection(false, group_tags);
		}

		/// <summary>
		/// Clone ctor
		/// </summary>
		/// <param name="other">object to copy from</param>
		ReferenceManager(ReferenceManager other) : base()
		{
			this.engine = other.engine;
			this.name = other.name;
			this.isReadOnly = other.isReadOnly;
			this.customGroupTags = other.customGroupTags;
			this.groupTags = other.groupTags;

			if (other.references.Count > 0)
			{
				this.referencePaths.AddRange(other.referencePaths);
				if (other.unusedPaths.Count > 0) this.unusedPaths.AddRange(other.unusedPaths);
				this.references.AddRange(other.references);
			}
		}

		/// <summary>
		/// Clone this ReferenceManager's data
		/// </summary>
		/// <returns></returns>
		/// <remarks>Copies everything except <see cref="IReferenceMangerObject"/> related data</remarks>
		public object Clone()	{ return new ReferenceManager(this); }
		#endregion

		#region Util
		#region Datum manipulation
		/// <summary>
		/// Construct a handle to a group_tag\path pair
		/// </summary>
		/// <param name="group_tag_index">Index to an object in <see cref="groupTags"/></param>
		/// <param name="path_index">Index to an object in <see cref="referencePaths"/></param>
		/// <returns>Salt = <paramref name="group_tag_index"/>. Index = <paramref name="path_index"/></returns>
		static Blam.DatumIndex MakeDatum(int group_tag_index, int path_index)
		{
			return new Blam.DatumIndex((ushort)(path_index & 0x7FFF), (short)(group_tag_index & 0x7FFF));
		}
		/// <summary>
		/// Get the group tag index component of a reference handle
		/// </summary>
		/// <param name="datum"></param>
		/// <returns></returns>
		static int DatumToGroupTagIndex(Blam.DatumIndex datum)			{ return datum.Salt; }
		/// <summary>
		/// Get the path index component of a reference handle
		/// </summary>
		/// <param name="datum"></param>
		/// <returns></returns>
		static int DatumToPathIndex(Blam.DatumIndex datum)				{ return datum.Index; }
		/// <summary>
		/// Get the <see cref="BlamLib.TagInterface.TagGroup"/> object that the handle 
		/// <paramref name="datum"/> uses
		/// </summary>
		/// <param name="datum"></param>
		/// <returns></returns>
		TagInterface.TagGroup DatumToGroupTag(Blam.DatumIndex datum)	{ return groupTags[DatumToGroupTagIndex(datum)]; }
		/// <summary>
		/// Get the path that the handle <paramref name="datum"/> uses
		/// </summary>
		/// <param name="datum"></param>
		/// <returns></returns>
		string DatumToPath(Blam.DatumIndex datum)						{ return referencePaths[DatumToPathIndex(datum)]; }
		#endregion

		/// <summary>
		/// If the reference datum is valid, gets its <see cref="TagInterface.TagGroup"/> or else 
		/// returns <see cref="TagInterface.TagGroup.Null"/>
		/// </summary>
		/// <param name="datum">reference datum in question</param>
		/// <returns></returns>
		public TagInterface.TagGroup GetTagGroup(Blam.DatumIndex datum)
		{
			if (IsValidHandle(datum))
				return DatumToGroupTag(datum);

			return TagInterface.TagGroup.Null;
		}

		#region TryAndFind
		/// <summary>
		/// Try and find the index of a reference datum based on a group_tag index and name value
		/// </summary>
		/// <param name="group_tag_index"></param>
		/// <param name="reference_name"></param>
		/// <returns>The reference datum which equals the input arguments or -1 if one isn't found</returns>
		int TryAndFindOptimized(int group_tag_index, string reference_name)
		{
			return references.FindIndex(di =>
					DatumToGroupTagIndex(di) == group_tag_index &&
					referencePaths[DatumToPathIndex(di)] == reference_name
				);
		}
		/// <summary>
		/// Searches for a matching group_tag\path pair
		/// </summary>
		/// <param name="group_tag_index">Index to an object in <see cref="groupTags"/>, which is the group tag value for the pair</param>
		/// <param name="reference_name">Path value for the pair</param>
		/// <param name="datum">Handle for the group_tag\path pair. <see cref="Blam.DatumIndex.Null"/> if not found.</param>
		/// <returns>True if the a matching pair was found, false if one wasn't</returns>
		/// <remarks>Performs the least amount of validation compared to the regular <see cref="TryAndFind"/> methods</remarks>
		bool TryAndFindOptimized(int group_tag_index, string reference_name, out Blam.DatumIndex datum)
		{
			// We can't use this code as referencePaths is not a string Set, it can contain multiple 
			// of the same name. It is however, a Set when viewed with respected group_tags
// 			int index = referencePaths.BinarySearch(reference_name);
// 			if (index >= 0) // validate the reference path exists
// 			{
// 				datum = MakeDatum(group_tag_index, index);
// 				if (IsValidHandle(datum)) // validate the actual group\path pair exists
// 					return true;
// 			}

			int index = TryAndFindOptimized(group_tag_index, reference_name);
			if(index != -1)
			{
				datum = references[index];
				return true;
			}

			datum = Blam.DatumIndex.Null;

			return false;
		}
		/// <summary>
		/// Searches for a matching group_tag\path pair
		/// </summary>
		/// <param name="group_tag_index">Index to an object in <see cref="groupTags"/>, which is the group tag value for the pair</param>
		/// <param name="reference_name">Path value for the pair</param>
		/// <param name="datum">Handle for the group_tag\path pair. <see cref="Blam.DatumIndex.Null"/> if not found.</param>
		/// <returns>True if the a matching pair was found, false if one wasn't</returns>
		internal bool TryAndFind(int group_tag_index, string reference_name, out Blam.DatumIndex datum)
		{
			if (group_tag_index >= 0 && group_tag_index < groupTags.Count) // at least validate the index first
				return TryAndFindOptimized(group_tag_index, reference_name, out datum);

			datum = Blam.DatumIndex.Null;

			return false;
		}
		/// <summary>
		/// Searches for a matching group_tag\path pair
		/// </summary>
		/// <param name="group_tag">Group tag value for the pair</param>
		/// <param name="reference_name">Path value for the pair</param>
		/// <param name="datum">Handle for the group_tag\path pair <see cref="Blam.DatumIndex.Null"/> if not found.</param>
		/// <returns>True if the a matching pair was found, false if one wasn't</returns>
		public bool TryAndFind(TagInterface.TagGroup group_tag, string reference_name, out Blam.DatumIndex datum)
		{
			return TryAndFind(groupTags.FindGroupIndex(group_tag), reference_name, out datum);
		}
		#endregion

		/// <summary>
		/// Determines if <paramref name="datum"/> is a valid handle used to track a reference
		/// </summary>
		/// <param name="datum"></param>
		/// <returns></returns>
		bool IsValidHandle(Blam.DatumIndex datum)
		{
			return datum != Blam.DatumIndex.Null &&
				references.Contains(datum);
		}

		/// <summary>
		/// Copy the group_tag\path pair referenced by <paramref name="datum"/> from <paramref name="old"/> to <paramref name="new"/>
		/// </summary>
		/// <param name="old">Manager that contains <paramref name="datum"/></param>
		/// <param name="new">Manager to copy <paramref name="datum"/> to</param>
		/// <param name="datum">Handle to the group_tag\path pair</param>
		/// <returns>Handle to the copied group_tag\path pair in <paramref name="new"/></returns>
		/// <remarks>
		/// If <paramref name="old"/> and <paramref name="new"/> are the same manager, <paramref name="datum"/> is just returned.
		/// 
		/// If the group_tag doesn't exist in <paramref name="new"/>, an exception is thrown
		/// </remarks>
		internal static Blam.DatumIndex CopyHandle(ReferenceManager old, ReferenceManager @new, Blam.DatumIndex datum)
		{
			if (old == @new) return datum;

			int group_tag_index = @new.groupTags.FindGroupIndex(old.groupTags[DatumToGroupTagIndex(datum)]);

			if (group_tag_index == -1) throw new Debug.ExceptionLog("Failed to copy handle! {0}\t{1} to {2}:\t{3}",
				"Group tag doesn't exist! ", old.Name, @new.Name, datum);

			datum = @new.Add(group_tag_index, old[datum]);

			return datum;
		}

		/// <summary>
		/// Copy the group_tag\path pair referenced by <paramref name="datum"/> from <paramref name="old"/> to <paramref name="new"/>
		/// </summary>
		/// <param name="obj">Object which references the group_tag\path pair</param>
		/// <param name="old">Manager that contains <paramref name="datum"/></param>
		/// <param name="new">Manager to copy <paramref name="datum"/> to</param>
		/// <param name="datum">Handle to the group_tag\path pair</param>
		/// <returns>Handle to the copied group_tag\path pair in <paramref name="new"/></returns>
		/// <remarks>
		/// If <paramref name="old"/> and <paramref name="new"/> are the same manager, <paramref name="datum"/> is just returned.
		/// 
		/// If the group_tag doesn't exist in <paramref name="new"/>, an exception is thrown
		/// </remarks>
		internal static Blam.DatumIndex CopyHandle(IReferenceMangerObject obj, ReferenceManager old, ReferenceManager @new, Blam.DatumIndex datum)
		{
			if (old == @new) return datum;

			int group_tag_index = @new.groupTags.FindGroupIndex(old.groupTags[DatumToGroupTagIndex(datum)]);

			if (group_tag_index == -1) throw new Debug.ExceptionLog("Failed to copy handle! {0}\t{1} to {2}:\t{3}",
				"Group tag doesn't exist! ", old.Name, @new.Name, datum);

			datum = @new.Add(obj, group_tag_index, old[datum]);

			return datum;
		}

		/// <summary>
		/// Find the group_tag\path pairs in this manager that also exist in 
		/// <paramref name="other"/>
		/// </summary>
		/// <param name="other"></param>
		/// <returns>List of references to *this* manager</returns>
		internal List<Blam.DatumIndex> FindSharedReferencesFromEqualManager(ReferenceManager other)
		{
			// we set the count to our ref count since thats the most we can EXPECT the found 
			// sharing refs to be
			List<Blam.DatumIndex> refs = new List<BlamLib.Blam.DatumIndex>(Count);

			Blam.DatumIndex found;
			foreach (Blam.DatumIndex di in other.references)
				if (TryAndFindOptimized(DatumToGroupTagIndex(di), other.DatumToPath(di), out found))
					refs.Add(found);

			refs.TrimExcess();
			return refs;
		}
		#endregion

		/// <summary>
		/// Retrieve the reference's path value
		/// </summary>
		/// <param name="handle">Handle to a group_tag\path pair</param>
		/// <returns>Path associated with <paramref name="handle"/> or <see cref="String.Empty"/> if it was invalid</returns>
		public string this[Blam.DatumIndex handle]
		{
			get {
				if(IsValidHandle(handle))	return referencePaths[DatumToPathIndex(handle)];

				return string.Empty;
			}
		}

		#region Add
		/// <summary>
		/// Adds <paramref name="obj"/> to <paramref name="datum"/>'s tracking list
		/// </summary>
		/// <param name="datum">Handle to a group_tag\path pair</param>
		/// <param name="obj">Object to add to the tracking list</param>
		internal void AddReferencer(Blam.DatumIndex datum, IReferenceMangerObject obj)
		{
			List<IReferenceMangerObject> refs;

			if (!referencers.TryGetValue(datum, out refs)) // validate that we're were already tracking things for the datum first
			{
				refs = new List<IReferenceMangerObject>();
				referencers.Add(datum, refs); // we weren't, so setup the entry

				refs.Add(obj);
			}
			else
			{
				if (!refs.Contains(obj)) // just in case some smart ass is trying to add it twice
					refs.Add(obj);
			}
		}

		/// <summary>
		/// Adds <paramref name="reference_name"/> to the paths list
		/// </summary>
		/// <param name="reference_name"></param>
		/// <returns>Index to <paramref name="reference_name"/> position in the paths list</returns>
		/// <remarks>Reuses any unused path indices</remarks>
		int AddReferenceName(string reference_name)
		{
			int index = referencePaths.Count;

			if (unusedPaths.Count > 0) // recycle path elements
			{
				index = unusedPaths[unusedPaths.Count - 1]; // get the last unused path index
				unusedPaths.RemoveAt(unusedPaths.Count - 1); // it won't be unused soon, so remove it from list

				referencePaths[index] = reference_name;
			}
			else // if the 'if' statement failed, our index will equal the index to which this will be located at
				referencePaths.Add(reference_name);

			return index;
		}

		/// <summary>
		/// Adds <paramref name="reference_name"/> to the paths list
		/// </summary>
		/// <param name="reference_name"></param>
		/// <returns>Index to <paramref name="reference_name"/> position in the paths list</returns>
		/// <remarks>Checks if the name already exists, and if so, reuses the index for the existing path</remarks>
		[Obsolete] // We shouldn't be reusing reference names since only one-per-group_tag can exist
		int AddReferenceNameWithReuse(string reference_name)
		{
			int index = referencePaths.BinarySearch(reference_name);

			if (index >= 0) return index;
			else return AddReferenceName(reference_name);
		}

		/// <summary>
		/// Add a group_tag\path pair to the manager
		/// </summary>
		/// <param name="group_tag_index">Index to an object in <see cref="groupTags"/></param>
		/// <param name="reference_name">Path value for the pair</param>
		/// <returns>Handle to the group_tag\path pair entry, or <see cref="Blam.DatumIndex.Null"/> if the operation failed</returns>
		/// <remarks>
		/// We don't add anything to <see cref="referencers"/> because nothing is tracked yet and it just wastes memory initializing the list here.
		/// 
		/// This will try to find an existing reference first, and return the handle to that if one exists, else a new one is created and added 
		/// to our tracking list.
		/// </remarks>
		internal Blam.DatumIndex Add(int group_tag_index, string reference_name)
		{
			if (group_tag_index == -1) throw new Debug.ExceptionLog(
				new ArgumentOutOfRangeException("group_tag_index"),
				"Tried to use an invalid tag group while adding a reference to '{0}' in '{1}'.", reference_name, name);
			if (string.IsNullOrEmpty(reference_name)) throw new Debug.ExceptionLog(
				new ArgumentException("reference_name"),
				"Tried to use an invalid reference name for a {0} tag in '{1}'.", groupTags[group_tag_index].Name, name);
			//Debug.Assert.If(group_tag_index != -1, "Tried to use an invalid tag group while adding a reference to '{0}' in '{1}'.", reference_name, name);
			//Debug.Assert.If(reference_name != null && reference_name != "", "Tried to use an invalid reference name for a {0} tag in '{1}'.", groupTags[group_tag_index].Name, name);

			Blam.DatumIndex datum = Blam.DatumIndex.Null;

			if (group_tag_index >= 0 && group_tag_index < groupTags.Count) // at least validate the index first
			{
				if (TryAndFind(group_tag_index, reference_name, out datum)) // does the reference already exist?
					return datum;

				datum = MakeDatum(group_tag_index, AddReferenceName(reference_name));

				references.Add(datum); // add the new datum to our datum tracking list
			}

			return datum;
		}
		/// <summary>
		/// Add the group_tag\path pair to the manager without validating that an existing handle does or doesn't exist already
		/// </summary>
		/// <param name="group_tag_index">Index to an object in <see cref="groupTags"/></param>
		/// <param name="reference_name">Path value for the pair</param>
		/// <returns>Handle to the group_tag\path pair entry, or <see cref="Blam.DatumIndex.Null"/> if the operation failed</returns>
		/// <remarks>This useful and faster in conditions when it is known that multiple group_tag\path pairs can't be added</remarks>
		internal Blam.DatumIndex AddOptimized(int group_tag_index, string reference_name)
		{
			if (group_tag_index == -1) throw new Debug.ExceptionLog(
				new ArgumentOutOfRangeException("group_tag_index"),
				"Tried to use an invalid tag group while adding a reference to '{0}' in '{1}'.", reference_name, name);
			if (string.IsNullOrEmpty(reference_name)) throw new Debug.ExceptionLog(
				new ArgumentException("reference_name"),
				"Tried to use an invalid reference name for a {0} tag in '{1}'.", groupTags[group_tag_index].Name, name);
			//Debug.Assert.If(group_tag_index != -1, "Tried to use an invalid tag group while adding a reference to '{0}' in '{1}'.", reference_name, name);
			//Debug.Assert.If(reference_name != null && reference_name != "", "Tried to use an invalid reference name for a {0} tag in '{1}'.", groupTags[group_tag_index].Name, name);

			Blam.DatumIndex datum = Blam.DatumIndex.Null;

			if (group_tag_index >= 0 && group_tag_index < groupTags.Count) // at least validate the index first
			{
				datum = MakeDatum(group_tag_index, AddReferenceName(reference_name));

				references.Add(datum); // add the new datum to our datum tracking list
			}

			return datum;
		}
		/// <summary>
		/// Add a object to the group_tag\path's tracking list
		/// </summary>
		/// <param name="obj">Object which references the group_tag\path pair</param>
		/// <param name="group_tag_index">Index to an object in <see cref="groupTags"/></param>
		/// <param name="reference_name">Path value for the pair</param>
		/// <returns>Handle to the group_tag\path pair entry, or <see cref="Blam.DatumIndex.Null"/> if the operation failed</returns>
		internal Blam.DatumIndex Add(IReferenceMangerObject obj, int group_tag_index, string reference_name)
		{
			Blam.DatumIndex datum = Add(group_tag_index, reference_name);

			// Perform this assertion here in case the other two parameters are fubar'd, in which case, the other Add will handle their sanity
			if(obj == null) throw new Debug.ExceptionLog(
				new ArgumentNullException("obj"),
				"Tried to add a reference to the {0} tag '{1}' with a null object in '{2}'.", groupTags[group_tag_index].Name, reference_name, name);
			//Debug.Assert.If(obj != null, "Tried to add a reference to the {0} tag '{1}' with a null object in '{2}'.", groupTags[group_tag_index].Name, reference_name, name);
			AddReferencer(datum, obj);

			return datum;
		}


		/// <summary>
		/// Add a group_tag\path pair to the manager
		/// </summary>
		/// <param name="group_tag">Group tag value for the pair</param>
		/// <param name="reference_name">Path value for the pair</param>
		/// <returns>Handle to the group_tag\path pair entry, or <see cref="Blam.DatumIndex.Null"/> if the operation failed</returns>
		public Blam.DatumIndex Add(TagInterface.TagGroup group_tag, string reference_name)								{ return Add(groupTags.FindGroupIndex(group_tag), reference_name); }

		/// <summary>
		/// Add the group_tag\path pair to the manager without validating that an existing handle does or doesn't exist already
		/// </summary>
		/// <param name="group_tag">Group tag value for the pair</param>
		/// <param name="reference_name">Path value for the pair</param>
		/// <returns>Handle to the group_tag\path pair entry, or <see cref="Blam.DatumIndex.Null"/> if the operation failed</returns>
		/// <remarks>This useful and faster in conditions when it is known that multiple group_tag\path pairs can't be added</remarks>
		internal Blam.DatumIndex AddOptimized(TagInterface.TagGroup group_tag, string reference_name)					{ return AddOptimized(groupTags.FindGroupIndex(group_tag), reference_name); }
		/// <summary>
		/// Add a object to the group_tag\path's tracking list
		/// </summary>
		/// <param name="obj">Object which references the group_tag\path pair</param>
		/// <param name="group_tag">Group tag value for the pair (represented in big endian)</param>
		/// <param name="reference_name">Path value for the pair</param>
		/// <returns>Handle to the group_tag\path pair entry, or <see cref="Blam.DatumIndex.Null"/> if the operation failed</returns>
		internal Blam.DatumIndex Add(IReferenceMangerObject obj, uint group_tag, string reference_name)					{ return Add(obj, groupTags.FindGroupIndex(group_tag), reference_name); }
		/// <summary>
		/// Add a object to the group_tag\path's tracking list
		/// </summary>
		/// <param name="obj">Object which references the group_tag\path pair</param>
		/// <param name="group_tag">Group tag value for the pair</param>
		/// <param name="reference_name">Path value for the pair</param>
		/// <returns>Handle to the group_tag\path pair entry, or <see cref="Blam.DatumIndex.Null"/> if the operation failed</returns>
		public Blam.DatumIndex Add(IReferenceMangerObject obj, TagInterface.TagGroup group_tag, string reference_name)	{ return Add(obj, groupTags.FindGroupIndex(group_tag), reference_name); }
		#endregion

		#region Remove
		/// <summary>
		/// Clears this manager of all references and their handles
		/// </summary>
		public void Clear()
		{
			referencePaths.Clear();
			unusedPaths.Clear();
			references.Clear();
			referencers.Clear();
		}

		/// <summary>
		/// Marks the path at index <paramref name="path_index"/> unused
		/// </summary>
		/// <param name="path_index"></param>
		void DeleteReference(int path_index)
		{
			// We don't use the following code anymore due to the fact that each pair has its own dedicated path index
// 			foreach (Blam.DatumIndex d in references)
// 				if (DatumToPathIndex(d) == path_index) // reference is still being used, don't remove from the list
// 					return;
			unusedPaths.Add(path_index); // update our unused listing with this since nothing is no longer using this index
			referencePaths[path_index] = null; // just to be safe
		}

		/// <summary>
		/// Delete the data associated with <paramref name="datum"/> from this manager
		/// </summary>
		/// <param name="datum">Reference Id value. Can be <see cref="Blam.DatumIndex.Null"/></param>
		public void Delete(Blam.DatumIndex datum)
		{
			if (datum == Blam.DatumIndex.Null) return;

			DeleteReference(DatumToPathIndex(datum));
			referencers.Remove(datum); // remove all reference objects linked to this datum
			references.Remove(datum); // remove the datum from the tracking list
		}

		/// <summary>
		/// Remove all references tracked with <paramref name="datum"/>
		/// </summary>
		/// <param name="datum">Reference Id value. Can be <see cref="Blam.DatumIndex.Null"/></param>
		/// <remarks><paramref name="datum"/> will continue to be a valid handle, just all references will be nulled out</remarks>
		public void Remove(Blam.DatumIndex datum)
		{
			if (datum == Blam.DatumIndex.Null) return;

			referencers[datum].Clear();
		}

		/// <summary>
		/// Stop tracking of <paramref name="obj"/>'s reference
		/// </summary>
		/// <param name="obj">Object supporting reference management. Can be null</param>
		public void Remove(IReferenceMangerObject obj)
		{
			if (obj == null) return;

			if (obj.ReferenceId != Blam.DatumIndex.Null) // remove the main reference
				referencers[obj.ReferenceId].Remove(obj);

			foreach(Blam.DatumIndex di in obj.GetReferenceIdEnumerator()) // remove any secondary references
				if (di != Blam.DatumIndex.Null) referencers[di].Remove(obj);
		}

		/// <summary>
		/// Remove any unused <see cref="IReferenceMangerObject"/> objects which may have failed to unlink 
		/// when their owner object was disposed of
		/// </summary>
		public void GarbageCollect()
		{
			foreach (List<IReferenceMangerObject> value in referencers.Values)
			{
				for(int x = 0; x < value.Count; )
				{
					if (!referencers.ContainsKey(value[x].ParentReferenceId))
						value.RemoveAt(x);
					else
						x++;
				}
			}
		}
		#endregion

		#region Change
		/// <summary>
		/// Updates the objects linked to <paramref name="datum"/> to now link to <paramref name="new_reference"/>
		/// </summary>
		/// <param name="datum">Datum handle to the reference in question</param>
		/// <param name="new_reference">The reference name to rename to</param>
		/// <returns>Whether or not the entire rename operation succeeded</returns>
		bool RenameReference(Blam.DatumIndex datum, Blam.DatumIndex new_reference)
		{
			bool result = true;

			List<IReferenceMangerObject> list;
			if (referencers.TryGetValue(datum, out list)) // [datum] may not be tracked by anything right now, so don't change [result]
			{
				foreach (IReferenceMangerObject obj in list)
					if(!obj.UpdateReferenceId(this, new_reference))
						result = false;
			}

			return result;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="datum">Datum handle to the reference in question</param>
		/// <param name="new_reference">The reference name to rename to</param>
		/// <returns>Whether or not the rename operation succeeded</returns>
		bool RenameImpl(Blam.DatumIndex datum, Blam.DatumIndex new_reference)
		{
			bool result = RenameReference(datum, new_reference);

			#region merging action
			// take the old reference objects for [datum] and place them in the [new_reference]'s objects list
			// afterwards, remove [datum] from tracking
			List<IReferenceMangerObject> list, new_list;
			if (result && referencers.TryGetValue(datum, out list))
			{
				if (!referencers.TryGetValue(new_reference, out new_list)) // [new_reference] may not have had anything tracking it before this change, so initialize the list
				{
					new_list = new List<IReferenceMangerObject>(list.Count); // new_list will have *at least* [Count] objects so save some CPU
					referencers.Add(new_reference, new_list);
				}

				new_list.AddRange(list);
				Delete(datum);
			}
			#endregion

			return result;
		}

		bool RenameValidateArguments(Blam.DatumIndex datum)
		{
			bool result = true;

			if (isReadOnly)
			{
				Debug.Warn.If(false, "Tried to rename a reference in the read-only manager '{0}'.", name);
				result = false;
			}
			else if (datum == Blam.DatumIndex.Null)
			{
				Debug.Warn.If(false, "Tried to rename an invalid reference in '{0}'.", name);
				result = false;
			}

			return result;
		}

		/// <summary>
		/// Updates the objects linked to <paramref name="datum"/> to now link to <paramref name="new_reference_name"/> 
		/// of the <paramref name="group_tag"/> group
		/// </summary>
		/// <param name="datum">Datum handle to the reference in question</param>
		/// <param name="group_tag_index"><paramref name="new_reference_name"/>'s group tag's index</param>
		/// <param name="new_reference_name">The reference name to rename to</param>
		/// <returns>Whether or not the rename operation succeeded</returns>
		/// <remarks>
		/// Doesn't perform any group tag compatibility checking like it's public method counterpart
		/// </remarks>
		internal bool Rename(Blam.DatumIndex datum, int group_tag_index, string new_reference_name)
		{
			if (!RenameValidateArguments(datum))
				return false;

			Blam.DatumIndex new_datum = Add(group_tag_index, new_reference_name);

			return RenameReference(datum, new_datum);
		}

		/// <summary>
		/// Updates the objects linked to <paramref name="datum"/> to now link to <paramref name="new_reference_name"/> 
		/// of the <paramref name="group_tag"/> group
		/// </summary>
		/// <param name="datum">Datum handle to the reference in question</param>
		/// <param name="group_tag"><paramref name="new_reference_name"/>'s group tag</param>
		/// <param name="new_reference_name">The reference name to rename to</param>
		/// <returns>Whether or not the rename operation succeeded</returns>
		public bool Rename(Blam.DatumIndex datum, TagInterface.TagGroup group_tag, string new_reference_name)
		{
			// yes, I know the Rename method we call at the end of this method checks for this, but I would rather not
			// perform all that code before the call just to turn around and say, "ohai, I'm readonly, gtfo"
			if (!RenameValidateArguments(datum))
				return false;

			int group_tag_index = groupTags.FindGroupIndex(group_tag);
			if (group_tag_index == -1)
			{
				Debug.Warn.If(false, "Tried use the group tag '{0}' which isn't valid in this manager '{1}'.", group_tag.Name, name);
				return false;
			}

			#region target and source reference compatiblity testing
			if(DatumToGroupTagIndex(datum) != group_tag_index) // is this even the exact same group?
			{
				TagInterface.TagGroup datum_group = groupTags[DatumToGroupTagIndex(datum)];
				
				if(!datum_group.IsRelative(group_tag)) // ok, is this even a compatible map to a relative group?
				{
					Debug.Warn.If(false, "Tried to rename the reference '{0}.{1}' to the {2} tag '{3}' (which is incompatible) in '{4}'.",
						referencePaths[DatumToPathIndex(datum)], datum_group.Name,
						group_tag.Name, new_reference_name,
						name);
					return false;
				}
			}
			#endregion

			return Rename(datum, group_tag_index, new_reference_name);
		}

		/// <summary>
		/// Rename the reference associated with <paramref name="datum"/> to now be named 
		/// <paramref name="new_reference_name"/>
		/// </summary>
		/// <param name="datum">Datum handle to the reference in question</param>
		/// <param name="new_reference_name">The reference name to rename to</param>
		/// <returns>Whether or not the rename operation succeeded</returns>
		public bool Rename(Blam.DatumIndex datum, string new_reference_name)		{ return Rename(datum, DatumToGroupTagIndex(datum), new_reference_name); }

		/// <summary>
		/// Updates the objects linked to <paramref name="datum"/> to link to <paramref name="new_reference"/>
		/// </summary>
		/// <param name="datum">Datum handle to the reference in question</param>
		/// <param name="new_reference">Datum handle to the new reference to rename <paramref name="datum"/> to</param>
		/// <returns>Whether or not the rename operation succeeded</returns>
		public bool Rename(Blam.DatumIndex datum, Blam.DatumIndex new_reference)
		{
			if (!RenameValidateArguments(datum))
				return false;

			if (!IsValidHandle(new_reference))
			{
				Debug.Warn.If(false, "Tried to rename a reference to the {0} tag '{1}' with a invalid new reference in '{2}'.", groupTags[DatumToGroupTagIndex(datum)], referencePaths[DatumToPathIndex(datum)], name);
				return false;
			}

			return RenameImpl(datum, new_reference);
		}
		#endregion

		#region IStreamable Members
		/// <summary>
		/// Stream reference manger data from a file
		/// </summary>
		/// <param name="s"></param>
		public override void Read(IO.EndianReader s)
		{
			int count;

			engine.Read(s);
			isReadOnly = s.ReadBool();
			customGroupTags = s.ReadBool();
			s.Seek(1 + 1, System.IO.SeekOrigin.Current);
			name = s.ReadAsciiString(kIONameLength);

			#region groupTags
			groupTags = engine.VersionToTagCollection();

			if(customGroupTags)
			{
				count = s.ReadInt32();
				uint group_tag;
				TagInterface.TagGroup[] groups = new BlamLib.TagInterface.TagGroup[count];
				for(int x = 0; x < count; x++)
				{
					group_tag = s.ReadTagUInt();
					groups[x] = groupTags.FindTagGroup(group_tag);
				}

				groupTags = new BlamLib.TagInterface.TagGroupCollection(false, groups);
			}
			#endregion

			#region unusedPaths
			count = s.ReadInt32();
			if(count > 0)
			{
				unusedPaths = new List<int>(count);
				for (int x = 0; x < count; x++)
					unusedPaths[x] = s.ReadInt32();
			}
			#endregion

			#region referencePaths
			count = s.ReadInt32();
			if (count > 0)
			{
				referencePaths = new List<string>(count);
				for (int x = 0; x < count; x++)
				{
					if (unusedPaths.Contains(x)) continue;
					referencePaths[x] = s.ReadString();
				}
			}
			#endregion

			#region references
			count = s.ReadInt32();
			if (count > 0)
			{
				references = new List<BlamLib.Blam.DatumIndex>(count);
				for (int x = 0; x < count; x++)
					references[x].Read(s);
			}
			#endregion
		}
		/// <summary>
		/// Stream reference manager data to a file
		/// </summary>
		/// <param name="s"></param>
		public override void Write(IO.EndianWriter s)
		{
			engine.Write(s);
			s.Write(isReadOnly);
			s.Write(customGroupTags);
			s.Write(ushort.MinValue);
			s.Write(name, kIONameLength);

			#region groupTags
			if (customGroupTags)
			{
				s.Write(groupTags.Count);
				groupTags.Write(s);
			}
			#endregion

			#region unusedPaths
			s.Write(unusedPaths.Count);
			if (unusedPaths.Count > 0)
				foreach (int i in unusedPaths)
					s.Write(i);
			#endregion

			#region referencePaths
			s.Write(referencePaths.Count);
			if(referencePaths.Count > 0)
				foreach (string str in referencePaths)
					// TODO: 1 byte header string. we shouldn't have any paths over 255 characters long
					if(str != null) s.Write(str); // if its a null string, its an unused path case
			#endregion

			#region references
			s.Write(references.Count);
			if (references.Count > 0)
				foreach (Blam.DatumIndex di in references)
					di.Write(s);
			#endregion
		}
		#endregion

		/// <summary>
		/// Build the debug streams used in cache files which store the tag name values
		/// </summary>
		/// <remarks>
		/// <paramref name="buffer"/> will most likely contain more bytes than needed for its data 
		/// so you may want to use <paramref name="buffer"/>.<see cref="System.IO.MemoryStream.ToArray()"/> 
		/// when writing the data to another stream. Note that ToArray returns a COPY of the underlying 
		/// byte[] so you, in some cases, may just want to use GetBuffer instead but use the 
		/// <see cref="System.IO.MemoryStream.Length"/> field (compared to <see cref="System.IO.MemoryStream.Capacity"/>) 
		/// to only write non-zero (ie, unused) bytes
		/// </remarks>
		/// <param name="offsets">Buffer containing the offsets of the string values in <paramref name="buffer"/></param>
		/// <param name="buffer">Buffer containing the string values</param>
		public void GenerateDebugStream(out System.IO.MemoryStream offsets, out System.IO.MemoryStream buffer)
		{
			int count = this.references.Count;
			offsets = new System.IO.MemoryStream(count * sizeof(int));
			buffer = new System.IO.MemoryStream(count * 256);

			using (var offsets_s = new IO.EndianWriter(offsets))
			using (var buffer_s = new IO.EndianWriter(buffer))
			{
				foreach(Blam.DatumIndex di in references)
				{
					offsets_s.Write(buffer_s.Position);
					buffer_s.Write(DatumToPath(di), true);
				}
			}
		}

		#region Output Util
		public static class OutputFlags
		{
			/// <summary>
			/// Will output all the group_tag\path pairs along with their handle values 
			/// first in the output stream
			/// </summary>
			public const uint OutputPairsList = 1 << 0;
			/// <summary>
			/// Will sort the output based on the group_tag handle value
			/// </summary>
			public const uint SortByGroupTag = 1 << 1;
		};
		const string kOutputReferenceFormat = "{0}\t{1}";

		void OutputByGroupTag(System.IO.StreamWriter s, string format, List<Blam.DatumIndex> names)
		{
			for (int x = 0; x < groupTags.Count; x++)
			{
				foreach(Blam.DatumIndex di in names) // foreach reference id...
					if(DatumToGroupTagIndex(di) == x) // find the ones which are part of the current group
						s.WriteLine(format, DatumToGroupTag(di).Name, DatumToPath(di));
			}
		}

		/// <summary>
		/// Output the references to a file on disk
		/// </summary>
		/// <param name="file_path">path on disk</param>
		/// <param name="flags"></param>
		public void Output(string file_path, uint flags)
		{
			using(System.IO.StreamWriter s = new System.IO.StreamWriter(file_path))
			{
				if(Util.Flags.Test(flags, OutputFlags.OutputPairsList))
				{
					if (Util.Flags.Test(flags, OutputFlags.SortByGroupTag))
						OutputByGroupTag(s, kOutputReferenceFormat, references);
					else
						foreach (Blam.DatumIndex di in references)
							s.WriteLine(kOutputReferenceFormat, DatumToGroupTag(di).NameToLeftPaddedString(), DatumToPath(di));

					s.WriteLine(); s.WriteLine();
				}

				// output all the referencing objects
				foreach (Blam.DatumIndex di in references)
				{
					List<IReferenceMangerObject> objs;
					if(referencers.TryGetValue(di, out objs)) // some tags may not have any other tags which reference them
					{
						s.WriteLine(kOutputReferenceFormat, DatumToGroupTag(di).Name, DatumToPath(di));
						s.WriteLine("\tchild references:");

						var refs_set = new List<Blam.DatumIndex>(objs.Count);
						#region build refs_set
						foreach (IReferenceMangerObject obj in objs) // go thru all the referencing objects...
						{
							Blam.DatumIndex pid = obj.ParentReferenceId;
							if (pid != Blam.DatumIndex.Null && !refs_set.Contains(pid)) // and get a 'set' of referencing tag names
								refs_set.Add(pid);
						}
						#endregion

						#region output refs_set
						// print out 'set'
						if (Util.Flags.Test(flags, OutputFlags.OutputPairsList))
							OutputByGroupTag(s, "\t" + kOutputReferenceFormat, refs_set);
						else
							foreach (Blam.DatumIndex pid in refs_set)
								s.WriteLine("\t" + kOutputReferenceFormat, DatumToGroupTag(pid).Name, DatumToPath(pid));
						#endregion
					}

					s.WriteLine(); s.WriteLine();
				}
			}
		}
		#endregion

		#region IEnumerable<DatumIndex> Members
		public IEnumerator<BlamLib.Blam.DatumIndex> GetEnumerator() { return references.GetEnumerator(); }
		System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator() { return references.GetEnumerator(); }
		#endregion

		public IEnumerator<IReferenceMangerObject> GetReferencersEnumerator(Blam.DatumIndex datum)
		{
			if (IsValidHandle(datum))
				return referencers[datum].GetEnumerator();

			return null;
		}
	};

	#region unused ReferenceActionsManager
#if false
	/// <summary>
	/// Utility class for manging actions which are preformed on a <see cref="ReferenceManager"/>.
	/// </summary>
	/// <remarks>
	/// An action is based on a applicator <see cref="ReferenceActionsManager.SourceType"/>) and a 
	/// operation (<see cref="ReferenceActionsManager.ActionType"/>). 
	/// </remarks>
	[IO.Class((int)IO.TagGroups.Enumerated.ReferenceManagerFile, 0)]
	public sealed class ReferenceActionsManager : IO.FileManageable
	{
		/// <summary>
		/// Source types which an action cab be applied to
		/// </summary>
		[Flags]
		public enum SourceType : int
		{
			/// <summary>
			/// Action for a single tag definition
			/// </summary>
			Specific		= 1 << 0,
			/// <summary>
			/// Action for a collection of tag definitions 
			/// </summary>
			Collection		= 1 << 1,

			//2
			//3
			//4
			//5
			//6
			//7

			/// <summary>
			/// Action for a single tag group
			/// </summary>
			Class			= 1 << 8,
			/// <summary>
			/// Action for a directory which houses multiple definitions 
			/// which can be based from multiple tag groups
			/// </summary>
			Directory		= 1 << 9,
		};

		/*public*/ class SourceData : IO.IStreamable
		{
	#region Implmentation fields
			/// <remarks>Specific & Class</remarks>
			TagInterface.TagGroup groupTag;
			/// <remarks>Specific | Directory</remarks>
			string name;

			/// <remarks>Collection + Directory</remarks>
			List<string> paths;
			/// <remarks>Collection</remarks>
			List<Blam.DatumIndex> tags;
	#endregion

	#region Type
			SourceType type;
			/// <summary>
			/// Type of information this object contains
			/// </summary>
			public SourceType Type	{ get { return type; } }
	#endregion

	#region Ctor
			/// <summary>
			/// <see cref="SourceType.Specific"/> tag ctor
			/// </summary>
			/// <param name="owner"></param>
			/// <param name="group_tag"></param>
			/// <param name="tag_name"></param>
			public SourceData(ReferenceActionsManager owner, TagInterface.TagGroup group_tag, string tag_name)
			{
				type = SourceType.Specific;
				groupTag = group_tag;
				name = tag_name;
			}
			/// <summary>
			/// Tag <see cref="SourceType.Collection"/> ctor
			/// </summary>
			/// <param name="owner"></param>
			/// <param name="tags"></param>
			public SourceData(ReferenceActionsManager owner, params Blam.DatumIndex[] tags)
			{
				type = SourceType.Collection;
				this.tags = new List<BlamLib.Blam.DatumIndex>(tags);
			}

			/// <summary>
			/// <see cref="SourceType.Specific"/> <see cref="SourceType.Class"/> ctor
			/// </summary>
			/// <param name="owner"></param>
			/// <param name="group_tag"></param>
			public SourceData(ReferenceActionsManager owner, TagInterface.TagGroup group_tag)
			{
				type = SourceType.Specific | SourceType.Class;
				groupTag = group_tag;
			}
			/// <summary>
			/// <see cref="SourceType.Collection"/> <see cref="SourceType.Class"/> ctor
			/// </summary>
			/// <param name="owner"></param>
			public SourceData(ReferenceActionsManager owner, params TagInterface.TagGroup[] groups)
			{
				type = SourceType.Collection | SourceType.Class;

				throw new NotImplementedException();
			}

			/// <summary>
			/// <see cref="SourceType.Specific"/> <see cref="SourceType.Directory"/> ctor
			/// </summary>
			/// <param name="owner"></param>
			/// <param name="directory"></param>
			public SourceData(ReferenceActionsManager owner, string directory)
			{
				type = SourceType.Specific | SourceType.Directory;
				name = directory;
			}
			/// <summary>
			/// <see cref="SourceType.Collection"/> <see cref="SourceType.Directory"/> ctor
			/// </summary>
			/// <param name="owner"></param>
			/// <param name="directories"></param>
			public SourceData(ReferenceActionsManager owner, params string[] directories)
			{
				type = SourceType.Collection | SourceType.Directory;
				paths = new List<string>(directories);
			}
	#endregion

	#region IStreamable Members
			public void Read(IO.EndianReader s)
			{
				type = (SourceType)s.ReadInt32();
				ReferenceActionsManager ram = s.Owner as ReferenceActionsManager;
				TagInterface.TagGroupCollection tgc = ram.refManager.GetGroupTags();

				uint gt;
				switch(type)
				{
					case SourceType.Specific:
						gt = s.ReadTagUInt();
						groupTag = tgc.FindTagGroup(gt);
						name = s.ReadCString();
						if (groupTag == null)
							throw new Debug.ExceptionLog("Invalid group tag for action data: '{0}' named \"{1}\". Reference Set: {2}",
								new string(TagInterface.TagGroup.FromUInt(gt)), name, ram.refManager.Name);
						break;

					case SourceType.Collection:
						tags = new List<BlamLib.Blam.DatumIndex>(s.ReadInt32());
						for (int x = 0; x < tags.Count; x++)
							tags[x].Read(s);
						break;

					case SourceType.Specific | SourceType.Class:
						gt = s.ReadTagUInt();
						groupTag = tgc.FindTagGroup(gt);
						if (groupTag == null)
							throw new Debug.ExceptionLog("Invalid group tag for action data: '{0}'. Reference Set: {1}",
								new string(TagInterface.TagGroup.FromUInt(gt)), ram.refManager.Name);
						break;

					//case SourceType.Collection | SourceType.Class:
					//	break;

					case SourceType.Specific | SourceType.Directory:
						name = s.ReadCString();
						break;

					case SourceType.Collection | SourceType.Directory:
						paths = new List<string>(s.ReadInt32());
						for (int x = 0; x < paths.Count; x++)
							paths[x] = s.ReadCString();
						break;
				}
			}

			public void Write(IO.EndianWriter s)
			{
				s.Write((int)type);

				switch (type)
				{
					case SourceType.Specific:
						groupTag.Write(s);
						s.Write(name);
						break;

					case SourceType.Collection:
						s.Write(tags.Count);
						foreach (Blam.DatumIndex di in tags)
							di.Write(s);
						break;

					case SourceType.Specific | SourceType.Class:
						groupTag.Write(s);
						break;

					//case SourceType.Collection | SourceType.Class:
					//	break;

					case SourceType.Specific | SourceType.Directory:
						s.Write(name);
						break;

					case SourceType.Collection | SourceType.Directory:
						s.Write(paths.Count);
						foreach (string dir in paths)
							s.Write(dir);
						break;
				}
			}
	#endregion
		};

		/// <summary>
		/// Actions which can be applied to a <see cref="SourceType"/> 
		/// condition
		/// </summary>
		public enum ActionType
		{
			Invalid,

			/// <summary>
			/// Ignore the sources
			/// </summary>
			Ignore,
		};


	#region Engine
		BlamVersion engine = BlamVersion.Unknown;
		/// <summary>
		/// Which engine this manager is for
		/// </summary>
		public BlamVersion Engine { get { return engine; } }
	#endregion

		List<string> refManagerDataFiles;

	#region implementation fields
		//List<SpecificActionData> actionData;
		//List<ClassActionData> classActionData;


		ReferenceManager refManager;

		// V = index to [actionData]
		Dictionary<Blam.DatumIndex, int> refActions;
	#endregion

		internal ReferenceActionsManager() {
			refManager = null;
			refActions = null;
		}

		/// <summary>
		/// Validate this manager is good-to-go and be built
		/// </summary>
		public bool Validate()
		{
			return true;
		}

		public void Build()
		{
		}

	#region IStreamable Members
		/// <summary>
		/// Stream reference action data from a file
		/// </summary>
		/// <param name="s"></param>
		public override void Read(IO.EndianReader s)
		{
			int count;

			engine.Read(s);
			s.Seek(sizeof(ushort), System.IO.SeekOrigin.Current);

	#region refManagerDataFiles
			count = s.ReadInt32();
			if (count > 0)
			{
				refManagerDataFiles = new List<string>(count);
				for (int x = 0; x < count; x++)
					refManagerDataFiles[x] = s.ReadString();
			}
	#endregion

	#region actionData
// 			count = s.ReadInt32();
// 			if (count > 0)
// 			{
// 				actionData = new List<SpecificActionData>(count);
// 				for (int x = 0; x < count; x++)
// 					actionData[x].Read(s);
// 			}
	#endregion

	#region classActionData
// 			count = s.ReadInt32();
// 			if (count > 0)
// 			{
// 				classActionData = new List<ClassActionData>(count);
// 				for (int x = 0; x < count; x++)
// 					classActionData[x].Read(s);
// 			}
	#endregion
		}
		/// <summary>
		/// Stream reference action data to a file
		/// </summary>
		/// <param name="s"></param>
		public override void Write(IO.EndianWriter s)
		{
			engine.Write(s);
			s.Write(ushort.MinValue);

	#region refManagerDataFiles
			s.Write(refManagerDataFiles.Count);
			foreach (string str in refManagerDataFiles)
				s.Write(str);
	#endregion

	#region actionData
// 			s.Write(actionData.Count);
// 			foreach (SpecificActionData ad in actionData)
// 				ad.Write(s);
	#endregion

	#region classActionData
// 			s.Write(classActionData.Count);
// 			foreach (ClassActionData ad in classActionData)
// 				ad.Write(s);
	#endregion
		}
	#endregion
	};
#endif
	#endregion
}