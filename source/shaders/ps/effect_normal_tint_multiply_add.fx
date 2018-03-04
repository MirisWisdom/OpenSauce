#include "include/effect/effect.fx"

Technique EffectNormalTintMultiplyAdd
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(false, effect_tint_normal, effect_blend_multiply_add, false);
 	}
}

#ifdef INCLUDE_SOFT
Technique EffectNormalTintMultiplyAddSoft
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(false, effect_tint_normal, effect_blend_multiply_add, true);
 	}
}
#endif