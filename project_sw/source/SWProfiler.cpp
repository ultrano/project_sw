#include "SWProfiler.h"
#include "SWGameContext.h"
#include "SWLog.h"

SWProfiler::SWProfiler( const char* file, int line, const char* scope )
	: m_beginTime( SW_GC.awakeTime() )
	, m_file( file )
	, m_line( line )
	, m_scope( scope )
{

}

SWProfiler::~SWProfiler()
{
	float nowTime = SW_GC.awakeTime();
	float spendTime = nowTime - m_beginTime;
	SWLogCenter::getInstance().write( m_file, m_line, "profile [%s:%d] %f ", m_scope, m_line, spendTime );
}
