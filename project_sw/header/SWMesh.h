#ifndef SWMesh_h__
#define SWMesh_h__

#include "SWComponent.h"

class SWMesh : public SWComponent
{
	SW_RTTI( SWMesh, SWComponent );
public:
	void render(){};
};
#endif // SWMesh_h__