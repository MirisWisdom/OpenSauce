/*
	BlamLib: .NET SDK for the Blam Engine

	See license\BlamLib\BlamLib for specific license information
*/
#pragma warning disable 1591 // "Missing XML comment for publicly visible type or member"
using System;
using TI = BlamLib.TagInterface;

namespace BlamLib.Blam.Cache.Tags
{
	#region cache_file_resource_layout_table
	partial class cache_file_resource_layout_table
	{
		#region pages_block
		partial class pages_block
		{
			#region Page and Segment loading
			System.Collections.Generic.List<int> 
				SegmentOffsets = new System.Collections.Generic.List<int>(),
				SegmentSizes = new System.Collections.Generic.List<int>();
			internal void AddSegment(int offset)
			{
				// If this page isn't null and the offset isn't already recorded...
				if (BlockSizeUncompressed.Value != 0 && !SegmentOffsets.Contains(offset))
				{
					SegmentOffsets.Add(offset);
					SegmentSizes.Add(-1); // we'll update this when we post-process in 'CalculateSegmentSizes'
				}
			}

			internal void CalculateSegmentSizes()
			{
				// If this page isn't null
				if (BlockSizeUncompressed.Value == 0) return;

				// We work backwards to find the sizes as we can just find the 
				// distances between each segment. For the last segment we can 
				// just use the page size minus the offset to find it's size
				int last_element = SegmentOffsets.Count-1;

				// TODO: there should be at least one segment referencing this page...?
				if(last_element == -1)
				{
					SegmentOffsets.Add(0);
					SegmentSizes.Add(BlockSizeUncompressed.Value);
				}
				// we only have one element so no point in loopin'
				else if (last_element == 0) SegmentSizes[0] = BlockSizeUncompressed.Value;
				else
				{
					// figure out the segment sizes via the next
					// segment's offset after 'x' segment
					SegmentOffsets.Sort(); // loop depends on the offsets being linear

					// Figure out the last segment first. This used to be done 
					// in the for loop with a check 'x == last_element' but doing 
					// it here leaves out that boolean check and possible code 
					// jump in the result code
					SegmentSizes[last_element] = BlockSizeUncompressed.Value - SegmentOffsets[last_element];
					for (int x = last_element-1; x >= 0; x--)
						SegmentSizes[x] = SegmentOffsets[x + 1] - SegmentOffsets[x];
				}
			}

			internal int GetSegmentSize(int segment_offset)
			{
				// Offsets are aligned with Sizes so the indices are transferable
				int x = SegmentOffsets.IndexOf(segment_offset);

				if (x != -1) // make sure the size has been post-processed first
					x = SegmentSizes[x];

				return x;
			}

			byte[] PageData;
			internal byte[] GetSegmentData(CacheFileGen3 cf, cache_file_resource_layout_table owner, int segment_offset)
			{
				if (PageData == null)
				{
					#region shared cache handling
					if (SharedCache.Value != -1) return null;
					if (SharedCache.Value > -1) // above stmt disables this code for now
					{
						shared_cache_block scb = owner.SharedCaches[SharedCache.Value];
						bool is_internal;
						var scf = Program.GetManager(cf.EngineVersion).GetCacheFileFromLocation(cf.EngineVersion, scb.CachePath.Value, out is_internal) 
							as CacheFileGen3;
						if (!is_internal) cf = scf;
						// if it says its internal, the shared cache file wasn't loaded
						else throw new Debug.Exceptions.UnreachableException();
					}
					#endregion

					// If the page is valid, figure out the decompression size
					int size = BlockSizeUncompressed.Value;
					if (size <= 0) return null;

					#region Perform codec operations
					uint base_offset = (uint)(cf.Header as CacheHeaderGen3).Interop[Blam.CacheSectionType.Resource].CacheOffset;
					uint offset = base_offset + (uint)BlockOffset.Value;
					cf.InputStream.Seek(offset);
					if (CompressionCodec.Value != -1)
					{
						// TODO: we would need to test the GUID here if\when bungie adds
						// more compression codecs. Since deflation and no-codec are the 
						// only things they do use, we can just assume for now.
						using (var deflate = new System.IO.Compression.DeflateStream(cf.InputStream.BaseStream, System.IO.Compression.CompressionMode.Decompress, true))
						{
							PageData = new byte[size];
							deflate.Read(PageData, 0, PageData.Length);
						}
					}
					// No codec used, plain data
					else PageData = cf.InputStream.ReadBytes(size);
					#endregion
				}

				int segment_size = GetSegmentSize(segment_offset);
				if (segment_size == -1) return null; // offset was either invalid or sizes haven't been post-processed

				byte[] segment_data = new byte[segment_size];
				// Extract the segment data from the page
				Array.Copy(PageData, segment_offset, segment_data, 0, segment_data.Length);

				return segment_data;
			}
			#endregion
		};
		#endregion

