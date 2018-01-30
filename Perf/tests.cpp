/*
* Sample codes
*/

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

PERF(c, d)
{

};


PERF(e, f)
{

};
