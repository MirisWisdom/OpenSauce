#include "include/effect/effect.fx"

Technique EffectMultitextureNormalTintDoubleMultiply
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(true, effect_tint_normal, effect_blend_double_multiply, false);
 	}
}

#ifdef INCLUDE_SOFT
Technique EffectMultitextureNormalTintDoubleMultiplySoft
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(true, effect_tint_normal, effect_blend_double_multiply, true);
 	}
}
#endif