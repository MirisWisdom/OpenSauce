/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#include <blamlib/Halo1/tag_files/tag_files_structures.hpp>
#include <blamlib/Halo1/main/console.hpp>
#include <blamlib/Halo1/tag_files/tag_collection_definitions.hpp>
#include <blamlib/Halo1/ai/actor_definitions.hpp>

#include <YeloLib/Halo1/units/unit_transform_definition.hpp>
#include <YeloLib/Halo1/units/unit_transform_definition.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario.hpp>
#include <YeloLib/Halo1/open_sauce/project_yellow_scenario_definitions.hpp>

#if PLATFORM_TYPE == PLATFORM_SAPIEN
#include <blamlib/Halo1/units/unit_structures.hpp>

#include <YeloLib/Halo1/ai/c_actor_variant_transform_manager.hpp>
#include <YeloLib/Halo1/units/units_yelo.hpp>
#endif

namespace Yelo
{
	namespace AI { namespace Transform
	{
#if PLATFORM_TYPE != PLATFORM_GUERILLA

#if PLATFORM_TYPE == PLATFORM_SAPIEN
#define PRINT_ERROR(format, ...) blam::console_warning(format, __VA_ARGS__);
#else
#define PRINT_ERROR(format, ...) YELO_ASSERT_DISPLAY(false, format, __VA_ARGS__);
#endif

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Validates a actor variant transform collection tag. </summary>
		///
		/// <param name="tag_index">   	Datum index of the tag. </param>
		/// <param name="print_errors">	(Optional) Flag for whether to print errors. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		static bool ValidateCollection(const datum_index tag_index, const bool print_errors = true)
		{
			auto& tag_definition = *blam::tag_get<TagGroups::actor_variant_transform_collection_definition>(tag_index);

			for(auto actor_variant : tag_definition.actor_variant_transforms)
			{
				if(actor_variant.actor_variant.tag_index.IsNull())
				{
					if(print_errors)
					{
						PRINT_ERROR("Missing actor variant tag in %s", blam::tag_get_name(tag_index));
					}
					return false;
				}

				for(auto& transform : actor_variant.transforms)
				{
					if(transform.transform_in.tag_index.IsNull())
					{
						if(print_errors)
						{
							PRINT_ERROR("Missing transform in tag in %s", blam::tag_get_name(tag_index));
						}
						return false;
					}

					if(transform.transform_out.tag_index.IsNull())
					{
						if(print_errors)
						{
							PRINT_ERROR("Missing transform out tag in %s", blam::tag_get_name(tag_index));
						}
						return false;
					}
				}

				// Process the transform probabilities
				for(auto& transform : actor_variant.transforms)
				{
					for(auto& selection_chance : transform.selection_chance)
					{
						if(selection_chance == 0.0f)
						{
							selection_chance = 1.0f;
						}
					}
				}
			}

			return true;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Validates an actor variant transform out tag. </summary>
		///
		/// <param name="tag_index">   	Datum index of the tag. </param>
		/// <param name="print_errors">	(Optional) Flag for whether to print errors. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		static bool ValidateTransformOut(const datum_index tag_index, const bool print_errors = true)
		{
			auto& tag_definition = *blam::tag_get<TagGroups::actor_variant_transform_out_definition>(tag_index);

			if(is_null_or_empty(tag_definition.transform_out_anim))
			{
				if(print_errors)
				{
					PRINT_ERROR("Missing transform out animation name in %s", blam::tag_get_name(tag_index));
				}
				return false;
			}

			for(auto& instigator : tag_definition.instigators)
			{
				if(instigator.unit.tag_index.IsNull())
				{
					if(print_errors)
					{
						PRINT_ERROR("Missing instigator type tag in %s", blam::tag_get_name(tag_index));
					}
					return false;
				}
			}

			for(auto& attachment : tag_definition.attachments)
			{
				if(attachment.object_type.tag_index.IsNull())
				{
					if(print_errors)
					{
						PRINT_ERROR("Missing attachment object type in %s", blam::tag_get_name(tag_index));
					}
					return false;
				}

				attachment.destination_marker_count = (attachment.destination_marker_count == 0 ? 1 : attachment.destination_marker_count);

				if((attachment.attachment_scale.lower == 0.0f) && (attachment.attachment_scale.upper == 0.0f))
				{
					attachment.attachment_scale.lower = 1.0f;
					attachment.attachment_scale.upper = 1.0f;
				}
			}

			return true;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Validates an actor variant transform in tag. </summary>
		///
		/// <param name="tag_index">   	Datum index of the tag. </param>
		/// <param name="print_errors">	(Optional) Flag for whether to print errors. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		static bool ValidateTransformIn(const datum_index tag_index, const bool print_errors = true)
		{
			auto& tag_definition = *blam::tag_get<TagGroups::actor_variant_transform_in_definition>(tag_index);

			if(tag_definition.targets.Count == 0)
			{
				if(print_errors)
				{
					PRINT_ERROR("An actor_variant_transform_in has no targets: %s", blam::tag_get_name(tag_index));
				}
				return false;
			}

			for(auto& target : tag_definition.targets)
			{
				if(target.actor_variant.tag_index.IsNull())
				{
					if(print_errors)
					{
						PRINT_ERROR("Missing actor variant tag in %s", blam::tag_get_name(tag_index));
					}
					return false;
				}

				if(is_null_or_empty(target.transform_in_anim))
				{
					if(print_errors)
					{
						PRINT_ERROR("Missing transform in animation name in %s", blam::tag_get_name(tag_index));
					}
					return false;
				}
			}

			// Normalise the target probabilities
			auto normalize_probability =
				[&](const Enums::game_difficulty_level difficulty)
				{
					real probability_total = 0.0f;

					if(tag_definition.targets.Count == 0)
					{
						return;
					}

					for(auto& target : tag_definition.targets)
					{
						if(target.selection_chance[difficulty] == 0.0f)
						{
							target.selection_chance[difficulty] = 1.0f;
						}

						probability_total += target.selection_chance[difficulty];
					}

					real normalisation_value = 1.0f / probability_total;
					for(auto& target : tag_definition.targets)
					{
						target.selection_chance[difficulty] *= normalisation_value;
					}
				};

			normalize_probability(Enums::_game_difficulty_level_easy);
			normalize_probability(Enums::_game_difficulty_level_normal);
			normalize_probability(Enums::_game_difficulty_level_hard);
			normalize_probability(Enums::_game_difficulty_level_impossible);

			return true;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>
		/// 	Builds all sparse transform collections into a single transform collection tag.
		/// </summary>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		bool CollateTransformCollections()
		{
			// Get the tag collection
			auto* yelo_definition = Scenario::GetYelo();
			if(!yelo_definition)
			{
				return false;
			}

			if(yelo_definition->IsNull())
			{
				return false;
			}

			if(yelo_definition->explicit_references.tag_index.IsNull())
			{
				return false;
			}
			auto& tag_collection = *blam::tag_get<TagGroups::s_tag_collection_definition>(yelo_definition->explicit_references.tag_index);

			// Create a new transform collection tag
			if(!blam::find_tag_instance(TagGroups::actor_variant_transform_collection_definition::k_group_tag, "actor_variant_transform_collection_complete").IsNull())
			{
				return false;
			}

			auto collection_index = blam::tag_new<TagGroups::actor_variant_transform_collection_definition>("actor_variant_transform_collection_complete");
			if(collection_index.IsNull())
			{
				return false;
			}

			auto& collection_complete = *blam::tag_get<TagGroups::actor_variant_transform_collection_definition>(collection_index);
			auto& collection_complete_entries = collection_complete.actor_variant_transforms;

			for(auto& tag_reference_entry : tag_collection.tag_references)
			{
				// Ignore tag references that aren't transform collections
				if(tag_reference_entry.tag.tag_index.IsNull() || (tag_reference_entry.tag.group_tag != TagGroups::actor_variant_transform_collection_definition::k_group_tag))
				{
					continue;
				}
				auto tag_index = tag_reference_entry.tag.tag_index;
				auto& collection = *blam::tag_get<TagGroups::actor_variant_transform_collection_definition>(tag_index);

				for(auto& actor_variant_entry : collection.actor_variant_transforms)
				{
					// Find an existing transform block for the actor variant
					auto found_actor_variant_entry = std::find_if(collection_complete_entries.begin(), collection_complete_entries.end(),
						[&](const TagGroups::actor_variant_transform_collection_entry& entry)
						{
							return entry.actor_variant.tag_index == actor_variant_entry.actor_variant.tag_index;
						}
					);

					// If no match is found, add a new block
					TagGroups::actor_variant_transform_collection_entry* transforms_entry = nullptr;
					if(found_actor_variant_entry == collection_complete_entries.end())
					{
						transforms_entry = blam::tag_block_add_and_get_element(collection_complete_entries);

						blam::tag_reference_set<TagGroups::s_actor_variant_definition>(transforms_entry->actor_variant, actor_variant_entry.actor_variant.name);
						transforms_entry->actor_variant.tag_index = actor_variant_entry.actor_variant.tag_index;
					}
					else
					{
						transforms_entry = found_actor_variant_entry;
					}

					// Add the transform types to the complete collection
					for(auto& transform : actor_variant_entry.transforms)
					{
						auto& collection_complete_entry_transform = *blam::tag_block_add_and_get_element(transforms_entry->transforms);

						collection_complete_entry_transform.flags = transform.flags;
						strcpy_s(collection_complete_entry_transform.transform_name, transform.transform_name);

						collection_complete_entry_transform.selection_chance[Enums::_game_difficulty_level_easy] = transform.selection_chance[Enums::_game_difficulty_level_easy];
						collection_complete_entry_transform.selection_chance[Enums::_game_difficulty_level_normal] = transform.selection_chance[Enums::_game_difficulty_level_normal];
						collection_complete_entry_transform.selection_chance[Enums::_game_difficulty_level_hard] = transform.selection_chance[Enums::_game_difficulty_level_hard];
						collection_complete_entry_transform.selection_chance[Enums::_game_difficulty_level_impossible] = transform.selection_chance[Enums::_game_difficulty_level_impossible];

						blam::tag_reference_set<TagGroups::actor_variant_transform_out_definition>(collection_complete_entry_transform.transform_out, transform.transform_out.name);
						collection_complete_entry_transform.transform_out.tag_index = transform.transform_out.tag_index;
						collection_complete_entry_transform.transform_out_ptr = nullptr;

						blam::tag_reference_set<TagGroups::actor_variant_transform_in_definition>(collection_complete_entry_transform.transform_in, transform.transform_in.name);
						collection_complete_entry_transform.transform_in.tag_index = transform.transform_in.tag_index;
						collection_complete_entry_transform.transform_in_ptr = nullptr;
					}
				}

				// Unload the sparse collection
				blam::tag_unload(tag_reference_entry.tag.tag_index);
			}

			// Remove sparse collection references from the item collection
			for(int32 index = tag_collection.tag_references.Count - 1; index >= 0; --index)
			{
				if(tag_collection.tag_references[index].tag.group_tag == TagGroups::actor_variant_transform_collection_definition::k_group_tag)
				{
					blam::tag_block_delete_element(tag_collection.tag_references, index);
				}
			}

			// Add the complete collection to the item collection
			auto& new_entry = *blam::tag_block_add_and_get_element(tag_collection.tag_references);

			blam::tag_reference_set<TagGroups::actor_variant_transform_collection_definition>(new_entry.tag, "actor_variant_transform_collection_complete");
			new_entry.tag.tag_index = collection_index;

			return true;
		}

		void InitialiseTagBlockProcedures()
		{
			auto& avtc_group = *blam::tag_group_get<TagGroups::actor_variant_transform_collection_definition>();
			avtc_group.postprocess_proc = 
				[](datum_index tag_index, Enums::tag_postprocess_mode mode) -> bool
				{
					if(mode == Enums::_tag_postprocess_mode_for_runtime)
					{
						return ValidateCollection(tag_index);
					}

					return true;
				};

			auto& avto_group = *blam::tag_group_get<TagGroups::actor_variant_transform_out_definition>();
			avto_group.postprocess_proc = 
				[](datum_index tag_index, Enums::tag_postprocess_mode mode) -> bool
				{
					if(mode == Enums::_tag_postprocess_mode_for_runtime)
					{
						return ValidateTransformOut(tag_index);
					}

					return true;
				};

			auto& avti_group = *blam::tag_group_get<TagGroups::actor_variant_transform_in_definition>();
			avti_group.postprocess_proc = 
				[](datum_index tag_index, Enums::tag_postprocess_mode mode) -> bool
				{
					if(mode == Enums::_tag_postprocess_mode_for_runtime)
					{
						return ValidateTransformIn(tag_index);
					}

					return true;
				};
		}

#undef PRINT_ERROR

#endif

#if PLATFORM_TYPE == PLATFORM_SAPIEN
		static c_actor_variant_transform_manager g_actor_variant_transform_manager;

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// <summary>	Validates the actor variant transform setup for the loaded scenario. </summary>
		///
		/// <param name="tag_index">	Datum index of the transform collection tag. </param>
		///
		/// <returns>	true if it succeeds, false if it fails. </returns>
		static bool ValidateTransformSetup(const datum_index tag_index)
		{
			if(!ValidateCollection(tag_index, false))
			{
				return false;
			}

			auto& tag_definition = *blam::tag_get<TagGroups::actor_variant_transform_collection_definition>(tag_index);
			for(auto& actor_variant : tag_definition.actor_variant_transforms)
			{
				for(auto& transform : actor_variant.transforms)
				{
					if(!ValidateTransformOut(transform.transform_out, false))
					{
						return false;
					}

					if(!ValidateTransformIn(transform.transform_in, false))
					{
						return false;
					}
				}
			}

			return true;
		}

		void UnitUpdate(const datum_index unit_index)
		{
			g_actor_variant_transform_manager.UnitUpdate(unit_index);
		}

		void UnitDamaged(const datum_index unit_index, const Objects::s_damage_data* damage_data)
		{
			g_actor_variant_transform_manager.UnitDamaged(unit_index, damage_data);
		}

		void Initialize()
		{
			Objects::Units::Animations::SetAnimationStateKeyframeHandler(Enums::_unit_animation_state_yelo_unit_transforming,
				[](const datum_index unit_index, const Enums::unit_animation_keyframe keyframe) { g_actor_variant_transform_manager.TriggerUnitTransformKeyframe(unit_index, keyframe); });
		}

		void InitializeForNewGameState()
		{
			g_actor_variant_transform_manager.AllocateGameStateMemory();
		}

		void InitializeForNewMap()
		{
			if(!CollateTransformCollections())
			{
				return;
			}

			TagGroups::c_tag_iterator iterator(TagGroups::actor_variant_transform_collection_definition::k_group_tag);

			// Get the first collection tag as there should be only one
			auto tag_index = iterator.Next();
			if(tag_index.IsNull())
			{
				return;
			}

			// Verify that the collection definition is usable
			if(ValidateTransformSetup(tag_index))
			{
				g_actor_variant_transform_manager.LoadActorVariantTransforms();
			}
			else
			{
				blam::console_warning("The scenario's actor variant transforms failed validation and have not been loaded");
			}
		}

		void DisposeFromOldMap()
		{
			g_actor_variant_transform_manager.UnloadActorVariantTransforms();
			g_actor_variant_transform_manager.ClearInProgressTransforms();
		}
#endif
	};};
};