		#region page_segment_block
		partial class page_segment_block
		{
			#region Page and Segment loading
			/// <summary>
			/// Update the page with this segment's offsets
			/// </summary>
			/// <param name="pages"></param>
			internal void UpdatePageData(TI.Block<pages_block> pages)
			{
				if(RequiredPageIndex.Value > 0)		pages[RequiredPageIndex.Value].AddSegment(RequiredSegmentOffset.Value);
				if(OptionalSegmentOffset.Value > 0)	pages[OptionalSegmentOffset.Value].AddSegment(OptionalSegmentOffset.Value);
			}
			/// <summary>
			/// Update this segment with the size the page calculated it to be
			/// </summary>
			/// <param name="pages"></param>
			internal void UpdateSegmentData(TI.Block<pages_block> pages)
			{
				if(RequiredPageIndex.Value > 0)		requiredSize = pages[RequiredPageIndex.Value].GetSegmentSize(RequiredSegmentOffset.Value);
				if(OptionalSegmentOffset.Value > 0)	optionalSize = pages[OptionalSegmentOffset.Value].GetSegmentSize(OptionalSegmentOffset.Value);
			}

			int requiredSize = -1;
			public int GetRequiredSize() { return requiredSize; }

			int optionalSize = -1;
			public int GetOptionalSize() { return optionalSize; }
			#endregion
		};
		#endregion

		#region Page and Segment loading
		bool interop_data_built = false;
		internal void BuildInteropData() // build the data we need for interoping with the cache resources in blamlib
		{
			if (interop_data_built) return;

			foreach (page_segment_block segmentation in PageSegments) // link the segments to their pages first
				segmentation.UpdatePageData(Pages);
			foreach (pages_block page in Pages) // post-process for segment sizes
				page.CalculateSegmentSizes();
			foreach (page_segment_block segmentation in PageSegments) // poll the segment sizes
				segmentation.UpdateSegmentData(Pages);
			interop_data_built = true;
		}
		#endregion
	};
	#endregion

	enum resource_fixup_type
	{
		Invalid,

		Data,	// fixup is to data located in the 'resource definition' tag data
		CacheRequired,	// fixup is to data located in the cache file resource page
		CacheOptional,

		Count,
	};

	#region cache_file_resource_gestalt
	partial class cache_file_resource_gestalt_group
	{
		#region Page and Segment loading
		bool resource_index_table_built = false;
		int resource_index_render_geometry_api_resource_definition = -1,
			resource_index_bitmap_texture_interop_resource = -1,
			resource_index_model_animation_tag_resource = -1,
			resource_index_sound_resource_definition = -1,
			resource_index_bitmap_texture_interleaved_interop_resource = -1,
			resource_index_structure_bsp_tag_resources = -1,
			resource_index_structure_bsp_cache_file_tag_resources;
		int resource_index_s_tag_d3d_vertex_buffer = -1,
			resource_index_s_tag_d3d_index_buffer = -1,
			resource_index_s_tag_d3d_texture = -1,
			resource_index_s_tag_d3d_texture_interleaved = -1;
		internal void BuildInteropData() // build the data we need for interoping with the cache resources in blamlib
		{
			if (resource_index_table_built) return;

			resource_type_block.BuildResourceIndices(this);
			resource_structure_type_block.BuildResourceIndices(this);
			resource_index_table_built = true;
		}
		#endregion


		// new to HaloOdst:
		// structure_bsp_cache_file_tag_resources
		#region resource_type_block
		partial class resource_type_block
		{
			internal static void BuildResourceIndices(cache_file_resource_gestalt_group owner)
			{
				int x = 0;
				foreach(resource_type_block def in owner.ResourceTypes)
				{
					string name = def.Name.ToString();

					switch (name)
					{
						case "render_geometry_api_resource_definition":
							owner.resource_index_render_geometry_api_resource_definition = x;
							break;
						case "bitmap_texture_interop_resource":
							owner.resource_index_bitmap_texture_interop_resource = x;
							break;
						case "model_animation_tag_resource":
							owner.resource_index_model_animation_tag_resource = x;
							break;
						case "sound_resource_definition":
							owner.resource_index_sound_resource_definition = x;
							break;
						case "bitmap_texture_interleaved_interop_resource":
							owner.resource_index_bitmap_texture_interleaved_interop_resource = x;
							break;
						case "structure_bsp_tag_resources":
							owner.resource_index_structure_bsp_tag_resources = x;
							break;
						case "structure_bsp_cache_file_tag_resources":
							owner.resource_index_structure_bsp_cache_file_tag_resources = x;
							break;
						default: throw new Debug.Exceptions.UnreachableException(name);
					}
					x++;
				}
			}
		};
		#endregion

