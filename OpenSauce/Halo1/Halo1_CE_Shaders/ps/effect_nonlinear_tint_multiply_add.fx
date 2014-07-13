#include "include/effect/effect.fx"

Technique EffectNonlinearTintMultiplyAdd
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(false, effect_tint_nonlinear, effect_blend_multiply_add, false);
 	}
}

#ifdef INCLUDE_SOFT
Technique EffectNonlinearTintMultiplyAddSoft
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(false, effect_tint_nonlinear, effect_blend_multiply_add, true);
 	}
}
#endif