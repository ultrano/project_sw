#include "SWOpenAL.h"
#include "SWRawBufferStream.h"
#include "SWLog.h"

tuint alGenWaveBuffer( tbyte* buf, tuint bufSize )
{
	SWHardRef<SWRawBufferInputStream> rbis = new SWRawBufferInputStream( buf, bufSize );
	SWInputStreamReader reader( rbis() );

	//! RIFF Header
	char  chunkID[4] = {0};
	tuint chunkSize = 0;
	char  riffFormat[4] = {0};

	//! Wave Format
	char    subChunk1ID[4] = {0};
	tuint   subChunk1Size = 0;
	tushort audioFormat = 0;
	tushort numChannels = 0;
	tuint   sampleRate = 0;
	tuint   byteRate = 0;
	tushort byteAlign = 0;
	tushort bitsPerSample = 0;

	//! Wave Data
	char   subChunk2ID[4] = {0};
	tuint  subChunk2Size = 0;
	tbyte* subChunk2Data = NULL;

	//! read and check
	{
		reader.read( chunkID );
		reader.read( chunkSize );
		reader.read( riffFormat );

		if (strncmp( chunkID, "RIFF", 4 ) != 0) return 0;
		if (strncmp( riffFormat, "WAVE", 4 ) != 0) return 0;

		reader.read( subChunk1ID );
		reader.read( subChunk1Size );
		reader.read( audioFormat );
		reader.read( numChannels );
		reader.read( sampleRate );
		reader.read( byteRate );
		reader.read( byteAlign );
		reader.read( bitsPerSample );

		if (strncmp( subChunk1ID, "fmt ", 4 ) != 0) return 0;

		subChunk2Data = (buf + 36);

		do 
		{
			reader.read( subChunk2ID );
			reader.read( subChunk2Size );
			subChunk2Data += 8;

			if ( strncmp( subChunk2ID, "LIST", 4 ) == 0 )
			{
				rbis()->skip( subChunk2Size );
				subChunk2Data += subChunk2Size;
			}
			else if (strncmp( subChunk2ID, "data", 4 ) == 0) break;

		} while ( true );
	}

	tuint bufferID = 0;
	tuint format = 0;
	if (numChannels == 1)
	{
		if (bitsPerSample == 8 ) format = AL_FORMAT_MONO8;
		else if (bitsPerSample == 16) format = AL_FORMAT_MONO16;
	}
	else if (numChannels == 2)
	{
		if (bitsPerSample == 8 ) format = AL_FORMAT_STEREO8;
		else if (bitsPerSample == 16) format = AL_FORMAT_STEREO16;
	}

	alGenBuffers(1, &bufferID);
	alErrorLog();
	alBufferData(bufferID, format, subChunk2Data, subChunk2Size, sampleRate);
	alErrorLog();

	return bufferID;
}

bool alErrorLog()
{
	tuint error = alGetError();
	if ( error != AL_NO_ERROR ) SWLog( "audio error: %d", error );
	return ( error == AL_NO_ERROR );
}