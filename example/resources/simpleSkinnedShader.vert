#version 460 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 color;
layout(location = 4) in vec3 tangent;
layout(location = 5) in vec3 bitangent;
layout(location = 6) in vec4 boneIds;
layout(location = 7) in vec4 boneWeights;

uniform mat4 _ModelViewProjection;
uniform mat4 _BoneTransforms[100];

out vec3 norm;

void main()
{
	vec4 finalPosition = vec4(0, 0, 0, 0);
	norm = normal;
	for (int i = 0; i < 4; i++)
	{
		if (boneIds[i] == -1.f)
			continue;

		if (boneIds[i] >= 100)
		{
			finalPosition = vec4(pos, 1.f);
			break;
		}

		vec4 localPosition = _BoneTransforms[int(boneIds[i])] * vec4(pos, 1.f);
		finalPosition += localPosition * boneWeights[i];
		vec3 localnormal = mat3(_BoneTransforms[int(boneIds[i])]) * normal;
		norm = localnormal;
	}
	
	gl_Position = _ModelViewProjection * finalPosition;
}