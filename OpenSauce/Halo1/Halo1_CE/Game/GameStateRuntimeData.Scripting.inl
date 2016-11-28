/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

//////////////////////////////////////////////////////////////////////////
// integers
static void* scripting_runtime_integers_reset_evaluate()
{
	IntegersReset();

	return nullptr;
}

static void* scripting_runtime_integer_get_evaluate(void** arguments)
{
	struct s_arguments {
		int16 value_index;
		PAD16;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

	result.int32 = IntegerGet(args->value_index);

	return result.pointer;
}

static void* scripting_runtime_integer_set_evaluate(void** arguments)
{
	struct s_arguments {
		int16 value_index;
		PAD16;
		int32 value;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

	result.int32 = IntegerGetAndSet(args->value_index, args->value);

	return result.pointer;
}

static void* scripting_runtime_integer_inc_evaluate(void** arguments)
{
	struct s_arguments {
		int16 value_index;
		PAD16;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

	result.int32 = IntegerGetAndIncrement(args->value_index);

	return result.pointer;
}

static void* scripting_runtime_integer_dec_evaluate(void** arguments)
{
	struct s_arguments {
		int16 value_index;
		PAD16;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

	result.int32 = IntegerGetAndDecrement(args->value_index);

	return result.pointer;
}

static void* scripting_runtime_integer_operation_evaluate(void** arguments)
{
	struct s_arguments {
		int16 value_index;
		PAD16;
		cstring op_name;
		int32 op_value;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

	result.int32 = IntegerDoOperation(args->value_index, args->op_name, args->op_value);

	return result.pointer;
}


//////////////////////////////////////////////////////////////////////////
// vectors
static void* scripting_runtime_vectors_reset_evaluate()
{
	VectorsReset();

	return nullptr;
}
static void* scripting_runtime_vector_get_element_evaluate(void** arguments)
{
	struct s_arguments {
		int16 value_index;
		PAD16;
		int16 element_index;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

	result.real = VectorGetElement(args->value_index, args->element_index);

	return result.pointer;
}
static void* scripting_runtime_vector_set_element_evaluate(void** arguments)
{
	struct s_arguments {
		int16 value_index;
		PAD16;
		int16 element_index;
		PAD16;
		cstring op_name;
		real op_value;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

	result.boolean = VectorSetElement(args->value_index, args->element_index,
		args->op_name, args->op_value);

	return result.pointer;
}
static void* scripting_runtime_vector_set_evaluate(void** arguments)
{
	struct s_arguments {
		int16 value_index;
		PAD16;
		cstring op_name;
		real op_x; real op_y; real op_z;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

	result.boolean = VectorSet(args->value_index, args->op_name, args->op_x, args->op_y, args->op_z);

	return result.pointer;
}
static void* scripting_runtime_vector_operation_evaluate(void** arguments)
{
	struct s_arguments {
		int16 value_index;
		PAD16;
		cstring op_name;
		int16 op_arg_vector_index;
		PAD16;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

	result.boolean = VectorDoOperation(args->value_index, args->op_name, args->op_arg_vector_index);

	return result.pointer;
}
static void* scripting_runtime_vector_to_string_evaluate(void** arguments)
{
	struct s_arguments {
		int16 value_index;
		PAD16;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = nullptr;

	result.pointer = VectorToString(args->value_index);

	return result.pointer;
}