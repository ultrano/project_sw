
varying   vec2 v_tex;
uniform   mat4 u_mvpMat;
uniform   mat4 u_texMat;
uniform sampler2D s_texture;

#ifdef VERTEX_SHADER

attribute vec4 a_pos;
attribute vec2 a_tex;

void main()
{
   gl_Position = u_mvpMat * a_pos;
   v_tex = a_tex;
}

#endif

#ifdef FRAGMENT_SHADER
#ifdef GL_ES
precision mediump float;
#endif

void main()
{
   gl_FragColor = texture2D( s_texture, v_tex );
}

#endif
