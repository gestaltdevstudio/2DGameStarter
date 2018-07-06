precision mediump float;
precision lowp sampler2D;

varying vec2 TexCoord;

// texture sampler
uniform sampler2D texture0;

void main()
{
	gl_FragColor = texture2D(texture0, TexCoord);
}
