#version 460 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 color;
layout(location = 4) in vec3 tangent;
layout(location = 5) in vec3 bitangent;

uniform mat4 _ModelViewProjection;

out vec3 norm;

void main()
{
	norm = normal;
	gl_Position = _ModelViewProjection * vec4(pos, 1.f);
}