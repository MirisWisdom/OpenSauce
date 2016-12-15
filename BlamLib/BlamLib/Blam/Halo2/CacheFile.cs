/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
using System;
using System.Collections.Generic;
using System.Text;

namespace BlamLib.Blam.Halo2
{
	#region Header
	public class CacheHeaderPcFields : ICacheLanguagePackContainer
	{
		public uint VirtualAddress;

		public int TagDependencyGraphOffset;
		public uint TagDependencyGraphSize;

		public int LanguagePacksOffset;
		public uint LanguagePacksSizeOf;

		public DatumIndex SecondarySoundGestalt;

		public int FastLoadGeometryBlockOffset;
		public uint FastLoadGeometryBlockSize;

		public uint MoppCodesChecksum;

		internal CacheHeaderPcFields()
		{
			VirtualAddress = 0;

			TagDependencyGraphOffset = -1;
			TagDependencyGraphSize = 0;

			LanguagePacksOffset = -1;
			LanguagePacksSizeOf = 0;

			SecondarySoundGestalt = DatumIndex.Null;

			FastLoadGeometryBlockOffset = -1;
			FastLoadGeometryBlockSize = 0;

			MoppCodesChecksum = 0;
		}

		/// <summary>
		/// Does this cache have a dependency graph for sharing its tags?
		/// </summary>
		public bool HasTagDependencyGraph { get { return TagDependencyGraphOffset != -1; } }

		/// <summary>
		/// Does the cache use a "custom" sound gestalt?
		/// </summary>
		/// <remarks>Should should only be true for multiplayer-pc maps as they use a shared tag scheme</remarks>
		public bool UsesCustomSoundGestalt { get { return SecondarySoundGestalt != DatumIndex.Null; } }

		/// <summary>
		/// Does the cache include a cache language packing header in a resource section instead of the globals tag?
		/// </summary>
		/// <remarks>Should should only be true for multiplayer-pc maps as they use a shared tag scheme</remarks>
		public bool UsesCustomLanguagePack { get { return LanguagePacksOffset != -1; } }

		#region Tag Dependency Graph
		public SharedDependencyGraph TagDependencyGraph;

		internal void TagDependencyGraphReadFromCache(Halo2.CacheFile cf)
		{
			TagDependencyGraph = new SharedDependencyGraph();
			TagDependencyGraph.Read(cf.InputStream);
		}
		#endregion

		#region Language Packs
		s_cache_language_pack[] languagePacks;
		public s_cache_language_pack LanguagePackGet(LanguageType lang)
		{
			if(languagePacks != null)
				return languagePacks[(int)lang];

			return null;
		}

		internal void LanguagePackHeadersReadFromCache(Halo2.CacheFile cf)
		{
			const int k_expected_size = s_cache_language_pack.kSizeOf * (int)LanguageType.kMax;

			if (LanguagePacksOffset != -1 && LanguagePacksSizeOf == (uint)k_expected_size)
			{
				cf.InputStream.Seek(LanguagePacksOffset);

				languagePacks = new s_cache_language_pack[(int)LanguageType.kMax];
				for (int x = 0; x < languagePacks.Length; x++)
					(languagePacks[x] = new s_cache_language_pack(null)).Read(cf.InputStream);
			}
			else throw new Debug.ExceptionLog("Tried to read a custom language pack header with unexpected data! @{0} size: {1}", 
				LanguagePacksOffset.ToString("X8"), LanguagePacksSizeOf.ToString("X8"));
		}

		internal void LanguagePacksReadFromCache(Halo2.CacheFile cf)
		{
			foreach (var lp in languagePacks)
				lp.ReadFromCache(cf);
		}
		#endregion
	};
	/// <summary>
	/// Halo 2 implementation of the <see cref="Blam.CacheHeader"/>
	/// </summary>
	public sealed class CacheHeader : Cache.CacheHeaderGen2
	{
		#region IndexStreamSize
		int indexStreamSize;
		/// <summary>
		/// Total size in bytes to read into memory
		/// starting at OffsetToIndex
		/// </summary>
		public int IndexStreamSize	{ get { return indexStreamSize; } }
		#endregion
	

		internal System.Runtime.InteropServices.ComTypes.FILETIME Filetime;

		System.Runtime.InteropServices.ComTypes.FILETIME[] SharedFiletimes = new System.Runtime.InteropServices.ComTypes.FILETIME[3];


		public CacheHeaderPcFields PcFields = new CacheHeaderPcFields();
		

