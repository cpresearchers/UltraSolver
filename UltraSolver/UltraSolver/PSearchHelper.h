#pragma once
#include "typedef.h"
#include <vector>
#include <memory>
#include <taskflow/taskflow.hpp>
#include "HModel.h"
#include "PPropagator.h"

namespace cp {
using namespace std;
class PSearchHelper {
public:
	// 各时间戳
	u64 global_stamp = 0;
	std::vector<u64> tab_stamp;
	std::vector<u64> var_stamp;
	// 搜索时间
	u64 time = 0;
	u64 nodes = 0;
	bool is_consistent = true;
	// 搜索上限
	u64 time_limit = 0;
	u64 node_limit = 0;
	u64 failure_limit = 0;

	int level = 0;

	// 只能用智能指针了
	unique_ptr<tf::Taskflow> pool;
	//vector<int> in_pool;
	vector<atomic<int>> in_cevt;

	// 线程启动次数
	u64 p_sum = 0;
	// 约束传播次数
	u64 c_sum = 0;

	vector<vector<PPropagator*>> subscription;

	void Initial(HModel& m, const int parallelism) {
		num_vars_ = m.vars.size();
		num_tabs_ = m.tabs.size();
		var_stamp.resize(num_vars_, 0);
		tab_stamp.resize(num_tabs_, 0);
		subscription.resize(num_tabs_);
		pool = make_unique<tf::Taskflow>(parallelism);
		//in_pool.resize(m.tabs.size(), 0);
		in_cevt = vector<atomic<int>>(num_tabs_, 0);
	}

	void ClearCevt() {
		for (auto i = 0; i < num_tabs_; i++) {
			in_cevt[i].store(0);
		}
	}

	void  AddToCevt(PPropagator* c) {
		if (in_cevt[c->Id()] == 0) {
			pool->emplace(move(*c));
			in_cevt[c->Id()].store(1);
		}
	}

protected:
	int num_vars_ = 0;
	int num_tabs_ = 0;
};
}
