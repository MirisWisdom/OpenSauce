/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include <YeloLib/memory/memory_interface_base.hpp>

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
					if(proc != nullptr)
						(*proc)();
			}
			static void CallPostprocesses()
			{
				if(TBlockPostprocessCount == 0) return;

				const function_process_proc* proc = &TBlockPostprocess[0];
				for(size_t x = 0; x < TBlockPostprocessCount; proc++, x++)
					if(proc != nullptr)
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

		// define these in the project implementation, if used
		class c_function_interface_system
		{
		public:
			static void Initialize();
			static void Dispose();
		};

#include <YeloLib/memory/function_interface.inl>
#include <YeloLib/memory/function_interface_macros.inl>
	};
};