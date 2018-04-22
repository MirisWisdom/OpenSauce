#include "include/effect/effect.fx"

Technique EffectMultitextureNonlinearTint
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(true, effect_tint_nonlinear, effect_blend_none, false);
 	}
}

#ifdef INCLUDE_SOFT
Technique EffectMultitextureNonlinearTintSoft
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(true, effect_tint_nonlinear, effect_blend_none, true);
 	}
}
#endif