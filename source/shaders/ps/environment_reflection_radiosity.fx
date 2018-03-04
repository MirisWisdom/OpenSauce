half4 P0() : COLOR
{
   half4 c7 = half4(1.0,0.0,0.0,1.0);

   return c7;
};

Technique EnvironmentReflectionRadiosity
{
   Pass P0
   {
      PixelShader = compile TGT P0();
   }
}
