#include "PCoarseSolver.h"
#include "SafeSimpleBitVar.h"
#include "TablePCT.h"

namespace cp {

PCoarseSolver::PCoarseSolver(HModel& hm, string& propagator_type, string& var_type, string& heu_type,
							 const int parallelism) :
	PSolver(hm, propagator_type, var_type, heu_type, parallelism) {
	vars.reserve(num_vars);
	tabs.reserve(num_tabs);
	//if (var_type_ == "SparseSet") {
	//	for (size_t i = 0; i < num_vars; i++) {
	//		auto xv = m->vars[i];
	//		vars.push_back(new SparseSetVar(xv->name, xv->id, num_vars, xv->vals, helper));
	//	}
	//}
	//else 
	if (var_type_ == "SSBV") {
		for (size_t i = 0; i < num_vars; i++) {
			auto xv = m->vars[i];
			vars.push_back(new SafeSimpleBitVar(xv->name, xv->id, num_vars, xv->vals, helper));
		}
	}

	if (propagator_type_ == "PCT") {
		for (size_t i = 0; i < num_tabs; ++i) {
			auto xc = m->tabs[i];
			vector<PVar*> scope(xc->scope.size());

			for (size_t j = 0; j < scope.size(); j++) {
				scope[j] = vars[xc->scope[j]->id];
			}

			tabs.emplace_back(new TablePCT(xc->id, xc->scope.size(), num_vars, scope, xc->tuples, helper));

			for (auto v : scope) {
				helper.subscription[v->Id()].push_back(tabs[i]);
			}
		}
	}
	/*else if (propagator_type_ == "CTBT") {
		for (size_t i = 0; i < num_tabs; ++i) {
			auto xc = m->tabs[i];
			vector<Var*> scope(xc->scope.size());

			for (auto j = 0; j < scope.size(); j++) {
				scope[j] = vars[xc->scope[j]->id];
			}

			tabs.emplace_back(new TableCTWithBitVar(xc->id, xc->scope.size(), num_vars, scope, xc->tuples, helper));

			for (auto v : scope) {
				helper.subscription[v->Id()].push_back(tabs[i]);
			}
		}
	}*/
}

}

