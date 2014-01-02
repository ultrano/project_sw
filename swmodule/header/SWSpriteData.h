#ifndef SWSpriteData_h__
#define SWSpriteData_h__

#include "SWObject.h"
#include "SWString.h"
#include "SWArray.h"
#include "SWTable.h"

class SWSpriteData : public SWObject
{
	SW_RTTI( SWSpriteData, SWObject );
private:
	SWHardRef<SWTable> m_sequences;
public:

	class ImageRegion : public SWObject
	{
		SW_RTTI( ImageRegion, SWObject );
	public:
		unsigned int texID;
		float x, y;
		float w, h;
	};

	SWSpriteData();
	~SWSpriteData();

	void addSequence( const tstring& name, const SWArray* regions );
	SWArray* findSequence( const tstring& name );

};

#endif // SWSpriteData_h__