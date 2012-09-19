/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
/*
	Function Interface Component	
		Original Author: TheFieryScythe/FireScythe	02-11-2009 (DD-MM-YYYY)
		Integration: kornman00
	This component allows you to hook a function call in the engine, and
	add other functions before and after it, thus preventing conflicts
	between components that hook the same function.
*/
#pragma once

#include "Memory/MemoryInterface.hpp"

namespace Yelo
{
	namespace Memory
	{
		typedef void (API_FUNC* function_process_proc)();

		//////////////////////////////////////////////////////////////////////////
		// t_function_process
		template<const void* TAddress, bool TIsDisabled = false> struct t_function_process
		{
			static const void* k_address;
			static bool g_is_disabled;

			static void FunctionHook()
			{
				if(!g_is_disabled)
					__asm	call k_address
			}
		};
		//////////////////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////////////////
		// t_function_process_block
		template<const void* TAddress, 
			const function_process_proc TBlockPreprocess[], const size_t TBlockPreprocessCount,
			const function_process_proc TBlockPostprocess[], const size_t TBlockPostprocessCount,
			bool TIsDisabled = false> 
		struct t_function_process_block
		{
		private:
			static void CallPreprocesses()
			{
				if(TBlockPreprocessCount == 0) return;

				const function_process_proc* proc = &TBlockPreprocess[0];
				for(size_t x = 0; x < TBlockPreprocessCount; proc++, x++)
					if(proc != NULL)
						(*proc)();
			}
			static void CallPostprocesses()
			{
				if(TBlockPostprocessCount == 0) return;

				const function_process_proc* proc = &TBlockPostprocess[0];
				for(size_t x = 0; x < TBlockPostprocessCount; proc++, x++)
					if(proc != NULL)
						(*proc)();
			}
		public:
			static const void* k_address;
			static bool g_is_disabled;

			static void FunctionHook()
			{
				CallPreprocesses();

				if(!g_is_disabled)
					__asm	call k_address

				CallPostprocesses();
			}
		};
		//////////////////////////////////////////////////////////////////////////

		class c_function_interface_system
		{
		public:
			static void Initialize();
			static void Dispose() {}
		};

#include "Memory/FunctionInterface.inl"
#include "Memory/FunctionInterfaceMacros.inl"

#if !PLATFORM_IS_DEDI
		bool& FunctionProcessRenderHudIsDisabled();
		bool& FunctionProcessUpdateUIWidgetsDisabled();
#endif
	};
};