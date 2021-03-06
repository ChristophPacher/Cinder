#if 0
//
// Generated by Microsoft (R) D3D Shader Disassembler
//
//
///
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// POSITION                 0   xyzw        0     NONE   float   xyzw
// NORMAL                   0   xyz         1     NONE   float       
// TEXCOORD                 0   xy          2     NONE   float       
// COLOR                    0   xyzw        3     NONE   float   xyzw
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_POSITION              0   xyzw        0      POS   float   xyzw
// NORMAL                   0   xyz         1     NONE   float   xyz 
// TEXCOORD                 0   xy          2     NONE   float   xy  
// COLOR                    0   xyzw        3     NONE   float   xyzw
// TEXCOORD                 1   xyz         4     NONE   float   xyz 
//
//
// Constant buffer to DX9 shader constant mappings:
//
// Target Reg Buffer  Start Reg # of Regs        Data Conversion
// ---------- ------- --------- --------- ----------------------
// c1         cb0             0         8  ( FLT, FLT, FLT, FLT)
//
//
// Runtime generated constant mappings:
//
// Target Reg                               Constant Description
// ---------- --------------------------------------------------
// c0                              Vertex Shader position offset
//
//
// Level9 shader bytecode:
//
    vs_2_0
    def c9, 0, 0, 0, 0
    dcl_texcoord v0
    dcl_texcoord3 v3
    mul r0, v0.y, c6
    mad r0, c5, v0.x, r0
    mad r0, c7, v0.z, r0
    mad r0, c8, v0.w, r0
    mul r1, r0.y, c2
    mad r1, c1, r0.x, r1
    mad r1, c3, r0.z, r1
    mad r0, c4, r0.w, r1
    mad oPos.xy, r0.w, c0, r0
    mov oPos.zw, r0
    mov oT0.xyz, c9.x
    mov oT1.xy, c9.x
    mov oT2, v3
    mov oT3.xyz, c9.x

// approximately 14 instruction slots used
vs_4_0
dcl_constantbuffer cb0[8], immediateIndexed
dcl_input v0.xyzw
dcl_input v3.xyzw
dcl_output_siv o0.xyzw, position
dcl_output o1.xyz
dcl_output o2.xy
dcl_output o3.xyzw
dcl_output o4.xyz
dcl_temps 2
mul r0.xyzw, v0.yyyy, cb0[5].xyzw
mad r0.xyzw, cb0[4].xyzw, v0.xxxx, r0.xyzw
mad r0.xyzw, cb0[6].xyzw, v0.zzzz, r0.xyzw
mad r0.xyzw, cb0[7].xyzw, v0.wwww, r0.xyzw
mul r1.xyzw, r0.yyyy, cb0[1].xyzw
mad r1.xyzw, cb0[0].xyzw, r0.xxxx, r1.xyzw
mad r1.xyzw, cb0[2].xyzw, r0.zzzz, r1.xyzw
mad o0.xyzw, cb0[3].xyzw, r0.wwww, r1.xyzw
mov o1.xyz, l(0,0,0,0)
mov o2.xy, l(0,0,0,0)
mov o3.xyzw, v3.xyzw
mov o4.xyz, l(0,0,0,0)
ret 
// Approximately 0 instruction slots used
#endif

