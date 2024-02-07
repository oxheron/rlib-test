$output v_texcoord

void main() 
{
    // UV from vertexID
	int vid = int(gl_VertexID);
	float x = (vid == 0 || vid == 2) ? -1.0 : 3.0;
	float y = (vid == 2) ? 3.0 : -1.0;
	gl_Position = vec4(x, y, 0, 1);

	// UV from Position 
    v_texcoord = 0.5 * gl_Position.xy + vec2(0.5);

    // No idea if this is just for vulkan or what
    // Ill just leave this
    v_texcoord = vec2(v_texcoord.x, -v_texcoord.y);
}
