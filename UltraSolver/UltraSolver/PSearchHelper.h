#pragma once
//#include <taskflow/taskflow.hpp>  // Cpp-Taskflow is header-only
#include "Timer.h"
#include "typedef.h"
#include <taskflow/taskflow.hpp>

#include "HModel.h"
#include "RSBitSet.h"
#include "AssignedStack.h"


namespace cp {
using namespace std;
using Closure = std::function<void()>;
class PSearchHelper;

class PVar {
public:
	virtual ~PVar() = default;

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
	const shared_ptr<PSearchHelper> helper;
	virtual bool propagate() = 0;
	virtual void NewLevel() = 0;
	virtual void BackLevel() = 0;
	//virtual void operator()() = 0;
protected:
	// last remove values and valid values
	vector<int> values_;
	int id_;

};

using Closure = std::function<void()>;
//
//class PSearchHelper {
//public:
//	int num_vars;
//	int num_tabs;
//	// 各时间戳
//	u64 global_stamp = 0;
//	vector<u64> var_stamp;
//	vector<u64> tab_stamp;
//
//	// 搜索时间
//	u64 time = 0;
//	u64 nodes = 0;
//	bool is_consistent = true;
//	// 搜索上限
//	u64 time_limit = 0;
//	u64 node_limit = 0;
//	u64 failure_limit = 0;
//
//	int level = 0;
//
//	// 线程启动次数
//	u64 p_sum = 0;
//	// 约束传播次数
//	u64 c_sum = 0;
//
//	u64 num_pro = 0;
//
//	vector<vector<PPropagator*>> subscription;
//	vector<Closure> initial_tasks;
//	vector<vector<Closure>> srp_tasks;
//
//	//tf::Taskflow pool;
//	unique_ptr<tf::WorkStealingThreadpool<Closure>>  pool;
//
//	//tf::WorkStealingThreadpool<Closure> pool;
//	//unique_ptr<tf::Taskflow> pool;
//	vector<atomic<int>> in_pool;
//
//	PSearchHelper(HModel& m, const int parallelism) :
//		num_vars(m.vars.size()),
//		num_tabs(m.tabs.size()),
//		var_stamp(vector<u64>(num_vars, 0)),
//		tab_stamp(vector<u64>(num_tabs, 0)),
//		subscription(vector<vector<PPropagator*>>(m.vars.size())),
//		initial_tasks(vector<Closure>(m.tabs.size())),
//		srp_tasks(vector<vector<Closure>>(m.vars.size())),
//		//pool(parallelism),
//		//pool(tf::Taskflow(parallelism)),
//		pool(make_unique<tf::WorkStealingThreadpool<Closure>>(parallelism)),
//		in_pool(vector<atomic<int>>(num_tabs)) {
//		//pool.reset(new tf::Taskflow(parallelism));
//		//tf::Taskflow t;
//		//tf::WorkStealingThreadpool<Propagator> pool2(5);
//
//		//pool.reset();
//
//	}
//
//	void run(PPropagator* c) const {
//		if (is_consistent)
//			c->propagate();
//	}
//
//	//void run(PPropagator* c, tf::SubflowBuilder subflow) {
//
//	//}
//
//	bool InPool(PPropagator* c) {
//		return !(!in_pool[c->Id()].load());
//	}
//
//	bool InPool(const int cid) {
//		return !(!in_pool[cid].load());
//	}
//
//	void AddToPool(PPropagator* c) {
//		if (!InPool(c->Id())) {
//			//pool.emplace(std::move(*c));
//			pool.emplace([c, this]() {run(c); });
//			in_pool[c->Id()].store(1);
//			++num_pro;
//		}
//	}
//
//	void ClearPool() {
//		num_pro = 0;
//		for (int i = 0; i < num_tabs; ++i) {
//			in_pool[i].store(0);
//		}
//	}
//
//	void WaitForAll() {
//		pool.wait_for_all();
//	}
//};



class PSearchHelper {
public:
	int num_vars;
	int num_tabs;
	// 各时间戳
	u64 global_stamp = 0;
	vector<u64> var_stamp;
	vector<u64> tab_stamp;

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

	u64 num_pro = 0;

	vector<vector<PPropagator*>> subscription;
	vector<Closure> tasks;
	//vector<vector<Closure>> srp_tasks;

	unique_ptr<tf::WorkStealingThreadpool<Closure>>  pool;
	vector<int> in_pool;
	promise<void> prom;
	future<void> fut = prom.get_future();
	atomic<u64> counter;
	bool flag = false;

