/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/memory/physical_memory_allocation.hpp>

namespace Yelo
{
	namespace Memory
	{
		bool c_physical_memory_allocation::IsValid() const			{ return m_flags.valid; }
		void* c_physical_memory_allocation::GetBaseAddress() const	{ return m_base_address; }
		size_t c_physical_memory_allocation::GetSize() const		{ return m_size; }
		uint32 c_physical_memory_allocation::GetErrorCode() const	{ return m_error_code; }

		c_physical_memory_allocation::c_physical_memory_allocation(void* base_address, size_t size) :
			m_base_address(base_address),
			m_size(size),
			m_flags(),
			m_error_code(ERROR_SUCCESS)
		{
			m_flags.valid = base_address != nullptr && size > 0;
		}

		c_physical_memory_allocation::~c_physical_memory_allocation()
		{
			this->Free();

			m_base_address = nullptr;
			m_size = 0;
			m_flags = s_flags();
		}

		bool c_physical_memory_allocation::Allocate()
		{
			void* address = VirtualAlloc(m_base_address, m_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

			if(address == nullptr || address != m_base_address)
				m_error_code = GetLastError();
			else
				m_error_code = ERROR_SUCCESS;

			return m_error_code == ERROR_SUCCESS;
		}

		bool c_physical_memory_allocation::Free()
		{
			bool result = true;

			if(m_flags.valid && m_flags.allocated)
			{
				result = VirtualFree(m_base_address, 0, MEM_RELEASE) != FALSE;
				m_error_code = result ? ERROR_SUCCESS : GetLastError();
			}

			return result;
		}
	};
};