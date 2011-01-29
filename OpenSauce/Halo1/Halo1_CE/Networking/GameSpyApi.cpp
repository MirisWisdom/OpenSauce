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
#include "Common/Precompile.hpp"
#include "Networking/GameSpyApi.hpp"

#include "Memory/MemoryInterface.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_NETWORKING
#define __EL_INCLUDE_FILE_ID	__EL_NETWORKING_GAMESPY_API
#include "Memory/_EngineLayout.inl"

	namespace Networking
	{
		s_gamespy_product* GsProducts()										PTR_IMP_GET2(gamespy_products_list);

		API_FUNC_NAKED s_gamespy_client* GsGetClient(int32 client_id)
		{
			static uint32 TEMP_CALL_ADDR = GET_FUNC_PTR(GAMESPY_GET_CLIENT_KEY_HASH);

			__asm {
				push	ebp
				mov		ebp, esp
				push	ecx

				mov		eax, client_id
				call	TEMP_CALL_ADDR
				cmp		eax, GET_DATA_PTR(compiler_null_string)
				jnz		_return
				mov		eax, 4 // will cause us to return NULL. Yes, I r a hacka
				
_return:
				sub		eax, 4

				pop		ecx
				pop		ebp
				retn	4 * 1
			}
		}
	};
};