		public override void Read(BlamLib.IO.EndianReader s)
		{
			bool is_alpha;
			bool is_pc = false;
			Blam.CacheFile.ValidateHeader(s, 0x800);

			#region figure out if this is a alpha map
			s.Seek(0x120);
			is_alpha = s.ReadTagString() == "02.06.28.07902";
			#endregion
			#region figure out if this is a pc map
			if (!is_alpha)
			{
				s.Seek(0x24);
				int test = s.ReadInt32();
				if (test == -1 || (test != 0 && test < s.Length)) is_pc = true;
			}
			#endregion
			s.Seek(4);

			version = s.ReadInt32();
			fileLength = s.ReadInt32();
			s.ReadInt32();
			offsetToIndex = s.ReadInt32();
			indexStreamSize = s.ReadInt32();

			tagBufferSize = s.ReadInt32(); // size of the tag data, excluding the bsp data
			s.ReadInt32(); // max size: 0x3200000, indexStreamSize+tagBufferSize

			if(is_pc)
			{
				PcFields.VirtualAddress = s.ReadUInt32(); // virtual base address

				PcFields.TagDependencyGraphOffset = s.ReadInt32(); // buffer offset (starts right before tag index, 512 byte aligned). not shared? should equal -1
				PcFields.TagDependencyGraphSize = s.ReadUInt32(); // buffer size. not shared? should be zero. Max: 0x100000
			}

			sourceFile = s.ReadAsciiString(256);
			build = s.ReadTagString();
			cacheType = (Blam.CacheType)s.ReadInt32();
				// SP Filesize Max: 0x11800000
				// MP Filesize Max: 0x5000000
				// Mainmenu Filesize Max: 0x20800000
				// Shared Filesize Max: 0xB400000
				// Single Player Shared Filesize Max: 0x20800000
			s.ReadInt32(); // cache resource (tag) crc
			s.ReadInt32(); // boolean, set to true in pc when building cache

			s.ReadInt32(); s.ReadInt32();

			s.ReadInt32(); // count of some sort? only on the xbox

			// offset, then size
			// (size+0xFFF) & 0xFFFFF000
			// test that with 0x1FF, if true, or with 0x1FF then add 1
			s.ReadInt32(); // begins with 0x200 bytes of a chunk of the tag filenames table and ends with the same
			s.ReadInt32(); // has something to do with bitmaps. the bigger the file, the bigger this is

			#region string id
			// each string this buffer is aligned to 128 characters (the max length + 1 of a string_id)
			/*stringIdsBufferAlignedOffset = */s.ReadInt32();
			stringIdsCount = s.ReadInt32();

			stringIdsBufferSize = s.ReadInt32();
			stringIdIndicesOffset = s.ReadInt32();
			stringIdsBufferOffset = s.ReadInt32();
			#endregion

			s.ReadInt32(); // 4 bools

			Filetime.dwHighDateTime = s.ReadInt32();
			Filetime.dwLowDateTime = s.ReadInt32();

			/*MainmenuFiletime.dwHighDateTime*/				SharedFiletimes[0].dwHighDateTime = s.ReadInt32();
			/*MainmenuFiletime.dwLowDateTime = */			SharedFiletimes[0].dwLowDateTime = s.ReadInt32();

			/*SharedFiletime.dwHighDateTime*/				SharedFiletimes[1].dwHighDateTime = s.ReadInt32();
			/*SharedFiletime.dwLowDateTime*/				SharedFiletimes[1].dwLowDateTime = s.ReadInt32();

			/*SharedSingleplayerFiletime.dwHighDateTime*/	SharedFiletimes[2].dwHighDateTime = s.ReadInt32();
			/*SharedSingleplayerFiletime.dwLowDateTime*/	SharedFiletimes[2].dwLowDateTime = s.ReadInt32();

			name = s.ReadTagString();
			s.ReadInt32(); // dword, but unused
			scenarioPath = s.ReadAsciiString(256);

			// minor version
			needsShared = s.ReadInt32() == 1; // doesn't exist on the pc

			#region tag names
			tagNamesCount = s.ReadInt32(); // not used in alpha
			tagNamesBufferOffset = s.ReadInt32(); // not used in alpha
			tagNamesBufferSize = s.ReadInt32(); // not used in alpha
			tagNameIndicesOffset = s.ReadInt32(); // not used in alpha
			#endregion

			if (is_pc)
			{
				// s_cache_language_pack
				PcFields.LanguagePacksOffset = s.ReadInt32(); // offset, shared database = -1
				// sizeof(s_cache_language_pack) * LanguageType.kMax
				PcFields.LanguagePacksSizeOf = s.ReadUInt32(); // sizeof, shared database = 0

				// secondary sound gestalt
				PcFields.SecondarySoundGestalt.Read(s); // shared database = -1

				// fast geometry load region
				PcFields.FastLoadGeometryBlockOffset = s.ReadInt32(); // offset to a cache block resource
				PcFields.FastLoadGeometryBlockSize = s.ReadUInt32(); // sizeof entire cache block resources partition. max size: 0x4000000

				checksum = s.ReadUInt32();

				// another xor checksum
				// sbsp
				//	- data 0x28C: global_structure_physics_struct->MoppCode
				//	- data 0x2BC: global_structure_physics_struct->BreakableSurfacesMoppCode
				//	- block 0xD4: structure_bsp_cluster_block
				//		* data 0xC4: "Collision mopp Code"
				//		* block 0x54: structure_bsp_cluster_data_block_new
				//			- data 0x3C: global_geometry_section_struct->"Visibility mopp Code"
				// phmo
				//	- data 0xE8: "mopp codes"
				// coll
				//	- block 0x28: collision_model_region_block
				//		* block 0x04: collision_model_permutation_block
				//			- block 0x10: collision_bsp_physics_block
				//				* data 0x64: "mopp Code Data"
				// mode
				//	- block 0x30: render_model_section_block
				//		* block 0x34: render_model_section_data_block
				//			- data 0x3C: global_geometry_section_struct->"Visibility mopp Code"
				// ltmp
				//	- block 0x80: structure_lightmap_group_block
				//		* block 0x30: lightmap_geometry_section_block
				//			- block 0x54: lightmap_geometry_section_cache_data_block
				//				* data 0x3C: global_geometry_section_struct->"Visibility mopp Code"
				//		* block 0x48: lightmap_geometry_section_block
				//			- block 0x54: lightmap_geometry_section_cache_data_block
				//				* data 0x3C: global_geometry_section_struct->"Visibility mopp Code"
				PcFields.MoppCodesChecksum = s.ReadUInt32(); // shared databases = 0

				s.Seek(1284 + sizeof(uint), System.IO.SeekOrigin.Current);
			}
			else
			{

				checksum = s.ReadUInt32();

				s.Seek(1320 + sizeof(uint), System.IO.SeekOrigin.Current);
			}

			CacheFile cf = s.Owner as CacheFile;
			if (is_pc) cf.EngineVersion = BlamVersion.Halo2_PC;
			else if (build == "02.08.28.09214") cf.EngineVersion = BlamVersion.Halo2_Epsilon;
			else if (is_alpha) cf.EngineVersion = BlamVersion.Halo2_Alpha;
			else cf.EngineVersion = BlamVersion.Halo2_Xbox;
		}

