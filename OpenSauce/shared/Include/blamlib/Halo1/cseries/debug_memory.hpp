/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Debug
	{
		void BlamMemoryManagerDispose();
	};

	namespace blam
	{
		void PLATFORM_API debug_dump_memory();


		// The engine's debug memory manager on win32 boils down to GlobalAlloc family of memory functions,
		// which is guaranteed to be aligned on an 8-byte boundary (http://msdn.microsoft.com/en-us/library/aa366574%28v=vs.12%29.ASPX)
		// See the system_malloc, etc functions in the engine's cseries.hpp

		// returns NULL when out of memory
		void* PLATFORM_API debug_malloc(const size_t size, const bool fill_with_garbage, cstring file, const uint32 line);

		template<typename T> inline
		T* debug_new(const bool fill_with_garbage, cstring file, const uint32 line)
		{
			return CAST_PTR(T*, debug_malloc(sizeof(T) * 1, fill_with_garbage, file, line));
		}
		template<typename T> inline
		T* debug_new_array(const size_t count, const bool fill_with_garbage, cstring file, const uint32 line)
		{
			return CAST_PTR(T*, debug_malloc(sizeof(T) * count, fill_with_garbage, file, line));
		}

		void PLATFORM_API debug_free(void* pointer, cstring file, const uint32 line);
		// Nulls [pointer] before returning
		template<typename T>
		void debug_free_with_null(T*& pointer, cstring file, const uint32 line)
		{
			debug_free(pointer, file, line);

			pointer = nullptr;
		}

		// Nulls [pointer] before returning
		template<typename T>
		void debug_delete(T*& pointer, cstring file, const uint32 line)
		{
			if(pointer != nullptr)
			{
				pointer->~T();
				debug_free(pointer, file, line);
			}

			pointer = nullptr;
		}
		// Nulls [pointer] before returning
		template<typename T>
		void debug_delete_array(T*& pointer, cstring file, const uint32 line)
		{
			debug_free(pointer, file, line);

			pointer = nullptr;
		}

		void* PLATFORM_API debug_realloc(void* pointer, const size_t new_size, cstring file, const uint32 line);

		template<typename T>
		T* debug_renew(T* pointer, cstring file, const uint32 line)
		{
			return CAST_PTR(T*, debug_realloc(pointer, sizeof(T) * 1, file, line));
		}
		template<typename T>
		T* debug_renew_array(T* pointer, const size_t count, cstring file, const uint32 line)
		{
			return CAST_PTR(T*, debug_realloc(pointer, sizeof(T) * count, file, line));
		}
	};
};