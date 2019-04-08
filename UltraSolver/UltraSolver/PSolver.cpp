//#include "PSearchHelper.h"
#include "PSolver.h"

namespace cp {
//PSolver::PSolver(HModel& m, string& propagator_type, string& var_type, string& heu_type, const int parallelism) :
//	propagator_type_(propagator_type),
//	var_type_(var_type),
//	heu_type_(heu_type),
//	num_vars(m.vars.size()),
//	num_tabs(m.tabs.size()),
//	m(&m) {
//	I.Initial(num_vars);
//	//helper.Initial(m);
//	helper.reset(new PSearchHelper(m, parallelism));
//	level_V_dense_.resize(num_vars, 0);
//	level_V_sparse_.resize(num_vars, 0);
//	for (size_t i = 0; i < num_vars; i++) {
//		level_V_dense_[i] = i;
//		level_V_sparse_[i] = i;
//	}
//}
//
//void PSolver::Search(int64_t time_limit) {
//	Timer t;
//	bool finished = false;
//	vector<PVar*> x_evt(num_vars);
//	x_evt.clear();
//
//	const u64 start = t.elapsed();
//	bool consistent = InitialPropagate();
//
//	if (!consistent) {
//		finished = false;
//		helper->time = t.elapsed() - start;
//	}
//
//	while (!finished) {
//		//超时
//		if (t.elapsed() > time_limit) {
//			helper->time = t.elapsed() - start;
//		}
//
//		// 选值 
//		PVal v_a = SelectVal();
//		++helper->nodes;
//		NewLevel();
//		I.push(v_a);
//		//cout << "push: " << v_a << ", " << helper->nodes << endl;
//
//		////选出的变量论域大小为1
//		//if (v_a.v->Size() != 1 && consistent) {
//		Bind(v_a);
//		x_evt.push_back(v_a.v);
//		consistent = CheckConsistencyAfterAssignment(x_evt);
//		x_evt.clear();
//		//}
//
//		if (consistent && I.full()) {
//			helper->time = t.elapsed() - start;
//			I.show();
//			finished = true;
//			return;
//		}
//
//		while (!consistent && !I.empty()) {
//			v_a = I.pop();
//			//cout << "pop:  " << v_a << ", " << helper->nodes << endl;
//
//			BackLevel();
//			//选出的变量论域大小不为1
//			Remove(v_a);
//
//			x_evt.push_back(v_a.v);
//			consistent = CheckConsistencyAfterRefutation(x_evt);
//			x_evt.clear();
//		}
//
//		if (!consistent) {
//			finished = true;
//		}
//	}
//
//	helper->time = t.elapsed() - start;
//}
//
//void PSolver::NewLevel() {
//	++helper->level;
//	for (auto v : vars) {
//		v->NewLevel();
//	}
//
//	for (auto c : tabs) {
//		c->NewLevel();
//	}
//}
//
//void PSolver::BackLevel() {
//	--helper->level;
//	for (auto v : vars) {
//		v->BackLevel();
//	}
//
//	for (auto c : tabs) {
//		c->BackLevel();
//	}
//}
//
//PVal PSolver::SelectVal() {
//	double mindmdd = DBL_MAX;
//	PVar* minv = nullptr;
//
//	for (auto i = helper->level; i < num_vars; ++i) {
//		const auto vid = level_V_dense_[i];
//		auto v = vars[vid];
//		double ddeg = 0.0;
//
//		for (auto c : helper->subscription[vid]) {
//			if (c->bind_count + 1 < c->arity) {
//				ddeg++;
//			}
//		}
//
//		if (ddeg == 0) {
//			return PVal{ v, v->MinValue() };
//		}
//
//		const auto sizeD = v->Size();
//		const double dmdd = sizeD / ddeg;
//		if (dmdd < mindmdd) {
//			minv = v;
//			mindmdd = dmdd;
//		}
//	}
//
//	return PVal{ minv, minv->MinValue() };
//}
//
//void PSolver::Bind(PVal& v_a) {
//	//在稀疏集上交换变量
//	const auto minvi = level_V_sparse_[v_a.v->Id()];
//	const auto a = level_V_dense_[helper->level - 1];
//	level_V_dense_[helper->level - 1] = level_V_dense_[minvi];
//
//	level_V_sparse_[a] = minvi;
//	level_V_sparse_[level_V_dense_[minvi]] = helper->level - 1;
//
//	level_V_dense_[minvi] = a;
//
//	for (auto c : helper->subscription[v_a.v->Id()]) {
//		c->bind_count++;
//	}
//
//	v_a.v->Bind(v_a.a);
//}
//
//void PSolver::Remove(PVal& v_a) const {
//	//约束的已实例化变量个数减1
//	for (auto c : helper->subscription[v_a.v->Id()]) {
//		c->bind_count--;
//	}
//	v_a.v->Remove(v_a.a);
//}
//
//PSolver::~PSolver() {
//	for (size_t i = 0; i < num_vars; i++) {
//		delete vars[i];
//	}
//
//	vars.clear();
//
//	for (size_t i = 0; i < num_tabs; i++) {
//		delete tabs[i];
//	}
//
//	tabs.clear();
//}
//
//void PSolver::show() {
//	vector<int> values;
//	cout << "----------------------show----------------------" << endl;
//	for (auto v : vars) {
//		v->GetValidValues(values);
//		cout << v->Id() << ": " << v->Size() << ": ";
//		for (auto a : values) {
//			cout << a << " ";
//		}
//		cout << endl;
//	}
//	cout << "----------------------show----------------------" << endl;
//}
PSolver::PSolver(HModel& m, string& propagator_type, string& var_type, string& heu_type, const int parallelism) :
	propagator_type_(propagator_type),
	var_type_(var_type),
	heu_type_(heu_type),
	num_vars(m.vars.size()),
	num_tabs(m.tabs.size()),
	m(&m) {
	I.Initial(num_vars);
	//helper.Initial(m);
	helper.reset(new PSearchHelper(m, parallelism));
	level_V_dense_.resize(num_vars, 0);
	level_V_sparse_.resize(num_vars, 0);
	for (size_t i = 0; i < num_vars; i++) {
		level_V_dense_[i] = i;
		level_V_sparse_[i] = i;
	}
}

void PSolver::Search(int64_t time_limit) {
	Timer t;
	bool finished = false;
	vector<PVar*> x_evt(num_vars);
	x_evt.clear();

	const u64 start = t.elapsed();
	bool consistent = InitialPropagate();

	if (!consistent) {
		finished = false;
		helper->time = t.elapsed() - start;
	}

	while (!finished) {
		//超时
		if (t.elapsed() > time_limit) {
			helper->time = t.elapsed() - start;
		}

		// 选值 
		PVal v_a = SelectVal();
		++helper->nodes;
		NewLevel();
		I.push(v_a);
		//cout << "push: " << v_a << ", " << helper->nodes << endl;

		////选出的变量论域大小为1
		//if (v_a.v->Size() != 1 && consistent) {
		Bind(v_a);
		x_evt.push_back(v_a.v);
		consistent = CheckConsistencyAfterAssignment(x_evt);
		x_evt.clear();
		//}

		if (consistent && I.full()) {
			helper->time = t.elapsed() - start;
			I.show();
			finished = true;
			return;
		}

		while (!consistent && !I.empty()) {
			v_a = I.pop();
			//cout << "pop:  " << v_a << ", " << helper->nodes << endl;

			BackLevel();
			//选出的变量论域大小不为1
			Remove(v_a);

			x_evt.push_back(v_a.v);
			consistent = CheckConsistencyAfterRefutation(x_evt);
			x_evt.clear();
		}

		if (!consistent) {
			finished = true;
		}
	}

	helper->time = t.elapsed() - start;
}

void PSolver::NewLevel() {
	++helper->level;
	for (auto v : vars) {
		v->NewLevel();
	}

	for (auto c : tabs) {
		c->NewLevel();
	}
}

void PSolver::BackLevel() {
	--helper->level;
	for (auto v : vars) {
		v->BackLevel();
	}

	for (auto c : tabs) {
		c->BackLevel();
	}
}

PVal PSolver::SelectVal() {
	double mindmdd = DBL_MAX;
	PVar* minv = nullptr;

	for (auto i = helper->level; i < num_vars; ++i) {
		const auto vid = level_V_dense_[i];
		auto v = vars[vid];
		double ddeg = 0.0;

		for (auto c : helper->subscription[vid]) {
			if (c->bind_count + 1 < c->arity) {
				ddeg++;
			}
		}

		if (ddeg == 0) {
			return PVal{ v, v->MinValue() };
		}

		const auto sizeD = v->Size();
		const double dmdd = sizeD / ddeg;
		if (dmdd < mindmdd) {
			minv = v;
			mindmdd = dmdd;
		}
	}

	return PVal{ minv, minv->MinValue() };
}

void PSolver::Bind(PVal& v_a) {
	//在稀疏集上交换变量
	const auto minvi = level_V_sparse_[v_a.v->Id()];
	const auto a = level_V_dense_[helper->level - 1];
	level_V_dense_[helper->level - 1] = level_V_dense_[minvi];

	level_V_sparse_[a] = minvi;
	level_V_sparse_[level_V_dense_[minvi]] = helper->level - 1;

	level_V_dense_[minvi] = a;

	for (auto c : helper->subscription[v_a.v->Id()]) {
		c->bind_count++;
	}

	v_a.v->Bind(v_a.a);
}

void PSolver::Remove(PVal& v_a) const {
	//约束的已实例化变量个数减1
	for (auto c : helper->subscription[v_a.v->Id()]) {
		c->bind_count--;
	}
	v_a.v->Remove(v_a.a);
}

PSolver::~PSolver() {
	for (size_t i = 0; i < num_vars; i++) {
		delete vars[i];
	}

	vars.clear();

	for (size_t i = 0; i < num_tabs; i++) {
		delete tabs[i];
	}

	tabs.clear();
}

void PSolver::show() {
	vector<int> values;
	cout << "----------------------show----------------------" << endl;
	for (auto v : vars) {
		v->GetValidValues(values);
		cout << v->Id() << ": " << v->Size() << ": ";
		for (auto a : values) {
			cout << a << " ";
		}
		cout << endl;
	}
	cout << "----------------------show----------------------" << endl;
}
}
