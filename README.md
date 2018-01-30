# perf
Simple libriary for functional performance testing.
Inspired by `GoogleTest` semantics, it implements simple and extendible way of 
wrapping your functional tests into C++ macrosses. 

Basic building blocks are: 

- [Perf.h](https://github.com/tigranmt/perf/blob/master/Perf/Perf.h):       Libriary almost completely is implemented inside it. 
- [Perf.cpp](https://github.com/tigranmt/perf/blob/master/Perf/Perf.cpp):   Test factory initialization and execution. 
- [Sys.h](https://github.com/tigranmt/perf/blob/master/Perf/Sys.h):         Performance API (Windows only) 
- [test.cpp](https://github.com/tigranmt/perf/blob/master/Perf/tests.cpp):  Sample test code



Include first 3 files into your project and create a `CPP` file, where you will define unit tests. 
Sample code:

```C++ 
#include <iostream>
#include "Perf.h"

using namespace std;

/*
* Custom reporter implementation, with different formatting option to console
*/
class SecReporter : public Perf::Reporter
{
	void ReportData(const Perf::PerfData& _perfData) override
	{
		cout << _perfData._testName.c_str() << "==>" << endl;
		cout << "\t Runtime: \t   " << _perfData._milliseconds/1000.0 << " sec" << endl;		
		cout << "\t Working set: \t   " << ((float)_perfData._workingSet) / 1024.0f /1024.f << " Mb" << endl;
		cout << "\t Working set peak: " << ((float)_perfData._workingSetPeak) / 1024.0f /1024.f << " Mb" << endl;		
		cout << endl;
	}
};


PERF(a, b)
{
	this->AssignReporter(new SecReporter());

	long a = 0; 
	while (a < 1000000000)
		a++;
};
```

In the code provided above we define a test `b` in fixture `a`. 
There is a possibility to extend default reporter, provided by libriary itself, with your custom implementaution. 
For example, code above demontrsates creation of `SecRepoter` reporter with different formatting option: reporting performance by seconds. 

The output on the consolde might look like this: 




![perf reporting](https://github.com/tigranmt/perf/blob/master/res/screen.png)




