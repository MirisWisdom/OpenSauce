/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Xml;

namespace BlamLib.Managers
{
	/// <summary>
	/// Tag Index that is based on files
	/// </summary>
	/// <remarks>
	/// See the remarks in <see cref="ITagIndex"/>
	/// 
	/// <b>CASE NOTE:</b> An unhandled case within the tag index is when you load a tag, then later 
	/// reference that tag, but then unload it. Then trying to use that reference. This can either pan out 
	/// in two ways. 1) The datum at that tag_index is null, or 2) a new datum has consumed the identity of tag_index.
	/// Really, putting this object into those cases shouldn't be happening in any code properly designed as the 
	/// tag index is designed to be either a 'load everything and deal with it then' (in which case, dependents are used) 
	/// or 'load and deal with it as you go' (in which case, dependents aren't, or shouldn't rather, be used)
	/// 
	/// Examples of the first design are building a cache tag index, or creating a database for use when doing mass
	/// reference renames.
	/// An example of the second design is when performing fixups on specific tag definitions
	/// </remarks>
	public sealed class TagIndex : TagIndexBase
	{
		const string kDefaultArrayName = "tag instances";

		#region Sentinels
		/// <summary>
		/// Datum index value used to represent a missing tag
		/// </summary>
		public static readonly Blam.DatumIndex kMissing
			= new BlamLib.Blam.DatumIndex((ushort)0xDEAD, 0);
		/// <summary>
		/// Datum index value used to represent a skipped tag
		/// </summary>
		public static readonly Blam.DatumIndex kSkipped
			= new BlamLib.Blam.DatumIndex((ushort)0xBEEF, 0);
		/// <summary>
		/// Datum index value used to represent a tag who has invalid versioning
		/// </summary>
		public static readonly Blam.DatumIndex kVersionInvalid
			= new BlamLib.Blam.DatumIndex((ushort)0xCAFE, 0);

		/// <summary>
		/// Determines if the tag_index is marked up with any special sentinel values
		/// </summary>
		/// <param name="di">The tag_index handle</param>
		/// <returns>True if tag index is marked up</returns>
		public static bool IsSentinel(Blam.DatumIndex di)
		{
			if (
				di == kMissing ||
				di == kSkipped ||
				di == kVersionInvalid
				)
				return true;
			return false;
		}

        public static bool IsValid(Blam.DatumIndex tagDatum)
        {
            return !tagDatum.IsNull && !IsSentinel(tagDatum);
        }

		void SentinelException(Blam.DatumIndex tag_index, string operation)
		{
			if (tag_index == kMissing) throw new Debug.ExceptionLog("Tried to {0} a tag_index marked as '{1}'!", operation, "missing");
			else if (tag_index == kSkipped) throw new Debug.ExceptionLog("Tried to {0} a tag_index marked as '{1}'!", operation, "skipped");
			else if (tag_index == kVersionInvalid) throw new Debug.ExceptionLog("Tried to {0} a tag_index marked as '{1}'!", operation, "failed: invalid version data");
		}
		#endregion

		/// <summary>
		/// Indexer into this tag index
		/// </summary>
		/// <param name="tag_index">datum index of a tag</param>
		/// <returns>Tag manager for the referenced tag</returns>
		/// <see cref="DataArray{T}"/>
		public override TagManager this[Blam.DatumIndex tag_index] { 
			get {
				SentinelException(tag_index, "get");

				return Array[tag_index];
			}
		}

		#region Engine
		BlamVersion engine;
		/// <summary>
		/// Engine version this tag index is for
		/// </summary>
		public override BlamVersion Engine { get { return engine; } }
		#endregion

		#region References
		ReferenceManager refManager;
		/// <summary>
		/// Reference manger for this index
		/// </summary>
		public override ReferenceManager References { get { return refManager; } }


		Managers.ErrorTagDatabase errorDatabase = null;

		void ErrorDatabaseUpdate(TagManager tm)
		{
			if (!tm.ContainsBadReferences) return;

			if(errorDatabase == null) // If this is the first tag to have errors, we need to create the database now
				errorDatabase = Program.GetManager(engine).CreateErrorTagDatabase();

			// Add our 'root' tag, which isn't problematic
			errorDatabase.SetRoot(tm.Name, tm.GroupTag.ID);

			foreach(TagInterface.TagReference tr in tm.BadReferences)
			{
				ErrorTagDatabase.ErrorFlags ef;

				// determine what problem we had with this reference
				if (tr.Datum == kSkipped)					ef = ErrorTagDatabase.ErrorFlags.Skipped;
				else if (tr.Datum == kMissing)				ef = ErrorTagDatabase.ErrorFlags.Missing;
				else if (tr.Datum == kVersionInvalid)		ef = ErrorTagDatabase.ErrorFlags.InvalidVersion;
				else if (tr.Datum == Blam.DatumIndex.Null)	ef = ErrorTagDatabase.ErrorFlags.Failure;
				else throw new Debug.Exceptions.UnreachableException(tr.Datum);

				// add this reference to the database under our 'root' tag
				int hash_value = errorDatabase.AddDependent(refManager[tr.ReferenceId], tr.GroupTagInt, ef);

				errorDatabaseBadTagHashes.Add(hash_value);
			}
		}

		HashSet<int> errorDatabaseBadTagHashes = new HashSet<int>();
		void ErrorDatabaseAddLocalHack(string path, TagInterface.TagGroup tag_group)
		{
			int hash = path.GetHashCode() ^ unchecked((int)tag_group.ID);
			errorDatabaseBadTagHashes.Add(hash);
		}
		bool ErrorDatabaseContains(string path, TagInterface.TagGroup tag_group)
		{
			//return errorDatabase == null ? false : errorDatabase.Contains(path, tag_group.ID);
			if (errorDatabaseBadTagHashes.Count == 0) return false;

			int hash = path.GetHashCode() ^ unchecked((int)tag_group.ID);
			return errorDatabaseBadTagHashes.Contains(hash);
		}

