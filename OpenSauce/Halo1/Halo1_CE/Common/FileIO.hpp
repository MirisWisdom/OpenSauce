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
		struct s_id_file_definition
		{
			struct{
				bool do_md5_check;
				bool file_exists;
				PAD16;
			}m_flags;

			char* m_id;
			char m_md5[33];
			PAD24;
			char* m_location;
		};

		struct s_file_io_globals
		{
			uint32 m_id_file_count;
			s_id_file_definition* m_id_file_definitions;
		};

		void Initialize();
		void Dispose();

		Enums::file_io_open_error OpenFileByID(s_file_info& info_out, cstring file_id);
	};
};