/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/

#define VARS_VIEWPROJ
#define VARS_TEXANIM
#define VARS_TEXSCALE
#define VARS_FAKELIGHT

#ifndef HALO_COMPILE
#include "environment_variables_fxc.inl"
#else
#include "environment_variables_halo.inl"
#endif

struct VS_INPUT{
	float4 Position 			: POSITION0;
	float3 Normal 				: NORMAL0;
	float3 BiNormal 			: BINORMAL0;
	float3 Tangent 				: TANGENT0;
	float3 Normal1 				: NORMAL1;
	float4 Tex0 				: TEXCOORD0;
	float4 Tex1 				: TEXCOORD0;
};

struct VS_OUTPUT{
	float4 Position				: POSITION;
	float4 Color0				: COLOR0;
	float2 Tex0					: TEXCOORD0;
	float2 Tex1					: TEXCOORD1;
	float2 Tex2					: TEXCOORD2;
	float3 Tex3					: TEXCOORD3;
};

VS_OUTPUT EnvironmentLightmap(in VS_INPUT IN)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;
	
	// (5) transform incident radiosity ------------------------------------------------------
	float4 IncidentRadiosity;
	IncidentRadiosity.x = dot(IN.Normal1, IN.Tangent);   
	IncidentRadiosity.y = dot(IN.Normal1, IN.BiNormal);   
	IncidentRadiosity.z = dot(IN.Normal1, IN.Normal);   
	IncidentRadiosity.w = dot(IN.Normal1, IN.Normal1);   

	// (4) output homogeneous point ----------------------------------------------------------
	OUT.Position = mul(IN.Position, c_world_view_projection);   

	// (1) output radiosity accuracy ---------------------------------------------------------
	OUT.Color0.w = IncidentRadiosity.w;   

	// (7) output texcoords ------------------------------------------------------------------
	float2 Texcoord;
	OUT.Tex0.x = dot(IN.Tex0, c_base_map_xform_x);   // base map
	OUT.Tex0.y = dot(IN.Tex0, c_base_map_xform_y);   
	OUT.Tex0 *= c_bump_scale;   // bump map
	OUT.Tex1.xy = OUT.Tex0 * c_self_illumination_scale; 
	OUT.Tex2.xy = IN.Tex1.xy;
	OUT.Tex3.xyz = normalize(IncidentRadiosity.xyz);

	// (9) output fake lighting ---------------------------------------------------------------
	float2 DistantAttenuation = c_fake_light_ambient;    
	DistantAttenuation.x = dot(IN.Normal, c_fake_light0_direction.xyz) * c_fake_light0_intensity;  
	DistantAttenuation.y = dot(IN.Normal, c_fake_light1_direction.xyz) * c_fake_light1_intensity;    
	DistantAttenuation = max(DistantAttenuation, 0.0f);	
	
	float3 FinalLighting = c_fake_light_ambient.rgb;
	FinalLighting += DistantAttenuation.x * c_fake_light_color.rgb;   
	FinalLighting += DistantAttenuation.y * c_fake_light_color.rgb;   
	
	OUT.Color0.xyz = FinalLighting;   
   
	return OUT;
}
