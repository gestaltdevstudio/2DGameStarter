attribute vec3 aPos;
attribute vec2 aTexCoord;

varying vec2 TexCoord;
uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(aPos, 1.0);
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
