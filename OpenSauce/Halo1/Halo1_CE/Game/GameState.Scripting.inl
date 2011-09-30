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

//////////////////////////////////////////////////////////////////////////
// physics constants

static void* scripting_physics_get_gravity_evaluate()
{
	TypeHolder result; result.pointer = NULL;

	result.real = GameState::Physics()->gravity;

	return result.pointer;
}

static void* scripting_physics_set_gravity_evaluate(void** arguments)
{
	struct s_arguments {
		real gravity_fraction;
	}* args = CAST_PTR(s_arguments*, arguments);

	GameState::Physics()->gravity = GameState::s_physics_globals::GravityConstant() * args->gravity_fraction;

	return NULL;
}

static void* scripting_physics_constants_reset_evaluate()
{
	GameState::Physics()->Reset();

	return NULL;
}


//////////////////////////////////////////////////////////////////////////
// RuntimeData

static void* scripting_runtime_integers_reset_evaluate()
{
	RuntimeData::IntegersReset();

	return NULL;
}

static void* scripting_runtime_integer_get_evaluate(void** arguments)
{
	struct s_arguments {
		int16 value_index;
		PAD16;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;

	result.int32 = RuntimeData::IntegerGet(args->value_index);

	return result.pointer;
}

static void* scripting_runtime_integer_set_evaluate(void** arguments)
{
	struct s_arguments {
		int16 value_index;
		PAD16;
		int32 value;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;

	result.int32 = RuntimeData::IntegerGetAndSet(args->value_index, args->value);

	return result.pointer;
}

static void* scripting_runtime_integer_inc_evaluate(void** arguments)
{
	struct s_arguments {
		int16 value_index;
		PAD16;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;

	result.int32 = RuntimeData::IntegerGetAndIncrement(args->value_index);

	return result.pointer;
}

static void* scripting_runtime_integer_dec_evaluate(void** arguments)
{
	struct s_arguments {
		int16 value_index;
		PAD16;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;

	result.int32 = RuntimeData::IntegerGetAndDecrement(args->value_index);

	return result.pointer;
}

namespace RuntimeData
{
	static void InitializeScripting()
	{
		Scripting::InitializeScriptFunction(Enums::_hs_function_runtime_integers_reset, 
			scripting_runtime_integers_reset_evaluate);
		Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_runtime_integer_get, 
			scripting_runtime_integer_get_evaluate);
		Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_runtime_integer_set, 
			scripting_runtime_integer_set_evaluate);
		Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_runtime_integer_inc, 
			scripting_runtime_integer_inc_evaluate);
		Scripting::InitializeScriptFunctionWithParams(Enums::_hs_function_runtime_integer_dec, 
			scripting_runtime_integer_dec_evaluate);
	}
};