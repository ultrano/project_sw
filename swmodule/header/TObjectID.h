#ifndef TObjectID_h__
#define TObjectID_h__

#include "SWPrimaryType.h"

class TObjectID : public SWMemory
{
public:
	TObjectID();
	~TObjectID();

	tuint getID() const { return m_id; }

	bool        operator ==( const TObjectID& objID ) const { return (m_id == objID.m_id); }
	bool        operator !=( const TObjectID& objID ) const { return (m_id != objID.m_id); }

private:
	tuint m_id;
};

#endif // TObjectID_h__