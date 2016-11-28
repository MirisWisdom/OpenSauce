/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#if PLATFORM_IS_EDITOR
#include <YeloLib/Halo1/tag_files/tag_group_memory.hpp>

#include <blamlib/Halo1/cseries/debug_memory.hpp>
#include <blamlib/Halo1/cseries/errors.hpp>
#include <blamlib/Halo1/tag_files/tag_groups.hpp>

namespace Yelo
{
	namespace TagGroups
	{
		//////////////////////////////////////////////////////////////////////////
		// s_tag_allocation_header
		void s_tag_allocation_header::Initialize()
		{
			signature = k_signature;
			hash = NONE;
			total_hash = NONE;
		}
		void s_tag_allocation_header::Initialize(const tag_block_definition* definition)
		{
			Initialize();
		}
		void s_tag_allocation_header::Initialize(const tag_data_definition* definition)
		{
			Initialize();
		}

		s_tag_allocation_header* s_tag_allocation_header::GetHeader(void* body_address, cstring body_definition_name)
		{
			s_tag_allocation_header* header = nullptr;

			if (s_tag_allocation_header::Enabled() && body_address)
			{
				header = GetHeaderFromBody(body_address);
				if (header->signature != k_signature)
				{
					header = nullptr;
					YELO_WARN("tried to get the allocation header of an %s @%p with an invalid header signature",
						body_definition_name, body_address);
				}
			}

			return header;
		}
		s_tag_allocation_header* s_tag_allocation_header::Get(tag_block& instance)
		{
			return GetHeader(instance.address, instance.definition->name);
		}
		s_tag_allocation_header* s_tag_allocation_header::Get(tag_data& instance)
		{
			return GetHeader(instance.address, instance.definition->name);
		}

		//////////////////////////////////////////////////////////////////////////
		// c_tag_block_allocations
		size_t c_tag_block_allocations::CalculateSize(const tag_block_definition* definition, int32 element_count)
		{
			// TODO: we don't factor in definition's desired alignment

			size_t size = definition->element_size * element_count;

			if (s_tag_allocation_header::Enabled())
				size += sizeof(s_tag_allocation_header);

			return size;
		}
		void* c_tag_block_allocations::New(const tag_block_definition* definition, int32 element_count, cstring file, uint32 line)
		{			
			size_t size = CalculateSize(definition, element_count);

			void* address = blam::debug_malloc(size, false, file, line);

			if (s_tag_allocation_header::Enabled())
			{
				auto header = CAST_PTR(s_tag_allocation_header*, address);
				header->Initialize(definition);

				address = header->GetBody();
			}

			return address;
		}
		void* c_tag_block_allocations::Realloc(tag_block& instance, int32 new_count, cstring file, uint32 line)
		{
			size_t new_size = CalculateSize(instance.definition, new_count);

			void* address = s_tag_allocation_header::Enabled()
				? s_tag_allocation_header::Get(instance)
				: instance.address;

			address = blam::debug_realloc(address, new_size, file, line);

			if (s_tag_allocation_header::Enabled())
				address = CAST_PTR(s_tag_allocation_header*, address)->GetBody();

			instance.count = new_count;
			return instance.address = address;
		}
		void c_tag_block_allocations::Delete(void* elements, const tag_block_definition* definition, cstring file, uint32 line)
		{
			if (s_tag_allocation_header::Enabled() && elements)
			{
				auto header = s_tag_allocation_header::GetHeader(elements, definition->name);

				elements = header;
			}

			if (elements != nullptr)
				blam::debug_free(elements, file, line);
		}
		void c_tag_block_allocations::Delete(tag_block& instance, cstring file, uint32 line)
		{
			void* elements = instance.address;

			Delete(elements, instance.definition, file, line);

			instance.address = nullptr;
		}

		//////////////////////////////////////////////////////////////////////////
		// c_tag_data_allocations
		int32 c_tag_data_allocations::CalculateSize(const tag_data_definition* definition, int32 size)
		{
			// TODO: we don't factor in definition's desired alignment

			if (s_tag_allocation_header::Enabled())
				size += sizeof(s_tag_allocation_header);

			return size;
		}
		void* c_tag_data_allocations::New(const tag_data_definition* definition, int32 size, cstring file, uint32 line)
		{
			size = CalculateSize(definition, size);

			void* address = blam::debug_malloc(CAST(size_t, size), false, file, line);

			if (s_tag_allocation_header::Enabled())
			{
				auto header = CAST_PTR(s_tag_allocation_header*, address);
				header->Initialize(definition);

				address = header->GetBody();
			}

			return address;
		}
		void* c_tag_data_allocations::Realloc(tag_data& instance, int32 new_size, cstring file, uint32 line)
		{
			new_size = CalculateSize(instance.definition, new_size);

			void* address = s_tag_allocation_header::Enabled()
				? s_tag_allocation_header::Get(instance)
				: instance.address;

			address = blam::debug_realloc(address, new_size, file, line);

			if (s_tag_allocation_header::Enabled())
				address = CAST_PTR(s_tag_allocation_header*, address)->GetBody();

			instance.size = new_size;
			return instance.address = address;
		}
		void c_tag_data_allocations::Delete(tag_data& instance, cstring file, uint32 line)
		{
			void* address = instance.address;

			if (s_tag_allocation_header::Enabled() && address)
			{
				auto header = s_tag_allocation_header::Get(instance);

				address = header;
			}

			if (address != nullptr)
			{
				blam::debug_free(address, file, line);
				instance.address = nullptr;
			}
		}
	};
};

#endif