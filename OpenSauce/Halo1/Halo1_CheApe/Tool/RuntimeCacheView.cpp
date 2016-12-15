/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#pragma once
#include "Common/Precompile.hpp"
#include "Tool/RuntimeCacheView.hpp"
#if PLATFORM_TYPE == PLATFORM_TOOL
#include <psapi.h>

#include <blamlib/Halo1/cache/cache_files_structures.hpp>

#include "Common/StringEditing.hpp"
#include "Engine/EngineFunctions.hpp"
#include "TagGroups/TagGroups.hpp"
#include "Tool/Console.hpp"

// TODO: also, we should now be able to use c_tag_field_scanner, instead of the manual code we have
// TODO: also, start using k_tag_field_definitions and remove the duplicated data from g_field_descriptions

namespace Yelo
{
	namespace Tool
	{
		enum runtime_cache_command
		{
			_runtime_cache_command_error,
			_runtime_cache_command_help,
			_runtime_cache_command_load,
			_runtime_cache_command_quit,
			_runtime_cache_command_list,
			_runtime_cache_command_list_filtered,
			_runtime_cache_command_open,
			_runtime_cache_command_set,
			_runtime_cache_command_find_address,

			_runtime_cache_command
		};

		struct s_tag_field_description
		{
			const long_enum				m_field_type;
			const char*					m_field_type_name;
			const char*					m_format_string;
			const int32					m_scan_count;
			const char*					m_scan_format;

			inline const TagGroups::s_tag_field_definition& Definition() const
			{
				return TagGroups::k_tag_field_definitions[m_field_type];
			}

			inline bool Editable() const
			{
				return m_scan_format != nullptr;
			}
		};

		
		static const void* k_runtime_109_cache_header_ptr = CAST_PTR(void*, 0x643060 + 0x4);
		static const void* k_runtime_109_cache_globals_ptr = CAST_PTR(void*, 0x643060 + 0x804);
		static const void* k_runtime_110_cache_header_ptr = CAST_PTR(void*, 0x643040 + 0x4);
		static const void* k_runtime_110_cache_globals_ptr = CAST_PTR(void*, 0x643040 + 0x804);

