/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;

namespace BlamLib.Managers
{
	/// <summary>
	/// Tag Index that is based on cache files
	/// </summary>
	/// <remarks>See the remarks in <see cref="ITagIndex"/></remarks>
	public partial class CacheTagIndex : TagIndexBase
	{
		const string kDefaultArrayName = "cache tag instances";

		static Debug.Trace extractionTrace = new Debug.Trace("CacheTagExtraction", "document extracted tag output");

		protected Blam.CacheFile cacheFile;

		/// <summary>
		/// Indexer into this tag index
		/// </summary>
		/// <param name="tag_index">datum index of a tag</param>
		/// <returns>Tag manager for the referenced tag</returns>
		/// <see cref="DataArray{T}"/>
		public override TagManager this[Blam.DatumIndex tag_index] { get { return Array[tag_index]; } }

		#region Engine
		/// <summary>
		/// Engine version this tag index is for
		/// </summary>
		/// <seealso cref="Blam.CacheFile.EngineVersion"/>
		public override BlamVersion Engine { get { return cacheFile.EngineVersion; } }
		#endregion

		#region References
		/// <summary>
		/// Reference manger for this index
		/// </summary>
		/// <seealso cref="Blam.CacheFile.References"/>
		public override ReferenceManager References { get { return cacheFile.References; } }
		#endregion

		#region StringIdManager
		/// <summary>
		/// String ids manager for this index
		/// </summary>
		public override StringIdManager StringIds { get { return cacheFile.StringIds; } }
		#endregion

		#region Ctor
		/// <summary>
		/// Create a new tag index
		/// </summary>
		/// <param name="cache_file">cache file this tag index is managing</param>
		internal CacheTagIndex(Blam.CacheFile cache_file) : base()
		{
			cacheFile = cache_file;

			base.ArrayInitialize(kDefaultArrayName);
		}

		public override void Dispose() { Array.CloseAll(); }
		#endregion

