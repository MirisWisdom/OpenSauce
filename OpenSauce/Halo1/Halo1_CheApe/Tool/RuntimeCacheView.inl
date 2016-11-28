/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/

#pragma region Forward declarations
int32 PrintBlock(const uintptr_t address, const Yelo::tag_block_definition* block_definition);
#pragma endregion

#pragma region User interface functions
////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Prints a human friendly string response to a commands status. </summary>
///
/// <param name="status_error">	The status error to print a string for. </param>
void PrintStatus(const int32 status_error)
{
	puts("");
	switch(status_error)
	{
	case k_status_ok:
		Console::ColorPrint(k_color_status_success, "SUCCESS: command completed successfully");
		break;
	case k_change_index_reloaded:
		Console::ColorPrint(k_color_status_warning, "WARNING: the runtime tag index has been reloaded");
		break;
	case k_action_end_function:
		Console::ColorPrint(k_color_status_warning, "WARNING: command ended by the user");
		break;
	case k_status_failed_to_read_runtime_memory:
		Console::ColorPrint(k_color_status_error, "ERROR: failed to read runtime memory");
		break;
	case k_status_failed_to_allocate_local_memory:
		Console::ColorPrint(k_color_status_error, "ERROR: failed to allocate local memory");
		break;
	case k_status_failed:
		Console::ColorPrint(k_color_status_error, "ERROR: command failed");
		break;
	}
}
#pragma endregion

