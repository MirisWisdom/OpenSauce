technique VS_BSP_N
{
	pass P0
	{
		VertexShader = asm
		{
			vs_1_1
			dcl_position v0
			dcl_normal v1
			dcl_binormal v2
			dcl_tangent v3
			dcl_texcoord v4
			
			// texcoord
			dp4 r10.x, v4, c11
			dp4 r10.y, v4, c12
			mul oT0.xy, r10, c10.xyyy
			
			// current position
			dp4 r0.x, v0, c0
			dp4 r0.y, v0, c1
			dp4 r0.z, v0, c2
			dp4 r0.w, v0, c3
			mov oPos, r0
			mul r0.z, r0.z, c229.x
			mov oT1, r0
			
			// TBN
			dp3 oT3.x, v1, c226
			dp3 oT3.y, v1, c227
			dp3 oT3.z, v1, c228
			dp3 oT4.x, v2, c226
			dp3 oT4.y, v2, c227
			dp3 oT4.z, v2, c228
			dp3 oT5.x, v3, c226
			dp3 oT5.y, v3, c227
			dp3 oT5.z, v3, c228
		};
	}
}

technique VS_BSP_N_V
{
	pass P0
	{
		VertexShader = asm
		{
			vs_1_1
			dcl_position v0
			dcl_normal v1
			dcl_binormal v2
			dcl_tangent v3
			dcl_texcoord v4
			
			// texcoord
			dp4 r10.x, v4, c11
			dp4 r10.y, v4, c12
			mul oT0.xy, r10, c10.xyyy
			
			// current position
			dp4 r0.x, v0, c0
			dp4 r0.y, v0, c1
			dp4 r0.z, v0, c2
			dp4 r0.w, v0, c3
			mov oPos, r0
			mul r0.z, r0.z, c229.x
			mov oT1, r0
			
			// previous position
			dp4 oT2.x, v0, c222
			dp4 oT2.y, v0, c223
			dp4 oT2.z, v0, c224
			dp4 oT2.w, v0, c225
			
			// TBN
			dp3 oT3.x, v1, c226
			dp3 oT3.y, v1, c227
			dp3 oT3.z, v1, c228
			dp3 oT4.x, v2, c226
			dp3 oT4.y, v2, c227
			dp3 oT4.z, v2, c228
			dp3 oT5.x, v3, c226
			dp3 oT5.y, v3, c227
			dp3 oT5.z, v3, c228
		};
	}
}

technique VS_Object_N
{
	pass P0
	{
		VertexShader = asm{
			vs_1_1
			dcl_position v0
			dcl_normal v1
			dcl_binormal v2
			dcl_tangent v3
			dcl_texcoord v4
			dcl_blendindices v5
			dcl_blendweight v6
			
			// skinning
			dp4 r10.x, v4, c11
			dp4 r10.y, v4, c12
			mov oT0.xy, r10
			mul r0.xy, v5, c9.w
			add r0.xy, r0, c5.w
			mov a0.x, r0.x
			mul r4, v6.x, c29[a0.x]
			mul r5, v6.x, c30[a0.x]
			mul r6, v6.x, c31[a0.x]
			mov a0.x, r0.y
			mad r4, v6.y, c29[a0.x], r4
			mad r5, v6.y, c30[a0.x], r5
			mad r6, v6.y, c31[a0.x], r6
			dp4 r0.x, v0, r4
			dp4 r0.y, v0, r5
			dp4 r0.z, v0, r6
			mov r0.w, v4.w
			
			// current position
			dp4 r2.x, r0, c0
			dp4 r2.y, r0, c1
			dp4 r2.z, r0, c2
			dp4 r2.w, r0, c3
			mov oPos, r2
			mul r2.z, r2.z, c229.x
			mov oT1, r2
			
			// normal
			dp3 r1.x, v1, r4
			dp3 r1.y, v1, r5
			dp3 r1.z, v1, r6
			dp3 oT3.x, r1, c226
			dp3 oT3.y, r1, c227
			dp3 oT3.z, r1, c228
		};
	}
}

