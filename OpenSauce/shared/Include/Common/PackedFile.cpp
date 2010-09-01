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
#include <Common/Precompile.hpp>
#include <Common/PackedFile.hpp>

#if PLATFORM_IS_EDITOR
//	#include <boost/filesystem.hpp>
	#include <fstream>
	#include "Engine/EngineFunctions.hpp"
#endif

namespace Yelo
{
#if !PLATFORM_IS_EDITOR
	void c_packed_file::OpenFile(const char* packed_file)
	{
		m_file_handle = CreateFile(packed_file, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(m_file_handle == INVALID_HANDLE_VALUE) return;

		m_mapping_handle = CreateFileMapping(m_file_handle, NULL, PAGE_READONLY, 0, 0, NULL);
		if(m_mapping_handle == INVALID_HANDLE_VALUE) return;

		m_address = MapViewOfFile(m_mapping_handle, FILE_MAP_READ, 0, 0, 0);

		if(m_header != NULL || m_header->IsValid())
			m_file_mapped = true;
	}

	void c_packed_file::CloseFile()
	{
		if(m_file_mapped)
		{
			UnmapViewOfFile(m_address);
			m_header = NULL;

			CloseHandle(m_mapping_handle);
			m_mapping_handle = INVALID_HANDLE_VALUE;

			CloseHandle(m_file_handle);
			m_file_handle = INVALID_HANDLE_VALUE; 

			m_file_mapped = false;
		}
	}

	void* c_packed_file::GetDataPointer(uint32 index, _Out_opt_ uint32* data_size)
	{
		if(!m_file_mapped || index > m_header->element_count)
			return NULL;

		s_element* element = CAST_PTR(s_element*, m_base_address + sizeof(s_header) + 
			(sizeof(s_element) * index));

		if(data_size)
			*data_size = element->element_size; 
		
		return CAST_PTR(void*, m_base_address + element->element_offset);
	}

	void* c_packed_file::GetDataPointer(const char* data_id, _Out_opt_ uint32* data_size)
	{
		if(!m_file_mapped || strlen(data_id) == 0)
			return NULL;

		for(uint32 i = 0; i < m_header->element_count; i++)
		{
			s_element* element = CAST_PTR(s_element*, m_base_address + sizeof(s_header) + 
				(sizeof(s_element) * i));

			if(strcmp(data_id, CAST_PTR(char*, m_base_address + (uint32)element->element_id_offset)) == 0)
			{
				if(data_size)
					*data_size = element->element_size; 

				return CAST_PTR(void*, m_base_address + element->element_offset);
			}
		}

		if(data_size)
			*data_size = 0;
		return NULL;
	}

#else
	c_packed_file::c_packed_file(uint32 element_count)
	{
		m_header.Ctor();

		m_elements = new s_element_editor[element_count];
		m_header.element_count = element_count;
	}

	c_packed_file::~c_packed_file()
	{
		delete [] m_elements;
		m_elements = NULL;

		m_header.element_count = 0;
		m_header.file_size = 0;
	}

	void c_packed_file::CalculateOffsets()
	{
		uint32 id_base_offset = sizeof(s_header) + (sizeof(s_element) * m_header.element_count);
		uint32 id_offset = 0;
		for(uint32 i = 0; i < m_header.element_count; i++)
		{
			m_elements[i].element_id_offset = id_base_offset + id_offset;
			id_offset += strlen(m_elements[i].source_id) + 1;
		}

		uint32 data_base_offset = id_base_offset + id_offset;
		uint32 data_offset = 0;
		for(uint32 i = 0; i < m_header.element_count; i++)
		{
			m_elements[i].element_offset = data_base_offset + data_offset;
			data_offset += m_elements[i].element_size;
		}

		m_header.file_size = data_base_offset + data_offset;
	}

	HRESULT c_packed_file::Save(cstring save_location)
	{

		CalculateOffsets();

		if(m_header.element_count == 0)
		{
			YELO_ERROR(_error_message_priority_warning, "c_packed_file: no elements to pack for %s", save_location);
			return E_FAIL;
		}

		std::ofstream file(save_location, std::ios::out | std::ios::binary);				
		if(file.fail())	
		{
			YELO_ERROR(_error_message_priority_warning, "c_packed_file: failed to open file for writing (%s) %s", 
				"file may be in use?", save_location);

			return E_FAIL;
		}

		file.write(CAST_PTR(char*, &m_header), sizeof(m_header));
		for(uint32 i = 0; i < m_header.element_count; i++)
			file.write(CAST_PTR(char*, &m_elements[i]), sizeof(s_element));

		char null_char = 0;
		for(uint32 i = 0; i < m_header.element_count; i++)
		{
			file.write(m_elements[i].source_id, strlen(m_elements[i].source_id));
			file.write(&null_char, sizeof(null_char));
		}

		for(uint32 i = 0; i < m_header.element_count; i++)
			file.write(CAST_PTR(char*, m_elements[i].source_data), m_elements[i].element_size);

		file.flush();
		file.close();

		return S_OK;
	}
#endif
};