/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/

///////////////////////////////////////////////////////////
// Forward declarations
BOOL PrintBlock(DWORD address, Yelo::tag_block_definition* block_definition);
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// User interface functions
/*!
 * \brief
 * Requests a command from the user.
 * 
 * \param command_list
 * A semi-colon delimited list of commands the user can input.
 * 
 * \param arguments_string
 * A pointer to a std::string that arguments will be copied to.
 * 
 * \param line_start
 * The text string to use at the start of the command line, defaults to "command".
 * 
 * \returns
 * Returns the index of the command entered, or -1 if the command did not match.
 * 
 * Requests a command from the user and returns the command index if the user enters a matching command.
 * Otherwise -1 is returned if no matches are found.
 */
int EnterCommand(const char* command_list, std::string* arguments_string, const char* line_start = "command")
{
	std::vector<std::string> command_array;
	std::string commands(command_list);

	bool newline_command = false;

	// split the command list into an array
	while(commands.length())
	{
		std::string segment;
		// get the next command in the list and add it to the command array
		StringEditing::GetStringSegment(commands, segment, NULL, ";");
		command_array.push_back(segment);

		// remove the command from the commands string
		StringEditing::RemoveStringSegment(commands, NULL, ";");

		// if the user can just press enter, we need to know to interpret that as a command
		if(!segment.compare("\n") && !newline_command)
			newline_command = true;
	}

	char command[128];
	puts("");
	// get the users command
	Console::ColorPrintF(k_color_command_line, "%s: ", line_start);
	do
	{
		fgets(command, sizeof(command), stdin);
		//ignore newlines unless it is a valid command
	}while(!newline_command && command[0] == '\n');

	// seperate into command and arguments
	std::string command_string(command);

	// if the command is just a newline no parsing is necessary
	if(command[0] != '\n')
		StringEditing::GetStringSegment(command_string, command_string, NULL, " \n");

	// if arguments are requested, set the passed strings value
	if(arguments_string)
	{
		// set the arguments string to the whole string, then remove the command
		arguments_string->assign(command);
		StringEditing::RemoveStringSegment(*arguments_string, NULL, " ");
		StringEditing::GetStringSegment(*arguments_string, *arguments_string, NULL, "\n");
	}

	int index = 0;
	bool command_found = false;

	std::vector<std::string>::iterator command_iter;

	// iterate through the commands until a matching one is/is not found
	for(command_iter = command_array.begin(); command_iter != command_array.end(); ++command_iter)
	{
		if(!command_string.compare(*command_iter))
		{
			command_found = true;
			break;
		}
		index++;
	}

	// return the index of the matching command, or -1 if it did not match
	return command_found ? index : -1;
}
/*!
 * \brief
 * Prints a human friendly string response to a commands status.
 * 
 * \param status_error
 * The status error to print a string for.
 * 
 * Prints a human friendly string response to a commands status.
 */
void PrintStatus(BOOL status_error)
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
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// Runtime memory I/O
/*!
 * \brief
 * Gets the handle of a running instance of Halo.
 * 
 * \returns
 * The commands end status.
 * 
 * Enumerates through all the systems processes, looking for an instance of Halo CE, 
 * and stores a handle to it once found.
 */
