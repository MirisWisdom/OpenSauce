/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;

namespace BlamLib.Managers
{
	/// <summary>
	/// Interface for any tag index
	/// </summary>
	/// <remarks>
	/// 'tag_datum' refers to the handles used by the index table we're wrapping (ie, a tag index in a cache file)
	/// 'tag_index' refers to the internal handles used to associate with <see cref="TagManager"/> objects
	/// </remarks>
	public interface ITagIndex : IDisposable
	{
		/// <summary>
		/// Engine version this tag index is for
		/// </summary>
		BlamVersion Engine { get; }

		/// <summary>
		/// Handle for this index to <see cref="BlamDefinition"/>'s management
		/// </summary>
		Blam.DatumIndex IndexId { get; }

		/// <summary>
		/// Tag groups to ignore
		/// </summary>
		TagInterface.TagGroupCollection IgnoreList { get; }

		/// <summary>
		/// Reference manger for this index
		/// </summary>
		ReferenceManager References { get; }

		/// <summary>
		/// String ids manager for this index
		/// </summary>
		StringIdManager StringIds { get; }

		/// <summary>
		/// Open an existing a tag
		/// </summary>
		/// <param name="name">name of the tag</param>
		/// <param name="tag_group">tag group of the tag</param>
		/// <returns>handle of the tag</returns>
		Blam.DatumIndex Open(string name, TagInterface.TagGroup tag_group);

		/// <summary>
		/// Updates the reference count for the tag,
		/// unloading the tag if there are no more
		/// references
		/// </summary>
		/// <param name="tag_index"></param>
		/// <returns>true if no longer in the tag index</returns>
		bool Unload(Blam.DatumIndex tag_index);

		/// <summary>
		/// Updates the reference count for the tag and it's dependents, unloading the tag or any of it's dependents if there are no more references
		/// </summary>
		/// <param name="tag_index">index of the tag to unload</param>
		void UnloadWithDependents(Blam.DatumIndex tag_index);

		/// <summary>
		/// Removes the tag from the index completely, disregarding
		/// any other cases (IE, multiple references to the tag)
		/// </summary>
		/// <param name="tag_index"></param>
		void UnloadForce(Blam.DatumIndex tag_index);

		/// <summary>
		/// Removes the tag, along with any of it's loaded dependents from the index completely, 
		/// disregarding any other cases (IE, multiple references to the tag)
		/// </summary>
		/// <param name="tag_index">index of the tag to force the unload upon</param>
		void UnloadForceWithDependents(Blam.DatumIndex tag_index);

		/// <summary>
		/// Removes all tags from the index completely, disregarding
		/// any other cases (IE, multiple references to the tag)
		/// </summary>
		void UnloadAll();

		/// <summary>
		/// Indexer into this tag index
		/// </summary>
		/// <param name="tag_index">datum index of a tag</param>
		/// <returns>Tag manager for the referenced tag</returns>
		TagManager this[Blam.DatumIndex tag_index] { get; }
	};


	/// <summary>
	/// Base class for tag index events
	/// </summary>
	public class TagIndexEventArgs : EventArgs
	{
		#region TagIndex
		TagIndexBase tagIndex;
		/// <summary>
		/// Index which the event took place in
		/// </summary>
		public TagIndexBase TagIndex { get { return tagIndex; } }
		#endregion

		#region TagManager
		TagManager tagMan;
		/// <summary></summary>
		public TagManager TagManager	{ get { return tagMan; } }
		#endregion

		public TagIndexEventArgs(TagIndexBase index, TagManager tag)
		{
			tagIndex = index;
			tagMan = tag;
		}
	};

	/// <summary>
	/// Abstraction between <see cref="TagIndex"/> and <see cref="CacheTagIndex"/> management classes
	/// </summary>
	public abstract class TagIndexBase : ITagIndex, IEnumerable<TagManager>
	{
		protected static Debug.Trace indexTrace = new Debug.Trace("TagIndex", "document tag index output");

		#region Array
		protected DataArray<TagManager> Array;
		/// <summary>
		/// Indexer into this tag index
		/// </summary>
		/// <param name="tag_index">datum index of a tag</param>
		/// <returns>Tag manager for the referenced tag</returns>
		/// <see cref="DataArray{T}"/>
		public abstract TagManager this[Blam.DatumIndex tag_index] { get; }

