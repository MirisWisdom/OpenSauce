/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Xml;

namespace BlamLib.Managers
{
	/// <summary>
	/// Defines constants and types for a general blam variant (eg Halo 1, Halo 2)
	/// </summary>
	public abstract class BlamDefinition
	{
		#region Localization
		/// <summary>
		/// Defines the constants for a locale type for a game engine
		/// </summary>
		public struct LocalizationType
		{
			#region Enumeration
			private int enumeration;
			/// <summary>
			/// Enumeration value for this locale type
			/// </summary>
			public int Enumeration { get { return enumeration; } }
			#endregion

			#region Name
			private string name;
			/// <summary>
			/// String value for this locale type
			/// </summary>
			public string Name { get { return name; } }
			#endregion

			#region LangName
			private string langName;
			/// <summary>
			/// 
			/// </summary>
			public string LangName { get { return langName; } }
			#endregion

			#region LangId
			private int langId;
			/// <summary>
			/// 
			/// </summary>
			public int LangId { get { return langId; } }
			#endregion

			#region LocalName
			private string localName;
			/// <summary>
			/// 
			/// </summary>
			public string LocalName { get { return localName; } }
			#endregion

			#region LocalId
			private int localId;
			/// <summary>
			/// 
			/// </summary>
			public int LocalId { get { return localId; } }
			#endregion

			/// <summary>
			/// Stream the definition data from XML data
			/// </summary>
			/// <param name="s"></param>
			public void Read(IO.XmlStream s)
			{
				s.ReadAttribute("id", 16, ref enumeration);
				s.ReadAttribute("name", ref name);

				s.ReadAttribute("langName", ref langName);
				s.ReadAttribute("langId", 16, ref langId);

				s.ReadAttribute("localName", ref localName);
				s.ReadAttribute("localId", 16, ref localId);
			}
		};
		/// <summary>
		/// Defines the constants about the localization system for a game engine
		/// </summary>
		public sealed class LocalizationList
		{
			#region Types
			private LocalizationType[] types;
			/// <summary>
			/// Various locale types
			/// </summary>
			public LocalizationType[] Types { get { return types; } }
			#endregion

			internal LocalizationList() {}

			/// <summary>
			/// Stream the definition data from XML data
			/// </summary>
			/// <param name="s">XML stream</param>
			public void Read(IO.XmlStream s)
			{
				types = new LocalizationType[s.Cursor.ChildNodes.Count];
				int index = 0;
				foreach (XmlNode n in s.Cursor.ChildNodes)
				{
					if (n.Name != "entry") continue;

					s.SaveCursor(n);
					(types[index++] = new LocalizationType()).Read(s);
					s.RestoreCursor();
				}
			}
		};
		#endregion

		#region Cache
		/// <summary>
		/// Defines the constants for a cache type for a specific game implementation
		/// </summary>
		public sealed class CacheType
		{
			#region Enumeration
			private int enumeration;
			/// <summary>
			/// Enumeration value for this cache type
			/// </summary>
			public int Enumeration { get { return enumeration; } }
			#endregion

			#region Version
			private int version;
			/// <summary>
			/// cache version this type supports
			/// </summary>
			public int Version { get { return version; } }
			#endregion

			#region MaxSize
			private int maxSize;
			/// <summary>
			/// Maximum size for this cache type
			/// </summary>
			public int MaxSize { get { return maxSize; } }
			#endregion

			#region Name
			private string name;
			/// <summary>
			/// String value for this cache type
			/// </summary>
			public string Name { get { return name; } }
			#endregion

			#region IsResource
			private bool isResource;
			/// <summary>
			/// Its a resource cache if its not a fully playable map (ie
			/// you can load it into a campaign or mp game)
			/// </summary>
			public bool IsResource { get { return isResource; } }
			#endregion

			internal CacheType() {}

			/// <summary>
			/// Stream the definition data from XML data
			/// </summary>
			/// <param name="s"></param>
			public void Read(IO.XmlStream s)
			{
				s.ReadAttribute("id", 16, ref enumeration);
				s.ReadAttribute("version", 10, ref version);
				s.ReadAttribute("name", ref name);
				s.ReadAttribute("maxSize", 16, ref maxSize);
				s.ReadAttribute("isResource", ref isResource);
			}
		};
		/// <summary>
		/// Defines the constants about the cache system for a game
		/// </summary>
		public sealed class CacheTypeList
		{
			#region Extension
			private string ext;
			/// <summary>
			/// File extension for the cache files
			/// </summary>
			public string Extension { get { return ext; } }
			#endregion

			#region BaseAddress
			private uint baseAddress;
			/// <summary>
			/// Base address in memory where the tag data begins at
			/// </summary>
			public uint BaseAddress { get { return baseAddress; } }
			#endregion

			#region BaseAddressBsp
			private uint baseAddressBsp;
			/// <summary>
			/// Base address in memory where the bsp tag data begins at
			/// </summary>
			public uint BaseAddressBsp { get { return baseAddressBsp; } }
			#endregion

			#region Types
			private CacheType[] types;
			/// <summary>
			/// Various cache types
			/// </summary>
			public CacheType[] Types { get { return types; } }
			#endregion

			internal CacheTypeList() {}

			/// <summary>
			/// Get a cache definition based on a supplied cache type name
			/// </summary>
			/// <param name="cache_type_name"></param>
			/// <returns>Cache definition linked to the supplied name, or null if not defined</returns>
			public CacheType this[string cache_type_name]
			{
				get {
					foreach (CacheType ct in types) if (ct.Name == cache_type_name) return ct;
					return null;
				}
			}

