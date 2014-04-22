#ifndef SWAnimation_h__
#define SWAnimation_h__

#include "SWComponent.h"

class SWAnimationClip;
class SWAnimation : public SWComponent
{
	SW_RTTI( SWAnimation, SWComponent );

public:

	SWAnimation();
	SWAnimation( factory_constructor );
	~SWAnimation();
	void addClip( const tstring& name, SWAnimationClip* clip );
	void play( const tstring& name );

private:

	void onAwake();
	void onRemove();
	void onUpdate();

private:

	typedef ttable< thashstr, SWHardRef<SWAnimationClip> > ClipTable;

	ClipTable m_clipTable;
	SWAnimationClip* m_current;
	float m_flowTime;
};

#endif // SWAnimation_h__