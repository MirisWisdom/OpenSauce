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
#include "Memory/MemoryInterface.hpp"

#include "Game/EngineFunctions.hpp"

// We apply '__declspec(noinline)' to the memory functions to avoid
// any invalid inlining cases with the linker when it thinks a supplied
// address is actually in our DLL's code when its not...its a pointer to halo's
// code.
// Ex:
// 1>\memory\memoryinterface.cpp(149) : fatal error C1001: An internal error has occurred in the compiler.
// 1>(compiler file 'F:\SP\vctools\compiler\utc\src\P2\main.c[0x10BEFEAB:0x003A0073]', line 182)
// 1> To work around this problem, try simplifying or changing the program near the locations listed above.
// 
// This was from 'WriteMemory' before it was marked as noinline, and was trying
// to inline with a halo address

namespace Yelo
{
	namespace Main
	{
		bool IsYeloEnabled();
	};

	namespace Memory
	{
		void* s_data_array::IteratorNext(void* iter)
		{
			return Engine::Memory::DataIteratorNext(iter);
		}

		bool IsYeloEnabled()
		{
			return Main::IsYeloEnabled();
		}

#define CRC_TABLE_SIZE 256
#define CRC32_POLYNOMIAL 0xEDB88320L
		static uint32 crc_table[CRC_TABLE_SIZE];
		static bool crc_table_initialized = false;

		static void BuildCrcTable()
		{
			for(int16 index = 0; index < CRC_TABLE_SIZE; ++index)
			{
				uint32 crc = index;
				for(int16 j = 0; j < 8; j++)
				{
					if(crc & 1) crc = (crc>>1) ^ CRC32_POLYNOMIAL;
					else crc >>= 1;
				}
				crc_table[index] = crc;
			}
		}

		uint32 CRC(uint32& crc_reference, void* buffer, int32 size)
		{
			if( !crc_table_initialized ) BuildCrcTable();

			byte* p;
			uint32 a;
			uint32 b;

			p = CAST_PTR(byte*, buffer);
			while (size--) 
			{
				a = (crc_reference >> 8) & 0x00FFFFFFL;
				b = crc_table[( (int32) crc_reference ^ *p++) & 0xFF];
				crc_reference = a^b;
			}

			return crc_reference;
		}
	};
};