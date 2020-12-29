#include "include/environment/environment_texture.fx"

Technique EnvironmentTextureNormalMultiplyBiasedMultiply
{
    Pass P0
	{
#ifdef pc
		ColorOp[0]			= Disable;
		AlphaOp[0]			= Disable;
#endif		
		PixelShader = compile TGT EnvironmentTexture(environment_texture_normal, environment_texture_blend_multiply, environment_texture_blend_biased_multiply);
 	}
}