		public override void Write(BlamLib.IO.EndianWriter s)
		{
			CacheFile cf = s.Owner as CacheFile;
			if (cf.EngineVersion == BlamVersion.Halo2_PC) return;
			if (cf.EngineVersion == BlamVersion.Halo2_Alpha) return;

			s.WriteTag((char[])MiscGroups.head);
			s.Write(8);
			s.Write(fileLength);
			s.Write(0);
			s.Write(offsetToIndex);
			s.Write(indexStreamSize);
			s.Write(tagBufferSize);
			s.Write(0); // needs to be calc'd

			s.Write(new byte[256]);
			s.Write("02.09.27.09809", false);
			s.Write((int)cacheType);
			s.Write(0); // needs to be calc'd
			s.Write(0); // needs to be calc'd
			s.Write(0); s.Write(0);

			s.Write(0); // needs to be calc'd

			s.Write(0); // needs to be calc'd
			s.Write(0); // needs to be calc'd

			s.Write(/*stringIdsBufferAlignedOffset*/0);
			s.Write(stringIdsCount);

			s.Write(stringIdsBufferSize);
			s.Write(stringIdIndicesOffset);
			s.Write(stringIdsBufferOffset);

			s.Write(0); // 4 bools

			s.Write(Filetime.dwHighDateTime);			s.Write(Filetime.dwHighDateTime);

			// mainmenu
			s.Write(SharedFiletimes[0].dwHighDateTime);	s.Write(SharedFiletimes[0].dwHighDateTime);
			// shared
			s.Write(SharedFiletimes[1].dwHighDateTime);	s.Write(SharedFiletimes[1].dwHighDateTime);
			// shared sp
			s.Write(SharedFiletimes[2].dwHighDateTime);	s.Write(SharedFiletimes[2].dwHighDateTime);

			s.Write(name, false);
			s.Write(0);
			s.Write(scenarioPath, 256);
			s.Write(Convert.ToInt32(needsShared));

			s.Write(tagNamesCount);
			s.Write(tagNamesBufferOffset);
			s.Write(tagNamesBufferSize);
			s.Write(tagNameIndicesOffset);

			s.Write(0); // checksum

			s.Write(new byte[1320]);

			s.WriteTag((char[])MiscGroups.foot);
		}
	};
	#endregion

	#region Index
	/// <summary>
	/// Halo 2 implementation of the <see cref="Blam.CacheIndex"/>
	/// </summary>
	public sealed class CacheIndex : Blam.Cache.CacheIndexGen2
	{
		#region Tags
		CacheItem[] items;
		public override Blam.CacheIndex.Item[] Tags { get { return items; } }

		public CacheItem this[int index]	{ get { return items[index]; } }
		#endregion

		#region GameGlobals
		DatumIndex gameGlobals;
		/// <summary>
		/// Handle to this cache file's game globals definition
		/// </summary>
		public DatumIndex GameGlobals { get { return gameGlobals; } }
		#endregion

		/// <remarks>Related to PC only</remarks>
		public bool HasExternalTags = false;

