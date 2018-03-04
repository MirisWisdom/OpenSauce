#include "include/screen/screen.fx"

Technique ScreenMultitextureAdd
{
    Pass P0
	{
#ifdef pc
		ColorOp[0]			= Disable;
		AlphaOp[0]			= Disable;
#endif		
 		PixelShader = compile TGT ScreenMultitexture(screen_blend_add, screen_blend_add);
 	}
}
