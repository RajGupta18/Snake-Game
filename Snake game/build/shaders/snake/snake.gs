#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform float gsize;

void build_square(vec4 pos) {
    gl_Position = pos;      // top left
    EmitVertex();
    gl_Position = pos + vec4(gsize, 0.0, 0.0, 0.0); //top right
    EmitVertex();
    gl_Position = pos + vec4(0.0, -gsize, 0.0, 0.0);    //bottom left
    EmitVertex();
    gl_Position = pos + vec4(gsize, -gsize, 0.0, 0.0);  //bottom right
    EmitVertex();
    EndPrimitive();
}

void main() {
    build_square(gl_in[0].gl_Position);
}