		public override void Read(BlamLib.IO.EndianReader s)
		{
			CacheFile cache = s.Owner as CacheFile;
			tagsOffset = (uint)(cache.Header.OffsetToIndex + cache.HeaderHalo2.IndexStreamSize);
			bool is_alpha = cache.EngineVersion == BlamVersion.Halo2_Alpha;
			bool is_echo = cache.EngineVersion == BlamVersion.Halo2_Epsilon;
			bool is_pc = cache.EngineVersion == BlamVersion.Halo2_PC;
			bool is_mp = cache.HeaderHalo2.CacheType == CacheType.Multiplayer;

			Managers.BlamDefinition bdef = Program.GetManager(cache.EngineVersion);

			uint tags_addressmask;
			if(!is_pc)
			{
				cache.AddressMask = 
					bdef[cache.EngineVersion].CacheTypes.BaseAddress - (uint)cache.Header.OffsetToIndex;
				tags_addressmask =
					(uint)(cache.Header.OffsetToIndex + cache.HeaderHalo2.IndexStreamSize);
			}
			else
			{
				// pc maps use virtual addresses which are actually offsets relative to 
				// the start of the tag memory buffer. since these are offsets and not actually 
				// addresses which we would normally mask the base off of, we have to do some 
				// number magic so our subtraction operations actually end up working in reverse 
				// to get the correct file offset
				tags_addressmask = 0 - (uint)cache.Header.OffsetToIndex;

				cache.AddressMask = tags_addressmask;
			}

			#region version dependant loading
			if (is_alpha)
			{
				groupTagsAddress = address = s.ReadUInt32();
				groupTagsCount = 0;
				scenario.Read(s);
				s.ReadInt32(); // crc
				tagCount = s.ReadInt32();
				items = new CacheItem[tagCount];
				s.ReadInt32(); // 'tags'
			}
			else if(is_pc)
			{
				groupTagsAddress = s.ReadPointer(); // offset (relative to the tag index offset) 
				groupTagsCount = s.ReadInt32();
				uint offset = s.ReadPointer(); // offset (relative to the tag index offset) to the tag entries
				scenario.Read(s);
				gameGlobals.Read(s);
				s.ReadInt32(); // crc
				tagCount = s.ReadInt32();
				items = new CacheItem[tagCount];
				s.ReadInt32(); // 'tags'

				s.Seek(offset /*- 32, System.IO.SeekOrigin.Current*/); // go to the first tag entry
			}
			else
			{
				groupTagsAddress = s.ReadUInt32();
				groupTagsCount = s.ReadInt32();
				address = s.ReadUInt32();
				scenario.Read(s);
				gameGlobals.Read(s);
				s.ReadInt32(); // crc
				tagCount = s.ReadInt32();
				items = new CacheItem[tagCount];
				s.ReadInt32(); // 'tags'

				s.Seek(groupTagsCount * 12, System.IO.SeekOrigin.Current); // go to the first tag entry
			}

			this.groupTagsOffset = this.groupTagsAddress - s.BaseAddress;
			#endregion

			CacheItem item;
			uint temp_pos = 0;
			uint sbsp_offset = 0;

			DatumIndex[] ltmps = null;
			Tags.scenario_structure_bsp_reference_block bsp_block = new Tags.scenario_structure_bsp_reference_block();

			if (is_pc) // MP maps need this adjustment
				cache.AddressMask = tags_addressmask += cache.HeaderHalo2.PcFields.VirtualAddress;

			for (int x = 0; x < items.Length; x++)
			{
				item = new CacheItem();
				items[x] = item;
				if (is_alpha)	item.ReadAlpha(s);
				else			item.Read(s);

				if (item.Location == ItemLocation.Unknown) items[x] = CacheItem.Null;
				else if (is_pc && item.HasExternalData) HasExternalTags = true;
			}

			if (!is_pc)
			{
				// While the tag definitions come right after the tag header in a cache file, when 
				// finally loaded into game memory this isn't the case. The 'stream' size of the tag 
				// header is how much actual memory is used by map's generated the tag header, but the 
				// map's tag header may not utilize the entire memory space dedicated to it in game memory.
				// So, the game would read the tag header data using the offset and 'stream' size data from 
				// the cache header, then it would seek to offset+stream_size to get to the tag definitions 
				// which it would then read into game memory at the memory location defined by 'address'
				tags_addressmask =
					items[0].Address - tags_addressmask;
				cache.AddressMask = tags_addressmask;
				for (int x = 0; x < items.Length; x++)
				{
					item = items[x];
					item.Offset = (int)(item.Address - tags_addressmask);

					#region on scnr tag
					if (!is_alpha && !is_echo && TagGroups.scnr.ID == item.GroupTag.ID)
					{
						temp_pos = s.PositionUnsigned;

						//if (is_alpha || is_echo)
						//	s.Seek(item.Offset + 828);
						//else
							s.Seek(item.Offset + 528);

						bspTags = new Item[s.ReadInt32()];
						sbsp_offset = s.ReadPointer();

						ltmps = new DatumIndex[bspTags.Length];

						s.Seek(temp_pos);
					}
					#endregion
					#region on sbsp tag
					else if (!is_alpha && !is_echo && TagGroups.sbsp.ID == item.GroupTag.ID)
					{
						temp_pos = s.PositionUnsigned;

						s.Seek(sbsp_offset + (uint)(bspCount *
							Halo2.Tags.scenario_structure_bsp_reference_block.kRuntimeSizeOf));
						bspTags[bspCount] = item;
						bsp_block.Read(cache);

						if (bsp_block.RuntimeOffset != 0)
						{
							item.Offset = bsp_block.RuntimeOffset;
							item.Size = bsp_block.RuntimeSize;
							item.Address = (uint)bsp_block.RuntimeAddress.Value;
							//cache.BspAddressMasks.Add((uint)(item.Address - item.Offset));
						}

						ltmps[bspCount] = bsp_block.Lightmap.Datum;
						item.BspIndex = bspCount++;

						s.Seek(temp_pos);
					}
					#endregion
				}
			}

			#region alpha tag name code
			if (is_alpha)
			{
				// following the tag datums in alpha builds is the tag names buffer
				foreach (Halo2.CacheItem ci in items)
				{
					ci.TagNameOffset = s.PositionUnsigned;
					ci.ReferenceName = cache.References.AddOptimized(ci.GroupTag, s.ReadCString());
				}
			}
			#endregion
			#region retail tag name & bsp offset fixup code
			else
			{
				// Build the absolute tag name offsets
				s.Seek(cache.HeaderHalo2.TagNameIndicesOffset);
				int[] offsets = new int[tagCount];
				for (int x = 0; x < offsets.Length; x++)
				{
					int offset = s.ReadInt32();
					// Offset will be -1 if the tag in question is 'null'
					if (offset != -1)
						offset += cache.HeaderHalo2.TagNamesBufferOffset;

					offsets[x] = offset;
				}
				// Fixup all tag instances which are named
				for (int x = 0; x < tagCount; x++)
				{
					if (offsets[x] != -1)
						FixupTagInstanceHeaderName(cache, items[x], offsets[x], s);
				}

				// PC maps store all zones in the tag memory, they don't need to swap out and thus don't 
				// need any fix ups (durrr, PCs have loltons of RAM)
				if (!is_pc && !is_echo)
				{
					var head = new Halo2.Tags.scenario_structure_bsps_header();
					foreach (CacheItem tmp_item in bspTags)
					{
						s.Seek(tmp_item.Offset);
						head.Read(cache);

						// bsp
						uint bsp_address_mask = head.FixupBspInstanceHeader(tmp_item, s.Position);
						cache.BspAddressMasks.Add(bsp_address_mask);

						// ltmp
						DatumIndex ltmp_datum = ltmps[tmp_item.BspIndex];
						if (ltmp_datum != DatumIndex.Null)
							head.FixupLightmapInstanceHeader(this.items[ltmp_datum.Index], tmp_item);
					}
				}
			}
			#endregion
		}

