/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once
#include <YeloLib/files/files.hpp>

#define FILE_IO_DO_MD5_CHECK

namespace Yelo
{
	namespace FileIO
	{
		void Initialize();
		void Dispose();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Opens a file by it's identifier. </summary>
		///
		/// <remarks>
		/// 	ID files can only be readonly, otherwise the md5 would change when the file is written
		/// 	to. If the file id definition has an MD5 checksum the file is memory mapped and it's
		/// 	checksum compared. If the checksums do not match, an error is returned, but the file
		/// 	remains open for reading.
		/// </remarks>
		///
		/// <param name="info_out">	[out] The file information output. </param>
		/// <param name="file_id"> 	Identifier for the file. </param>
		///
		/// <returns>	An Enums::file_io_open_error. </returns>
		Enums::file_io_open_error OpenFileByID(s_file_info& info_out, cstring file_id);
	};
};