			/// <summary>
			/// Stream the definition data from XML data
			/// </summary>
			/// <param name="s">XML stream</param>
			public void Read(IO.XmlStream s)
			{
				s.ReadAttribute("ext", ref ext);
				s.ReadAttribute("baseAddress", 16, ref baseAddress);
				s.ReadAttribute("baseAddressBsp", 16, ref baseAddressBsp);

				XmlNode list = null;
				#region find types element
				foreach (XmlNode n in s.Cursor.ChildNodes)
					if (n.Name == "types")
					{
						list = n;
						break;
					}
				#endregion
				Debug.Assert.If(list != null, "Game has no cache types! {0}", ((BlamDefinition)s.Owner).Name.ToString());
				#region load cache types
				types = new CacheType[list.ChildNodes.Count];
				int index = 0;
				foreach (XmlNode n in list.ChildNodes)
				{
					if (n.Name != "type") continue;

					s.SaveCursor(n);
					(types[index++] = new CacheType()).Read(s);
					s.RestoreCursor();
				}
				#endregion
			}
		};
		#endregion

		/// <summary>
		/// Defines constants about the tag system for a game
		/// </summary>
		public sealed class Tags
		{
			/// <summary>
			/// Represents a tag that is required by a Game
			/// </summary>
			public struct Required
			{
				public readonly TagInterface.TagGroup GroupTag;
				public readonly string Name;

				public Required(IO.XmlStream s, TagInterface.TagGroupCollection tgc)
				{
					Name = null;
					s.ReadAttribute("groupTag", ref Name); // hell, we don't use it yet, so why not save some stack?
					GroupTag = tgc.FindGroup(Name);
					Debug.Assert.If(GroupTag != null, "Couldn't find a tag group! {0}", Name);

					s.ReadAttribute("name", ref Name);
				}
			};

			#region MaxCount
			private int maxCount = -1;
			/// <summary>
			/// Max allowed tags that can be loaded
			/// </summary>
			public int MaxCount { get { return maxCount; } }
			#endregion

			#region MaxBufferSize
			private int maxBufferSize = -1;
			/// <summary>
			/// Max allowed size of a tag buffer (flat) in memory
			/// </summary>
			public int MaxBufferSize { get { return maxBufferSize; } }
			#endregion

			#region RequiredTags
			private List<Required> requiredTags = new List<Required>();
			/// <summary>
			/// Tags needed to run or build a cache
			/// </summary>
			public ICollection<Required> RequiredTags { get { return requiredTags; } }
			#endregion

			internal Tags() {}

			/// <summary>
			/// Stream the definition data from XML data
			/// </summary>
			/// <param name="s">XML stream</param>
			/// <param name="engine">Engine this listing is for</param>
			public void Read(IO.XmlStream s, BlamVersion engine)
			{
				s.ReadAttribute("maxCount", 16, ref maxCount);
				s.ReadAttribute("maxBufferSize", 16, ref maxBufferSize);

				TagInterface.TagGroupCollection tgc = engine.VersionToTagCollection();

				foreach (XmlNode n in s.Cursor.ChildNodes)
				{
					if (n.Name == "required")
					{
						Required req;
						foreach (XmlNode req_n in n.ChildNodes)
						{
							if (req_n.Name != "entry") continue;

							s.SaveCursor(req_n);
							req = new Required(s, tgc);
							requiredTags.Add(req);
							s.RestoreCursor();
						}
					}
				}
			}
		};

		#region Game
		/// <summary>
		/// Delegate for identifying resources for a game
		/// </summary>
		/// <param name="owner"></param>
		/// <param name="resource_name"></param>
		/// <param name="resource_path"></param>
		internal delegate void IdentifyResource(Game owner, string resource_name, string resource_path);
		/// <summary>
		/// Interface for resources managed by game definitions
		/// </summary>
		public interface IGameResource
		{
			/// <summary>
			/// Load this resource from the specified manifest file into memory
			/// </summary>
			/// <param name="path">Type path to the file</param>
			/// <param name="name">File name (with extension)</param>
			/// <returns>true if successful</returns>
			bool Load(string path, string name);
			/// <summary>
			/// Unload this resource from memory
			/// </summary>
			void Close();
		}
		/// <summary>
		/// Defines a specific game (ie Halo 1 Xbox)
		/// </summary>
		public sealed class Game
		{
			const int ResourceDefaultCapacity = 4;

			#region Owner
			BlamDefinition owner;
			/// <summary>
			/// Definition that owns this game
			/// </summary>
			public BlamDefinition Owner	{ get { return owner; } }
			#endregion

			#region Engine
			BlamVersion engine = BlamVersion.Unknown;
			/// <summary>
			/// Engine variant this game uses
			/// </summary>
			public BlamVersion Engine { get { return engine; } }
			#endregion

			#region Platform
			GameManager.Platform platform = GameManager.Platform.Unknown;
			/// <summary>
			/// Platform this game is on
			/// </summary>
			public GameManager.Platform Platform { get { return platform; } }
			#endregion

			#region CacheTypes
			CacheTypeList cacheTypes = new CacheTypeList();
			/// <summary>
			/// Cache definitions for this game
			/// </summary>
			public CacheTypeList CacheTypes { get { return cacheTypes; } }

			/// <summary>
			/// Get a cache definition based on a supplied cache type name
			/// </summary>
			/// <param name="cache_type_name"></param>
			/// <returns>Cache definition linked to the supplied name, or null if not defined</returns>
			public CacheType this[string cache_type_name] { get { return cacheTypes[cache_type_name]; } }
			#endregion

			#region Tags
			Tags tags = new Tags();
			/// <summary>
			/// Tag definitions for this game
			/// </summary>
			public Tags Tags { get { return tags; } }
			#endregion

			#region Resources
			/// <summary>
			/// This game's resources by name to resource path
			/// </summary>
			Dictionary<string, string> resources = new Dictionary<string, string>(ResourceDefaultCapacity);

