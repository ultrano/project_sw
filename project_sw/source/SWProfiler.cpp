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

SWProfileCenter::SWProfileCenter()
	: m_beginFrameTime( 0 )
{

}

SWProfileCenter::~SWProfileCenter()
{

}

SWProfileCenter& SWProfileCenter::getInstance()
{
	static SWProfileCenter instance;
	return instance;
}

SWProfileCenter::MainFrame::MainFrame()
{
	SWProfileCenter::getInstance().m_beginFrameTime = SW_GC.awakeTime();
}

SWProfileCenter::MainFrame::~MainFrame()
{
	float endTime = SW_GC.awakeTime();
	float beginTime = SWProfileCenter::getInstance().m_beginFrameTime;
	float totalTime = endTime - beginTime;
	std::vector<Profile>& profiles = SWProfileCenter::getInstance().m_profiles;
	for ( int i = 0 ; i < profiles.size() ; ++i)
	{
		Profile& itor = profiles[i];
		float percent = (itor.spendTime/totalTime)*100;
		SWLogCenter::getInstance().write( itor.file, itor.line, "profile [%s:%d] %f \%", itor.scope, itor.line, percent );
	}
	profiles.clear();
}
