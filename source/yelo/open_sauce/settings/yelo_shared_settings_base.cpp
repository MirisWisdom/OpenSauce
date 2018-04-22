/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#include <yelo/yelo.h>
#include <yelo/open_sauce/settings/yelo_shared_settings_base.hpp>

#include <TinyXml/tinyxml.hpp>

namespace Yelo
{
	namespace Settings
	{
#if PLATFORM_IS_EDITOR
		cstring K_DEBUG_FILENAME = "Editor.debug.log";
		cstring K_EVENT_LOG_FILENAME = "Editor.Log.txt";
#else
		cstring K_DEBUG_FILENAME = "Game.debug.log";
		cstring K_EVENT_LOG_FILENAME = "Game.Log.txt";
#endif
	};
};