		/// <summary>
		/// Save the current error tag database state
		/// </summary>
		/// <remarks>If there have been no errors so far, this does nothing</remarks>
		public void ErrorDatabaseSave()
		{
			if (errorDatabase == null) return;

			// Ex. database\20081220_1535(50)
			string name = string.Format(@"database\{0:yyyy}{0:MM}{0:dd}_{0:HH}{0:mm}({0:ss})", DateTime.Now);

			Blam.DatumIndex tag_index = this.Add(name, errorDatabase.Definition);
			this.Save(tag_index);
			// TODO: if 'Unload's code ever changes, this may cause some problems...
			refManager.Remove(this[tag_index].ReferenceName); // we don't want the name lingering either
			this.Unload(tag_index); // we don't want it lingering
		}
		#endregion

		#region StringIdManager
		StringIdManager stringIdManager = null;
		/// <summary>
		/// String ids manager for this index
		/// </summary>
		public override StringIdManager StringIds { get { return stringIdManager; } }

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

		#region Directory
		string directory;
		/// <summary>
		/// Directory for the tags this tag index for
		/// </summary>
		public string Directory { get { return directory; } }
		#endregion

		#region Ctor
		/// <summary>
		/// Create a new tag index
		/// </summary>
		/// <param name="version">Engine version for the tags</param>
		/// <param name="dir">Base directory for the tags</param>
		/// <param name="tag_dir">Name of the tags folder</param>
		/// <param name="create">Create the directory if it doesn't exist on disk</param>
		/// <remarks>"<paramref name="tag_dir"/>\" gets appended to <paramref name="dir"/>'s path.
		/// 
		/// If <paramref name="create"/> is false and full tags path doesn't exist on disk, this will create it
		/// </remarks>
		public TagIndex(BlamVersion version, string dir, string tag_dir, bool create) : base()
		{
			engine = version;
			directory = System.IO.Path.Combine(dir, tag_dir);
			if (!directory.EndsWith("\\"))	directory = directory + "\\";

			if (!System.IO.Directory.Exists(directory))
			{
				if (create) System.IO.Directory.CreateDirectory(directory);
				else throw new System.IO.DirectoryNotFoundException(directory);
			}

			base.ArrayInitialize(kDefaultArrayName);

			refManager = new ReferenceManager(version, string.Format("[TagIndex {0}]", directory), false);

			if (engine.UsesStringIds())
				StringTableInitialize();
		}
		/// <summary>
		/// Create a new tag index
		/// </summary>
		/// <param name="version">Engine version for the tags</param>
		/// <param name="dir">Base directory for the tags</param>
		/// <param name="create">Create the directory if it doesn't exist on disk</param>
		/// <remarks>"tags\" gets appended to <paramref name="dir"/>'s path.
		/// 
		/// If <paramref name="create"/> is false and <paramref name="dir"/> doesn't exist on disk, this will create it
		/// </remarks>
		public TagIndex(BlamVersion version, string dir, bool create) : this(version, dir, "tags", create)
		{
		}

		public override void Dispose()
		{
			UnloadAll();

			if (engine.UsesStringIds())
				StringTableDispose();
		}
		#endregion

		#region Error Events
		public class TagIndexErrorArgs : EventArgs
		{
			public string Message { get; private set; }
			public Exception ThrownException { get; private set; }

			public TagIndexErrorArgs(string message, Exception thrownException)
			{
				Message = message;
				ThrownException = thrownException;
			}

			public TagIndexErrorArgs(string message)
				: this(message, null)
			{ }
		}

		public event EventHandler<TagIndexErrorArgs> ErrorOccurred;

		private void OnErrorOccurred(Exception thrownException, string messageFormat, params object[] args)
		{
			indexTrace.WriteLine(messageFormat, args);

			var handler = ErrorOccurred;

			if (handler != null)
			{
				handler(this, new TagIndexErrorArgs(String.Format(messageFormat, args), thrownException));
			}
		}

		private void OnErrorOccurred(string message, params object[] args)
		{
			OnErrorOccurred(null, message, args);
		}
		#endregion

		#region Adding
		#region Add
		/// <summary>
		/// Fired <b>after</b> a tag is added (via either <see cref="TagIndex.New"/> or <see cref="TagIndex.Add"/>) to the index
		/// </summary>
		public event EventHandler<TagIndexEventArgs> EventAdd;

		private void OnEventAdd(TagIndexEventArgs args)
		{
			var handler = EventAdd;

			if (handler != null)
			{
				handler(this, args);
			}
		}

		/// <summary>
		/// Create a new tag
		/// </summary>
		/// <param name="name">name of the tag (relative to the directory this tag index uses)</param>
		/// <param name="tag_group">tag group for the tag</param>
		/// <returns>The tag_index handle associated with the <see cref="TagManager"/> used to create the tag</returns>
		/// <remarks>
		/// If <paramref name="tag_group"/> is setup to be ignored, we return <see cref="kSkipped"/>.
		/// If the name\tag_group tag already exists (either in memory or on disk), we return <see cref="Blam.DatumIndex.Null"/>.
		/// </remarks>
		public Blam.DatumIndex New(string name, TagInterface.TagGroup tag_group)
		{
			if (Ignore(tag_group)) return kSkipped;

			// Checks to see if the tag exists on disk
			string path = string.Format("{0}.{1}", Path.Combine(directory, name), tag_group.Name);
			if (Exists(path))
			{
				OnErrorOccurred("Tag Index: Couldn't create a new tag! '{0}' already exists on disk", path);
				return Blam.DatumIndex.Null;
			}

			// ...or if there is one in memory
			if (IsLoaded(name, tag_group) != Blam.DatumIndex.Null)
			{
				OnErrorOccurred("Tag Index: Couldn't create a new tag! '{0}.{1}' already exists", name, tag_group.Name);
				return Blam.DatumIndex.Null;
			}

			TagManager tm = new TagManager(this);
			tm.ReferenceName = refManager.Add(tm, tag_group, name);
			tm.Manage(tag_group); // setup the tag group
			tm.TagIndex = Array.Add(tm);

			base.OnEventOpen(new TagIndexEventArgs(this, tm));
			return tm.TagIndex;
		}

