#ifndef SWAnimation_h__
#define SWAnimation_h__

#include "SWComponent.h"

class SWAnimationClip;
class SWAnimation : public SWComponent
{
	SW_RTTI( SWAnimation, SWComponent );
	
	typedef ttable< thashstr, SWHardRef<SWAnimationClip> > ClipTable;

	ClipTable m_clipTable;
	SWAnimationClip* m_current;
	float m_flowTime;

private:

	void onAwake();
	void onRemove();
	void onUpdate();

public:

	void addClip( const tstring& name, SWAnimationClip* clip );
	void play( const tstring& name );
};

#endif // SWAnimation_h__