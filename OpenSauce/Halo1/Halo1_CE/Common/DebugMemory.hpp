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
#pragma once

//#define API_DEBUG_MEMORY

#ifdef API_DEBUG_MEMORY

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

	void DumpAllocatedMemory();
};

static c_memory_tracker g_mem_tracker;

void DumpAllocatedMemory();

void* _cdecl operator new(size_t size);
void _cdecl operator delete(void* pointer);
// these may cause undefined behaviour, the original operators seemed to do something specific to arrays,
// but would not use the new and delete operators above so array memory information was being lost
void* _cdecl operator new[](size_t size);
void _cdecl operator delete[](void* pointer);

#endif
