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

// Name of the memory map file with definitions for CheApe to load
#define CHEAPE_CACHE_FILE_NAME "CheApe.map"

namespace Yelo
{
	namespace Enums
	{
		enum {
			k_cheape_cache_header_signature = 'head',
			k_cheape_cache_footer_signature = 'tail',
			k_cheape_cache_version = 2,

			k_cheape_cache_signature_halo1 = 'blm1',
			k_cheape_cache_signature_halo2 = 'blm2',

			k_max_number_of_new_tag_groups = 64,
		};
	};

	namespace CheApe
	{
		struct s_cache_header
		{
			tag Head;
			uint32 Version;
			tag EngineSignature;	// Signature for the engine this is for (eg, Halo1, Halo2, etc)
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

			bool SignaturesValid() const	{ return Head == Enums::k_cheape_cache_header_signature && Tail == Enums::k_cheape_cache_footer_signature; }
			bool VersionIsValid() const		{ return Version == Enums::k_cheape_cache_version; }
			// Is the cache for 1st gen engine tools? IE, Halo Custom Edition
			bool IsFirstGenCache() const	{ return EngineSignature == Enums::k_cheape_cache_signature_halo1 || EngineSignature == 0; }
			// Is the cache for 2nd gen engine tools? IE, Halo 2 Vista
			bool IsSecondGenCache() const	{ return EngineSignature == Enums::k_cheape_cache_signature_halo2; }

			// Returns the reason why this header is invalid or NULL if everything appears gravy
			cstring GetInvalidReasonString(tag engine_sig, void* base_address) const
			{
				if(!SignaturesValid())
					return "Invalid header signatures";
				else if(!VersionIsValid())
					return "Invalid cache version";
				else if((engine_sig == Enums::k_cheape_cache_signature_halo1 && !IsFirstGenCache()) ||
						(engine_sig == Enums::k_cheape_cache_signature_halo2 && !IsSecondGenCache()) )
					return "Invalid engine signature";
				else if(BaseAddress != base_address)
					return "Invalid base address";

				return NULL;
			}
		}; BOOST_STATIC_ASSERT( sizeof(s_cache_header) == 0x800 );
		s_cache_header& GlobalCacheHeader();

		bool GetCompressedCacheFile(void*& buffer, uint32& size, uint32& compressed_size);
	};
};