/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;
using BlamLib.TagInterface;

namespace BlamLib.Managers
{
	/// <summary>
	/// Owner object for endian streams on tags
	/// </summary>
	public sealed class TagManager : IReferenceMangerObject, IStructureOwner, IO.ITagStream, IDisposable, ITagDatabaseAddable
	{
		#region OwnerId
		Blam.DatumIndex ownerId = Blam.DatumIndex.Null;
		/// <summary>
		/// Handle to the owner <see cref="ITagIndex"/> of this tag manager
		/// </summary>
		public Blam.DatumIndex OwnerId	{ get { return ownerId; } }
		#endregion

		#region TagIndex
		Blam.DatumIndex tagIndex = Blam.DatumIndex.Null;
		/// <summary>
		/// Handle of this manager within the owner <see cref="ITagIndex"/>
		/// </summary>
		public Blam.DatumIndex TagIndex
		{
			get { return tagIndex; }
			internal set { tagIndex = value; }
		}
		#endregion

		#region OwnerObject
		/// <summary>
		/// Returns null. TagManager is the highest level in the <see cref="IStructureOwner"/> chain
		/// </summary>
		public IStructureOwner OwnerObject { get { return null; } internal set { } }
		#endregion


		#region Flags
		Util.Flags flags = new Util.Flags(0);
		/// <summary>
		/// Special flags
		/// </summary>
		public Util.Flags Flags	{ get { return flags; } }

		/// <summary>
		/// Does this tag reside in a cache file?
		/// </summary>
		public bool InCache { get { return flags.Test(IO.ITagStreamFlags.ResidesInCacheFile); } }
		#endregion

		#region References
		#region Valid References
		HashSet<Blam.DatumIndex> references = null;
		/// <summary>
		/// Add a tag datum handle of a tag existing in this tag's <see cref="OwnerId"/> to this tag's list of child references
		/// </summary>
		/// <param name="child_tag_index">Handle to a child tag in this tag's <see cref="OwnerId"/></param>
		public void ReferencesAdd(Blam.DatumIndex child_tag_index)
		{
			if (references == null) references = new HashSet<Blam.DatumIndex>(Blam.DatumIndex.kEqualityComparer);
			references.Add(child_tag_index);
		}

		/// <summary>
		/// Determines if <paramref name="tag_index"/> is referenced by anything in this tag
		/// </summary>
		/// <param name="tag_index">Handle to a tag in this tag's <see cref="OwnerId"/></param>
		/// <returns>True if <paramref name="tag_index"/> is a child reference</returns>
		public bool ReferencesContains(Blam.DatumIndex tag_index)	{ if (references != null) return references.Contains(tag_index); return false; }

		/// <summary>
		/// Resets the list of valid references we were tracking
		/// </summary>
		public void ReferencesClear()								{ if (references != null) { references.Clear(); references = null; } }

		/// <summary>
		/// All the valid tag references this tag has
		/// </summary>
		/// <remarks>Datum Indexers are relative to <see cref="OwnerId"/></remarks>
		public IEnumerable<Blam.DatumIndex> References {
			get {
				if (references == null) yield break;

				foreach (Blam.DatumIndex di in references) yield return di;
			}
		}
		#endregion

		#region Bad References
		List<TagReference> badReferences = null;
		/// <summary>
		/// Add a tag reference that is a child field of this tag's <see cref="TagDefinition"/> to this tag's list of bad child references
		/// </summary>
		/// <param name="tag_reference">Child field of this tag which contains a faulty tag reference</param>
		public void BadReferencesAdd(TagReference tag_reference)
		{
			if (badReferences == null) badReferences = new List<TagReference>();
			badReferences.Add(tag_reference);
		}

		/// <summary>
		/// Does this tag contain any bad references?
		/// </summary>
		public bool ContainsBadReferences	{ get { if (badReferences != null) return badReferences.Count != 0; return false; } }

		/// <summary>
		/// Resets the list of bad references we were tracking
		/// </summary>
		public void BadReferencesClear()	{ if (badReferences != null) { badReferences.Clear(); badReferences = null; } }

		/// <summary>
		/// All reference fields in this definition whose data can't be resolved
		/// </summary>
		public IEnumerable<TagReference> BadReferences {
			get {
				if(badReferences == null) yield break;

				foreach (TagReference tr in badReferences) yield return tr;
			}
		}
		#endregion
		#endregion

