#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
uniform float deviation;
out vec3 ourColor;
out vec3 ourPos;

void main()
{
    gl_Position = vec4(aPos.x+deviation,-aPos.y,aPos.z, 1.0f);
	ourPos = vec3(aPos.x+deviation,-aPos.y,aPos.z);
	//gl_Position = vec4(aPos, 1.0f);
	//ourPos = aPos;
    ourColor = aColor;
}