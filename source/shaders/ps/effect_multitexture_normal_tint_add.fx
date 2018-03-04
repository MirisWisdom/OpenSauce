#include "include/effect/effect.fx"

Technique EffectMultitextureNormalTintAdd
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(true, effect_tint_normal, effect_blend_add, false);
 	}
}

#ifdef INCLUDE_SOFT
Technique EffectMultitextureNormalTintAddSoft
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(true, effect_tint_normal, effect_blend_add, true);
 	}
}
#endif