			/// <summary></summary>
			/// <param name="resource_name"></param>
			/// <param name="resource_location"></param>
			/// <remarks>Used by sub-classes of <see cref="BlamDefinition"/> and their implementation only</remarks>
			internal void AddResourceLocation(string resource_name, string resource_location)
			{
				resources.Add(resource_name, resource_location);
			}
			/// <summary></summary>
			/// <param name="resource_name"></param>
			/// <param name="resource_location"></param>
			/// <returns></returns>
			/// <remarks>Used for <see cref="BlamDefinition"/>'s implementation only</remarks>
			internal bool TryAndGetResourceLocation(string resource_name, out string resource_location)
			{
				return resources.TryGetValue(resource_name, out resource_location);
			}


			/// <summary>
			/// The current resources we have loaded into objects
			/// </summary>
			Dictionary<string, IGameResource> loadedResources = new Dictionary<string, IGameResource>(ResourceDefaultCapacity);
			internal object ResourceLockObject { get { return loadedResources; } }

			/// <summary>
			/// Get a loaded resource by it's usage name
			/// </summary>
			/// <typeparam name="T"></typeparam>
			/// <param name="resource_name"></param>
			/// <returns></returns>
			public T GetResource<T>(string resource_name) where T : class, IGameResource
			{
				lock (ResourceLockObject)
					return loadedResources[resource_name] as T;
			}

			/// <summary></summary>
			/// <param name="resource_name"></param>
			/// <param name="resource_obj"></param>
			/// <remarks>Used for <see cref="BlamDefinition"/>'s implementation only</remarks>
			internal void AddResource(string resource_name, IGameResource resource_obj)
			{
				lock (ResourceLockObject)
					loadedResources.Add(resource_name, resource_obj);
			}
			/// <summary></summary>
			/// <param name="resource_name"></param>
			/// <remarks>Used for <see cref="BlamDefinition"/>'s implementation only</remarks>
			internal void CloseResource(string resource_name)
			{
				lock (ResourceLockObject)
				{
					IGameResource gr = loadedResources[resource_name];
					gr.Close();
					loadedResources.Remove(resource_name);
				}
			}
			/// <summary></summary>
			/// <remarks>Used for <see cref="BlamDefinition"/>'s implementation only</remarks>
			internal void Close()
			{
				foreach (string s in loadedResources.Keys)
					loadedResources[s].Close();
				loadedResources.Clear();
			}
			#endregion

			#region Overrides
			private GameManager.Overrides overrides;
			/// <summary>
			/// Game's path overrides
			/// </summary>
			public GameManager.Overrides Overrides
			{
				get { return overrides; }
				internal set { overrides = value; }
			}
			#endregion

			IdentifyResource IdentifyResourcesProc = null;

			internal Game() { }
			internal Game(BlamDefinition owner, IdentifyResource identify_resources_proc)
			{
				this.owner = owner;
				IdentifyResourcesProc = identify_resources_proc;
			}

			/// <summary>
			/// Read the game specific definition
			/// </summary>
			/// <param name="s"></param>
			public void Read(IO.XmlStream s)
			{
				s.ReadAttribute("id", ref engine);
				s.ReadAttribute("platform", ref platform);

				foreach (XmlNode n in s.Cursor.ChildNodes)
				{
					if (n.Name == "cache")
					{
						s.SaveCursor(n);
						cacheTypes.Read(s);
						s.RestoreCursor();
					}
					else if (n.Name == "tags")
					{
						s.SaveCursor(n);
						tags.Read(s, engine);
						s.RestoreCursor();
					}
					else if (n.Name == "resources" && IdentifyResourcesProc != null)
					{
						s.SaveCursor(n);
						string key = string.Empty, val = string.Empty;
						foreach (XmlNode rsrc in s.Cursor.ChildNodes)
						{
							if (rsrc.Name != "resource") continue;

							s.SaveCursor(rsrc);
							s.ReadAttribute("key", ref key);
							s.ReadAttribute("value", ref val);
							IdentifyResourcesProc(this, key, val);
							s.RestoreCursor();
						}
						s.RestoreCursor();
					}
				}
			}
		};
		#endregion

		/// <summary>
		/// This system is one of the quickest and nastiest hacks I've done in a long time. I've since given up 
		/// on adding newer functionality to BlamLib unless it's in support of a specific engine feature. I don't 
		/// have the time to keep this codebase up to date when I have to work on .NET 4 tech in another heavy project.
		/// </summary>
		public abstract class SettingsInterface
		{
			protected static bool PathIsUndefined(string path)
			{
				return string.IsNullOrEmpty(path);
			}
			protected static bool PathIsValid(string path)
			{
				// The path must be null\empty, or contain a valid path
				return PathIsUndefined(path) || System.IO.File.Exists(path);
			}

			public abstract bool ValidateSettings();

			public abstract void Read(IO.XmlStream s);

			public abstract void Write(System.Xml.XmlWriter writer);
		};

		#region Generic game resource identifier names
		/// <summary>
		/// Name used for elements identifying a scripting definition resource
		/// </summary>
		internal const string ResourceScripts = "Scripting";
		/// <summary>
		/// Name used for elements identifying a string id resource
		/// </summary>
		internal const string ResourceStringIds = "StringId";
		/// <summary>
		/// Name used for elements identifying a <see cref="BlamLib.Render.VertexBufferInterface.VertexBuffers"/> resource
		/// </summary>
		internal const string ResourceVertexBuffers = "VertexBuffer";
		#endregion


		protected object syncRoot = new object();
		//internal object SyncRoot { get { return syncRoot; } }