		static void FixupTagInstanceHeaderName(CacheFile cache, CacheItem instance, int name_offset, IO.EndianReader s)
		{
			s.Seek(name_offset);
			instance.TagNameOffset = s.PositionUnsigned;
			instance.ReferenceName = cache.References.AddOptimized(instance.GroupTag, s.ReadCString());
		}

		public CacheItemGroupTag[] ReadGroupTags(BlamLib.IO.EndianReader s)
		{
			CacheItemGroupTag[] group_tags = new CacheItemGroupTag[groupTagsCount];
			s.Seek(groupTagsOffset, System.IO.SeekOrigin.Begin);
			for (int x = 0; x < groupTagsCount; x++)
				(group_tags[x] = new CacheItemGroupTag()).Read(s);

			return group_tags;
		}

		internal override Item AddFeignItem(BlamLib.Blam.CacheFile cf, string tag_name, BlamLib.TagInterface.TagGroup group_tag)
		{
			var last_item = items[items.Length - 1];
			CacheItem item = new CacheItem();
			item.InitializeForFeigning(last_item.Datum, group_tag);

			// We don't use the optimized version
			item.ReferenceName = cf.References.Add(group_tag, tag_name);
			if (item.ReferenceName == DatumIndex.Null)
				throw new Debug.ExceptionLog("Unable to create feign item's name reference!");

			Array.Resize(ref items, items.Length + 1);
			
			return items[items.Length - 1] = item;
		}
	};
	#endregion

	#region ItemGroupTag
	public sealed class CacheItemGroupTag : Cache.CacheItemGroupTagGen2
	{
		#region IStreamable Members
		public override void Read(BlamLib.IO.EndianReader s)
		{
			var gd = Program.Halo2.Manager;

			uint gt = s.ReadUInt32();
			if (gt != uint.MaxValue)	GroupTag1 = gd.TagGroupFind(TagInterface.TagGroup.FromUInt(gt));
			else						GroupTag1 = TagInterface.TagGroup.Null;

			gt = s.ReadUInt32();
			if (gt != uint.MaxValue)	GroupTag2 = gd.TagGroupFind(TagInterface.TagGroup.FromUInt(gt));
			else						GroupTag2 = TagInterface.TagGroup.Null;

			gt = s.ReadUInt32();
			if (gt != uint.MaxValue)	GroupTag3 = gd.TagGroupFind(TagInterface.TagGroup.FromUInt(gt));
			else						GroupTag3 = TagInterface.TagGroup.Null;
		}
		#endregion
	};
	#endregion

	#region Item
	/// <summary>
	/// Halo 2 implementation of the <see cref="Blam.CacheIndex.Item"/>
	/// </summary>
	public sealed class CacheItem : Blam.CacheIndex.Item
	{
		#region Null
		public static readonly CacheItem Null = new CacheItem();
		static CacheItem()
		{
			Null.referenceName = DatumIndex.Null;
			Null.offset = -1;
			Null.tagNameOffset = Null.address = 0xFFFFFFFF;
			Null.size = -1;
			Null.datum = DatumIndex.Null;
			Null.groupTag = TagInterface.TagGroup.Null;
			Null.bspIndex = -1;
			Null.location = CacheIndex.ItemLocation.Unknown;
		}
		#endregion

		public override void Read(IO.EndianReader s)
		{
			GroupTagInt = s.ReadUInt32();
			groupTag = Program.Halo2.Manager.TagGroupFind(TagInterface.TagGroup.FromUInt(GroupTagInt));
			IO.ByteSwap.SwapUDWord(ref GroupTagInt);

			datum.Read(s);
			address = s.ReadUInt32();

			if (GroupTagInt == uint.MaxValue)	location = CacheIndex.ItemLocation.Unknown;
			else if (address == 0)				location = CacheIndex.ItemLocation.External;
			else								offset = (int)(address - s.BaseAddress);

			size = s.ReadInt32();
		}