const BYTE FixedFunctionVS[] =
{
     68,  88,  66,  67, 150, 119, 
    242, 224, 211,  31, 227, 123, 
    115,  49, 234,  10, 217,  81, 
    245, 136,   1,   0,   0,   0, 
    240,   4,   0,   0,   4,   0, 
      0,   0,  48,   0,   0,   0, 
    140,   1,   0,   0, 180,   3, 
      0,   0,  68,   4,   0,   0, 
     65, 111, 110,  57,  84,   1, 
      0,   0,  84,   1,   0,   0, 
      0,   2, 254, 255,  32,   1, 
      0,   0,  52,   0,   0,   0, 
      1,   0,  36,   0,   0,   0, 
     48,   0,   0,   0,  48,   0, 
      0,   0,  36,   0,   1,   0, 
     48,   0,   0,   0,   0,   0, 
      8,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   2, 254, 255,  81,   0, 
      0,   5,   9,   0,  15, 160, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  31,   0, 
      0,   2,   5,   0,   0, 128, 
      0,   0,  15, 144,  31,   0, 
      0,   2,   5,   0,   3, 128, 
      3,   0,  15, 144,   5,   0, 
      0,   3,   0,   0,  15, 128, 
      0,   0,  85, 144,   6,   0, 
    228, 160,   4,   0,   0,   4, 
      0,   0,  15, 128,   5,   0, 
    228, 160,   0,   0,   0, 144, 
      0,   0, 228, 128,   4,   0, 
      0,   4,   0,   0,  15, 128, 
      7,   0, 228, 160,   0,   0, 
    170, 144,   0,   0, 228, 128, 
      4,   0,   0,   4,   0,   0, 
     15, 128,   8,   0, 228, 160, 
      0,   0, 255, 144,   0,   0, 
    228, 128,   5,   0,   0,   3, 
      1,   0,  15, 128,   0,   0, 
     85, 128,   2,   0, 228, 160, 
      4,   0,   0,   4,   1,   0, 
     15, 128,   1,   0, 228, 160, 
      0,   0,   0, 128,   1,   0, 
    228, 128,   4,   0,   0,   4, 
      1,   0,  15, 128,   3,   0, 
    228, 160,   0,   0, 170, 128, 
      1,   0, 228, 128,   4,   0, 
      0,   4,   0,   0,  15, 128, 
      4,   0, 228, 160,   0,   0, 
    255, 128,   1,   0, 228, 128, 
      4,   0,   0,   4,   0,   0, 
      3, 192,   0,   0, 255, 128, 
      0,   0, 228, 160,   0,   0, 
    228, 128,   1,   0,   0,   2, 
      0,   0,  12, 192,   0,   0, 
    228, 128,   1,   0,   0,   2, 
      0,   0,   7, 224,   9,   0, 
      0, 160,   1,   0,   0,   2, 
      1,   0,   3, 224,   9,   0, 
      0, 160,   1,   0,   0,   2, 
      2,   0,  15, 224,   3,   0, 
    228, 144,   1,   0,   0,   2, 
      3,   0,   7, 224,   9,   0, 
      0, 160, 255, 255,   0,   0, 
     83,  72,  68,  82,  32,   2, 
      0,   0,  64,   0,   1,   0, 
    136,   0,   0,   0,  89,   0, 
      0,   4,  70, 142,  32,   0, 
      0,   0,   0,   0,   8,   0, 
      0,   0,  95,   0,   0,   3, 
    242,  16,  16,   0,   0,   0, 
      0,   0,  95,   0,   0,   3, 
    242,  16,  16,   0,   3,   0, 
      0,   0, 103,   0,   0,   4, 
    242,  32,  16,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
    101,   0,   0,   3, 114,  32, 
     16,   0,   1,   0,   0,   0, 
    101,   0,   0,   3,  50,  32, 
     16,   0,   2,   0,   0,   0, 
    101,   0,   0,   3, 242,  32, 
     16,   0,   3,   0,   0,   0, 
    101,   0,   0,   3, 114,  32, 
     16,   0,   4,   0,   0,   0, 
    104,   0,   0,   2,   2,   0, 
      0,   0,  56,   0,   0,   8, 
    242,   0,  16,   0,   0,   0, 
      0,   0,  86,  21,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      5,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,   6,  16, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   6,   0, 
      0,   0, 166,  26,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   7,   0,   0,   0, 
    246,  31,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  56,   0, 
      0,   8, 242,   0,  16,   0, 
      1,   0,   0,   0,  86,   5, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      6,   0,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      2,   0,   0,   0, 166,  10, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,  10, 
    242,  32,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0, 246,  15,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     54,   0,   0,   8, 114,  32, 
     16,   0,   1,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  54,   0,   0,   8, 
     50,  32,  16,   0,   2,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   5, 242,  32,  16,   0, 
      3,   0,   0,   0,  70,  30, 
     16,   0,   3,   0,   0,   0, 
     54,   0,   0,   8, 114,  32, 
     16,   0,   4,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  62,   0,   0,   1, 
     73,  83,  71,  78, 136,   0, 
      0,   0,   4,   0,   0,   0, 
      8,   0,   0,   0, 104,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,  15,   0,   0, 113,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
      7,   0,   0,   0, 120,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
      3,   0,   0,   0, 129,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   3,   0,   0,   0, 
     15,  15,   0,   0,  80,  79, 
     83,  73,  84,  73,  79,  78, 
      0,  78,  79,  82,  77,  65, 
     76,   0,  84,  69,  88,  67, 
     79,  79,  82,  68,   0,  67, 
     79,  76,  79,  82,   0, 171, 
     79,  83,  71,  78, 164,   0, 
      0,   0,   5,   0,   0,   0, 
      8,   0,   0,   0, 128,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,   0,   0,   0, 140,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
      7,   8,   0,   0, 147,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
      3,  12,   0,   0, 156,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   3,   0,   0,   0, 
     15,   0,   0,   0, 147,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   4,   0,   0,   0, 
      7,   8,   0,   0,  83,  86, 
     95,  80,  79,  83,  73,  84, 
     73,  79,  78,   0,  78,  79, 
     82,  77,  65,  76,   0,  84, 
     69,  88,  67,  79,  79,  82, 
     68,   0,  67,  79,  76,  79, 
     82,   0, 171, 171
};