		// Last I checked, compiler was initializing with a dynamic initializer.
		// Guess there's just a shit load of fields, or the use of enum values is causing this. Oh well.
		static const s_tag_field_description g_field_descriptions[] = 
		{
			{ Enums::_field_string,					"string",				"%s",									1,	"%s" },
			{ Enums::_field_char_integer,			"char_integer",			"%d",									1,	"%d" },
			{ Enums::_field_short_integer,			"short_integer",		"%i",									1,	"%i" },
			{ Enums::_field_long_integer,			"long_integer",			"%i",									1,	"%i" },
			{ Enums::_field_angle,					"angle",				"%e degrees",							1,	"%f" },
			{ Enums::_field_tag,					"tag",					"%4s",									1,	"%04s" },
			{ Enums::_field_enum,					"enum",					"%u",									1,	"%u" },
			{ Enums::_field_long_flags,				"long_flags",			"%u",									1,	"%u" },
			{ Enums::_field_word_flags,				"word_flags",			"%u",									1,	"%u" },
			{ Enums::_field_byte_flags,				"byte_flags",			"%u",									1,	"%u" },
			{ Enums::_field_point_2d,				"point_2d",				"x: %i\ty: %i",							2,	"%i %i" },
			{ Enums::_field_rectangle_2d,			"rectangle_2d",			"t: %i\tb: %i\tl: %i\tr: %i",			4,	"%i %i %i %i" },
			{ Enums::_field_rgb_color,				"rgb_color",			"r: %i\tg: %i\tb: %i",					3,	"%i %i %i" },
			{ Enums::_field_argb_color,				"argb_color",			"a: %i\tr: %i\tg: %i\tb: %i",			4,	"%i %i %i %i" },
			{ Enums::_field_real,					"real",					"%0.6f",								1,	"%f" },
			{ Enums::_field_real_fraction,			"real_fraction",		"%0.6f",								1,	"%f" },
			{ Enums::_field_real_point_2d,			"real_point_2d",		"x: %0.6f y: %0.6f",					2,	"%f %f" },
			{ Enums::_field_real_point_3d,			"real_point_3d",		"x: %0.6f y: %0.6f z: %0.6f",			3,	"%f %f %f" },
			{ Enums::_field_real_vector_2d,			"real_vector_2d",		"i: %0.6f j: %0.6f",					2,	"%f %f" },
			{ Enums::_field_real_vector_3d,			"real_vector_3d",		"i: %0.6f j: %0.6f k: %0.6f",			3,	"%f %f %f" },
			{ Enums::_field_real_quaternion,		"real_quaternion",		"i: %0.6f j: %0.6f k: %0.6f w: %0.6f",	4,	"%f %f %f %f" },
			{ Enums::_field_real_euler_angles_2d,	"real_euler_angles_2d",	"x: %0.6f y: %0.6f",					2,	"%f %f" },
			{ Enums::_field_real_euler_angles_3d,	"real_euler_angles_3d",	"x: %0.6f y: %0.6f z: %0.6f",			3,	"%f %f %f" },
			{ Enums::_field_real_plane_2d,			"real_plane_2d",		"i: %0.6f j: %0.6f d: %0.6f",			3,	"%f %f %f" },
			{ Enums::_field_real_plane_3d,			"real_plane_3d",		"i: %0.6f j: %0.6f k: %0.6f d: %0.6f",	4,	"%f %f %f %f" },
			{ Enums::_field_real_rgb_color,			"real_rgb_color",		"r: %0.6f g: %0.6f b: %0.6f",			3,	"%f %f %f" },
			{ Enums::_field_real_argb_color,		"real_argb_color",		"a: %0.6f r: %0.6f g: %0.6f b: %0.6f",	4,	"%f %f %f %f" },
			{ Enums::_field_real_hsv_color,			"real_hsv_color",		"h: %0.6f s: %0.6f v: %0.6f",			3,	"%f %f %f" },
			{ Enums::_field_real_ahsv_color,		"real_ahsv_color",		"a: %0.6f h: %0.6f s: %0.6f v: %0.6f",	4,	"%f %f %f %f" },
			{ Enums::_field_short_bounds,			"short_bounds",			"%i to %i",								2,	"%i %i"},
			{ Enums::_field_angle_bounds,			"angle_bounds",			"%0.6f to %0.6f",						2,	"%f %f" },
			{ Enums::_field_real_bounds,			"real_bounds",			"%0.6f to %0.6f",						2,	"%f %f" },
			{ Enums::_field_real_fraction_bounds,	"real_fraction_bounds",	"%0.6f to %0.6f",						2,	"%f %f" },
			{ Enums::_field_tag_reference,			"tag_reference",		"tag: %4s\tindex: %i",					0,	nullptr },
			{ Enums::_field_block,					"block",				"count: %i\taddress: 0x%08X",			0,	nullptr },
			{ Enums::_field_short_block_index,		"short_block_index",	"%i",									0,	nullptr },
			{ Enums::_field_long_block_index,		"long_block_index",		"%i",									0,	nullptr },
			{ Enums::_field_data,					"data",					"size: %i",								0,	nullptr },
			{ Enums::_field_array_start,			"array_start",			nullptr,								0,	nullptr },
			{ Enums::_field_array_end,				"array_end",			nullptr,								0,	nullptr },
			{ Enums::_field_pad,					"pad",					nullptr,								0,	nullptr },
			{ Enums::_field_skip,					"skip",					nullptr,								0,	nullptr },
			{ Enums::_field_explanation,			"explanation",			nullptr,								0,	nullptr },
			{ Enums::_field_custom,					"custom",				nullptr,								0,	nullptr },
			{ Enums::_field_terminator,				"terminator",			nullptr,								0,	nullptr },
		};

		struct s_cache_view_globals
		{
			BOOL m_is_loaded;
			HANDLE m_halo_handle;
			BOOL m_accepted_edit_warning;

			Cache::s_cache_header m_cache_file_header;
			Cache::s_cache_tag_instance* m_cache_tag_instances;
			Cache::s_cache_tag_header m_cache_file_globals;
		};
		static s_cache_view_globals g_cache_view_globals;

		enum{
			k_color_default = Enums::_console_color_white,
			k_color_status_error = Enums::_console_color_lightred,
			k_color_status_success = Enums::_console_color_lightgreen,
			k_color_status_warning = Enums::_console_color_lightyellow,

			k_color_command_line = Enums::_console_color_lightyellow,

			k_color_index = Enums::_console_color_lightgreen,
			k_color_tag = Enums::_console_color_lightpurple,
			k_color_address = Enums::_console_color_lightaqua,
			k_color_block_offset = Enums::_console_color_aqua,
			k_color_name = Enums::_console_color_lightyellow,

			k_color_fieldtype = Enums::_console_color_lightred,
			k_color_block = Enums::_console_color_lightpurple,
			k_color_fieldvalue = Enums::_console_color_white,

			k_status_ok = 0,
			k_status_failed,
			k_status_failed_to_read_runtime_memory,
			k_status_failed_to_write_runtime_memory,
			k_status_failed_to_allocate_local_memory,
			k_status_index_matches,
			k_status_index_changed,

			k_change_index_reloaded,
			
			k_action_end_function,
		};

#include "Tool/RuntimeCacheView.inl"
		