		public void ReadAlpha(IO.EndianReader s)
		{
			GroupTagInt = s.ReadUInt32();
			groupTag = Program.Halo2.Manager.TagGroupFind(TagInterface.TagGroup.FromUInt(GroupTagInt));
			IO.ByteSwap.SwapUDWord(ref GroupTagInt);

			s.ReadUInt32(); s.ReadUInt32();
			datum.Read(s);
			tagNameOffset = s.ReadUInt32();
			address = s.ReadUInt32();			//offset = (int)(address - s.BaseAddress);
			size = s.ReadInt32();
			s.ReadInt32();
		}
	};
	#endregion

	// pc mp maps seem to have a max tag count of 10,000. non-pc seems to be double that.
	// NOTE: this is probably due PC using a shared tag (not just resource) model in multiplayer
	#region File
	/// <summary>
	/// Bit values for use in <see cref="Halo2.CacheFile"/>'s 
	/// <see cref="Blam.CacheFile.Flags"/> field
	/// </summary>
	public static class CacheFlags
	{
		/// <summary>
		/// Preform the code thats done when reconstructing a tag for tool editor runtime
		/// </summary>
		public const uint ReconstructForExtraction = 1 << (CacheFile.NextBitIndex + 0);
		/// <summary>
		/// Perform the code thats done when loading a cache file for blamlib editing
		/// </summary>
		public const uint ReconstructForEditor = 1 << (CacheFile.NextBitIndex + 1);
		/// <summary>
		/// Perform the code that's done when tearing down a cache or parts of 
		/// its tags for later reconstruction into another map
		/// </summary>
		public const uint ReconstructForRebuilding = 1 << (CacheFile.NextBitIndex + 2);
	};

	/// <summary>
	/// Halo 2 implementation of the <see cref="Blam.CacheFile"/>
	/// </summary>
	public sealed class CacheFile : Blam.Cache.CacheFileGen2
	{
		#region Header
		Halo2.CacheHeader cacheHeader = null;
		public override BlamLib.Blam.CacheHeader Header { get { return cacheHeader; } }

		public Halo2.CacheHeader HeaderHalo2 { get { return cacheHeader; } }
		#endregion

		public override string GetUniqueName()
		{
			var ft = HeaderHalo2.Filetime;
			return string.Format("{0}_{1}{2}", HeaderHalo2.Name, ft.dwHighDateTime.ToString("X8"), ft.dwLowDateTime.ToString("X8"));
		}

		#region Index
		Halo2.CacheIndex cacheIndex = null;
		public override BlamLib.Blam.CacheIndex Index { get { return cacheIndex; } }

		public Halo2.CacheIndex IndexHalo2 { get { return cacheIndex; } }
		#endregion

		#region HasExternalReferences
		/// <summary>
		/// Returns whether this cache has tags which are
		/// stored in another cache
		/// </summary>
		/// <remarks>Related to PC only</remarks>
		public bool HasExternalReferences { get { return cacheIndex.HasExternalTags; } }
		#endregion

		bool SharableReferenceXbox(string path)
		{
			if (SharableReference(path, Program.Halo2.XboxMainmenu))		ShareCacheStreams(this, Program.Halo2.XboxMainmenu);
			else if (SharableReference(path, Program.Halo2.XboxShared))		ShareCacheStreams(this, Program.Halo2.XboxShared);
			else if (SharableReference(path, Program.Halo2.XboxCampaign))	ShareCacheStreams(this, Program.Halo2.XboxCampaign);
			else return false;

			return true;
		}

		bool SharableReferencePc(string path)
		{
			if (SharableReference(path, Program.Halo2.PcMainmenu))		ShareCacheStreams(this, Program.Halo2.PcMainmenu);
			else if (SharableReference(path, Program.Halo2.PcShared))	ShareCacheStreams(this, Program.Halo2.PcShared);
			else if (SharableReference(path, Program.Halo2.PcCampaign))	ShareCacheStreams(this, Program.Halo2.PcCampaign);
			else return false;

			return true;
		}

		public CacheFile(string map_name)
		{
			if (!SharableReferenceXbox(map_name) && !SharableReferencePc(map_name))
			{
				InputStream = new BlamLib.IO.EndianReader(map_name, BlamLib.IO.EndianState.Little, this);
				if(!CacheIsReadonly(map_name))
					OutputStream = new BlamLib.IO.EndianWriter(map_name, BlamLib.IO.EndianState.Little, this);
			}

			cacheHeader = new CacheHeader();
			cacheIndex = new CacheIndex();
		}

		protected override Managers.CacheTagIndex NewTagIndexImplementation()
		{
			return new InternalCacheTagIndex(this);
		}

		void TagIndexInitializeAndRead(BlamLib.IO.EndianReader s)
		{
			base.StringIdManagerInitializeAndRead();
			base.InitializeReferenceManager(s.FileName);
			base.InitializeTagIndexManager();

			s.Seek(cacheHeader.OffsetToIndex, System.IO.SeekOrigin.Begin);
			cacheIndex.Read(s);
		}

