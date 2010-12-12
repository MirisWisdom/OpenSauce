/*
    Yelo: Open Sauce SDK
		Halo 1 (CE) Edition
    Copyright (C) 2005-2010  Kornner Studios (http://kornner.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#pragma once

#include "Memory/MemoryInterface.hpp"

#define GET_NEW_MDP_DEFINITION(name) mdp_definition_##name

#define MDP_DEFINITION_START(name, type, max_iterations, header, fieldc)			\
	static Yelo::MessageDeltas::message_delta_definition mdp_definition_##name = {	\
		Yelo::Enums::_message_delta_##type ,										\
		NONE,																		\
		NONE,																		\
		NONE,																		\
		NONE,																		\
		max_iterations ,															\
		false,																		\
		header ,																	\
		{																			\
			fieldc ,																\
			NONE,																	\
			{

#define MDP_DEFINITION_FIELD(cls, name, type)	\
	{											\
		MDP_GET_FIELD_TYPE_DEFINITION( type ),	\
		FIELD_OFFSET( mdp_##cls , name ),		\
		FIELD_OFFSET( mdp_##cls , name ),		\
		false									\
	}

#define MDP_DEFINITION_FIELD_CUSTOM(cls, name, type)\
	{												\
		&MDP_GET_FIELD_TYPE_DEFINITION( type ),		\
		FIELD_OFFSET( mdp_##cls , name ),			\
		FIELD_OFFSET( mdp_##cls , name ),			\
		false										\
	}

#define MDP_DEFINITION_END()	\
			{NULL, 0, 0, false}	\
			}					\
		}						\
	}

#define MDP_FIELD_PROPERTIES_DEFINITION_START(type, name1, name2, enc, dec)												\
	extern Yelo::MessageDeltas::field_type_definition_parameters pmessage_delta_field_##name1##_properties_parameters;	\
	static Yelo::MessageDeltas::field_properties_definition pmessage_delta_field_##name1##_properties = {				\
		Yelo::Enums::_field_type_##type ,																	\
		name2 ,																								\
		(mdp_field_encode) enc ,																			\
		(mdp_field_decode) dec ,																			\
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
#define MDP_GET_FIELD_TYPE_DEFINITION( type ) GET_PTR2(message_delta_field_##type)

#define MDP_GET_FIELD_SET_DEFINITION( type ) GET_PTR2(message_delta_##type##_field_set)

#define __EL_INCLUDE_ID			__EL_INCLUDE_NETWORKING
#define __EL_INCLUDE_FILE_ID	__EL_NETWORKING_MDP
#include "Memory/_EngineLayout.inl"
	};
};