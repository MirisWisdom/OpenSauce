    vs_2_0
    def c4, 3, 0, 0, 0
    dcl_position v0
    dcl_normal v1
    dcl_texcoord v2
    dcl_blendindices v3
    dcl_blendweight v4
    dp4 oT0.x, v2, c11
    dp4 oT0.y, v2, c12
    frc r0.xy, v3
    add r0.xy, -r0, v3
    mul r0.xy, r0, c4.x
    mova a0.xy, r0.yxzw
    mul r0, v4.y, c29[a0.x]
    mad r0, c29[a0.y], v4.x, r0
    dp4 r1.x, v0, r0
    dp3 r0.x, v1, r0
    mul r2, v4.y, c30[a0.x]
    mul r3, v4.y, c31[a0.x]
    mad r3, c31[a0.y], v4.x, r3
    mad r2, c30[a0.y], v4.x, r2
    dp4 r1.y, v0, r2
    dp3 r0.y, v1, r2
    dp4 r1.z, v0, r3
    dp3 r0.z, v1, r3
    mov r1.w, v2.w
    dp4 r2.z, r1, c2
    mul oT1.z, r2.z, c229.x
    dp3 oT3.x, r0, c226
    dp3 oT3.y, r0, c227
    dp3 oT3.z, r0, c228
    dp4 r2.x, r1, c0
    dp4 r2.y, r1, c1
    dp4 r2.w, r1, c3
    mov oPos, r2
    mov oT1.xyw, r2
    mov oT2, c4.y
    mov oT4.xyz, c4.y
    mov oT5.xyz, c4.y

// approximately 32 instruction slots used