		#region Add
		/// <summary>
		/// Open a tag in the cache file
		/// </summary>
		/// <param name="tag_datum">index of tag</param>
		/// <returns>
		/// The tag_index handle associated to the <see cref="TagManager"/> object used to open the tag, 
		/// or <see cref="Blam.DatumIndex.Null"/> if this operations fails
		/// </returns>
		public Blam.DatumIndex Open(Blam.DatumIndex tag_datum)
		{
			if (tag_datum == Blam.DatumIndex.Null)
				return Blam.DatumIndex.Null;


			Blam.CacheIndex.Item i = cacheFile.Index.Tags[tag_datum.Index];

			if (i.IsEmpty)
				throw new ArgumentNullException("tag_datum", string.Format("The tag handle {0} references an empty tag. We can't open this!", tag_datum.ToString()));

			// How can we open it if we don't know where it FUCKING IS YOU CUNT??
			if (i.Location != Blam.CacheIndex.ItemLocation.Internal && !i.IsFeignItem)
				return Blam.DatumIndex.Null;

			// Is this tag already loaded? if so, reuse handle
			Blam.DatumIndex di = IsLoaded(i);
			if (di != Blam.DatumIndex.Null)
			{
				Array.IncrementReference(di);
				return di;
			}

			TagManager tm = new TagManager(this);
			tm.ReferenceName = i.ReferenceName; // sync the tag manager's name with the cache item's

			bool group_tag_hack = false;
			BlamBuild build = Engine.ToBuild();
			// HACK: Halo1 PC uses gbx's variant of the model tag
			if (Engine == BlamVersion.Halo1_CE && i.GroupTag == Blam.Halo1.TagGroups.mode)
			{
				tm.Manage(Blam.Halo1.TagGroups.mod2);
				group_tag_hack = true;
			} else
			// HACK: Halo 2/3 use a the 'sound' group tag for their 'cache_file_sound' definitions
#if !NO_HALO2
			if (build == BlamBuild.Halo2 && i.GroupTag == Blam.Halo2.TagGroups.snd_)
			{
				tm.Manage(Blam.Halo2.TagGroups.shit);
				group_tag_hack = true;
			} else
#endif
#if !NO_HALO3
			if (build == BlamBuild.Halo3 && i.GroupTag == Blam.Halo3.TagGroups.snd_)
			{
				tm.Manage(Blam.Halo3.TagGroups.shit);
				group_tag_hack = true;
			} else
#endif
#if !NO_HALO_ODST
			if (build == BlamBuild.HaloOdst && i.GroupTag == Blam.Halo3.TagGroups.snd_)
			{
				tm.Manage(Blam.Halo3.TagGroups.shit);
				group_tag_hack = true;
			} else
#endif
#if !NO_HALO_REACH
			if (build == BlamBuild.HaloReach && i.GroupTag == Blam.HaloReach.TagGroups.snd_)
			{
				tm.Manage(Blam.HaloReach.TagGroups.shit);
				group_tag_hack = true;
			} else
#endif
#if !NO_HALO4
			if (build == BlamBuild.Halo4 && i.GroupTag == Blam.Halo4.TagGroups.snd_)
			{
				tm.Manage(Blam.Halo4.TagGroups.shit);
				group_tag_hack = true;
			} else
#endif
			tm.Manage(i.GroupTag);

			if (!i.IsFeignItem)
			{
				// We don't care about this shit when extracting or opening from a cache file
				const uint k_open_flags = IO.ITagStreamFlags.DontTrackTagManagerReferences | 
					IO.ITagStreamFlags.DontTrackTagReferencers;

				tm.Flags.Add(k_open_flags);

				try { tm.Read(i, cacheFile, group_tag_hack); }
				catch (Exception ex)
				{
					indexTrace.WriteLine("Cache Index: Failed to open tag from cache: {0}{1}'{2}.{3}'{4}{5}",
						cacheFile.InputStream.FileName, Program.NewLine,
						cacheFile.GetReferenceName(i), i.GroupTag.Name, Program.NewLine, ex);
					return Blam.DatumIndex.Null;
				}

				// Remove the flag we set before reading
				tm.Flags.Remove(k_open_flags);
			}

			tm.TagIndex = Array.Add(tm);
			base.OnEventOpen(new TagIndexEventArgs(this, tm));
			return tm.TagIndex;
		}

		/// <summary>
		/// Find a cache tag datum based on its name and group tag
		/// </summary>
		/// <param name="name">name of the tag</param>
		/// <param name="tag_group">group tag of tag</param>
		/// <returns>The tag_datum associated with the name\tag_group, or <see cref="Blam.DatumIndex.Null"/> if no match is found</returns>
		Blam.DatumIndex FindDatum(string name, TagInterface.TagGroup tag_group)
		{
			foreach(Blam.CacheIndex.Item i in cacheFile.Index.Tags)
			{
				if (cacheFile.GetReferenceName(i) == name && i.GroupTag.ID == tag_group.ID)
					return i.Datum;
			}

			return Blam.DatumIndex.Null;
		}

		/// <summary>
		/// Open an existing a tag
		/// </summary>
		/// <param name="name">name of the tag</param>
		/// <param name="tag_group">group tag of the tag</param>
		/// <returns>The tag_index handle associated with the <see cref="TagManager"/> used to load the tag</returns>
		public override Blam.DatumIndex Open(string name, TagInterface.TagGroup tag_group)
		{
			Blam.DatumIndex index = FindDatum(name, tag_group);
			if (index == Blam.DatumIndex.Null)
				indexTrace.WriteLine("Cache Index: Failed to find the following tag in the cache {0}.{1}{2}.{3}", cacheFile.Header.Name, Program.NewLine, name, tag_group.Name);

			return Open(index);
		}