		#region Name
		protected GameManager.Namespace name = GameManager.Namespace.Unknown;
		/// <summary>Blam engine's namespace</summary>
		[System.ComponentModel.Browsable(false)]
		public GameManager.Namespace Name { get { return name; } }
		#endregion

		#region Games
		protected List<Game> games = new List<Game>();
		/// <summary>List of definitions for each individual implementation (on a platform) of the game </summary>
		[System.ComponentModel.Browsable(false)]
		public ICollection<Game> Games { get { return games; } }

		/// <summary>
		/// Get the <see cref="BlamDefinition.Game"/> object associated with <paramref name="game"/>
		/// </summary>
		/// <param name="game"></param>
		/// <returns>One of the <see cref="BlamDefinition.Game"/> objects in <see cref="Games"/>, or null if the game isn't found</returns>
		public Game FindGame(BlamVersion game)
		{
			foreach (var g in games) if (g.Engine == game) return g;
			
			return null;
		}

		/// <summary>
		/// Get a game definition by it's engine version id
		/// </summary>
		/// <param name="engine"></param>
		/// <returns></returns>
		public Game this[BlamVersion engine] { get { return FindGame(engine); } }

		/// <summary>
		/// Setup the overrides for a specific engine
		/// </summary>
		/// <param name="ver">Engine to look for</param>
		/// <param name="overrides">Engine's overrides</param>
		internal void SetupOverrides(BlamVersion ver, GameManager.Overrides overrides)
		{
			foreach (var g in games)
			{
				if (g.Engine == ver)
				{
					g.Overrides = overrides;
					break;
				}
			}
		}
		#endregion

		#region TagGroups
		/// <summary>This game's tag group definitions collection</summary>
		[System.ComponentModel.Browsable(false)]
		public abstract TagInterface.TagGroupCollection TagGroups { get; }
		/// <summary>Tag groups which should never occupy space in a cache UI's tag tree</summary>
		/// <remarks>Default implementation returns <see cref="TagInterface.TagGroupCollection.Empty"/></remarks>
		[System.ComponentModel.Browsable(false)]
		public virtual TagInterface.TagGroupCollection TagGroupsInvalidForCacheViewer { get { return TagInterface.TagGroupCollection.Empty; } }
		/// <summary>Tag groups which should never be allowed to be extracted (due to problems, incomplete code, etc)</summary>
		/// <remarks>Default implementation returns <see cref="TagInterface.TagGroupCollection.Empty"/></remarks>
		public virtual TagInterface.TagGroupCollection TagGroupsInvalidForExtraction { get { return TagInterface.TagGroupCollection.Empty; } }

		/// <summary>
		/// Finds a tag group based on its 4 byte ID and returns its full name
		/// </summary>
		/// <param name="tag">four character code</param>
		/// <returns><paramref name="tag"/>'s full name, or "unknown" if not found</returns>
		public string TagGroupFullName(char[] tag)
		{
			foreach (var t in TagGroups.GroupTags)
				if (TagInterface.TagGroup.Test(tag, t.Tag))
					return t.Name;

			return "unknown";
		}

		/// <summary>
		/// Finds a tag group based on its 4 byte ID
		/// </summary>
		/// <param name="tag">four character code</param>
		/// <returns><paramref name="tag"/>'s <see cref="TagInterface.TagGroup"/> definition, or null if not found</returns>
		public TagInterface.TagGroup TagGroupFind(string tag) { return TagGroupFind(tag.ToCharArray()); }

		/// <summary>
		/// Finds a tag group based on its 4 byte ID
		/// </summary>
		/// <param name="tag">four character code</param>
		/// <returns><paramref name="tag"/>'s <see cref="TagInterface.TagGroup"/> definition, or null if not found</returns>
		public TagInterface.TagGroup TagGroupFind(char[] tag)
		{
			foreach (var t in TagGroups.GroupTags)
				if (t.Test(tag))
					return t;

			return null;
		}

		/// <summary>
		/// Finds a tag group based on its 4 byte ID
		/// </summary>
		/// <param name="tag">four character code stored in a 32bit integer</param>
		/// <returns><paramref name="tag"/>'s <see cref="TagInterface.TagGroup"/> definition, or null if not found</returns>
		public TagInterface.TagGroup TagGroupFind(uint tag)
		{
			foreach (var t in TagGroups.GroupTags)
				if (t.ID == tag)
					return t;

			return null;
		}
		#endregion

		#region Localization
		protected LocalizationList localization = null;
		/// <summary>This game engine's localization system</summary>
		[System.ComponentModel.Browsable(false)]
		public LocalizationList Localization	{ get { return localization; } }
		#endregion

		#region Resources
		/// <summary>
		/// Function used for identifying a resource associated with a specific game
		/// </summary>
		/// <param name="owner"></param>
		/// <param name="resource_name"></param>
		/// <param name="resource_path"></param>
		internal abstract void IdentifyResourceProc(Game owner, string resource_name, string resource_path);
		/// <summary>
		/// Function used for warming resource definition caches
		/// </summary>
		/// <param name="owner"></param>
		/// <param name="resource_name"></param>
		/// <param name="r_path">Type path to the manifest file</param>
		/// <param name="r_name">File name (with extension) of the manifest file</param>
		/// <returns></returns>
		internal protected abstract IGameResource PrecacheResource(Game owner, string resource_name, string r_path, string r_name);
		/// <summary>
		/// Function used for warming resource definition caches
		/// </summary>
		/// <param name="owner"></param>
		/// <param name="resource_name"></param>
		/// <returns></returns>
		bool PrecacheResource(Game owner, string resource_name)
		{
			IGameResource gr = null;
			string r_path = null, r_name = null;

			if (owner.TryAndGetResourceLocation(resource_name, out r_name)) // if this resource type is defined...
			{
				// get the path to the definition
				r_path = GameManager.GetRelativePath(owner.Owner.Name, owner.Platform, Managers.GameManager.PlatformFolder.Definitions);

				gr = PrecacheResource(owner, resource_name, r_path, r_name); // allow the game definition to do what it needs to load the cache

				if (gr != null) // if it was successful in caching it, then say it was loaded and return true
				{
					owner.AddResource(resource_name, gr);
					return true;
				}
			}

			return false; // something fucked up
		}

