#include "include/effect/effect.fx"

Technique EffectMultitextureNonlinearTintMultiplyAdd
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(true, effect_tint_nonlinear, effect_blend_multiply_add, false);
 	}
}

#ifdef INCLUDE_SOFT
Technique EffectMultitextureNonlinearTintMultiplyAddSoft
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(true, effect_tint_nonlinear, effect_blend_multiply_add, true);
 	}
}
#endif