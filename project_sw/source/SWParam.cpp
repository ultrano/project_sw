#include "SWParam.h"

SWParam::SWParam( SWArray* params )
	: m_params( params )
{
}

SWParam::~SWParam()
{
}

const SWObject* SWParam::get( unsigned int index )
{
	if ( !m_params.isValid() ) return NULL;
	return m_params()->get( index );
}

unsigned int SWParam::count()
{
	if ( !m_params.isValid() ) return 0;
	return m_params()->count();
}