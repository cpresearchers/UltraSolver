#pragma once
#include <taskflow/threadpool/threadpool.hpp>
#include <taskflow/taskflow.hpp>  // Cpp-Taskflow is header-only
#include "typedef.h"
#include "Propagator.h"
#include "HModel.h"
//#include <taskflow/taskflow.hpp>  // Cpp-Taskflow is header-only
#include "PPropagator.h"
#include <atomic>


namespace cp {
using namespace std;
using Closure = std::function<void()>;
class PSearchHelper {
public:
	// 各时间戳
	u64 global_stamp = 0;
	vector<u64> tab_stamp;
	vector<u64> var_stamp;
	// 搜索时间
	u64 time = 0;
	u64 nodes = 0;
	bool is_consistent = true;
	// 搜索上限
	u64 time_limit = 0;
	u64 node_limit = 0;
	u64 failure_limit = 0;

	int level = 0;

	// 线程启动次数
	u64 p_sum = 0;
	// 约束传播次数
	u64 c_sum = 0;

	vector<vector<PPropagator*>> subscription;

	tf::Taskflow pool;
	//unique_ptr<tf::WorkStealingThreadpool<PPropagator*>>  pool;

	//tf::WorkStealingThreadpool<Closure> pool;
	//unique_ptr<tf::Taskflow> pool;
	vector<atomic<int>> in_pool;

	PSearchHelper(HModel& m, const int parallelism) :
		tab_stamp(vector<u64>(m.tabs.size(), 0)),
		var_stamp(vector<u64>(m.vars.size(), 0)),
		subscription(vector<vector<PPropagator*>>(m.vars.size())),
		//pool(parallelism),
		//pool(tf::Taskflow(parallelism)),
		pool(tf::Taskflow(parallelism)),
		in_pool(vector<atomic<int>>(m.vars.size())) {
		//pool.reset(new tf::Taskflow(parallelism));
		//tf::Taskflow t;
		//tf::WorkStealingThreadpool<Propagator> pool2(5);

		//pool.reset();
	}

	//void Initial(HModel& m) {
	//	var_stamp.resize(m.vars.size(), 0);
	//	tab_stamp.resize(m.tabs.size(), 0);
	//	subscription.resize(m.vars.size());
	//}
};
}
