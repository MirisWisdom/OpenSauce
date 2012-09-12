/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
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

	GameState::Physics()->SetGravityScale(args->gravity_fraction);

	return NULL;
}

static void* scripting_physics_constants_reset_evaluate()
{
	GameState::Physics()->Reset();

	return NULL;
}

