#include "PCoarseSolver.h"
namespace cp {
	PCoarseSolver::PCoarseSolver(HModel& m, string& propagator_type, string& var_type, string& heu_type,
		const int parallelism):PSolver(m,propagator_type,var_type,heu_type,parallelism)
	{
	}

bool PCoarseSolver::InitialPropagate() {
	helper->is_consistent = true;
	Y_evt.clear();
	helper->ClearPool();

	for (auto& v : vars) {
		Y_evt.push_back(v);
	}
	/*for (auto& c : tabs) {
		helper->AddToPool(c);
	}

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
	}*/

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
