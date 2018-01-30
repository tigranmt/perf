

#pragma once
#include <iostream>
#include <chrono>
#include <vector>
#include <memory>
#include "Sys.h"
using namespace std;

namespace Perf
{
	
	/*
	* 	Sample performance data struct. 
	*	This structure is reported back to the caller of the test  
	*/
	struct PerfData
	{
		double _milliseconds;
		unsigned long _workingSet;		
		unsigned long _workingSetPeak;  
		string _hardwareInfo;
		string _testName;
	};


	/*
	* 	Run confioguraiton 	
	*/
	struct RunConfiguration
	{
		bool reportMemoryData;
		bool reportHardwareInfo; 

		RunConfiguration() : reportMemoryData(true), reportHardwareInfo(true) {}
	};


	/*
	*	Base class for reporter: a type responsible for visualizing, 
	*	reporting recovered performance information.
	*/
	class Reporter
	{
		public:
			void virtual ReportData(const Perf::PerfData&) {};
	};
	
	/*
	*	Built-in type for reporting performance data in formatted output on console
	*/
	class ConsoleReporter : public Reporter
	{
		void ReportData(const Perf::PerfData& _perfData)
		{
			cout << _perfData._testName.c_str() << "==>" << endl;
			cout << "\t Runtime: \t   " << _perfData._milliseconds << " ms" << endl;
			if (_perfData._workingSet > 0)
				cout << "\t Working set: \t   " << ((float)_perfData._workingSet)/1024.0f << " Kb" << endl;
			if (_perfData._workingSetPeak > 0)
				cout << "\t Working set peak: " << ((float)_perfData._workingSetPeak)/1024.0f << " Kb" << endl;
			cout << endl;
		}
	};

	
	class BasePerf 
	{

	private:
		PerfData _pd;	
		

	protected:
		unique_ptr<Reporter> _reporter;
		RunConfiguration _configuration;
		string _test_name;		

	
		BasePerf() 
		{ 
			_reporter = unique_ptr<Perf::Reporter>(new Perf::ConsoleReporter());
		}

		const PerfData& Data() const { return _pd; }	

		//Allows to inject user-specified reporter
		void AssignReporter(Perf::Reporter* reporter)  { _reporter = unique_ptr<Perf::Reporter>(reporter); }

		virtual void RunTestBody() = 0;
	public:
		
		void Run() 
		{
			
			//retrieve perf data _before_ user test invokation 
			auto start = chrono::steady_clock::now();	
			unsigned long prev_workingSet = 0, prev_workingSetPeak = 0;
			if (_configuration.reportMemoryData)
			{
				prev_workingSet		= System::Info::GetProcessWorkingSet();
				prev_workingSetPeak = System::Info::GetProcessWorkingSetPeak();
			}

			RunTestBody();

			//retrieve perf data _after_ user test invokation 
			auto end = chrono::steady_clock::now();
			
			if (_configuration.reportMemoryData)
			{
				auto cur_workingSet		= System::Info::GetProcessWorkingSet();
				auto cur_workingSetPeak = System::Info::GetProcessWorkingSetPeak();
				_pd._workingSet			= (cur_workingSet == prev_workingSet)? cur_workingSet :  cur_workingSet - prev_workingSet;
				_pd._workingSetPeak		= (cur_workingSetPeak == prev_workingSetPeak) ? cur_workingSetPeak : cur_workingSetPeak - prev_workingSetPeak;
			}

			_pd._milliseconds = chrono::duration <double, milli>(end - start).count();
			_pd._testName = _test_name;

			//reportm collected information
			_reporter->ReportData(_pd);

		}
	};

	/*
	*	Factory for constructing (code-generation) of a specified test cases
	*/
	class Factory
	{
	private:
		vector<unique_ptr<BasePerf>> tests;
		static Factory _runner;

	public:
		Factory() = default;
		bool Add(BasePerf* perf)
		{			
			tests.push_back(move(unique_ptr<BasePerf>(perf)));
			return true;
		}

		static bool Register(BasePerf* perf)
		{
			return _runner.Add(perf);
		}

		const vector<unique_ptr<BasePerf>>& Tests() const { return tests;}

		static void RunAllTest()
		{
			for (const auto& t : _runner.Tests())
				t->Run();
		}
	};


/*
*	Set of macrosses for generating classes from the name of the Ficture and Name of the test
*/

#define TYPE_FROM(perf_case, perf_test) perf_case##_##perf_test##_perf

#define PERF(perf_case, perf_test) PERF__(perf_case, perf_test, Perf::BasePerf)

#define PERF__(perf_case, perf_test, base_class) \
		class TYPE_FROM(perf_case, perf_test) : public base_class \
		{	\
			protected:\
				void RunTestBody() override; \
				static bool registered_sucessfully;\
				\
				TYPE_FROM(perf_case, perf_test)() { _test_name = " "  #perf_case "." #perf_test " "; } \
			\
		};\
		\
		bool TYPE_FROM(perf_case, perf_test)::registered_sucessfully = \
			::Perf::Factory::Register(new TYPE_FROM(perf_case, perf_test)());\
		\
		void TYPE_FROM(perf_case, perf_test)::RunTestBody()
}
