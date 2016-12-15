/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#if defined(API_DEBUG_MEMORY) && PLATFORM_TARGET != PLATFORM_TARGET_XBOX

class c_memory_tracker
{
	struct s_memory_block
	{
		unsigned int m_allocation_index;
		void* m_pointer;
		size_t m_size;
	};

	unsigned int m_allocation_index;
	unsigned int m_total_memory;
	unsigned int m_block_count;
	s_memory_block* m_memory_blocks;

public:
	void AddBlock(void* pointer, size_t size);
	void RemoveBlock(void* pointer);

	void DumpAllocatedMemory(const char* process_name);
};

void DumpAllocatedMemory(const char* process_name);

void* _cdecl operator new(size_t size);
void _cdecl operator delete(void* pointer);
// these may cause undefined behaviour, the original operators seemed to do something specific to arrays,
// but would not use the new and delete operators above so array memory information was being lost
void* _cdecl operator new[](size_t size);
void _cdecl operator delete[](void* pointer);

#endif


// Turns off the CRT debug filling for the scope of the object
// See _CrtSetDebugFillThreshold
class c_crt_debug_fill_off
{
	size_t m_old_threshold;
public:
	c_crt_debug_fill_off();
	~c_crt_debug_fill_off();
};
#if PLATFORM_TARGET != PLATFORM_TARGET_XBOX
	// Temporarily turn off CRT debug fill
	#define CRT_DEBUG_FILL_OFF()	c_crt_debug_fill_off BOOST_JOIN(zzz_crt_debug_fill_off, __LINE__);
#else
	#define CRT_DEBUG_FILL_OFF()	__noop;
#endif

// Temporarily turn off CRT debug fill
#define CRT_DEBUG_FILL_OFF_BEGIN()	{ CRT_DEBUG_FILL_OFF();
// Turn the CRT debug fill back to the state it was at before the matching CRT_DEBUG_FILL_OFF_BEGIN
#define CRT_DEBUG_FILL_OFF_END()	}