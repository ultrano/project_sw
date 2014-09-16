#ifndef SWAudioClip_h__
#define SWAudioClip_h__

#include "SWResource.h"

class SWByteBuffer;
class SWAudioClip : public SWResource
{
	SW_RTTI( SWAudioClip, SWResource );

public:

	class Source : public SWRefCountable
	{
	public:
		void setLooping( bool val );
		void setPitch( float val );
		void setVolume( float val );
		void setPosition( const tvec3& val );
		void setVelocity( const tvec3& val );
		void setDirection( const tvec3& val );

		bool isLooping() const;
		float getPitch() const;
		float getVolume() const;
		const tvec3& getPosition() const;
		const tvec3& getVelocity() const;
		const tvec3& getDirection() const;

		void play();
		void stop();

		~Source();
	private:
		friend class SWAudioClip;
		Source( tuint sourceID, const SWAudioClip* audioClip );
	private:
		tuint m_sourceID;
		bool  m_loop;
		float m_pitch, m_volume;
		tvec3 m_pos, m_vel, m_dir;
		SWHardRef<SWAudioClip> m_audioClip;
	};

public:

	static SWHardRef<SWAudioClip> create( tbyte* buffer, tuint bufSize );

	SWHardRef<Source> createSource() const;

	~SWAudioClip();

private:

	SWAudioClip( tuint bufferID );

private:

	tuint m_bufferID;

};

#endif // SWAudioClip_h__