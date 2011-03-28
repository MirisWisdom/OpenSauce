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

t_multiplayer_map_data* MultiplayerMaps()		PTR_IMP_GET2(multiplayer_maps);
cstring* MapListIgnoredMapNames()				PTR_IMP_GET2(map_list_ignored_map_names);

static void MapListInitialize()
{
	static uint32 Initialize = GET_FUNC_PTR(MULTIPLAYER_MAP_LIST_INITIALIZE);

	__asm	call	Initialize
	MapListInitializeYelo();
}

API_FUNC_NAKED void MapListReIntialize()
{
	static uint32 Dispose = GET_FUNC_PTR(MULTIPLAYER_MAP_LIST_DISPOSE);

	__asm {
		call	Dispose
		call	MapListInitialize
		retn
	}
}