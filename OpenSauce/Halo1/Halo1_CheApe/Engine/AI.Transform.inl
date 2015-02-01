/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#include <blamlib/Halo1/tag_files/tag_groups.hpp>
#include <blamlib/Halo1/tag_files/tag_files_structures.hpp>
#include <blamlib/Halo1/main/console.hpp>

#include <YeloLib/Halo1/units/unit_transform_definition.hpp>

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

					for(auto& instigator : transform.instigators)
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