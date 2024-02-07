// Handles proper framebuffer and texture coordinates between the different graphics apis 

#ifndef __COORDSPACE_SH
#define __COORDSPACE_SH

// Framebuffer 
// Always down
vec2 convert_framebuffer(vec2 coord, float fb_height)
{
    #if BGFX_SHADER_LANGUAGE_GLSL
        return vec2(coord.x, fb_height - coord.y);
    #else
        return coord;
    #endif
}

// Framebuffer range to position (expected converted coords for opengl)
vec2 convert_fb_clippos(vec2 converted_coords, vec2 fb_size)
{   
    return (converted_coords / fb_size) * 2.0 - 1.0;
}

#endif __COORDSPACE_SH


