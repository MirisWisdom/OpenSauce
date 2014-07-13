#include "include/util/interpolation.fx"

float4 c0 : register(c0);
float4 c1 : register(c1);
float4 c2 : register(c2);
float4 c3 : register(c3);
float4 c4 : register(c4);
float4 c5 : register(c5);

sampler TexS0 : register(s[0]);

// PASS
half4 P0(half2 Tex0 : TEXCOORD0) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	half4 t0 = tex2D(TexS0, Tex0);

	// c0 - meter gradient min color
	// c1 - meter gradient max color / meter gradient color
	// c2 - meter gradient max color
	// c3 - meter flash color
	// c4 - meter background color
	// c5 - meter tint color

	t0.rgb = t0.a;
	t0.a = t0.b;
	r0.rgb = (c0.a + -t0) * 4;
	r0.a = c1.a * t0.b;
	r1.a = saturate((r0.a + r0.a) * 4);
	r0.rgb = Lerp(r1.w, c2, c0);
	r0.a = -(2 * (r0.b + 0.5));
	r0.rgb = (r0.a * c3.rgb) + r0;
	r0.a = t0.b + c3.a;
	r0.rgb = lerp(r0, c4.rgb, step(0.5, r0.a));
	r0.a = lerp(c5.a, c4.a, step(0.5, r0.a));
	r0 = (r0 * t0.a) * 2;
	
	return r0;
}

Technique ScreenMeter
{
    Pass P0
	{
#ifdef pc
		ColorOp[0]			= Disable;
		AlphaOp[0]			= Disable;
#endif		
 		PixelShader = compile TGT P0();
 	}
}
