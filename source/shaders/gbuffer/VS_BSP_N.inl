    vs_2_0
    def c4, 0, 0, 0, 0
    dcl_position v0
    dcl_normal v1
    dcl_binormal v2
    dcl_tangent v3
    dcl_texcoord v4
    dp4 r0.x, v4, c11
    dp4 r0.y, v4, c12
    mul oT0.xy, r0, c10
    dp4 r0.z, v0, c2
    mul oT1.z, r0.z, c229.x
    dp4 oT3.x, v1, c226
    dp4 oT3.y, v1, c227
    dp4 oT3.z, v1, c228
    dp4 oT4.x, v2, c226
    dp4 oT4.y, v2, c227
    dp4 oT4.z, v2, c228
    dp4 oT5.x, v3, c226
    dp4 oT5.y, v3, c227
    dp4 oT5.z, v3, c228
    dp4 r0.x, v0, c0
    dp4 r0.y, v0, c1
    dp4 r0.w, v0, c3
    mov oPos, r0
    mov oT1.xyw, r0
    mov oT2, c4.x

// approximately 20 instruction slots used