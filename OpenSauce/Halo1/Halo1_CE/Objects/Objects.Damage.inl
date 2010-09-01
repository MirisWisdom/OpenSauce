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


namespace Yelo
{
	namespace Objects
	{
		static void PLATFORM_API object_damage_object_body_hook(datum_index object_index, 
			int32 region_index, int32 node_index, void* /*unknown*/,
			const void* damage_resistance, const void* damage_material, 
			const void* damage_data_definition, void* damage_data,
			void*, __out real&, __out real&, real, bool)
		{
		}
	};
};