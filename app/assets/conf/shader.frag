#version 300 es
precision mediump float;
out vec4 FragColor;

in vec2 TexCoord;

// texture sampler
uniform sampler2D texture0;

void main()
{
	FragColor = texture(texture0, TexCoord);
}
