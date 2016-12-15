/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"
#include "Interface/Interface.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_INTERFACE
#define __EL_INCLUDE_FILE_ID	__EL_INTERFACE_INTERFACE
#include "Memory/_EngineLayout.inl"

	namespace GameState
	{
		s_user_interface_controller_globals* _UserInterfaceControllerGlobals()	DPTR_IMP_GET(_UserInterfaceControllerGlobals);
	};
};