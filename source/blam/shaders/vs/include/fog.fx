#ifndef FOG_FX
#define FOG_FX

half Fog_Complex(float4 v0)
{
   half4 a0, r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11; 
   
   R_PLANAR_FOG_VERTEX_DENSITY = dot(V_POSITION, c_planar_fog_gradient1);							// x
   R_PLANAR_FOG_EYE_DISTANCE = dot(V_POSITION, c_planar_fog_gradient2);								// y
   R_ATMOSPHERIC_FOG_DENSITY = dot(V_POSITION, c_atmospheric_fog_gradient);							// z
   R_FOG_DENSITY.xy = V_ONE + -R_FOG_DENSITY;														// {1 - x, 1 - y}
   R_FOG_DENSITY.xyz = max(R_FOG_DENSITY.xyz, V_ZERO);												// clamp to zero
   R_FOG_DENSITY.xy = (R_FOG_DENSITY.xy) * (R_FOG_DENSITY);											// {(1 - x)^2, (1 - y)^2}
   R_FOG_DENSITY.xyz = min(R_FOG_DENSITY.xyz, V_ONE);												// clamp to one
   R_FOG_DENSITY.x = R_FOG_DENSITY.x + R_FOG_DENSITY.y;												// (1 - x)^2 + (1 - y)^2
   R_FOG_DENSITY.x = min(R_FOG_DENSITY.x, V_ONE);													// clamp to one
   R_FOG_DENSITY.xy = V_ONE + -R_FOG_DENSITY;														// {1 - (1 - x)^2 - (1 - y)^2, 1 - (1 - y)^2}
   R_FOG_DENSITY.xy = (R_FOG_DENSITY.xy) * (R_FOG_DENSITY);											// {(1 - (1 - x)^2 - (1 - y)^2)^2, (1 - (1 - y)^2)^2}
   R_FOG_DENSITY.y = R_FOG_DENSITY.y + -R_FOG_DENSITY.x;   
   R_PLANAR_FOG_DENSITY = (c_planar_fog_eye_density) * (R_FOG_DENSITY.y) + R_FOG_DENSITY.x;   
   R_PLANAR_FOG_DENSITY = (R_PLANAR_FOG_DENSITY) * (c_planar_fog_max_density);					// Pf
   R_ATMOSPHERIC_FOG_DENSITY = (R_ATMOSPHERIC_FOG_DENSITY) * (c_atmospheric_fog_max_density);	// Af
   R_FOG_DENSITY.xyzw = -R_FOG_DENSITY.xyzw + V_ONE;												// (1 - Af),(1 - Pf)
   return (R_ATMOSPHERIC_FOG_DENSITY) * (R_PLANAR_FOG_DENSITY);										// (1 - Af)*(1 - Pf)
}

#endif