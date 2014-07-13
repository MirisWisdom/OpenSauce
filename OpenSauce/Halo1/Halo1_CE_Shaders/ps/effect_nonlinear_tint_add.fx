#include "include/effect/effect.fx"

Technique EffectNonlinearTintAdd
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(false, effect_tint_nonlinear, effect_blend_add, false);
 	}
}

#ifdef INCLUDE_SOFT
Technique EffectNonlinearTintAddSoft
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(false, effect_tint_nonlinear, effect_blend_add, true);
 	}
}
#endif