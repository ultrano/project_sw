#ifndef SWAnimationClip_h__
#define SWAnimationClip_h__

#include "SWAnimationLine.h"
#include "SWRtti.h"

class SWGameObject;
class SWAnimationClip : public SWRefCountable
{
private:

	struct AnimInfo
	{
		const SWRtti*  type;
		const thashstr key;
		SWAnimationLine::Ref line;

		AnimInfo( const SWRtti* _type, const thashstr& _key, SWAnimationLine* _line );
	};

	tarray<AnimInfo> m_animInfos;

public:

	void addLine( const SWRtti* type, const tstring& key, SWAnimationLine* line );
	void addLine( const SWRtti* type, const tstring& key, SWAnimationLine::Ref line );
	void updatePlay( SWGameObject* go, float time );
};

#endif // SWAnimationClip_h__