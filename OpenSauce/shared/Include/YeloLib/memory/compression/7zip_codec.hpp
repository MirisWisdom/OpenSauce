/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#ifndef API_NO_7ZIP_CODEC
namespace Yelo
{
	namespace Compression { namespace SevenZip
	{
		bool DecompressFileEntry(cstring archive_file,
			cstring file_name,
			byte*& uncompressed_data,
			size_t& uncompressed_data_size);
	}; };
};
#endif