		// This used to be in the Ctor, but CacheTagIndexes were exceptioning due 
		// to their internal cacheFile not being set when the base ctor ran (which 
		// calls the abstract Engine property...)
		protected void ArrayInitialize(string array_name)
		{
			int max_tag_count = 1024;

			var g = Program.GetManager(Engine).FindGame(Engine);
			if (g != null) max_tag_count = g.Tags.MaxCount;

			Array = new DataArray<TagManager>(max_tag_count, array_name);
		}
		#endregion


		#region Ignore List
		TagInterface.TagGroupCollection ignoreList = null;
		/// <summary>
		/// Get the ignore tag group list for this tag index
		/// </summary>
		public TagInterface.TagGroupCollection IgnoreList { get { return ignoreList; } }

		/// <summary>
		/// Set the ignore tag group list for this tag index
		/// </summary>
		/// <param name="list"></param>
		/// <remarks>
		/// If any <see cref="TagInterface.TagGroup"/>s in <paramref name="list"/> have any children, 
		/// they will be implicitly added to the 'ignore list'
		/// </remarks>
		public void SetupIgnoreList(params TagInterface.TagGroup[] list)
		{
			if (list.Length > 0)
			{
				List<TagInterface.TagGroup> others = new List<BlamLib.TagInterface.TagGroup>(list.Length);
				foreach (TagInterface.TagGroup tg in list)
				{
					others.Add(tg);
					if (tg.HasChildren) others.AddRange(tg.Children);
				}

				ignoreList = new TagInterface.TagGroupCollection(false, others.ToArray());
			}
		}

		/// <summary>
		/// Check to verify if the tag group is ignored
		/// </summary>
		/// <param name="tg">tag group t check</param>
		/// <returns>true if we ignore it</returns>
		protected bool Ignore(TagInterface.TagGroup tg)
		{
			if (ignoreList == null) return false;

			return ignoreList.Contains(tg);
		}
		#endregion

		#region Engine
		/// <summary>
		/// Engine version this tag index is for
		/// </summary>
		public abstract BlamVersion Engine { get; }
		#endregion

		#region IndexId
		protected Blam.DatumIndex indexId = Blam.DatumIndex.Null;
		/// <summary>
		/// Handle for this index to <see cref="BlamDefinition"/>'s management
		/// </summary>
		public Blam.DatumIndex IndexId
		{
			get { return indexId; }
			internal set { indexId = value; }
		}
		#endregion

		#region References
		/// <summary>
		/// Reference manger for this index
		/// </summary>
		public abstract ReferenceManager References { get; }
		#endregion

		#region StringIdManager
		/// <summary>
		/// String ids manager for this index
		/// </summary>
		public abstract StringIdManager StringIds { get; }
		#endregion

		#region Ctor
		protected TagIndexBase()
		{
		}

		public abstract void Dispose();
		#endregion

		#region ITagIndex
		#region Open
		/// <summary>
		/// Fired <b>after</b> a tag is opened in the index
		/// </summary>
		public event EventHandler<TagIndexEventArgs> EventOpen;
		protected void OnEventOpen(TagIndexEventArgs args)						{ if (EventOpen != null) EventOpen(this, args); }

		/// <summary>
		/// Open an existing a tag
		/// </summary>
		/// <param name="name">name of the tag</param>
		/// <param name="tag_group">tag group of the tag</param>
		/// <returns>handle of the tag</returns>
		public abstract Blam.DatumIndex Open(string name, TagInterface.TagGroup tag_group);
		#endregion

		#region Unload
		/// <summary>
		/// Fired <b>before</b> a tag is unloaded from the index
		/// </summary>
		public event EventHandler<TagIndexEventArgs> EventUnload;
		protected void OnEventUnload(TagIndexEventArgs args)					{ if (EventUnload != null) EventUnload(this, args); }

		/// <summary>
		/// Updates the reference count for the tag, unloading the tag if there are no more references
		/// </summary>
		/// <param name="tag_index"></param>
		/// <returns>true if no longer in the tag index</returns>
		public abstract bool Unload(Blam.DatumIndex tag_index);
		#endregion

