#ifndef SWOpenGL_h__
#define SWOpenGL_h__

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

#ifdef WIN32
# include "glew.h"
#elif TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
# include <OpenGLES/ES1/gl.h>
# include <OpenGLES/ES1/glext.h>
# include <OpenGLES/ES2/gl.h>
# include <OpenGLES/ES2/glext.h>
#elif TARGET_OS_MAC
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
# include <OpenGL/glext.h>
# include <GLUT/glut.h>
#elif ANDROID
# include <jni.h>
# include <GLES2/gl2.h>
# include <GLES2/gl2ext.h>
#endif

unsigned int glLoadTexture( const char* fileName, int& width, int& height );
unsigned int glLoadTextureFromMemory( const unsigned char* buf, int len, int& width, int& height );
unsigned int glLoadProgram( const char* source );

#endif // SWOpenGL_h__
