/*
	Yelo: Open Sauce SDK
	Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

namespace Yelo
{
	namespace Objects
	{
		struct s_damage_data;
	}

	namespace AI
	{
		namespace Transform
		{
			void UnitUpdate(
				datum_index unit_index);

			void UnitDamaged(
				datum_index unit_index,
				const Objects::s_damage_data* damage_data);

			void Initialize();
			void InitializeForNewGameState();
			void InitializeForNewMap();
			void DisposeFromOldMap();
			void HandleGameStateLifeCycle(
				_enum life_state);

			bool& TransformsEnabled();

			void* HS_AITransformActor(
				void** arguments);
			void* HS_AITransformActors(
				void** arguments);
			void* HS_AITransformActorsByType(
				void** arguments);
			void* HS_AIActorIsTransforming(
				void** arguments);
		}
	}
}