		#region resource_structure_type_block
		partial class resource_structure_type_block
		{
			internal static void BuildResourceIndices(cache_file_resource_gestalt_group owner)
			{
				int x = 0;
				foreach (resource_structure_type_block def in owner.ResourceStructureTypes)
				{
					string name = def.Name.ToString();

					switch (name)
					{
						case "s_tag_d3d_vertex_buffer":
							owner.resource_index_s_tag_d3d_vertex_buffer = x;
							break;
						case "s_tag_d3d_index_buffer":
							owner.resource_index_s_tag_d3d_index_buffer = x;
							break;
						case "s_tag_d3d_texture":
							owner.resource_index_s_tag_d3d_texture = x;
							break;
						case "s_tag_d3d_texture_interleaved":
							owner.resource_index_s_tag_d3d_texture_interleaved = x;
							break;
						default: throw new Debug.Exceptions.UnreachableException(name);
					}
					x++;
				}
			}
		};
		#endregion

		#region cache_file_resource_gestalt_tag_resource_block
		partial class cache_file_resource_gestalt_tag_resource_block
		{
			#region Page and Segment loading
			class resource_tag_stream : IO.ITagStream
			{
				CacheFileGen3 cache;
				DatumIndex tag_datum;
				public resource_tag_stream(CacheFileGen3 c, DatumIndex tag, IO.EndianReader s)
				{ cache = c; tag_datum = tag; InputStream = s; }

				public DatumIndex OwnerId		{ get { return cache.TagIndexManager.IndexId; } }
				Util.Flags flags = new Util.Flags(0);
				public Util.Flags Flags			{ get { return flags; } }
				public DatumIndex TagIndex		{ get { return DatumIndex.Null; } }
				public DatumIndex ReferenceName { get { return cache.Index.Tags[tag_datum.Index].ReferenceName; } }
				public BlamVersion Engine		{ get { return cache.EngineVersion; } }

				IO.EndianReader InputStream;
				public BlamLib.IO.EndianReader GetInputStream()	{ return InputStream; }
				public BlamLib.IO.EndianWriter GetOutputStream(){ throw new NotImplementedException(); }

				public string GetExceptionDescription()
				{
					return string.Format("cache resource stream: {0}.{1}", cache.References[ReferenceName], cache.Index.Tags[tag_datum.Index].GroupTag.Name);
				}
			};

