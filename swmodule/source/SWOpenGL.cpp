#include "SWOpenGL.h"
#include "stb_image.h"

unsigned int glLoadTexture( const char* fileName, int& width, int& height )
{
	if ( !fileName ) return 0;

	int comp;

	unsigned char* data = stbi_load( fileName, &width, &height, &comp, 0 );

	if ( !data ) return 0;

	unsigned int texID[1];

	glGenTextures(1,&texID[0]);
	glBindTexture(GL_TEXTURE_2D,texID[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glTexImage2D(GL_TEXTURE_2D, 0
	, (comp==4)? GL_RGBA : (comp==3)? GL_RGB : GL_INVALID_ENUM
	, width, height, 0
	, (comp==4)? GL_RGBA : (comp==3)? GL_RGB : GL_INVALID_ENUM
                 , GL_UNSIGNED_BYTE, data);
    
	GLenum err = glGetError();

	stbi_image_free(data);
	if ( err ) return 0;

	return texID[0];
}

unsigned int glLoadTextureFromMemory( const unsigned char* buf, int len, int& width, int& height )
{
	if ( !buf || !len ) return 0;

	int comp;

	unsigned char* data = stbi_load_from_memory( buf, len, &width, &height, &comp, 0 );

	if ( !data ) return 0;

	unsigned int texID[1];

	glGenTextures(1,&texID[0]);
	glBindTexture(GL_TEXTURE_2D,texID[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	glTexImage2D(GL_TEXTURE_2D, 0
	, (comp==4)? GL_RGBA : (comp==3)? GL_RGB : GL_INVALID_ENUM
	, width, height, 0
	, (comp==4)? GL_RGBA : (comp==3)? GL_RGB : GL_INVALID_ENUM
                 , GL_UNSIGNED_BYTE, data);
    
	GLenum err = glGetError();

	stbi_image_free(data);
	if ( err ) return 0;

	return texID[0];
}