/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#pragma once

#include "Memory/MemoryInterface.hpp"

#include "Networking/MessageDeltaDefinitions.hpp"

#define GET_NEW_MDP_DEFINITION(name) mdp_definition_##name

#define MDP_STRUCT_NAME(base_name)	s_##base_name##_network_data

#define MDP_STRUCT_GET_TRAITS(base_name) MDP_STRUCT_NAME(base_name)::definition_traits

#define MDP_STRUCT_DEFINE_TRAITS(max_iterations, field_count, msg_type)	\
	class definition_traits { public: enum {							\
		k_max_iterations = max_iterations,								\
		k_field_count = field_count,									\
		k_message_type = Yelo::Enums::msg_type,							\
	}; };

#define MDP_DEFINITION_START(type, header)											\
	static Yelo::MessageDeltas::message_delta_definition mdp_definition_##type = {	\
		MDP_STRUCT_GET_TRAITS(type)::k_message_type,								\
		NONE, NONE,	NONE, NONE, 													\
		MDP_STRUCT_GET_TRAITS(type)::k_max_iterations,								\
		false,																		\
		header ,																	\
		{																			\
			MDP_STRUCT_GET_TRAITS(type)::k_field_count,								\
			NONE,																	\
			{

#define MDP_DEFINITION_FIELD(cls, name, type)			\
	{													\
		MDP_GET_FIELD_TYPE_DEFINITION( type ),			\
		FIELD_OFFSET( MDP_STRUCT_NAME(cls) , name ),	\
		FIELD_OFFSET( MDP_STRUCT_NAME(cls) , name ),	\
		false											\
	}

#define MDP_DEFINITION_FIELD_CUSTOM(cls, name, type)	\
	{													\
		&MDP_FIELD_TYPE_NAME( type ),					\
		FIELD_OFFSET( MDP_STRUCT_NAME(cls) , name ),	\
		FIELD_OFFSET( MDP_STRUCT_NAME(cls) , name ),	\
		false											\
	}

#define MDP_DEFINITION_END()			\
				{nullptr, 0, 0, false}	\
			}							\
		}								\
	}

#define MDP_FIELD_PROPERTIES_DEFINITION_START(type, name1, name2, enc, dec)												\
	extern Yelo::MessageDeltas::field_type_definition_parameters pmessage_delta_field_##name1##_properties_parameters;	\
	static Yelo::MessageDeltas::field_properties_definition pmessage_delta_field_##name1##_properties = {				\
		Yelo::Enums::_field_type_##type ,																	\
		name2 ,																								\
		CAST(mdp_field_encode, enc),																		\
		CAST(mdp_field_decode, dec),																		\
		&pmessage_delta_field_##name1##_properties_parameters,												\
		NONE,																								\
		0,																									\
		false,																								\
	};																										\
	static Yelo::MessageDeltas::field_type_definition_parameters pmessage_delta_field_##name1##_properties_parameters = {

#define MDP_FIELD_PROPERTIES_DEFINITION_END() \
	}

namespace Yelo
{
	namespace MessageDeltas
	{
#define MDP_FIELD_TYPE_NAME( type )				BOOST_PP_CAT(message_delta_field_, type)
#define MDP_FIELD_SET_NAME( type )				BOOST_PP_CAT(BOOST_PP_CAT(message_delta_, type), _field_set)

#define MDP_GET_FIELD_TYPE_DEFINITION( type ) MessageDeltas::GET_PTR2( MDP_FIELD_TYPE_NAME(type) )

#define MDP_GET_FIELD_SET_DEFINITION( type )  MessageDeltas::GET_PTR2( MDP_FIELD_SET_NAME(type) )

#define __EL_INCLUDE_ID			__EL_INCLUDE_NETWORKING
#define __EL_INCLUDE_FILE_ID	__EL_NETWORKING_MDP
#include "Memory/_EngineLayout.inl"

		/*
			fixed_width_normal_4bit
				 4 - internet
				20 - lan
			fixed_width_normal_8bit
				 8 - internet
				20 - lan
			fixed_width_normal_16bit
				16 - internet
				20 - lan

			angular_velocity
				-8.0, 8.0
				16 - internet
				32 - lan
			translational_velocity
				-100.0, 100.0
				16 - internet
				32 - lan
		*/

#ifndef YELO_NO_NETWORK
		inline field_type_translated_index_parameters* ObjectIndexParameters()
		{
			return &MDP_GET_FIELD_TYPE_DEFINITION(object_index)->parameters->translated_index;
		}
		inline field_type_translated_index_parameters* PlayerIndexParameters()
		{
			return &MDP_GET_FIELD_TYPE_DEFINITION(player_index)->parameters->translated_index;
		}
#endif
	};
};