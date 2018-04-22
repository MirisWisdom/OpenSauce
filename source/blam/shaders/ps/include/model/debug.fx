#include "include/model/model_shared_variables.fx"
#include "include/model/normals.fx"
#include "include/model/reflection.fx"
#include "include/model/lighting.fx"
#include "include/model/specular_color.fx"

static const int model_debug_albedo = 0;
static const int model_debug_multipurpose = 1;
static const int model_debug_normals = 2;
static const int model_debug_lighting = 3;
static const int model_debug_specular = 4;
static const int model_debug_specular_lighting = 5;
static const int model_debug_reflection = 6;

float4 ModelDebug(PS_INPUT IN,
	uniform const int Type)
{
	half4 T0 = tex2D( TexSampler0, IN.DiffuseMultiUV.xy );
	half4 T1 = tex2D( TexSampler1, IN.DetailUV.xy );
	half4 T2 = tex2D( TexSampler2, IN.DiffuseMultiUV.xy );
	half4 T4 = tex2D( TexSampler4, IN.DiffuseMultiUV.xy );
	half4 T5 = tex2D( TexSampler5, IN.NormalDetailUV.xy );
	half4 T6 = tex2D( TexSampler6, IN.NormalDetailUV.zw );
	half4 T7 = tex2D( TexSampler7, IN.DiffuseMultiUV.xy );
	
	if(Type == model_debug_albedo)
	{
		return float4(T0.rgb, 1);
	}
	else if(Type == model_debug_multipurpose)
	{
		return float4(T2.rgb, 1);
	}
	else
	{
		// get the world space normals
		float3 Normals = GetNormals(bBaseNorm, bDetailNorm,	IN.TBNTranspose, T4, T5.xy, T6.xy);
		
		if(Type == model_debug_normals)
		{	
			return float4(Normals, 1);
		}
		else if(Type == model_debug_lighting)
		{
			float3 EyeVector = c_eye_view_position.xyz - IN.Position3D;
			float3 EyeVectorNorm = normalize(EyeVector);
			
			// calculate the lighting contributions
			return float4(GetLighting(true, true, Normals, EyeVectorNorm, IN.Position3D, IN.DetailUV.w, T7.w, T2.b), 1);		
		}
		else if(Type == model_debug_specular)
		{
		
		}
		else if(Type == model_debug_reflection)
		{
		
		}
	}
}