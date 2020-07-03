/*
    Yelo: Open Sauce SDK
		Halo 2 (Editing Kit) Edition

	See license\OpenSauce\Halo2_CheApe for specific license information
*/
#pragma once

#include <blamlib/Halo2/cseries/errors.hpp>
#include <blamlib/Halo2/tag_files/files.hpp>

namespace Yelo
{
	namespace Enums
	{
		enum known_folder_location
		{
			_known_folder_location_temp_saved_games,
			_known_folder_location_temp_maps,
			_known_folder_location_temp_secondary,

			_known_folder_location_editor,
			_known_folder_location_game_maps,
			_known_folder_location_game,
			_known_folder_location_game_screenshots,
			_known_folder_location_game_map_data_files,

			_known_folder_location
		};
	};

	namespace EngineFunctions
	{
		typedef void (PLATFORM_API* proc_error)(Enums::error_message_priority priority, 
			cstring format, ...);
		typedef void (PLATFORM_API* proc_error_category)(Enums::error_category category, Enums::error_message_priority priority, 
			cstring format, ...);

		extern proc_error error;
		extern proc_error_category error_category;

		typedef void (PLATFORM_API* proc_error_unicode)(Enums::error_message_priority priority, 
			wcstring format, ...);
		typedef void (PLATFORM_API* proc_error_unicode_category)(Enums::error_category category, Enums::error_message_priority priority, 
			wcstring format, ...);

		extern proc_error_unicode error_unicode;
		extern proc_error_unicode_category error_unicode_category;

#ifdef _DEBUG
	#define YELO_ERROR(priority, format, ...) \
		Yelo::EngineFunctions::error(Yelo::Enums::priority, format, __VA_ARGS__)
	
	#define YELO_ERROR_CATEGORY(category, priority, format, ...) \
		Yelo::EngineFunctions::error_category(Yelo::Enums::category, Yelo::Enums::priority, format, __VA_ARGS__)
#else
	#define YELO_ERROR(priority, format, ...) __noop

	#define YELO_ERROR_CATEGORY(category, priority, format, ...) __noop
#endif

#define YELO_ASSERT(category, expression)												\
	if( !(expression) )																	\
	{																					\
		YELO_ERROR_CATEGORY(category, _error_message_priority_assert, "%s[%d]: %s",		\
			__FILE__, __LINE__, #expression);											\
	}
#define YELO_ASSERT_WARN(category, expression)											\
	if( !(expression) )																	\
	{																					\
		YELO_ERROR_CATEGORY(category, _error_message_priority_warning, "%s[%d]: %s",	\
			__FILE__, __LINE__, #expression);											\
	}


		void* debug_malloc(const size_t size, cstring file, const uint32 line, const uint32 alignment_bit = 0,
			cstring type_name = nullptr, cstring subtype_name = nullptr, cstring memory_name = nullptr);

		template<typename T>
		T* debug_new(const size_t count, cstring file, const uint32 line, const uint32 alignment_bit = 0, 
			cstring type_name = nullptr, cstring subtype_name = nullptr, cstring memory_name = nullptr)
		{
			return CAST_PTR(T*, debug_malloc(sizeof(T) * count, file, line, alignment_bit,
				type_name, subtype_name, memory_name));
		}

		void debug_free(void* pointer, cstring file, const uint32 line);

		template<typename T>
		void debug_delete(T* pointer, cstring file, const uint32 line)
		{
			debug_free(pointer, file, line);
		}

		void* debug_realloc(void* pointer, const size_t new_size, cstring file, const uint32 line, 
			const uint32 alignment_bit = 0,
			cstring type_name = nullptr, cstring subtype_name = nullptr, cstring memory_name = nullptr);

		template<typename T>
		void* debug_renew(T* pointer, const size_t count, cstring file, const uint32 line, 
			const uint32 alignment_bit = 0,
			cstring type_name = nullptr, cstring subtype_name = nullptr, cstring memory_name = nullptr)
		{
			return CAST_PTR(T*, debug_realloc(pointer, sizeof(T) * count, file, line,
				alignment_bit, type_name, subtype_name, memory_name));
		}


		void file_reference_create(s_file_reference& reference, long_enum location = 1); // 1 = tags

		s_file_reference& file_reference_add_directory(s_file_reference& reference, cstring directory);

		s_file_reference& file_reference_set_name(s_file_reference& reference, cstring name);

		s_file_reference& file_reference_create(s_file_reference& reference, cstring directory, cstring name, cstring ext, 
			long_enum location = 1);

		int16 find_files(long_flags, const s_file_reference& directory, int32 maximum_count, s_file_reference references[]);
	};
};

#define YELO_MALLOC(size, ...)				Yelo::EngineFunctions::debug_malloc( (size), __FILE__, __LINE__, __VA_ARGS__)
#define YELO_NEW_ARRAY(type, count, ...)	Yelo::EngineFunctions::debug_new<type>( (count), __FILE__, __LINE__, __VA_ARGS__)
#define YELO_NEW(type, ...)					YELO_NEW_ARRAY(type, 1, __VA_ARGS__)

#define YELO_FREE(ptr)		Yelo::EngineFunctions::debug_free( (ptr), __FILE__, __LINE__)
#define YELO_DELETE(ptr)	Yelo::EngineFunctions::debug_delete( (ptr), __FILE__, __LINE__ )

#define YELO_REALLOC(ptr, new_size, ...)		Yelo::EngineFunctions::debug_realloc( (ptr), (new_size), __FILE__, __LINE__, __VA_ARGS__)
#define YELO_RENEW_ARRAY(type, ptr, count, ...)	Yelo::EngineFunctions::debug_renew<type>( (ptr), (count), __FILE__, __LINE__, __VA_ARGS__)
#define YELO_RENEW(type, ptr, ...)				YELO_RENEW_ARRAY(type, ptr, 1, __VA_ARGS__)