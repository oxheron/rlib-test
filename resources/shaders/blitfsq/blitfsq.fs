$input v_texcoord

#include "common.sh"
#include "coordspace.sh"

SAMPLER2D(framebuffer, 1);

uniform mat4 inv_viewproj;
uniform vec4 width_height;
uniform vec4 light;

void main()
{
    // float clip_z = toClipSpaceDepth(texture2D(framebuffer, v_texcoord.x);
    // vec2 clip_xy = convert_fb_clippos(gl_FragCoord.xy, width_height.xy);
    // vec3 worldpos = clipToWorld(inv_viewproj, vec3(clip_xy.x, clip_xy.y, clip_z));
    // gl_FragColor = vec4(0, 0, clip_z, 1);
    // gl_FragColor = vec4(distance(light.xyz, worldpos), 0, 0, 1);
    gl_FragColor = vec4(texture2D(framebuffer, v_texcoord));
}

