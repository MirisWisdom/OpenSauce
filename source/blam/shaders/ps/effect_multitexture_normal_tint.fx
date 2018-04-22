#include "include/effect/effect.fx"

Technique EffectMultitextureNormalTint
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(true, effect_tint_normal, effect_blend_none, false);
 	}
}

#ifdef INCLUDE_SOFT
Technique EffectMultitextureNormalTintSoft
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(true, effect_tint_normal, effect_blend_none, true);
 	}
}
#endif