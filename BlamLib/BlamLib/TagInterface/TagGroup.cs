/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib.TagInterface
{
	/// <summary>
	/// Defines various properties and information for a group tag
	/// </summary>
	public sealed class TagGroup : IO.IStreamable, IComparer<TagGroup>, System.Collections.IComparer, IComparable<TagGroup>, IComparable
	{
		// NOTE: currently, the longest tag name is Halo 3's "gui_widget_texture_coordinate_animation_definition"
		#region Name
		internal const int kLongestGroupNameLength = 50;

		string name;
		/// <summary>Full name of this group</summary>
		public string Name { get { return name; } }

		/// <summary>
		/// Formats <see cref="Name"/> to a properly (left) aligned string (using blank-white-space)
		/// </summary>
		/// <returns><see cref="string.PadLeft(int)"/> on <see cref="Name"/></returns>
		/// <remarks>Pad width is determined by the longest group name found in any of the Blam engine variants</remarks>
		public string NameToLeftPaddedString()	{ return name.PadLeft(kLongestGroupNameLength); }
		/// <summary>
		/// Formats <see cref="Name"/> to a properly (right) aligned string (using blank-white-space)
		/// </summary>
		/// <returns><see cref="string.PadRight(int)"/> on <see cref="Name"/></returns>
		/// <remarks>Pad width is determined by the longest group name found in any of the Blam engine variants</remarks>
		public string NameToRightPaddedString()	{ return name.PadRight(kLongestGroupNameLength); }
		#endregion

		#region ID
		uint iD;
		/// <summary>The four character code translated into a unsigned int</summary>
		public uint ID { get { return iD; } }
		#endregion

		#region Tag
		string tagAsString;
		char[] tag;
		/// <summary>The four character code of this group</summary>
		[System.ComponentModel.Browsable(false)]
		public char[] Tag { get { return tag; } }
		public string TagToString() { return /*new string(tag)*/tagAsString; }
		#endregion

		#region Handle
		Blam.DatumIndex handle = Blam.DatumIndex.Null;
		/// <summary>Handle that can be used to serialize references to this group tag</summary>
		[System.ComponentModel.Browsable(false)]
		public Blam.DatumIndex Handle	{ get { return handle; } }

		/// <summary>
		/// Create a handle from a base engine, index into the groups collection and if it's a struct or not
		/// </summary>
		/// <param name="engine"></param>
		/// <param name="index"></param>
		/// <param name="is_struct"></param>
		/// <remarks>Also initializes <see cref="TagGroup.Definition"/>'s <see cref="DefinitionState.Handle"/> (if its not null)</remarks>
		internal void InitializeHandle(BlamVersion engine, int index, bool is_struct)
		{
			if (iD == uint.MaxValue) return; // in case this is 'Null'

			ushort i = (ushort)((is_struct ? 0x8000 : 0) | (index & 0x7FFF));

			handle = new BlamLib.Blam.DatumIndex(i, (short)engine);

			if (definition != null) definition.Handle = handle;
			else if (
				engine != BlamVersion.Unknown &&
					(engine & BlamVersion.Halo3) == 0 &&// TODO: remove this when Halo3 support is good to go
					(engine & BlamVersion.HaloOdst) == 0 &&
					(engine & BlamVersion.HaloReach) == 0 &&
					(engine & BlamVersion.Halo4) == 0
				)
				throw new Debug.ExceptionLog("{0}\t{1}\t{2:X4}\t{3}", engine, name, index, is_struct);
		}
		#endregion

		#region Parent
		TagGroup parent = null;
		/// <summary>Parent group tag</summary>
		[System.ComponentModel.Browsable(false)]
		public TagGroup Parent { get { return parent; } }

		/// <summary>
		/// Walks UP this tag group's hierarchy and determines if <paramref name="tag_group"/> 
		/// is a parent group
		/// </summary>
		/// <param name="tag_group">Tag group to test for inheritance</param>
		/// <returns>True if <paramref name="tag_group"/> appears anywhere UP in the hierarchy, false otherwise</returns>
		public bool IsParent(TagGroup tag_group)
		{
			if (parent == null) return false;

			if (parent.iD == tag_group.iD || parent.IsParent(tag_group)) return false;

			return false;
		}

		/// <summary>
		/// Finds the top most parent for this tag group
		/// </summary>
		/// <param name="parent_group">Reference to the top most parent</param>
		/// <returns>True if <paramref name="parent_group"/> isn't null, false if no parent groups exist</returns>
		public bool GetUltimateParent(out TagGroup parent_group)
		{
			if (parent == null)
			{
				parent_group = null;
				return false;
			}

			parent_group = this;
			while (parent_group.parent != null)
				parent_group = parent_group.parent;
			return true;

		}
		#endregion

		#region Children
		List<TagGroup> children = null;
		/// <summary>Tag groups which inherit from this tag group</summary>
		[System.ComponentModel.Browsable(false)]
		public IEnumerable<TagGroup> Children { get { return children; } }

		public bool HasChildren { get { return children != null && children.Count > 0; } }

		void AddChild(TagGroup child)
		{
			if (children == null) children = new List<TagGroup>(16);
			if (children.IndexOf(child) == -1) children.Add(child);
		}

		/// <summary>
		/// Walks DOWN this tag group's hierarchy and determines if <paramref name="tag_group"/> 
		/// is a child group
		/// </summary>
		/// <param name="tag_group">Tag group to test for inheritance</param>
		/// <returns>True if <paramref name="tag_group"/> appears anywhere DOWN in the hierarchy, false otherwise</returns>
		public bool IsChild(TagGroup tag_group)
		{
			if (children == null) return false;

			foreach(TagGroup tg in children)
				if (tag_group.iD == tg.iD || tg.IsChild(tag_group)) return true;

			return false;
		}

		/// <summary>
		/// Determines if <paramref name="tag_group"/> exists ANYWHERE in this tag group's hierarchy
		/// </summary>
		/// <param name="tag_group"></param>
		/// <returns></returns>
		public bool IsRelative(TagGroup tag_group)
		{
			TagGroup parent_group;
			if (GetUltimateParent(out parent_group))
				return parent_group.IsChild(tag_group);
			else
				return IsChild(tag_group);
		}
		#endregion

		#region Definition
		DefinitionState definition = null;
		/// <summary>
		/// The definition of this tag group
		/// </summary>
		[System.ComponentModel.Browsable(false)]
		public DefinitionState Definition
		{
			get { return definition; }
			internal set { definition = value; }
		}
		#endregion

		#region Filter
		string filter;
		/// <summary>The file dialog file filter string for this tag group</summary>
		public string Filter { get { return filter; } }
		#endregion

		#region EngineData
		object engineData = null;
		/// <summary>Data thats only used by the owning engine's code</summary>
		/// <see cref="Blam.Halo3.BlamFile"/>
		[System.ComponentModel.Browsable(false)]
		internal object EngineData
		{
			get { return engineData; }
			set { engineData = value; }
		}
		#endregion


		#region Null
		public static readonly TagGroup Null = new TagGroup();
		TagGroup()
		{
			tag = new char[4];
			tag[0] = (char)0xFF;
			tag[1] = (char)0xFF;
			tag[2] = (char)0xFF;
			tag[3] = (char)0xFF;
			name = "none";
			filter = "none|*.none";
			iD = uint.MaxValue;
		}
		#endregion

		#region Constructors
		public TagGroup(string _tag, string _name)
		{
			Debug.Assert.If(_tag.Length <= 4, "{0} has a tag group id thats to big", name);
			this.tagAsString = _tag;
			this.tag = _tag.ToCharArray();
			this.name = _name;

			this.iD = (uint)(
					((byte)tag[3] << 24) |
					((byte)tag[2] << 16) |
					((byte)tag[1] << 8) |
					((byte)tag[0])
						);
			this.filter = _name + " tags|*." + _name;
		}

		public TagGroup(string tag, TagGroup parent, string name) : this(tag, name)
		{
			this.parent = parent;
			parent.AddChild(this);
		}
		#endregion

		#region IStreamable
		/// <summary>
		/// Moves the stream ahead by the sizeof
		/// a four character code (4 bytes)
		/// </summary>
		/// <param name="s"></param>
		/// <remarks>Doesn't actually read any data from the stream, only seeks forward</remarks>
		public void Read(IO.EndianReader s) { s.Seek(4, System.IO.SeekOrigin.Current); }

		/// <summary>
		/// Writes this tag group's four character code
		/// </summary>
		/// <param name="s"></param>
		public void Write(IO.EndianWriter s) { s.WriteTag(this.tag); }
		#endregion

		#region Overrides
		/// <summary>
		/// Compares two <see cref="TagGroup"/> objects testing their
		/// group tags for equality
		/// </summary>
		/// <param name="obj">other <see cref="TagGroup"/> object</param>
		/// <returns>true if both this object and <paramref name="obj"/> are equal</returns>
		public override bool Equals(object obj)	{ return this.iD == ((TagGroup)obj).iD; }

		/// <summary>
		/// Returns the hash code for this instance
		/// </summary>
		/// <returns>This object's group tag</returns>
		public override int GetHashCode()		{ return (int)iD; }

		public override string ToString()		{ return string.Format("['{0,4}'  {1}]", TagToString(), name); }
		#endregion

		#region Operators
		/// <summary>
		/// Returns the name of this tag group
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		public static explicit operator string(TagGroup value)			{ return value.name; }
		/// <summary>
		/// Returns the definition state of this tag group
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		public static explicit operator DefinitionState(TagGroup value) { return value.definition; }
		/// <summary>
		/// Returns the group tag in char[] form
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		public static explicit operator char[](TagGroup value)			{ return value.tag; }
		/// <summary>
		/// Returns the group tag in integer form
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		public static explicit operator uint(TagGroup value)			{ return value.iD; }
		#endregion

		#region Util
		/// <summary>
		/// Takes a four character code and performs
		/// a dword byte swap on it, storing the
		/// result in a new four character code
		/// </summary>
		/// <param name="tag">value to be byte swapped</param>
		/// <returns>dword byte swapped four character code</returns>
		public static char[] Swap(char[] tag)
		{
			char[] swap = new char[4];
			swap[0] = tag[3];
			swap[1] = tag[2];
			swap[2] = tag[1];
			swap[3] = tag[0];

			return swap;
		}

		/// <summary>
		/// Takes another four character code
		/// and performs a check on it against this
		/// object's tag to see if they are completely equal
		/// </summary>
		/// <param name="other"></param>
		/// <returns>True if equal to this</returns>
		public bool Test(char[] other) { return TagGroup.Test(this.tag, other); }

		/// <summary>
		/// Takes two four character codes and
		/// performs a check on them to see if they are
		/// completely equal
		/// </summary>
		/// <param name="tag1"></param>
		/// <param name="tag2"></param>
		/// <returns>True if both are equal</returns>
		public static bool Test(char[] tag1, char[] tag2)
		{
			if (tag1[0] == tag2[0] &&
				tag1[1] == tag2[1] &&
				tag1[2] == tag2[2] &&
				tag1[3] == tag2[3])
				return true;

			return false;
		}

		#region UInt
		/// <summary>
		/// Takes a fourcc and converts it into its (unsigned) integer value
		/// </summary>
		/// <param name="tag"></param>
		/// <returns></returns>
		/// <remarks>assumes <paramref name="tag"/> is in big-endian order, though in most cases order doesn't matter</remarks>
		public static uint ToUInt(char[] tag)
		{
			return (uint)(
					((byte)tag[0] << 24) |
					((byte)tag[1] << 16) |
					((byte)tag[2] << 8) |
					((byte)tag[3])
						);
		}

		/// <summary>
		/// Takes a fourcc and converts it into its (unsigned) integer value
		/// </summary>
		/// <param name="tag"></param>
		/// <returns></returns>
		/// <remarks>assumes <paramref name="tag"/> is in big-endian order, though in most cases order doesn't matter</remarks>
		public static uint ToUInt(string tag)
		{
			return (uint)(
					((byte)tag[0] << 24) |
					((byte)tag[1] << 16) |
					((byte)tag[2] << 8) |
					((byte)tag[3])
						);
		}

		/// <summary>
		/// Takes a (unsigned) integer and converts it into its fourcc value
		/// </summary>
		/// <param name="group_tag"></param>
		/// <returns>big-endian ordered fourcc</returns>
		public static char[] FromUInt(uint group_tag)
		{
			char[] tag = new char[4];
			tag[0] = (char)((group_tag & 0xFF000000) >> 24);
			tag[1] = (char)((group_tag & 0x00FF0000) >> 16);
			tag[2] = (char)((group_tag & 0x0000FF00) >>  8);
			tag[3] = (char) (group_tag & 0x000000FF)       ;

			return tag;
		}

		/// <summary>
		/// Takes a (unsigned) integer and converts it into its fourcc value
		/// </summary>
		/// <param name="group_tag"></param>
		/// <param name="is_big_endian">endian order override</param>
		/// <returns>big-endian ordered fourcc if <paramref name="is_big_endian"/> is true, little-endian if false</returns>
		public static char[] FromUInt(uint group_tag, bool is_big_endian)
		{
			if (is_big_endian) return FromUInt(group_tag);

			char[] tag = new char[4];
			tag[3] = (char)((group_tag & 0xFF000000) >> 24);
			tag[2] = (char)((group_tag & 0x00FF0000) >> 16);
			tag[1] = (char)((group_tag & 0x0000FF00) >>  8);
			tag[0] = (char) (group_tag & 0x000000FF)       ;

			return tag;
		}
		#endregion

		/// <summary>
		/// Get a file search pattern string for tags of this group
		/// </summary>
		/// <returns></returns>
		public string ToFileSearchPattern()
		{
			return string.Format("*.{0}", name);
		}
		#endregion

		#region ICompare Members
		/// <summary>
		/// Does a comparison based on the tag group's names
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <returns></returns>
		/// <seealso cref="string.Compare(string, string, bool)"/>
		public int Compare(TagGroup x, TagGroup y)	{ return string.Compare(x.name, y.name, true); }
		/// <summary>
		/// Does a comparison based on the tag group's names
		/// </summary>
		/// <param name="other"></param>
		/// <returns></returns>
		/// <seealso cref="string.Compare(string, string, bool)"/>
		public int CompareTo(TagGroup other)		{ return string.Compare(this.name, other.name, true); }
		/// <summary>
		/// Does a comparison based on the tag group's names
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <returns></returns>
		/// <seealso cref="string.Compare(string, string, bool)"/>
		public int Compare(object x, object y)		{ return Compare((TagGroup)x, (TagGroup)y); }
		/// <summary>
		/// Does a comparison based on the tag group's names
		/// </summary>
		/// <param name="obj"></param>
		/// <returns></returns>
		/// <seealso cref="string.Compare(string, string, bool)"/>
		public int CompareTo(object obj)			{ return CompareTo((TagGroup)obj); }
		#endregion
	};

	/// <summary>
	/// Holds a collection of group tags (either in a ordered fashion or in the order they were added)
	/// </summary>
	public sealed class TagGroupCollection : ICollection<TagGroup>
	{
		/// <summary>Represents a tag group collection with no entries</summary>
		public static TagGroupCollection Empty { get; private set; }

		class ByNameComparer : System.Collections.IComparer, System.Collections.Generic.IComparer<TagGroup>
		{
			public int Compare(object x, object y)		{ return Compare((TagGroup)x, (TagGroup)y); }
			public int Compare(TagGroup x, TagGroup y)	{ return string.Compare(x.Name, y.Name); }
		};

		struct Enumerator : IEnumerator<TagGroup>
		{
			TagGroupCollection m_coll;
			int m_index;

			public Enumerator(TagGroupCollection collection)
			{
				m_coll = collection;
				m_index = -1;
			}

			#region IEnumerator<TagGroup> Members
			void IDisposable.Dispose()
			{
			}

			TagGroup GetCurrent()
			{
				if (m_index >= 0 && m_index < m_coll.Count)
					return m_coll.groupTags[m_index];

				return TagGroup.Null;
			}

			public TagGroup Current							{ get { return GetCurrent(); } }
			object System.Collections.IEnumerator.Current	{ get { return GetCurrent(); } }

			public bool MoveNext()
			{
				if (m_index < m_coll.Count)
				{
					m_index++;
					return true;
				}

				return false;
			}

			public void Reset()
			{
				m_index = -1;
			}
			#endregion
		};

		#region GroupTags
		TagGroup[] groupTags;
		public TagGroup[] GroupTags { get { return groupTags; } }
		#endregion

		#region Filter
		string filter;
		/// <summary>
		/// The file dialog file filter string for this tag group
		/// </summary>
		public string Filter { get { return filter; } }
		#endregion

		#region Indexers
		/// <summary>
		/// Gets a TagGroup in this collection based on a index
		/// </summary>
		public TagGroup this[int index] { get { return groupTags[index]; } }

		/// <summary>
		/// Returns the full name of a group_tag
		/// </summary>
		public string this[char[] tag]
		{
			get
			{
				foreach (TagGroup t in groupTags)
					if (TagGroup.Test(tag, t.Tag))
						return t.Name;

				return "unknown";
			}
		}
		#endregion

		#region Searching
		/// <summary>
		/// Finds the index of a <see cref="TagGroup"/>
		/// </summary>
		/// <param name="group_tag">The id of a tag_group to search for</param>
		/// <returns></returns>
		public int FindGroupIndex(uint group_tag)
		{
			int index = 0;
			foreach (TagGroup g in groupTags)
			{
				if (g.ID == group_tag) return index;
				index++;
			}

			return -1;
		}

		/// <summary>
		/// Finds the index of a <see cref="TagGroup"/>
		/// </summary>
		/// <param name="group_tag">The <see cref="TagGroup"/>'s 'tag' to search for</param>
		/// <returns></returns>
		public int FindGroupIndex(char[] group_tag)
		{
			int index = 0;
			foreach (TagGroup g in groupTags)
			{
				if (TagGroup.Test(g.Tag, group_tag)) return index;
				index++;
			}

			return -1;
		}

		/// <summary>
		/// Finds the index of a <see cref="TagGroup"/>
		/// </summary>
		/// <param name="group_tag">The name of a tag_group to search for</param>
		/// <returns></returns>
		public int FindGroupIndex(string group_tag)
		{
			int index = 0;
			foreach (TagGroup g in groupTags)
			{
				if (g.Name == group_tag) return index;
				index++;
			}

			return -1;
		}

		/// <summary>
		/// Finds the index of a supplied <see cref="TagGroup"/> object
		/// </summary>
		/// <param name="group"><see cref="TagGroup"/> object whose index we want to find</param>
		/// <returns></returns>
		public int FindGroupIndex(TagGroup group)
		{
			int index = 0;
			foreach(TagGroup g in groupTags)
			{
				if (object.ReferenceEquals(g, group)) return index;
				index++;
			}

			return -1;
		}

		/// <summary>
		/// Finds a TagGroup of this collection based on its group tag
		/// </summary>
		/// <param name="group_tag">The <see cref="TagGroup"/>'s 'tag' to search for</param>
		/// <returns>Null if <paramref name="group_tag"/> isn't a part of this collection</returns>
		public TagGroup FindGroup(char[] group_tag)
		{
			int index = FindGroupIndex(group_tag);
			if (index == -1) return null;

			return groupTags[index];
		}

		/// <summary>
		/// Finds a <see cref="TagGroup"/> of this collection based on its group tag
		/// </summary>
		/// <param name="group_tag">The name of a <see cref="TagGroup"/> to search for</param>
		/// <returns>Null if <paramref name="group_tag"/> isn't a part of this collection</returns>
		public TagGroup FindGroup(string group_tag)
		{
			int index = FindGroupIndex(group_tag);
			if (index == -1) return null;

			return groupTags[index];
		}

		/// <summary>
		/// Finds the char[] group_tag element in this collection
		/// based on a uint group_tag
		/// </summary>
		/// <param name="tag"></param>
		/// <returns></returns>
		public char[] FindTag(uint tag)
		{
			foreach (TagGroup g in groupTags)
				if (g.ID == tag) return g.Tag;

			return null;
		}

		/// <summary>
		/// Find a <see cref="TagGroup"/> object in this collection based on it's group tag
		/// </summary>
		/// <param name="tag">Group tag to find</param>
		/// <returns><see cref="TagGroup"/> object existing in this collection, or null if not found.</returns>
		public TagGroup FindTagGroup(uint tag)
		{
			foreach (TagGroup g in groupTags)
				if (g.ID == tag) return g;

			return null;
		}

		/// <summary>
		/// Find a <see cref="TagGroup"/> object in this collection based on it's group tag
		/// </summary>
		/// <param name="tag">Group tag to find</param>
		/// <param name="big_endian">If this isn't true, we will byte swap the tag before searching</param>
		/// <returns><c>TagGroup</c> object existing in this collection, or null if not found.</returns>
		public TagGroup FindTagGroup(uint tag, bool big_endian)
		{
			if (!big_endian) IO.ByteSwap.SwapUDWord(ref tag);
			foreach (TagGroup g in groupTags)
				if (g.ID == tag) return g;

			return null;
		}

		/// <summary>
		/// Determines if a uint group_tag is
		/// apart of this collection
		/// </summary>
		/// <param name="group_tag">the char[4] group_tag</param>
		/// <param name="big_endian">If this isn't true, we will byte swap the tag before any operations</param>
		/// <returns>True if the group tag exists in this collection</returns>
		public bool IsTag(uint group_tag, bool big_endian)
		{
			if (big_endian) group_tag = IO.ByteSwap.SwapUDWord(group_tag);

			foreach (TagGroup t in groupTags)
				if (group_tag == t.ID) return true;

			return false;
		}
		#endregion

		/// <summary>
		/// Sort the collection by the names of the group tags
		/// </summary>
		public void Sort() { Array.Sort<TagGroup>(groupTags, new ByNameComparer()); }

		/// <summary>
		/// Determines if <paramref name="c"/> is the same as this collection
		/// </summary>
		/// <param name="c"></param>
		/// <returns></returns>
		/// <remarks>Comparison is VERY strict to the point where the group tags must be in the same order</remarks>
		public bool IsSameCollection(TagGroupCollection c)
		{
			if (c == null) return false;

			if (object.ReferenceEquals(this, c)) return true;

			if(this.Count == c.Count)
			{
				for (int x = 0; x < this.Count; x++)
					if (groupTags[x].ID != c.groupTags[x].ID) return false;

				return true;
			}

			return false;
		}

		#region Ctor
		static TagGroupCollection()
		{
			Empty = new TagGroupCollection();
		}

		/// <summary>
		/// Only for <see cref="TagGroupCollection.Empty"/>
		/// </summary>
		private TagGroupCollection()
		{
			groupTags = new TagGroup[0];
			filter = string.Empty;
		}

		/// <summary>
		/// Create a collection based on an existing list of group tags
		/// </summary>
		/// <param name="tag_groups"></param>
		public TagGroupCollection(TagGroup[] tag_groups)
		{
			groupTags = new TagGroup[tag_groups.Length];
			for (int x = 0; x < tag_groups.Length; x++)
				groupTags[x] = tag_groups[x];

			StringBuilder sb = new StringBuilder();
			sb.Append("All supported tags|");
			foreach (TagGroup tg in groupTags)	sb.AppendFormat("*.{0};", tg.Name);
			sb.Remove(sb.Length - 1, 1);
			foreach (TagGroup tg in groupTags)	sb.AppendFormat("|{0}", tg.Filter);
			filter = sb.ToString();
		}

		/// <summary>
		/// Create a collection using an explicit list of group tags
		/// </summary>
		/// <param name="sort">Should we sort the list?</param>
		/// <param name="tag_groups"></param>
		public TagGroupCollection(bool sort, params TagGroup[] tag_groups)
		{
			groupTags = tag_groups;
			if (sort) Sort();

			StringBuilder sb = new StringBuilder();
			sb.Append("All supported tags|");
			foreach(TagGroup tg in groupTags)	sb.AppendFormat("*.{0};", tg.Name);
			sb.Remove(sb.Length - 1, 1);
			foreach(TagGroup tg in groupTags)	sb.AppendFormat("|{0}", tg.Filter);
			filter = sb.ToString();
		}
		#endregion

		#region IEnumerable Members
		System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()	{ return new Enumerator(this); }
		public IEnumerator<TagGroup> GetEnumerator()									{ return new Enumerator(this); }
		#endregion

		#region ICollection Members
		/// <summary>Not Implemented.</summary>
		/// <param name="item"></param>
		public void Add(TagGroup item) { throw new NotSupportedException(); }
		/// <summary>Not Implemented.</summary>
		public void Clear() { throw new NotSupportedException(); }
		/// <summary>
		/// Determines if there is TagGroup which has the same matching <see cref="TagGroup.ID"/> as <paramref name="item"/>
		/// </summary>
		/// <param name="item"></param>
		/// <returns></returns>
		public bool Contains(TagGroup item) { foreach (TagGroup tg in groupTags) if (item.ID == tg.ID) return true; return false; }
		/// <summary>
		/// Copy the elements in this array to <paramref name="array"/>
		/// </summary>
		/// <param name="array"></param>
		/// <param name="arrayIndex"></param>
		public void CopyTo(TagGroup[] array, int arrayIndex) { groupTags.CopyTo(array, arrayIndex); }

		/// <summary>
		/// How many group tags are in this collection
		/// </summary>
		public int Count { get { return groupTags.Length; } }

		/// <summary>Always true</summary>
		public bool IsReadOnly { get { return true; } }
		/// <summary></summary>
		public bool IsSynchronized { get { return groupTags.IsSynchronized; } }
		/// <summary></summary>
		public object SyncRoot { get { return groupTags.SyncRoot; } }

		/// <summary>Not Implemented.</summary>
		/// <param name="item"></param>
		/// <returns></returns>
		public bool Remove(TagGroup item) { throw new NotSupportedException(); }
		#endregion

		#region IStreamable
		/// <summary>
		/// Moves the stream ahead by the sizeof a four character code (4 bytes) times the count of the group tags
		/// </summary>
		/// <param name="s"></param>
		/// <remarks>Doesn't actually read any data from the stream, only seeks forward</remarks>
		public void Read(IO.EndianReader s)		{ s.Seek(groupTags.Length * 4, System.IO.SeekOrigin.Current); }

		/// <summary>
		/// Writes each tag group's four character code
		/// </summary>
		/// <param name="s"></param>
		public void Write(IO.EndianWriter s)	{ foreach(TagGroup t in groupTags) s.WriteTag(t.ID); }
		#endregion
	};
}