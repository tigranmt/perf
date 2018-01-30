/*
* Class contains Windows performance primitves only
*/
#pragma once
#ifdef _WIN32 
	#include "windows.h"
	#include "psapi.h"
#endif
namespace System
{
	struct Info 
	{
		static unsigned long GetProcessWorkingSet()
		{
	#ifdef _WIN32 
			PROCESS_MEMORY_COUNTERS pmc;
			GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
			return pmc.WorkingSetSize;
	#endif
		}

		static unsigned long GetProcessWorkingSetPeak()
		{
	#ifdef _WIN32 
			PROCESS_MEMORY_COUNTERS pmc;
			GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
			return pmc.PeakWorkingSetSize;
	#endif
		}
	};
}



