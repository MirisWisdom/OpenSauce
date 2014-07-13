#include "include/effect/effect.fx"

Technique EffectNonlinearTintMultiply
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(false, effect_tint_nonlinear, effect_blend_multiply, false);
 	}
}

#ifdef INCLUDE_SOFT
Technique EffectNonlinearTintMultiplySoft
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(false, effect_tint_nonlinear, effect_blend_multiply, true);
 	}
}
#endif