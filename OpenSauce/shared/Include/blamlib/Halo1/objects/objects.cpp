/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include <blamlib/Halo1/objects/objects.hpp>

#include <blamlib/Halo1/objects/object_structures.hpp>

namespace Yelo
{
	namespace Objects
	{
		void s_object_data::CopyToPlacementData(s_object_placement_data& data) const
		{
			data.player_index = player_index;
			data.owner_object_index = owner_object_index;
			std::memcpy(&data.position,				&position, sizeof(position));
			std::memcpy(&data.transitional_velocity,&transitional_velocity, sizeof(transitional_velocity));
			std::memcpy(&data.forward,				&forward, sizeof(forward));
			std::memcpy(&data.up,					&up, sizeof(up));
			std::memcpy(&data.angular_velocity,		&angular_velocity, sizeof(angular_velocity));
		}

		bool s_object_data::VerifyType(long_flags type_mask) const
		{
			return TEST_FLAG( type_mask, type );
		}

		bool c_object_iterator::operator!=(const c_object_iterator& other) const
		{
			if (other.m_state.IsEndHack())
				return this->m_object != nullptr;
			else if (this->m_state.IsEndHack())
				return other.m_object != nullptr;

			return this->m_object != other.m_object;
		}
	};

	namespace blam
	{
		s_object_iterator& object_iterator_new(s_object_iterator& iter, long_flags type_mask, Flags::object_header_flags ignore_flags)
		{
			iter.signature = s_object_iterator::k_signature;
			iter.type_mask = type_mask;
			iter.ignore_flags = ignore_flags;
			iter.next_index = 0;
			iter.object_index = datum_index::null;

			return iter;
		}
#if !PLATFORM_IS_EDITOR // TODO: need to define object_get for editor builds
		s_object_data* object_get_and_verify_type(datum_index object_index, long_flags expected_types)
		{
			s_object_data* object = object_get(object_index);
			YELO_ASSERT_DISPLAY(object->VerifyType(expected_types),
				"got an object type we didn't expect (expected one of 0x%08x but got #%d).",
				expected_types, (long_enum)object->type);

			return object;
		}

		s_object_data* PLATFORM_API object_try_and_get_and_verify_type(datum_index object_index, long_flags expected_types)
		{
			s_object_data* object = object_get(object_index);

			return object && object->VerifyType(expected_types) 
				? object 
				: nullptr;
		}
#endif
	};
};