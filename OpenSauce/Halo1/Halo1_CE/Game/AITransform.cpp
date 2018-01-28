/*
	Yelo: Open Sauce SDK
	Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include <Common/Precompile.hpp>
#include <Game/AITransform.hpp>

#include <blamlib/units/unit_structures.hpp>
#include <yelolib/ai/c_actor_variant_transform_manager.hpp>
#include <yelolib/saved_games/game_state_yelo.hpp>
#include <yelolib/units/units_yelo.hpp>

namespace Yelo
{
	namespace AI
	{
		namespace Transform
		{
			static c_actor_variant_transform_manager g_actor_variant_transform_manager;

			void UnitUpdate(
				const datum_index unit_index)
			{
				g_actor_variant_transform_manager.UnitUpdate(unit_index);
			}

			void UnitDamaged(
				const datum_index unit_index,
				const Objects::s_damage_data* const damage_data)
			{
				g_actor_variant_transform_manager.UnitDamaged(unit_index, damage_data);
			}

			void Initialize()
			{
				const auto handler = [](
					const datum_index unit_index,
					const Enums::unit_animation_keyframe keyframe)
				{
					g_actor_variant_transform_manager.TriggerUnitTransformKeyframe(unit_index, keyframe);
				};
				Objects::Units::Animations::SetAnimationStateKeyframeHandler(
					Enums::_unit_animation_state_yelo_unit_transforming,
					handler);
			}

			void InitializeForNewGameState()
			{
				g_actor_variant_transform_manager.AllocateGameStateMemory();
			}

			void InitializeForNewMap()
			{
				g_actor_variant_transform_manager.LoadActorVariantTransforms();
			}

			void DisposeFromOldMap()
			{
				g_actor_variant_transform_manager.UnloadActorVariantTransforms();
				g_actor_variant_transform_manager.ClearInProgressTransforms();
			}

			void HandleGameStateLifeCycle(
				const _enum life_state)
			{
				if (life_state == Enums::_game_state_life_cycle_before_load)
				{
					g_actor_variant_transform_manager.ClearInProgressTransforms();
				}
			}

			bool& TransformsEnabled()
			{
				return g_actor_variant_transform_manager.TransformsEnabled();
			}

			void* HS_AITransformActor(
				void** arguments)
			{
				struct s_arguments
				{
					const datum_index unit_index;
					cstring transform_name;
					cstring target_name;
				}* args = CAST_PTR(s_arguments*, arguments);

				TypeHolder result;

				result.boolean = g_actor_variant_transform_manager.TransformActor(
					args->unit_index,
					args->transform_name,
					args->target_name);

				return result.pointer;
			}

			void* HS_AITransformActors(
				void** arguments)
			{
				struct s_arguments
				{
					const datum_index unit_list_index;
					cstring transform_name;
					cstring target_name;
				}* args = CAST_PTR(s_arguments*, arguments);

				TypeHolder result;

				result.boolean = g_actor_variant_transform_manager.TransformActors(
					args->unit_list_index,
					args->transform_name,
					args->target_name);

				return result.pointer;
			}

			void* HS_AITransformActorsByType(
				void** arguments)
			{
				struct s_arguments
				{
					const datum_index unit_list_index;
					const datum_index tag_index;
					cstring transform_name;
					cstring target_name;
				}* args = CAST_PTR(s_arguments*, arguments);

				TypeHolder result;

				result.boolean = g_actor_variant_transform_manager.TransformActorsByType(
					args->unit_list_index,
					args->tag_index,
					args->transform_name,
					args->target_name);

				return result.pointer;
			}

			void* HS_AIActorIsTransforming(
				void** arguments)
			{
				struct s_arguments
				{
					const datum_index unit_index;
				}* args = CAST_PTR(s_arguments*, arguments);

				TypeHolder result;

				result.boolean = g_actor_variant_transform_manager.ActorIsTransforming(
					args->unit_index);

				return result.pointer;
			}
		}
	}
}
