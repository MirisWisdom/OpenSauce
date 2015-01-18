/*
	Yelo: Open Sauce SDK
		Halo 1 (Editing Kit) Edition

	See license\OpenSauce\Halo1_CheApe for specific license information
*/
#include "Common/Precompile.hpp"
#include "Engine/Objects.hpp"

#if PLATFORM_TYPE == PLATFORM_SAPIEN

#include <blamlib/Halo1/objects/objects.hpp>
#include <blamlib/Halo1/objects/damage.hpp>
#include <blamlib/Halo1/units/units.hpp>

#include "Engine/GrenadeTypesUpgrade.hpp"
#include "Engine/AI.hpp"

namespace Yelo
{
	namespace Objects { namespace Units
	{
		FUNC_PTR(UNIT_DAMAGE_AFTERMATH_CALL,					PTR_NULL, PTR_NULL, 0x64A128);

		void PLATFORM_API UnitDamageAftermathHook(const datum_index unit_index
			, const s_damage_data* damage_data
			, const Flags::object_damage_flags damage_flags
			, const real shield_amount
			, const real body_amount
			, void* arg6
			, const int32 damage_part
			, const datum_index dead_unit_index)
		{
			blam::unit_damage_aftermath(unit_index
				, damage_data
				, damage_flags
				, shield_amount
				, body_amount
				, arg6
				, damage_part
				, dead_unit_index);

			AI::UnitDamageAftermath(unit_index, damage_data);
		}

		void Initialize()
		{
			Memory::WriteRelativeCall(&UnitDamageAftermathHook, GET_FUNC_VPTR(UNIT_DAMAGE_AFTERMATH_CALL), true);
		}

		void Dispose() { }

		void InitializeForNewMap()
		{
			Items::InitializeGrenadesForNewMap();
		}

		void DisposeFromOldMap() { }
	};};
};
#endif