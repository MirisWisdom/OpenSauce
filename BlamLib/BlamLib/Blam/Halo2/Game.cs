/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Threading;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo2
{
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
	/// <summary>
	/// Supported languages
	/// </summary>
	public enum LanguageType
	{
		English,
		Japanese,
		German,
		French,
		Spanish,
		Italian,
		Korean,
		/// <summary>
		/// Traditional
		/// </summary>
		Chinese,
		Portuguese,

		kMax
	};

	#region s_cache_language_pack
	/// <summary>
	/// Structure used to reference a single language's string data in a cache's language pack
	/// </summary>
	struct s_cache_unicode_string_reference : IO.IStreamable
	{
		public Blam.StringId NameId;
		public int Offset;

		#region IStreamable Members
		public void Read(BlamLib.IO.EndianReader s)
		{
			NameId.Read(s, (s.Owner as CacheFile).StringIds.Definition.Description);
			Offset = s.ReadInt32();
		}

		public void Write(BlamLib.IO.EndianWriter s)
		{
			NameId.Write(s);
			s.Write(Offset);
		}
		#endregion
	};

	public interface ICacheLanguagePackContainer
	{
		s_cache_language_pack LanguagePackGet(LanguageType lang);
	};

	public class s_cache_language_pack : IO.IStreamable
	{
		public const int kSizeOf = (4 * 2) + (4 * 4) + (1 + 3);

		public TI.LongInteger Count;
		public TI.LongInteger Size;
		TI.LongInteger OffsetReferences;
		TI.LongInteger OffsetStrings;

		public Blam.ResourcePtr GetOffsetReferences() { return OffsetReferences.Value; }
		public Blam.ResourcePtr GetOffsetStrings() { return OffsetStrings.Value; }

		/// <summary>
		/// Initialize the language pack to a tag definition or as a stand-alone
		/// </summary>
		/// <param name="parent"></param>
		public s_cache_language_pack(TI.Definition parent)
		{
			Count = new TI.LongInteger();
			Size = new TI.LongInteger();
			OffsetReferences = new TI.LongInteger();
			OffsetStrings = new TI.LongInteger();

			if (parent != null)
			{
				parent.Add(new TI.Pad(4 + 4)); // runtime pointers
					// s_cache_unicode_string_reference* references_buffer
					// byte* strings_buffer
				parent.Add(Count);
				parent.Add(Size);
				parent.Add(OffsetReferences);
				parent.Add(OffsetStrings);
				parent.Add(new TI.Pad(1 + 3));
			}
		}

		#region IStreamable Members
		public void Read(BlamLib.IO.EndianReader s)
		{
			s.Seek(4 + 4, System.IO.SeekOrigin.Current);
			Count.Read(s);
			Size.Read(s);
			OffsetReferences.Read(s);
			OffsetStrings.Read(s);
			s.Seek(1 + 3, System.IO.SeekOrigin.Current);
		}

		public void Write(BlamLib.IO.EndianWriter s)
		{
			s.Write(ulong.MinValue);
			Count.Write(s);
			Size.Write(s);
			OffsetReferences.Write(s);
			OffsetStrings.Write(s);
			s.Write(uint.MinValue);
		}
		#endregion


		#region Cache data
		s_cache_unicode_string_reference[] stringReferences;
		byte[] stringData;
		/// <summary>
		/// Has this language pack already been loaded from a cache file?
		/// </summary>
		internal bool IsLoaded { get { return stringReferences != null; } }

		int GetStringBufferSize(int reference_index)
		{
			if (reference_index == stringReferences.Length-1)
				return Size.Value - stringReferences[reference_index].Offset;

			return stringReferences[reference_index+1].Offset -
				stringReferences[reference_index].Offset;
		}

		int GetTotalStringBufferSize(int reference_index, int count)
		{
			if (reference_index+count == stringReferences.Length)
				return Size.Value - stringReferences[reference_index].Offset;

			return stringReferences[reference_index + count].Offset -
				stringReferences[reference_index].Offset;
		}

		/// <summary>
		/// Get the base offset of a set of string references based on the their 
		/// initial reference index
		/// </summary>
		/// <param name="reference_index">First string's reference index</param>
		/// <returns></returns>
		int GetStringBaseOffset(int reference_index)
		{
			return stringReferences[reference_index].Offset;
		}


		public void ReadFromCache(Blam.Halo2.CacheFile cf)
		{
			int count = this.Count.Value;

			if(count > 0)
			{
				#region Read the string references
				var rsrc_offset = GetOffsetReferences();
				var rsrc_cache = Program.Halo2.FromLocation(cf, rsrc_offset);

				if (rsrc_cache != null)
				{
					rsrc_cache.InputStream.Seek(rsrc_offset.Offset);
					stringReferences = new s_cache_unicode_string_reference[count];
					for (int x = 0; x < stringReferences.Length; x++)
						stringReferences[x].Read(cf.InputStream);
				}
				#endregion

				#region Read the string data buffer
				rsrc_offset = GetOffsetStrings();
				rsrc_cache = Program.Halo2.FromLocation(cf, rsrc_offset);

				if (rsrc_cache != null)
				{
					rsrc_cache.InputStream.Seek(rsrc_offset.Offset);
					stringData = rsrc_cache.InputStream.ReadBytes(Size.Value);
				}
				else // just in case references was valid but somehow the string data buffer wasn't, can't have one without the other
					stringReferences = null;
				#endregion
			}
		}
		#endregion

		#region Reconstruction interface
		/// <summary>
		/// Copy the string data of <paramref name="reference_index"/> into the <paramref name="dst"/> array 
		/// at starting index <paramref name="dst_offset"/> and return the total bytes copied. Also provides the 
		/// string id name of <paramref name="reference_index"/>.
		/// </summary>
		/// <param name="reference_index"></param>
		/// <param name="dst"></param>
		/// <param name="dst_offset"></param>
		/// <param name="name"></param>
		/// <returns></returns>
		internal int CopyStringReferenceData(int reference_index, byte[] dst, int dst_offset, out Blam.StringId name)
		{
			int offset = GetStringBaseOffset(reference_index);
			int size = GetStringBufferSize(reference_index);

			Array.Copy(stringData, offset, dst, dst_offset, size);

			name = stringReferences[reference_index].NameId;

			return size;
		}
		/// <summary>
		/// Predict how much space this language's string data will need to consume in a 
		/// <see cref="multilingual_unicode_string_list_group"/> tag
		/// </summary>
		/// <param name="reference_index">Start of the tag's string references</param>
		/// <param name="count">Number of string references the tag has</param>
		/// <returns></returns>
		internal int PredictRequiredStringDataSize(int reference_index, int count)
		{
			return GetTotalStringBufferSize(reference_index, count);
		}
		#endregion
	};
	#endregion
#pragma warning restore 1591 // "Missing XML comment for publicly visible type or member"

	/// <summary>
	/// Halo 2 game definition implementation
	/// </summary>
	public sealed class GameDefinition : Managers.BlamDefinition, Managers.IStringIdController, Managers.IScriptingController, Managers.IVertexBufferController
	{
		#region Implementation
		public override TI.TagGroupCollection TagGroups						{ get { return Halo2.TagGroups.Groups; } }
		public override TI.TagGroupCollection TagGroupsInvalidForCacheViewer{ get { return Halo2.TagGroups.GroupsInvalidForCacheViewer; } }
		public override TI.TagGroupCollection TagGroupsInvalidForExtraction	{ get { return Halo2.TagGroups.GroupsInvalidForExtraction; } }

		/// <remarks>Explicit resource identifying. Yes, there are reasons for this. Ask km00 if you care that much</remarks>
		internal override void IdentifyResourceProc(Managers.BlamDefinition.Game owner, string resource_name, string resource_path)
		{
			bool add_rsrc = false;

			switch (owner.Engine)
			{
				case BlamVersion.Halo2_Xbox:
					switch (resource_name)
					{
						case Managers.BlamDefinition.ResourceScripts:
						case Managers.BlamDefinition.ResourceStringIds:
						case Managers.BlamDefinition.ResourceVertexBuffers: add_rsrc = true; break;
					}
					break;
				case BlamVersion.Halo2_PC:
					switch (resource_name)
					{
						case Managers.BlamDefinition.ResourceScripts:
						case Managers.BlamDefinition.ResourceStringIds:
						case Managers.BlamDefinition.ResourceVertexBuffers: add_rsrc = true; break;
					}
					break;
				case BlamVersion.Halo2_Alpha:
					switch (resource_name)
					{
						case Managers.BlamDefinition.ResourceScripts:
						case Managers.BlamDefinition.ResourceStringIds:
						case Managers.BlamDefinition.ResourceVertexBuffers: add_rsrc = true; break;
					}
					break;
				case BlamVersion.Halo2_Epsilon:
					switch (resource_name)
					{
						case Managers.BlamDefinition.ResourceScripts:
						case Managers.BlamDefinition.ResourceStringIds: add_rsrc = true; break;
						// we don't have the xbe to the epsilon, so eat a dick.
					}
					break;

				default: throw new Debug.Exceptions.UnreachableException();
			}

			if (add_rsrc)
				owner.AddResourceLocation(resource_name, resource_path);
		}

		internal protected override IGameResource PrecacheResource(Game owner, string resource_name, string r_path, string r_name)
		{
			IGameResource gr = null;
			bool result = false;

			switch (resource_name)
			{
				case Managers.BlamDefinition.ResourceScripts:
					gr = new Scripting.XmlInterface();
					result = gr.Load(r_path, r_name);
					break;

				case Managers.BlamDefinition.ResourceStringIds:
					gr = new Managers.StringIdStaticCollection();
					result = gr.Load(r_path, r_name);
					break;

				case Managers.BlamDefinition.ResourceVertexBuffers:
					gr = new Render.VertexBufferInterface.VertexBuffersGen2();
					result = gr.Load(r_path, r_name);
					break;
			}

			if (!result && gr != null)
			{
				gr.Close();
				gr = null;
			}

			return gr;
		}

		internal protected override Blam.Cache.BuilderBase ConstructCacheBuilder(BlamVersion game)
		{
			Blam.Cache.BuilderBase cb = null;

			if ((game & BlamVersion.Halo2) != 0)
			{
				cb = new Halo2.Builder();
			}

			return cb;
		}

		internal protected override Blam.CacheFile LoadCacheFile(BlamVersion game, string file_path, bool is_resource)
		{
			Blam.CacheFile cf = null;

			if ((game & BlamVersion.Halo2) != 0)
			{
				cf = new Halo2.CacheFile(file_path);
			}

			return cf;
		}

		public override Managers.TagDatabase CreateTagDatabase() { return new Halo2.Tags.TagDatabase(); }

		protected override Managers.CacheTagDatabase CreateCacheTagDatabaseInternal(DatumIndex cache_id) { return new Halo2.Tags.CacheTagDatabase((Halo2.CacheFile)Program.GetCacheFile(cache_id)); }

		public override Managers.ErrorTagDatabase CreateErrorTagDatabase() { return new Halo2.Tags.ErrorTagDatabase(); }

		public override TI.TagGroup TagDatabaseGroup { get { return Halo2.TagGroups.tag_; } }
		#endregion

		internal GameDefinition() { }

		#region IStringIdController Members
		// Sure, we could use a hashtable for keeping references and such, but this uses way less memory, 
		// and allows us to use the method logic below and make sure we're not trying to implement any unsupported
		// engine variants. Savvy?
		private int StringIdCacheReferencesXbox = 0, 
			StringIdCacheReferencesPC = 0, 
			StringIdCacheReferencesAlpha = 0, 
			StringIdCacheReferencesEpsilon = 0;

		/// <summary>
		/// <see cref="BlamLib.Managers.IStringIdController"/>
		/// </summary>
		/// <param name="game"></param>
		/// <returns></returns>
		public bool StringIdCacheOpen(BlamVersion game)
		{
			int count = 0;

			switch (game)
			{
				case BlamVersion.Halo2_Xbox:	count = Interlocked.Increment(ref StringIdCacheReferencesXbox);		break;
				case BlamVersion.Halo2_PC:		count = Interlocked.Increment(ref StringIdCacheReferencesPC);		break;
				case BlamVersion.Halo2_Alpha:	count = Interlocked.Increment(ref StringIdCacheReferencesAlpha);	break;
				case BlamVersion.Halo2_Epsilon:	count = Interlocked.Increment(ref StringIdCacheReferencesEpsilon);	break;

				default: throw new Debug.Exceptions.UnreachableException();
			}

			if(count == 1)
			{
				base.PrecacheResource(game, Managers.BlamDefinition.ResourceStringIds);
				return true;
			}
			else if (count == 0) throw new Debug.Exceptions.UnreachableException();

			return false;
		}

		/// <summary>
		/// <see cref="BlamLib.Managers.IStringIdController"/>
		/// </summary>
		/// <param name="game"></param>
		/// <returns></returns>
		public bool StringIdCacheClose(BlamVersion game)
		{
			int count = -1;

			switch (game)
			{
				case BlamVersion.Halo2_Xbox:	count = Interlocked.Decrement(ref StringIdCacheReferencesXbox);		break;
				case BlamVersion.Halo2_PC:		count = Interlocked.Decrement(ref StringIdCacheReferencesPC);		break;
				case BlamVersion.Halo2_Alpha:	count = Interlocked.Decrement(ref StringIdCacheReferencesAlpha);	break;
				case BlamVersion.Halo2_Epsilon:	count = Interlocked.Decrement(ref StringIdCacheReferencesEpsilon);	break;

				default: throw new Debug.Exceptions.UnreachableException();
			}

			if(count == 0) // since it's pre-decrement assigned, it will equal to zero when nothing is using it anymore
			{
				base.CloseResource(game, Managers.BlamDefinition.ResourceStringIds);
				return true;
			}
			else if (count == -1) throw new Debug.Exceptions.UnreachableException();

			return false;
		}
		#endregion

		#region IScriptingController Members
		int ScriptingCacheReferencesXbox = 0,
			ScriptingCacheReferencesPC = 0,
			ScriptingCacheReferencesAlpha = 0,
			ScriptingCacheReferencesEpsilon = 0;

		/// <summary>
		/// <see cref="BlamLib.Managers.IScriptingController"/>
		/// </summary>
		/// <param name="game"></param>
		/// <returns></returns>
		public bool ScriptingCacheOpen(BlamVersion game)
		{
			int count = 0;

			switch (game)
			{
				case BlamVersion.Halo2_Xbox:	count = Interlocked.Increment(ref ScriptingCacheReferencesXbox);	break;
				case BlamVersion.Halo2_PC:		count = Interlocked.Increment(ref ScriptingCacheReferencesPC);		break;
				case BlamVersion.Halo2_Alpha:	count = Interlocked.Increment(ref ScriptingCacheReferencesAlpha);	break;
				case BlamVersion.Halo2_Epsilon:	count = Interlocked.Increment(ref ScriptingCacheReferencesEpsilon);	break;

				default: throw new Debug.Exceptions.UnreachableException();
			}

			if(count == 1)
			{
				base.PrecacheResource(game, Managers.BlamDefinition.ResourceScripts);
				return true;
			}
			else if (count == 0) throw new Debug.Exceptions.UnreachableException();

			return false;
		}

		/// <summary>
		/// <see cref="BlamLib.Managers.IScriptingController"/>
		/// </summary>
		/// <param name="game"></param>
		/// <returns></returns>
		public bool ScriptingCacheClose(BlamVersion game)
		{
			int count = -1;

			switch (game)
			{
				case BlamVersion.Halo2_Xbox:	count = Interlocked.Decrement(ref ScriptingCacheReferencesXbox);	break;
				case BlamVersion.Halo2_PC:		count = Interlocked.Decrement(ref ScriptingCacheReferencesPC);		break;
				case BlamVersion.Halo2_Alpha:	count = Interlocked.Decrement(ref ScriptingCacheReferencesAlpha);	break;
				case BlamVersion.Halo2_Epsilon:	count = Interlocked.Decrement(ref ScriptingCacheReferencesEpsilon);	break;

				default: throw new Debug.Exceptions.UnreachableException();
			}

			if(count == 0) // since it's pre-decrement assigned, it will equal to zero when nothing is using it anymore
			{
				base.CloseResource(game, Managers.BlamDefinition.ResourceScripts);
				return true;
			}
			else if (count == -1) throw new Debug.Exceptions.UnreachableException();

			return false;
		}
		#endregion

		#region IVertexBufferController Members
		// Sure, we could use a hashtable for keeping references and such, but this uses way less memory, 
		// and allows us to use the method logic below and make sure we're not trying to implement any unsupported
		// engine variants. Savvy?
		int VertexBufferCacheReferencesXbox = 0, 
			VertexBufferCacheReferencesPC = 0, 
			VertexBufferCacheReferencesAlpha = 0,
			VertexBufferCacheReferencesEpsilon = 0;

		/// <summary>
		/// <see cref="BlamLib.Managers.IVertexBufferController"/>
		/// </summary>
		/// <param name="game"></param>
		/// <returns></returns>
		public bool VertexBufferCacheOpen(BlamVersion game)
		{
			int count = 0;

			switch (game)
			{
				case BlamVersion.Halo2_Xbox:	count = Interlocked.Increment(ref VertexBufferCacheReferencesXbox);		break;
				case BlamVersion.Halo2_PC:		count = Interlocked.Increment(ref VertexBufferCacheReferencesPC);		break;
				case BlamVersion.Halo2_Alpha:	count = Interlocked.Increment(ref VertexBufferCacheReferencesAlpha);	break;
				case BlamVersion.Halo2_Epsilon:	count = Interlocked.Increment(ref VertexBufferCacheReferencesEpsilon);	break;

				default: throw new Debug.Exceptions.UnreachableException();
			}

			if(count == 1)
			{
				base.PrecacheResource(game, Managers.BlamDefinition.ResourceVertexBuffers);
				return true;
			}
			else if (count == 0) throw new Debug.Exceptions.UnreachableException();

			return false;
		}

		/// <summary>
		/// <see cref="BlamLib.Managers.IVertexBufferController"/>
		/// </summary>
		/// <param name="game"></param>
		/// <returns></returns>
		public bool VertexBufferCacheClose(BlamVersion game)
		{
			int count = -1;

			switch (game)
			{
				case BlamVersion.Halo2_Xbox:	count = Interlocked.Decrement(ref VertexBufferCacheReferencesXbox);		break;
				case BlamVersion.Halo2_PC:		count = Interlocked.Decrement(ref VertexBufferCacheReferencesPC);		break;
				case BlamVersion.Halo2_Alpha:	count = Interlocked.Decrement(ref VertexBufferCacheReferencesAlpha);	break;
				case BlamVersion.Halo2_Epsilon:	count = Interlocked.Decrement(ref VertexBufferCacheReferencesEpsilon);	break;

				default: throw new Debug.Exceptions.UnreachableException();
			}

			if(count == 0) // since it's pre-decrement assigned, it will equal to zero when nothing is using it anymore
			{
				base.CloseResource(game, Managers.BlamDefinition.ResourceVertexBuffers);
				return true;
			}
			else if (count == -1) throw new Debug.Exceptions.UnreachableException();

			return false;
		}
		#endregion

		#region SettingsInterface
		/// <summary>Dirty Hacks, Done Dirt Cheap</summary>
		class _SettingsInterface : Managers.BlamDefinition.SettingsInterface
		{
			[System.ComponentModel.Category("Xbox - Alpha")]
			[System.ComponentModel.Description("Path to mainmenu.map")]
			public string AlphaMainmenuPath
			{
				get { return Program.Halo2.AlphaMainmenuPath; }
				set { Program.Halo2.AlphaMainmenuPath = value; }
			}
			[System.ComponentModel.Category("Xbox - Alpha")]
			[System.ComponentModel.Description("Path to shared.map")]
			public string AlphaSharedPath
			{
				get { return Program.Halo2.AlphaSharedPath; }
				set { Program.Halo2.AlphaSharedPath = value; }
			}

			[System.ComponentModel.Category("Xbox")]
			[System.ComponentModel.Description("Path to mainmenu.map")]
			public string XboxMainmenuPath
			{
				get { return Program.Halo2.XboxMainmenuPath; }
				set { Program.Halo2.XboxMainmenuPath = value; }
			}
			[System.ComponentModel.Category("Xbox")]
			[System.ComponentModel.Description("Path to shared.map")]
			public string XboxSharedPath
			{
				get { return Program.Halo2.XboxSharedPath; }
				set { Program.Halo2.XboxSharedPath = value; }
			}
			[System.ComponentModel.Category("Xbox")]
			[System.ComponentModel.Description("Path to single_player_shared.map")]
			public string XboxCampaignPath
			{
				get { return Program.Halo2.XboxCampaignPath; }
				set { Program.Halo2.XboxCampaignPath = value; }
			}

			[System.ComponentModel.Category("PC")]
			[System.ComponentModel.Description("Path to mainmenu.map")]
			public string PcMainmenuPath
			{
				get { return Program.Halo2.PcMainmenuPath; }
				set { Program.Halo2.PcMainmenuPath = value; }
			}
			[System.ComponentModel.Category("PC")]
			[System.ComponentModel.Description("Path to shared.map")]
			public string PcSharedPath
			{
				get { return Program.Halo2.PcSharedPath; }
				set { Program.Halo2.PcSharedPath = value; }
			}
			[System.ComponentModel.Category("PC")]
			[System.ComponentModel.Description("Path to single_player_shared.map")]
			public string PcCampaignPath
			{
				get { return Program.Halo2.PcCampaignPath; }
				set { Program.Halo2.PcCampaignPath = value; }
			}

			public override bool ValidateSettings()
			{
				return PathIsValid(AlphaMainmenuPath) && PathIsValid(AlphaSharedPath) &&
					PathIsValid(XboxMainmenuPath) && PathIsValid(XboxSharedPath) && PathIsValid(XboxCampaignPath) &&
					PathIsValid(PcMainmenuPath) && PathIsValid(PcSharedPath) && PathIsValid(PcCampaignPath);
			}

			public override void Read(IO.XmlStream s)
			{
			}

			public override void Write(System.Xml.XmlWriter writer)
			{
			}
		};
		_SettingsInterface settingsInterface = new _SettingsInterface();
		public override BlamLib.Managers.BlamDefinition.SettingsInterface Settings { get { return settingsInterface; } }
		#endregion
	};
}