		/// <summary>
		/// Pre-cache a resource for use
		/// </summary>
		/// <param name="game"></param>
		/// <param name="resource_name"></param>
		internal void PrecacheResource(BlamVersion game, string resource_name)
		{
			Game g = FindGame(game);
			lock (g.ResourceLockObject)
			{
				bool result = PrecacheResource(g, resource_name);
				if (!result)
					throw new Debug.ExceptionLog("{0}'s resource failed to pre-cache: {0}.", g.Engine, resource_name);
			}
		}
		/// <summary>
		/// Unload a resource from use
		/// </summary>
		/// <param name="game"></param>
		/// <param name="resource_name"></param>
		internal void CloseResource(BlamVersion game, string resource_name)
		{
			Game g = FindGame(game);
			lock (g.ResourceLockObject)
				g.CloseResource(resource_name);
		}
		#endregion

		/// <summary>Gets the settings interface object for this definition, if one exists</summary>
		public virtual SettingsInterface Settings { get { return null; } }

		// TODO: note that if used over time, some of these handles, if not closed, could raise conflicts.
		// However, there is a maximum of 255 handles for each system which I don't think will ever be overflowed 
		// even after extended use. Worse case scenario, TagIndex shit could cause this, but who the hell would open 
		// that many fucking tag index objects?

		#region CacheBuilders
		const ushort kCacheBuilderSaltBase = (0xBEEF & 0xFF00);
		byte CacheBuilderSalt = 0;

		internal static bool IsCacheBuilderHandle(Blam.DatumIndex handle) { return (ushort)(handle.Index & 0xFF00) == kCacheBuilderSaltBase; }

		Dictionary<Blam.DatumIndex, Blam.Cache.BuilderBase> cacheBuilders = new Dictionary<BlamLib.Blam.DatumIndex, BlamLib.Blam.Cache.BuilderBase>();

		/// <summary>Only used internally to enumerate all the cache builder objects so our code isn't wasting time calling <see cref="GetCacheBuilder"/></summary>
		[System.ComponentModel.Browsable(false)]
		/*internal*/ IEnumerable<Blam.Cache.BuilderBase> CacheBuilderObjects { get { foreach (Blam.Cache.BuilderBase cf in cacheBuilders.Values) yield return cf; } }

		/// <summary>Enumeration for all the cache builder handles for this game</summary>
		[System.ComponentModel.Browsable(false)]
		/*public*/ IEnumerable<Blam.DatumIndex> CacheBuilders { get { foreach (Blam.DatumIndex di in cacheBuilders.Keys) yield return di; } }


		/// <summary>
		/// Get the blam engine component of a cache builder handle
		/// </summary>
		/// <param name="builder_id"></param>
		/// <returns></returns>
		internal static BlamVersion CacheBuilderDatumToEngine(Blam.DatumIndex builder_id) { return (BlamVersion)builder_id.Salt; }

		/// <summary>
		/// Request the cache builder object associated with the identifier
		/// </summary>
		/// <param name="builder_id">Value returned from <see cref="CreateCacheBuilder"/></param>
		/// <returns>Associated value with <paramref name="builder_id"/></returns>
		public Blam.Cache.BuilderBase GetCacheBuilder(Blam.DatumIndex builder_id)
		{
			Debug.Assert.If(builder_id != Blam.DatumIndex.Null, "[{0}] Null builder id.", this.name);

			Blam.Cache.BuilderBase cf = null;
			lock(cacheBuilders)
				if (!cacheBuilders.TryGetValue(builder_id, out cf))
				{
					Debug.Assert.If(false, "[{0}] Invalid builder id: {1}. ({2:X})", this.name, builder_id, CacheBuilderSalt);
				}

			return cf;
		}

		/// <summary>
		/// Implemented in the game's definition for handling the creation of a cache builder
		/// </summary>
		/// <param name="game"></param>
		/// <returns></returns>
		internal protected abstract Blam.Cache.BuilderBase ConstructCacheBuilder(BlamVersion game);

		/// <summary>
		/// Create a cache builder and add it to the list of running cache builders
		/// </summary>
		/// <param name="game">Engine the builder is expected to be for</param>
		/// <returns>Identifier that can be used in cache builder management</returns>
		public Blam.DatumIndex CreateCacheBuilder(BlamVersion game)
		{
			Blam.Cache.BuilderBase cf = ConstructCacheBuilder(game);
			Debug.Assert.If(cf != null, "[{0}] Failed to create a cache builder: [{1}].", this.name, game);

			Blam.DatumIndex builder_id;
			lock (cacheBuilders)
			{
				// Id is built from the a salt value, and placed in the datum's index field, 
				// then the engine version is placed in the salt field. Yes, I realize I should 
				// call it something else besides salt since it's not placed in the salt field...
				builder_id = new BlamLib.Blam.DatumIndex((ushort)(kCacheBuilderSaltBase | CacheBuilderSalt++), (short)game);

				cacheBuilders.Add(builder_id, cf);
			}

			return cf.BuilderId = builder_id;
		}

		/// <summary>
		/// Close a cache builder from memory
		/// </summary>
		/// <param name="builder_id">Value returned from <see cref="CreateCacheBuilder"/></param>
		public void CloseCacheBuilder(Blam.DatumIndex builder_id)
		{
			Blam.Cache.BuilderBase cf = GetCacheBuilder(builder_id); // validate handle

			if (cf != null)
			{
				lock (cacheBuilders)
					cacheBuilders.Remove(builder_id);

				cf.Dispose();
			}
		}

