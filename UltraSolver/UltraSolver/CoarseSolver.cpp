#include "CoarseSolver.h"
#include "SimpleBitVar.h"
#include "TableCTWithBitVar.h"


namespace cp {

CoarseSolver::CoarseSolver(HModel& hm, string& propagator_type, string& var_type, string& heu_type) :
	Solver(hm, propagator_type, var_type, heu_type) {
	Q_.reset(new CoarseQueue<Var*>(num_vars));
	Y_evt_.reserve(m->max_arity());
	//q = new CoarseQueue<Var*>(num_vars);
	//q.Initial(num_vars);
	vars.reserve(num_vars);
	tabs.reserve(num_tabs);

	if (var_type_ == "SparseSet") {
		for (size_t i = 0; i < num_vars; i++) {
			auto xv = m->vars[i];
			vars.push_back(new SparseSetVar(xv->name, xv->id, num_vars, xv->vals, std::move(helper)));
		}
	}
	else if (var_type_ == "SBV") {
		for (size_t i = 0; i < num_vars; i++) {
			auto xv = m->vars[i];
			vars.push_back(new SimpleBitVar(xv->name, xv->id, num_vars, xv->vals, std::move(helper)));
		}
	}

	if (propagator_type_ == "CT") {
		for (size_t i = 0; i < num_tabs; ++i) {
			auto xc = m->tabs[i];
			vector<Var*> scope(xc->scope.size());

			for (size_t j = 0; j < scope.size(); j++) {
				scope[j] = vars[xc->scope[j]->id];
			}

			tabs.emplace_back(new TableCT(xc->id, xc->scope.size(), num_vars, scope, xc->tuples, std::move(helper)));

			for (auto v : scope) {
				helper->subscription[v->Id()].push_back(tabs[i]);
			}
		}
	}
	else if (propagator_type_ == "CTBT") {
		for (size_t i = 0; i < num_tabs; ++i) {
			auto xc = m->tabs[i];
			vector<Var*> scope(xc->scope.size());

			for (auto j = 0; j < scope.size(); j++) {
				scope[j] = vars[xc->scope[j]->id];
			}

			tabs.emplace_back(new TableCTWithBitVar(xc->id, xc->scope.size(), num_vars, scope, xc->tuples, std::move(helper)));

			for (auto v : scope) {
				helper->subscription[v->Id()].push_back(tabs[i]);
			}
		}
	}
}

bool CoarseSolver::InitialPropagate() {
	Y_evt_.clear();
	// 随便传一个为空的集合
	for (auto x : vars) {
		Y_evt_.push_back(x);
	}
	return propagate(Y_evt_);
}

bool CoarseSolver::CheckConsistencyAfterAssignment(vector<Var*>& x_evt) {
	return propagate(x_evt);
}

bool CoarseSolver::CheckConsistencyAfterRefutation(vector<Var*>& x_evt) {
	return propagate(x_evt);
}

bool CoarseSolver::propagate(vector<Var*>& x_evt) {
	Q_->clear();
	for (auto x : x_evt) {
		insert(x);
	}

	while (!Q_->IsEmpty()) {
		const auto v = Q_->pop();
		for (auto c : helper->subscription[v->Id()]) {
			if (helper->var_stamp[v->Id()] > helper->tab_stamp[c->Id()]) {
				Y_evt_.clear();
				const auto consistent = c->propagate(Y_evt_);
				if (!consistent) {
					return false;
				}
				else {
					for (auto y : Y_evt_) {
						insert(y);
					}
				}
				helper->global_stamp++;
				helper->tab_stamp[c->Id()] = helper->global_stamp;
			}
		}
	}
	return true;
}

void CoarseSolver::insert(Var* x) {
	Q_->push(x);
	++helper->global_stamp;
	helper->var_stamp[x->Id()] = helper->global_stamp;
}
}
