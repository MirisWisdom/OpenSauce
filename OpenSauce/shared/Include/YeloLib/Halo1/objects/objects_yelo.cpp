/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <YeloLib/Halo1/objects/objects_yelo.hpp>

#include <blamlib/Halo1/objects/objects.hpp>
#include <blamlib/Halo1/objects/object_structures.hpp>
#include <blamlib/Halo1/units/unit_structures.hpp>
#include <blamlib/Halo1/models/model_animation_definitions.hpp>

namespace Yelo
{
	namespace Objects
	{
		template<typename ObjectType>
		ObjectType* IteratorNext(s_object_iterator& iter)
		{
			YELO_ASSERT_DISPLAY( TEST_FLAG(iter.type_mask, ObjectType::k_object_type), "Wrong object_type given to IteratorNext<T>" );

			return CAST_PTR(ObjectType*, blam::object_iterator_next(iter));
		}

		void PlacementDataNewAndCopy(s_object_placement_data& data
			, const datum_index source_object_index
			, datum_index tag_index_override
			, const datum_index owner_object_index)
		{
			const auto* source_object = blam::object_get(source_object_index);

			if(tag_index_override.IsNull())
			{
				tag_index_override = source_object->definition_index;
			}

			blam::object_placement_data_new(data, tag_index_override, owner_object_index);
			source_object->CopyToPlacementData(data);
		}

		datum_index GetUltimateObject(const datum_index object_index)
		{
			if(!object_index.IsNull())
			{
				const auto* object_header_datums = Objects::ObjectHeader().Datums();
				const auto* current_obj = object_header_datums[object_index.index]._object;

				datum_index parent_index;
				while( !(parent_index = current_obj->parent_object_index).IsNull() )
				{
					current_obj = object_header_datums[parent_index.index]._object;
				}
			}

			return object_index;
		}

		datum_index GetNextObjectN(datum_index object_index, int32 step_count)
		{
			if(!object_index.IsNull())
			{
				const auto* object_header_datums = Objects::ObjectHeader().Datums();
				const auto* current_obj = object_header_datums[object_index.index]._object;

				for(object_index = current_obj->next_object_index;
					!object_index.IsNull() && step_count > 0;
					object_index = current_obj->next_object_index, --step_count)
				{
					current_obj = object_header_datums[object_index.index]._object;
				}
			}

			return step_count == 0 ? object_index : datum_index::null;
		}

		TagGroups::s_object_definition const* GetObjectDefinition(const datum_index object_index)
		{
			if(!object_index.IsNull())
			{
				const auto* object = blam::object_get(object_index);

				return blam::tag_get<TagGroups::s_object_definition>(object->definition_index);
			}

			return nullptr;
		}

		TagGroups::model_animation_graph const* GetObjectAnimations(const datum_index object_index)
		{
			if(!object_index.IsNull())
			{
				const auto* object = blam::object_get(object_index);
				auto tag_index = object->animation.definition_index;

				return blam::tag_get<TagGroups::model_animation_graph>(tag_index);
			}

			return nullptr;
		}

		real GetObjectDistanceFromPoint(const datum_index object_index, const real_vector3d& destination_point)
		{
			real dist = -1.0f;

			if(!object_index.IsNull())
			{
				real_vector3d object_origin;
				blam::object_get_origin(object_index, object_origin);

				real_vector3d relative_pos = object_origin - destination_point;
				dist = relative_pos.Magnitude();
			}

			return dist;
		}

		static void PerformActionOnChildrenByType(const datum_index parent
			, const long_flags object_type_mask
			, const std::function<void (const datum_index)>& action_performer)
		{
			const auto* object_header_datums = Objects::ObjectHeader().Datums();
			const auto* parent_object = object_header_datums[parent.index]._object;
			s_object_data* child_object;

			for(datum_index child_index = parent_object->first_object_index;
				!child_index.IsNull();
				child_index = child_object->next_object_index)
			{
				child_object = object_header_datums[child_index.index]._object;

				if(child_object->VerifyType(object_type_mask))
				{
					action_performer(child_index);
				}
			}
		}

		void DeleteChildrenByType(const datum_index parent, const long_flags object_type_mask)
		{
			PerformActionOnChildrenByType(parent, object_type_mask, [](const datum_index object_index) { blam::object_delete(object_index); });
		}