technique VS_Object_TBN
{
	pass P0
	{
		VertexShader = asm{
			vs_1_1
			dcl_position v0
			dcl_normal v1
			dcl_binormal v2
			dcl_tangent v3
			dcl_texcoord v4
			dcl_blendindices v5
			dcl_blendweight v6
			
			// skinning
			dp4 r10.x, v4, c11
			dp4 r10.y, v4, c12
			mov oT0.xy, r10
			mul r0.xy, v5, c9.w
			add r0.xy, r0, c5.w
			mov a0.x, r0.x
			mul r4, v6.x, c29[a0.x]
			mul r5, v6.x, c30[a0.x]
			mul r6, v6.x, c31[a0.x]
			mov a0.x, r0.y
			mad r4, v6.y, c29[a0.x], r4
			mad r5, v6.y, c30[a0.x], r5
			mad r6, v6.y, c31[a0.x], r6
			dp4 r0.x, v0, r4
			dp4 r0.y, v0, r5
			dp4 r0.z, v0, r6
			mov r0.w, v4.w
			
			// current position
			dp4 r2.x, r0, c0
			dp4 r2.y, r0, c1
			dp4 r2.z, r0, c2
			dp4 r2.w, r0, c3
			mov oPos, r2
			mul r2.z, r2.z, c229.x
			mov oT1, r2
			
			// TBN
			dp3 r1.x, v1, r4
			dp3 r1.y, v1, r5
			dp3 r1.z, v1, r6
			dp3 oT3.x, r1, c226
			dp3 oT3.y, r1, c227
			dp3 oT3.z, r1, c228
			dp3 r1.x, v2, r4
			dp3 r1.y, v2, r5
			dp3 r1.z, v2, r6
			dp3 oT4.x, r1, c226
			dp3 oT4.y, r1, c227
			dp3 oT4.z, r1, c228
			dp3 r1.x, v3, r4
			dp3 r1.y, v3, r5
			dp3 r1.z, v3, r6
			dp3 oT5.x, r1, c226
			dp3 oT5.y, r1, c227
			dp3 oT5.z, r1, c228
		};
	}
}

technique VS_Object_N_V
{
	pass P0
	{
		VertexShader = asm{
			vs_1_1
			dcl_position v0
			dcl_normal v1
			dcl_binormal v2
			dcl_tangent v3
			dcl_texcoord v4
			dcl_blendindices v5
			dcl_blendweight v6
			
			// skinning
			dp4 r10.x, v4, c11
			dp4 r10.y, v4, c12
			mov oT0.xy, r10
			mul r0.xy, v5, c9.w
			add r0.xy, r0, c5.w
			mov a0.x, r0.x
			mul r4, v6.x, c29[a0.x]
			mul r5, v6.x, c30[a0.x]
			mul r6, v6.x, c31[a0.x]
			mov a0.x, r0.y
			mad r4, v6.y, c29[a0.x], r4
			mad r5, v6.y, c30[a0.x], r5
			mad r6, v6.y, c31[a0.x], r6
			
			// current position
			dp4 r0.x, v0, r4
			dp4 r0.y, v0, r5
			dp4 r0.z, v0, r6
			mov r0.w, v4.w
			dp4 r2.x, r0, c0
			dp4 r2.y, r0, c1
			dp4 r2.z, r0, c2
			dp4 r2.w, r0, c3
			mov oPos, r2
			mul r2.z, r2.z, c229.x
			mov oT1, r2
			
			// normal
			dp3 r1.x, v1, r4
			dp3 r1.y, v1, r5
			dp3 r1.z, v1, r6
			dp3 oT3.x, r1, c226
			dp3 oT3.y, r1, c227
			dp3 oT3.z, r1, c228
			
			// previous position
			/*mul r0.xy, v5, c9.w
			add r0.xy, r0, c5.w
			mov a0.x, r0.x
			mul r4, v6.x, c104[a0.x]
			mul r5, v6.x, c105[a0.x]
			mul r6, v6.x, c106[a0.x]
			mov a0.x, r0.y
			mad r4, v6.y, c104[a0.x], r4
			mad r5, v6.y, c105[a0.x], r5
			mad r6, v6.y, c106[a0.x], r6
			dp4 r0.x, v0, r4
			dp4 r0.y, v0, r5
			dp4 r0.z, v0, r6
			mov r0.w, v4.w
			dp4 oT2.x, r0, c221
			dp4 oT2.y, r0, c222
			dp4 oT2.z, r0, c223
			dp4 oT2.w, r0, c224*/
		};
	}
}

