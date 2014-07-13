#include "include/effect/effect.fx"

Technique EffectMultitextureNormalTintMultiplyAdd
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(true, effect_tint_normal, effect_blend_multiply_add, false);
 	}
}

#ifdef INCLUDE_SOFT
Technique EffectMultitextureNormalTintMultiplyAddSoft
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(true, effect_tint_normal, effect_blend_multiply_add, true);
 	}
}
#endif