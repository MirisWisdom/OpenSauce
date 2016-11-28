/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/files/packed_file.hpp>

#if !PLATFORM_IS_EDITOR
	#include <YeloLib/files/files.hpp> // for OpenFileByID forward declaration
#else
	#include <fstream>
	#include <blamlib/Halo1/cseries/errors.hpp>
#endif

namespace Yelo
{
#if !PLATFORM_IS_EDITOR
	namespace FileIO
	{
		Enums::file_io_open_error OpenFileByID(s_file_info& info_out, cstring file_id);
	};

	void c_packed_file::OpenFile(cstring packed_file, bool is_file_id)
	{
		Enums::file_io_open_error open_success;
		if(is_file_id)
			open_success = FileIO::OpenFileByID(file_info, packed_file);
		else
			open_success = FileIO::OpenFile(file_info, packed_file);
		if(open_success != Enums::_file_io_open_error_none) return;

		Enums::file_io_read_error read_success = FileIO::MemoryMapFile(file_info);
		if(read_success != Enums::_file_io_read_error_none) return;

		m_address = file_info.data_pointer;

		if(m_header != nullptr && m_header->IsValid())
			m_file_mapped = true;
	}

	void c_packed_file::CloseFile()
	{
		if(m_file_mapped)
		{
			m_header = nullptr;

			FileIO::CloseFile(file_info);

			m_file_mapped = false;
		}
	}

	void* c_packed_file::GetDataPointer(uint32 index, _Out_opt_ uint32* data_size)
	{
		if(!m_file_mapped || index > m_header->element_count)
			return nullptr;

		s_element* element = CAST_PTR(s_element*, m_base_address + sizeof(s_header) + 
			(sizeof(s_element) * index));

		if(data_size)
			*data_size = element->element_size; 
		
		return CAST_PTR(void*, m_base_address + element->element_offset);
	}

	void* c_packed_file::GetDataPointer(const char* data_id, _Out_opt_ uint32* data_size)
	{
		if(!m_file_mapped || is_null_or_empty(data_id))
			return nullptr;

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
		return nullptr;
	}

#else
	c_packed_file::c_packed_file()
	{
		m_header.Ctor();

		m_elements.clear();
		m_header.element_count = 0;
	}

	c_packed_file::~c_packed_file()
	{
		for(auto iter = m_elements.begin(); iter != m_elements.end(); ++iter)
			iter->Delete();
		m_elements.clear();

		m_header.element_count = 0;
		m_header.file_size = 0;
	}

	void c_packed_file::CalculateOffsets()
	{
		uint32 id_base_offset = sizeof(s_header) + (sizeof(s_element) * m_elements.size());
		uint32 id_offset = 0;

		for(auto iter = m_elements.begin(); iter != m_elements.end(); ++iter)
		{
			iter->element_id_offset = id_base_offset + id_offset;
			id_offset += strlen(iter->source_id) + 1;
		}

		uint32 data_base_offset = id_base_offset + id_offset;
		uint32 data_offset = 0;

		for(auto iter = m_elements.begin(); iter != m_elements.end(); ++iter)
		{
			iter->element_offset = data_base_offset + data_offset;
			data_offset += iter->element_size;
		}

		m_header.file_size = data_base_offset + data_offset;
	}

	HRESULT c_packed_file::Save(cstring save_location)
	{
		CalculateOffsets();

		m_header.element_count = m_elements.size();

		if(m_header.element_count == 0)
		{
			YELO_WARN("c_packed_file: no elements to pack for %s", save_location);
			return E_FAIL;
		}

		std::ofstream file(save_location, std::ios::out | std::ios::binary);				
		if(file.fail())	
		{
			YELO_WARN("c_packed_file: failed to open file for writing (%s) %s",
				"file may be in use?", save_location);

			return E_FAIL;
		}

		file.write(CAST_PTR(char*, &m_header), sizeof(m_header));

		for(auto iter = m_elements.cbegin(); iter != m_elements.cend(); ++iter)
			file.write(CAST_PTR(const char*, &(*iter)), sizeof(s_element));

		char null_char = 0;
		for(auto iter = m_elements.cbegin(); iter != m_elements.cend(); ++iter)
		{
			file.write(iter->source_id, strlen(iter->source_id));
			file.write(&null_char, sizeof(null_char));
		}

		for(auto iter = m_elements.cbegin(); iter != m_elements.cend(); ++iter)
			file.write(CAST_PTR(char*, iter->source_data), iter->element_size);

		file.flush();
		file.close();

		return S_OK;
	}
	void c_packed_file::AddElement(s_element_editor& element)
	{
		m_elements.push_back(element);
	}
#endif
};