		#region Engine
		BlamVersion engine;
		/// <summary>
		/// Engine version this tag belongs to
		/// </summary>
		public BlamVersion Engine
		{
			get { return engine; }
			internal set { engine = value; }
		}
		#endregion

		#region Path
		/// <summary>
		/// The tag name of this tag, relative to the the <see cref="OwnerId"/> or null 
		/// if there is no owner.
		/// </summary>
		public string Name
		{
			get
			{
				if(ownerId != Blam.DatumIndex.Null)	return Program.GetTagIndex(ownerId).References[referenceName];
				else								return null; // if owner is null, then something is messed up...

//				string value = string.Format("{0}\\{1}",
//					System.IO.Path.GetDirectoryName(path),
//					System.IO.Path.GetFileNameWithoutExtension(path));
//				return value.Substring(value.IndexOf("tags\\") + 5);
			}
		}

		/// <summary>
		/// The full file path of this tag on disk.
		/// </summary>
		string Path { get { return ((Managers.TagIndex)Program.GetTagIndex(ownerId)).GetFullPath(this); } }

		#region ReferenceName
		Blam.DatumIndex referenceName = Blam.DatumIndex.Null;
		/// <summary>
		/// Handle for the <see cref="ReferenceManager"/> in the <see cref="OwnerId"/> object 
		/// which stores all the reference names
		/// </summary>
		/// <remarks>This is <see cref="Blam.DatumIndex.Null"/> if this instance has no owner</remarks>
		public Blam.DatumIndex ReferenceName
		{
			get { return referenceName; }
			internal set { referenceName = value; }
		}
		#endregion
		#endregion

		#region EndianState
		IO.EndianState endianState;
		/// <summary>
		/// Byte ordering for the tag file
		/// </summary>
		public IO.EndianState EndianState { get { return endianState; } }

		void DetermineEndianState(bool in_cache)
		{
			if (!in_cache) // for tag files
			{
				// NOTE: We store Halo3, etc in big-endian because we don't know their entire formats 
				// yet and don't want to fuck up any data if we extract
				// Also, the only platform for the game is the 360, so hell, why not?

				if ((engine & BlamVersion.Halo1) != 0)			this.endianState = IO.EndianState.Big;
				else if ((engine & BlamVersion.Halo2) != 0)		this.endianState = IO.EndianState.Little;
				else if ((engine & BlamVersion.Halo3) != 0)		this.endianState = IO.EndianState.Big;
				else if ((engine & BlamVersion.Stubbs) != 0)	this.endianState = IO.EndianState.Big;
				else if ((engine & BlamVersion.HaloOdst) != 0)	this.endianState = IO.EndianState.Big;
				else if ((engine & BlamVersion.HaloReach) != 0)	this.endianState = IO.EndianState.Big;
				else											this.endianState = IO.EndianState.Big;
			}
			else // for cache files
			{
				if (engine.IsXbox360())							this.endianState = IO.EndianState.Big;
				else											this.endianState = IO.EndianState.Little;
			}
		}
		#endregion

		#region TagDefinition
		TagGroupAttribute tagGroupAttr;
		TagGroup tagGroup;
		/// <summary>
		/// Group tag for this tag instance
		/// </summary>
		public TagGroup GroupTag { get { return tagGroup; } }

		Definition tagDefinition;
		/// <summary>
		/// Definition data of the this tag instance
		/// </summary>
		public Definition TagDefinition { get { return tagDefinition; } }
		#endregion

		#region Manage
		void ManageSetupTagGroup() { if (engine != BlamVersion.Unknown) tagGroup = engine.VersionToTagCollection()[tagGroupAttr.GroupIndex]; }

		/// <summary>
		/// Attaches this tag manager to the specified tag definition
		/// </summary>
		/// <param name="tag_def"></param>
		/// <remarks>don't pull any funny tricks and pass a non-tag-group definition....</remarks>
		public void Manage(Definition tag_def)
		{
			tag_def.SetupState(); // just in-case
			if (tagDefinition != null) tagDefinition = null;

			tagDefinition = tag_def;
			tagGroupAttr = tag_def.Attribute as TagGroupAttribute;

			ManageSetupTagGroup();
			tag_def.SetOwnerObject(this);
		}

