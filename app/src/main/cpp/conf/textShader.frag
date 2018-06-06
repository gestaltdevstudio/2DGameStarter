#version 330 core
//precision lowp sampler2D;
out vec4 fragColor;

in vec2 UV;

// Values that stay constant for the whole mesh.
uniform sampler2D textTexture;
//uniform vec3 objColor;

void main(){

	fragColor = texture( textTexture, UV );

}
