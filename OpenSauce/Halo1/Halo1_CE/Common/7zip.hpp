/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if !PLATFORM_IS_DEDI

namespace Yelo
{
	namespace Compression { namespace SevenZip
	{
		bool DecompressFileEntry(const char* archive_file,
			const char* file_name,
			byte*& uncompressed_data,
			size_t& uncompressed_data_size);
	};};
};

#endif