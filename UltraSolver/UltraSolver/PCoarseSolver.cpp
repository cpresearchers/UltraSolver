#include "PCoarseSolver.h"
#include "TablePCT.h"

namespace cp {
PCoarseSolver::PCoarseSolver(HModel& hm, string& propagator_type, string& var_type, string& heu_type,
							 const int parallelism) :PSolver(hm, propagator_type, var_type, heu_type, parallelism) {
	if (var_type_ == "SSBV") {
		for (size_t i = 0; i < num_vars; i++) {
			auto xv = m->vars[i];
			vars.push_back(new SafeSimpleBitVar(xv->name, xv->id, num_vars, xv->vals, std::move(helper)));
		}
	}

	if (propagator_type_ == "PCT") {
		for (size_t i = 0; i < num_tabs; ++i) {
			auto xc = m->tabs[i];
			vector<PVar*> scope(xc->scope.size());

			for (size_t j = 0; j < scope.size(); j++) {
				scope[j] = vars[xc->scope[j]->id];
			}

			tabs.emplace_back(new TablePCT(xc->id, xc->scope.size(), num_vars, scope, xc->tuples, std::move(helper)));

			for (auto v : scope) {
				helper->subscription[v->Id()].push_back(tabs[i]);
			}
		}
	}

}

bool PCoarseSolver::InitialPropagate() {
	helper->is_consistent = true;
	Y_evt.clear();
	helper->ClearPool();

	for (auto& v : vars) {
		Y_evt.push_back(v);
	}

	while (!Y_evt.empty()) {
		helper->ClearPool();

		for (auto v : Y_evt) {
			for (auto c : helper->subscription[v->Id()]) {
				helper->AddToPool(c);
			}
		}

		Y_evt.clear();

		++helper->global_stamp;
		helper->WaitForAll();
		helper->c_sum += helper->num_pro;
		++helper->p_sum;

		if (!helper->is_consistent) {
			return false;
		}

		++helper->global_stamp;
		Y_evt.clear();

		for (int i = helper->level; i < num_vars; ++i) {
			const auto vid = level_V_dense_[i];
			auto v = vars[vid];
			if (helper->var_stamp[vid] == helper->global_stamp) {
				Y_evt.push_back(v);
			}
		}
	}

	return true;
}

bool PCoarseSolver::CheckConsistencyAfterAssignment(vector<PVar*>& x_evt) {
	helper->is_consistent = true;
	Y_evt.clear();

	for (auto& x : x_evt) {
		Y_evt.push_back(x);
	}

	while (!Y_evt.empty()) {
		helper->ClearPool();

		for (auto v : Y_evt) {
			for (auto c : helper->subscription[v->Id()]) {
				helper->AddToPool(c);
			}
		}

		Y_evt.clear();

		++helper->global_stamp;
		helper->WaitForAll();
		helper->c_sum += helper->num_pro;
		++helper->p_sum;

		if (!helper->is_consistent) {
			return false;
		}

		++helper->global_stamp;
		Y_evt.clear();

		for (int i = helper->level; i < num_vars; ++i) {
			const auto vid = level_V_dense_[i];
			auto v = vars[vid];
			if (helper->var_stamp[vid] == helper->global_stamp) {
				Y_evt.push_back(v);
			}
		}
	}

	return true;
}

bool PCoarseSolver::CheckConsistencyAfterRefutation(vector<PVar*>& x_evt) {
	helper->is_consistent = true;
	Y_evt.clear();

	for (auto& x : x_evt) {
		Y_evt.push_back(x);
	}

	while (!Y_evt.empty()) {
		helper->ClearPool();

		for (auto v : Y_evt) {
			for (auto c : helper->subscription[v->Id()]) {
				helper->AddToPool(c);
			}
		}

		Y_evt.clear();

		++helper->global_stamp;
		helper->WaitForAll();
		helper->c_sum += helper->num_pro;
		++helper->p_sum;

		if (!helper->is_consistent) {
			return false;
		}

		++helper->global_stamp;
		Y_evt.clear();

		for (int i = helper->level; i < num_vars; ++i) {
			const auto vid = level_V_dense_[i];
			auto v = vars[vid];
			if (helper->var_stamp[vid] == helper->global_stamp) {
				Y_evt.push_back(v);
			}
		}
	}

	return true;
}
}
