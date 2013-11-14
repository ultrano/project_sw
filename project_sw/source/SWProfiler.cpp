#include "SWProfiler.h"
#include "SWTime.h"
#include "SWLog.h"

SWProfiler::SWProfiler( const char* file, int line, const char* scope )
	: m_beginTime( SWTime.getTime() )
	, m_file( file )
	, m_line( line )
	, m_scope( scope )
{

}

SWProfiler::~SWProfiler()
{
	float nowTime = SWTime.getTime();
	float spendTime = nowTime - m_beginTime;
	SWLogCenter::getInstance().write( m_file, m_line, "profile [%s:%d] %f ", m_scope, m_line, spendTime );
}