		/// <summary>
		/// Manages a new tag of type <paramref name="tag_group" />
		/// </summary>
		/// <param name="tag_group">Tag group which we'll be handling</param>
		public void Manage(TagGroup tag_group)
		{
			if (tagDefinition != null) tagDefinition = null;
			if(tag_group.Definition == null)
				Debug.Assert.If(false, "Tried to manage a tag using a group ({2}) with no definition. {0}:{1}", engine, this.Name, tag_group);

			tagDefinition = tag_group.Definition.NewInstance(null);
			tagGroupAttr = TagGroupAttribute.FromType(tagDefinition.GetType());
			tagGroup = tag_group;
			tagDefinition.SetOwnerObject(this);
		}

		/// <summary>
		/// If the tag manager upgraded the tag, this will set <paramref name="tag_def"/> to 
		/// the object we're now managing, then clears the <see cref="IO.ITagStreamFlags.DefinitionWasUpgraded"/> bit 
		/// in <see cref="Flags"/>
		/// </summary>
		/// <typeparam name="T"><see cref="BlamLib.TagInterface.Definition"/></typeparam>
		/// <param name="tag_def"></param>
		public void Resync<T>(ref T tag_def) where T : Definition
		{
			if (flags.Test(IO.ITagStreamFlags.DefinitionWasUpgraded))
			{
				tag_def = (T)tagDefinition;
				flags.Remove(IO.ITagStreamFlags.DefinitionWasUpgraded);
			}
		}
		#endregion

		#region Ctor
		void Reset(ITagIndex owner, BlamVersion version)
		{
			if (owner != null)
			{
				ownerId = owner.IndexId;
				flags.Add(owner is CacheTagIndex, IO.ITagStreamFlags.ResidesInCacheFile);
			}

			tagIndex = Blam.DatumIndex.Null;
			headerGroupTag = uint.MaxValue;
			headerVersion = -1;
			headerSignature = uint.MaxValue;
			tagGroupAttr = null;
			tagGroup = null;
			tagDefinition = null;

			Close();

			engine = version;

			DetermineEndianState(InCache);
		}

		/// <summary>
		/// Resets a tag manager based on a tag index and a file path
		/// </summary>
		/// <param name="owner">Index this tag belong to</param>
		/// <remarks>Generally used for creating\writing</remarks>
		void Reset(ITagIndex owner)		{ Reset(owner, owner.Engine); }

		/// <summary>
		/// Resets a tag manager based on a tag index and a 
		/// endian state for the internal tag I\O streams
		/// </summary>
		/// <param name="owner">Index this tag belong to</param>
		/// <param name="endian_state">The endian format of the tag file</param>
		/// <remarks>Generally used for creating\writing</remarks>
		void Reset(ITagIndex owner, IO.EndianState endian_state) { Reset(owner); this.endianState = endian_state; }


		/// <summary>
		/// Construct a primitive tag manager (nothing is setup, ie engine version)
		/// </summary>
		public TagManager() { Reset(null, BlamVersion.Unknown); }
		
		/// <summary>
		/// Construct a tag manager which is an instance owned by the specified tag index
		/// </summary>
		/// <param name="owner">Index this tag belongs to</param>
		internal TagManager(ITagIndex owner) { Reset(owner, owner.Engine); }
		#endregion

		#region Create\Open\Close
		void SetupBaseAddress()
		{
			if (InputStream != null) InputStream.BaseAddress = 0x40;
			if (OutputStream != null) OutputStream.BaseAddress = 0x40;
		}

		static void CreatePathIfNoneExists(string path)
		{
			string dir = System.IO.Path.GetDirectoryName(path);
			if (!System.IO.Directory.Exists(dir)) System.IO.Directory.CreateDirectory(dir);
		}
		static void CreatePathAndFileIfNoneExists(string path)
		{
			CreatePathIfNoneExists(path);

			if (!System.IO.File.Exists(path)) System.IO.File.Create(path).Close();
		}

		string CreateInternal()
		{
			string path = this.Path;

			CreatePathAndFileIfNoneExists(path);

			Close();

			return path;
		}

		/// <summary>
		/// Creates the tag file
		/// </summary>
		/// <remarks>Closes existing tag streams</remarks>
		public void Create()
		{
			if (InCache) return;
			CreateInternal();
		}

