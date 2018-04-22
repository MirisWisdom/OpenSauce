float4 c0 : register(c0);
float4 c1 : register(c1);
   
half4 PS_FlashInvert() : COLOR
{
   half4 r0, r1;
   
   half4 c7 = half4(1, 1, 1, 0);
   
   r1 = c7 * c0;
   r0.xyz = (c0 * (2.0f * (r1 - 0.5f))) + c0.w;
   r0.w = c7;
   
   return r0;
}

Technique FlashInvert
{
   Pass P0
   {
      PixelShader = compile TGT PS_FlashInvert();
   }
}