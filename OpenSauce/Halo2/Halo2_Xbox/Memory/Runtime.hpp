/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#pragma once

#include "Memory/MemoryInterface.hpp"

#define YELO_MEM_WRITE(func, ...) \
	{ \
		CR0_BEGIN(); \
		Yelo::Memory::func(__VA_ARGS__); \
		CR0_END(); \
	};

// begin a 'list' of memory writes
#define YELO_MEM_WLIST_BEGIN() \
	{ \
		CR0_BEGIN();

// perform a memory write with supplied arguments
#define YELO_MEM_WLIST_ITEM(func, ...) Yelo::Memory::func(__VA_ARGS__)

// end a pre-existing 'list' of memory writes
#define YELO_MEM_WLIST_END() \
		CR0_END(); \
	};

namespace Yelo
{
	namespace Memory
	{
#if FALSE // Can't touch this, ha!
		// [address] address to perform the store-push-ret code at
		// [target] address to make the store-push-ret goto
		// REMARKS:
		// Ported for legacy code. USE IS HIGHLY DISCOURAGED!
		void StorePushRet(void* address, void* target);
#endif
	};
};