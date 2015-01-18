/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Engine/Objects.hpp"

#if PLATFORM_TYPE == PLATFORM_SAPIEN

#include <blamlib/Halo1/objects/objects.hpp>

#include "Engine/Units.hpp"
#include "Engine/AI.hpp"

namespace Yelo
{
	namespace Objects
	{
		FUNC_PTR(OBJECTS_UPDATE_CALL,				PTR_NULL, PTR_NULL, 0x617C89);

		void PLATFORM_API ObjectsUpdate()
		{
			AI::ObjectsUpdate();

			blam::objects_update();
		}

		void Initialize()
		{
			Units::Initialize();

			Memory::WriteRelativeCall(&ObjectsUpdate, GET_FUNC_VPTR(OBJECTS_UPDATE_CALL), true);
		}

		void Dispose()
		{
			Units::Dispose();
		}
	};
};
#endif