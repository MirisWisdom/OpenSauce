/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include "YeloLib/Halo1/cache/cache_file_memory.hpp"
#if PLATFORM_TYPE == PLATFORM_TOOL

#include <YeloLib/Halo1/cache/cache_file_memory_gestalt_definitions.hpp>
#include <YeloLib/Halo1/cache/cache_file_memory_layout_table_definitions.hpp>
#include <YeloLib/Halo1/tag_files/string_id_yelo.hpp>

namespace Yelo
{
	namespace TagGroups
	{
	};

	namespace Cache
	{
		int16 c_cache_file_memory_controller::RegisterBlockDefinition(const tag_block_definition* block_definition)
		{
			return NONE;
		}
		int16 c_cache_file_memory_controller::RegisterDataDefinition(const tag_data_definition* data_definition)
		{
			return NONE;
		}

		uintptr_t c_cache_file_memory_controller::GetVirtualBaseAddress() const
		{
			return m_layout->virtual_base_address;
		}
		void c_cache_file_memory_controller::SetVirtualBaseAddress(uintptr_t base_address)
		{
			m_layout->virtual_base_address = base_address;
		}
	};
};
#endif