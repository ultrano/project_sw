#ifndef SWProfiler_h__
#define SWProfiler_h__

#include "SWMemory.h"
#include <vector>

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

class SWProfileCenter : SWMemory
{
	SWProfileCenter();
	~SWProfileCenter();
public:
	struct Profile
	{
		const char* file;
		int line;
		const char* scope;
		float spendTime;
	};
	class MainFrame
	{
	public:
		MainFrame();
		~MainFrame();
	};
	float m_beginFrameTime;
	std::vector<Profile> m_profiles;

	static SWProfileCenter& getInstance();
};

#endif // SWProfiler_h__