		#region UnloadWithDependents
		/// <summary>
		/// Fired <b>before</b> a tag, along with all it's dependents, are unloaded
		/// </summary>
		/// <remarks>
		/// Only called once, for the actual tag being unloaded. Will not be called as each dependent is unloaded, 
		/// <see cref="TagIndexBase.OnEventUnload"/> will be called on each and every tag however
		/// </remarks>
		public event EventHandler<TagIndexEventArgs> EventUnloadWithDependents;
		protected void OnEventUnloadWithDependents(TagIndexEventArgs args)		{ if (EventUnloadWithDependents != null) EventUnloadWithDependents(this, args); }

		/// <summary>
		/// Updates the reference count for the tag and it's dependents, unloading the tag or any of it's dependents if there are no more references
		/// </summary>
		/// <param name="tag_index">index of the tag to unload</param>
		public abstract void UnloadWithDependents(Blam.DatumIndex tag_index);
		#endregion

		#region UnloadForce
		/// <summary>
		/// Fired <b>before</b> a tag is forcefully unloaded from the index
		/// </summary>
		public event EventHandler<TagIndexEventArgs> EventUnloadForce;
		protected void OnEventUnloadForce(TagIndexEventArgs args)				{ if (EventUnloadForce != null) EventUnloadForce(this, args); }

		/// <summary>
		/// Removes the tag from the index completely, disregarding
		/// any other cases (IE, multiple references to the tag)
		/// </summary>
		/// <param name="tag_index"></param>
		public abstract void UnloadForce(Blam.DatumIndex tag_index);
		#endregion

		#region UnloadForceWithDependents
		/// <summary>
		/// Fired <b>before</b> a tag, along with all it's dependents, are forcefully unloaded
		/// </summary>
		/// <remarks>
		/// Only called once, for the actual tag being unloaded. Will not be called as each dependent is unloaded, 
		/// <see cref="TagIndexBase.OnEventUnloadForce"/> will be called on each and every tag however
		/// </remarks>
		public event EventHandler<TagIndexEventArgs> EventUnloadForceWithDependents;
		protected void OnEventUnloadForceWithDependents(TagIndexEventArgs args)	{ if (EventUnloadForceWithDependents != null) EventUnloadForceWithDependents(this, args); }

		/// <summary>
		/// Removes the tag, along with any of it's loaded dependents from the index completely, 
		/// disregarding any other cases (IE, multiple references to the tag)
		/// </summary>
		/// <param name="tag_index">index of the tag to force the unload upon</param>
		public abstract void UnloadForceWithDependents(Blam.DatumIndex tag_index);
		#endregion

		#region UnloadAll
		/// <summary>
		/// Fired <b>before</b> everything is unloaded from the index
		/// </summary>
		public event EventHandler<TagIndexEventArgs> EventUnloadAll;
		protected void OnEventUnloadAll(TagIndexEventArgs args)					{ if (EventUnloadAll != null) EventUnloadAll(this, args); }

		/// <summary>
		/// Removes all tags from the index completely, disregarding
		/// any other cases (IE, multiple references to the tag)
		/// </summary>
		public abstract void UnloadAll();
		#endregion
		#endregion

		#region IEnumerable<TagManager> Members
		public IEnumerator<TagManager> GetEnumerator()									{ return Array.GetEnumerator(); }
		System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()	{ return Array.GetEnumerator(); }
		#endregion

		#region Util
		/// <summary>
		/// Figures out if the tag is loaded in the tag index
		/// </summary>
		/// <param name="name">name of tag</param>
		/// <param name="tag_group">group tag of tag</param>
		/// <returns>The tag_index of the <see cref="TagManager"/> for the name\tag_group, or <see cref="Blam.DatumIndex.Null"/> if not loaded</returns>
		public Blam.DatumIndex IsLoaded(string name, TagInterface.TagGroup tag_group)
		{
			foreach (TagManager tm in Array)
				if (tm.Compare(this, name, tag_group)) return tm.TagIndex;

			return Blam.DatumIndex.Null;
		}
		#endregion
	};
}