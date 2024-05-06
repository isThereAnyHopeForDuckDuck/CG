#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture0;


void main()
{
	FragColor = vec4(texture(texture0, TexCoord).rgb, 1.0);
}