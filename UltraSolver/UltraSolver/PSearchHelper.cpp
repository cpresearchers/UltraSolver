#include "PSearchHelper.h"

namespace cp {
PSearchHelper::PSearchHelper(HModel& m, const int parallelism) :
	num_vars(m.vars.size()),
	num_tabs(m.tabs.size()),
	var_stamp(vector<u64>(num_vars, 0)),
	tab_stamp(vector<u64>(num_tabs, 0)),
	subscription(vector<vector<PPropagator*>>(m.vars.size())),
	tasks(vector<Closure>(num_tabs)),
	//srp_tasks(vector<vector<Closure>>(m.vars.size())),
	pool(make_unique<tf::WorkStealingThreadpool<Closure>>(parallelism)),
	in_pool(vector<int>(num_tabs, 0)) {}

void PSearchHelper::init_tasks(vector<PPropagator*>& pp) {
	for (size_t i = 0; i < num_tabs; i++) {
		tasks[i] = [this, &pp, i] {
			if (is_consistent) {
				pp[i]->propagate();
			}

			if (!(--counter) && flag) {
				prom.set_value();
			}
		};
	}
}

bool PSearchHelper::InPool(PPropagator* c) {
	return !(!in_pool[c->Id()]);
}

bool PSearchHelper::InPool(const int cid) {
	return !(!in_pool[cid]);
}

void PSearchHelper::Emplace(const int cid)
{
	if (!InPool(cid))
	{
		++counter;
		pool->emplace(std::move(tasks[cid]));
		in_pool[cid] = 1;
		++num_pro;
	}
}
}