		/// <summary>
		/// Add an existing tag group definition to the index
		/// </summary>
		/// <param name="name">name of the tag (relative to the directory this tag index uses)</param>
		/// <param name="definition">Instance of a tag group definition</param>
		/// <returns>The tag_index handle associated with the <see cref="TagManager"/> now managing the definition</returns>
		/// <remarks>
		/// <paramref name="definition"/> must have a <see cref="TagInterface.TagGroupAttribute"/> applied to it.
		/// If <paramref name="tag_group"/> is setup to be ignored, we return <see cref="kSkipped"/>.
		/// If the name\tag_group tag already exists (in memory only), we return <see cref="Blam.DatumIndex.Null"/>.
		/// </remarks>
		public Blam.DatumIndex Add(string name, TagInterface.Definition definition)
		{
			TagInterface.TagGroupAttribute tga = definition.State.Attribute as TagInterface.TagGroupAttribute;

			TagInterface.TagGroup tag_group = definition.State.Engine.VersionToTagCollection()[tga.GroupIndex];

			if (Ignore(tag_group)) return kSkipped;

			// Checks to see or if there is one in memory
			if (IsLoaded(name, tag_group) != Blam.DatumIndex.Null)
			{
				OnErrorOccurred("Tag Index: Couldn't add an existing tag! '{0}.{1}' already exists", name, tag_group.Name);
				return Blam.DatumIndex.Null;
			}

			#region Initialize tag manager
			TagManager tm = new TagManager(this);
			tm.ReferenceName = refManager.Add(tm, tag_group, name);
			tm.Manage(definition);
			tm.TagIndex = Array.Add(tm);
			#endregion

			base.OnEventOpen(new TagIndexEventArgs(this, tm));
			return tm.TagIndex;
		}
		#endregion

		#region Open
		/// <summary>
		/// Open an existing tag
		/// </summary>
		/// <param name="name">name of the tag</param>
		/// <param name="tag_group">group tag of the tag</param>
		/// <returns>
		/// The tag_index handle associated with the <see cref="TagManager"/> used to load the tag, or <see cref="Blam.DatumIndex.Null"/> if this operations fails
		/// </returns>
		/// <remarks>Will return existing handles if tag is already open.</remarks>
		public override Blam.DatumIndex Open(string name, TagInterface.TagGroup tag_group) { return Open(name, tag_group, 0); }

		/// <summary>
		/// Open an existing tag
		/// </summary>
		/// <param name="name">name of the tag</param>
		/// <param name="tag_group">group tag of the tag</param>
		/// <param name="flags">special flags to use when opening</param>
		/// <returns>
		/// The tag_index handle associated with the <see cref="TagManager"/> used to load the tag, or <see cref="Blam.DatumIndex.Null"/> if this operations fails
		/// </returns>
		/// <remarks>
		/// Will return existing handles if tag is already open. 
		/// If <paramref name="tag_group"/> is setup to be ignored, we return <see cref="kSkipped"/>.
		/// </remarks>
		public Blam.DatumIndex Open(string name, TagInterface.TagGroup tag_group, uint flags)
		{
			// HACK: Halo1 PC uses gbx's variant of the model tag
			if (Engine == BlamVersion.Halo1_CE && tag_group == Blam.Halo1.TagGroups.mode)
			{
				tag_group = Blam.Halo1.TagGroups.mod2;
			}

			if (Ignore(tag_group)) return kSkipped;

			// Does this tag even exist on disk?
			string path = string.Format("{0}.{1}", Path.Combine(directory, name), tag_group.Name);
			if (!Exists(path)) return kMissing;

			// Is this tag already loaded? if so, reuse handle
			Blam.DatumIndex di = IsLoaded(name, tag_group);
			if (di != Blam.DatumIndex.Null)
			{
				Array.IncrementReference(di);
				return di;
			}

			// If the tag had errors, don't try loading it again.
			// Note that valid tags can exist in the error database since they act as root tags for problem 
			// tags, but since we call [IsLoaded] above this it will catch any valid cases
			if (ErrorDatabaseContains(name, tag_group))
				return Blam.DatumIndex.Null;

			#region Initialize tag manager
			TagManager tm = new TagManager(this);
			tm.ReferenceName = refManager.Add(tm, tag_group, name);
			tm.Flags.Add(flags);
			tm.Manage(tag_group);
			tm.TagIndex = di = Array.Add(tm); // in order to read the tag, the tag index must be setup first (due how TagManager.Path is setup)
			#endregion
			#region Stream tag manager
			tm.OpenForRead();
			try { tm.Read(); }
			catch (TagInterface.Exceptions.InvalidVersion /*ex_version*/)
			{
				//OnErrorOccurred(ex_version, "Tag Index: Failed to open tag from disk (invalid\\unhandled version): {0}", path);
				di = kVersionInvalid;
			}
			catch (Exception ex)
			{
				OnErrorOccurred(ex, "Tag Index: Failed to open tag from disk: {0}; Message: {1}", path, ex.Message);
				di = Blam.DatumIndex.Null;
			}
			finally { tm.Close(); }

			if(di != tm.TagIndex)
			{
				Array.Remove(tm.TagIndex);
				ErrorDatabaseAddLocalHack(name, tag_group);

				return di;
			}
			#endregion
			ErrorDatabaseUpdate(tm);

			base.OnEventOpen(new TagIndexEventArgs(this, tm));
			return tm.TagIndex;
		}
		#endregion
		#endregion

		#region Remove
		/// <summary>
		/// Updates the reference count for the tag, unloading the tag if there are no more references
		/// </summary>
		/// <param name="tag_index">index of the tag to unload</param>
		/// <returns>true if no longer in the tag index</returns>
		public override bool Unload(Blam.DatumIndex tag_index)
		{
			SentinelException(tag_index, "unload");

			base.OnEventUnload(new TagIndexEventArgs(this, this[tag_index]));

			//TagManager tm = this[tag_index];
			bool removed = Array.Remove(tag_index);
			//if(removed)
			//	refManager.Remove(tm.ReferenceName);

			return removed;
		}

