sampler TexS0 : register(s[0]);
sampler TexS1 : register(s[1]);
sampler TexS2 : register(s[2]);
sampler TexS3 : register(s[3]);

float4 c_desaturation_tint : register(c[0]);
float4 c_light_enhancement : register(c[1]);

// PASS (Just going to trust this one :|)

half3 FilterTextures( half4 t0, half4 t1, half4 t2, half4 t3 )
{
	half4 r0;
	
	r0 = (t2-0.5)+(t1-0.5);
	r0 = r0*0.33333333;
	r0 = ((t3-0.5)*0.33333333)+r0;
	r0 = r0+0.5;
	r0 = lerp( t1, r0, t0.a );

	return (half3)r0;
}

///////////////////////////////////////////////////////////////////////////////
// Pixel Shader 2.0 shaders
///////////////////////////////////////////////////////////////////////////////
half4 PS_VideoOffFilter(
    half2 Tex0  : TEXCOORD0,
    half2 Tex1  : TEXCOORD1,
    half2 Tex2  : TEXCOORD2,
    half2 Tex3  : TEXCOORD3,
    uniform bool bUseConvolutionMask,
    uniform bool bLightUsesConvolution,
    uniform bool bDesaturationUsesConvolution) : COLOR
{
	half4 T0 = tex2D(TexS0, Tex0);
	half4 T1 = tex2D(TexS1, Tex1);
	half4 T2 = tex2D(TexS2, Tex2);
	half4 T3 = tex2D(TexS3, Tex3);
	
	half3 R0	= FilterTextures(T0,T1,T2,T3);
	half  R0a	= bLightUsesConvolution ? c_light_enhancement.a * -((2*T0.b)-1) : c_light_enhancement.a;
	half3 R1	= c_light_enhancement.a ? pow(1-R0,4) : R0;
	half  R1a	= bDesaturationUsesConvolution ? c_desaturation_tint.a * -((2*T0.b)-1) : c_desaturation_tint.a;

	// Boy this hurts.
	R0	= c_desaturation_tint.a ? (c_light_enhancement.a ? lerp( R0, 1-R1, R0a) : R0) : R0;
	
	R1	= dot( R0, 0.33333333 );
	
	half3	SRCCOLOR = ((c_desaturation_tint * R1) * R1a) + R0;
	half	SRCALPHA = bLightUsesConvolution ? T0.b : 0.0;
	
	return half4( SRCCOLOR, SRCALPHA );
}

Technique VideoOffConvolvedMaskFilterLightAndDesaturation
{
    Pass P0
	{
		PixelShader = compile TGT PS_VideoOffFilter( true, true, true );
 	}
}

Technique VideoOffConvolvedMaskFilterLight
{
    Pass P0
	{
		PixelShader = compile TGT PS_VideoOffFilter( true, true, false );
 	}
}

Technique VideoOffConvolvedMaskFilterDesaturation
{
    Pass P0
	{
		PixelShader = compile TGT PS_VideoOffFilter( true, true, true );
 	}
}

Technique VideoOffConvolvedFilterLightAndDesaturation
{
    Pass P0
	{
		PixelShader = compile TGT PS_VideoOffFilter( false, false, true );
 	}
}

Technique VideoOffConvolvedFilterLight_ps
{
    Pass P0
	{
		PixelShader = compile TGT PS_VideoOffFilter( false, false, false );
 	}
}

Technique VideoOffConvolvedFilterDesaturation
{
    Pass P0
	{
		PixelShader = compile TGT PS_VideoOffFilter( false, false, true );
 	}
}

///////////////////////////////////////////////////////////////////////////////
// Pixel Shader 1.1 shaders
///////////////////////////////////////////////////////////////////////////////
half4 PS_VideoOnPass0(half2 Tex0 : TEXCOORD0) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	half4 t0 = tex2D(TexS0, Tex0);

	r0 = t0;
	
	return r0;
}

half4 PS_VideoOnPass1(half2 Tex0 : TEXCOORD0, half2 Tex1 : TEXCOORD1, half2 Tex2 : TEXCOORD2) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	half4 c2 = half4(1.0,1.0,1.0,1.0);

	half4 t0 = tex2D(TexS0, Tex0);// screen
	half4 t1 = tex2D(TexS1, Tex1);// scanline
	half4 t2 = tex2D(TexS2, Tex2);// noise


	r1 = c2 + (c_desaturation_tint.a) * (t2 - (c2));
	t1 = saturate((r1) * (t1));
	r0 = (t0) * (t0);
	r0 = (r0) * (r0);
	r0 = (r0) * (c_desaturation_tint);
	r1 = (t0) * (c_desaturation_tint);
	r0.rgb = (r0 + (t1.rgb) * (r1.rgb - (r0))) * 2;

	return r0;
}

Technique VideoOnPass0
{
	Pass P0
	{
		PixelShader = compile TGT PS_VideoOnPass0();
	}
}

Technique VideoOnPass1
{
	Pass P0
	{
		PixelShader = compile TGT PS_VideoOnPass1();
	}
}

half4 PS_VideoOffNonConvolved(half2 Tex0 : TEXCOORD0, half2 Tex1 : TEXCOORD1, half2 Tex2 : TEXCOORD2) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	half4 t0 = tex2D(TexS0, Tex0);

	r0.rgb = t0;
	r0.a = t0.b;
	
	return r0;
}

half4 PS_VideoOffNonConvolvedPassThrough(half2 Tex0 : TEXCOORD0, half2 Tex1 : TEXCOORD1, half2 Tex2 : TEXCOORD2) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	half4 t0 = tex2D(TexS0, Tex0);

	r0.rgb = t0;
	r0.a = c_desaturation_tint.a;
	
	return r0;
}

