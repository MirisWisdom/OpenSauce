/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#define TAG_ADDRESS(orig, curr, pointer) curr + (pointer - orig)

/*!
 * \brief
 * Calculates a map cache's crc for data already loaded in to memory.
 * 
 * \param cache_file
 * Pointer to the cache data.
 * 
 * \returns
 * The cache's CRC.
 * 
 * Calculates a map cache's crc for data already loaded in to memory.
 */
uint32 CalculateCRC(void* cache_file)
{
	uint32 CRC = 0xFFFFFFFF;

	// get pointers to the necessary cache data
	s_cache_header* cache = CAST_PTR(s_cache_header*, cache_file);
	s_cache_tag_header* tag_header = CAST_PTR(s_cache_tag_header*, (uint32)cache + cache->offset_to_index);
	s_cache_tag_instance* tag_instances = CAST_PTR(s_cache_tag_instance*, (uint32)tag_header + sizeof(s_cache_tag_header));

	// the tag address needs correcting to match the datas starting point
	uint32 orig = (uint32)tag_header->tags_address;
	uint32 curr = (uint32)tag_instances;

	// get the scenario tag
	s_cache_tag_instance& scenario_tag_instance = tag_instances[tag_header->scenario.index];
	TagGroups::scenario* scenario_tag = CAST_PTR(TagGroups::scenario*, TAG_ADDRESS(orig, curr, (uint32)scenario_tag_instance.definition));

	uint32 bsp_count = scenario_tag->structure_bsps.Count;
	if(bsp_count != 0)
	{
		// can't access the block normally as its pointer is not correct for the cache's starting point
		TagGroups::scenario_structure_bsps* structure_bsps_block = CAST_PTR(TagGroups::scenario_structure_bsps*,
			TAG_ADDRESS(orig, curr, (uint32)scenario_tag->structure_bsps.Address));

		// this isn't correct for non-PC maps apparently, but we ARE on a pc...AREN'T we
		uint32 bsp_data_start = (uint32)cache + sizeof(s_cache_header);

		// crc the data for all bsp's
		uint32 bsp_data_offset = 0;
		for(uint32 i = 0; i < bsp_count; i++)
		{
			int32 bsp_data_size = structure_bsps_block[i].bsp_data_size;
			Memory::CRC(CRC, CAST_PTR(void*, bsp_data_start + bsp_data_offset), bsp_data_size);
			bsp_data_offset += bsp_data_size;
		}
	}

	// crc raw data
	Memory::CRC(CRC, CAST_PTR(void*, (uint32)cache + tag_header->vertices.offset), tag_header->model_data_size);

	// crc tag data
	Memory::CRC(CRC, CAST_PTR(void*, tag_header), cache->tag_memory_size);

	return CRC;
}

/*!
 * \brief
 * Calculates a map cache's crc.
 * 
 * \param cache_file_path
 * String pointer containing the maps name
 * 
 * \returns
 * The cache's CRC, if a failure occured it will return 0xFFFFFFFF
 * 
 * Calculates a map cache's crc.
 */
uint32 CalculateCRC(const char* cache_file_path)
{
	char map_path[MAX_PATH];

	uint32 map_crc = 0xFFFFFFFF;

	if(!FileIO::PathBuild(map_path, false, 2, "maps", cache_file_path))
		return map_crc;

	FileIO::s_file_info map_file;
	//do-while-false for simpler cleanup
	do
	{
		if(Enums::_file_io_open_error_none != FileIO::OpenFile(map_file, map_path))
			break;

		if(Enums::_file_io_read_error_none != FileIO::MemoryMapFile(map_file))
			break;

		map_crc = CalculateCRC(map_file.data_pointer);
	}while(false);

	FileIO::CloseFile(map_file);

	return map_crc;

}
#undef TAG_ADDRESS