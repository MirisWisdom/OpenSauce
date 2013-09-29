/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <blamlib/Halo1/cseries/errors.hpp>

namespace Yelo
{
	namespace Engine
	{
		// NOTE: command line parameters are not populated in guerilla nor tool.exe
		// So basically only use this in sapien builds
		bool GetCmdLineParameter(cstring parameter, __out_opt cstring* value_out = nullptr);

		// Primarily a deleter for std::unique_ptr
		template<typename T>
		struct debug_memory_deleter
		{
			inline void operator()(T* ptr) const;
		};
		template<typename T>
		struct debug_memory_array_deleter
		{
			inline void operator()(T* ptr) const;
		};
	};

	// TODO: move to cseries/debug_memory.hpp
	namespace blam
	{
		void* PLATFORM_API debug_malloc(const size_t size, const bool fill_with_garbage, cstring file, const uint32 line);

		template<typename T>
		T* debug_new(const size_t count, const bool fill_with_garbage, cstring file, const uint32 line)
		{
			return CAST_PTR(T*, debug_malloc(sizeof(T) * count, fill_with_garbage, file, line));
		}

		void PLATFORM_API debug_free(void* pointer, cstring file, const uint32 line);

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
		template<typename T>
		void debug_delete_array(T*& pointer, cstring file, const uint32 line)
		{
			if(pointer != nullptr)
				debug_free(pointer, file, line);

			pointer = nullptr;
		}

		void* PLATFORM_API debug_realloc(void* pointer, const size_t new_size, cstring file, const uint32 line);

		template<typename T>
		T* debug_renew(T* pointer, const size_t count, cstring file, const uint32 line)
		{
			return CAST_PTR(T*, debug_realloc(pointer, sizeof(T) * count, file, line));
		}
	};
};

// NOTE: it is not advisable to actually use these with types which define their own destructors

#define YELO_MALLOC(size, fill_with_garbage)	Yelo::blam::debug_malloc( (size), fill_with_garbage, __FILE__, __LINE__)
#define YELO_NEW_ARRAY(type, count)				Yelo::blam::debug_new<type>( (count), false, __FILE__, __LINE__)
#define YELO_NEW(type)							YELO_NEW_ARRAY(type, 1)

#define YELO_MALLOC_UNIQUE(size, fill_with_garbage)			\
	std::unique_ptr<void, Yelo::Engine::debug_memory_deleter<void>>( YELO_MALLOC(size, fill_with_garbage) )
#define YELO_NEW_UNIQUE(type)								\
	std::unique_ptr<type, Yelo::Engine::debug_memory_deleter<T>>( YELO_NEW(type) )
#define YELO_NEW_ARRAY_UNIQUE(type, count)					\
	std::unique_ptr<type[], Yelo::Engine::debug_memory_array_deleter<T>>( YELO_NEW_ARRAY(type, count) )

#define YELO_FREE(ptr)							Yelo::blam::debug_free( (ptr), __FILE__, __LINE__)
#define YELO_DELETE(ptr)						Yelo::blam::debug_delete( (ptr), __FILE__, __LINE__ )
#define YELO_DELETE_ARRAY(ptr)					Yelo::blam::debug_delete_array( (ptr), __FILE__, __LINE__ )

#define YELO_REALLOC(ptr, new_size)				Yelo::blam::debug_realloc( (ptr), (new_size), __FILE__, __LINE__)
#define YELO_RENEW_ARRAY(type, ptr, count)		Yelo::blam::debug_renew<type>( (ptr), (count), __FILE__, __LINE__)
#define YELO_RENEW(type, ptr)					YELO_RENEW_ARRAY(type, ptr, 1)

namespace Yelo
{
	namespace Engine
	{
		template<typename T>
		void debug_memory_deleter<T>::operator()(T* ptr) const			{ YELO_DELETE(ptr); }
		template<typename T>
		void debug_memory_array_deleter<T>::operator()(T* ptr) const	{ YELO_DELETE_ARRAY(ptr); }
	};
}