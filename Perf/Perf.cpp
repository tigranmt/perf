#include "Perf.h"

//initializes static runner with collection of tests
Perf::Factory Perf::Factory::_runner{};


//runs complete test suite
int main()
{	

	::Perf::Factory::RunAllTest();
	getchar();
}

