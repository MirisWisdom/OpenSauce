#include "include/screen/screen.fx"

Technique ScreenMultitextureSubtractMultiply2x
{
    Pass P0
	{
#ifdef pc
		ColorOp[0]			= Disable;
		AlphaOp[0]			= Disable;
#endif		
 		PixelShader = compile TGT ScreenMultitexture(screen_blend_subtract, screen_blend_multiply2x);
 	}
}
