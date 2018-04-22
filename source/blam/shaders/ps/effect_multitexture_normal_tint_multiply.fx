#include "include/effect/effect.fx"

Technique EffectMultitextureNormalTintMultiply
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(true, effect_tint_normal, effect_blend_multiply, false);
 	}
}

#ifdef INCLUDE_SOFT
Technique EffectMultitextureNormalTintMultiplySoft
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(true, effect_tint_normal, effect_blend_multiply, true);
 	}
}
#endif