#include "Clock.h"

//------------------------------------------------------------------------
// Purpose  : Constructor
//------------------------------------------------------------------------
Clock::Clock()
{
	__int64	countsPerSec	=	m_initialTimeStamp;
	m_secondsPerCount		=	0.0f;


	// Function retrives the frequency of the high resolution
	// performance counter if it exixts
	// this frequency cannot change while the system is runnning!
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	m_secondsPerCount		=	1.0f/(float)countsPerSec;

	// StopWatch Stuff
	QueryPerformanceFrequency(&m_frequency);
	m_stopWatchTimer.start.QuadPart = 0;
	m_stopWatchTimer.stop.QuadPart = 0;
}

//------------------------------------------------------------------------
// Purpose  : Initialize Counter
//------------------------------------------------------------------------
void	Clock::Initialize()
{
	// retrieves the current value of the high resoltion performance counter 
	QueryPerformanceCounter((LARGE_INTEGER*)&m_initialTimeStamp);
}

//------------------------------------------------------------------------
// Purpose  : Get Total time since the Game Started
//------------------------------------------------------------------------
float	Clock::getTotaltime()
{
	return ((float)m_currentTimeStamp) * m_secondsPerCount;
}

//------------------------------------------------------------------------
// Purpose  : Update & generate the Tick!
//------------------------------------------------------------------------
float	Clock::GetTheTick()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_currentTimeStamp);

	float dt = ((float)m_currentTimeStamp - (float)m_initialTimeStamp) * m_secondsPerCount;
	m_initialTimeStamp = m_currentTimeStamp;

	return dt;
}

double Clock::LIToSecs( const LARGE_INTEGER & L) 
{
	return ((double)L.QuadPart /(double)m_frequency.QuadPart) ;
}

void Clock::StartTimer( ) 
{
	QueryPerformanceCounter(&m_stopWatchTimer.start) ;
}
 
 void Clock::StopTimer( ) 
 {
     QueryPerformanceCounter(&m_stopWatchTimer.stop) ;
 }

 double Clock::getStopWatchTime() 
 {
     LARGE_INTEGER time;
     time.QuadPart = m_stopWatchTimer.stop.QuadPart - m_stopWatchTimer.start.QuadPart;
     return LIToSecs( time) ;
 }
 
