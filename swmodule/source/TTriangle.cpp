#include "TTriangle.h"

bool TTriangle::testPoint( const TVector2f& position ) const
{
	bool check1 = (0 <= ( pos2 - pos1 ).dot( ( position - pos1 ) ));
	bool check2 = (0 <= ( pos3 - pos2 ).dot( ( position - pos2 ) ));
	bool check3 = (0 <= ( pos1 - pos3 ).dot( ( position - pos3 ) ));
	return ( check1 && check2 && check3 );
}