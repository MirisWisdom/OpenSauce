/*
    Yelo: Open Sauce SDK

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

/*
	Packed File Object
		TheFieryScythe/FireScythe	23-02-2010 (DD-MM-YYYY)	
	Provides a generic file structure that simplifies storing component
	specific data. Memory mapping is used to open the file and provide
	pointers to each data element. Usertool contains the code for 
	creating data files that use this structure.
*/
#pragma once

namespace Yelo
{
	class c_packed_file
	{
		struct s_header
		{
			enum {
				k_header_signature = 'head',
				k_footer_signature = 'foot',
			};

			uint32 header;
			uint32 file_size;
			uint32 element_count;
			uint32 footer;

			inline bool IsValid()
			{
				return header == k_header_signature &&
					footer == k_footer_signature;
			}

			inline void Ctor()
			{
				header = k_header_signature;
				file_size = element_count = 0;
				footer = k_footer_signature;
			}
		};

		struct s_element
		{
			uint32 element_id_offset;
			uint32 element_size;
			uint32 element_offset;
		};

#if !PLATFORM_IS_EDITOR
		HANDLE			m_file_handle;
		HANDLE			m_mapping_handle;
		union {
			void*		m_address;
			s_header*	m_header;

			// utility member for pointer math
			uint32		m_base_address;
		};
		bool			m_file_mapped;
		PAD24;
#else
		struct s_element_editor : s_element
		{
			char* source_id;
			void* source_data;
		};

		s_header			m_header;
		s_element_editor*	m_elements;
#endif

	public:
#if !PLATFORM_IS_EDITOR
		c_packed_file() : 
			m_file_handle(INVALID_HANDLE_VALUE), m_mapping_handle(INVALID_HANDLE_VALUE) {}

		// Opens a packed file and memory maps it, read for accessing by pointer
		void OpenFile(const char* packed_file);
		// Releases the mapped file
		void CloseFile();
		// Returns a pointer to a block of data reference by index. Returns NULL if invalid.
		// The data block size is put into data_size if not null.
		void* GetDataPointer(uint32 index, _Out_opt_ uint32* data_size);
		// Returns a pointer to a block of data reference by a string id. Returns NULL if invalid.
		// The data block size is put into data_size if not null.
		void* GetDataPointer(const char* data_id, _Out_opt_ uint32* data_size);
		// Returns whether the file has been successfully mapped
		API_INLINE bool IsFileMapped() const { return m_file_mapped; }

#else
		c_packed_file(uint32 element_count);
		~c_packed_file();

		void CalculateOffsets();
		HRESULT Save(cstring save_location);
#endif
	};
};