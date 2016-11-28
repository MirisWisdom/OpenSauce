/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/memory/compression/zip_codec.hpp>

#ifndef API_NO_ZIP_CODEC
#include <zlib/contrib/minizip/unzip.h>

namespace Yelo
{
	namespace Compression { namespace Zip
	{
		bool OpenZipFile(const char* archive_file, unzFile* output_zip_file)
		{
			YELO_ASSERT_DISPLAY(output_zip_file, "no zip file struct passed to OpenZipFile");

			*output_zip_file = unzOpen64(archive_file);

			if(!*output_zip_file)
				return false;

			return true;
		}

		void CloseZipFile(unzFile zip_file)
		{
			unzClose(zip_file);
		}

		bool DecompressCurrentFile(unzFile zip_file,
			byte*& uncompressed_data,
			size_t& uncompressed_data_size)
		{
			char output_filename[MAX_PATH] = "";

			// get the file name from the archive
			unz_file_info64 file_info;
			if(unzGetCurrentFileInfo64(zip_file, &file_info, output_filename, MAX_PATH, nullptr, 0, nullptr, 0) != UNZ_OK)
				return false;

			// open the file for reading
			if(unzOpenCurrentFilePassword(zip_file, nullptr) != UNZ_OK)
				return false;

			// allocate memory to read the file in to
			uncompressed_data = new byte[(uint32)file_info.uncompressed_size];
			if(!uncompressed_data)
			{
				unzCloseCurrentFile(zip_file);
				return false;
			}

			// read the compressed file into the allocated memory
			int error = unzReadCurrentFile(zip_file, uncompressed_data, (uint32)file_info.uncompressed_size);
			unzCloseCurrentFile(zip_file);

			if(error != file_info.uncompressed_size)
			{
				// decompression failed, delete the allocated memory
				delete [] uncompressed_data;
				uncompressed_data = nullptr;

				uncompressed_data_size = 0;
				return false;
			}

			uncompressed_data_size = (uint32)file_info.uncompressed_size;
			return true;
		}

		bool DecompressFileEntry(unzFile zip_file,
			cstring file,
			byte*& uncompressed_data,
			size_t& uncompressed_data_size)
		{
			// find the requested file in the archive
			if(unzLocateFile(zip_file, file, 0) != UNZ_OK)
				return false;

			if(!DecompressCurrentFile(zip_file, uncompressed_data, uncompressed_data_size))
				return false;
			else
				return true;
		}
	}; };
};
#endif