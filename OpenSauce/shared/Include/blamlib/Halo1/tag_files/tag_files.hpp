/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <blamlib/memory/data_base.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		struct s_tag_header : Memory::s_datum_base
		{
			enum { k_signature = 'blam' };

			byte_flags flags;	// UNUSED in Halo
			byte type;			// UNUSED in Halo

			tag_string name;	// UNUSED in Halo

			tag group_tag;
			uint32 crc;
			int32 offset;		// actual offset in the file
			int32 size;			// UNUSED in Halo

			uint32 user_data;	// UNUSED in Halo
			int16 version;

			sbyte foundation_tag_file_index;	// UNUSED in Halo
			sbyte owner_index;	// UNUSED in Halo. default value is NONE

			tag signature;
		}; BOOST_STATIC_ASSERT( sizeof(s_tag_header) == 0x40 );
	};

	namespace blam
	{
		void PLATFORM_API tag_files_flush();


		bool PLATFORM_API tag_file_open(tag group_tag, cstring filename, 
			bool* out_is_readonly, uint32* out_crc, bool verify_exist_first);

		bool PLATFORM_API tag_file_read(int32 file_position, size_t buffer_size, void* buffer);

		// Is the tag file read only?
		bool PLATFORM_API tag_file_read_only(tag group_tag, cstring name);
		// Does the tag file exist?
		bool PLATFORM_API tag_file_exists(tag group_tag, cstring name);
	};
};