	PSearchHelper(HModel& m, const int parallelism);

	//void run(PPropagator* c) const {
	//	if (is_consistent)
	//		c->propagate();
	//}

	void init_tasks(vector<PPropagator*>& pp);

	//void run(PPropagator* c, tf::SubflowBuilder subflow) {

	//}

	bool InPool(PPropagator* c);

	bool InPool(const int cid);

	void Emplace(const int cid);

	//void AddToPool(PPropagator* c) {
	//	if (!InPool(c->Id())) {
	//		//pool.emplace(std::move(*c));
	//		pool.emplace([c, this]() {run(c); });
	//		pool->batch()
	//		in_pool[c->Id()].store(1);
	//		++num_pro;
	//	}
	//}

	void ClearPool() {
		//num_pro = 0;
		for (int i = 0; i < num_tabs; ++i) {
			in_pool[i].store(0);
		}
	}

	//void WaitForAll() {
	//	pool.wait_for_all();
	//}

	//void InvokeAll(const int vid) {
	//	pool->batch(std::move(srp_tasks[vid]));

	//}

	void WaitForAll() {
		fut.get();
	}
};

class PVal {
public:
	PVar* v;
	int a;
};

inline ostream& operator<<(ostream& os, PVal& va) {
	os << va.v->Id() << ", " << va.a << " ";
	return os;
}

class SafeSimpleBitVar :public PVar {
public:
	SafeSimpleBitVar(string& name, const int id, const int num_vars, vector<int>& values, shared_ptr<PSearchHelper>&& helper);
	int NewLevel() override;
	int BackLevel() override;
	int Size() override;
	void Bind(const int a) override;
	void Remove(const int a) override;
	bool IsEmpty() override;
	void Restrict() override;
	void Mark(const int a) override;
	bool FullMark() override;
	bool Contains(const int a) override;
	int MinValue() override;
	int MaxValue() override;
	int NextValue(const int a) override;
	void GetLastRemoveValues(const u64 last, vector<int>& values) override;
	void GetValidValues(vector<int>& values) override;
	u64 SimpleMask() override;
	u64 SubmitMask(const u64 mask) override;
	u64 SubmitMaskAndGet(const u64 mask) override;
	u64 GetAndSubmitMask(const u64 mask) override;
protected:
	int limit_;
	int num_level_;
	vector<int> size_level_;
	atomic<u64> bit_mark_;
	vector<atomic<u64>> bit_doms_;
};
//
//class TablePCT :public PPropagator {
//public:
//	vector<std::vector<int>> tuples;
//
//	TablePCT(const int id, const int arity, const int num_vars, vector<PVar*> scope, vector<vector<int>>& tuples,
//			 shared_ptr<PSearchHelper>&& helper);
//
//	//检查变量
//	bool InitGAC();
//
//	bool UpdateTable();
//
//	bool FilterDomains();
//
//	bool propagate() override;
//
//	void NewLevel() override;
//
//	void BackLevel() override;
//
//	void operator()();
//
//private:
//	int num_bit_;
//
//	unique_ptr<RSBitSet> curr_table_;
//	vector<vector<vector<u64>>> supports_;
//	vector<vector<int>> residues_;
//	vector<int> Ssup_;
//	vector<int> Sval_;
//	vector<u64> last_mask_;
//	vector<u64> old_mask_;
//	bool first_prop_ = true;
//
//};
//
//class PSolver {
//public:
//	const int num_vars;
//	const int num_tabs;
//	vector<PVar*> vars;
//	vector<PPropagator*> tabs;
//	shared_ptr<PSearchHelper> helper;
//	AssignedStack<PVal> I;
//	HModel* m;
//
//	PSolver(HModel& m, string& propagator_type, string& var_type, string& heu_type, const int parallelism);
//
//	void Search(int64_t time_limit);;
//
//	void NewLevel();
//
//	void BackLevel();
//
//
//	//修改levelvdense
//	PVal SelectVal();
//
//	void Bind(PVal& v_a);
//
//	void Remove(PVal& v_a) const;
//
//	virtual bool InitialPropagate() = 0;
//
//	virtual bool CheckConsistencyAfterAssignment(vector<PVar*>& x_evt) = 0;
//
//	virtual bool CheckConsistencyAfterRefutation(vector<PVar*>& x_evt) = 0;
//
//	virtual ~PSolver();
//
//	void show();
//protected:
//	vector<int> level_V_dense_;
//	vector<int> level_V_sparse_;
//	string& propagator_type_;
//	string& var_type_;
//	string& heu_type_;
//};

}
