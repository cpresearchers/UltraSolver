#pragma once
#include "Var.h"
#include "Propagator.h"
#include "SearchHelper.h"
#include "Timer.h"
#include "AssignedStack.h"
#include "Val.h"
#include "HModel.h"

namespace cp {

class Solver {
public:
	const int num_vars;
	const int num_tabs;
	vector<Var*> vars;
	vector<Propagator*> tabs;
	SearchHelper helper;
	AssignedStack<Val> I;
	HModel* m;

	Solver(HModel& m, string& propagator_type, string& var_type, string& heu_type) :
		propagator_type_(propagator_type),
		var_type_(var_type),
		heu_type_(heu_type),
		num_vars(m.vars.size()),
		num_tabs(m.tabs.size()),
		m(&m) {
		I.Initial(num_vars);
		helper.Initial(m);
	}

	void Search(int64_t time_limit) {
		Timer t;
		bool finished = false;
		vector<Var*> x_evt(num_vars);
		x_evt.clear();

		const u64 start = t.elapsed();
		bool consistent = InitialPropagate();

		if (!consistent) {
			finished = false;
			helper.time = t.elapsed() - start;
		}

		while (!finished) {
			//超时
			if (t.elapsed() > time_limit) {
				helper.time = t.elapsed() - start;
			}

			// 选值 
			Val v_a = SelectVal();
			++helper.nodes;
			NewLevel();
			I.push(v_a);
			cout << "---------------------------" << endl;
			cout << "push: " << v_a << endl;
			cout << "---------------------------" << endl;

			////选出的变量论域大小为1
			//if (v_a.v->Size() != 1 && consistent) {
			Bind(v_a);
			x_evt.push_back(v_a.v);
			consistent = CheckConsistencyAfterAssignment(x_evt);
			x_evt.clear();
			//}

			if (consistent && I.full()) {
				helper.time = t.elapsed() - start;
				I.show();
				finished = true;
				return;
			}

			while (!consistent && !I.empty()) {
				v_a = I.pop();
				cout << "---------------------------" << endl;
				cout << "pop:  " << v_a << endl;
				cout << "---------------------------" << endl;
				BackLevel();
				//选出的变量论域大小不为1
				Remove(v_a);
				++helper.nodes;
				x_evt.push_back(v_a.v);
				consistent = CheckConsistencyAfterRefutation(x_evt);
				x_evt.clear();
			}

			if (!consistent) {
				finished = true;
			}
		}

		helper.time = t.elapsed() - start;
	};

	void  NewLevel() {
		++helper.level;
		for (auto v : vars) {
			v->NewLevel();
		}

		for (auto c : tabs) {
			c->NewLevel();
		}
	}

	void BackLevel() {
		--helper.level;
		for (auto v : vars) {
			v->BackLevel();
		}

		for (auto c : tabs) {
			c->BackLevel();
		}
	}

	virtual Val SelectVal() = 0;

	virtual void Bind(Val& va) = 0;

	virtual void Remove(Val& va) = 0;

	virtual bool InitialPropagate() = 0;

	virtual bool CheckConsistencyAfterAssignment(vector<Var*>& x_evt) = 0;

	virtual bool CheckConsistencyAfterRefutation(vector<Var*>& x_evt) = 0;

	~Solver() {
		for (size_t i = 0; i < num_vars; i++) {
			delete vars[i];
		}

		vars.clear();

		for (size_t i = 0; i < num_tabs; i++) {
			delete tabs[i];
		}

		tabs.clear();
	}

protected:
	string& propagator_type_;
	string& var_type_;
	string& heu_type_;
};
}
