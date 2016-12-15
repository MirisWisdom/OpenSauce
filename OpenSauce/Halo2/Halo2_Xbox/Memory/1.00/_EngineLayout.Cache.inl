/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
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
	ENGINE_DPTR(Cache::s_tag_index, _TagIndex,			0x547F00);
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