		public override void Read(BlamLib.IO.EndianReader s)
		{
			if (isLoaded) return;

			// goto the start if we're leaching off another cache's IO
			if (IsSharedReference)
				s.Seek(0);

			cacheHeader.Read(s);

			TagIndexInitializeAndRead(s);

			isLoaded = true;
		}
	};

	internal class InternalCacheTagIndex : Managers.CacheTagIndex, ICacheLanguagePackContainer
	{
		#region ICacheLanguagePackContainer
		ICacheLanguagePackContainer cacheLanguagePackContainer = null;
		public s_cache_language_pack LanguagePackGet(LanguageType lang)
		{
			return cacheLanguagePackContainer.LanguagePackGet(lang);
		}
		#endregion

		public InternalCacheTagIndex(Blam.CacheFile cf) : base(cf) {}

		// We need to load the globals tag so we properly extract unicode string lists
		// oh and then there is that little tag called the sound_cache_file_gestalt...
		DatumIndex globals_handle = DatumIndex.Null,
			sound_gestalt_handle = DatumIndex.Null;

		// Index to tag datums of the "invalid" variants of specific tags
		DatumIndex invalid_handle_shader = DatumIndex.Null;

		public Render.VertexBufferInterface.VertexBuffersGen2 kVertexBuffers;

		internal DatumIndex SoundGestaltTagIndex { get { return sound_gestalt_handle; } }

		public override void ExtractionInitialize()
		{
			// If we have kVertexBuffers initialized, then that means we've already ran extract init.
			if (kVertexBuffers != null) return;

			base.ExtractionInitialize();

			var h2cf = base.cacheFile as CacheFile;

			Program.Halo2.Manager.VertexBufferCacheOpen(Engine);
			kVertexBuffers = Program.Halo2.Manager.FindGame(Engine).GetResource
				<Render.VertexBufferInterface.VertexBuffersGen2>
				(Managers.BlamDefinition.ResourceVertexBuffers);

			// We only need the globals tag for language pack headers. If this is a PC multiplayer map, then 
			// the headers are stored elsewhere in the cache so we don't have to exception when there is no globals tag
			if ((globals_handle = base.Open(@"globals\globals", TagGroups.matg)) == DatumIndex.Null
				&& Engine != BlamVersion.Halo2_PC)
				throw new Debug.ExceptionLog("ExtractionInitialize: couldn't open the globals tag!");

			#region Initialze the cache language pack interface
			if (globals_handle != DatumIndex.Null)
			{
				var globals = (base[globals_handle].TagDefinition as Tags.globals_group);
				globals.LanguagePacksReadFromCache(h2cf);

				cacheLanguagePackContainer = globals;
			}
			// either this map's globals tag is fucked up or it's a MP map for PC and thus doesn't have a local copy)
			else if (h2cf.HeaderHalo2.PcFields.UsesCustomLanguagePack)
			{
				var pc_fields = h2cf.HeaderHalo2.PcFields;
				pc_fields.LanguagePackHeadersReadFromCache(h2cf);
				pc_fields.LanguagePacksReadFromCache(h2cf);

				cacheLanguagePackContainer = pc_fields;
			}
			#endregion

			// TODO: we may want to use the tag_index found in global's sound_block instead...
			if ((sound_gestalt_handle = base.Open(@"i've got a lovely bunch of coconuts", TagGroups.ugh_)) == DatumIndex.Null)
			{
				if (h2cf.HeaderHalo2.PcFields.UsesCustomSoundGestalt)
					sound_gestalt_handle = base.Open(h2cf.HeaderHalo2.PcFields.SecondarySoundGestalt);

				if(sound_gestalt_handle == DatumIndex.Null
					&& Engine != BlamVersion.Halo2_PC) // TODO: tag sharing logic
					throw new Debug.ExceptionLog("ExtractionInitialize: couldn't open the sound gestalt!");
			}

			#region Initialize the feign invalid-shader tag
			const string invalid_handle_shader_name = @"shaders\invalid";

			Blam.CacheIndex.Item i;
			if (h2cf.TryAndFind(invalid_handle_shader_name, TagGroups.shad, out i))
				invalid_handle_shader = i.Datum;
			else
				invalid_handle_shader = h2cf.AddFeignTagInstance(invalid_handle_shader_name, TagGroups.shad).Datum;
			#endregion
		}

		public override void ExtractionDispose()
		{
			cacheLanguagePackContainer = null;

			if (globals_handle != DatumIndex.Null)
			{
				Unload(globals_handle);
				globals_handle = DatumIndex.Null;
			}
			if (sound_gestalt_handle != DatumIndex.Null)
			{
				Unload(sound_gestalt_handle);
				sound_gestalt_handle = DatumIndex.Null;
			}

			if (kVertexBuffers != null)
			{
				kVertexBuffers = null;
				Program.Halo2.Manager.VertexBufferCacheClose(cacheFile.EngineVersion);
			}

			base.ExtractionDispose();
		}

		internal override bool ExtractionTagReferenceChange(TagInterface.TagReference tr)
		{
			bool changed = false;

			if(tr.Datum != DatumIndex.Null)
			{
// 				if (tr.GroupTag == TagGroups.shad)
// 				{
// 					tr.Datum = invalid_handle_shader;
// 					changed = true;
// 				}
			}

			return changed;
		}
	};
	#endregion

