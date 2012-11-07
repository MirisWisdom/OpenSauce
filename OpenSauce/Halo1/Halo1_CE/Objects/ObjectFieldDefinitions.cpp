/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
#include "Common/Precompile.hpp"
#include "Objects/ObjectFieldDefinitions.hpp"

#include "Objects/DeviceDefinitions.hpp"
#include "Objects/ItemDefinitions.hpp"
#include "Objects/UnitDefinitions.hpp"

#include "Game/EngineFunctions.hpp"
#include "Game/Scripting.hpp"
#include "Game/ScriptLibrary.hpp"
#include "Networking/Networking.hpp"
#include "Objects/Objects.hpp"
#include "TagGroups/TagGroups.hpp"

namespace Yelo
{
	namespace Objects
	{
		struct s_object_field_definition
		{
			cstring name;
			size_t name_length;
			Enums::hs_type hs_type;
			_enum definition_index;

			BOOL is_networked : 1;
			BOOL is_readonly : 1;
			// field is subscripted, so we only bsearch compare the first [name_length] characters
			// eg, weapon2 would be compare as equal to "weapon"
			BOOL is_subscripted : 1;

			static int __cdecl qsort_proc(void* ctxt, const void* _lhs, const void* _rhs)
			{
				const s_object_field_definition* lhs = CAST_PTR(const s_object_field_definition*, _lhs);
				const s_object_field_definition* rhs = CAST_PTR(const s_object_field_definition*, _rhs);

				return _stricmp(lhs->name, rhs->name);
			}
			template<size_t _SizeOfArray>
			static void qsort_list(s_object_field_definition (&list)[_SizeOfArray])
			{
				qsort_s(list, _SizeOfArray, sizeof(s_object_field_definition), qsort_proc, NULL);
			}

