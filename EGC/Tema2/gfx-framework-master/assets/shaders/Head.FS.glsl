#version 330

// Uniform properties
uniform vec3 color;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    out_color = vec4(0.972, 0.952, 0.768, 1);
}
