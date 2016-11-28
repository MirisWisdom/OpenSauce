/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Memory
	{
		class c_physical_memory_allocation
		{
			void* m_base_address;
			size_t m_size;
			struct s_flags {
				unsigned valid : 1;
				unsigned allocated : 1;
			}m_flags;
			uint32 m_error_code;
		public:
			// Check whether this allocation is both valid and the memory is allocated
			bool IsValid() const;

			// Get the base address of this allocation
			void* GetBaseAddress() const;
			// Get the size of this allocation
			size_t GetSize() const;
			// Get the platform-specific error code gathered from a bad allocation
			uint32 GetErrorCode() const;

			// Initializes an instance with the given values
			// If the values are invalid, operations will safely fail and IsValid() will return false
			c_physical_memory_allocation(void* base_address, size_t size);
			// Renders the instance invalid, Free'ing the memory as well
			~c_physical_memory_allocation();

			// Allocate the memory at the given base address. Returns false if an error occurs
			bool Allocate();
			// Frees the allocated memory at the given base address. Returns false if an error occurs
			bool Free();
		};
	};
};