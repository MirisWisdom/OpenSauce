#include "include/screen/screen.fx"

Technique ScreenMultitextureMultiply2xSubtract
{
    Pass P0
	{
#ifdef pc
		ColorOp[0]			= Disable;
		AlphaOp[0]			= Disable;
#endif		
 		PixelShader = compile TGT ScreenMultitexture(screen_blend_multiply2x, screen_blend_subtract);
 	}
}
