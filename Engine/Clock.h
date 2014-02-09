// ***************************************************************
//  CLOCK			
//  -------------------------------------------------------------
//  Purpose :	Engine Clock System, used to produce HeartBeat 
//				for entire system.
// ***************************************************************

#ifndef CLOCK_H
#define CLOCK_H

#include <Windows.h>

struct StopWatch
{
	LARGE_INTEGER	start;
	LARGE_INTEGER	stop;
};

class Clock
{
public:
	Clock();
	void		Initialize();
	float		GetTheTick();
	float		getTotaltime();

	void		StartTimer();
	void		StopTimer();
	double		getStopWatchTime();
	double		LIToSecs(const LARGE_INTEGER& L);

private:
	__int64			m_initialTimeStamp,
					m_currentTimeStamp;

	float			m_secondsPerCount;

	StopWatch		m_stopWatchTimer;
	LARGE_INTEGER	m_frequency;

};

#endif