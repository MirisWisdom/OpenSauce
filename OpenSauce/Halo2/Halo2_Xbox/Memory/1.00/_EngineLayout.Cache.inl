/*
    Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition
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

//////////////////////////////////////////////////////////////////////////
// Cache.cpp
#if __EL_INCLUDE_FILE_ID == __EL_CACHE_CACHE
namespace Cache
{
	FUNC_PTR(GAME_STATE_INITIALIZE_FOR_NEW_MAP_MOD, 0x123D17);
};

namespace GameState
{
	ENGINE_DPTR(Cache::s_header, _CacheHeader,			0x547700);
	ENGINE_DPTR(Cache::s_tag_instance, _TagInstances,	0x4E3B44);
};


//////////////////////////////////////////////////////////////////////////
// XboxCache.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_CACHE_XBOX_CACHE
namespace GameState
{
	ENGINE_PTR(Cache::s_sound_cache, _XboxSounds,		0x502104);
	ENGINE_PTR(Cache::s_texture_cache, _XboxTextures,	0x4E6454);
	ENGINE_PTR(Cache::s_geometry_cache, _XboxGeometry,	0x4E648C);
};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_CACHE
#endif