		void CloseAllCacheBuilders()
		{
			// we have to modify cacheBuilders to remove the items, so we can't use a foreach directly on it
			Blam.DatumIndex[] cache_buidlers;
			{
				var keys = cacheBuilders.Keys;
				cache_buidlers = new BlamLib.Blam.DatumIndex[keys.Count];
				keys.CopyTo(cache_buidlers, 0);
			}
			foreach (var di in cache_buidlers) CloseCacheBuilder(di);
		}
		#endregion

		#region CacheFiles
		const ushort kCacheFileSaltBase = 0xCAFE & 0xFF00;
		byte CacheFileSalt = 0;

		internal static bool IsCacheHandle(Blam.DatumIndex handle) { return (ushort)(handle.Index & 0xFF00) == kCacheFileSaltBase; }

		Dictionary<Blam.DatumIndex, Blam.CacheFile> cacheFiles = new Dictionary<BlamLib.Blam.DatumIndex,BlamLib.Blam.CacheFile>();

		/// <summary>Only used internally to enumerate all the cache file objects so our code isn't wasting time calling <see cref="GetCacheFile"/></summary>
		[System.ComponentModel.Browsable(false)]
		/*internal*/ IEnumerable<Blam.CacheFile> CacheFilesObjects { get { foreach (Blam.CacheFile cf in cacheFiles.Values) yield return cf; } }

		/// <summary>Enumeration for all the cache file handles for this game</summary>
		[System.ComponentModel.Browsable(false)]
		/*public*/ IEnumerable<Blam.DatumIndex> CacheFiles { get { foreach (Blam.DatumIndex di in cacheFiles.Keys) yield return di; } }


		/// <summary>
		/// Get the blam engine component of a cache file handle
		/// </summary>
		/// <param name="cache_id"></param>
		/// <returns></returns>
		internal static BlamVersion CacheDatumToEngine(Blam.DatumIndex cache_id)	{ return (BlamVersion)cache_id.Salt; }

		/// <summary>
		/// Request the cache file object associated with the identifier
		/// </summary>
		/// <param name="cache_id">Value returned from <see cref="OpenCacheFile"/></param>
		/// <returns>Associated value with <paramref name="cache_id"/></returns>
		public Blam.CacheFile GetCacheFile(Blam.DatumIndex cache_id)
		{
			Debug.Assert.If(cache_id != Blam.DatumIndex.Null, "[{0}] Null cache id.", this.name);

			Blam.CacheFile cf = null;
			lock(cacheFiles)
				if(!cacheFiles.TryGetValue(cache_id, out cf))
				{
					Debug.Assert.If(false, "[{0}] Invalid cache id: {1}. ({2:X})", this.name, cache_id, CacheFileSalt);
				}

			return cf;
		}

		/// <summary>
		/// Implemented in the game's definition for handling loading a cache file from disk
		/// </summary>
		/// <param name="game"></param>
		/// <param name="file_path"></param>
		/// <param name="is_resource"></param>
		/// <returns></returns>
		internal protected abstract Blam.CacheFile LoadCacheFile(BlamVersion game, string file_path, bool is_resource);

		Blam.DatumIndex OpenCacheFileImp(BlamVersion game, string file_path, bool is_resource)
		{
			Blam.CacheFile cf = LoadCacheFile(game, file_path, is_resource);
			Debug.Assert.If(cf != null, "[{0}] Failed to load {1}cache file: [{2}] '{3}'.", this.name, is_resource ? "resource " : "", game, file_path);

			Blam.DatumIndex cache_id;
			lock (cacheFiles)
			{
				// Id is built from the a salt value, and placed in the datum's index field, 
				// then the engine version is placed in the salt field. Yes, I realize I should 
				// call it something else besides salt since it's not placed in the salt field...
				cache_id = new BlamLib.Blam.DatumIndex((ushort)(kCacheFileSaltBase | CacheFileSalt++), (short)game);

				cacheFiles.Add(cache_id, cf);
			}

			return cf.CacheId = cache_id;
		}

		/// <summary>
		/// Load a cache file into memory and add it to the list of loaded cache files
		/// </summary>
		/// <param name="game">Engine the cache is expected to be for</param>
		/// <param name="file_path">Path to the cache file on disk</param>
		/// <returns>Identifier that can be used in cache file management</returns>
		public Blam.DatumIndex OpenCacheFile(BlamVersion game, string file_path)
		{
			return OpenCacheFileImp(game, file_path, false);
		}

		/// <summary>
		/// Load a resource cache file into memory and add it to the list of loaded cache files
		/// </summary>
		/// <param name="game">Engine the cache is expected to be for</param>
		/// <param name="file_path">Path to the cache file on disk</param>
		/// <returns>Identifier that can be used in cache file management</returns>
		public Blam.DatumIndex OpenResourceCacheFile(BlamVersion game, string file_path)
		{
			return OpenCacheFileImp(game, file_path, true);
		}

		/// <summary>
		/// Close a loaded cache file from memory
		/// </summary>
		/// <param name="cache_id">Value returned from <see cref="OpenCacheFile"/></param>
		public void CloseCacheFile(Blam.DatumIndex cache_id)
		{
			Blam.CacheFile cf = GetCacheFile(cache_id); // validate handle

			if (cf != null)
			{
				lock (cacheFiles)
					cacheFiles.Remove(cache_id);

				cf.Dispose();
			}
		}