		/// <summary>
		/// Updates the reference count for the tag and it's dependents, unloading the tag or any of it's dependents if there are no more references
		/// </summary>
		/// <param name="tag_index">index of the tag to unload</param>
		public override void UnloadWithDependents(Blam.DatumIndex tag_index)
		{
			SentinelException(tag_index, "unload (with dependents)");

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
			SentinelException(tag_index, "(force) unload");

			base.OnEventUnloadForce(new TagIndexEventArgs(this, this[tag_index]));

			//TagManager tm = this[tag_index];
			//refManager.Delete(tm.ReferenceName);
			Array.Close(tag_index);
		}

		/// <summary>
		/// Removes the tag, along with any of it's loaded dependents from the index completely, 
		/// disregarding any other cases (IE, multiple references to the tag)
		/// </summary>
		/// <param name="tag_index">index of the tag to force the unload upon</param>
		public override void UnloadForceWithDependents(Blam.DatumIndex tag_index)
		{
			SentinelException(tag_index, "(force) unload (with dependents)");

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
			refManager.Clear();
		}
		#endregion

		#region Util
		#region Save
		/// <summary>
		/// Fired <b>before</b> a tag is saved from this index
		/// </summary>
		public event EventHandler<TagIndexEventArgs> EventBeforeSave;
		void OnEventBeforeSave(TagIndexEventArgs args)	{ if (EventBeforeSave != null) EventBeforeSave(this, args); }
		/// <summary>
		/// Fired <b>after</b> a tag is saved from this index
		/// </summary>
		public event EventHandler<TagIndexEventArgs> EventAfterSave;
		void OnEventAfterSave(TagIndexEventArgs args)	{ if (EventAfterSave != null) EventAfterSave(this, args); }

		/// <summary>
		/// Save an existing a tag to file
		/// </summary>
		/// <param name="tag_index">index of tag to save</param>
		/// <returns>false if it can't save the tag</returns>
		public bool Save(Blam.DatumIndex tag_index)
		{
			SentinelException(tag_index, "save");

			if(!Array.Exists(tag_index)) return false;
			
			TagManager tm = Array[tag_index];
			TagIndexEventArgs eventargs = new TagIndexEventArgs(this, tm);

			OnEventBeforeSave(eventargs);
			try { tm.CreateForWrite(); tm.Write(); }
			catch (Exception ex) { indexTrace.WriteLine("Tag Index: Couldn't save tag: '{0}.{1}'{2}{3}", tm.Name, tm.GroupTag.Name, Program.NewLine, ex); return false; }
			OnEventAfterSave(eventargs);

			return true;
		}
		#endregion

		#region Exists
		/// <summary>
		/// Does the path exist on disk?
		/// </summary>
		/// <param name="path">path to check for existence</param>
		/// <returns></returns>
		static bool Exists(string path)
		{
			if (!File.Exists(path)) return false;
			return true;
		}

		/// <summary>
		/// Figures out if the tag exists in this tag index's namespace
		/// </summary>
		/// <param name="name">name of tag</param>
		/// <param name="tag_group">group tag of tag</param>
		/// <returns></returns>
		public bool Exists(string name, TagInterface.TagGroup tag_group)
		{
			return Exists(string.Format("{0}.{1}", Path.Combine(directory, name), tag_group.Name));
		}
		#endregion

		#region Paths
		/// <summary>
		/// Builds the relative file path to an existing tag
		/// </summary>
		/// <param name="tag_index">index of tag</param>
		/// <returns>(tag name).(group tag name)</returns>
		public string GetRelativePath(Blam.DatumIndex tag_index)
		{
			TagManager tm = this[tag_index];

			return string.Format("{0}.{1}", References[tm.ReferenceName], tm.GroupTag.Name);
		}

		internal string GetFullPath(TagManager tm)
		{
			return string.Format("{0}.{1}", System.IO.Path.Combine(directory, refManager[tm.ReferenceName]), tm.GroupTag.Name);
		}

		/// <summary>
		/// Builds the full file path to an existing tag
		/// </summary>
		/// <param name="tag_index">index of tag</param>
		/// <returns>(directory)\(tag name).(group tag name)</returns>
		public string GetFullPath(Blam.DatumIndex tag_index)
		{
			TagManager tm = this[tag_index];

			return GetFullPath(tm);
		}
		#endregion

		[System.Diagnostics.Conditional("DEBUG")]
		public void Dump(StreamWriter s)
		{
			const string format_header ="{0,5}\t{1,12}\t{2,16}\t{3}\t{4}";
			// {2} is 10 here because we also have ' bytes' which sums to 16
			const string format =		"{0,5}\t{1,12}\t{2,10} bytes\t{3}\t{4}";
			System.Globalization.NumberFormatInfo nfi = new System.Globalization.NumberFormatInfo();
			nfi.NumberDecimalDigits = 0;

			s.WriteLine("{0}\t{1}", engine, directory);
			s.WriteLine();
			s.WriteLine(format_header, "", "handle", "memory usage", "tag group".PadLeft(TagInterface.TagGroup.kLongestGroupNameLength), "name");
			s.WriteLine();
			int x = 0;
			int total_size = 0, total_cache_size = 0, size;
			foreach(TagManager tm in Array)
			{
				total_cache_size += tm.CalculateRuntimeSize(engine, true);
				size = tm.CalculateRuntimeSize(engine, false);
				total_size += size;

				s.WriteLine(format, x++, tm.TagIndex, size.ToString("N", nfi), tm.GroupTag.NameToLeftPaddedString(), refManager[tm.ReferenceName]);
			}
			s.WriteLine();
			s.WriteLine("Predicted Memory Usage Statistics");
			s.WriteLine("File Memory Usage : {0:X8} \\ {1} bytes", total_size, total_size.ToString("N", nfi));

			s.WriteLine("Cache Memory Usage: {0:X8} \\ {1} bytes", total_cache_size, total_cache_size.ToString("N", nfi));
		}