#pragma region Runtime memory I/O
////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Gets the handle of a running instance of Halo. </summary>
///
/// <returns>	The commands end status. </returns>
int32 GetHaloHandle()
{
	// this is mostly refactored from MSDN

	Console::ColorPrint(k_color_default, "enumerating processes...", true);

	// create the initial process array
	DWORD byte_count = 1024 * sizeof(DWORD);
	DWORD* processes = CAST_PTR(DWORD*, malloc(byte_count));

	if(!processes)
		return k_status_failed_to_allocate_local_memory;

	memset(processes, 0, byte_count);

	// create a list of all the current processes
	DWORD bytes_written = 0;
	do
	{
		// if the array was not big enough reallocate a bigger array
		if(bytes_written == byte_count)
		{
			byte_count += 1024;
			processes = CAST_PTR(DWORD*, realloc(processes, byte_count));

			if(!processes)
				return k_status_failed_to_allocate_local_memory;
		}
		// enumerate the systems processes
		if(!EnumProcesses( processes, byte_count, &bytes_written ))
		{
			free(processes);
			return k_status_failed;
		}
	}
	while(bytes_written == byte_count);

	// calculate how many processes were written to the list
	DWORD process_count = bytes_written / sizeof(DWORD);

	Console::ColorPrintF(k_color_default, "%i processes found\n", process_count);
	Console::ColorPrint(k_color_default, "searching for haloce.exe...", true);

	bool found_halo = false;
	// loop through all of the processes
	for (DWORD i = 0; i < process_count; i++ )
	{
		if( processes[i] != 0 )
		{
			// get a handle to the process
			// we need read and write access to get the tag index and write new field values
			HANDLE process = OpenProcess( 
				PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION,
				FALSE,
				processes[i] );

			// get the process name
			if (process)
			{
				HMODULE hMod;
				DWORD modules_byte_needed;

				if ( EnumProcessModules( 
					process, 
					&hMod, 
					sizeof(hMod), 
					&modules_byte_needed) )
				{
					char process_name[MAX_PATH] = "";

					GetModuleBaseName( process, 
						hMod, 
						process_name, 
						sizeof(process_name));

					_strlwr(process_name);

					// determine whether the process is an instance of haloce
					if(!strcmp(process_name, "haloce.exe"))
					{
						g_cache_view_globals.m_halo_handle = process;
						found_halo = true;
						break;
					}
				}
			}

			CloseHandle( process );
		}
	}

	if(found_halo)
		Console::ColorPrint(k_color_default, "haloce.exe process found!", true);
	else
		Console::ColorPrint(k_color_default, "unable to find a running instance of haloce.exe", true);

	free(processes);

	return found_halo ? k_status_ok : k_status_failed;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Reads a number of bytes from an address in Halos memory. </summary>
///
/// <param name="address">		   	The memory address to read from. </param>
/// <param name="destination">	   	[in] The buffer to read to. </param>
/// <param name="destination_size">	Size of the destination. </param>
///
/// <returns>	The commands end status. </returns>
int32 ReadHaloMemory(const void* address, void* destination, const size_t destination_size)
{
	SIZE_T bytes_read;
	BOOL success = ReadProcessMemory(g_cache_view_globals.m_halo_handle, 
		address, 
		destination, 
		destination_size, 
		&bytes_read);

	if(!success || (bytes_read != destination_size))
		return k_status_failed_to_read_runtime_memory;
	return k_status_ok; // TODO: k_status_ok is '0'...yet this function returns 'BOOL'?
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Writes a number of bytes to Halos memory. </summary>
///
/// <param name="destination">	[in] The destination address in Halo. </param>
/// <param name="source">	  	[in] The source buffer to write. </param>
/// <param name="size">		  	The number of bytes to write. </param>
///
/// <returns>
/// 	k_status_ok if successful, otherwise k_status_failed_to_write_runtime_memory.
/// </returns>
int32 WriteHaloMemory(void* destination, void* source, const size_t size)
{
	SIZE_T bytes_written;
	BOOL success = WriteProcessMemory(g_cache_view_globals.m_halo_handle, 
		destination, 
		source, 
		size, 
		&bytes_written);

	if(!success || (bytes_written != size))
		return k_status_failed_to_write_runtime_memory;
	return k_status_ok; // TODO: k_status_ok is '0'...yet this function returns 'BOOL'?
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Reads a NULL terminated string from Halos memory. </summary>
///
/// <remarks>
/// 	The memory allocated for "destination" must be deleted by the calling application.
/// </remarks>
///
/// <param name="address">	  	The start address of the string. </param>
/// <param name="destination">
/// 	[out] A char*&amp; reference that will be allocated with memory for the string.
/// </param>
///
/// <returns>	The commands end status. </returns>
int32 ReadHaloString(const cstring address, cstring& destination)
{
	// allocate the buffer
	int buffer_size = 2048;
	char* buffer = CAST_PTR(char*, malloc(buffer_size));

	if(!buffer)
		return k_status_failed_to_allocate_local_memory;

	int32 status = k_status_ok;
	// read the string
	int string_index = 0;
	do
	{
		// if the string is longer than the current buffer, allocate a larger buffer
		if(string_index == buffer_size)
		{
			buffer_size += 2048;
			char* new_buffer = CAST_PTR(char*, realloc(buffer, buffer_size));

			if(!new_buffer)
			{
				free(buffer);
				return k_status_failed_to_allocate_local_memory;
			}

			buffer = new_buffer;
		}

		// read a single character from halos memory
		status &= ReadHaloMemory((void*)(address + string_index),
			&buffer[string_index],
			1);

		if(status != k_status_ok)
		{
			free(buffer);
			return status;
		}

		// if the character was NULL the end of the string has been found
	}while(buffer[string_index++] && (status == k_status_ok));
	
	if(status == k_status_ok)
	{
		// allocate memory for the string
		int string_length = strlen(buffer);
		destination = new char[string_length + 1];
		
		if(!destination)
		{
			free(buffer);
			return k_status_failed_to_allocate_local_memory;
		}

		// copy the string to the destination
		strcpy_s((char*)destination, string_length + 1, buffer);
	}

	// delete the temporary buffer
	free(buffer);

	return status;
}
#pragma endregion

#pragma region Halo cache functions
////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Creates a local copy of Halos tag index and tag instance array. </summary>
///
/// <returns>	The commands end status. </returns>
int32 LoadTagIndex()
{
	Console::ColorPrint(k_color_default, "\nreading cache file globals...", true);

	// read the cache header
	auto status = ReadHaloMemory(k_runtime_110_cache_header_ptr, 
		&g_cache_view_globals.m_cache_file_header, 
		sizeof(Cache::s_cache_header));

	if(status != k_status_ok)
		return status;

	// read the variable containing the cache file globals pointer
	void* cache_tag_header_ptr;

	status = ReadHaloMemory(k_runtime_110_cache_globals_ptr, 
		&cache_tag_header_ptr, 
		sizeof(cache_tag_header_ptr));

	if(status != k_status_ok)
		return status;

	// copy cache globals into local memory
	status = ReadHaloMemory(cache_tag_header_ptr, 
		&g_cache_view_globals.m_cache_file_globals, 
		sizeof(g_cache_view_globals.m_cache_file_globals));

	if(status != k_status_ok)
		return status;

	Console::ColorPrint(k_color_default, "successfully read cache file globals", true);

	Console::ColorPrint(k_color_default, "reading tag instance array...", true);

	// allocate memory for a local copy of the tag instance array
	g_cache_view_globals.m_cache_tag_instances = new Cache::s_cache_tag_instance[g_cache_view_globals.m_cache_file_globals.count];

	// copy the tag instance array into local memory
	// since the tag instance array in the cache file globals has no defined elements, using
	// the g_cache_view_globals.m_cache_file_globals size to get to the top of the array works
	status = ReadHaloMemory(CAST_PTR(void*, ((uint32)cache_tag_header_ptr) + sizeof(g_cache_view_globals.m_cache_file_globals)), 
		g_cache_view_globals.m_cache_tag_instances, 
		sizeof(Cache::s_cache_tag_instance) * g_cache_view_globals.m_cache_file_globals.count);

	if(status != k_status_ok)
	{
		delete [] g_cache_view_globals.m_cache_tag_instances;
		g_cache_view_globals.m_cache_tag_instances = nullptr;
		return status;
	}

	// since this is a local copy of the tag instance array we can do some processing on it
	// so that we don't have to later
	for(int i = 0; i < g_cache_view_globals.m_cache_file_globals.count; i++)
	{
		auto& tag_instance = g_cache_view_globals.m_cache_tag_instances[i];

		// swap the tag groups now for later printing
		TagGroups::TagSwap(tag_instance.group_tag);
		TagGroups::TagSwap(tag_instance.parent_groups[0]);
		TagGroups::TagSwap(tag_instance.parent_groups[1]);

		// copy the tag name strings into local memory
		status = ReadHaloString(tag_instance.name, tag_instance.name);

		if(status != k_status_ok)
			return status;
	}
	Console::ColorPrint(k_color_default, "tag index read complete\n", true);

	// print some info about the cache
	Console::ColorPrint(k_color_command_line, "tag instance count:\t");
	Console::ColorPrintF(k_color_index, "%i\n", g_cache_view_globals.m_cache_file_globals.count);
	Console::ColorPrint(k_color_command_line, "tag data address:\t");
	Console::ColorPrintF(k_color_address, "0x%08X\n", g_cache_view_globals.m_cache_file_globals.tags_address);

	g_cache_view_globals.m_is_loaded = true;

	return k_status_ok;
}

/// <summary>	Deletes the local copy of the runtime tag index. </summary>
void UnloadTagIndex()
{
	if(!g_cache_view_globals.m_is_loaded)
		return;

	Console::ColorPrint(k_color_default, "deleting local tag index", true);

	// delete the tag names
	for(int i = 0; i < g_cache_view_globals.m_cache_file_globals.count; i++)
		delete [] g_cache_view_globals.m_cache_tag_instances[i].name;
	// delete the tag instances
	delete [] g_cache_view_globals.m_cache_tag_instances;
	g_cache_view_globals.m_cache_tag_instances = nullptr;

	// reset globals
	memset(&g_cache_view_globals.m_cache_file_globals, 0, sizeof(g_cache_view_globals.m_cache_file_globals));

	g_cache_view_globals.m_is_loaded = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Determines if the runtime cache has changed. </summary>
///
/// <remarks>
/// 	Creates a temporary copy of the runtime's cache header and compares the crc with the
/// 	local copy.
/// </remarks>
///
/// <returns>
/// 	k_status_index_changed if the cache is different otherwise k_status_index_matches. If a
/// 	read error occurs a read error status will be returned.
/// </returns>
int32 HasCacheChanged()
{
	Console::ColorPrint(k_color_default, "seeing if the cache has changed", true);	

	Cache::s_cache_header comparison_header;

	// read the current cache header 
	auto status = ReadHaloMemory(k_runtime_110_cache_header_ptr, 
		&comparison_header, 
		sizeof(Cache::s_cache_header));

	if(status != k_status_ok)
		return status;

	// Compare the cache crc's
	return (g_cache_view_globals.m_cache_file_header.crc != comparison_header.crc) ? k_status_index_changed : k_status_index_matches;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Finds out if the cache has changed and reloads the index if it has. </summary>
///
/// <returns>
/// 	k_change_index_reloaded if the index is reloaded, otherwise k_status_ok if nothings
/// 	changed.
/// </returns>
int32 ReloadCacheCheck()
{
	// has the cache changed
	auto status = HasCacheChanged();
	if(status == k_status_index_changed)
	{
		Console::ColorPrint(k_color_default, "different map cache present", true);
		// unload the current index
		UnloadTagIndex();
		// load the new index
		status = LoadTagIndex();
		status = (status != k_status_ok) ? status : k_change_index_reloaded;
	}
	return (status == k_status_index_matches ? k_status_ok : status);
}
#pragma endregion

#pragma region Edit command functions
////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// 	Informs the user that bad things can happen if they mess with the runtime memory
/// 	incorrectly.
/// </summary>
///
/// <remarks>
/// 	The user must accept responsibility before continuing. They will only be asked this once
/// 	per session.
/// </remarks>
void DisplayEditWarning()
{
	if(g_cache_view_globals.m_accepted_edit_warning)
		return;

	puts("");
	Console::ColorPrint(k_color_status_warning, "manually writing to Halo's memory can cause unexpected results\nif incorrect values are entered.", true);

	int answer = Console::EnterCommand("y;n", nullptr, "do you want to continue (y/n)?");

	g_cache_view_globals.m_accepted_edit_warning = (answer == 0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// 	Parses a string of user defined values and writes them to a memory address in the runtime.
/// </summary>
///
/// <param name="desc_index">
/// 	The field description index of the type at the memory address.
/// </param>
/// <param name="address">	   	The runtime memory address to write to. </param>
/// <param name="value_string">	A string containing the new value(s). </param>
///
/// <returns>	The status of the command. </returns>
int32 WriteField(const int desc_index, const void* address, const char* value_string)
{
	union {
		byte					_byte;
		int16					_short;
		int32					_int;
		uint16					_ushort;
		uint32					_uint;
		real					_real;
		real_vector2d			_real2;
		real_vector3d			_real3;
		real_quaternion			_real4;
		rgb_color				_rgb_color;
		argb_color				_argb_color;
		tag						_tag;
		tag_string				_tag_string;
		point2d					_point_2d;
		rectangle2d				_rectangle_2d;
		short_bounds			_short_bounds;
	}value;

	auto& field_descriptor = g_field_descriptions[desc_index];

	// get the formatting string to parse the users arguments
	int count = 0;
	const char* scan_format = field_descriptor.m_scan_format;

	// parse the users arguments
	switch(field_descriptor.m_field_type)
	{
	case Enums::_field_byte_flags:
	case Enums::_field_char_integer:
		count = sscanf_s(value_string, scan_format,
			&value._byte);
		break;

	case Enums::_field_short_integer:
		count = sscanf_s(value_string, scan_format,
			&value._short);
		break;

	case Enums::_field_enum:
	case Enums::_field_word_flags:
		count = sscanf_s(value_string, scan_format,
			&value._ushort);
		break;

	case Enums::_field_long_integer:
		count = sscanf_s(value_string, scan_format,
			&value._int);
		break;

	case Enums::_field_long_flags:
		count = sscanf_s(value_string, scan_format,
			&value._uint);
		break;

	case Enums::_field_angle:
	case Enums::_field_real_fraction:
	case Enums::_field_real:
		count = sscanf_s(value_string, scan_format,
			&value._real);
		break;

	case Enums::_field_real_fraction_bounds:
	case Enums::_field_real_bounds:
	case Enums::_field_angle_bounds:
	case Enums::_field_real_euler_angles_2d:
	case Enums::_field_real_point_2d:
	case Enums::_field_real_vector_2d:
		count = sscanf_s(value_string, scan_format,
			&value._real2.i,
			&value._real2.j);
		break;

	case Enums::_field_real_hsv_color:
	case Enums::_field_real_rgb_color:
	case Enums::_field_real_plane_2d:
	case Enums::_field_real_euler_angles_3d:
	case Enums::_field_real_point_3d:
	case Enums::_field_real_vector_3d:
		count = sscanf_s(value_string, scan_format,
			&value._real3.i,
			&value._real3.j,
			&value._real3.k);
		break;

	case Enums::_field_real_ahsv_color:
	case Enums::_field_real_argb_color:
	case Enums::_field_real_plane_3d:
	case Enums::_field_real_quaternion:
		count = sscanf_s(value_string, scan_format,
			&value._real4.i,
			&value._real4.j,
			&value._real4.k,
			&value._real4.w);
		break;

	case Enums::_field_short_bounds:
	case Enums::_field_point_2d:
		count = sscanf_s(value_string, scan_format,
			&value._point_2d.x, 
			&value._point_2d.y);
		break;

	case Enums::_field_rgb_color:
		count = sscanf_s(value_string, scan_format,
			&value._rgb_color.red,
			&value._rgb_color.green,
			&value._rgb_color.blue);
		break;

	case Enums::_field_argb_color:
		count = sscanf_s(value_string, scan_format,
			&value._argb_color.alpha,
			&value._argb_color.red,
			&value._argb_color.green,
			&value._argb_color.blue);
		break;

	case Enums::_field_rectangle_2d:
		count = sscanf_s(value_string, scan_format,
			&value._rectangle_2d.top,
			&value._rectangle_2d.bottom,
			&value._rectangle_2d.left,
			&value._rectangle_2d.right);
		break;

	case Enums::_field_string:
		count = sscanf_s(value_string, scan_format,
			&value._tag_string);
		break;

	case Enums::_field_tag:
		{
			TagGroups::group_tag_to_string tag_group;

			count = sscanf_s(value_string, scan_format,
				&tag_group.str, 4);

			value._tag = tag_group.Terminate().TagSwap().group;
		}
		break;
	}

	// if the user did not specify the right about of arguments, the command fails
	if(count != field_descriptor.m_scan_count)
	{
		Console::ColorPrint(k_color_command_line, "incorrent number of arguments supplied", true);
		return k_status_failed;
	}

	// write the new values to the runtime's memory
	return WriteHaloMemory((void*)address, &value, field_descriptor.Definition().size);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Gets the users command details and passes them to WriteField. </summary>
///
/// <param name="arguments">	String containing the users command arguments. </param>
///
/// <returns>	The commands end status. </returns>
int32 ChangeFieldValue(const char* arguments)
{
	// warn of unexpected consequences
	DisplayEditWarning();
	if(!g_cache_view_globals.m_accepted_edit_warning)
		return k_status_ok;

	int count = 0;
	std::string arguments_string(arguments);
	std::string argument;

	// get the address string
	StringEditing::GetStringSegment(arguments_string, argument, nullptr, " ");

	// parse the address
	void* address;
	count = sscanf_s(argument.c_str(), "0x%08x", &address);
	if(!count)
		return k_status_failed;

	// remove the address from the arguments string
	StringEditing::RemoveStringSegment(arguments_string, nullptr, " ");
	// get the type id string
	StringEditing::GetStringSegment(arguments_string, argument, nullptr, " ");

	// get the description index for the requested type
	int index = 0;
	for(; index < NUMBEROF(g_field_descriptions); index++)
		if(!strcmp(g_field_descriptions[index].m_field_type_name, argument.c_str()))
			break;

	// if no matching type was found or the type is not valid, return
	if(index == NUMBEROF(g_field_descriptions) || !g_field_descriptions[index].Editable())
	{
		puts("");
		Console::ColorPrint(k_color_status_error, "invalid field type");
		return k_status_failed;
	}

	// remove the type id from the arguments string
	StringEditing::RemoveStringSegment(arguments_string, nullptr, " ");

	// set the new values
	return WriteField(index, address, arguments_string.c_str());
}
#pragma endregion

#pragma region Print tag command functions
////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Gets the size of a field in bytes. </summary>
///
/// <param name="field">	The field. </param>
///
/// <returns>	The field size in bytes. </returns>
int32 GetFieldSize(const Yelo::tag_field* field)
{
	switch(field->type)
	{
	case Enums::_field_pad:
	case Enums::_field_skip:
		return field->DefinitionCast<int32>();
	default:
		return g_field_descriptions[field->type].Definition().size;
	};
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Prints a fields value to the console. </summary>
///
/// <param name="field_data">	   	The local memory address of the field. </param>
/// <param name="field_definition">	The fields definition. </param>
void PrintFieldValue(const void* field_data, const Yelo::tag_field* field_definition)
{
	union {
		byte*					_byte;
		int16*					_short;
		int32*					_int;
		uint16*					_ushort;
		uint32*					_uint;
		real*					_real;
		real_vector2d*			_real2;
		real_vector3d*			_real3;
		real_quaternion*		_real4;
		rgb_color*				_rgb_color;
		argb_color*				_argb_color;
		tag*					_tag;
		tag_string*				_tag_string;
		point2d*				_point_2d;
		rectangle2d*			_rectangle_2d;
		short_bounds*			_short_bounds;
		tag_reference*			_tag_reference;
		tag_block*				_tag_block;
		tag_data*				_tag_data;

		const void*					pointer;
	}value;

	value.pointer = field_data;
	
	// print the fields value
	Console::ColorPrint(k_color_default, "  value:\t");

	const char* format = g_field_descriptions[field_definition->type].m_format_string;
	switch(field_definition->type)
	{
	case Enums::_field_byte_flags:
	case Enums::_field_char_integer:
		Console::ColorPrintF(k_color_fieldvalue, format,
			*value._byte);
		break;

	case Enums::_field_short_block_index:
	case Enums::_field_short_integer:
		Console::ColorPrintF(k_color_fieldvalue, format,
			*value._short);
		break;

	case Enums::_field_enum:
	case Enums::_field_word_flags:
		Console::ColorPrintF(k_color_fieldvalue, format,
			*value._ushort);
		break;

	case Enums::_field_long_block_index:
	case Enums::_field_long_integer:
		Console::ColorPrintF(k_color_fieldvalue, format,
			*value._int);
		break;

	case Enums::_field_long_flags:
		Console::ColorPrintF(k_color_fieldvalue, format,
			*value._uint);
		break;

	case Enums::_field_angle:
	case Enums::_field_real_fraction:
	case Enums::_field_real:
		Console::ColorPrintF(k_color_fieldvalue, format,
			*value._real);
		break;

	case Enums::_field_real_fraction_bounds:
	case Enums::_field_real_bounds:
	case Enums::_field_angle_bounds:
	case Enums::_field_real_euler_angles_2d:
	case Enums::_field_real_point_2d:
	case Enums::_field_real_vector_2d:
		Console::ColorPrintF(k_color_fieldvalue, format,
			value._real2->i,
			value._real2->j);
		break;

	case Enums::_field_real_hsv_color:
	case Enums::_field_real_rgb_color:
	case Enums::_field_real_plane_2d:
	case Enums::_field_real_euler_angles_3d:
	case Enums::_field_real_point_3d:
	case Enums::_field_real_vector_3d:
		Console::ColorPrintF(k_color_fieldvalue, format,
			value._real3->i,
			value._real3->j,
			value._real3->k);
		break;

	case Enums::_field_real_ahsv_color:
	case Enums::_field_real_argb_color:
	case Enums::_field_real_plane_3d:
	case Enums::_field_real_quaternion:
		Console::ColorPrintF(k_color_fieldvalue, format,
			value._real4->i,
			value._real4->j,
			value._real4->k,
			value._real4->w);
		break;

	case Enums::_field_short_bounds:
	case Enums::_field_point_2d:
		Console::ColorPrintF(k_color_fieldvalue, format,
			value._point_2d->x, 
			value._point_2d->y);
		break;

	case Enums::_field_rgb_color:
		Console::ColorPrintF(k_color_fieldvalue, format,
			value._rgb_color->red,
			value._rgb_color->green,
			value._rgb_color->blue);
		break;

	case Enums::_field_argb_color:
		Console::ColorPrintF(k_color_fieldvalue, format,
			value._argb_color->alpha,
			value._argb_color->red,
			value._argb_color->green,
			value._argb_color->blue);
		break;

	case Enums::_field_rectangle_2d:
		Console::ColorPrintF(k_color_fieldvalue, format,
			value._rectangle_2d->top,
			value._rectangle_2d->bottom,
			value._rectangle_2d->left,
			value._rectangle_2d->right);
		break;

	case Enums::_field_string:
		Console::ColorPrintF(k_color_fieldvalue, format,
			*value._tag_string);
		break;

	case Enums::_field_tag:
		{
			TagGroups::group_tag_to_string tag_group;
			tag_group.group = *value._tag;
			tag_group.Terminate();
			tag_group.TagSwap();

			Console::ColorPrintF(k_color_fieldvalue, format, tag_group.str);
		}
		break;

	case Enums::_field_tag_reference:
		{
			// TODO: add support for string_id_yelo

			Console::ColorPrintF(k_color_fieldvalue, format,
				TagGroups::group_tag_to_string{ value._tag_reference->group_tag }.ToString(),
				value._tag_reference->tag_index.index);
		}
		break;

	case Enums::_field_block:
		Console::ColorPrintF(k_color_fieldvalue, format,
			value._tag_block->count,
			value._tag_block->address);
		break;

	case Enums::_field_data:
		Console::ColorPrintF(k_color_fieldvalue, format,
			value._tag_data->size);
		break;

	case Enums::_field_pad:
	case Enums::_field_skip:
		{
			// print each byte of a pad/skip separately, breaking at each 4 bytes
			// and adding a new line after 4 quads
			int pad_length = field_definition->DefinitionCast<int32>();
			int byte_count = 0;
			int quad_count = 0;

			char* data = (char*)field_data;
			for(int i = 0; i < pad_length; i++)
			{
				if(byte_count == 4)
				{
					putc(' ', stdout);
					byte_count = 0;
					quad_count++;
				}
				if(quad_count == 4)
				{
					fputs("\n\t\t", stdout);
					quad_count = 0;
					byte_count = 0;
				}

				Console::ColorPrintF(k_color_fieldvalue, "%02X", data[i]);
				byte_count++;
			}
		}
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Print information about a field to the console. </summary>
///
/// <param name="field_name">  	Name of the field. </param>
/// <param name="tag_data">	   	The local tag data address for the field. </param>
/// <param name="address">	   	The address of the field in Halo's memory. </param>
/// <param name="block_offset">	The fields block offset. </param>
/// <param name="field">	   	The field description. </param>
void PrintFieldInfo(const std::string& field_name, const void* tag_data, const uintptr_t address, const int32 block_offset, const Yelo::tag_field* field)
{
	auto& field_descriptor = g_field_descriptions[field->type];

	// Print the address of the field in Halo
	Console::ColorPrintF(k_color_address, "0x%08X", address);
	// Print the block offset of the field in Halo
	Console::ColorPrintF(k_color_block_offset, " 0x%04X", block_offset);
	// Print the field type
	Console::ColorPrint(k_color_default, " [");
	Console::ColorPrintF(k_color_fieldtype, "%s", field_descriptor.m_field_type_name);
	Console::ColorPrintF(k_color_default, "]%*s", 26 - strlen(field_descriptor.m_field_type_name), " ");

	Console::ColorPrint(k_color_name, field_name.c_str(), true);

	// Print the fields value
	PrintFieldValue(tag_data, field);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Print's information about a block field. </summary>
///
/// <param name="field_name">	Name of the field. </param>
/// <param name="tag_data">  	The local tag data address for the field. </param>
/// <param name="field">	 	The field description. </param>
void PrintBlockInfo(const std::string& field_name, const void* tag_data, const Yelo::tag_field* field)
{	
	auto definition = field->Definition<tag_block_definition>();

	// Print the block fields name and type
	Console::ColorPrint(k_color_block, "block field:\t\t");
	Console::ColorPrint(k_color_name, field_name.c_str(), true);
	Console::ColorPrint(k_color_block, "block definition:\t");
	Console::ColorPrintF(k_color_block, definition->name, true);

	// Print the fields value
	PrintFieldValue(tag_data, field);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Prints the fields of a tag block to the console. </summary>
///
/// <param name="tag_data">	   	[in,out] The local memory pointer where the block resides. </param>
/// <param name="address">	   	[in,out] The memory address of the block in Halo's memory. </param>
/// <param name="block_offset">	[in,out] The fields block offset. </param>
/// <param name="start_field"> 	The start field descriptor. </param>
///
/// <returns>	The commands end status. </returns>
int32 PrintFields(byte*& tag_data, uintptr_t& address, int32& block_offset, const Yelo::tag_field* start_field)
{
	int count = 0;
	const Yelo::tag_field* current = start_field;

	// enumerate through all of the blocks fields
	while(current->type != Enums::_field_terminator)
	{
		switch(current->type)
		{
		case Enums::_field_array_start:
			{
				int array_count = current->DefinitionCast<int32>();

				// store the first field in the array list
				const tag_field* first_array_field = current + 1;

				// print the array
				for(int i = 0; i < array_count; i++)
				{
					auto status = PrintFields(tag_data, address, block_offset, first_array_field);
					if(status != k_status_ok)
						return status;
				}

				// move the current field pointer past the array definition
				while(current->type != Enums::_field_array_end)
					current++;
				current++;
				continue;
			}
		case Enums::_field_array_end:
			// should only be reached when called from the array_start code so return
			return k_status_ok;
		case Enums::_field_explanation:
		case Enums::_field_custom:
			current++;
			continue;
		};

		// Get the field name
		std::string field_name("");
		if(current->name)
		{
			field_name.assign(current->name);
			StringEditing::GetStringSegment(field_name, field_name, nullptr, "^:#*");
		}

		puts("");

		PrintFieldInfo(field_name, tag_data, address, block_offset, current);

		// if the field is a tag block, print all of its elements
		if(current->type == Enums::_field_block)
		{
			auto block = CAST_PTR(tag_block*, tag_data);
			auto definition = current->Definition<tag_block_definition>();
			block->definition = definition; // set the block view's definition so we can use our tag APIs on it

			if(block->count > 0)
			{
				// clearly separate the block from the parent tag fields
				puts("\n");

				// print the block fields name and type
				PrintBlockInfo(field_name, tag_data, current);

				// print the block elements
				for(auto element : *block)
				{
					puts("");
					Console::ColorPrint(k_color_name, field_name.c_str());
					Console::ColorPrintF(k_color_block, " element %i:\n", element.index);

					auto status = PrintBlock(CAST_PTR(uintptr_t, element.address), definition);
					if(status != k_status_ok)
						return status;

					puts("");
					Console::ColorPrint(k_color_name, field_name.c_str());
					Console::ColorPrintF(k_color_block, " element %i end\n", element.index);

					// if the user enters s, skip past the rest of the blocks
					// if the user enters q, close the tag
					puts("");
				
					Console::ColorPrint(k_color_command_line, "press enter to continue, \"s\" to skip this block type, or \"q\" to quit");

					int command = Console::EnterCommand("q;s;\n", nullptr);

					if(command == 0)
						return k_action_end_function;
					else if (command == 1)
						break;

					count = 0;
				}

				puts("");
				Console::ColorPrint(k_color_block, "block field end:\t");
				Console::ColorPrint(k_color_name, field_name.c_str(), true);
			}
		}
		
		// increment the address'
		size_t field_size = GetFieldSize(current);
		address += field_size;
		tag_data +=  field_size;
		block_offset += field_size;

		current++;
		count++;

		// if 20 fields have been listed in a row, pause until the user wants to continue
		if(count == 20)
		{
			puts("");
			Console::ColorPrint(k_color_command_line, "press enter to continue or \"q\" to quit");
			int command = Console::EnterCommand("q;\n", nullptr);

			if(command == 0)
				return k_action_end_function;

			count = 0;
		}
	}
	return k_status_ok;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Prints a tag block to the console. </summary>
///
/// <param name="address">		   	The address in Halo's memory where the block resides. </param>
/// <param name="block_definition">
/// 	The tag block definition that describes the blocks contents.
/// </param>
///
/// <returns>	The commands end status. </returns>
int32 PrintBlock(uintptr_t address, const Yelo::tag_block_definition* block_definition)
{
	// copy the block into local memory
	auto block_data = YELO_NEW_ARRAY_UNIQUE(byte, block_definition->element_size);
	byte* data_pointer = block_data.get();

	auto status = ReadHaloMemory((void*)address, data_pointer, block_definition->element_size);

	int32 block_offset = 0;

	// print the blocks fields to the console
	if(status == k_status_ok)
		status = PrintFields(data_pointer, address, block_offset, block_definition->fields);

	return status;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Reads a tag from Halos cache and prints its fields. </summary>
///
/// <param name="arguments">	A string containing the user's arguments. </param>
///
/// <returns>	The commands end status. </returns>
int32 OpenTag(const char* arguments)
{
	// read the index from the arguments string
	int32 index;
	int count = sscanf_s(arguments, "%i", &index);

	// if no index was read, return
	if(count == 0)
	{
		Console::ColorPrint(k_color_status_error, "invalid tag index", true);
		return k_status_failed;
	}

	//if the index is out of bounds, return
	if((index < 0) || (index > g_cache_view_globals.m_cache_file_globals.count))
	{
		Console::ColorPrint(k_color_status_error, "tag index is out of bounds", true);
		return k_status_failed;
	}

	auto& tag_instance = g_cache_view_globals.m_cache_tag_instances[index];

	// get the tag group definition for the tag type
	TagGroups::group_tag_to_string tag_group = { tag_instance.group_tag };
	tag_group.Terminate().TagSwap();

	Yelo::tag_group* tag_group_def = blam::tag_group_get(tag_group.group);

	if(!tag_group_def)
	{
		Console::ColorPrint(k_color_status_error, "invalid tag group definition", true);
		return k_status_failed;
	}

	// modify the tag name string to fit onto the command line
	std::string tag_name(tag_instance.name);
	StringEditing::StringTruncate(tag_name, __max(Console::GetConsoleWidth() - 10, 0), 10);

	// print some interesting bits of data
	Console::ColorPrintF(k_color_block, "tag type: %s\n", tag_group_def->name);
	Console::ColorPrint(k_color_block, "tag name:\n");
	Console::ColorPrintF(k_color_name, "\t%s\n\n", tag_name.c_str());

	// get the root block definition of the tag
	Yelo::tag_block_definition* root_definition = tag_group_def->header_block_definition;

	// print the tags contents
	return PrintBlock(CAST_PTR(uintptr_t, tag_instance.base_address), root_definition);
}
#pragma endregion

#pragma region Print tag index functions
////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
/// 	Prints the tag instance index to the command line. Can optionally filter a specific tag
/// 	group type.
/// </summary>
///
/// <param name="filter">	A tag group string to filter out specific tag types. </param>
///
/// <returns>	An int32. </returns>
int32 PrintTagIndex(const char* filter)
{
	puts("");
	// loop through all of the tag instances
	int count = 0;
	for(int i = 0; i < g_cache_view_globals.m_cache_file_globals.count; i++)
	{
		auto& tag_instance = g_cache_view_globals.m_cache_tag_instances[i];

		TagGroups::group_tag_to_string tags[3];
		tags[0].group = tag_instance.group_tag;
		tags[0].Terminate();
		tags[1].group = tag_instance.parent_groups[0];
		tags[1].Terminate();
		tags[2].group = tag_instance.parent_groups[1];
		tags[2].Terminate();

		// if a filter is used and this tag doesn't match, skip it
		if(filter != nullptr && strcmp(filter, tags[0].str))
			continue;

		// modify the tag name to fit on the command line
		std::string tag_name(tag_instance.name);
		StringEditing::StringTruncate(tag_name, __max(Console::GetConsoleWidth() - 10, 0), 10);

		// print the tag instance information
		Console::ColorPrintF(k_color_index, "%i", tag_instance.GetAbsoluteIndex());
		Console::ColorPrint(k_color_default, ":\t[");
		Console::ColorPrintF(k_color_tag, "%s", tags[0].str);
		Console::ColorPrint(k_color_default, ":");
		Console::ColorPrintF(k_color_tag, "%s", tags[1].str);
		Console::ColorPrint(k_color_default, ":");
		Console::ColorPrintF(k_color_tag, "%s", tags[2].str);
		Console::ColorPrint(k_color_default, "] : ");
		Console::ColorPrintF(k_color_address, "0x%08X\n", tag_instance.base_address);
		Console::ColorPrintF(k_color_name, "\t%s\n", tag_name.c_str());

		count++;

		// if 20 tags have been listed, pause until the user wants to continue
		if(count == 20)
		{
			puts("");
			Console::ColorPrint(k_color_command_line, "press enter to continue, or \"q\" to quit", true);
			int command = Console::EnterCommand("q;\n", nullptr);
			if(command == 0)
				return k_action_end_function;
			count = 0;
		}
	}
	return k_status_ok;
}
#pragma endregion

#pragma region Find by address functions
struct s_field_tree_entry
{
	const uintptr_t field_address;
	const uint32 field_block_offset;
	const tag_field* field_definition;
	const tag_block_definition* block_definition;
	const int32 block_index;
};

int32 FindInBlock(std::vector<s_field_tree_entry>& field_tree, const uintptr_t test_address, uintptr_t address, const Yelo::tag_block_definition* block_definition);

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Searches for the test address in a list of field's. </summary>
///
/// <param name="field_tree">  	[in,out] The field tree to add to if the address is found. </param>
/// <param name="block_offset">	[in,out] The fields block offset. </param>
/// <param name="test_address">	The test address to find. </param>
/// <param name="tag_data">	   	[in,out] The local copy of the block data. </param>
/// <param name="address">	   	[in,out] The current fields address in Halo's memory. </param>
/// <param name="start_field"> 	The start field descriptor. </param>
///
/// <returns>
/// 	k_status_ok if the field was found, otherwise a non-zero code if not found or an error
/// 	occurs.
/// </returns>
int32 FindField(std::vector<s_field_tree_entry>& field_tree, int32& block_offset, const uintptr_t test_address, byte*& tag_data, uintptr_t& address, const Yelo::tag_field* start_field)
{
	const Yelo::tag_field* current = start_field;

	// Enumerate through all of the blocks fields
	while(current->type != Enums::_field_terminator)
	{
		// Get the size of the field
		size_t field_size = GetFieldSize(current);

		// Loop through array types and skip non-value types
		switch(current->type)
		{
		case Enums::_field_array_start:
			{
				int array_count = current->DefinitionCast<int32>();
				const tag_field* first_array_field = current + 1;
				for(int i = 0; i < array_count; i++)
				{
					auto status = FindField(field_tree, block_offset, test_address, tag_data, address, first_array_field);
					if(status == k_status_ok)
					{
						return status;
					}
				}
				// Move the current field pointer past the array definition
				while(current->type != Enums::_field_array_end)
				{
					current++;
				}
				current++;
				continue;
			}
		case Enums::_field_array_end:
			return k_status_failed;
		case Enums::_field_explanation:
		case Enums::_field_custom:
			current++;
			continue;
		};

		// Test whether the address is part of the current field
		if((test_address >= address) && (test_address < address + field_size))
		{
			// Add a tree entry for the field
			s_field_tree_entry path_entry { address, block_offset, current, nullptr, -1 };
			field_tree.push_back(path_entry);
			return k_status_ok;
		}

		// If the field is a data field, test whether the address is within the fields data array
		if(current->type == Enums::_field_data)
		{
			auto data = CAST_PTR(Yelo::tag_data*, tag_data);
			if((test_address >= CAST_PTR(uintptr_t, data->address)) && (test_address < CAST_PTR(uintptr_t, data->address) + data->size))
			{
				// Add a tree entry for the field
				s_field_tree_entry path_entry { address, block_offset, current, nullptr, -1 };
				field_tree.push_back(path_entry);
				return k_status_ok;
			}
		}

		// If the field is a tag block, test if the address is within the blocks data
		if(current->type == Enums::_field_block)
		{
			auto block = CAST_PTR(Yelo::tag_block*, tag_data);
			auto definition = current->Definition<tag_block_definition>();
			block->definition = definition;
			
			for(auto element : *block)
			{
				// Find the field in the block
				auto status = FindInBlock(field_tree, test_address, CAST_PTR(uintptr_t, element.address), definition);
				if(status == k_status_ok)
				{
					// Add a tree entry for the block
					s_field_tree_entry path_entry { address, 0, current, definition, element.index };
					field_tree.push_back(path_entry);
					return status;
				}
			}
		}

		address += field_size;
		tag_data += field_size;
		block_offset += field_size;
		current++;
	}
	return k_status_failed;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Searches for the field at an address within a block. </summary>
///
/// <param name="field_tree">
/// 	[in,out] The field tree to add to if the address is found.
/// </param>
/// <param name="test_address">	   	The test address to find. </param>
/// <param name="address">		   	The block's address in Halo's memory. </param>
/// <param name="block_definition">	The block definition. </param>
///
/// <returns>
/// 	k_status_ok if the field was found, otherwise a non-zero code if not found or an error
/// 	occurs.
/// </returns>
int32 FindInBlock(std::vector<s_field_tree_entry>& field_tree, const uintptr_t test_address, uintptr_t address, const Yelo::tag_block_definition* block_definition)
{
	// Copy the block into local memory
	auto block_data = YELO_NEW_ARRAY_UNIQUE(byte, block_definition->element_size);
	byte* data_pointer = block_data.get();

	auto status = ReadHaloMemory((void*)address, data_pointer, block_definition->element_size);
	if(status != k_status_ok)
	{
		return k_status_failed;
	}

	int32 block_offset = 0;
	// Find the field address in the block
	return FindField(field_tree, block_offset, test_address, data_pointer, address, block_definition->fields);;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	Searches for the field at a specified address. </summary>
///
/// <param name="address">	The address to find. </param>
///
/// <returns>
/// 	k_status_ok if the field was found, otherwise a non-zero code if not found or an error
/// 	occurs.
/// </returns>
int32 FindFieldByAddress(const char* address)
{
	// Parse the address
	uintptr_t test_address;
	int value_count = sscanf_s(address, "0x%08x", &test_address);
	if(!value_count)
	{
		return k_status_failed;
	}

	// Test whether the address is within the tag data bounds
	uintptr_t tags_address = CAST_PTR(uintptr_t, g_cache_view_globals.m_cache_file_globals.tags_address);
	if((test_address < tags_address) || (test_address >= (tags_address + g_cache_view_globals.m_cache_file_header.tag_memory_size- 0x28)))
	{
		// Display not found
		Console::ColorPrint(k_color_status_error, "the provided address does not reside in the cache's tag data\n");
		return k_status_failed;
	}

	// Find the tag that the address is in
	Cache::s_cache_tag_instance* found_tag = nullptr;
	for(auto* instance = &g_cache_view_globals.m_cache_tag_instances[g_cache_view_globals.m_cache_file_globals.count - 1];
		instance >= g_cache_view_globals.m_cache_tag_instances;
		--instance)
	{
		if(instance->base_address && (test_address >= CAST_PTR(uintptr_t, instance->base_address)))
		{
			found_tag = instance;
			break;
		}
	}

	// If no tag was found then the address is not within the tag data
	if(found_tag == nullptr)
	{
		Console::ColorPrint(k_color_status_error, "the provided address does not reside in the cache's tag data\n");
		return k_status_failed;
	}

	// Get the tag group definition for the found tag type
	TagGroups::group_tag_to_string tag_group = { found_tag->group_tag };
	tag_group.Terminate().TagSwap();

	Yelo::tag_group* tag_group_def = blam::tag_group_get(tag_group.group);
	if(!tag_group_def)
	{
		Console::ColorPrint(k_color_status_error, "invalid tag group definition", true);
		return k_status_failed;
	}

	// Modify the tag name string to fit onto the command line
	std::string tag_name(found_tag->name);
	StringEditing::StringTruncate(tag_name, __max(Console::GetConsoleWidth() - 10, 0), 10);

	// Print the tag name and type
	Console::ColorPrint(k_color_block, "tag index: ");
	Console::ColorPrintF(k_color_index, "%i\n", found_tag->handle.index);
	Console::ColorPrintF(k_color_block, "tag type: %s\n", tag_group_def->name);
	Console::ColorPrint(k_color_block, "tag name:\n");
	Console::ColorPrintF(k_color_name, "\t%s\n\n", tag_name.c_str());
	
	// Find the tag field
	Yelo::tag_block_definition* root_definition = tag_group_def->header_block_definition;
	std::vector<s_field_tree_entry> field_tree;

	auto status = FindInBlock(field_tree, test_address, CAST_PTR(uintptr_t, found_tag->base_address), root_definition);
	if(status == k_status_ok)
	{
		int block_address = 0;

		// Print the field tree
		// Tree entries are added in reverse order once a field has been found so print the tree in reverse
		for(auto iter = field_tree.rbegin(); iter != field_tree.rend(); iter++)
		{
			// Get the field name
			std::string field_name("");
			if(iter->field_definition->name)
			{
				field_name.assign(iter->field_definition->name);
				StringEditing::GetStringSegment(field_name, field_name, nullptr, "^:#*");
			}

			// Get the size of the field
			size_t field_size = GetFieldSize(iter->field_definition);

			// Get the field data from Halo's memory
			auto tag_data = YELO_NEW_ARRAY_UNIQUE(byte, field_size);
			status = ReadHaloMemory((void*)iter->field_address, tag_data.get(), field_size);
			if(status != k_status_ok)
			{
				return k_status_failed;
			}

			byte* tag_data_ptr = tag_data.get();

			// Print the field info
			PrintFieldInfo(field_name, tag_data_ptr, iter->field_address, iter->field_block_offset, iter->field_definition);
			puts("");
			
			// If the path entry is a block definition print the block element info
			if(iter->block_definition)
			{
				puts("");
				PrintBlockInfo(field_name, tag_data_ptr, iter->field_definition);
				puts("");
				Console::ColorPrint(k_color_name, field_name.c_str());
				Console::ColorPrintF(k_color_block, " element %i:\n", iter->block_index);
				puts("");
			}
		}

		return k_status_ok;
	}
	else
	{
		return k_status_failed;
	}
}
#pragma endregion

#pragma region Print help functions
/// <summary>	Print's help for all runtime-cache-view commands. </summary>
void PrintHelp()
{
	puts(""); 
	Console::ColorPrint(k_color_command_line,
		"help				: prints this help list",
		true);
	Console::ColorPrint(k_color_command_line, 
		"load				: finds the Halo CE runtime and reads its\n					tag index",
		true);
	Console::ColorPrint(k_color_command_line, 
		"list [group filter]		: lists all of the tags in the runtime cache,\n					optionally filtering a tag group",
		true);
	Console::ColorPrint(k_color_command_line, 
		"open {index}			: opens a tag to view its contents",
		true);
	Console::ColorPrint(k_color_command_line, 
		"set {address} {type} {values}	: sets the value of a memory address in Halo",
		true);
	Console::ColorPrint(k_color_command_line, 
		"findaddr {address}		: finds the tag field that resides at the provided address",
		true);
	Console::ColorPrint(k_color_command_line, 
		"quit				: ends the program",
		true);
}
#pragma endregion