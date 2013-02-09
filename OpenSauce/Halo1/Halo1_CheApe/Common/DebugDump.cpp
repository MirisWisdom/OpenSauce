/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Common/DebugDump.hpp"

#include <dbghelp.h>

#include <YeloLib/cseries/pc_minidump_yelo.hpp>
#include <YeloLib/Halo1/shell/shell_windows_command_line.hpp>

#include "Common/YeloSettings.hpp"
#include "Memory/MemoryInterface.hpp"

namespace Yelo
{
	namespace Debug
	{
#define __EL_INCLUDE_ID			__EL_INCLUDE_OPEN_SAUCE
#define __EL_INCLUDE_FILE_ID	__EL_COMMON_DEBUG_DUMP
#include "Memory/_EngineLayout.inl"

		void PLATFORM_API GenericExceptionFilterHook(DWORD ExceptionCode, EXCEPTION_POINTERS *ExceptionPointers)
		{
			typedef void (PLATFORM_API* exception_filter_proc)(DWORD ExceptionCode, EXCEPTION_POINTERS *ExceptionPointers);

			static const exception_filter_proc k_generic_exception_filter = CAST_PTR(exception_filter_proc, GET_FUNC_VPTR(GENERIC_EXCEPTION_FILTER));
			static cstring k_process_name = PLATFORM_VALUE("guerilla", "tool", "sapien");

#if PLATFORM_ID != PLATFORM_TOOL
			if(CMDLINE_GET_PARAM(full_dump).ParameterSet())
				Debug::OutputExceptionData(k_process_name, Settings::ReportsPath(), true);
			else
				Debug::OutputExceptionData(k_process_name, Settings::ReportsPath(), false);
#else
			Debug::OutputExceptionData(k_process_name, Settings::ReportsPath(), Yelo::Settings::Get().active_profile.tool.do_full_crashdump);
#endif

			k_generic_exception_filter(ExceptionCode, ExceptionPointers);
		}

		void DumpInitialize()
		{
#if PLATFORM_ID != PLATFORM_GUERILLA
			Memory::WriteRelativeCall(GenericExceptionFilterHook, GET_FUNC_VPTR(GENERIC_EXCEPTION_FILTER_CALL));
#endif
		}

		void DumpDispose()
		{}
	};
};