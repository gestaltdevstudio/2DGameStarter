precision mediump float;
precision lowp sampler2D;

varying vec2 UV;

// Values that stay constant for the whole mesh.
uniform sampler2D textTexture;
//uniform vec3 objColor;

void main(){

	gl_FragColor = texture2D(textTexture, UV );

}