		[System.Diagnostics.Conditional("DEBUG")]
		public void DumpToXml()
		{
			using (var util = new TagIndexDumpUtil(this.engine, directory))
			{
				util.Setup(TagIndexDumpUtil.DumpFormat.Xml, TagIndexDumpUtil.DumpType.Memory);

				util.TagInstancesBegin();
				foreach (TagManager tm in Array)
				{
					util.TagInstanceBegin(tm.TagIndex, refManager[tm.ReferenceName], tm.GroupTag);
					util.TagInstanceMemoryUsageAdd(tm.CalculateRuntimeSize(engine, false), tm.CalculateRuntimeSize(engine, true));
					util.TagInstanceEnd();
				}
				util.TagInstancesEnd();
			}
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="archive"></param>
		/// <param name="custom_output_directory">
		/// True if <paramref name="archive"/> also contains the output directory, false 
		/// if <see cref="Directory"/> should be used instead
		/// </param>
		public void WinRar(string archive, bool custom_output_directory)
		{
			Debug.Assert.If(archive != null && archive != "");
			if (!Windows.HasWinRar) return;

			string base_directory = !custom_output_directory ? directory :
				Path.GetDirectoryName(archive);

			string listpath = Path.Combine(base_directory, "winrar_tagindex.lst");
			using(System.IO.StreamWriter listfile = new StreamWriter(listpath))
			{
				listfile.WriteLine("// Game={0}", engine);
				listfile.WriteLine("// OriginalDirectory=\"{0}\"", directory);
				foreach (TagManager tm in Array)
					listfile.WriteLine("{0}", GetRelativePath(tm.TagIndex));
					//listfile.WriteLine("{0,-256};", GetRelativePath(tm.TagIndex));
			}

			string cmd_line =
				" u" + // commands: update files, 
				" -cl -m5 -s \"-z{1}\"" + // switches: lower case filenames, best compression, solid archive
				" \"{0}\"" +
				" \"@{1}\""
			;

			#region winrar process
			System.Diagnostics.Process process = new System.Diagnostics.Process();
			process.StartInfo.UseShellExecute = true;
			process.StartInfo.CreateNoWindow = false;
			process.StartInfo.FileName = Windows.WinRarExePath;
			process.StartInfo.Arguments = string.Format(cmd_line, archive, listpath);
			process.StartInfo.WorkingDirectory = directory;
			process.Start();

			Console.WriteLine(process.StartInfo.Arguments);
			while (!process.HasExited) System.Threading.Thread.Sleep(1000);

			if (process.ExitCode != 0)
			{
				indexTrace.WriteLine("Tag Index: winrar result: {0}. for '{1}' in '{2}'",
					process.ExitCode, archive, directory);
				indexTrace.WriteLine(process.StartInfo.Arguments);
			}
			#endregion

			System.IO.File.Delete(listpath);
		}

		#region Tag Database
		TagDatabase ToDatabase()
		{
			TagDatabase db = Program.GetManager(engine).CreateTagDatabase();
			foreach (TagManager tm in Array)
			{
				db.SetRoot(tm);
				foreach (Blam.DatumIndex di in tm.References)
				{
					TagManager dep = Array[di];
					db.AddDependent(dep);
				}
			}

			return db;
		}

		public void ToDatabase(string db_name)
		{
			TagDatabase db = ToDatabase();

			using (TagManager tm = new TagManager())
			{
				tm.Engine = this.engine;
				tm.Manage(db.Definition);
				tm.CreateForWriteHack(db_name);
				tm.Write();
			}
		}

		public void PrintDatabase(string output_name)
		{
			Debug.Assert.If(output_name != null && output_name != "");
			string output_path = System.IO.Path.Combine(directory, output_name + ".txt");

			const string format_parent = "{0}\t{1}";
			const string format_child = "\t" + format_parent;
			using (var s = new System.IO.StreamWriter(output_path))
			{
				s.WriteLine("; Game={0}", engine);
				s.WriteLine("; OriginalDirectory=\"{0}\"", directory);

				foreach (TagManager tm in Array)
				{
					s.WriteLine(format_parent, tm.GroupTag.Name, tm.Name);
					foreach (Blam.DatumIndex di in tm.References)
					{
						TagManager dep = Array[di];
						s.WriteLine(format_child, dep.GroupTag.NameToLeftPaddedString(), dep.Name);
					}
					s.Flush();
				}
			}
		}

		public void PrintDatabase()
		{
			using (var util = new TagIndexDumpUtil(this.engine, directory))
			{
				util.Setup(TagIndexDumpUtil.DumpFormat.Xml, TagIndexDumpUtil.DumpType.Dependencies);

				util.TagInstancesBegin();
				foreach (TagManager tm in Array)
				{
					util.TagInstanceBegin(tm.TagIndex, refManager[tm.ReferenceName], tm.GroupTag);
					util.TagInstanceDependentsBegin();
					foreach (Blam.DatumIndex di in tm.References)
					{
						TagManager dep = Array[di];
						util.TagInstanceDependentAdd(dep.Name, dep.GroupTag);
					}
					util.TagInstanceDependentsEnd();
					util.TagInstanceEnd();
				}
				util.TagInstancesEnd();
			}
		}
		#endregion

		#region SharedReferences
		List<Blam.DatumIndex> FindSharedReferencesFromEqualIndex(TagIndex other)
		{
			List<Blam.DatumIndex> ref_names = refManager.FindSharedReferencesFromEqualManager(other.refManager);
			List<Blam.DatumIndex> tag_indices = new List<BlamLib.Blam.DatumIndex>(ref_names.Count);

			foreach (TagManager tm in Array)
				if (ref_names.Contains(tm.ReferenceName))
					tag_indices.Add(tm.TagIndex);

			return tag_indices;
		}

		public List<Blam.DatumIndex> FindSharedReferences(TagIndex other)
		{
			if (other == null || this.engine.ToBuild() != other.engine.ToBuild()) return null;

			// this conditioning doesn't really matter right now with
			// how ReferenceManager does the searching
			//if (this.ignoreList == null || this.ignoreList.IsSameCollection(other.ignoreList))
			//	return FindSharedReferencesFromEqualIndex(other);

			return FindSharedReferencesFromEqualIndex(other);
		}

		[System.Diagnostics.Conditional("DEBUG")]
		public void DumpSharedReferences(StreamWriter s, TagIndex other)
		{
			List<Blam.DatumIndex> tag_indices = FindSharedReferences(other);
			if (tag_indices == null) return;

			foreach(Blam.DatumIndex di in tag_indices)
				s.WriteLine("{0}", GetRelativePath(di));
		}

		[System.Diagnostics.Conditional("DEBUG")]
		public void DumpSharedReferences(TagIndex other)
		{
			List<Blam.DatumIndex> tag_indices = FindSharedReferences(other);
			if (tag_indices == null) return;

			using (var util = new TagIndexDumpUtil(this.engine, directory))
			{
				util.Setup(TagIndexDumpUtil.DumpFormat.Xml, TagIndexDumpUtil.DumpType.SharedReferences);

				//util.SharedReferencesAddLeftIndex();
				//util.SharedReferencesAddRightIndex();
				util.SharedReferencesBegin();
				foreach (Blam.DatumIndex di in tag_indices)
				{
					TagManager tm = Array[di];
					util.TagInstanceBegin(tm.TagIndex, refManager[tm.ReferenceName], tm.GroupTag);
					util.TagInstanceEnd();
				}
				util.SharedReferencesEnd();
			}
		}
		#endregion

		#region DumpNonTagFiles
		void DumpNonTagFilesRecursive(StreamWriter s, System.IO.DirectoryInfo diri, TagInterface.TagGroupCollection coll)
		{
			System.IO.FileInfo[] files = diri.GetFiles();
			System.IO.DirectoryInfo[] directories = diri.GetDirectories();
			if (files.Length == 0 && directories.Length == 0)
			{
				s.WriteLine(";empty directory {0}", diri.FullName.Replace(directory, ""));
				s.WriteLine();
			}
			else
			{
				bool is_first = true;
				foreach (System.IO.FileInfo fi in files)
				{
					string ext;
					if (fi.Extension != "")
						ext = fi.Extension.Remove(0, 1).ToLowerInvariant(); // remove the '.'
					else continue;

					if (coll.FindGroupIndex(ext) == -1)
					{
						if (is_first)
						{
							s.WriteLine(";{0}", diri.FullName.Replace(directory, ""));
							is_first = false;
						}

						s.WriteLine(fi.FullName.Replace(directory, ""));
					}
				}

				// if we wrote some entries, add a new line
				if (!is_first) s.WriteLine();

				foreach (System.IO.DirectoryInfo di in directories)
					DumpNonTagFilesRecursive(s, di, coll);
			}
		}

		public void DumpNonTagFiles(StreamWriter s)
		{
			System.IO.DirectoryInfo diri = new System.IO.DirectoryInfo(directory);

			DumpNonTagFilesRecursive(s, diri, engine.VersionToTagCollection());
		}

		void DumpNonTagFilesRecursive(TagIndexDumpUtil util, System.IO.DirectoryInfo diri, TagInterface.TagGroupCollection coll)
		{
			System.IO.FileInfo[] files = diri.GetFiles();
			System.IO.DirectoryInfo[] directories = diri.GetDirectories();
			if (files.Length == 0 && directories.Length == 0)
			{
				util.NonTagsDirectoryBegin(diri.FullName.Replace(directory, ""), true);
				util.NonTagsDirectoryEnd();
			}
			else
			{
				bool is_first = true;
				foreach (System.IO.FileInfo fi in files)
				{
					string ext;
					if (fi.Extension != "")
						ext = fi.Extension.Remove(0, 1).ToLowerInvariant(); // remove the '.'
					else continue;

					if (coll.FindGroupIndex(ext) == -1)
					{
						if (is_first)
						{
							util.NonTagsDirectoryBegin(diri.FullName.Replace(directory, ""), false);
							is_first = false;
						}

						util.NonTagsEntryAdd(fi.FullName.Replace(directory, ""));
					}
				}

				// if we wrote some entries, close the directory dump...
				if (!is_first) util.NonTagsDirectoryEnd();

				foreach (System.IO.DirectoryInfo di in directories)
					DumpNonTagFilesRecursive(util, di, coll);
			}
		}

		public void DumpNonTagFiles()
		{
			using (var util = new TagIndexDumpUtil(this.engine, directory))
			{
				util.Setup(TagIndexDumpUtil.DumpFormat.Xml, TagIndexDumpUtil.DumpType.NonTags);

				System.IO.DirectoryInfo diri = new System.IO.DirectoryInfo(directory);

				util.NonTagsBegin();
				DumpNonTagFilesRecursive(util, diri, engine.VersionToTagCollection());
				util.NonTagsEnd();
			}
		}
		#endregion
		#endregion
	};

