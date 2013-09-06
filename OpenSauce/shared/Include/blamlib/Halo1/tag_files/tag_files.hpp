/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace blam
	{
		bool PLATFORM_API tag_file_read(int32 file_position, size_t buffer_size, void *buffer);

		// Is the tag file read only?
		bool PLATFORM_API tag_file_read_only(tag group_tag, cstring name);
		// Does the tag file exist?
		bool PLATFORM_API tag_file_exists(tag group_tag, cstring name);
	};
};