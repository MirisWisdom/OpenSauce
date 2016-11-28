/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
﻿#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Halo2.Tags
{
	#region bitmap
	partial class bitmap_group
	{
		#region bitmap_data_block
		partial class bitmap_data_block
		{
			public Blam.ResourcePtr GetOffset(int index) { return (int)Offsets[index]; }
			public int GetSize(int index) { return Sizes[index]; }

			#region Cache postprocess
			internal override bool Reconstruct(BlamLib.Blam.CacheFile c)
			{
				LowDetailMipmapCount.Value = 0;
				return true;
			}

			bool CacheRead(BlamLib.Blam.CacheFile c, out byte[] data)
			{
				data = null;

				ResourcePtr offset = GetOffset(0);
				int size = GetSize(0);

				var rsrc_cache = Program.Halo2.FromLocation(c as Halo2.CacheFile, offset);

				// the shared cache isn't loaded, break
				if (rsrc_cache == null)
					return false;

				// get the input stream we need
				IO.EndianReader er = rsrc_cache.InputStream;

				// read the bitmap
				er.Seek(offset.Offset);
				data = er.ReadBytes(size);

				return true;
			}

			internal byte[] CacheRead(BlamLib.Blam.CacheFile c)
			{
				// read the cache block...
				byte[] bytes;
				if (!CacheRead(c, out bytes)) return null;

				if (c.EngineVersion == BlamVersion.Halo2_PC) // ...and perform zlib decompression
					bytes = Util.ZLibBufferFromBytes(bytes, 0, GetPixelDataSize(c.EngineVersion));

				return bytes;
			}
			#endregion

			public override int GetDepth() { return Depth.Value; }
			public override short GetMipmapCount() { return MipmapCount.Value; }
		};
		#endregion

		#region postprocess
		/* _bump
		 * objects\characters - 0xA
		 * objects\weapons - 0x6
		 * scenarios\objects - 0x4
		 * objects - 0xC
		 * ui\hud - 0xF
		 * ui - 0x10
		 * scenarios\skies - 0x11
		 * fp_arms - 0x12
		 * rasterizer - 0x13
		 * scenarios && (lightmap_bitmaps || lightmap_truecolor_bitmaps) - 0x1
		 * 0x3
		 * 
		 * 30h
1
5
2
0Fh
3
3
4
20h
5
22h
6
2Eh
7
2Ah
8
19h
9
23h
0Ah

16h
0Bh
1Ch
0Ch
26h
0Dh
23h
0Eh
24h
0Fh
0
10h
0Bh
11h
0Eh
12h
0Ch
13h
0
		 */
		#endregion

		internal override bool Reconstruct(Blam.CacheFile c)
		{
			//CompressedColorPlateData.Delete();

			Flags.Remove(1 << 5); // remove "UNUSED"

			// just checking the sanity...you never know if there will be a silly 
			// user building (with unofficial tools!) a cache with an empty bitmap
			if (Bitmaps.Count < 1) return true;

			int tag = Bitmaps[0].OwnerTagIndex;

			string filename = c.GetReferenceName(c.LocateTagByDatum((
				c.ExtractionState.CurrentTag == DatumIndex.Null ? tag : (int)c.ExtractionState.CurrentTag)));

			using (var ms = new System.IO.MemoryStream())
			{
				byte[] buffer;

				// read all of the bitmap data cache blocks
				for (int x = 0; x < Bitmaps.Count; x++)
				{
					if ((buffer = Bitmaps[x].CacheRead(c)) == null)
						Debug.LogFile.WriteLine("Failed to load bitmap data for '{0}' [{1}]", filename, x);
					else
						ms.Write(buffer, 0, buffer.Length);
				}

				// reset the pixel data
				ProcessedPixelData.Reset(ms.ToArray());
				buffer = null;
			}

			return true;
		}
	};
	#endregion

	#region multilingual_unicode_string_list
	partial class multilingual_unicode_string_list_group
	{
		static int LanguageIdGetReferenceIndex(int id)
		{
			return id & 0xFFFF;
		}
		static int LanguageIdGetCount(int id)
		{
			return (id >> 16) & 0xFFFF;
		}


		struct s_language_cache_interop
		{
			s_cache_language_pack languagePack;
			int referenceIndex;
			int referenceCount;
			int tagDataOffset;
			int tagDataSize;

			#region Initialize
			void DecodeHandle(int handle)
			{
				referenceIndex = LanguageIdGetReferenceIndex(handle);
				referenceCount = LanguageIdGetCount(handle);
			}
			void PredictRequiredStringDataSize()
			{
				if(referenceCount > 0)
					tagDataSize = languagePack.PredictRequiredStringDataSize(referenceIndex, referenceCount);
			}

			public int Initialize(s_cache_language_pack lang_pack, int tag_data_offset, int handle)
			{
				languagePack = lang_pack;
				tagDataOffset = tag_data_offset;

				DecodeHandle(handle);
				PredictRequiredStringDataSize();

				return tag_data_offset + tagDataSize;
			}
			#endregion

			#region Reconstruct
			static multilingual_unicode_string_reference_block AddOrGetReferenceByName(multilingual_unicode_string_list_group def, Blam.StringId name)
			{
				// See if a reference already exists for [name]...
				foreach (var sref in def.StringRefs)
					if (sref.StringId.Handle == name)
						return sref;

				// one doesn't, so add it and return the new block data
				multilingual_unicode_string_reference_block sr;
				def.StringRefs.Add(out sr);

				// Initialize name id
				sr.StringId.Handle = name;

				// Initialize all the offsets to be invalid
				foreach (var lang_offset in sr.LanguageOffsets)
					lang_offset.Value = -1;

				return sr;
			}
			public void ReconstructTagData(multilingual_unicode_string_list_group owner, LanguageType this_lang)
			{
				for (int x = 0, dst_offset = tagDataOffset, size; x < referenceCount; x++, dst_offset += size)
				{
					var dst = owner.StringData.Value;

					Blam.StringId name;
					size = languagePack.CopyStringReferenceData(referenceIndex + x, dst, dst_offset, out name);

					var sref = AddOrGetReferenceByName(owner, name);
					sref.LanguageOffsets[(int)this_lang].Value = dst_offset;
				}
			}
			#endregion
		};
		internal override bool Reconstruct(BlamLib.Blam.CacheFile c)
		{
			var clpi = c.TagIndexManager as ICacheLanguagePackContainer;

			if (clpi != null)
			{
				var h2_cache = c as Halo2.CacheFile;

				var interop = new s_language_cache_interop[(int)LanguageType.kMax];
				int total_string_buffer_size = 0;

				// For each language, get the string data referenced by this tag
				for (LanguageType lang = LanguageType.English; lang < LanguageType.kMax; lang++)
					total_string_buffer_size = interop[(int)lang].Initialize(clpi.LanguagePackGet(lang), 
						total_string_buffer_size, LanguageHandles[(int)lang]);

				// Resize the string data container to have enough memory for all of our language strings
				this.StringData.Value = new byte[total_string_buffer_size];

				// For each language, copy the string data and initialize the reference blocks
				for (LanguageType lang = LanguageType.English; lang < LanguageType.kMax; lang++)
					interop[(int)lang].ReconstructTagData(this, lang);

				// Null the handles for the extracted tag
				foreach (var handle in LanguageHandles)
					handle.Value = 0;

				return true;
			}

			return false;
		}
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

		public TagDatabase() : base(BlamVersion.Halo2) { }
		public TagDatabase(Managers.ITagDatabaseAddable root) : base(BlamVersion.Halo2, root) { }
		public TagDatabase(string tag_name, TagInterface.TagGroup group_tag) : base(BlamVersion.Halo2, tag_name, group_tag) { }

		protected override int Add(HandleData data)
		{
			int index = -1;

			tag_database_entry_block entry;
			if (!lookup.TryGetValue(data.Hash, out entry)) // hey, not in the database
			{
				index = db.Entries.Count;				// count will be the index of our new entry
				db.Entries.Add(out entry);				// add a new entry
				entry.Name.Value = data.Name;			// set the name
				entry.GroupTag.Value = data.GroupTag;	// set the group tag

				lookup.Add(data.Hash, entry);			// add entry to the dictionary
				return entry.Flags.Value = index;
			}

			return entry.Flags.Value;
		}

		protected override int AddDependent(HandleData tag, HandleData dependent_tag)
		{
			int index = Add(dependent_tag); // add and\or get the index of the child id entry

			tag_database_entry_reference_block eref;
			lookup[tag.Hash].ChildIds.Add(out eref); // add a new entry reference

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
				entry.Name.Value =						// set the name
					cacheFile.GetReferenceName(tag);
				entry.GroupTag.Value = tag.GroupTag.Tag;// set the group tag

				lookup.Add(tag.Datum, entry);			// add entry to the dictionary
				return entry.Flags.Value = index;
			}

			return entry.Flags.Value;
		}

		public override int AddDependent(Blam.CacheIndex.Item tag, Blam.CacheIndex.Item dependent_tag)
		{
			int index = Add(dependent_tag); // add and\or get the index of the child id entry

			tag_database_entry_reference_block eref;
			lookup[tag.Datum].ChildIds.Add(out eref); // add a new entry reference

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

		public ErrorTagDatabase() : base(BlamVersion.Halo2) { }

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
				entry.Name.Value = e.Path;	// set the name
				entry.GroupTag.Value = TI.TagGroup.FromUInt(e.GroupTag, false); // set the group tag

				lookup.Add(e, entry);		// add entry to the dictionary
				return (entry.Flags.Value = index) & 0xFFFF;
			}

			return entry.Flags.Value & 0xFFFF;
		}

		public override int AddDependent(string path, uint group_tag, ErrorFlags flags)
		{
			Entry e = new Entry(path, group_tag);
			int index = Add(path, group_tag, flags); // add and\or get the index of the child id entry

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