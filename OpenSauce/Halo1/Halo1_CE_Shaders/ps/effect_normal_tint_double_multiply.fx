#include "include/effect/effect.fx"

Technique EffectNormalTintDoubleMultiply
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(false, effect_tint_normal, effect_blend_double_multiply, false);
 	}
}

#ifdef INCLUDE_SOFT
Technique EffectNormalTintDoubleMultiplySoft
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(false, effect_tint_normal, effect_blend_double_multiply, true);
 	}
}
#endif