half4 PS_VideoOffNonConvolvedMask(half2 Tex0 : TEXCOORD0, half2 Tex1 : TEXCOORD1, half2 Tex2 : TEXCOORD2) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	half4 t0 = tex2D(TexS0, Tex0);
	half4 t1 = tex2D(TexS1, Tex1);

	r0.rgb = t1;
	r0.a = t0.b;
	
	return r0;
}

Technique VideoOffNonConvolved
{
	Pass P0
	{
		PixelShader = compile TGT PS_VideoOffNonConvolved();
	}
}

Technique VideoOffNonConvolvedPassThrough
{
	Pass P0
	{
		PixelShader = compile TGT PS_VideoOffNonConvolvedPassThrough();
	}
}

Technique VideoOffNonConvolvedMask
{
	Pass P0
	{
		PixelShader = compile TGT PS_VideoOffNonConvolvedMask();
	}
}

half4 PS_VideoOffConvolvedMaskThreeStage(half2 Tex0 : TEXCOORD0, half2 Tex1 : TEXCOORD1, half2 Tex2 : TEXCOORD2, half2 Tex3 : TEXCOORD3) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	half4 c0 = half4(0.3333333333333333,0.3333333333333333,0.3333333333333333,0.5);
	half4 c1 = half4(0.1,0.1,0.1,0.1);

	// even passes render to offscreen surface
	// odd passes render to backbuffer

	half4 t0 = tex2D(TexS0, Tex0);// convolution mask
	half4 t1 = tex2D(TexS1, Tex1);// backbuffer on even passes, offscreen surface on odd passes
	half4 t3 = tex2D(TexS1, Tex3);// backbuffer on even passes, offscreen surface on odd passes

	r0.rgb = (t1-0.5).rgb + (t1-0.5);// (T1+T2-1)
	r0.rgb = (r0.rgb) * (c0);// (T1+T2-1)/3
	r0.rgb = ((t3-0.5).rgb) * (c0.rgb) + r0;// (T1+T2-1)/3 + (T3-1/2)/3 == (T1+T2+T3)/3 - 1/2
	r0.rgb = r0.rgb + c0.a;// (T1+T2+T3)/3
	r0.rgb = t1 + (t0.a) * (r0.rgb - (t1));// (1-T0a)*T1 + T0a*(T1+T2+T3)/3
	r0.a = t0.b;
	
	return r0;
}

Technique VideoOffConvolvedMaskThreeStage
{
    Pass P0
	{
		PixelShader = compile TGT PS_VideoOffConvolvedMaskThreeStage();
 	}
}

half4 PS_VideoOffConvolvedMask(half2 Tex0 : TEXCOORD0, half2 Tex1 : TEXCOORD1, half2 Tex2 : TEXCOORD2, half2 Tex3 : TEXCOORD3) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	half4 c0 = half4(0.3333333333333333,0.3333333333333333,0.3333333333333333,0.5);
	half4 c1 = half4(0.1,0.1,0.1,0.1);

	// even passes render to offscreen surface
	// odd passes render to backbuffer

	half4 t0 = tex2D(TexS0, Tex0);// convolution mask
	half4 t1 = tex2D(TexS1, Tex1);// backbuffer on even passes, offscreen surface on odd passes
	half4 t2 = tex2D(TexS2, Tex2);// backbuffer on even passes, offscreen surface on odd passes
	half4 t3 = tex2D(TexS3, Tex3);// backbuffer on even passes, offscreen surface on odd passes

	r0.rgb = (t1-0.5).rgb + (t2-0.5);// (T1+T2-1)
	r0.rgb = (r0.rgb) * (c0);// (T1+T2-1)/3
	r0.rgb = ((t3-0.5).rgb) * (c0.rgb) + r0;// (T1+T2-1)/3 + (T3-1/2)/3 == (T1+T2+T3)/3 - 1/2
	r0.rgb = r0.rgb + c0.a;// (T1+T2+T3)/3
	r0.rgb = t1 + (t0.a) * (r0.rgb - (t1));// (1-T0a)*T1 + T0a*(T1+T2+T3)/3
	r0.a = t0.b;
	
	return r0;
}

Technique VideoOffConvolvedMask
{
    Pass P0
	{
		PixelShader = compile TGT PS_VideoOffConvolvedMask();
 	}
}

half4 PS_VideoOffConvolved(half2 Tex0 : TEXCOORD0, half2 Tex1 : TEXCOORD1, half2 Tex2 : TEXCOORD2, half2 Tex3 : TEXCOORD3) : COLOR
{
	half4 r0, r1, r2, r3, r4, r5, r6;

	half4 c7 = half4(0.0,0.0,0.0,0.5);

	half4 t0 = tex2D(TexS0, Tex0);
	half4 t1 = tex2D(TexS1, Tex1);
	half4 t2 = tex2D(TexS2, Tex2);
	half4 t3 = tex2D(TexS3, Tex3);

	r0 = ((t0-0.5) + (t1-0.5)) / 2;// (T0+T1-1)/2
	r1 = ((t2-0.5) + (t3-0.5)) / 2;// (T2+T3-1)/2
	r0 = (r0 + r1) / 2;// (T0+T1+T2+T3)/4
	r0.rgb = r0.rgb + c7.a;
	r0.a = (c_desaturation_tint.a) * (c7.b);
	
	return r0;
}

Technique VideoOffConvolved
{
    Pass P0
	{
		PixelShader = compile TGT PS_VideoOffConvolved();
 	}
}