		void DetachChildrenByType(const datum_index parent, const long_flags object_type_mask)
		{
			PerformActionOnChildrenByType(parent, object_type_mask, [](const datum_index object_index) { blam::object_detach(object_index); });
		}

		void DeleteChildActors(const datum_index parent)
		{
			PerformActionOnChildrenByType(parent, Enums::_object_type_mask_unit,
				[](const datum_index object_index)
				{
					const auto* unit_datum = blam::object_try_and_get_and_verify_type<s_unit_datum>(object_index);
					if(unit_datum
						&& (!unit_datum->unit.actor_index.IsNull() || !unit_datum->unit.swarm_actor_index.IsNull()))
					{
						blam::object_delete(object_index);
					}
				}
			);
		}

		void DetachChildActors(const datum_index parent)
		{
			PerformActionOnChildrenByType(parent, Enums::_object_type_mask_unit,
				[](const datum_index object_index)
				{
					const auto* unit_datum = blam::object_try_and_get_and_verify_type<s_unit_datum>(object_index);
					if(unit_datum
						&& (!unit_datum->unit.actor_index.IsNull() || !unit_datum->unit.swarm_actor_index.IsNull()))
					{
						blam::object_detach(object_index);
					}
				}
			);
		}

		void DestroyChildrenByDefinition(const datum_index parent, const datum_index definition)
		{
			PerformActionOnChildrenByType(parent, Enums::_object_type_mask_all,
				[&](const datum_index object_index)
				{
					auto* object_datum = blam::object_get(object_index);
					if(object_datum && (object_datum->definition_index == definition))
					{
						blam::object_destroy(object_index);
					}
				});
		}

		void DeleteChildrenByDefinition(const datum_index parent, const datum_index definition)
		{
			PerformActionOnChildrenByType(parent, Enums::_object_type_mask_all,
				[&](const datum_index object_index)
				{
					auto* object_datum = blam::object_get(object_index);
					if(object_datum && (object_datum->definition_index == definition))
					{
						blam::object_delete(object_index);
					}
				});
		}

		void DetachChildrenByDefinition(const datum_index parent, const datum_index definition)
		{
			PerformActionOnChildrenByType(parent, Enums::_object_type_mask_all,
				[&](const datum_index object_index)
				{
					auto* object_datum = blam::object_get(object_index);
					if(object_datum && (object_datum->definition_index == definition))
					{
						blam::object_detach(object_index);
					}
				});
		}

		size_t PredictMemoryPoolUsage(const Enums::object_type type, const int32 node_count, const bool include_yelo_upgrades)
		{
			size_t total_headers_size = sizeof(Memory::s_memory_pool_block);
			size_t total_node_memory_size = (sizeof(real_orientation3d) * 2) + sizeof(real_matrix4x3);
			total_node_memory_size *= node_count;

			size_t object_type_size = 0;
			switch(type)
			{
			case Enums::_object_type_biped:			object_type_size = Enums::k_object_size_biped; break;
			case Enums::_object_type_vehicle:		object_type_size = Enums::k_object_size_vehicle; break;

			case Enums::_object_type_weapon:		object_type_size = Enums::k_object_size_weapon; break;
			case Enums::_object_type_equipment:		object_type_size = Enums::k_object_size_equipment; break;
			case Enums::_object_type_garbage:		object_type_size = Enums::k_object_size_garbage; break;

			case Enums::_object_type_projectile:	object_type_size = Enums::k_object_size_projectile; break;
			case Enums::_object_type_scenery:		object_type_size = Enums::k_object_size_scenery; break;

			case Enums::_object_type_machine:		object_type_size = Enums::k_object_size_machine; break;
			case Enums::_object_type_control:		object_type_size = Enums::k_object_size_control; break;
			case Enums::_object_type_light_fixture:	object_type_size = Enums::k_object_size_light_fixture; break;

			case Enums::_object_type_placeholder:	object_type_size = Enums::k_object_size_placeholder; break;
			case Enums::_object_type_sound_scenery:	object_type_size = Enums::k_object_size_sound_scenery; break;

			default: return 0;
			}

			return object_type_size + total_node_memory_size + total_headers_size;
		}
	};
};