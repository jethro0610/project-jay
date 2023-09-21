vec3 a_position : POSITION;
vec3 a_normal : NORMAL;
vec3 a_tangent : TANGENT;
vec3 a_bitangent : BITANGENT;
vec2 a_texcoord0 : TEXCOORD0;
uvec4 a_color0 : COLOR0;
vec4 a_color1 : COLOR1;

vec3 v_wposition : WORLDPOSITION;
vec4 v_sposition : SHADOWPOSITION;
vec3 v_normal : NORMAL;
vec3 v_tangent : TANGENT;
vec3 v_bitangent : BITANGENT;
mat3 v_tbn : TBN;
vec2 v_texcoord0 : TEXCOORD0;
vec4 v_color : VCOLOR;
float v_edgeDistance : EDGEDIST;
float v_time : TIME;

vec4 i_data0: TEXCOORD7;
vec4 i_data1: TEXCOORD6;
vec4 i_data2: TEXCOORD5;
vec4 i_data3: TEXCOORD4;
vec4 i_data4: TEXCOORD3;
