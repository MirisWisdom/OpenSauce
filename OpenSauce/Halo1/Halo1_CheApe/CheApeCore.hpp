/*
    Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition
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
	struct tag_group_definition;

	namespace CheApe
	{
		struct s_custom_tag_group_datum : Memory::s_datum_base
		{
			uint16 flags;
			tag_group_definition* definition;
		}; BOOST_STATIC_ASSERT( sizeof(s_custom_tag_group_datum) == 0x8 );
		typedef Memory::DataArray<s_custom_tag_group_datum, Enums::k_max_number_of_new_tag_groups> t_custom_tag_group_data;
	};
};