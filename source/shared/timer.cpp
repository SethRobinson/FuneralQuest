

#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#else
#include <sys/time.h>
#endif

#include "timer.h"

static int initialized = 0;

#ifdef _WIN32

static BOOL has_hires_timer;

static LARGE_INTEGER start_ticks;
static LARGE_INTEGER ticks_per_second;
static LARGE_INTEGER ticks;

static unsigned int start_time;

#else

static unsigned int start_time;
static struct timeval now;

#endif

// ===========================================================================
// init_timer
// ===========================================================================

void init_timer(void)
{
	if (initialized) 
	{ 
		return; 
	}

	initialized = 1;

#ifdef _WIN32

	if (QueryPerformanceFrequency(&ticks_per_second) != 0)
	{
		has_hires_timer = TRUE;
		QueryPerformanceCounter(&start_ticks);
	}
	else
	{
		has_hires_timer = FALSE;
		start_time = timeGetTime();
	}	

#else

	gettimeofday(&now, NULL);
	start_time = (now.tv_sec * 1000) + (now.tv_usec / 1000);

#endif
}

// ===========================================================================
// get_time
// ===========================================================================

unsigned int get_time(void)
{
#ifdef _WIN32

	if (has_hires_timer)
	{
		QueryPerformanceCounter(&ticks);

		ticks.QuadPart -= start_ticks.QuadPart;
		ticks.QuadPart *= 1000;
		ticks.QuadPart /= ticks_per_second.QuadPart;

		return (unsigned int) ticks.QuadPart;
	}
	else
	{
		return (timeGetTime() - start_time);
	}

#else

	gettimeofday(&now, NULL);
	return (now.tv_sec * 1000) + (now.tv_usec / 1000) - start_time;

#endif
}
