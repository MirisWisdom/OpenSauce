/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/cseries/debug_memory_yelo.hpp>

#if defined(API_DEBUG_MEMORY) && PLATFORM_TARGET != PLATFORM_TARGET_XBOX

static c_memory_tracker g_mem_tracker;
static unsigned int g_break_on_allocation = NONE;
static unsigned int g_break_on_deallocation = NONE;

void c_memory_tracker::AddBlock(void* pointer, size_t size)
{
	// increase the number of blocks and allocation index
	m_allocation_index++;

	if(g_break_on_allocation == m_allocation_index)
		while(true);

	m_block_count++;
	// reallocate the memory block array for the new size
	m_memory_blocks = (s_memory_block*)realloc(m_memory_blocks, sizeof(c_memory_tracker) * m_block_count);

	// set the last blocks values
	m_memory_blocks[m_block_count - 1].m_allocation_index = m_allocation_index;
	m_memory_blocks[m_block_count - 1].m_pointer = pointer;
	m_memory_blocks[m_block_count - 1].m_size = size;

	// update the current memory usage
	m_total_memory += size;
}
void c_memory_tracker::RemoveBlock(void* pointer)
{
	// return on a null pointer
	if(pointer == nullptr)
		return;

	int index = -1;
	// find the memory block by pointer
	for(unsigned int i = 0; i < m_block_count; i++)
	{
		if(pointer == m_memory_blocks[i].m_pointer)
		{
			index = i;
			break;
		}
	}

	// something is very wrong if there is no memory block!
	if(index == -1)
		throw;

	if(g_break_on_deallocation == m_memory_blocks[index].m_allocation_index)
		while(true);

	// reduce the block count and update the total memory allocated
	m_block_count--;
	m_total_memory -= m_memory_blocks[index].m_size;

	// shift the blocks after the deleted block up to remove it
	for(unsigned int i = index; i < m_block_count; i++)
		m_memory_blocks[i] = m_memory_blocks[i + 1];

	// reallocate the memory block array to free up the last element
	m_memory_blocks = (s_memory_block*)realloc(m_memory_blocks, sizeof(c_memory_tracker) * m_block_count);
}
void c_memory_tracker::DumpAllocatedMemory(const char* process_name)
{
	// create the memdump directory if it does not exist
	// could do with deleting the directory contents first if it does exists
	CreateDirectory("memdump", nullptr);

	// create the memory allocations file
	FILE* memory_list;
	fopen_s(&memory_list, "memdump\\memdump.txt", "w");

	// write the allocations descriptions
	fprintf(memory_list, "OpenSauce (%s) allocated memory dump\n", process_name);
	fputs("\n", memory_list);
	fprintf(memory_list, "Total allocations: %i\n", m_allocation_index);
	fprintf(memory_list, "Current allocations: %i\n", m_block_count);
	fprintf(memory_list, "Current memory usage: %i bytes\n", m_total_memory);

	// write the allocation information of each block and dump the memory to file
	for(unsigned int i = 0; i < m_block_count; i++)
	{
		// write information about the allocation
		fputs("\n", memory_list);
		fprintf(memory_list, "allocation index: %i\n", m_memory_blocks[i].m_allocation_index);
		fprintf(memory_list, "memory pointer: 0x%08X\n", m_memory_blocks[i].m_pointer);
		fprintf(memory_list, "allocation size: %i bytes\n", m_memory_blocks[i].m_size);

		// write the memory itself to file
		char dump_filename[MAX_PATH];
		sprintf_s(dump_filename, MAX_PATH, "memdump\\dump_index%i_size%i.bin", m_memory_blocks[i].m_allocation_index, m_memory_blocks[i].m_size);
		
		HANDLE memory_dump = CreateFile(dump_filename, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		
		DWORD bytes_written;
		BOOL success = WriteFile(memory_dump, m_memory_blocks[i].m_pointer, m_memory_blocks[i].m_size, &bytes_written, nullptr);

		// something is wrong! currently unhandled
		if(!success || (bytes_written != m_memory_blocks[i].m_size))
			throw;

		FlushFileBuffers(memory_dump);
		CloseHandle(memory_dump);
	}
	fclose(memory_list);
}

void DumpAllocatedMemory(const char* process_name)
{
	g_mem_tracker.DumpAllocatedMemory(process_name);
}

void* _cdecl operator new(size_t size)
{
	void* pointer = malloc(size);
	g_mem_tracker.AddBlock(pointer, size);
	return pointer;
}
void _cdecl operator delete(void* pointer)
{
	g_mem_tracker.RemoveBlock(pointer);
	free(pointer);
}
void* _cdecl operator new[](size_t size)
{
	void* pointer = malloc(size);
	g_mem_tracker.AddBlock(pointer, size);
	return pointer;
}
void _cdecl operator delete[](void* pointer)
{
	g_mem_tracker.RemoveBlock(pointer);
	free(pointer);
}
#endif

#if PLATFORM_TARGET != PLATFORM_TARGET_XBOX
#include <crtdbg.h>

c_crt_debug_fill_off::c_crt_debug_fill_off() :
	m_old_threshold( _CrtSetDebugFillThreshold(0) )
{
}
c_crt_debug_fill_off::~c_crt_debug_fill_off()
{
	_CrtSetDebugFillThreshold( m_old_threshold );
}
#else
c_crt_debug_fill_off::c_crt_debug_fill_off()
{
}
c_crt_debug_fill_off::~c_crt_debug_fill_off()
{
}
#endif