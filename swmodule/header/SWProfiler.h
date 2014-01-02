#ifndef SWProfiler_h__
#define SWProfiler_h__

#include "SWMemory.h"
#include "SWType.h"

#define SW_PROFILER(name) SWProfiler name(__FILE__,__LINE__,__FUNCTION__)
class SWProfiler : SWMemory
{
	float m_beginTime;
	const char* m_file;
	int m_line;
	const char* m_scope;
public:
	SWProfiler( const char* file, int line, const char* scope);
	~SWProfiler();
};

#endif // SWProfiler_h__