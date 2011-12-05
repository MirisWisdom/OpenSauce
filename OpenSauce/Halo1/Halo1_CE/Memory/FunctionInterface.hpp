/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
				for(int32 x = 0; x < TBlockPreprocessCount; proc++, x++)
					if(proc != NULL)
						(*proc)();
			}
			static void CallPostprocesses()
			{
				if(TBlockPostprocessCount == 0) return;

				const function_process_proc* proc = &TBlockPostprocess[0];
				for(int32 x = 0; x < TBlockPostprocessCount; proc++, x++)
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
#endif
	};
};