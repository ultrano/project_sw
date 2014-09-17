#include "SWAudioClip.h"
#include "SWOpenAL.h"
#include "SWLog.h"
#include "SWByteBufferStream.h"

SWHardRef<SWAudioClip> SWAudioClip::create( tbyte* buffer, tuint bufSize)
{
	tuint bufferID = alGenWaveBuffer( buffer, bufSize );
	if ( bufferID == 0 ) return NULL;
	/*
	tuint sourceID = 0;
	alGenSources( 1, &sourceID );
	alSourcei( sourceID, AL_BUFFER, bufferID );
	alSourcePlay( sourceID );
	//*/
	return new SWAudioClip( bufferID );
}

SWHardRef<SWAudioClip::Source> SWAudioClip::createSource() const
{
	if ( m_bufferID == 0 ) return NULL;
	
	tuint sourceID = 0;
	alGenSources( 1, &sourceID );
	alErrorLog();

	alSourcei(sourceID, AL_BUFFER, m_bufferID);
	alErrorLog();

	return new SWAudioClip::Source( sourceID, this );
}

SWAudioClip::SWAudioClip( tuint bufferID )
	: m_bufferID( bufferID )
{

}

SWAudioClip::~SWAudioClip()
{
	if ( m_bufferID != 0 ) alDeleteBuffers( 1, &m_bufferID );
}

//////////////////////////////////////////////////////////////////////////

SWAudioClip::Source::Source( tuint sourceID, const SWAudioClip* audioClip )
	: m_sourceID( sourceID )
	, m_audioClip( audioClip )
{

}

SWAudioClip::Source::~Source()
{
	if ( m_sourceID != 0 ) alDeleteSources( 1, &m_sourceID );
}

void SWAudioClip::Source::setLooping( bool val )
{
	m_loop = val;
	alSourcei(m_sourceID, AL_LOOPING, (val? AL_TRUE : AL_FALSE));
	alErrorLog();
}
void SWAudioClip::Source::setPitch( float val )
{
	m_pitch = val;
	alSourcef(m_sourceID, AL_PITCH, val);
	alErrorLog();
}
void SWAudioClip::Source::setVolume( float val )
{
	m_volume = val;
	alSourcef(m_sourceID, AL_GAIN, val);
	alErrorLog();
}
void SWAudioClip::Source::setPosition( const tvec3& val )
{
	m_pos = val;
	alSource3f(m_sourceID, AL_POSITION, val.x, val.y, val.z);
	alErrorLog();
}
void SWAudioClip::Source::setVelocity( const tvec3& val )
{
	m_vel = val;
	alSource3f(m_sourceID, AL_VELOCITY, val.x, val.y, val.z);
	alErrorLog();
}
void SWAudioClip::Source::setDirection( const tvec3& val )
{
	m_dir = val;
	alSource3f(m_sourceID, AL_DIRECTION, val.x, val.y, val.z);
	alErrorLog();
}

bool SWAudioClip::Source::isLooping() const
{
	return m_loop;
}
float SWAudioClip::Source::getPitch() const
{
	return m_pitch;
}
float SWAudioClip::Source::getVolume() const
{
	return m_volume;
}
const tvec3& SWAudioClip::Source::getPosition() const
{
	return m_pos;
}
const tvec3& SWAudioClip::Source::getVelocity() const
{
	return m_vel;
}
const tvec3& SWAudioClip::Source::getDirection() const
{
	return m_dir;
}

void SWAudioClip::Source::play()
{
	alSourcePlay(m_sourceID);
	alErrorLog();
}

void SWAudioClip::Source::stop()
{
	alSourceStop(m_sourceID);
	alErrorLog();
}

bool SWAudioClip::Source::isPlaying()
{
	tint state = AL_NONE;
	alGetSourcei( m_sourceID, AL_SOURCE_STATE, &state );
	
	return ( state == AL_PLAYING );
}