	class TagIndexDumpUtil : IDisposable
	{
		[Flags]
		public enum DumpType : uint
		{
			Dependencies =		(1<<0),
			SharedReferences =	(1<<1),
			Memory =			(1<<2),
			NonTags =			(1<<3),
		};

		public enum DumpFormat
		{
			Text,
			Xml,
		};

		#region Providers
		interface IProvider : IDisposable
		{
			#region Dump
			void SharedReferencesBegin();
			void SharedReferencesAddLeftIndex(string name, TagInterface.TagGroup group);
			void SharedReferencesAddRightIndex(string name, TagInterface.TagGroup group);
			void SharedReferencesEnd();

			void NonTagsBegin();
			void NonTagsEntryAdd(string filename);
			void NonTagsDirectoryBegin(string dir, bool empty);
			void NonTagsDirectoryEnd();
			void NonTagsEnd();
			#endregion

			#region TagInstance
			void TagInstancesBegin();

			void TagInstanceBegin(Blam.DatumIndex handle, string name, TagInterface.TagGroup group);

			void TagInstanceDependentsBegin();

			void TagInstanceDependentAdd(string name, TagInterface.TagGroup group);

			void TagInstanceDependentsEnd();

			void TagInstanceMemoryUsageAdd(int predicted_file_size, int predicted_cache_size);

			void TagInstanceEnd();

			void TagInstancesEnd();
			#endregion
		};

