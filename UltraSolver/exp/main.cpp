#include "XCSP3CoreParser.h"
#include "XCSP3PrintCallbacks.h"
#include "HModel.h"
#include "BMFileParser.h"
#include "Timer.h"
using namespace XCSP3Core;
using namespace cudacp;

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
	m.show();
	return 0;
}