		void CloseAllCacheFiles()
		{
			// we have to modify cacheFiles to remove the items, so we can't use a foreach directly on it
			Blam.DatumIndex[] cache_files;
			{
				var keys = cacheFiles.Keys;
				cache_files = new BlamLib.Blam.DatumIndex[keys.Count];
				keys.CopyTo(cache_files, 0);
			}
			foreach (var di in cache_files) CloseCacheFile(di);
		}

		/// <summary>
		/// Returns a CacheFile object based on a cache name
		/// </summary>
		/// <param name="ver">Engine version of the expected cache</param>
		/// <param name="cache_name">Blam based cache name</param>
		/// <returns>The CacheFile associated with <paramref name="cache_name"/></returns>
		public virtual Blam.CacheFile GetCacheFileFromLocation(BlamVersion ver, string cache_name) { throw new NotImplementedException(); }
		/// <summary>
		/// Returns a CacheFile object based on a cache name
		/// </summary>
		/// <remarks>
		/// If the <paramref name="cache_name"/> is null or empty then <paramref name="is_internal"/> 
		/// gets set to true and null is returned. If null and <paramref name="is_internal"/> is not set, 
		/// the CacheFile is either not loaded or the location was invalid.
		/// </remarks>
		/// <param name="ver">Engine version of the expected cache</param>
		/// <param name="cache_name">Blam based cache name</param>
		/// <param name="is_internal">bool reference to set if the reference is internal</param>
		/// <returns>The CacheFile associated with <paramref name="cache_name"/></returns>
		public virtual Blam.CacheFile GetCacheFileFromLocation(BlamVersion ver, string cache_name, out bool is_internal) { throw new NotImplementedException(); }
		#endregion

		#region TagIndexes
		const ushort kTagIndexSaltBase = 0xDFAC & 0xFF00;
		byte TagIndexSalt = 0;

		internal static bool IsTagIndexHandle(Blam.DatumIndex handle) { return (ushort)(handle.Index & 0xFF00) == kTagIndexSaltBase; }

		Dictionary<Blam.DatumIndex, Managers.ITagIndex> tagIndexes = new Dictionary<BlamLib.Blam.DatumIndex, Managers.ITagIndex>();

		/// <summary>Only used internally to enumerate all the tag index objects so our code isn't wasting time calling <see cref="GetTagIndex"/></summary>
		[System.ComponentModel.Browsable(false)]
		/*internal*/ IEnumerable<Managers.ITagIndex> TagIndexObjects { get { foreach (Managers.ITagIndex ti in tagIndexes.Values) yield return ti; } }

		/// <summary>Enumeration for all the tag index handles for this game</summary>
		[System.ComponentModel.Browsable(false)]
		/*public*/ IEnumerable<Blam.DatumIndex> TagIndexes { get { foreach (Blam.DatumIndex di in tagIndexes.Keys) yield return di; } }


		/// <summary>
		/// Get the blam engine component of a tag index handle
		/// </summary>
		/// <param name="index_id"></param>
		/// <returns></returns>
		internal static BlamVersion TagIndexDatumToEngine(Blam.DatumIndex index_id)	{ return (BlamVersion)index_id.Salt; }

		/// <summary></summary>
		/// <param name="cf"></param>
		/// <param name="cache_index"></param>
		/// <remarks>Don't call this method outside of this class unless you're <c>kornman00</c></remarks>
		internal void AddTagIndex(Blam.CacheFile cf, Managers.CacheTagIndex cache_index)
		{
			lock (tagIndexes)
			{
				Blam.DatumIndex index_id = new BlamLib.Blam.DatumIndex((ushort)(kTagIndexSaltBase | TagIndexSalt++), (short)cf.EngineVersion);

				tagIndexes.Add(cache_index.IndexId = index_id, cache_index);
			}
		}

		/// <summary>
		/// Close an open tag index manager
		/// </summary>
		/// <param name="index_id"></param>
		/// <param name="external_call"></param>
		/// <remarks>Don't call this method outside of this class unless you're <c>kornman00</c></remarks>
		internal void CloseTagIndex(Blam.DatumIndex index_id, bool external_call)
		{
			Managers.ITagIndex ti = GetTagIndex(index_id); // validate handle

			// Note: If it's a Cache tag index and it's an external call, index will be removed
			if (ti != null /*|| (ti is Managers.CacheTagIndex && external_call)*/)
			{
				ti.Dispose();

				lock (tagIndexes)
					tagIndexes.Remove(index_id);
			}
		}

		/// <summary>
		/// Request the tag index manager object associated with the identifier
		/// </summary>
		/// <param name="index_id">Value returned from <see cref="OpenTagIndex"/></param>
		/// <returns>Associated value with <paramref name="index_id"/></returns>
		public Managers.ITagIndex GetTagIndex(Blam.DatumIndex index_id)
		{
			Debug.Assert.If(index_id != Blam.DatumIndex.Null, "[{0}] Null tag index id.", this.name);

			Managers.ITagIndex ti = null;
			lock (tagIndexes)
				if (!tagIndexes.TryGetValue(index_id, out ti))
				{
					Debug.Assert.If(false, "[{0}] Invalid tag index id: {1}. ({2:X})", this.name, index_id, CacheFileSalt);
				}

			return ti;
        }

        /// <summary>
        /// Open a tag index
        /// </summary>
        /// <param name="game">Engine the tag index is for</param>
        /// <param name="base_directory">Base directory on disk for the index</param>
        /// <returns>Identifier that can be used in tag index management</returns>
        public Blam.DatumIndex OpenTagIndex(BlamVersion game, string base_directory)
        {
            return OpenTagIndex(game, base_directory, "tags", false);
        }

        /// <summary>
        /// Open a tag index
        /// </summary>
        /// <param name="game">Engine the tag index is for</param>
        /// <param name="base_directory">Base directory on disk for the index</param>
        /// <param name="tags_dir">Tags folder name</param>
        /// <returns>Identifier that can be used in tag index management</returns>
        public Blam.DatumIndex OpenTagIndex(BlamVersion game, string base_directory, string tags_dir)
        {
            return OpenTagIndex(game, base_directory, tags_dir, false);
        }