		class XmlProvider : IProvider
		{
			XmlWriter tagsWriter;

			public XmlProvider(string file)
			{
				tagsWriter = new XmlTextWriter(file, Encoding.ASCII);
			}

			public void Dispose()
			{
				tagsWriter.Close();
				tagsWriter = null;
			}

			void MemoryUsageAdd(int predicted_file_size, int predicted_cache_size)
			{
				tagsWriter.WriteStartElement("memoryUsage");

				tagsWriter.WriteStartElement("predictedFile");
				tagsWriter.WriteAttributeString("sizeInBytes", predicted_file_size.ToString("X8"));
				tagsWriter.WriteEndElement();

				tagsWriter.WriteStartElement("predictedCache");
				tagsWriter.WriteAttributeString("sizeInBytes", predicted_cache_size.ToString("X8"));
				tagsWriter.WriteEndElement();

				tagsWriter.WriteEndElement();
			}

			#region Dump
			public void SharedReferencesBegin()
			{
				tagsWriter.WriteStartElement("shared");
			}

			public void SharedReferencesAddLeftIndex(string name, TagInterface.TagGroup group)
			{
				TagInstanceBegin(Blam.DatumIndex.Null, name, group);
				TagInstanceEnd();
			}

			public void SharedReferencesAddRightIndex(string name, TagInterface.TagGroup group)
			{
				TagInstanceBegin(Blam.DatumIndex.Null, name, group);
				TagInstanceEnd();
			}

			public void SharedReferencesEnd()
			{
				tagsWriter.WriteEndElement();
			}


			public void NonTagsBegin()
			{
				tagsWriter.WriteStartElement("nonTagFiles");
			}

			public void NonTagsEntryAdd(string filename)
			{
				tagsWriter.WriteStartElement("entry");
				tagsWriter.WriteAttributeString("name", filename);
				tagsWriter.WriteEndElement();
			}

			public void NonTagsDirectoryBegin(string dir, bool empty)
			{
				tagsWriter.WriteStartElement("directory");
				tagsWriter.WriteAttributeString("name", dir);
				if(empty) tagsWriter.WriteAttributeString("isEmpty", bool.TrueString);
			}

			public void NonTagsDirectoryEnd()
			{
				tagsWriter.WriteEndElement();
			}

			public void NonTagsEnd()
			{
				tagsWriter.WriteEndElement();
			}
			#endregion

			#region TagInstance
			public void TagInstancesBegin()
			{
				tagsWriter.WriteStartElement("tags");
			}

			public void TagInstanceBegin(Blam.DatumIndex handle, string name, TagInterface.TagGroup group)
			{
				tagsWriter.WriteStartElement("tag");
				tagsWriter.WriteAttributeString("group", group.Name);
				tagsWriter.WriteAttributeString("name", name);
			}

			public void TagInstanceDependentsBegin()
			{
				tagsWriter.WriteStartElement("dependents");
			}

			public void TagInstanceDependentAdd(string name, TagInterface.TagGroup group)
			{
				TagInstanceBegin(Blam.DatumIndex.Null, name, group);
				TagInstanceEnd();
			}

			public void TagInstanceDependentsEnd()
			{
				tagsWriter.WriteEndElement();
			}

			public void TagInstanceMemoryUsageAdd(int predicted_file_size, int predicted_cache_size)
			{
				MemoryUsageAdd(predicted_file_size, predicted_cache_size);
			}

			public void TagInstanceEnd()
			{
				tagsWriter.WriteEndElement();
			}

			public void TagInstancesEnd()
			{
				tagsWriter.WriteEndElement();
			}
			#endregion
		};

		/// <remarks>Multiple <see cref="DumpType"/> flags not supported (yet)!</remarks>
		class TextProvider : IProvider
		{
			static System.Globalization.NumberFormatInfo nfi;
			static TextProvider()
			{
				nfi = new System.Globalization.NumberFormatInfo();
				nfi.NumberDecimalDigits = 0;
			}

			StreamWriter writer;
			DumpType type;

			public TextProvider(string file, DumpType type)
			{
				writer = new StreamWriter(file, false, Encoding.ASCII);
				this.type = type;
			}

			public void Dispose()
			{
				writer.Dispose();
				writer = null;
			}

			int currentIndent;
			void WriteIndention()
			{
				for (int x = 0; x < currentIndent; x++)
					writer.Write('\t');
			}


			#region Dump
			public void SharedReferencesBegin()													{ writer.WriteLine("; Shared Info"); }
			public void SharedReferencesAddLeftIndex(string name, TagInterface.TagGroup group)	{ writer.WriteLine(";\t{0}{1}", group.NameToLeftPaddedString(), name); }
			public void SharedReferencesAddRightIndex(string name, TagInterface.TagGroup group)	{ writer.WriteLine(";\t{0}{1}", group.NameToLeftPaddedString(), name); }
			public void SharedReferencesEnd()													{ writer.WriteLine(); }

			public void NonTagsBegin()						{ writer.WriteLine("; Non-Tags"); currentIndent = 0; }
			public void NonTagsEntryAdd(string filename)	{ WriteIndention(); writer.WriteLine("\t{0}", filename); }
			public void NonTagsDirectoryBegin(string dir, bool empty)
			{
				writer.Write(';');
				WriteIndention();
				if (!empty)
					writer.WriteLine("\t{0}", dir);
				else
					writer.WriteLine("\tempty directory {0}", dir);
				currentIndent++;
			}
			public void NonTagsDirectoryEnd()
			{
				if (--currentIndent == 0)
					writer.WriteLine();
			}
			public void NonTagsEnd()						{ writer.WriteLine(); }
			#endregion

			#region TagInstance
			const string TagInstancesMemoryFormatHeader =	";{0,5}\t{1,12}\t{2,16}\t{3,16}\t{4}\t{5}";
			// {2} is 10 here because we also have ' bytes' which sums to 16
			const string TagInstancesMemoryFormat =			"{0,5}\t{1,12}\t{2,10} bytes\t{3,10} bytes\t{4}\t{5}";

