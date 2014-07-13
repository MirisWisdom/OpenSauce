#include "include/effect/effect.fx"

Technique EffectMultitextureNonlinearTintMultiply
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(true, effect_tint_nonlinear, effect_blend_multiply, false);
 	}
}

#ifdef INCLUDE_SOFT
Technique EffectMultitextureNonlinearTintMultiplySoft
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(true, effect_tint_nonlinear, effect_blend_multiply, true);
 	}
}
#endif