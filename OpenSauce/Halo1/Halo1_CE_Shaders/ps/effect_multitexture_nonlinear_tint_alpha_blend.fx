#include "include/effect/effect.fx"

Technique EffectMultitextureNonlinearTintAlphaBlend
{
    Pass P0
	{
#ifdef pc   
ColorOp[0]	= Disable;
AlphaOp[0]	= Disable;
#endif
		PixelShader = compile TGT PS_Effect(true, effect_tint_nonlinear, effect_blend_alpha_blend, false);
 	}
}

#ifdef INCLUDE_SOFT
Technique EffectMultitextureNonlinearTintAlphaBlendSoft
{
    Pass P0
	{
#ifdef pc   
ColorOp[0]	= Disable;
AlphaOp[0]	= Disable;

#endif
		PixelShader = compile TGT PS_Effect(true, effect_tint_nonlinear, effect_blend_alpha_blend, true);
 	}
}
#endif