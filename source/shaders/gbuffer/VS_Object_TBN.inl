    vs_2_0
    def c4, 3, 0, 0, 0
    dcl_position v0
    dcl_normal v1
    dcl_binormal v2
    dcl_tangent v3
    dcl_texcoord v4
    dcl_blendindices v5
    dcl_blendweight v6
    dp4 oT0.x, v4, c11
    dp4 oT0.y, v4, c12
    frc r0.xy, v5
    add r0.xy, -r0, v5
    mul r0.xy, r0, c4.x
    mova a0.xy, r0.yxzw
    mul r0, v6.y, c29[a0.x]
    mad r0, c29[a0.y], v6.x, r0
    dp4 r1.x, v0, r0
    mul r2, v6.y, c30[a0.x]
    mul r3, v6.y, c31[a0.x]
    mad r3, c31[a0.y], v6.x, r3
    mad r2, c30[a0.y], v6.x, r2
    dp4 r1.y, v0, r2
    dp4 r1.z, v0, r3
    mov r1.w, v4.w
    dp4 r4.z, r1, c2
    mul oT1.z, r4.z, c229.x
    dp3 r5.x, v1, r0
    dp3 r5.y, v1, r2
    dp3 r5.z, v1, r3
    dp3 oT3.x, r5, c226
    dp3 oT3.y, r5, c227
    dp3 oT3.z, r5, c228
    dp3 r5.x, v2, r0
    dp3 r0.x, v3, r0
    dp3 r5.y, v2, r2
    dp3 r0.y, v3, r2
    dp3 r5.z, v2, r3
    dp3 r0.z, v3, r3
    dp3 oT4.x, r5, c226
    dp3 oT4.y, r5, c227
    dp3 oT4.z, r5, c228
    dp3 oT5.x, r0, c226
    dp3 oT5.y, r0, c227
    dp3 oT5.z, r0, c228
    dp4 r4.x, r1, c0
    dp4 r4.y, r1, c1
    dp4 r4.w, r1, c3
    mov oPos, r4
    mov oT1.xyw, r4
    mov oT2, c4.y

// approximately 42 instruction slots used