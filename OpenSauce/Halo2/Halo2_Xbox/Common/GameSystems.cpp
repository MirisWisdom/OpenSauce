/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"
#include "Common/GameSystems.hpp"

#include "Game/Main.hpp"
#include "Memory/Config.hpp"
#include "Interface/Input.hpp"
#include "Cache/Cache.hpp"
#include "AI/AI.hpp"
#include "Game/Players.hpp"
#include "Networking/Statistics.hpp"
#include "Game/Director.hpp"

//#define API_YELO_NO_PROJECT_COMPONENTS


namespace Yelo
{
	namespace Main
	{
		int32 GetProjectComponents(s_project_component*& out_components)
		{
			out_components = NULL;
			int32 components_count = NONE;

#if !defined(API_YELO_NO_PROJECT_COMPONENTS)
			static s_project_component k_components[] = {
				#include "Common/GameSystems.ProjComponents.inl"
			};

			out_components = k_components;
			components_count = NUMBEROF(k_components)-1;
#endif

			return components_count;
		}
	};
};


#undef API_YELO_NO_PROJECT_COMPONENTS