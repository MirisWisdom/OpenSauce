#include "include/effect/effect.fx"

Technique EffectNonlinearTintAlphaBlend
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(false, effect_tint_nonlinear, effect_blend_alpha_blend, false);
 	}
}

#ifdef INCLUDE_SOFT
Technique EffectNonlinearTintAlphaBlendSoft
{
    Pass P0
	{
		PixelShader = compile TGT PS_Effect(false, effect_tint_nonlinear, effect_blend_alpha_blend, true);
 	}
}
#endif