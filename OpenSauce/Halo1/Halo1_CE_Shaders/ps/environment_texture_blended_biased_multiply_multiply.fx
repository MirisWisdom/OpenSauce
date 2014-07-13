#include "include/environment/environment_texture.fx"

Technique EnvironmentTextureBlendedBiasedMultiplyMultiply
{
    Pass P0
	{
#ifdef pc
		ColorOp[0]			= Disable;
		AlphaOp[0]			= Disable;
#endif		
		PixelShader = compile TGT EnvironmentTexture(environment_texture_blended, environment_texture_blend_biased_multiply, environment_texture_blend_multiply);
 	}
}
