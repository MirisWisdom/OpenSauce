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

// See this header for more "Engine pointer markup system" documentation
#include <Memory/MemoryInterfaceShared.hpp>

//////////////////////////////////////////////////////////////////////////
// Engine pointer markup system
//
// [ce_offset]		: (Client) Latest version's memory address
// [cededi_offset]	: (Dedi server) Latest version's memory address
//
//////////////////////////////////////////////////////////////////////////

#if PLATFORM_IS_DEDI
	#define ENGINE_DPTR(type, name, ce_offset, cededi_offset) \
		static type##** const pp##name = CAST_PTR(type##**, cededi_offset); BOOST_STATIC_ASSERT( cededi_offset != NULL );

	#define ENGINE_PTR(type, name, ce_offset, cededi_offset) \
		static type##* const p##name = CAST_PTR(type##*, cededi_offset); BOOST_STATIC_ASSERT( cededi_offset != NULL );

	#define FUNC_PTR(name, ce_offset, cededi_offset) enum FUNC_PTR_##name { PTR_##name = cededi_offset }; BOOST_STATIC_ASSERT( PTR_##name != NULL );
	#define DATA_PTR(name, ce_offset, cededi_offset) enum DATA_PTR_##name { PTR_##name = cededi_offset }; BOOST_STATIC_ASSERT( PTR_##name != NULL );

	#define PLATFORM_VALUE(ce_value, cededi_value) cededi_value
#elif PLATFORM_IS_USER
	#define ENGINE_DPTR(type, name, ce_offset, cededi_offset) \
		static type##** const pp##name = CAST_PTR(type##**, ce_offset); BOOST_STATIC_ASSERT( ce_offset != NULL );

	#define ENGINE_PTR(type, name, ce_offset, cededi_offset) \
		static type##* const p##name = CAST_PTR(type##*, ce_offset); BOOST_STATIC_ASSERT( ce_offset != NULL );

	#define FUNC_PTR(name, ce_offset, cededi_offset) enum FUNC_PTR_##name { PTR_##name = ce_offset }; BOOST_STATIC_ASSERT( PTR_##name != NULL );
	#define DATA_PTR(name, ce_offset, cededi_offset) enum DATA_PTR_##name { PTR_##name = ce_offset }; BOOST_STATIC_ASSERT( PTR_##name != NULL );

	#define PLATFORM_VALUE(ce_value, cededi_value) ce_value
#endif

namespace Yelo
{
	namespace Memory
	{
		bool IsYeloEnabled();

		uint32 CRC(uint32& crc_reference, void* buffer, int32 size);
	};
};