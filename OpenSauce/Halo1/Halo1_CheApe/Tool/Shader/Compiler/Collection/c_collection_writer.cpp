/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Tool/Shader/Compiler/Collection/c_collection_writer.hpp"

#if PLATFORM_TYPE == PLATFORM_TOOL
#include <YeloLib/files/files.hpp>
#include <YeloLib/cryptography/TEA.hpp>
#include <blamlib/Halo1/cryptography/md5.hpp>

using namespace boost::filesystem;
using namespace Yelo::FileIO;

namespace Yelo
{
	namespace Tool { namespace Shader { namespace Compiler
	{
		void c_collection_writer::AppendEffectToCollection(FileIO::s_file_info& collection_file, const path& effect_file)
		{
			s_file_info source_file;
			auto open_result = OpenFile(source_file, effect_file.string().c_str());
			YELO_ASSERT_DISPLAY(open_result == Enums::_file_io_open_error_none, "ERROR: Failed to open an effect file for reading");

			auto read_result = ReadFileToInfoMemory(source_file);
			YELO_ASSERT_DISPLAY(read_result == Enums::_file_io_read_error_none, "ERROR: Failed to read an effect file into memory");

			auto write_result = WriteToFile(collection_file, CAST_PTR(char*, source_file.data_pointer), source_file.data_length);
			YELO_ASSERT_DISPLAY(write_result == Enums::_file_io_write_error_none, "ERROR: Failed to write to a collection file");

			CloseFile(source_file);
		}

		void c_collection_writer::WriteCollection(const path& source_directory, const path& collection_path, i_collection_definition& collection_definition)
		{
			s_file_info collection_file;
			auto open_result = OpenFile(collection_file, collection_path.string().c_str(),
				Enums::_file_io_open_access_type_write | Enums::_file_io_open_access_type_read,
				Enums::_file_io_open_create_option_new);

			YELO_ASSERT_DISPLAY(open_result == Enums::_file_io_open_error_none, "ERROR: Failed to open a collection file for writing");

			WriteEffects(collection_file, source_directory, collection_definition);

			// Read file in memory
			auto read_result = ReadFileToInfoMemory(collection_file);
			YELO_ASSERT_DISPLAY(read_result == Enums::_file_io_read_error_none, "ERROR: Failed to read a collection file into memory");

			// Generate MD5 checksum
			char md5_checksum[33] = "";
			blam::generate_md5(CAST_PTR(char*, collection_file.data_pointer), collection_file.data_length, md5_checksum);

			// Append checksum string to the file
			auto write_result = WriteToFile(collection_file, md5_checksum, sizeof(md5_checksum));
			YELO_ASSERT_DISPLAY(write_result == Enums::_file_io_write_error_none, "ERROR: Failed to write to a collection file");

			// Read the file back into memory
			read_result = ReadFileToInfoMemory(collection_file);
			YELO_ASSERT_DISPLAY(read_result == Enums::_file_io_read_error_none, "ERROR: Failed to read a collection file into memory");

			// encode the data
			Cryptography::TEA::Encode(CAST_PTR(char*, collection_file.data_pointer), collection_file.data_length);

			// write back out to file
			write_result = WriteToFile(collection_file, CAST_PTR(char*, collection_file.data_pointer), collection_file.data_length, 0);
			YELO_ASSERT_DISPLAY(write_result == Enums::_file_io_write_error_none, "ERROR: Failed to write to a collection file");

			CloseFile(collection_file);
		}
	};};};
};
#endif