			TI.Definition loadedResources = null;
			internal TI.Definition LoadResources(CacheFileGen3 c, cache_file_resource_gestalt_group owner, cache_file_resource_layout_table cache_layout, bool mega_hack)
			{
				if (Reference.Datum == DatumIndex.Null) return null; // this is a null entry
				if (loadedResources != null) return loadedResources; // data already loaded, return

				var rdf = c.GetCacheFileResourceDefinitionFactory();

				// pre-process cache for resource loading
				cache_layout.BuildInteropData();
				owner.BuildInteropData();

				int resource_stream_definition_size = BlockSize.Value;
				
				// sound resource case hack
				bool use_sound_resource_hack = false;
				if (resource_stream_definition_size == 0)
				{
					Debug.Assert.If(ResourceType.Value == owner.resource_index_sound_resource_definition);
					resource_stream_definition_size = sound_resource_definition.kSizeOf;
					use_sound_resource_hack = true;
				}

				int resource_stream_size_required = resource_stream_definition_size;
				// base address to use on cache fixups, cache data will be appended 
				// later on
				uint cache_required_base_address = 0, cache_optional_base_address = 0;
				byte[] resource_stream_data;

				#region create resource buffer
				{// Get our page segment data so we can build our resource buffer for loading
					int seg_index = SegmentIndex.Value;
					cache_file_resource_layout_table.page_segment_block page_segment = cache_layout.PageSegments[seg_index];
					int required_size = page_segment.GetRequiredSize();
					int optional_size = page_segment.GetOptionalSize();

					cache_required_base_address = (uint)resource_stream_size_required;
					if (required_size > 0) resource_stream_size_required += required_size;

					if (optional_size > 0) // optional isn't always used so don't set the base address if it isn't
					{
						cache_optional_base_address = (uint)resource_stream_size_required;
						resource_stream_size_required += optional_size;
					}

					// get our definition data buffer
					resource_stream_data = new byte[resource_stream_size_required];
					if (use_sound_resource_hack) // sound_resources don't occupy space in the resource-definition-data, so we have to create faux def data
					{
						int data_size = 0;
						if (required_size > 0) data_size += required_size;
						if (optional_size > 0) data_size += optional_size;
						if (data_size > 0) rdf.InsertDataSizeIntoFauxDefinitionData(resource_stream_data, (uint)data_size);
					}
					else
						Array.Copy(owner.ResourceDefinitionData.Value, BlockOffset.Value, resource_stream_data, 0, resource_stream_definition_size);

					{ // get cache data and append it
						byte[] page_data = 
							cache_layout.Pages[page_segment.RequiredPageIndex.Value].GetSegmentData(c, cache_layout, page_segment.RequiredSegmentOffset.Value);

						Array.Copy(page_data, 0, resource_stream_data, (int)cache_required_base_address, required_size);

						if (page_segment.OptionalPageIndex.Value >=0 && cache_optional_base_address > 0)
						{
							page_data = cache_layout.Pages[page_segment.OptionalPageIndex.Value].GetSegmentData(c, cache_layout, page_segment.OptionalSegmentOffset.Value);
							Array.Copy(page_data, 0, resource_stream_data, (int)cache_optional_base_address, optional_size);
						}
					}
				}
				#endregion

				Util.OptionalValue optv = new Util.OptionalValue((byte)resource_fixup_type.Count);

				#region perform fixups
				using (var ms = new System.IO.MemoryStream(resource_stream_data, true))
				{
					foreach (resource_fixup_block def in ResourceFixups)
					{
						uint address = (uint)def.Address.Value;
						uint address_offset = optv.GetValue(address);
						ms.Seek(def.BlockOffset.Value, System.IO.SeekOrigin.Begin);
						resource_fixup_type rft = (resource_fixup_type)optv.GetOption(address);

						switch (rft)
						{
							case resource_fixup_type.Data: break;
							case resource_fixup_type.CacheRequired: address_offset += cache_required_base_address; break;
							case resource_fixup_type.CacheOptional: address_offset += cache_optional_base_address; break;

							default: throw new Debug.Exceptions.UnreachableException();
						}

						IO.ByteSwap.SwapUDWordAndWrite(address_offset, ms);
						
						// hack identifier for StructReference fields when the definition is at offset '0' 
						// as that fucks with the init code
						//if (address_offset == 0) IO.ByteSwap.SwapUDWordAndWrite(1, ms);
					}

//					foreach (resource_definition_fixup_block def in ResourceDefinitionFixups)
//					{
//					}
				}
				#endregion

				#region create and stream definition
				using (var s = new IO.EndianReader(resource_stream_data, IO.EndianState.Big, null))
				{
					int res_type = ResourceType.Value;
					if (res_type == owner.resource_index_render_geometry_api_resource_definition)			loadedResources = rdf.GenerateRenderGeometryApiResource();
					else if (res_type == owner.resource_index_bitmap_texture_interop_resource)				loadedResources = rdf.GenerateBitmapTextureInteropResource();
					else if (res_type == owner.resource_index_model_animation_tag_resource)					loadedResources = rdf.GenerateModelAnimationTagResource();
					// TODO: haven't quite figured this one out yet. Currently using hacked up code (see [use_sound_resource_hack])
					else if (res_type == owner.resource_index_sound_resource_definition)					loadedResources = rdf.GenerateSoundResourceResource();
					else if (res_type == owner.resource_index_bitmap_texture_interleaved_interop_resource)	loadedResources = rdf.GenerateBitmapTextureInterleavedInteropResource();
					else if (res_type == owner.resource_index_structure_bsp_tag_resources)					loadedResources = rdf.GenerateStructureBspTagResource();
					// TODO: haven't figured this one out yet
					else if (res_type == owner.resource_index_structure_bsp_cache_file_tag_resources)		loadedResources = rdf.GenerateStructureBspCacheFileTagResource();
					else throw new Debug.Exceptions.UnreachableException();

					s.Seek(optv.GetValue((uint)DefinitionOffset.Value));
					IO.ITagStream ts = new resource_tag_stream(c, Reference.Datum, s);

					ts.Flags.Add(
						IO.ITagStreamFlags.UseStreamPositions |
						IO.ITagStreamFlags.DontStreamStringData |
						IO.ITagStreamFlags.DontStreamFieldSetHeader |
						IO.ITagStreamFlags.DontPostprocess);
					if (mega_hack) ts.Flags.Add(IO.ITagStreamFlags.Halo3VertexBufferMegaHack);
					loadedResources.Read(ts);
				}
				#endregion

				return loadedResources;
			}
			#endregion
		};

		#region Page and Segment loading
		public TI.Definition LoadResources(DatumIndex resource, CacheFileGen3 c, cache_file_resource_layout_table cache_layout)
		{
			return LoadResources(resource, c, cache_layout, false);
		}
		public TI.Definition LoadResources(DatumIndex resource, CacheFileGen3 c, cache_file_resource_layout_table cache_layout, bool mega_hack)
		{
			if (resource == DatumIndex.Null || c == null || cache_layout == null ||
				resource.Index < 0 || resource.Index > TagResources.Count)
				return null;

			return TagResources[resource.Index].LoadResources(c, this, cache_layout, mega_hack);
		}
		#endregion
		#endregion
	};
	#endregion
}