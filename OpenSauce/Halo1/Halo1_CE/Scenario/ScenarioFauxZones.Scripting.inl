/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/

static void* scripting_scenario_faux_zones_reset_evaluate()
{
	ScenarioFauxZones::Reset();

	return NULL;
}

static void* scripting_scenario_faux_zone_current_switch_variant_evaluate(void** arguments)
{
	struct s_arguments {
		cstring variant_name;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;

	result.boolean = ScenarioFauxZones::SwitchCurrentZoneVariant(args->variant_name);

	return result.pointer;
}
static void* scripting_scenario_faux_zone_switch_variant_evaluate(void** arguments)
{
	struct s_arguments {
		cstring zone_name;
		cstring variant_name;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;

	result.boolean = ScenarioFauxZones::SwitchZoneVariant(args->zone_name, args->variant_name);

	return result.pointer;
}
static void* scripting_scenario_faux_zone_switch_sky_evaluate(void** arguments)
{
	struct s_arguments {
		cstring zone_sky_name;
	}* args = CAST_PTR(s_arguments*, arguments);
	TypeHolder result; result.pointer = NULL;

	result.boolean = ScenarioFauxZones::SwitchZoneSky(args->zone_sky_name);

	return result.pointer;
}