technique VS_Object_TBN_V
{
	pass P0
	{
		VertexShader = asm{
			vs_1_1
			dcl_position v0
			dcl_normal v1
			dcl_binormal v2
			dcl_tangent v3
			dcl_texcoord v4
			dcl_blendindices v5
			dcl_blendweight v6
			
			// skinning
			dp4 r10.x, v4, c11
			dp4 r10.y, v4, c12
			mov oT0.xy, r10
			mul r0.xy, v5, c9.w
			add r0.xy, r0, c5.w
			mov a0.x, r0.x
			mul r4, v6.x, c29[a0.x]
			mul r5, v6.x, c30[a0.x]
			mul r6, v6.x, c31[a0.x]
			mov a0.x, r0.y
			mad r4, v6.y, c29[a0.x], r4
			mad r5, v6.y, c30[a0.x], r5
			mad r6, v6.y, c31[a0.x], r6
			dp4 r0.x, v0, r4
			dp4 r0.y, v0, r5
			dp4 r0.z, v0, r6
			mov r0.w, v4.w
			
			// current position
			dp4 r2.x, r0, c0
			dp4 r2.y, r0, c1
			dp4 r2.z, r0, c2
			dp4 r2.w, r0, c3
			mov oPos, r2
			mul r2.z, r2.z, c229.x
			mov oT1, r2
			
			// TBN
			dp3 r1.x, v1, r4
			dp3 r1.y, v1, r5
			dp3 r1.z, v1, r6
			dp3 oT3.x, r1, c226
			dp3 oT3.y, r1, c227
			dp3 oT3.z, r1, c228
			dp3 r1.x, v2, r4
			dp3 r1.y, v2, r5
			dp3 r1.z, v2, r6
			dp3 oT4.x, r1, c226
			dp3 oT4.y, r1, c227
			dp3 oT4.z, r1, c228
			dp3 r1.x, v3, r4
			dp3 r1.y, v3, r5
			dp3 r1.z, v3, r6
			dp3 oT5.x, r1, c226
			dp3 oT5.y, r1, c227
			dp3 oT5.z, r1, c228
			
			// previous position
			/*mul r0.xy, v5, c9.w
			add r0.xy, r0, c5.w
			mov a0.x, r0.x
			mul r4, v6.x, c104[a0.x]
			mul r5, v6.x, c105[a0.x]
			mul r6, v6.x, c106[a0.x]
			mov a0.x, r0.y
			mad r4, v6.y, c104[a0.x], r4
			mad r5, v6.y, c105[a0.x], r5
			mad r6, v6.y, c106[a0.x], r6
			dp4 r0.x, v0, r4
			dp4 r0.y, v0, r5
			dp4 r0.z, v0, r6
			mov r0.w, v4.w
			dp4 oT2.x, r0, c221
			dp4 oT2.y, r0, c222
			dp4 oT2.z, r0, c223
			dp4 oT2.w, r0, c224*/
		};
	}
}

technique VS_Object_NONE
{
	pass P0
	{
		VertexShader = asm{
			vs_1_1
			dcl_position v0
			dcl_normal v1
			dcl_binormal v2
			dcl_tangent v3
			dcl_texcoord v4
			dcl_blendindices v5
			dcl_blendweight v6
			
			// skinning
			dp4 r10.x, v4, c11
			dp4 r10.y, v4, c12
			mov oT0.xy, r10
			mul r0.xy, v5, c9.w
			add r0.xy, r0, c5.w
			mov a0.x, r0.x
			mul r4, v6.x, c29[a0.x]
			mul r5, v6.x, c30[a0.x]
			mul r6, v6.x, c31[a0.x]
			mov a0.x, r0.y
			mad r4, v6.y, c29[a0.x], r4
			mad r5, v6.y, c30[a0.x], r5
			mad r6, v6.y, c31[a0.x], r6
			dp4 r0.x, v0, r4
			dp4 r0.y, v0, r5
			dp4 r0.z, v0, r6
			mov r0.w, v4.w
			
			// current position
			dp4 r2.x, r0, c0
			dp4 r2.y, r0, c1
			dp4 r2.z, r0, c2
			dp4 r2.w, r0, c3
			mov oPos, r2
		};
	}
}
