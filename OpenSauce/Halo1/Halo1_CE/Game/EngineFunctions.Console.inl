/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

void ProcessCommand(cstring command)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(CONSOLE_PROCESS_COMMAND);

#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
	char local[k_engine_function_string_buffer_size];
	memset(local, 0, k_engine_function_string_buffer_size);
	strcpy_s(local, command);
#else
	cstring local = command;
#endif

	__asm {
#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
		lea		edi, local
#else
		mov		edi, local
#endif
		push	0
		call	TEMP_CALL_ADDR
		add		esp, 4
	}
}

void ProcessRemoteCommand(int32 machine_index, cstring command)
{
	Yelo::Console::TerminalGlobals()->rcon_machine_index = machine_index;
	ProcessCommand(command);
	Yelo::Console::TerminalGlobals()->rcon_machine_index = NONE;
}

void TerminalPrint(cstring msg)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(CONSOLE_TERMINAL_PRINTF);

#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
	char local[k_engine_function_string_buffer_size];
	memset(local, 0, k_engine_function_string_buffer_size);
	strcpy_s(local, msg);
#else
	cstring local = msg;
#endif

	__asm {
#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
		lea		ebx, local
#else
		mov		ebx, local
#endif
		push	ebx
		xor		al, al
		call	TEMP_CALL_ADDR
		add		esp, 4
	}
}

void TerminalPrintF(cstring format, ...)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(CONSOLE_TERMINAL_PRINTF);

	char local[k_engine_function_string_buffer_size];
	memset(local, 0, k_engine_function_string_buffer_size);

	va_list args;
	va_start(args, format);
	vsprintf_s(local, format, args);
	va_end(args);

	__asm {
		lea		ebx, local
		push	ebx
		xor		al, al
		call	TEMP_CALL_ADDR
		add		esp, 4
	}
}

void PrintF(cstring msg)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(CONSOLE_PRINTF);

#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
	char local[k_engine_function_string_buffer_size];
	memset(local, 0, k_engine_function_string_buffer_size);
	strcpy_s(local, msg);
#else
	cstring local = msg;
#endif

	__asm {
#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
		lea		ebx, local
#else
		mov		ebx, local
#endif
		push	ebx
		push	0
		call	TEMP_CALL_ADDR
		add		esp, 4 * 1
	}
}

void Warning(cstring msg)
{
	static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(CONSOLE_WARNING);

#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
	char local[k_engine_function_string_buffer_size];
	memset(local, 0, k_engine_function_string_buffer_size);
	strcpy_s(local, msg);
#else
	cstring local = msg;
#endif

	__asm {
#if defined(ENGINE_FUNCTIONS_USE_LOCAL)
		lea		ebx, local
#else
		mov		ebx, local
#endif
		push	ebx
		call	TEMP_CALL_ADDR
		add		esp, 4
	}
}