BOOL GetHaloHandle()
{
	// this is mostly refactored from MSDN

	Console::ColorPrint(k_color_default, "enumerating processes...", true);

	// create the initial process array
	DWORD byte_count = 1024 * sizeof(DWORD);
	DWORD* processes = (DWORD*)malloc(byte_count);

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
			processes = (DWORD*)realloc(processes, byte_count);

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

			// get the process name.
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
/*!
 * \brief
 * Reads a number of bytes from an address in Halos memory.
 * 
 * \param address
 * The memory address to read from.
 * 
 * \param destination
 * The buffer to read to.
 * 
 * \param destination_size
 * the size of the destination buffer.
 * 
 * \returns
 * The commands end status.
 * 
 * Reads a number of bytes from an address in Halos memory.
 */
BOOL ReadHaloMemory(const void* address, void* destination, const DWORD destination_size)
{
	SIZE_T bytes_read;
	BOOL success = ReadProcessMemory(g_cache_view_globals.m_halo_handle, 
		address, 
		destination, 
		destination_size, 
		&bytes_read);

	if(!success || (bytes_read != destination_size))
		return k_status_failed_to_read_runtime_memory;
	return k_status_ok;
}
/*!
 * \brief
 * Writes a number of bytes to Halos memory.
 * 
 * \param destination
 * The destination address in Halo.
 * 
 * \param source
 * The source buffer to write.
 * 
 * \param size
 * The number of bytes to write.
 * 
 * \returns
 * k_status_ok if succesful, otherwise k_status_failed_to_write_runtime_memory.
 *
 * Writes a block of bytes to Halo's memory.
 */
BOOL WriteHaloMemory(void* destination, void* source, const DWORD size)
{
	SIZE_T bytes_written;
	BOOL success = WriteProcessMemory(g_cache_view_globals.m_halo_handle, 
		destination, 
		source, 
		size, 
		&bytes_written);

	if(!success || (bytes_written != size))
		return k_status_failed_to_write_runtime_memory;
	return k_status_ok;
}

/*!
 * \brief
 * Reads a NULL terminated string from Halos memory.
 * 
 * \param address
 * The start address of the string.
 * 
 * \param destination
 * A char& reference that will be allocated with memory for the string.
 * 
 * \returns
 * The commands end status.
 * 
 * Reads a null terminated string from Halos memory and copies it into a char array in tool.
 * The memory allocated for "destination" must be deleted by the calling application.
 */
BOOL ReadHaloString(const cstring address, cstring& destination)
{
	// allocate the buffer
	int buffer_size = 2048;
	char* buffer = (char*)malloc(buffer_size);

	if(!buffer)
		return k_status_failed_to_allocate_local_memory;

	BOOL status = k_status_ok;
	// read the string
	int string_index = 0;
	do
	{
		// if the string is longer than the current buffer, allocate a larger buffer
		if(string_index == buffer_size)
		{
			buffer_size += 2048;
			char* new_buffer = (char*)realloc(buffer, buffer_size);

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
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// Halo cache functions
/*!
 * \brief
 * Creates a local copy of Halos tag index and tag instance array.
 * 
 * \returns
 * The commands end status.
 * 
 * Creates a local copy of Halos tag index and tag instance array.
 */
BOOL LoadTagIndex()
{
	void* cache_tag_header_ptr;

	Console::ColorPrint(k_color_default, "\nreading cache file globals...", true);

	// read the variable containing the cache file globals pointer
	BOOL status = ReadHaloMemory(k_runtime_109_cache_globals_ptr, 
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
		g_cache_view_globals.m_cache_tag_instances = NULL;
		return status;
	}

	// since this is a local copy of the tag instance array we can do some processing on it
	// so that we don't have to later
	for(int i = 0; i < g_cache_view_globals.m_cache_file_globals.count; i++)
	{
		// swap the tag groups now for later printing
		TagGroups::TagSwap(g_cache_view_globals.m_cache_tag_instances[i].group_tag);
		TagGroups::TagSwap(g_cache_view_globals.m_cache_tag_instances[i].parent_groups[0]);
		TagGroups::TagSwap(g_cache_view_globals.m_cache_tag_instances[i].parent_groups[1]);

		// copy the tag name strings into local memory
		status = ReadHaloString(g_cache_view_globals.m_cache_tag_instances[i].name, g_cache_view_globals.m_cache_tag_instances[i].name);

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
/*!
 * \brief
 * Deletes the local copy of the runtime tag index.
 * 
 * Deletes the local copy of the runtime tag index.
 */
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
	g_cache_view_globals.m_cache_tag_instances = NULL;

	// reset globals
	memset(&g_cache_view_globals.m_cache_file_globals, 0, sizeof(g_cache_view_globals.m_cache_file_globals));

	g_cache_view_globals.m_is_loaded = false;
}
/*!
 * \brief
 * Determines if the runtime cache has changed
 * 
 * \returns
 * k_status_index_changed if the cache is different otherwise k_status_index_matches.
 * If an error occurred when reading the cache globals the status id will be returned instead.
 * 
 * Creates a temporary copy of the runtimes cache globals and compares the crc with the local
 * copy.
 */
BOOL HasCacheChanged()
{
	Console::ColorPrint(k_color_default, "seeing if the cache has changed", true);

	void* cache_tag_header_ptr;

	// read the variable containing the cache file globals pointer
	BOOL status = ReadHaloMemory(k_runtime_109_cache_globals_ptr, 
		&cache_tag_header_ptr, 
		sizeof(cache_tag_header_ptr));

	if(status != k_status_ok)
		return status;

	Cache::s_cache_tag_header comparison_globals;

	// copy cache globals into local memory
	status = ReadHaloMemory(cache_tag_header_ptr, 
		&comparison_globals, 
		sizeof(g_cache_view_globals.m_cache_file_globals));

	if(status != k_status_ok)
		return status;

	// TODO: model_data_size used to be crc, but it isn't a crc, should change this to get the crc in the cache header
	return (g_cache_view_globals.m_cache_file_globals.model_data_size != comparison_globals.model_data_size) ? k_status_index_changed : k_status_index_matches;
}
/*!
 * \brief
 * Finds out if the cache has changed and reloads the index if it has.
 * 
 * \returns
 * k_change_index_reloaded if the index is reloaded, otherwise k_status_ok if nothings changed.
 * 
 * Finds out if the cache has changed and reloads the index if it has.
 */
BOOL ReloadCacheCheck()
{
	// has the cache changed
	BOOL status = HasCacheChanged();
	if(status == k_status_index_changed)
	{
		Console::ColorPrint(k_color_default, "different map cache present", true);
		// unload the current index
		UnloadTagIndex();
		// load the new index
		status = LoadTagIndex();
		status = (status != k_status_ok)? status : k_change_index_reloaded;
	}
	return (status == k_status_index_matches ? k_status_ok : status);
}
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// Edit command functions
/*!
 * \brief
 * Informs the user of possible unexpected results.
 * 
 * Informs the user that bad things can happen if they mess with the runtime memory incorrectly.
 * They must accept responsibility before continuing. They will only be asked this once per session.
 */
void DisplayEditWarning()
{
	if(g_cache_view_globals.m_accepted_edit_warning)
		return;

	puts("");
	Console::ColorPrint(k_color_status_warning, "manually writing to Halo's memory can cause unexpected results\nif incorrect values are entered.", true);

	int answer = EnterCommand("y;n", NULL, "do you want to continue (y/n)?");

	g_cache_view_globals.m_accepted_edit_warning = (answer == 0);
}
/*!
 * \brief
 * Writes new values to an address in Halos memory.
 * 
 * \param desc_index
 * The field desription index of the type at the memory address.
 * 
 * \param address
 * The runtime memory address to write to.
 * 
 * \param value_string
 * A string containing the new value(s).
 * 
 * \returns
 * The status of the command.
 * 
 * Parses a string of user defined values and writes them to a memory address in the runtime.
 */
BOOL WriteField(const int desc_index, const void* address, const char* value_string)
{
	union{
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

	// get the formatting string to parse the users arguments
	int count = 0;
	const char* scan_format = g_field_descriptions[desc_index].m_scan_format;
	// parse the users arguments
	switch(g_field_descriptions[desc_index].m_field_type)
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

			tag_group.Terminate();
			tag_group.TagSwap();
			value._tag = tag_group.group;
		}
		break;
	}

	// if the user didnt not specify the right about of arguments, the command fails
	if(count != g_field_descriptions[desc_index].m_scan_count)
		return k_status_failed;

	// write the new values to the runtimes memory
	BOOL status = WriteHaloMemory((void*)address, &value, g_field_descriptions[desc_index].m_field_size);
	return status;
}
/*!
 * \brief
 * Gets the users command details and passes them to WriteField.
 * 
 * \param arguments
 * String containing the users command arguments.
 * 
 * \returns
 * The commands end status.
 * 
 * Warns the user of possible problems when messing with memory before breaking
 * the users arguments into an address and values to be passed to WriteField.
 */
BOOL ChangeFieldValue(const char* arguments)
{
	// warn of unexpected consequences
	DisplayEditWarning();
	if(!g_cache_view_globals.m_accepted_edit_warning)
		return k_status_ok;

	int count = 0;
	std::string arguments_string(arguments);
	std::string argument;

	// get the address string
	StringEditing::GetStringSegment(arguments_string, argument, NULL, " ");

	// parse the address
	void* address;
	count = sscanf_s(argument.c_str(), "0x%08x", &address);
	if(!count)
		return k_status_failed;

	// remove the address from the arguments string
	StringEditing::RemoveStringSegment(arguments_string, NULL, " ");
	// get the type id string
	StringEditing::GetStringSegment(arguments_string, argument, NULL, " ");

	// get the description index for the requested type
	int index = 0;
	for(; index < NUMBEROF(g_field_descriptions); index++)
		if(!strcmp(g_field_descriptions[index].m_field_type_name, argument.c_str()))
			break;

	// if no matching type was found or the type is not valid, return
	if(index == NUMBEROF(g_field_descriptions) || !g_field_descriptions[index].m_valid_edit)
	{
		puts("");
		Console::ColorPrint(k_color_status_error, "invalid field type");
		return k_status_failed;
	}

	// remove the type id from the arguments string
	StringEditing::RemoveStringSegment(arguments_string, NULL, " ");

	// set the new values
	BOOL status = WriteField(index, address, arguments_string.c_str());
	return status;
}
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// Print tag command functions
/*!
 * \brief
 * Prints a fields value to the console.
 * 
 * \param field_data
 * The local memory address of the field.
 * 
 * \param field_definition
 * The fields definition.
 * 
 * Prints the value of a field in memory to the console.
 */
void PrintFieldValue(void* field_data, Yelo::tag_field* field_definition)
{
	union{
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

		void*					pointer;
	}value;

	value.pointer = field_data;
	
	// print the fields value
	Console::ColorPrint(k_color_default, "  value:\t");

	const char* format = g_field_descriptions[field_definition->field_type].m_format_string;
	switch(field_definition->field_type)
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
			TagGroups::group_tag_to_string tag_group;
			tag_group.group = value._tag_reference->group_tag;
			tag_group.Terminate();
			tag_group.TagSwap();

			Console::ColorPrintF(k_color_fieldvalue, format,
				tag_group.str,
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
			// print each byte of a pad/skip seperately, breaking at each 4 bytes
			// and adding a new line after 4 quads
			int pad_length = (int)field_definition->definition;
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
/*!
 * \brief
 * Prints the fields of a tag block to the console
 * 
 * \param tag_data
 * The local memory pointer where the block resides
 * 
 * \param address
 * The memory address of the block in Halos memory.
 * 
 * \param start_field
 * The first field of the block definition.
 * 
 * \returns
 * The commands end status.
 * 
 * Prints the fields of a tag block to the console
 */
BOOL PrintFields(char*& tag_data, DWORD& address, Yelo::tag_field* start_field)
{
	int count = 0;
	Yelo::tag_field* current = start_field;
	// enumerate through all of the blocks fields
	while(current->field_type != Enums::_field_terminator)
	{
		switch(current->field_type)
		{
		case Enums::_field_array_start:
			{
				int array_count = (int)current->definition;

				// store the first field in the array list
				tag_field* first_array_field = current + 1;
				// print the array
				for(int i = 0; i < array_count; i++)
				{
					BOOL status = PrintFields(tag_data, address, first_array_field);
					if(status != k_status_ok)
						return status;
				}
				// move the current field pointer past the array definition
				while(current->field_type != Enums::_field_array_end)
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

		puts("");
		// print the address of the field in Halo
		Console::ColorPrintF(k_color_address, "0x%08X", address);
		// print the field type
		Console::ColorPrint(k_color_default, " [");
		Console::ColorPrintF(k_color_fieldtype, "%s", g_field_descriptions[current->field_type].m_field_type_name);
		Console::ColorPrintF(k_color_default, "]%*s", 26 - strlen(g_field_descriptions[current->field_type].m_field_type_name), " ");

		// if the field has a name, print it
		std::string field_name("");
		if(current->name)
		{
			field_name.assign(current->name);
			StringEditing::GetStringSegment(field_name, field_name, NULL, "^:#*");
		}
		Console::ColorPrint(k_color_name, field_name.c_str(), true);

		// print the fields value
		PrintFieldValue(tag_data, current);

		// if the field is a tag block, print all of its elements
		if(current->field_type == Enums::_field_block)
		{
			tag_block* block = (tag_block*)tag_data;
			tag_block_definition* definition = current->Definition<tag_block_definition>();

			// clearly seperate the block from the parent tag fields
			puts("\n");

			// print the block fields name and type
			Console::ColorPrint(k_color_block, "block field:\t\t");
			Console::ColorPrint(k_color_name, field_name.c_str(), true);
			Console::ColorPrint(k_color_block, "block definition:\t");
			Console::ColorPrintF(k_color_block, definition->name, true);

			DWORD block_address = (DWORD)block->address;
			// print the block elements
			for(int i =0; i <  block->count; i++)
			{
				puts("");
				Console::ColorPrint(k_color_name, field_name.c_str());
				Console::ColorPrintF(k_color_block, " element %i:\n", i);

				BOOL status = PrintBlock((DWORD)block_address, definition);
				if(status != k_status_ok)
					return status;

				block_address += definition->element_size;

				puts("");
				Console::ColorPrint(k_color_name, field_name.c_str());
				Console::ColorPrintF(k_color_block, " element %i end\n", i);

				// if the user enters s, skip past the rest of the blocks
				// if the user enters q, close the tag
				puts("");
				
				Console::ColorPrint(k_color_command_line, "press enter to continue, \"s\" to skip this block type, or \"q\" to quit");

				int command = EnterCommand("q;s;\n", NULL);

				if(command == 0)
					return k_action_end_function;
				else if (command == 1)
					break;

				count = 0;
			}

			puts("");
			Console::ColorPrint(k_color_block, "block field end:\t\n");
			Console::ColorPrint(k_color_name, field_name.c_str(), true);
		}

		// increment the address'
		switch(current->field_type)
		{
		case Enums::_field_pad:
		case Enums::_field_skip:
			address += (DWORD)current->definition;
			tag_data +=  (DWORD)current->definition;
			break;
		default:
			address += g_field_descriptions[current->field_type].m_field_size;
			tag_data += g_field_descriptions[current->field_type].m_field_size;
		};

		current++;
		count++;

		// if 20 fields have been listed in a row, pause until the user wants to continue
		if(count == 20)
		{
			puts("");
			Console::ColorPrint(k_color_command_line, "press enter to continue or \"q\" to quit");
			int command = EnterCommand("q;\n", NULL);

			if(command == 0)
				return k_action_end_function;

			count = 0;
		}
	}
	return k_status_ok;
}
/*!
 * \brief
 * Prints a tag block to the console.
 * 
 * \param address
 * The address in Halos memory where the block resides.
 * 
 * \param block_definition
 * The tag block definition thatd escribes the blocks contents.
 *
 * \returns
 * The commands end status.
 * 
 * Copies a tag block element from Halo into local memory, then
 * prints its fields to the console.
 */
BOOL PrintBlock(DWORD address, Yelo::tag_block_definition* block_definition)
{
	// copy the block into local memory
	char* block_data = new char[block_definition->element_size];
	BOOL status = ReadHaloMemory((void*)address, block_data, block_definition->element_size);

	char* data_pointer = block_data;
	// print the blocks fields to the console
	if(status == k_status_ok)
		status = PrintFields(data_pointer, address, block_definition->fields);

	// delete the allocated memory
	delete [] block_data;

	return status;
}
/*!
 * \brief
 * Reads a tag from Halos cache and prints its fields.
 * 
 * \param argument
 * A string continung the users arguments.
 * 
 * Reads a tag from Halos cache and prints its fields.
 */
BOOL OpenTag(const char* arguments)
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

	// get the tag group definition for the tag type
	TagGroups::group_tag_to_string tag_group;
	tag_group.group = g_cache_view_globals.m_cache_tag_instances[index].group_tag;
	tag_group.Terminate();
	tag_group.TagSwap();

	Yelo::tag_group_definition* tag_group_def = Yelo::tag_group_get(tag_group.group);

	if(!tag_group_def)
	{
		Console::ColorPrint(k_color_status_error, "invalid tag group definition", true);
		return k_status_failed;
	}

	// modify the tag name string to fit onto the command line
	std::string tag_name(g_cache_view_globals.m_cache_tag_instances[index].name);
	StringEditing::StringTruncate(tag_name, 70, 10);

	// print some interesting bits of data
	Console::ColorPrintF(k_color_block, "tag type: %s\n", tag_group_def->name);
	Console::ColorPrint(k_color_block, "tag name:\n");
	Console::ColorPrintF(k_color_name, "\t%s\n\n", tag_name.c_str());

	// get the root block definition of the tag
	Yelo::tag_block_definition* root_definition = tag_group_def->definition;

	// print the tags contents
	return PrintBlock((DWORD)g_cache_view_globals.m_cache_tag_instances[index].definition, root_definition);
}
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// Print tag index functions
/*!
 * \brief
 * Prints the tag instance index to the command line.
 * 
 * \param filter
 * A tag group string to filter out specific tag types.
 * 
 * Prints the tag instance index to the command line. Can optionally filter a specific tag group type.
 */
BOOL PrintTagIndex(const char* filter)
{
	puts("");
	// loop through all of the tag instances
	int count = 0;
	for(int i = 0; i < g_cache_view_globals.m_cache_file_globals.count; i++)
	{
		TagGroups::group_tag_to_string tags[3];
		tags[0].group = g_cache_view_globals.m_cache_tag_instances[i].group_tag;
		tags[0].Terminate();
		tags[1].group = g_cache_view_globals.m_cache_tag_instances[i].parent_groups[0];
		tags[1].Terminate();
		tags[2].group = g_cache_view_globals.m_cache_tag_instances[i].parent_groups[1];
		tags[2].Terminate();

		// if a filter is used and this tag doesnt match, skip it
		if(filter != NULL && strcmp(filter, tags[0].str))
			continue;

		// modify the tag name to fit on the command line
		std::string tag_name(g_cache_view_globals.m_cache_tag_instances[i].name);
		StringEditing::StringTruncate(tag_name, 70, 10);

		// print the tag instance information
		Console::ColorPrintF(k_color_index, "%i", g_cache_view_globals.m_cache_tag_instances[i].datum.index);
		Console::ColorPrint(k_color_default, ":\t[");
		Console::ColorPrintF(k_color_tag, "%s", tags[0].str);
		Console::ColorPrint(k_color_default, ":");
		Console::ColorPrintF(k_color_tag, "%s", tags[1].str);
		Console::ColorPrint(k_color_default, ":");
		Console::ColorPrintF(k_color_tag, "%s", tags[2].str);
		Console::ColorPrint(k_color_default, "] : ");
		Console::ColorPrintF(k_color_address, "0x%08X\n", g_cache_view_globals.m_cache_tag_instances[i].definition);
		Console::ColorPrintF(k_color_name, "\t%s\n", tag_name.c_str());

		count++;

		// if 20 tags have been listed, pause until the user wants to continue
		if(count == 20)
		{
			puts("");
			Console::ColorPrint(k_color_command_line, "press enter to continue, or \"q\" to quit", true);
			int command = EnterCommand("q;\n", NULL);
			if(command == 0)
				return k_action_end_function;
			count = 0;
		}
	}
	return k_status_ok;
}
///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// Print help functions
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
		"quit				: ends the program",
		true);
}
///////////////////////////////////////////////////////////