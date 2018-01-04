/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

#include "Game/Camera.hpp"
#include "Game/GameStateRuntimeData.hpp"
#include "Objects/Objects.hpp"
#include "Objects/Objects.WeaponSettings.hpp"
#include <YeloLib/cseries/value_conversion.hpp>
#include <YeloLib/Halo1/objects/objects_yelo.hpp>
#include <TagGroups\TagGroups.hpp>
#include <Interface/UIWidgets.hpp>

namespace Yelo
{
	namespace Objects
	{
		static const TagGroups::s_network_game_player_unit* FindUnit(cstring unit_name, const TAG_TBLOCK(&units, TagGroups::s_network_game_player_unit))
		{
			for (const auto& unit_to_swap : units)
			{
				if (StrCmp(unit_to_swap.name, unit_name) == 0 && unit_to_swap.definition.tag_index.IsNull())
					return &unit_to_swap;
			}

			return nullptr;
		}
		bool SwitchUnit(s_unit_datum* unit, cstring name)
		{
			const TagGroups::s_network_game_player_unit* unit_to_swap = nullptr;
	
			if (unit_to_swap == nullptr)
				unit_to_swap = FindUnit(name, Scenario::GetYeloGlobals()->networking.player_units);

			if (unit_to_swap != nullptr)
			{
				return true;
			}

			return false;
		};


static void* scripting_objects_distance_to_object_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index object_list;
		datum_index dest_object;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;
	result.real = -1.0f;

	if(!args->dest_object.IsNull())
	{
		real min_dist = FLT_MAX;

		// Get the destination object's origin so that we can compare it, relative to each object in the list
		real_vector3d dest_object_origin;
		blam::object_get_origin(args->dest_object, dest_object_origin);

		// Enumerate the object list, testing each object's origin with dest
		for(datum_index curr_list_reference, curr_object_index = blam::object_list_get_first(args->object_list, curr_list_reference); 
			!curr_object_index.IsNull(); 
			curr_object_index = blam::object_list_get_next(args->object_list, curr_list_reference))
		{
			// Compare the current object from the list to the destination object
			real dist = GetObjectDistanceFromPoint(curr_object_index, dest_object_origin);

			// We want the smallest distance of all the objects
			if(min_dist > dist) min_dist = dist;
		}

		if(min_dist != FLT_MAX) result.real = min_dist;
	}

	return result.pointer;
}


static void* scripting_object_data_get_real_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index object_index;
		cstring data_name;
		cstring subdata_name;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;
	result.real = -1.0f;

	if(!args->object_index.IsNull())
	{
		s_object_header_datum* object = Objects::ObjectHeader()[args->object_index];

		ObjectDataGetRealByName(object, args->data_name, args->subdata_name, result);
	}

	return result.pointer;
}
static void* scripting_object_data_set_real_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index object_index;
		cstring data_name;
		cstring subdata_name;
		real data_value;
	}* args = CAST_PTR(s_arguments*, arguments);

	if(!args->object_index.IsNull())
	{
		s_object_header_datum* object = Objects::ObjectHeader()[args->object_index];

		ObjectDataSetRealByName(object, args->data_name, args->subdata_name, args->data_value);
	}

	return nullptr;
}
static void* scripting_object_data_set_vector_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index object_index;
		cstring data_name;
		int16 vector_index;
		PAD16;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

	if(!args->object_index.IsNull())
	{
		s_object_header_datum* object = Objects::ObjectHeader()[args->object_index];

		real_vector3d* obj_vector = ObjectDataGetVectorByName(object, args->data_name);
		const real_vector3d* vector = GameState::RuntimeData::VectorValueGet(args->vector_index);
		if(obj_vector != nullptr && vector != nullptr)
		{
			*obj_vector = *vector;
			result.boolean = true;
		}
	}

	return result.pointer;
}
static void* scripting_object_data_save_vector_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index object_index;
		cstring data_name;
		int16 vector_index;
		PAD16;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

	if(!args->object_index.IsNull())
	{
		s_object_header_datum* object = Objects::ObjectHeader()[args->object_index];

		real_vector3d* obj_vector = ObjectDataGetVectorByName(object, args->data_name);
		real_vector3d* vector = GameState::RuntimeData::VectorValueGetForModify(args->vector_index);
		if(obj_vector != nullptr && vector != nullptr)
		{
			*vector = *obj_vector;
			result.boolean = true;
		}
	}

	return result.pointer;
}


//////////////////////////////////////////////////////////////////////////
// WEAPONS