		/// <summary>
		/// Creates the tag file and opens it for writing
		/// </summary>
		/// <remarks>Closes existing tag streams</remarks>
		public void CreateForWrite()
		{
			if (InCache) return;
			string path = CreateInternal();

			OutputStream = new BlamLib.IO.EndianWriter(path, endianState, this, true);
			OutputStream.AutoFlush = false;

			SetupBaseAddress();
		}

		internal void CreateForWriteHack(string path)
		{
			DetermineEndianState(false);
			CreatePathAndFileIfNoneExists(path);

			OutputStream = new BlamLib.IO.EndianWriter(path, endianState, this, true);
			OutputStream.AutoFlush = false;

			SetupBaseAddress();
		}

		/// <summary>
		/// Creates the tag file and opens it for reading and writing
		/// </summary>
		/// <remarks>Closes existing tag streams</remarks>
		public void CreateForReadWrite()
		{
			if (InCache) return;
			string path = CreateInternal();

			InputStream = new BlamLib.IO.EndianReader(path, endianState, this);
			OutputStream = new BlamLib.IO.EndianWriter(path, endianState, this, true);
			OutputStream.AutoFlush = false;

			SetupBaseAddress();
		}

		/// <summary>
		/// Opens a file to store the extracted tag in a cache
		/// </summary>
		/// <param name="base_dir">Root directory</param>
		/// <param name="name_override">Optional, if not null, this is the name of the file we store the tag in</param>
		/// <remarks>This object MUST be managing a cache tag item</remarks>
		public void OpenForExtract(string base_dir, string name_override)
		{
			if (!InCache) return;
			DetermineEndianState(false);

			string path = System.IO.Path.Combine(base_dir, name_override ?? Name) + "." + tagGroup.Name;
			CreatePathIfNoneExists(path);

			OutputStream = new BlamLib.IO.EndianWriter(path, endianState, this, true);
			OutputStream.AutoFlush = false;

			SetupBaseAddress();
		}

		/// <summary>
		/// Opens the tag file for reading
		/// </summary>
		/// <remarks>Closes existing tag streams</remarks>
		public void OpenForRead()
		{
			if (InCache) return;
			Close();

			string path = this.Path;
			InputStream = new BlamLib.IO.EndianReader(path, endianState, this);

			SetupBaseAddress();
		}

		/// <summary>
		/// Opens the tag file for writing
		/// </summary>
		/// <remarks>Closes existing tag streams</remarks>
		public void OpenForWrite()
		{
			if (InCache) return;
			Close();

			string path = this.Path;
			OutputStream = new BlamLib.IO.EndianWriter(path, endianState, this);
			OutputStream.AutoFlush = false;

			SetupBaseAddress();
		}

		/// <summary>
		/// Opens the tag file for reading and writing
		/// </summary>
		/// <remarks>Closes existing tag streams</remarks>
		public void OpenForReadWrite()
		{
			if (InCache) return;
			Close();

			string path = this.Path;
			InputStream = new BlamLib.IO.EndianReader(path, endianState, this);
			OutputStream = new BlamLib.IO.EndianWriter(path, endianState, this);
			OutputStream.AutoFlush = false;

			SetupBaseAddress();
		}

		/// <summary>
		/// Closes the tag manager's streams
		/// </summary>
		public void Close()
		{
			//if (InCache) return;
			if (this.InputStream != null) { InputStream.Close(); InputStream = null; }
			if (this.OutputStream != null) { OutputStream.Close(); OutputStream = null; }
		}
		#endregion

		#region I\O
		IO.EndianReader InputStream;
		IO.EndianWriter OutputStream;

		IO.EndianReader IO.ITagStream.GetInputStream() { return InputStream; }
		IO.EndianWriter IO.ITagStream.GetOutputStream() { return OutputStream; }


		uint headerGroupTag;
		short headerVersion;
		uint headerSignature;


