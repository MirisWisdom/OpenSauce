/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
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

#if !PLATFORM_IS_EDITOR
#include "Common/FileIO.hpp"
#endif

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
		FileIO::s_file_info	file_info;
		union {
			void*		m_address;
			s_header*	m_header;

			// utility member for pointer math
			uint32		m_base_address;
		};
		bool			m_file_mapped;
		PAD24;
#else
	public:
		struct s_element_editor : s_element
		{
			char* source_id;
			char* source_data;

			s_element_editor() : source_id(NULL), source_data(NULL) {}
			void Delete()
			{
				delete [] source_id;
				delete [] source_data;
				source_id = NULL;
				source_data = NULL;
			}
		};
	private:

		s_header						m_header;
		std::vector<s_element_editor>	m_elements;
#endif

	public:
#if !PLATFORM_IS_EDITOR
		// Opens a packed file and memory maps it, read for accessing by pointer
		void OpenFile(const char* packed_file, bool is_file_id = false);
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
		c_packed_file();
		~c_packed_file();

		void CalculateOffsets();
		HRESULT Save(cstring save_location);
		void AddElement(s_element_editor& element);
#endif
	};
};