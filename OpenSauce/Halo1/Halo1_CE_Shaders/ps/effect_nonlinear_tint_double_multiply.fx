#include "include/effect/effect.fx"

Technique EffectNonlinearTintDoubleMultiply
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(false, effect_tint_nonlinear, effect_blend_double_multiply, false);
 	}
}

#ifdef INCLUDE_SOFT
Technique EffectNonlinearTintDoubleMultiplySoft
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(false, effect_tint_nonlinear, effect_blend_double_multiply, true);
 	}
}
#endif