		/// <summary>
		/// Tries to figure out what build of the engine the tag is 
		/// for from a little endian read group tag signature
		/// </summary>
		/// <param name="group_tag"></param>
		/// <returns></returns>
		BlamBuild FromSignature(uint group_tag)
		{
			// since halo 1 tags are stored in big endian, reading
			// the signature will result in the endian stream
			// swapping it since its expecting a group tag in little
			// endian format
			if		(group_tag == Blam.MiscGroups.malb.ID)		return BlamBuild.Halo1;
			else if (group_tag == Blam.MiscGroups.blam2a.ID)	return BlamBuild.Halo2;
			else if (group_tag == Blam.MiscGroups.blam2b.ID)	return BlamBuild.Halo2;
			else if (group_tag == Blam.MiscGroups.blam2c.ID)	return BlamBuild.Halo2;
			else if (group_tag == Blam.MiscGroups.blam2d.ID)	return BlamBuild.Halo2;
			else if (group_tag == Blam.MiscGroups.blm2.ID)		return BlamBuild.Halo2;
			else if (group_tag == Blam.MiscGroups.blm3.ID)		return BlamBuild.Halo3;
			else if (group_tag == Blam.MiscGroups.stub.ID)		return BlamBuild.Stubbs;
			else												return BlamBuild.Unknown;
		}

		/// <summary>
		/// Takes the input stream and does any version specific
		/// data operations to get the tag manager ready for reading
		/// </summary>
		void CalculateVersion()
		{
			InputStream.Seek(60);
			InputStream.State = BlamLib.IO.EndianState.Little;
			uint group_tag = InputStream.ReadTagUInt();
			BlamBuild bb = FromSignature(group_tag);

			#region Endian handling
			if (bb == BlamBuild.Halo1)	endianState = BlamLib.IO.EndianState.Big;
			else						endianState = BlamLib.IO.EndianState.Little;

			if (InputStream != null)	InputStream.State = endianState;
			if (OutputStream != null)	OutputStream.State = endianState;
			#endregion

			if (engine == BlamVersion.Unknown)
			{
				engine = bb.ToVersion();

				ManageSetupTagGroup();
			}
		}

		/// <summary>
		/// Read and preprocess header information
		/// </summary>
		void ReadPreprocess()
		{
			InputStream.Seek(4 + 32, System.IO.SeekOrigin.Begin);
			headerGroupTag = InputStream.ReadTagUInt();
			InputStream.Seek(4 + 4 + 8, System.IO.SeekOrigin.Current);
			headerVersion = InputStream.ReadInt16();
			InputStream.Seek(2, System.IO.SeekOrigin.Current);
			headerSignature = InputStream.ReadTagUInt();

			if ((engine & BlamVersion.Halo2) != 0)
			{
				if(Blam.MiscGroups.Halo2TagSignatureIsOldFieldsetHeaderFormat(headerSignature))
					flags.Add(IO.ITagStreamFlags.Halo2OldFormat_Fieldset);
				if (Blam.MiscGroups.Halo2TagSignatureIsOldStringId(headerSignature))
					flags.Add(IO.ITagStreamFlags.Halo2OldFormat_StringId);
				if (Blam.MiscGroups.Halo2TagSignatureUsesUselessPadding(headerSignature))
					flags.Add(IO.ITagStreamFlags.Halo2OldFormat_UselessPadding);
			}

			if(tagGroup.ID != headerGroupTag)
				throw new Exceptions.InvalidTagHeader(this.Path, tagGroup.TagToString(), new string(TagGroup.FromUInt(headerGroupTag)));

			if (tagGroupAttr.TagVersion != headerVersion)
			{
				Debug.LogFile.WriteLine("Version mismatch for {0}: {1}, should be {2}",
					this.Path,
					headerVersion,
					tagGroupAttr.Version);
				throw new Exceptions.InvalidVersion(this, tagGroupAttr.TagVersion, headerVersion);
			}
		}

