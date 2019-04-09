#include "XCSP3CoreParser.h"
#include "XCSP3PrintCallbacks.h"
#include "HModel.h"
#include "BMFileParser.h"
#include "Timer.h"
#include "CoarseSolver.h"
//#include <PSearchHelper.h>
#include "PCoarseSolver.h"
#include <stdlib.h>  
#include <crtdbg.h>  

using namespace XCSP3Core;
using namespace cp;

const string x_path = "BMPath.xml";

//#ifdef _DEBUG  
//#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)  
//#endif  
//
//void EnableMemLeakCheck() {
//	int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
//	tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
//	_CrtSetDbgFlag(tmpFlag);
//}


int main() {

	//EnableMemLeakCheck();
	string bm_path;
	if (FindBMPath(x_path))
		bm_path = _bm_path;
	cout << bm_path << endl;
	HModel m(true);
	Timer t;
	GetHModel(bm_path, m);
	const auto build_time = t.elapsed();
	cout << build_time << endl;
	//m.show();
	const u64 time_limit = 1800000000000;

	string propagator_name = "CT";
	//string propagator_name = "CTBT";
	string var_name = "SparseSet";
	//string var_name = "SBV";
	string heu_name;
	cout << "----------------ct----------------" << endl;
	CoarseSolver ct(m, propagator_name, var_name, heu_name);
	ct.Search(time_limit);
	cout << "time: " << ct.helper->time << endl;
	cout << "nodes: " << ct.helper->nodes << endl;

	//string propagator_name = "CT";
	propagator_name = "PCT";
	//string var_name = "SparseSet";
	var_name = "SSBV";
	for (auto i = 0; i <= 12; ++i) {
		cout << "---------------pct: " << i << " --------------" << endl;
		PCoarseSolver pct(m, propagator_name, var_name, heu_name, i);
		pct.Search(time_limit);
		cout << "time: " << pct.helper->time << endl;
		cout << "nodes: " << pct.helper->nodes << endl;
	}
	//EnableMemLeakCheck();
	return 0;
}


