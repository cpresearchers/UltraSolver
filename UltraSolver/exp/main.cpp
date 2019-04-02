#include "XCSP3CoreParser.h"
#include "XCSP3PrintCallbacks.h"
#include "HModel.h"
#include "BMFileParser.h"
#include "Timer.h"
#include "CoarseSolver.h"
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
	string propagator_name = "CT";
	string var_name = "SparseSet";
	string heu_name;
	CoarseSolver ct(m, propagator_name, var_name, heu_name);
	ct.Search(time_limit);
	cout << "time: " << ct.helper.time << endl;
	cout << "nodes: " << ct.helper.nodes << endl;
	return 0;
}
