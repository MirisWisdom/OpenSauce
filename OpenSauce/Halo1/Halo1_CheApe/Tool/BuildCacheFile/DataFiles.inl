/*
    Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
enum data_file_type {
	_data_file_type_bitmaps,
	_data_file_type_sounds,
	_data_file_type_locale,

	_data_file_type,
};

struct s_data_file_header
{
	int32 type;
	int32 file_names_offset;
	int32 file_index_table_offset;
	int32 tag_count;
}; BOOST_STATIC_ASSERT( sizeof(s_data_file_header) == 0x10 );

struct s_data_file_item
{
	int32 name_offset;
	int32 size;
	int32 data_offset;
}; BOOST_STATIC_ASSERT( sizeof(s_data_file_item) == 0xC );

struct s_data_file
{
	s_data_file_header header;
	struct {
		s_data_file_item* address;
		int32 count;
	}file_index_table;
	struct {
		int32 total_size;
		int32 used_size;
		char* address;
	}file_names;
	bool writable;
	PAD24;
	struct {
		int32 count;
		int32 size;
	}unreferenced_items, referenced_items;
	cstring name;
	HANDLE file_handle;

	static void DeleteForCopy(cstring file)
	{
		BOOL delete_result = DeleteFile(file);
		if(delete_result == ERROR_ACCESS_DENIED)
			printf_s("Could not delete %s for copy\n", file);
	}

	void PreprocessForSave()
	{
		SetFilePointer(this->file_handle, this->header.file_names_offset, NULL, FILE_BEGIN);
		SetEndOfFile(this->file_handle);

		// data_file_open does this before it returns, so do the same
		SetFilePointer(this->file_handle, this->header.file_names_offset, NULL, FILE_BEGIN);
	}

	void Save()
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
}; BOOST_STATIC_ASSERT( sizeof(s_data_file) == 0x40 );


class c_data_files
{
public:
	char m_names[_data_file_type][64];

public:
	// Initialize the data file system to either use a mod-set or the stock 
	// data files.
	// NOTE: [maps_path] will be modified if [using_mod_sets] is true!
	void InitializeForCache(bool using_mod_sets, cstring mod_name, char maps_path[MAX_PATH])
	{
		memset(m_names, 0, sizeof(m_names));

		if(using_mod_sets)
		{
			strcat_s(maps_path, MAX_PATH, "data_files\\");
			_mkdir(maps_path);

			for(int32 x = 0; x < NUMBEROF(m_names); x++)
			{
				strcpy_s(m_names[x], "data_files\\");
				strcat_s(m_names[x], mod_name);
			}
			strcat_s(m_names[_data_file_type_bitmaps],	"-bitmaps");
			strcat_s(m_names[_data_file_type_sounds],	"-sounds");
			strcat_s(m_names[_data_file_type_locale],	"-loc");
		}
		else
		{
			strcat_s(m_names[_data_file_type_bitmaps],	"bitmaps");
			strcat_s(m_names[_data_file_type_sounds],	"sounds");
			strcat_s(m_names[_data_file_type_locale],	"loc");
		}
	}

	// Copy the stock data files that come with the game. These copies will then be used as 
	// the input data files for the cache being built
	void CopyStock()
	{
		#pragma region s_progress_report
		struct s_progress_report {
			bool stage_completed[100/25];
			BOOL cancel;

			static DWORD CALLBACK CopyProgressRoutine(
				LARGE_INTEGER TotalFileSize,	LARGE_INTEGER TotalBytesTransferred,
				LARGE_INTEGER StreamSize,		LARGE_INTEGER StreamBytesTransferred,
				DWORD dwStreamNumber,
				DWORD dwCallbackReason,
				HANDLE hSourceFile,	HANDLE hDestinationFile,
				LPVOID lpData)
			{
				s_progress_report* report = CAST_PTR(s_progress_report*, lpData);

				if(dwCallbackReason == CALLBACK_CHUNK_FINISHED)
				{
					LONGLONG percent = (LONGLONG)(((double)TotalBytesTransferred.QuadPart / (double)TotalFileSize.QuadPart)*100.0);

					if(percent >= 25 && !report->stage_completed[0])
					{
						printf_s("\t25%%...\n");
						report->stage_completed[0] = true;
					}
					else if(percent >= 50 && !report->stage_completed[1])
					{
						printf_s("\t50%%...\n");
						report->stage_completed[1] = true;
					}
					else if(percent >= 75 && percent < 100 && !report->stage_completed[2])
					{
						printf_s("\t75%%...\n");
						report->stage_completed[2] = true;
					}
				}

				return PROGRESS_CONTINUE;
			}
		};
		#pragma endregion

		s_progress_report report;
		char target_name[64];
		BOOL failure = false;


		printf_s("copying bitmaps...\n");
		sprintf_s(target_name, "maps\\%s.map", m_names[_data_file_type_bitmaps]);
		s_data_file::DeleteForCopy(target_name);
		{
			memset(&report, 0, sizeof(report));
			if( !CopyFileExA("maps\\bitmaps.map",	target_name, 
				&s_progress_report::CopyProgressRoutine, &report, &report.cancel, 0) )
			{	printf_s("failed to copy\n"); failure = true; }

			if(failure)
			{
				printf_s("reason: %X\n", GetLastError());
				return;
			}
		}

		printf_s("copying sounds...\n");
		sprintf_s(target_name, "maps\\%s.map", m_names[_data_file_type_sounds]);
		s_data_file::DeleteForCopy(target_name);
		{
			memset(&report, 0, sizeof(report));
			if( !CopyFileExA("maps\\sounds.map",	target_name, 
				&s_progress_report::CopyProgressRoutine, &report, &report.cancel, 0) )
			{	printf_s("failed to copy\n"); failure = true; }

			if(failure)
			{
				printf_s("reason: %X\n", GetLastError());
				return;
			}
		}

		printf_s("copying loc...\n");
		sprintf_s(target_name, "maps\\%s.map", m_names[_data_file_type_locale]);
		s_data_file::DeleteForCopy(target_name);
		{
			memset(&report, 0, sizeof(report));
			if( !CopyFileExA("maps\\loc.map",	target_name, 
				&s_progress_report::CopyProgressRoutine, &report, &report.cancel, 0) )
			{	printf_s("failed to copy\n"); failure = true; }

			if(failure)
			{
				printf_s("reason: %X\n", GetLastError());
				return;
			}
		}

		printf_s("done\n");
	}
};