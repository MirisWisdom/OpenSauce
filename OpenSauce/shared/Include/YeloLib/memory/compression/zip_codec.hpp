/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#ifndef API_NO_ZIP_CODEC
typedef void* unzFile;

namespace Yelo
{
	namespace Compression { namespace Zip
	{
		bool OpenZipFile(const char* archive_file, unzFile* output_zip_file);

		void CloseZipFile(unzFile zip_file);

		bool DecompressCurrentFile(unzFile zip_file,
			byte*& uncompressed_data,
			size_t& uncompressed_data_size);

		bool DecompressFileEntry(unzFile zip_file,
			cstring file,
			byte*& uncompressed_data,
			size_t& uncompressed_data_size);
	}; };
};
#endif