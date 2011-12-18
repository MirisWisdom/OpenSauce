/*
	Yelo: Open Sauce SDK
		Halo 1 (CE) Edition

	See license\OpenSauce\Halo1_CE for specific license information
*/
namespace EvaluateRuntimeValue
{
	//typedef void (*t_evaluate_runtime_value)(void*);

	static void Color_PlayerTeamColor(void* data)
	{
		Objects::s_unit_datum* player_unit = Players::GetPlayerUnit(Players::LocalPlayerIndex());

		if(!player_unit)
			return;

		//TODO: this doesn't seem to work quite right, doesn't always match the units colour
		real_rgb_color* unit_color = &player_unit->object.GetChangeColors()[2];

		D3DXCOLOR* color = CAST_PTR(D3DXCOLOR*, data);
		color->a = 1.0f;
		color->r = unit_color->red;
		color->g = unit_color->green;
		color->b = unit_color->blue;
	}
};