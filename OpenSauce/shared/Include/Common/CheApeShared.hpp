/*
    Yelo: Open Sauce SDK

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

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_cache_version = 2,

			k_max_number_of_new_tag_groups = 64,
		};
	};

	namespace CheApe
	{
		struct s_cache_header
		{
			tag Head;
			uint32 Version;
			PAD32;
			PAD32;

			void* BaseAddress;
			uint32 DataOffset;
			struct {
				int32 Size;
				void* Address;
			}StringPool;

			struct {
				struct {
					int32 Count;
					// address to a list of pointers to the scripting definitions
					void** Address;
				}Functions, Globals;
			}Scripting;

			struct {
				int32 Count;
				// address to a list of pointers to the fix-up definitions
				void** Address;
			}Fixups;
			PAD32;
			PAD32;

			uint32 Pad[495];
			tag Tail;
		}; BOOST_STATIC_ASSERT( sizeof(s_cache_header) == 0x800 );
		s_cache_header& GlobalCacheHeader();
	};
};