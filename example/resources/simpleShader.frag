#version 460 core

in vec3 norm;

out vec4 _Color;

void main()
{
	_Color = vec4(norm, 1.f);
}