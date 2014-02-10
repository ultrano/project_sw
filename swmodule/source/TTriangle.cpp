#include "TTriangle.h"

bool TTriangle::testPoint( const TVector2f& position ) const
{
	bool check1 = (0 <= ( pos2 - pos1 ).dot( ( position - pos1 ) ));
	if ( !check1 ) return false;

	bool check2 = (0 <= ( pos3 - pos2 ).dot( ( position - pos2 ) ));
	if ( !check2 ) return false;

	bool check3 = (0 <= ( pos1 - pos3 ).dot( ( position - pos3 ) ));
	if ( !check3 ) return false;

	return true;
}