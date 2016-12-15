/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/cseries/cseries.hpp>

namespace Yelo
{
	namespace blam
	{
		void* PLATFORM_API system_malloc(size_t size)
		{
			return GlobalAlloc(GMEM_FIXED, size);
		}

		void PLATFORM_API system_free(void* pointer)
		{
			GlobalFree(pointer);
		}

		void* PLATFORM_API system_realloc(void* pointer, size_t size)
		{
			//YELO_ASSERT(size >= 0);  we use size_t, which is unsigned and thus this will always be true
			YELO_ASSERT(pointer||size);

			if (pointer == nullptr)
				return system_malloc(size);
			else if (size == 0)
			{
				system_free(pointer);
				return nullptr;
			}

			return GlobalReAlloc(pointer, size, GMEM_MOVEABLE);
		}
	};
};