	#region Resource Database
	struct SharedDependencyGraphEntry : IO.IStreamable
	{
		internal const int kSizeOf = 4 + 4;

		/// <summary>
		/// Index of the first dependent of the tag this entry describes
		/// </summary>
		public int Start;
		/// <summary>
		/// How many dependents the tag this entry describes has
		/// </summary>
		public int Count;

		public bool IsNull { get { return Start == -1; } }

		#region IStreamable Members
		public void Read(BlamLib.IO.EndianReader s)
		{
			Start = s.ReadInt32();
			Count = s.ReadInt32();
		}

		public void Write(BlamLib.IO.EndianWriter s)
		{
			s.Write(Start);
			s.Write(Count);
		}
		#endregion
	};
	public class SharedDependencyGraph : IO.IStreamable
	{
		public const int kNumberOfRelativeTagInstances = 0x2710;

		List<SharedDependencyGraphEntry> Entries;
		public List<DatumIndex> Dependencies;

		public bool GetDependencyRange(DatumIndex tag_index, out int start_index, out int count)
		{
			start_index = -1;
			count = 0;

			if(tag_index != DatumIndex.Null && tag_index.Index >= kNumberOfRelativeTagInstances)
			{
				var entry = Entries[tag_index.Index - kNumberOfRelativeTagInstances];

				if(!entry.IsNull)
				{
					start_index = entry.Start;
					count = entry.Count;

					return true;
				}
			}

			return false;
		}

		#region IStreamable Members
		public void Read(BlamLib.IO.EndianReader s)
		{
			var ch = (s.Owner as CacheFile).HeaderHalo2;
			uint total_size = ch.PcFields.TagDependencyGraphSize;

			int count = s.ReadInt32();
			Entries = new List<SharedDependencyGraphEntry>(count);

			count = (int)((total_size - sizeof(int) -
				(Entries.Count - SharedDependencyGraphEntry.kSizeOf)) / DatumIndex.kSizeOf);
			Dependencies = new List<DatumIndex>(count);


			for (int x = 0; x < Entries.Count; x++)		Entries[x].Read(s);
			for (int x = 0; x < Dependencies.Count; x++)Dependencies[x].Read(s);
		}

		public void Write(BlamLib.IO.EndianWriter s)
		{
			s.Write(Entries.Count);
			foreach (var entry in Entries)		entry.Write(s);
			foreach (var datum in Dependencies)	datum.Write(s);
		}
		#endregion
	};

	/// <summary>
	/// Interface to the item data found in a Halo 2 cache resource
	/// database file
	/// </summary>
	public sealed class CacheResourceDatabaseItem : IO.IStreamable
	{
		#region Size
		int size;
		/// <summary>
		/// Size of this item's data block
		/// </summary>
		public int Size
		{
			get { return size; }
			set { size = value; }
		}
		#endregion

		#region NameHash
		uint nameHash;
		/// <summary>
		/// Hash to this item's name
		/// </summary>
		public uint NameHash
		{
			get { return nameHash; }
			set { nameHash = value; }
		}
		#endregion

		#region Offset
		ResourcePtr offset;
		/// <summary>
		/// Offset to this item's data block
		/// </summary>
		public ResourcePtr Offset
		{
			get { return offset; }
			set { offset = value; }
		}
		#endregion

		#region IStreamable Members
		public void Read(BlamLib.IO.EndianReader s)
		{
			// cache resource object ptr
			s.ReadInt32(); // only used at runtime, reset when read from disk
			size = s.ReadInt32();
			// data ptr
			s.ReadInt32(); // only used at runtime, reset when read from disk
			nameHash = s.ReadUInt32();
			offset.Read(s);
		}

		public void Write(BlamLib.IO.EndianWriter s)
		{
			s.Write(0);
			s.Write(size);
			s.Write(0);
			s.Write(nameHash);
			offset.Write(s);
		}
		#endregion
	};
	/// <summary>
	/// Interface to a Halo 2 cache resource database file
	/// </summary>
	public sealed class CacheResourceDatabase : IO.IStreamable
	{
		const int kVersion = 1;

		#region Items
		List<CacheResourceDatabaseItem> items;
		/// <summary>
		/// 
		/// </summary>
		public List<CacheResourceDatabaseItem> Items
		{
			get { return items; }
			set { items = value; }
		}
		#endregion

		#region IStreamable Members
		public void Read(BlamLib.IO.EndianReader s)
		{
			if (!MiscGroups.crdb.Test(s.ReadTag())) throw new Debug.ExceptionLog("{0} is not a resource db!", s.FileName);
			if (s.ReadInt32() != kVersion) throw new Debug.ExceptionLog("{0} is an unsupported db!", s.FileName);
			items = new List<CacheResourceDatabaseItem>(s.ReadInt32());
			for(int x = 0; x < items.Count; x++)
				(items[x] = new CacheResourceDatabaseItem()).Read(s);
		}

		public void Write(BlamLib.IO.EndianWriter s)
		{
			MiscGroups.crdb.Write(s);
			s.Write(kVersion);
			s.Write(items.Count);
			for (int x = 0; x < items.Count; x++) items[x].Write(s);
		}
		#endregion
	};
	#endregion
}