/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#include <YeloLib/Halo1/units/c_unit_transform_manager.hpp>

namespace Yelo
{
	namespace Objects { namespace Units { namespace Transform
	{
		static c_unit_transform_manager g_unit_transform_manager;

		void UnitUpdate(const datum_index unit_index)
		{
			g_unit_transform_manager.UnitUpdate(unit_index);
		}

		void UnitDamaged(const datum_index unit_index, const s_damage_data* damage_data)
		{
			g_unit_transform_manager.UnitDamaged(unit_index, damage_data);
		}

		void InitializeForNewMap()
		{
			g_unit_transform_manager.LoadTransformDefinitions();
		}

		void DisposeFromOldMap()
		{
			g_unit_transform_manager.UnloadTransformDefinitions();
		}
	};};};
};