/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/PackedFile/Processing/c_packed_file_processor_archive.hpp"

#if PLATFORM_TYPE == PLATFORM_TOOL
#include <YeloLib/files/files.hpp>

namespace Yelo
{
	namespace Tool { namespace PackedFile
	{
		bool c_packed_file_processor_archive::ProcessElement(c_packed_file::s_element_editor& element, const boost::filesystem::path& file_path)
		{
			FileIO::s_file_info file;

			// Open the file
			auto open_result = FileIO::OpenFile(file
				, file_path.string().c_str()
				, Enums::_file_io_open_access_type_read
				, Enums::_file_io_open_create_option_open_existing);
			if(Enums::_file_io_open_error_none != open_result)
			{
				return false; // failed to open file
			}

			// read the file into the element
			element.element_size = file.file_size;
			
			auto read_result = FileIO::ReadFileToMemory(file, CAST_PTR(void**, &element.source_data), 0, 0);
			if(Enums::_file_io_read_error_none != read_result)
			{
				FileIO::CloseFile(file);
				element.element_size = 0;
				return false;
			}
			FileIO::CloseFile(file);
			return true;
		}
	};};
};
#endif