static void* scripting_weapon_data_get_real_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index weapon_index;
		cstring data_name;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;
	result.real = -1.0f;

	if(!args->weapon_index.IsNull())
	{
		auto weapon = blam::object_try_and_get_and_verify_type<s_weapon_datum>(args->weapon_index);

		if (weapon != nullptr)
			WeaponDataGetRealByName(weapon, args->data_name, result);
	}

	return result.pointer;
}
static void* scripting_weapon_data_set_real_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index weapon_index;
		cstring data_name;
		real data_value;
	}* args = CAST_PTR(s_arguments*, arguments);

	if(GameState::IsLocal() && !args->weapon_index.IsNull())
	{
		auto weapon = blam::object_try_and_get_and_verify_type<s_weapon_datum>(args->weapon_index);

		if(weapon != nullptr)
			WeaponDataSetRealByName(weapon, args->data_name, args->data_value);
	}

	return nullptr;
}

//////////////////////////////////////////////////////////////////////////
// WEAPONS - MAGAZINES

static void* scripting_weapon_data_magazine_get_integer_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index weapon_index;
		int32 magazine_index;
		cstring data_name;
		cstring subdata_name;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;
	result.int32 = NONE;

	if(GameState::IsLocal() && !args->weapon_index.IsNull())
	{
		auto weapon = blam::object_try_and_get_and_verify_type<s_weapon_datum>(args->weapon_index);

		if(weapon != nullptr)
			WeaponDataMagazineGetIntegerByName(weapon, args->magazine_index, args->data_name, args->subdata_name, result);
	}

	return result.pointer;
}
static void* scripting_weapon_data_magazine_set_integer_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index weapon_index;
		int32 magazine_index;
		cstring data_name;
		cstring subdata_name;
		int32 data_value;
	}* args = CAST_PTR(s_arguments*, arguments);

	if(GameState::IsLocal() && !args->weapon_index.IsNull())
	{
		auto weapon = blam::object_try_and_get_and_verify_type<s_weapon_datum>(args->weapon_index);

		if(weapon != nullptr)
			WeaponDataMagazineSetIntegerByName(weapon, args->magazine_index, args->data_name, args->subdata_name, args->data_value);
	}

	return nullptr;
}

//////////////////////////////////////////////////////////////////////////
// WEAPONS - TRIGGERS

static void* scripting_weapon_data_trigger_set_real_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index weapon_index;
		int32 trigger_index;
		cstring data_name;
		cstring subdata_name;
		real data_value;
	}* args = CAST_PTR(s_arguments*, arguments);

	// IF ANY OF YOUR DESIGNERS USE THIS FUCKING SCRIPT FUNCTION, THEN YOU ALL DESERVE TO BE FUCKING SHOT.

	// SRSLY.

	// We don't support modifying trigger data in anything but local games because it writes to tag memory
	if(GameState::IsLocal() && !args->weapon_index.IsNull())
	{
		auto weapon = blam::object_try_and_get_and_verify_type<s_weapon_datum>(args->weapon_index);

		if(weapon != nullptr)
			WeaponTagDataTriggerSetRealByName(weapon, args->trigger_index, args->data_name, args->subdata_name, args->data_value);
	}

	return nullptr;
}


//////////////////////////////////////////////////////////////////////////
// UNITS

static void* scripting_unit_data_get_object_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index unit_index;
		cstring data_name;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;
	result.datum = datum_index::null;

	if(!args->unit_index.IsNull())
	{
		auto* unit = blam::object_try_and_get_and_verify_type<s_unit_datum>(args->unit_index);
		if(!unit)
		{
			return result.pointer;
		}

		UnitDataGetObjectIndexByName(unit, args->data_name, result);
	}

	return result.pointer;
}

static void* scripting_unit_data_get_integer_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index unit_index;
		cstring data_name;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;
	result.int32 = NONE;

	if(!args->unit_index.IsNull())
	{
		auto* unit = blam::object_try_and_get_and_verify_type<s_unit_datum>(args->unit_index);
		if(!unit)
		{
			return result.pointer;
		}

		UnitDataGetIntegerByName(unit, args->data_name, result);
	}

	return result.pointer;
}
static void* scripting_unit_data_set_integer_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index unit_index;
		cstring data_name;
		int32 data_value;
	}* args = CAST_PTR(s_arguments*, arguments);

	if(!args->unit_index.IsNull())
	{
		auto* unit = blam::object_try_and_get_and_verify_type<s_unit_datum>(args->unit_index);
		if(!unit)
		{
			return nullptr;
		}

		UnitDataSetIntegerByName(unit, args->data_name, args->data_value);
	}

	return nullptr;
}