		///////////////////////////////////////////////////////////
		// Main command function
		/*!
		 * \brief
		 * Processes user input.
		 *
		 * Processes user input and initiates the various commands available
		 */
		void PLATFORM_API runtime_cache_view(char* /*arguments*/[])
		{
			// print disclaimer
			Console::ColorPrint(k_color_status_warning,
				"Disclaimer:\n"
				"	This program will directly access Halo's memory and as such\n"
				"	it is possible that should something go awry, your PC may\n"
				"	lock up. So use it at your own risk.\n"
				"\n"
				"	Type 'help' for a list of commands.");

			g_cache_view_globals.m_is_loaded = false;

			auto user_command = _runtime_cache_command_error;
			do
			{
				std::string arguments;
				int index = Console::EnterCommand("quit;load;help;list;open;set;findaddr", &arguments);

				// determine what command has been issued
				switch(index)
				{
				case -1:
					user_command = _runtime_cache_command_error;
					break;
				case 0:
					user_command = _runtime_cache_command_quit;
					break;
				case 1:
					user_command = _runtime_cache_command_load;
					break;
				case 2:
					user_command = _runtime_cache_command_help;
					break;
				case 3:
					{
						// if there are arguments use the filtered list
						if(arguments.length()) 
							user_command = _runtime_cache_command_list_filtered;
						else
							user_command = _runtime_cache_command_list;
					}
					break;
				case 4:
					{
						// open must have an argument
						if(arguments.length()) 
							user_command = _runtime_cache_command_open;
						else
							user_command = _runtime_cache_command_error;
					}
					break;
				case 5:
					{
						// set must have an argument
						if(arguments.length()) 
							user_command = _runtime_cache_command_set;
						else
							user_command = _runtime_cache_command_error;
					}
					break;
				case 6:
					user_command = _runtime_cache_command_find_address;
					break;
				}

				BOOL status = k_status_ok;
				switch(user_command)
				{
				case _runtime_cache_command_error:
					user_command = _runtime_cache_command_error;

					// the command was invalid
					Console::ColorPrint(k_color_status_error, "\ninvalid command"); 
					break;
				case _runtime_cache_command_help:
					{
						user_command = _runtime_cache_command_error;
						PrintHelp();
					}
					break;
				case _runtime_cache_command_load:
					{
						user_command = _runtime_cache_command_error;

						// unloads the current tag index and handle if present
						UnloadTagIndex();
						CloseHandle(g_cache_view_globals.m_halo_handle);
						g_cache_view_globals.m_halo_handle = INVALID_HANDLE_VALUE;

						// find a running instance of Halo CE
						status = GetHaloHandle();
						if(status != k_status_ok)
						{
							PrintStatus(status);
							break;
						}

						// load the tag index from the runtime
						status = LoadTagIndex();
						PrintStatus(status);
					}
					break;
				case _runtime_cache_command_list:
				case _runtime_cache_command_list_filtered:
					{
						// find out if the cache has changed
						// reload it if it has
						status = ReloadCacheCheck();
						if((status != k_status_ok) && (status != k_change_index_reloaded))
						{
							PrintStatus(status);
							user_command = _runtime_cache_command_error;
							break;
						}

						status = PrintTagIndex((user_command == _runtime_cache_command_list_filtered) ? arguments.c_str() : nullptr);
						PrintStatus(status);

						user_command = _runtime_cache_command_error;
					}
					break;
				case _runtime_cache_command_open:
					{
						user_command = _runtime_cache_command_error;

						// find out if the cache has changed
						// reload it if it has
						status = ReloadCacheCheck();
						if(status != k_status_ok)
						{
							PrintStatus(status);
							break;
						}

						status = OpenTag(arguments.c_str());
						PrintStatus(status);
					}
					break;
				case _runtime_cache_command_set:
					{
						user_command = _runtime_cache_command_error;

						// find out if the cache has changed
						// reload it if it has
						status = ReloadCacheCheck();
						if(status != k_status_ok)
						{
							PrintStatus(status);
							break;
						}

						status = ChangeFieldValue(arguments.c_str());
						PrintStatus(status);
					}
					break;
				case _runtime_cache_command_find_address:
					{
						user_command = _runtime_cache_command_error;

						// find out if the cache has changed
						// reload it if it has
						status = ReloadCacheCheck();
						if(status != k_status_ok)
						{
							PrintStatus(status);
							break;
						}

						status = FindFieldByAddress(arguments.c_str());
						PrintStatus(status);
					}
					break;
				};

			}while(user_command != _runtime_cache_command_quit);
			
			UnloadTagIndex();
			CloseHandle(g_cache_view_globals.m_halo_handle);
		}
		///////////////////////////////////////////////////////////
	};
};
#endif