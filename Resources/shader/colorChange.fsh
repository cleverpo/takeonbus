#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec3 u_originColor;
uniform vec3 u_targetColor;

void main(void)
{
	vec4 c = texture2D(CC_Texture0, v_texCoord);
    if ( distance(c.xyz, u_originColor) < 0.1 )
    {
        gl_FragColor.xyz = u_targetColor;
    } else {
        gl_FragColor.xyz = c.xyz;
    }
    gl_FragColor.w = c.w;
}