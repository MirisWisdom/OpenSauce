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
	FUNC_PTR(GAME_STATE_INITIALIZE_FOR_NEW_MAP_MOD,		0xB3F32);
};

namespace GameState
{
	ENGINE_DPTR(Cache::s_header, _CacheHeader,			0x6F79C0);
	ENGINE_DPTR(Cache::s_tag_index, _TagIndex,			0x6F81C0);
	ENGINE_DPTR(Cache::s_tag_instance, _TagInstances,	0x655C04);
};


//////////////////////////////////////////////////////////////////////////
// XboxCache.cpp
#elif __EL_INCLUDE_FILE_ID == __EL_CACHE_XBOX_CACHE
namespace GameState
{
	ENGINE_PTR(Cache::s_sound_cache, _XboxSounds,		0x6C2F94);
	ENGINE_PTR(Cache::s_texture_cache, _XboxTextures,	0x65BF20);
	ENGINE_PTR(Cache::s_geometry_cache, _XboxGeometry,	0x65BF48);
};


#else
	#error Undefined engine layout include for: __EL_INCLUDE_CACHE
#endif