#ifndef SWOpenGL_h__
#define SWOpenGL_h__

#include "SWPlatform.h"

#ifdef PLATFORM_WIN32
# include "glew.h"
#elif PLATFORM_IOS
# include <OpenGLES/ES2/gl.h>
# include <OpenGLES/ES2/glext.h>
# define glClearDepth glClearDepthf
#elif PLATFORM_OSX
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
# include <OpenGL/glext.h>
# include <GLUT/glut.h>
#elif PLATFORM_ANDROID
# include <jni.h>
# include <GLES2/gl2.h>
# include <GLES2/gl2ext.h>
#endif

unsigned int glLoadTexture( const char* fileName, int& width, int& height );
unsigned int glLoadTextureFromMemory( const unsigned char* buf, int len, int& width, int& height );
unsigned int glLoadProgram( const char* source );

#endif // SWOpenGL_h__
