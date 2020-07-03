/*
	Yelo: Open Sauce SDK
		Halo 2 (Xbox) Edition

	See license\OpenSauce\Halo2_Xbox for specific license information
*/
#include "Common/Precompile.hpp"
#include "Objects/Objects.hpp"

namespace Yelo
{
#define __EL_INCLUDE_ID			__EL_INCLUDE_OBJECTS
#define __EL_INCLUDE_FILE_ID	__EL_OBJECTS_OBJECTS
#include "Memory/_EngineLayout.inl"

	namespace Objects
	{
		void GetAngleAndDistance(datum_index source_object, datum_index target_object, real& angle, real& distance)
		{
			GameState::s_object_data* source = GameState::ObjectTryAndGet<GameState::s_object_data>(source_object, NULL, NULL);
			GameState::s_object_data* target = GameState::ObjectTryAndGet<GameState::s_object_data>(target_object, NULL, NULL);

			real_vector3d relative_pos = 
				(*target->GetOrigin()) - (*source->GetOrigin());
			distance = relative_pos.Magnitude();
			relative_pos.Normalize();
			real dot_product =
				relative_pos * (*source->GetForward());
			angle = XboxLib::Math::fabsf( XboxLib::Math::acosf(dot_product) );
		}

		void GetObjectInView(datum_index source_object, long_flags types_mask, datum_index::index_t& index, real& distance)
		{
			datum_index::index_t closest_datum;
			real closest_angle = 0.0f;
			real closest_dist;
			int32 num_found = 0;

			GameState::t_object_data::Iterator iter(GameState::_Object());
			GameState::s_object_header_datum* header_datum;
			while( (header_datum = iter.Next()) != NULL )
			{
				GameState::s_object_data* obj_data = header_datum->As.Object;
				if(!obj_data || !ObjectTypesInMask(types_mask, (_enum)obj_data->GetType()) ) continue;

				datum_index obj_index = iter.Current();
				num_found++;

				// compute relative position
				real_vector3d* object_pos = obj_data->GetOrigin();
				real angle, dist;
				GetAngleAndDistance(source_object, obj_index, angle, dist);

				// store if closer than prev object
				if(angle < 0.785f && (num_found == 1 || angle < closest_angle))
				{
					closest_datum = obj_index.index;
					closest_angle = angle;
					closest_dist = dist;
				}
			}

			index = num_found > 0 ? closest_datum : NONE;
			distance = closest_dist;
		}
	};

	namespace GameState
	{
		t_device_groups_data* _DeviceGroups()							DPTR_IMP_GET(_DeviceGroups);
		t_object_data* _Object()										DPTR_IMP_GET(_Object);
		Memory::s_memory_pool* _Objects()								DPTR_IMP_GET(_Objects);
		s_object_name_list* _ObjectNameList()							DPTR_IMP_GET(_ObjectNameList);
		s_object_globals* _ObjectGlobals()								DPTR_IMP_GET(_ObjectGlobals);
		s_object_clusters* _ObjectClusters()							DPTR_IMP_GET(_ObjectClusters);
		s_object_scripting_globals* _ObjectScriptingGlobals()			DPTR_IMP_GET(_ObjectScriptingGlobals);
		t_cached_object_render_states_data* _CachedObjectRenderStates()	DPTR_IMP_GET(_CachedObjectRenderStates);
		s_object_placement_data* _ObjectPlacementData()					DPTR_IMP_GET(_ObjectPlacementData);

		int32 ObjectGetTypeCount(short desired_type)
		{
			using namespace Enums;

			int32 count = 0;
			s_object_header_datum* datum;
			int32 x = 0;

			const Memory::s_data_array* object_header = &_Object()->Header;
			s_object_header_datum* obj_data = *_Object();

			if(desired_type >= 0)
			{
				for(; x < object_header->maximum_count; x++)
					if( !(datum = &obj_data[x])->IsNull() && datum->Type == desired_type)
						count++;
			}
			else
			{
				if(desired_type == _object_type_object) count = (int16)object_header->GetLength();
				else if(desired_type == _object_type_device)
				{
					for(; x < object_header->maximum_count; x++)
						if( !(datum = &obj_data[x])->IsNull() && 
							datum->Type == _object_type_machine ||
							datum->Type == _object_type_control ||
							datum->Type == _object_type_light_fixture)
							count++;
				}
				else if(desired_type == _object_type_item)
				{
					for(; x < object_header->maximum_count; x++)
						if( !(datum = &obj_data[x])->IsNull() && 
							datum->Type == _object_type_weapon ||
							datum->Type == _object_type_equipment ||
							datum->Type == _object_type_garbage)
							count++;
				}
				else if(desired_type == _object_type_unit)
				{
					for(; x < object_header->maximum_count; x++)
						if( !(datum = &obj_data[x])->IsNull() && 
							datum->Type == _object_type_biped ||
							datum->Type == _object_type_vehicle)
							count++;
				}
			}

			return count;
		}
	};
};