		/// <summary>
		/// Read the tag from the supplied I\O stream
		/// </summary>
		public void Read()
		{
			if (InputStream == null)
				Debug.Assert.If(false, "Tried to read a tag that we didn't open. {0}:{1}", engine, this.Path);

			if(engine == BlamVersion.Unknown) CalculateVersion();

			ReadPreprocess();
			bool needs_upgrading = false;

			// Read past the tag field set header of the tag group
			if (DefinitionState.FieldSetRequiresVersioningHeader(this))
			{
				IFieldSetVersioning fs = tagDefinition.State.FieldSetReadVersion(this, FieldType.Tag, InputStream.PositionUnsigned);

				if (needs_upgrading = fs.NeedsUpgrading)
				{
					int index, size_of;
					fs.GetUpgradeParameters(out index, out size_of);

					if (!fs.UseImplicitUpgrading)
					{
						tagDefinition = tagDefinition.NewInstance((tagDefinition as IStructureOwner).OwnerObject, index, size_of);
						flags.Add(IO.ITagStreamFlags.DefinitionWasUpgraded);
					}
					else
						tagDefinition.VersionImplicitUpgradeBegin(size_of, this);
				}
			}

			tagDefinition.Read(this);

			if (needs_upgrading)
				if (tagDefinition.VersionImplicitUpgradeIsActive)
					tagDefinition.VersionImplicitUpgradeEnd();
				else
				{
#if DEBUG
					if(!tagDefinition.Upgrade())
						Debug.Assert.If(false, "Failed to upgrade tag group. {0} {1}", tagDefinition.GetType().Name, tagDefinition.ToVersionString());
#else
					tagDefinition.Upgrade();
#endif
				}
		}

		/// <summary>
		/// Read the tag from the supplied I\O stream, then close streams
		/// </summary>
		public void QuickLoad()
		{
			OpenForRead();
			try { Read(); }
			catch (Exception ex) { Debug.LogFile.WriteLine("Failed to open & read tag. {0}{1}{2}", this.Path, Program.NewLine, ex); }
			finally { Close(); }
		}

		/// <summary>
		/// Little hack for <see cref="CacheTagIndex"/>
		/// </summary>
		/// <param name="item"></param>
		/// <param name="cache"></param>
		/// <param name="ignore_group_hack">Hack used for Halo 2/3 where the sound definitions use different group tags</param>
		internal void Read(Blam.CacheIndex.Item item, Blam.CacheFile cache, bool ignore_group_hack)
		{
			Debug.Assert.If(InCache, "Tried to read a cache tag while we were NOT in a cache. {0}:{1}.{2}", engine, Name, tagGroup.Name);
			// HACK: for Halo 2/3 sound groups
			if (!ignore_group_hack && tagGroup.ID != item.GroupTag.ID)
				Debug.Assert.If(false, "Tried to read a cache tag with a non-matching cache tag datum. {0}:{1} [{2} !{3}",
					engine, Name, tagGroup.TagToString(), item.GroupTag.TagToString());

			cache.InputStream.Seek(item.Offset);

			VersionCtorAttribute upgrade_parameters;
			bool needs_upgrading = false;
			if (cache.EngineVersion.SupportsTagVersioning())
			{
				if ((upgrade_parameters = tagDefinition.VersionForEngine(cache.EngineVersion)) != null)
				{
					needs_upgrading = true;
					// It is ASSUMED that the group tag won't ever be needed for version construction
					tagDefinition = tagDefinition.NewInstance((tagDefinition as IStructureOwner).OwnerObject, upgrade_parameters.Major, upgrade_parameters.Minor);
					flags.Add(IO.ITagStreamFlags.DefinitionWasUpgraded);
				}
				else
					needs_upgrading = false;
			}

			tagDefinition.Read(cache);

			if (needs_upgrading)
#if DEBUG
				if (!tagDefinition.Upgrade())
					Debug.Assert.If(false, "Failed to upgrade tag group. {0} {1}", tagDefinition.GetType().Name, tagDefinition.ToVersionString());
#else
				tagDefinition.Upgrade();
#endif
		}

		/// <summary>
		/// Write the header information
		/// </summary>
		void WritePreprocess()
		{
			OutputStream.Write(new byte[4 + 32]);
			OutputStream.WriteTag(tagGroup.ID);
			OutputStream.Write(0); OutputStream.Write(64); OutputStream.Write(0); OutputStream.Write(0);
			OutputStream.Write(tagGroupAttr.TagVersion);
			engine.Write(OutputStream);
			OutputStream.WriteTag(Blam.MiscGroups.VersionToTag(engine, false));
		}

		void WriteImpl()
		{
			WritePreprocess();

			// Write the tag field set header for the tag group
			if (DefinitionState.FieldSetRequiresVersioningHeader(this))
				tagDefinition.State.FieldSetWriteVersion(this, FieldType.Tag);

			if (!tagDefinition.PostProcessGroup(false))
				Debug.Warn.If(false, "{0} failed to postprocess. {1}.{2}", tagDefinition.GetType().FullName, this.Name, this.tagGroup.Name);
			tagDefinition.Write(this);
		}