			int currentInstanceIndex;
			public void TagInstancesBegin()
			{
				currentInstanceIndex = 0;
				currentIndent = 0;

				if (type == DumpType.Memory)
				{
					predictedFileSize = predictedCacheSize = 0;

					writer.WriteLine(TagInstancesMemoryFormatHeader,
						"", "handle", "file memory", "cache memory", "tag group".PadLeft(TagInterface.TagGroup.kLongestGroupNameLength), "name");
				}
			}

			Blam.DatumIndex currentInstanceHandle;
			string currentInstanceName, currentInstanceGroup;
			public void TagInstanceBegin(Blam.DatumIndex handle, string name, TagInterface.TagGroup group)
			{
				switch (type)
				{
					case DumpType.Dependencies:
						WriteIndention();
						break;

					case DumpType.SharedReferences:
						break;

					case DumpType.Memory:
						currentInstanceHandle = handle;
						currentInstanceName = name;
						currentInstanceGroup = group.NameToLeftPaddedString();
						break;
				}

				currentInstanceIndex++;
				currentIndent++;
			}

			public void TagInstanceDependentsBegin()
			{
			}

			public void TagInstanceDependentAdd(string name, TagInterface.TagGroup group)
			{
				TagInstanceBegin(Blam.DatumIndex.Null, name, group);
				TagInstanceEnd();
			}

			public void TagInstanceDependentsEnd()
			{
				//writer.WriteLine();
			}

			int predictedFileSize, predictedCacheSize;
			public void TagInstanceMemoryUsageAdd(int predicted_file_size, int predicted_cache_size)
			{
				switch (type)
				{
					case DumpType.Memory:
						writer.WriteLine(TagInstancesMemoryFormat,
							currentInstanceIndex.ToString(), 
							currentInstanceHandle.ToString(), 
							predicted_file_size.ToString("N", nfi),
							predicted_cache_size.ToString("N", nfi),
							currentInstanceGroup, currentInstanceName);

						predictedFileSize += predicted_file_size;
						predictedCacheSize += predicted_cache_size;
						break;
				}
			}

			public void TagInstanceEnd()
			{
				if(--currentIndent == 0)
					writer.WriteLine();
			}

			public void TagInstancesEnd()
			{
				switch (type)
				{
					case DumpType.Dependencies:
						break;

					case DumpType.SharedReferences:
						break;

					case DumpType.Memory:
						writer.WriteLine();
						writer.WriteLine("; Predicted Memory Usage Statistics");
						writer.WriteLine("File Memory Usage : {0:X8} \\ {1} bytes", predictedFileSize, predictedFileSize.ToString("N", nfi));
						writer.WriteLine("Cache Memory Usage: {0:X8} \\ {1} bytes", predictedCacheSize, predictedCacheSize.ToString("N", nfi));
						break;
				}
				writer.WriteLine();
			}
			#endregion
		};
		#endregion

		IProvider provider;

		#region Engine
		private BlamVersion engine;
		/// <summary>
		/// Engine version this tag index util is for
		/// </summary>
		public BlamVersion Engine { get { return engine; } }
		#endregion

		string directory;
		string file;
		DumpType type;
		DumpFormat format;

		public TagIndexDumpUtil(BlamVersion engine, string directory)
		{
			this.engine = engine;
			this.directory = directory;
		}

		public void Setup(DumpFormat format, DumpType type)
		{
			this.format = format;
			this.type = type;

			switch(format)
			{
				case DumpFormat.Text:
					file = Path.Combine(directory, string.Format("TagIndexDump.{0:yyyy}{0:MM}{0:dd}_{0:HH}{0:mm}({0:ss}).{1}", DateTime.Now, "txt"));
					provider = new TextProvider(file, type);
					break;

				case DumpFormat.Xml:
					file = Path.Combine(directory, string.Format("TagIndexDump.{0:yyyy}{0:MM}{0:dd}_{0:HH}{0:mm}({0:ss}).{1}", DateTime.Now, "xml"));
					provider = new XmlProvider(file);
					break;
			}
		}

		public void Dispose()
		{
			provider.Dispose();
			provider = null;
		}

		#region Dump
		public void SharedReferencesBegin()													{ provider.SharedReferencesBegin(); }
		public void SharedReferencesAddLeftIndex(string name, TagInterface.TagGroup group)	{ provider.SharedReferencesAddLeftIndex(name, group); }
		public void SharedReferencesAddRightIndex(string name, TagInterface.TagGroup group)	{ provider.SharedReferencesAddRightIndex(name, group); }
		public void SharedReferencesEnd()													{ provider.SharedReferencesEnd(); }

		public void NonTagsBegin()									{ provider.NonTagsBegin(); }
		public void NonTagsEntryAdd(string filename)				{ provider.NonTagsEntryAdd(filename); }
		public void NonTagsDirectoryBegin(string dir, bool empty)	{ provider.NonTagsDirectoryBegin(dir, empty); }
		public void NonTagsDirectoryEnd()							{ provider.NonTagsDirectoryEnd(); }
		public void NonTagsEnd()									{ provider.NonTagsEnd(); }
		#endregion

		#region TagInstance
		public void TagInstancesBegin()																	{ provider.TagInstancesBegin(); }
		public void TagInstanceBegin(Blam.DatumIndex handle, string name, TagInterface.TagGroup group)	{ provider.TagInstanceBegin(handle, name, group); }
		public void TagInstanceDependentsBegin()														{ provider.TagInstanceDependentsBegin(); }
		public void TagInstanceDependentAdd(string name, TagInterface.TagGroup group)					{ provider.TagInstanceDependentAdd(name, group); }
		public void TagInstanceDependentsEnd()															{ provider.TagInstanceDependentsEnd(); }
		public void TagInstanceMemoryUsageAdd(int predicted_file_size, int predicted_cache_size)		{ provider.TagInstanceMemoryUsageAdd(predicted_file_size, predicted_cache_size); }
		public void TagInstanceEnd()																	{ provider.TagInstanceEnd(); }
		public void TagInstancesEnd()																	{ provider.TagInstancesEnd(); }
		#endregion
	};
}