			static int __cdecl bsearch_proc(void* ctxt, const void* key, const void* field_definition)
			{
				const char* lhs = CAST_PTR(const char*, key);
				const s_object_field_definition* rhs = CAST_PTR(const s_object_field_definition*, field_definition);

				return rhs->is_subscripted ? _strnicmp(lhs, rhs->name, rhs->name_length) : _stricmp(lhs, rhs->name);
			}
			template<size_t _SizeOfArray>
			static const s_object_field_definition* bsearch_list(const s_object_field_definition (&list)[_SizeOfArray], cstring name)
			{
				return CAST_PTR(s_object_field_definition*, 
					bsearch_s(name, list, _SizeOfArray, sizeof(s_object_field_definition), bsearch_proc, NULL));
			}
		}; BOOST_STATIC_ASSERT( sizeof(s_object_field_definition) == 0x10 );
#define FIELD_INDEX_NAME(object_type, field_type, field_name)		\
		_##object_type##_field_##field_type##_##field_name

#define FIELD_ENTRY(object_type, hs_type, field_type, name, ...)	\
		{#name, sizeof( #name )-1,			HS_TYPE(hs_type), Enums::FIELD_INDEX_NAME(object_type, field_type, name), __VA_ARGS__}
#define FIELD_ENTRY2(object_type, hs_type, field_type, name, str_name, ...)	\
		{str_name, sizeof( str_name )-1,	HS_TYPE(hs_type), Enums::FIELD_INDEX_NAME(object_type, field_type, name), __VA_ARGS__}

#include "Objects/ObjectFieldDefinitions.inl"

#undef FIELD_INDEX_NAME
#undef FIELD_ENTRY
#undef FIELD_ENTRY2


		static int ObjectFunctionNameToIndex(cstring function_name)
		{
			if(function_name != NULL)
			{
				switch(function_name[0])
				{
				case 'a': return Enums::_outgoing_object_function_a;
				case 'b': return Enums::_outgoing_object_function_b;
				case 'c': return Enums::_outgoing_object_function_c;
				case 'd': return Enums::_outgoing_object_function_d;
				}
			}

			return NONE;
		}
		static bool ObjectFieldIndexIsVector(_enum definition_index)
		{
			using namespace Enums;

			return 
				(definition_index >= _object_field_vector_position && definition_index <= _object_field_vector_angular_velocity) || 
				(definition_index >= _unit_field_vector_desired_facing && definition_index <= _unit_field_vector_looking_velocity);
		}
		static int VectorFieldNameToIndex(cstring field_name, size_t vector_size)
		{
			int field_index = 0; // default to the first field ie: 'x', 'i', etc

			if(field_name != NULL)
			{
				switch(field_name[0])
				{
				case 'y':	case 'j':	if(vector_size >= sizeof(real_vector2d))
						field_index = 1; break;
				case 'z':	case 'k':	if(vector_size >= sizeof(real_vector3d))
						field_index = 2; break;
				case 'w':	case 'd':	if(vector_size >= sizeof(real_quaternion))
						field_index = 3; break;
				}
			}

			return field_index;
		}
		// NOTE: currently only supports subscripts 0 through 9
		// e.g., weapon2 would return 2
		static int SubscriptStringToIndex(cstring subscripted_name)
		{
			int index = 0;

			if(subscripted_name != NULL && subscripted_name[0] != '\0')
			{
				size_t last_char_index = strlen(subscripted_name)-1;
				char last_char = subscripted_name[last_char_index];

				if(last_char >= '0' && last_char <= '9')
					index = last_char - '0';
			}

			return index;
		}

		template<size_t _SizeOfArray, typename TObjectDatum, typename TGetterParam>
		static void ObjectFieldGetImpl(
			s_object_field_definition (&list)[_SizeOfArray], cstring name, TObjectDatum& obj, 
			__inout TypeHolder& result,
			TGetterParam getter_param,
			void (API_FUNC* getter_proc)(const s_object_field_definition&, TObjectDatum&, __inout TypeHolder&, TGetterParam)
			)
		{
			const s_object_field_definition* field = 
				s_object_field_definition::bsearch_list(list, name);

			if( field != NULL )
			{
				Enums::hs_type result_type = field->hs_type;

				getter_proc(*field, obj, result, getter_param);
				Scripting::UpdateTypeHolderFromPtrToData(result, result_type);
			}
		}
		template<size_t _SizeOfArray, typename TObjectDatum, typename TGetterParam, typename TDataType>
		static void ObjectFieldSetImpl(
			s_object_field_definition (&list)[_SizeOfArray], cstring name, TObjectDatum& obj, 
			TDataType data_value,
			TGetterParam getter_param,
			void (API_FUNC* getter_proc)(const s_object_field_definition&, TObjectDatum&, __inout TypeHolder&, TGetterParam)
			)
		{
			const s_object_field_definition* field = 
				s_object_field_definition::bsearch_list(list, name);

			if( field != NULL && 
				!field->is_readonly && 
				(Networking::IsLocal() || field->is_networked))
			{
				Enums::hs_type result_type = field->hs_type;
				TypeHolder result;

				getter_proc(*field, obj, result, getter_param);
				Scripting::UpdateTypeHolderDataFromPtr(result, result_type, &data_value);
			}
		}

#include "Objects/ObjectFieldDefinitions.ImplObjects.inl"
		real_vector3d* ObjectDataGetVectorByName(s_object_header_datum* header, 
			cstring data_name)
		{
			const s_object_field_definition* field = 
				s_object_field_definition::bsearch_list(g_object_real_fields, data_name);

			if( field != NULL && ObjectFieldIndexIsVector(field->definition_index) )
				return ObjectDataFieldGetVector(*field, *header);

			return NULL;
		}
		//////////////////////////////////////////////////////////////////////////
		// Real
		void ObjectDataGetRealByName(s_object_header_datum* header, 
			cstring data_name, cstring subdata_name,
			__inout TypeHolder& result)
		{
			ObjectFieldGetImpl(g_object_real_fields, data_name, *header, 
				result, subdata_name,
				ObjectDataFieldGetReal);
		}
		void ObjectDataSetRealByName(s_object_header_datum* header, 
			cstring data_name, cstring subdata_name,
			real data_value)
		{
			ObjectFieldSetImpl(g_object_real_fields, data_name, *header, 
				data_value, subdata_name,
				ObjectDataFieldGetReal);
		}

		
#include "Objects/ObjectFieldDefinitions.ImplWeapon.inl"
		//////////////////////////////////////////////////////////////////////////
		//
		void WeaponTagDataTriggerSetRealByName(s_weapon_datum* weapon, int32 trigger_index, 
			cstring data_name, cstring subdata_name, 
			real data_value)
		{
			datum_index definition_index = weapon->object.definition_index;

			const TagGroups::s_weapon_definition* definition = TagGroups::TagGet<TagGroups::s_weapon_definition>(definition_index);

			if (trigger_index >= 0 && trigger_index < definition->weapon.triggers.Count)
			{
				// We're fucking with the tag definition...at runtime :|
				TagGroups::weapon_trigger_definition& trigger = CAST_QUAL(TagGroups::weapon_trigger_definition&, 
					definition->weapon.triggers[trigger_index]);

				ObjectFieldSetImpl(g_weapon_tag_real_trigger_fields, data_name, trigger, 
					data_value, subdata_name,
					WeaponTagDataFieldTriggerSetReal);
			}
		}
		//////////////////////////////////////////////////////////////////////////
		// Real
		void WeaponDataGetRealByName(s_weapon_datum* weapon, 
			cstring data_name,
			__inout TypeHolder& result)
		{
			ObjectFieldGetImpl(g_weapon_real_fields, data_name, weapon->weapon, 
				result, (void*)NULL,
				WeaponDataFieldGetReal);
		}
		void WeaponDataSetRealByName(s_weapon_datum* weapon, 
			cstring data_name,
			real data_value)
		{
			ObjectFieldSetImpl(g_weapon_real_fields, data_name, weapon->weapon, 
				data_value, (void*)NULL,
				WeaponDataFieldGetReal);
		}


#include "Objects/ObjectFieldDefinitions.ImplUnits.inl"
		//////////////////////////////////////////////////////////////////////////
		// object_index
		void UnitDataGetObjectIndexByName(s_unit_datum* unit, 
			cstring data_name,
			__inout TypeHolder& result)
		{
			ObjectFieldGetImpl(g_unit_object_index_fields, data_name, unit->unit, 
				result, data_name, // getter processes data_name for the subscript
				UnitDataFieldGetObjectIndex);
		}
		//////////////////////////////////////////////////////////////////////////
		// Integer
		void UnitDataGetIntegerByName(s_unit_datum* unit, 
			cstring data_name,
			__inout TypeHolder& result)
		{
			ObjectFieldGetImpl(g_unit_integer_fields, data_name, unit->unit, 
				result, (void*)NULL,
				UnitDataFieldGetInteger);
		}
		void UnitDataSetIntegerByName(s_unit_datum* unit, 
			cstring data_name,
			int32 data_value)
		{
			ObjectFieldSetImpl(g_unit_integer_fields, data_name, unit->unit, 
				data_value, (void*)NULL,
				UnitDataFieldGetInteger);
		}
		//////////////////////////////////////////////////////////////////////////
		// Real
		void UnitDataGetRealByName(s_unit_datum* unit, 
			cstring data_name,
			__inout TypeHolder& result)
		{
			ObjectFieldGetImpl(g_unit_real_fields, data_name, unit->unit, 
				result, (void*)NULL,
				UnitDataFieldGetReal);
		}
		void UnitDataSetRealByName(s_unit_datum* unit, 
			cstring data_name,
			real data_value)
		{
			ObjectFieldSetImpl(g_unit_real_fields, data_name, unit->unit, 
				data_value, (void*)NULL,
				UnitDataFieldGetReal);
		}



		void InitializeObjectFieldDefinitions()
		{
			s_object_field_definition::qsort_list(g_object_real_fields);
			s_object_field_definition::qsort_list(g_weapon_tag_real_trigger_fields);
			s_object_field_definition::qsort_list(g_weapon_real_fields);
			s_object_field_definition::qsort_list(g_unit_object_index_fields);
			s_object_field_definition::qsort_list(g_unit_integer_fields);
			s_object_field_definition::qsort_list(g_unit_real_fields);
		}
	};
};