		/// <summary>
		/// Write the tag to the supplied I\O stream
		/// </summary>
		public void Write()
		{
			Debug.Assert.If(!InCache, "Tried to write a tag while we were in a cache. {0}:{1}.{2}", engine, Name, tagGroup.Name);
			Debug.Assert.If(OutputStream, "Tried to write a tag that we didn't open. {0}:{1}", engine, Name);

			WriteImpl();
		}

		/// <summary>
		/// Extract a tag to the supplied I\O stream
		/// </summary>
		/// <remarks>MUST call <see cref="OpenForExtract"/> first</remarks>
		public void Extract()
		{
			Debug.Assert.If(InCache, "Tried to extract a tag while we weren't in a cache. {0}:{1}.{2}", engine, Name, tagGroup.Name);
			Debug.Assert.If(OutputStream, "Tried to extract a tag that we didn't initialize. {0}:{1}.{2}", engine, Name, tagGroup.Name);

			WriteImpl();
		}
		#endregion

		#region Util
		bool CompareTagGroup(TagGroup tag_group)
		{
			// we even want to catch cases were we are asking if this is a 'object' or 'unit' tag
			return this.tagGroup == tag_group || tag_group.IsChild(this.tagGroup);
		}

		/// <summary>
		/// Determines if the supplied data matches this tag
		/// </summary>
		/// <param name="path">complete name (not path) to tag (without extension)</param>
		/// <param name="tag_group"></param>
		/// <returns></returns>
		/// <remarks>
		/// If <paramref name="tag_group"/> is a parent of the group tag of this tag, 
		/// the <see cref="TagGroup"/> comparison will return true.
		/// </remarks>
		public bool Compare(string path, TagGroup tag_group)
		{
			if (CompareTagGroup(tag_group) && this.Name == path) return true;

			return false;
		}

		internal bool Compare(TagIndexBase caller, string path, TagGroup tag_group)
		{
			if (CompareTagGroup(tag_group) && caller.References[referenceName] == path) return true;

			return false;
		}

		/// <summary>
		/// Calculate how much memory this tag definition consumes in native memory conditions
		/// </summary>
		/// <param name="engine">Engine build we're calculating for</param>
		/// <param name="cache">Is this memory in a cache?</param>
		/// <returns>Size of memory in bytes</returns>
		/// <see cref="TagInterface.Definition.CalculateRuntimeSize"/>
		public int CalculateRuntimeSize(BlamVersion engine, bool cache)
		{
			int val = tagDefinition.CalculateRuntimeSize(engine, cache);
			val += cache ? 0 : 0x40; // tag file header
			val += !cache && engine.UsesFieldSetVersionHeader() ? 16 : 0;

			return val;
		}

		/// <summary>
		/// Get a string which can be used for detailing exceptions with operations that are related to this tag
		/// </summary>
		/// <returns></returns>
		public string GetExceptionDescription()
		{
			return string.Format("tag file: {0}.{1}", Name, GroupTag.Name);
		}
		#endregion

		#region IDisposable Members
		public void Dispose()
		{
			tagGroupAttr = null;
			tagGroup = null;
			tagDefinition = null;

			ReferencesClear();
			BadReferencesClear();

			Close();
		}
		#endregion

		#region IReferenceMangerObject Members
		Blam.DatumIndex IReferenceMangerObject.ReferenceId			{ get { return this.referenceName; } }

		Blam.DatumIndex IReferenceMangerObject.ParentReferenceId	{ get { return Blam.DatumIndex.Null; } }

		IEnumerable<Blam.DatumIndex> IReferenceMangerObject.GetReferenceIdEnumerator()	{ yield break; }

		bool IReferenceMangerObject.UpdateReferenceId(ReferenceManager manager, Blam.DatumIndex new_datum)
		{
			this.referenceName = new_datum;

			return true;
		}
		#endregion

		#region ITagDatabaseAddable Members
		string ITagDatabaseAddable.GetTagName()		{ return Name; }

		char[] ITagDatabaseAddable.GetGroupTag()	{ return tagGroup.Tag; }
		#endregion
	};
}