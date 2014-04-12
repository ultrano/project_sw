#ifndef SWOpenGL_h__
#define SWOpenGL_h__

#ifdef WIN32
# include "glew.h"
#elif __APPLE__
#  include "TargetConditionals.h"
# ifdef TARGET_OS_IPHONE
#   include <OpenGLES/ES1/gl.h>
#   include <OpenGLES/ES1/glext.h>
#   include <OpenGLES/ES2/gl.h>
#   include <OpenGLES/ES2/glext.h>
#  elif TARGET_OS_MAC
#   include <OpenGL/gl.h>
#   include <OpenGL/glu.h>
#   include <OpenGL/glext.h>
#   include <GLUT/glut.h>
#  endif
#endif

#endif // SWOpenGL_h__