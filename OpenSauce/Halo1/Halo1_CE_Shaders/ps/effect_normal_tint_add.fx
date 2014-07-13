#include "include/effect/effect.fx"

Technique EffectNormalTintAdd
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(false, effect_tint_normal, effect_blend_add, false);
 	}
}

#ifdef INCLUDE_SOFT
Technique EffectNormalTintAddSoft
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(false, effect_tint_normal, effect_blend_add, true);
 	}
}
#endif