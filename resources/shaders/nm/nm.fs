$input v_normal

// Note: had to include this here to prevent "fragData not supported in this glsl version" errors
#if BGFX_SHADER_LANGUAGE_GLSL >= 400
out vec4 bgfx_FragData[2];
#define gl_FragData bgfx_FragData
#endif

#include "common.sh"

void main()
{
    gl_FragColor = vec4(v_normal.xyz, 1);
}

