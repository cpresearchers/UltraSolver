#pragma once
#include <taskflow/taskflow.hpp>  // Cpp-Taskflow is header-only
#include "typedef.h"
#include "HModel.h"

namespace cp {
using namespace std;
using Closure = std::function<void()>;
class PSearchHelper;

class PVar {
public:
	PVar(string& name, const int id, const int num_vars, vector<int>& values,
		 shared_ptr<PSearchHelper>&& helper) :
		helper(helper),
		name_(name),
		id_(id),
		num_vars_(num_vars),
		values_(values),
		capacity_(values.size()) {};
	int Id() const { return id_; }
	int Capacity() const { return capacity_; }
	//vector<int> last_remove_values;
	//vector<int> valid_values;

	virtual int NewLevel() = 0;
	virtual int BackLevel() = 0;
	virtual int Size() = 0;
	virtual void Bind(const int a) = 0;
	inline bool IsBind() const { return bind_level_ != Constants::kINTMAX; };
	//inline bool IsBindOrLastPast() const { return bind_level_ <= level_; };
	inline bool IsLastPast() const { return bind_level_ == level_; };
	inline bool NeedFilterDomain() const { return level_ <= bind_level_; };
	virtual void Remove(const int a) = 0;
	virtual bool IsEmpty() = 0;
	virtual void Restrict() = 0;
	virtual void Mark(const int a) = 0;
	virtual bool FullMark() = 0;
	int BindLevel() const { return bind_level_; }
	virtual bool Contains(const int a) = 0;
	virtual int MinValue() = 0;
	virtual int MaxValue() = 0;
	virtual int NextValue(const int a) = 0;
	//vector<int>& values
	//virtual void GetLastRemoveValues(const int last) = 0;
	//virtual void GetValidValues() = 0;
	virtual void GetLastRemoveValues(const u64 last, vector<int>& values) = 0;
	virtual void GetValidValues(vector<int>& values) = 0;
	virtual u64 SimpleMask() = 0;
	virtual u64 SubmitMask(const u64 mask) = 0;
	virtual u64 SubmitMaskAndGet(const u64 mask) = 0;
	virtual u64 GetAndSubmitMask(const u64 mask) = 0;
	const shared_ptr<PSearchHelper> helper;

protected:
	string name_;
	int id_;
	int num_vars_;
	vector<int> values_;
	int level_ = 0;
	int capacity_ = 0;
	int bind_level_ = Constants::kINTMAX;
};

class PPropagator {
public:
	PPropagator(const int id, const int num_vars, vector<PVar*>& scope, shared_ptr<PSearchHelper>&& helper) :
		arity(scope.size()),
		num_vars(num_vars),
		scope(scope),
		helper(helper),
		id_(id) {};
	virtual ~PPropagator() = default;
	inline int Id() const { return id_; };
	int arity;
	int bind_count = 0;
	int level = 0;
	int num_assigned = 0;
	int num_vars = 0;
	vector<PVar*> scope;
	//SearchHelper* helper;
	const shared_ptr<PSearchHelper> helper;
	virtual bool propagate(vector<PVar*>& x_evt) = 0;
	virtual void NewLevel() = 0;
	virtual void BackLevel() = 0;

protected:
	// last remove values and valid values
	vector<int> values_;
	int id_;

};

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
		//subscription(vector<vector<PPropagator*>>(m.vars.size())),
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
