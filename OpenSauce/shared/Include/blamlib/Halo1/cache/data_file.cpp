/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/cache/data_file.hpp>

namespace Yelo
{
	namespace Cache
	{
		void s_data_file::DeleteForCopy(cstring file)
		{
			BOOL delete_result = DeleteFile(file);
			if(delete_result == ERROR_ACCESS_DENIED)
				printf_s("Could not delete %s for copy\n", file);
		}

		void s_data_file::PreprocessForSave()
		{
			SetFilePointer(this->file_handle, this->header.file_names_offset, NULL, FILE_BEGIN);
			SetEndOfFile(this->file_handle);

			// data_file_open does this before it returns, so do the same
			SetFilePointer(this->file_handle, this->header.file_names_offset, NULL, FILE_BEGIN);
		}

		void s_data_file::Save()
		{
			SetFilePointer(this->file_handle, 0, NULL, FILE_END);

			this->header.file_names_offset = SetFilePointer(this->file_handle, 0, NULL, FILE_CURRENT);
			DWORD bytes_written;
			// Write the file names
			WriteFile(this->file_handle, 
				this->file_names.address, this->file_names.used_size, 
				&bytes_written, NULL);

			// Write the file index table
			this->header.file_index_table_offset = SetFilePointer(this->file_handle, 0, NULL, FILE_CURRENT);
			WriteFile(this->file_handle, 
				this->file_index_table.address, this->file_index_table.count * sizeof(s_data_file_item), 
				&bytes_written, NULL);


			// Go back to the start and write the new header
			SetFilePointer(this->file_handle, 0, NULL, FILE_BEGIN);

			WriteFile(this->file_handle, 
				&this->header, sizeof(this->header), 
				&bytes_written, NULL);
		}
	};
};