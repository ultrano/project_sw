
uniform   mat4 MATRIX_P;
uniform   vec4 COLOR;

#ifdef VERTEX_SHADER

attribute vec4 a_pos;

void main()
{
   gl_Position = MATRIX_P * a_pos;
}

#endif

#ifdef FRAGMENT_SHADER

void main()
{
   gl_FragColor = COLOR;
}

#endif
