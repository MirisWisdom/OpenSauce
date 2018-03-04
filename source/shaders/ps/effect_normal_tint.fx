#include "include/effect/effect.fx"

Technique EffectNormalTint
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(false, effect_tint_normal, effect_blend_none, false);
 	}
}

#ifdef INCLUDE_SOFT
Technique EffectNormalTintSoft
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(false, effect_tint_normal, effect_blend_none, true);
 	}
}
#endif