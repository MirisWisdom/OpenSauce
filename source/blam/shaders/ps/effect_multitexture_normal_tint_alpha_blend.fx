#include "include/effect/effect.fx"

Technique EffectMultitextureNormalTintAlphaBlend
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(true, effect_tint_normal, effect_blend_alpha_blend, false);
 	}
}

#ifdef INCLUDE_SOFT
Technique EffectMultitextureNormalTintAlphaBlendSoft
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(true, effect_tint_normal, effect_blend_alpha_blend, true);
 	}
}
#endif