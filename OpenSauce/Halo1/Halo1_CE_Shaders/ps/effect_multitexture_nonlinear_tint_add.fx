#include "include/effect/effect.fx"

Technique EffectMultitextureNonlinearTintAdd
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(true, effect_tint_nonlinear, effect_blend_add, false);
 	}
}

#ifdef INCLUDE_SOFT
Technique EffectMultitextureNonlinearTintAddSoft
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(true, effect_tint_nonlinear, effect_blend_add, true);
 	}
}
#endif