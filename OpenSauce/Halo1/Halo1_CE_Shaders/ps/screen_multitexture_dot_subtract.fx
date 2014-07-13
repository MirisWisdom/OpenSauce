#include "include/screen/screen.fx"

Technique ScreenMultitextureDotSubtract
{
    Pass P0
	{
 		PixelShader = compile TGT ScreenMultitexture(screen_blend_dot, screen_blend_subtract);
 	}
}
