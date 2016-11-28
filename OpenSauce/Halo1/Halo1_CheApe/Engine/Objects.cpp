/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Engine/Objects.hpp"

#if PLATFORM_TYPE == PLATFORM_SAPIEN

#include <blamlib/Halo1/objects/objects.hpp>
#include <blamlib/Halo1/units/unit_structures.hpp>
#include <blamlib/Halo1/units/units.hpp>

#include <YeloLib/Halo1/units/units_yelo.hpp>

#include "Engine/Units.hpp"
#include "Engine/AI.hpp"

namespace Yelo
{
	namespace Objects
	{
		FUNC_PTR(OBJECTS_UPDATE_CALL,				PTR_NULL, PTR_NULL, 0x617C89);
		FUNC_PTR(OBJECT_DEPLETE_BODY_UNIT_KILL_CALL,			PTR_NULL, PTR_NULL, 0x64AE13);

		void PLATFORM_API ObjectDepleteBodyKillChildUnit(const datum_index object_index)
		{
			auto* unit_datum = blam::object_try_and_get_and_verify_type<s_unit_datum>(object_index);
			if(unit_datum && (unit_datum->unit.vehicle_seat_index != NONE))
			{
				auto* seat_extension_definition = GetSeatExtensionDefinition(unit_datum->object.parent_object_index, unit_datum->unit.vehicle_seat_index);
				if(seat_extension_definition)
				{
					if(TEST_FLAG(seat_extension_definition->flags, Flags::_unit_seat_extensions_flags_prevent_death_when_unit_dies))
					{
						return;
					}
				}
			}

			blam::unit_kill(object_index);
		}

		void PLATFORM_API ObjectsUpdate()
		{
			AI::ObjectsUpdate();
			Units::ObjectsUpdate();

			blam::objects_update();
		}

		void Initialize()
		{
			Units::Initialize();

			Memory::WriteRelativeCall(&ObjectsUpdate, GET_FUNC_VPTR(OBJECTS_UPDATE_CALL), true);
			Memory::WriteRelativeCall(&ObjectDepleteBodyKillChildUnit, GET_FUNC_VPTR(OBJECT_DEPLETE_BODY_UNIT_KILL_CALL), true);
		}

		void Dispose()
		{
			Units::Dispose();
		}
	};
};
#endif