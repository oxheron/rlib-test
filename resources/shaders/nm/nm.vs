$input a_position, a_texcoord0, a_normal, i_data0, i_data1, i_data2, i_data3, i_data4
$output v_normal

#include "common.sh"
#include "coordspace.sh"

void main()
{
    mat4 model = mtxFromCols(i_data0, i_data1, i_data2, i_data3);
    vec4 world_pos = mul(model, vec4(a_position, 1.0));
    world_pos = vec4(world_pos.x, world_pos.y, world_pos.z, world_pos.w);
    gl_Position = mul(u_viewProj, world_pos);
    v_normal = a_normal;
}
