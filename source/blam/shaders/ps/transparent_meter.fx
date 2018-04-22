#include "include/util/interpolation.fx"

float4 c0 : register(c0);
float4 c1 : register(c1);
float4 c2 : register(c2);
float4 c3 : register(c3);
float4 c4 : register(c4);
float4 c5 : register(c5);
   
sampler TexS0 : register(s[0]);
sampler TexS1 : register(s[1]);
sampler TexS2 : register(s[2]);
sampler TexS3 : register(s[3]);

// PASS
half4 P0(half2 Tex0 : TEXCOORD0) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	half4 t0 = tex2D(TexS0, Tex0);

	// c0 - meter_flash_color
	// c1 - meter_gradient_max_color
	// c2 - meter_gradient_min_color
	// c3 - meter_background_color
	// c4 - meter_tint_color
	// c5 - meter_flash_color_variable

	r1.rgb = c2.a;
	r1.a = c1.a * t0.b;
	r0.rgb = (c0.rgb - t0.rgb) * 4;
	r0.a = saturate((r1.a + r1.a) * 4);
	r0.rgb = Lerp(r0.w, c1, c2);
	r0.a = -(2 * (r0.b - 0.5));
	r0.rgb = (r0.a * c5.rgb) + r0.rgb;
	r0.a = t0.b -(r1.z - 0.5);
	r0.rgb = lerp(r0.rgb, c3.rgb, step(0.5, r0.a));
	r0.a = lerp(c4.a, c3.a, step(0.5, r0.a));
	r0.rgb = (r0.rgb) * (t0.a);
	
	return r0;
}

Technique TransparentMeter
{
    Pass P0
	{
 		PixelShader = compile TGT P0();
 	}
}