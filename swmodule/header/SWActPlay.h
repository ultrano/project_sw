#ifndef SWActPlay_h__
#define SWActPlay_h__

#include "SWAct.h"
#include "SWString.h"
#include "SWArray.h"

class SWMesh;
class SWMeshRenderer;
class SWActPlay : public SWAct
{
	SW_RTTI( SWActPlay, SWAct );

private:

	SWHardRef<SWMeshRenderer> m_renderer;
	SWHardRef<SWMesh> m_mesh;
	SWHardRef<SWArray> m_sequence;
	float m_duration;
	float m_accumulation;

public:

	SWActPlay( SWArray* sequence, float duration );
	~SWActPlay();

	virtual bool isDone();
	virtual bool onStart();
	virtual void onUpdate();
};

#endif // SWActPlay_h__