        /// <summary>
        /// Open a tag index
        /// </summary>
        /// <param name="game">Engine the tag index is for</param>
        /// <param name="base_directory">Base directory on disk for the index</param>
        /// <param name="create">Create the directory if it doesn't exist on disk</param>
        /// <returns>Identifier that can be used in tag index management</returns>
        public Blam.DatumIndex OpenTagIndex(BlamVersion game, string base_directory, bool create)
        {
            return OpenTagIndex(game, base_directory, "tags", create);
        }

		/// <summary>
		/// Open a tag index
		/// </summary>
        /// <param name="game">Engine the tag index is for</param>
        /// <param name="base_directory">Base directory on disk for the index</param>
        /// <param name="tags_dir">Tags folder name</param>
		/// <param name="create">Create the directory if it doesn't exist on disk</param>
		/// <returns>Identifier that can be used in tag index management</returns>
		public Blam.DatumIndex OpenTagIndex(BlamVersion game, string base_directory, string tags_dir, bool create)
		{
            Managers.TagIndex ti = new Managers.TagIndex(game, base_directory, tags_dir, create);
			Debug.Assert.If(ti != null, "[{0}] Failed to open a tag index: [{1}] '{2}'.", this.name, game, base_directory);

			Blam.DatumIndex index_id;
			lock (tagIndexes)
			{
				// Id is built from the a salt value, and placed in the datum's index field, 
				// then the engine version is placed in the salt field. Yes, I realize I should 
				// call it something else besides salt since it's not placed in the salt field...
				index_id = new BlamLib.Blam.DatumIndex((ushort)(kTagIndexSaltBase | TagIndexSalt++), (short)game);

				tagIndexes.Add(index_id, ti);
			}

			return ti.IndexId = index_id;
		}

		/// <summary>
		/// Close an open tag index manager
		/// </summary>
		/// <param name="index_id"></param>
		public void CloseTagIndex(Blam.DatumIndex index_id)	{ CloseTagIndex(index_id, false); }

		void CloseAllTagIndexes()
		{
			// we have to modify tagIndexes to remove the items, so we can't use a foreach directly on it
			Blam.DatumIndex[] tag_indexes;
			{
				var keys = tagIndexes.Keys;
				tag_indexes = new BlamLib.Blam.DatumIndex[keys.Count];
				keys.CopyTo(tag_indexes, 0);
			}
			foreach (var di in tag_indexes) CloseTagIndex(di);
		}
		#endregion

		#region Tag Databases
		/// <summary>
		/// Create a game specific Tag Database object
		/// </summary>
		/// <returns>Generic interface for Tag Databases over the game specific implementation</returns>
		public abstract TagDatabase CreateTagDatabase();

		/// <summary>
		/// Game specific implementation interface function for creating Cache Tag database objects
		/// </summary>
		/// <param name="cache_id">Handle to a loaded cache file belonging to this specific game</param>
		/// <returns>Generic interface for Cache Tag Databases over the game specific implementation</returns>
		protected abstract CacheTagDatabase CreateCacheTagDatabaseInternal(Blam.DatumIndex cache_id);

		/// <summary>
		/// Create a game specific Cache Tag Database object
		/// </summary>
		/// <param name="cache_id">Handle to a loaded cache file belonging to this specific game</param>
		/// <returns>Generic interface for Cache Tag Databases over the game specific implementation</returns>
		/// <remarks>
		/// Verifies that <paramref name="cache_id"/> is not null, is infact a valid cache handle, and is currently loaded. 
		/// If the check fails, this returns null
		/// </remarks>
		public CacheTagDatabase CreateCacheTagDatabase(Blam.DatumIndex cache_id)
		{
			if (cache_id != Blam.DatumIndex.Null && IsCacheHandle(cache_id) && cacheFiles.ContainsKey(cache_id))
				return CreateCacheTagDatabaseInternal(cache_id);

			return null;
		}

		/// <summary>
		/// Create a game specific Error Tag Database object
		/// </summary>
		/// <returns>Generic interface for Error Tag Databases over the game specific implementation</returns>
		public abstract ErrorTagDatabase CreateErrorTagDatabase();

		/// <summary>Group tag for the tag_database definition</summary>
		[System.ComponentModel.Browsable(false)]
		public abstract TagInterface.TagGroup TagDatabaseGroup { get; }
		#endregion

		internal BlamDefinition() { }

		#region Loading
		/// <summary>
		/// Process a blam definition from a manifest file
		/// </summary>
		/// <param name="path">path to file on a disk</param>
		/// <param name="name"></param>
		public void Read(string path, string name)
		{
			IdentifyResource proc = new IdentifyResource(IdentifyResourceProc);

			using (IO.XmlStream s = new BlamLib.IO.XmlStream(path, name, this))
			{
				s.ReadAttribute("game", ref this.name);

				#region find and load game elements
				Game g;
				foreach (XmlNode n in s.Cursor.ChildNodes)
					if (n.Name == "game")
					{
						Games.Add(g = new Game(this, proc));
						s.SaveCursor(n);
						g.Read(s);
						s.RestoreCursor();
					}
					else if (n.Name == "localization")
					{
						localization = new LocalizationList();
						s.SaveCursor(n);
						localization.Read(s);
						s.RestoreCursor();
					}
				#endregion
			}
		}

		internal void Close()
		{
			foreach (var g in games) g.Close();

			CloseAllCacheBuilders();
			CloseAllCacheFiles();
			CloseAllTagIndexes();
		}
		#endregion
	};
}