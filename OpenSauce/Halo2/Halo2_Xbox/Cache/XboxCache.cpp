/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"
#include "Cache/XboxCache.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_CACHE
#define __EL_INCLUDE_FILE_ID	__EL_CACHE_XBOX_CACHE
#include "Memory/_EngineLayout.inl"

	namespace GameState
	{
		Cache::s_sound_cache* _XboxSounds()			PTR_IMP_GET2(_XboxSounds);
		Cache::s_texture_cache* _XboxTextures()		PTR_IMP_GET2(_XboxTextures);
		Cache::s_geometry_cache* _XboxGeometry()	PTR_IMP_GET2(_XboxGeometry);
	};
};