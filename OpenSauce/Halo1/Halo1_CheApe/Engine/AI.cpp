/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Engine/AI.hpp"

#include <blamlib/Halo1/objects/objects.hpp>
#include <blamlib/Halo1/objects/damage.hpp>

#include "Engine/AI.Transform.inl"

namespace Yelo
{
	namespace AI
	{
		void InitializeForNewMap()
		{
			Transform::InitializeForNewMap();
		}

		void DisposeFromOldMap()
		{
			Transform::DisposeFromOldMap();
		}

		void ObjectsUpdate()
		{
			Objects::c_object_iterator iter(Enums::_object_type_mask_unit);

			for(auto object_index : iter)
			{
				Transform::UnitUpdate(object_index.index);
			}
		}

		void UnitDamageAftermath(const datum_index object_index, const Objects::s_damage_data* damage_data)
		{
			Transform::UnitDamaged(object_index, damage_data);
		}
	};
};