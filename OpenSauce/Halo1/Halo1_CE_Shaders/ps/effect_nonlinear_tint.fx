#include "include/effect/effect.fx"

Technique EffectNonlinearTint
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(false, effect_tint_nonlinear, effect_blend_none, false);
 	}
}

#ifdef INCLUDE_SOFT
Technique EffectNonlinearTintSoft
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(false, effect_tint_nonlinear, effect_blend_none, true);
 	}
}
#endif