		/// <summary>
		/// Opens the first instance of a specific tag group
		/// </summary>
		/// <param name="tag_group">Tag group to search for</param>
		/// <returns>Returns <see cref="Blam.DatumIndex.Null"/> if no <paramref name="tag_group"/> instances were found</returns>
		public Blam.DatumIndex OpenFirstInstance(TagInterface.TagGroup tag_group)
		{
			foreach (Blam.CacheIndex.Item i in cacheFile.Index.Tags)
				if (i.GroupTag == tag_group)
					return Open(i.Datum);

			return Blam.DatumIndex.Null;
		}
		#endregion

		#region Remove
		/// <summary>
		/// Updates the reference count for the tag,
		/// unloading the tag if there are no more references
		/// </summary>
		/// <param name="tag_index">index of the tag to unload</param>
		/// <returns>true if no longer in the tag index</returns>
		public override bool Unload(Blam.DatumIndex tag_index)
		{
			base.OnEventUnload(new TagIndexEventArgs(this, this[tag_index]));
			return Array.Remove(tag_index);
		}

		/// <summary>
		/// Updates the reference count for the tag and it's dependents, unloading the tag or any of it's dependents if there are no more references
		/// </summary>
		/// <param name="tag_index">index of the tag to unload</param>
		public override void UnloadWithDependents(Blam.DatumIndex tag_index)
		{
			base.OnEventUnloadWithDependents(new TagIndexEventArgs(this, this[tag_index]));
			UnloadWithDependentsRecursive(tag_index);
		}

		void UnloadWithDependentsRecursive(Blam.DatumIndex tag_index)
		{
			Managers.TagManager tagman = this[tag_index];
			foreach (Blam.DatumIndex di in tagman.References) UnloadWithDependentsRecursive(di);

			Unload(tag_index);
		}

		/// <summary>
		/// Removes the tag from the index completely, disregarding
		/// any other cases (IE, multiple references to the tag)
		/// </summary>
		/// <param name="tag_index">index of the tag to force the unload upon</param>
		public override void UnloadForce(Blam.DatumIndex tag_index)
		{
			base.OnEventUnloadForce(new TagIndexEventArgs(this, this[tag_index]));
			Array.Close(tag_index);
		}

		/// <summary>
		/// Removes the tag, along with any of it's loaded dependents from the index completely, 
		/// disregarding any other cases (IE, multiple references to the tag)
		/// </summary>
		/// <param name="tag_index">index of the tag to force the unload upon</param>
		public override void UnloadForceWithDependents(Blam.DatumIndex tag_index)
		{
			base.OnEventUnloadForceWithDependents(new TagIndexEventArgs(this, this[tag_index]));
			UnloadForceWithDependentsRecursive(tag_index);
		}

		void UnloadForceWithDependentsRecursive(Blam.DatumIndex tag_index)
		{
			Managers.TagManager tagman = this[tag_index];
			foreach (Blam.DatumIndex di in tagman.References) UnloadWithDependentsRecursive(di);

			UnloadForce(tag_index);
		}

		/// <summary>
		/// Removes all tags from the index completely, disregarding
		/// any other cases (IE, multiple references to the tag)
		/// </summary>
		public override void UnloadAll()
		{
			base.OnEventUnloadAll(new TagIndexEventArgs(this, null));
			Array.CloseAll();
		}
		#endregion

		#region Util
		/// <summary>
		/// Figures out if the tag defined by <paramref name="tag_datum"/> is loaded in the tag index
		/// </summary>
		/// <param name="tag_datum">tag item defined in this cache's tag index</param>
		/// <returns><see cref="Blam.DatumIndex.Null"/> if not loaded</returns>
		public Blam.DatumIndex IsLoaded(Blam.CacheIndex.Item tag_datum)
		{
			if (tag_datum.IsEmpty) return Blam.DatumIndex.Null;

			return IsLoaded(References[tag_datum.ReferenceName], tag_datum.GroupTag);
		}
		#endregion
	};
}