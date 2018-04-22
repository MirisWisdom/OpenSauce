#include "include/screen/screen.fx"

Technique ScreenMultitextureDotAdd
{
    Pass P0
	{
#ifdef pc
		ColorOp[0]			= Disable;
		AlphaOp[0]			= Disable;
#endif		
 		PixelShader = compile TGT ScreenMultitexture(screen_blend_dot, screen_blend_add);
 	}
}

