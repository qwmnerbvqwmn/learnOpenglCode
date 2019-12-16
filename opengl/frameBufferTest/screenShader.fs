#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
//uniform sampler2D texture_diffuse1;

const float offset = 1.0 / 300.0;  

void main()
{    
    // FragColor = (1.0-texture(screenTexture, TexCoords));

	// Inversion effect
	// FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
	

	// // gray 
	// FragColor = texture(screenTexture, TexCoords);
    // float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    // FragColor = vec4(average, average, average, 1.0);

	vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // ����
        vec2( 0.0f,    offset), // ����
        vec2( offset,  offset), // ����
        vec2(-offset,  0.0f),   // ��
        vec2( 0.0f,    0.0f),   // ��
        vec2( offset,  0.0f),   // ��
        vec2(-offset, -offset), // ����
        vec2( 0.0f,   -offset), // ����
        vec2( offset, -offset)  // ����
    );

    // float kernel[9] = float[](
    //     -1, -1, -1,
    //     -1,  8, -1,
    //     -1, -1, -1
    // );

	// float kernel[9] = float[](
    //     -1,-1,-1,
    //     -1, 8, -1,
    //     -1,-1,-1
    // );
	float kernel[9] = float[](
        0.0947416,0.118318,0.0947416,
        0.118318, 0.147761, 0.118318,
        0.0947416,0.118318,0.0947416
    );

	vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

		

	//col += 0.5f;
    FragColor = vec4(col, 1.0);
	// float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
	// FragColor = vec4(average, average, average, 1.0);

}