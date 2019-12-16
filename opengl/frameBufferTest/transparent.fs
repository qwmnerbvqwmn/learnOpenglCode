#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture3;

void main()
{             
    vec4 texColor = texture(texture3, TexCoords);
    if(texColor.a < 0.1)
        discard;
    FragColor = texColor;
}