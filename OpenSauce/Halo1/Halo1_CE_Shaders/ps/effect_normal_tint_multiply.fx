#include "include/effect/effect.fx"

Technique EffectNormalTintMultiply
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(false, effect_tint_normal, effect_blend_multiply, false);
 	}
}

#ifdef INCLUDE_SOFT
Technique EffectNormalTintMultiplySoft
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(false, effect_tint_normal, effect_blend_multiply, true);
 	}
}
#endif