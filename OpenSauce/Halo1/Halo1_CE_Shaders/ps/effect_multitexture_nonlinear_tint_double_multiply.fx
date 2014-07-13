#include "include/effect/effect.fx"

Technique EffectMultitextureNonlinearTintDoubleMultiply
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(true, effect_tint_nonlinear, effect_blend_double_multiply, false);
 	}
}

#ifdef INCLUDE_SOFT
Technique EffectMultitextureNonlinearTintDoubleMultiplySoft
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(true, effect_tint_nonlinear, effect_blend_double_multiply, true);
 	}
}
#endif