/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once

#include <blamlib/Halo1/cseries/errors.hpp>
#include <blamlib/Halo1/tag_files/files.hpp>

namespace Yelo
{
	namespace EngineFunctions
	{
		typedef void (PLATFORM_API* proc_error)(Enums::error_message_priority priority, cstring format, ...);

		extern proc_error error;

#ifndef YELO_NO_ERROR_MACRO
	#define YELO_ERROR(priority, format, ...) \
		Yelo::EngineFunctions::error(Yelo::Enums::priority, format, __VA_ARGS__)
#else
	#define YELO_ERROR(priority, format, ...) __noop
#endif

#define YELO_WARN(format, ...) \
	Yelo::EngineFunctions::error(Yelo::Enums::_error_message_priority_warning, format, __VA_ARGS__)

		// warn = halt == false
		void display_assert(cstring reason, cstring file, const uint32 line, bool halt);
#define YELO_ASSERT(expression)															\
	if( !(expression) )																	\
	{																					\
		Yelo::EngineFunctions::display_assert( #expression , __FILE__, __LINE__, true);	\
	}
#define YELO_ASSERT_WARN(expression)													\
	if( !(expression) )																	\
	{																					\
		Yelo::EngineFunctions::display_assert( #expression , __FILE__, __LINE__, false);\
	}


		void* debug_malloc(const size_t size, const bool fill_with_garbage, cstring file, const uint32 line);

		template<typename T>
		T* debug_new(const size_t count, const bool fill_with_garbage, cstring file, const uint32 line)
		{
			return CAST_PTR(T*, debug_malloc(sizeof(T) * count, fill_with_garbage, file, line));
		}

		void debug_free(void* pointer, cstring file, const uint32 line);

		template<typename T>
		void debug_delete(T*& pointer, cstring file, const uint32 line)
		{
			if(pointer != NULL)
				debug_free(pointer, file, line);

			pointer = NULL;
		}

		void* debug_realloc(void* pointer, const size_t new_size, cstring file, const uint32 line);

		template<typename T>
		void* debug_renew(T* pointer, const size_t count, cstring file, const uint32 line)
		{
			return CAST_PTR(T*, debug_realloc(pointer, sizeof(T) * count, file, line));
		}


		void file_reference_create(s_file_reference& reference, long_enum location = Enums::_file_reference_location_tags);

		s_file_reference& file_reference_add_directory(s_file_reference& reference, cstring directory);

		s_file_reference& file_reference_set_name(s_file_reference& reference, cstring name);

		s_file_reference& file_reference_create(s_file_reference& reference, cstring directory, cstring name, cstring ext, 
			long_enum location = Enums::_file_reference_location_tags);

		int16 find_files(long_flags, const s_file_reference& directory, int32 maximum_count, s_file_reference references[]);
	};
};

#define YELO_MALLOC(size, fill_with_garbage)	Yelo::EngineFunctions::debug_malloc( (size), fill_with_garbage, __FILE__, __LINE__)
#define YELO_NEW_ARRAY(type, count)				Yelo::EngineFunctions::debug_new<type>( (count), false, __FILE__, __LINE__)
#define YELO_NEW(type)							YELO_NEW_ARRAY(type, 1)

#define YELO_FREE(ptr)							Yelo::EngineFunctions::debug_free( (ptr), __FILE__, __LINE__)
#define YELO_DELETE(ptr)						Yelo::EngineFunctions::debug_delete( (ptr), __FILE__, __LINE__ )

#define YELO_REALLOC(ptr, new_size)				Yelo::EngineFunctions::debug_realloc( (ptr), (new_size), __FILE__, __LINE__)
#define YELO_RENEW_ARRAY(type, ptr, count)		Yelo::EngineFunctions::debug_renew<type>( (ptr), (count), __FILE__, __LINE__)
#define YELO_RENEW(type, ptr)					YELO_RENEW_ARRAY(type, ptr, 1)