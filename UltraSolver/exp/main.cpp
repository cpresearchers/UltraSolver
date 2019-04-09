#include "XCSP3CoreParser.h"
#include "XCSP3PrintCallbacks.h"
#include "HModel.h"
#include "BMFileParser.h"
#include "Timer.h"
#include "CoarseSolver.h"
//#include <PSearchHelper.h>
#include "PCoarseSolver.h"
using namespace XCSP3Core;
using namespace cp;

const string x_path = "BMPath.xml";

int main() {
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

	//string propagator_name = "CT";
	string propagator_name = "CTBT";
	//string var_name = "SparseSet";
	string var_name = "SBV";
	string heu_name;
	CoarseSolver ct(m, propagator_name, var_name, heu_name);
	ct.Search(time_limit);
	cout << "----------------ct----------------" << endl;
	cout << "time: " << ct.helper->time << endl;
	cout << "nodes: " << ct.helper->nodes << endl;

	//string propagator_name = "CT";
	propagator_name = "PCT";
	//string var_name = "SparseSet";
	var_name = "SSBV";
	for (auto i = 0; i <= 12; ++i) {
		PCoarseSolver pct(m, propagator_name, var_name, heu_name, i);
		pct.Search(time_limit);
		cout << "---------------pct: " << i << " --------------" << endl;
		cout << "time: " << pct.helper->time << endl;
		cout << "nodes: " << pct.helper->nodes << endl;

	}

	return 0;
}


