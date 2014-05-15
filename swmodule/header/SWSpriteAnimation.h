#ifndef SWSpriteAnimation_h__
#define SWSpriteAnimation_h__

#include "SWResource.h"

class SWSprite;
class SWSpriteSequence;
class SWSpriteAnimation : public SWResource
{
	SW_RTTI( SWSpriteAnimation, SWResource );

public:

	static SWHardRef<SWSpriteAnimation> create( const tstring& json );

	SWSpriteAnimation();
	~SWSpriteAnimation();

	void addSequence( SWSpriteSequence* sequence );
	SWSpriteSequence* getSequenceByName( const tstring& name ) const;
	SWSpriteSequence* getSequenceAt( tuint index ) const;
	tuint count() const;

private:

	//typedef ttable< tstring, SWHardRef<SWSprite> > SpriteTable;
	//typedef ttable< tstring, SWHardRef<SWSpriteSequence> > SequenceTable;
	typedef tarray< SWHardRef<SWSpriteSequence> > SequenceArray;

	//SpriteTable    m_batch;
	SequenceArray m_sequences;

};

#endif // SWSpriteAnimation_h__