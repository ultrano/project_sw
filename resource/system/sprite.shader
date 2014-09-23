
uniform   mat4 MATRIX_MVP;
uniform   vec4 SCALER;
uniform   vec4 COLOR;
uniform sampler2D TEXTURE_0;

varying   vec2 v_tex;

#ifdef VERTEX_SHADER

attribute vec4 a_pos;
attribute vec2 a_tex;

void main()
{
   gl_Position = MATRIX_MVP * a_pos;
   v_tex = (a_tex*SCALER.zw) + SCALER.xy;
}

#endif

#ifdef FRAGMENT_SHADER

void main()
{
   gl_FragColor = texture2D( TEXTURE_0, v_tex ) * COLOR;
}

#endif