static void* scripting_unit_data_get_real_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index unit_index;
		cstring data_name;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;
	result.real = -1.0f;

	if(!args->unit_index.IsNull())
	{
		auto* unit = blam::object_try_and_get_and_verify_type<s_unit_datum>(args->unit_index);
		if(!unit)
		{
			return result.pointer;
		}

		UnitDataGetRealByName(unit, args->data_name, result);
	}

	return result.pointer;
}
static void* scripting_unit_data_set_real_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index unit_index;
		cstring data_name;
		real data_value;
	}* args = CAST_PTR(s_arguments*, arguments);

	if(GameState::IsLocal() && !args->unit_index.IsNull())
	{
		auto* unit = blam::object_try_and_get_and_verify_type<s_unit_datum>(args->unit_index);
		if(!unit)
		{
			return nullptr;
		}

		UnitDataSetRealByName(unit, args->data_name,args->data_value);
	}

	return nullptr;
}


static void* scripting_vehicle_remapper_enabled_evaluate(void** arguments)
{
	struct s_arguments {
		cstring state_name;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

	result.boolean = !c_settings_objects::Instance()->m_vehicle_remapper_enabled;
	if( args->state_name[0] != '\0' && strcmp(args->state_name, "get")!=0 )
	{
		bool value = false;
		ValueConversion::FromString(args->state_name, value);
		VehicleRemapperEnable( value );
	}

	return result.pointer;
}

static void* scripting_unit_is_key_down_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index unit_index;
		short keypress;
	}*args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;
	result.boolean = false;

	if (!args->unit_index.IsNull())
	{
		auto* unit = blam::object_try_and_get_and_verify_type<s_unit_datum>(args->unit_index);
		if (!unit)
		{
			return result.pointer;
		}
		cstring keypress;
		keypress = "keypress";
		int16 keystroke;
		keystroke = args->keypress;
		if (GetAsyncKeyState(args->keypress) & 0x8000)
		{
			UnitDataSetIntegerByName(unit, keypress, keystroke);
			UnitDataGetIntegerByName(unit, keypress, result);
			result.boolean = true;
		}
	}
	return result.pointer;
}

static void* scripting_unit_camera_fov_set_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index unit_index;
		real fov;
	}*args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;
	result.boolean = false;

	if (!args->unit_index.IsNull())
	{
		auto* unit = blam::object_try_and_get_and_verify_type<s_unit_datum>(args->unit_index);
		if (!unit)
		{
			return result.pointer;
		}
		if (args->fov)
		{
			Fov::GetFieldOfView();
			Fov::SetFieldOfView(args->fov);
			result.boolean = true;
		}
	}
	return result.pointer;
}

static void* scripting_unit_weapon_set_position_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index unit_index;
		real weapon_pos_x;
		real weapon_pos_y;
		real weapon_pos_z;
	}*args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;
	result.boolean = false;

	if (!args->unit_index.IsNull())
	{
		auto* unit = blam::object_try_and_get_and_verify_type<s_unit_datum>(args->unit_index);
		if (!unit)
		{
			return result.pointer;
		}
		if (args->weapon_pos_x && args->weapon_pos_y && args->weapon_pos_z, NONE)
		{
			real_vector3d weapon_position{ args->weapon_pos_x, args->weapon_pos_y, args->weapon_pos_z };
			Weapon::Initialize();
			Weapon::GetWeaponPosition();
			Weapon::SetWeaponPosition(weapon_position / 100);
			Weapon::Dispose();
			result.boolean = true;
		}
	}
	return result.pointer;
}

static void* scripting_camera_fov_get_evaluate()
{
	TypeHolder result; result.pointer = nullptr;
	result.real = Fov::GetFieldOfView();
	return result.pointer;
}

static void* scripting_switch_unit_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index unit_index;
		cstring tag_name;
	}*args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;
	result.boolean = false;

	if (!args->unit_index.IsNull())
	{
		auto* unit = blam::object_try_and_get_and_verify_type<s_unit_datum>(args->unit_index);
		if (!unit)
		{
			return result.pointer;
		}
		result.boolean = SwitchUnit(unit, args->tag_name);
	}
	return result.pointer;;
}

static void* scripting_switch_weapon_evaluate(void** arguments)
{
	struct s_arguments {
		datum_index unit_index;
		int32 weapon_index;
		datum_index name_index;
		datum_index item_index;
	}*args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;
	result.boolean = false;

	if (!args->unit_index.IsNull())
	{
		auto* unit = blam::object_try_and_get_and_verify_type<s_unit_datum>(args->unit_index);
		if (!unit)
		{
			return result.pointer;
		}
		result.boolean = true;
	}
	return result.pointer;
}
	};
};