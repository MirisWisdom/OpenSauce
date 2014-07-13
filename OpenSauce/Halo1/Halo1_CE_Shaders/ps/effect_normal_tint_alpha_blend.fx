#include "include/effect/effect.fx"

Technique EffectNormalTintAlphaBlend
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(false, effect_tint_normal, effect_blend_alpha_blend, false);
 	}
}

#ifdef INCLUDE_SOFT
Technique EffectNormalTintAlphaBlendSoft
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(false, effect_tint_normal, effect_blend_alpha_blend, true);
 	}
}
#endif