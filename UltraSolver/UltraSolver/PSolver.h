#pragma once
#include "PSearchHelper.h"
#include "Timer.h"
#include "AssignedStack.h"
#include "HModel.h"

namespace cp {

class PSolver {
public:
	const int num_vars;
	const int num_tabs;
	vector<PVar*> vars;
	vector<PPropagator*> tabs;
	shared_ptr<PSearchHelper> helper;
	AssignedStack<PVal> I;
	HModel* m;

	PSolver(HModel& m, string& propagator_type, string& var_type, string& heu_type, const int parallelism);

	void Search(int64_t time_limit);;

	void NewLevel();

	void BackLevel();


	//修改levelvdense
	PVal SelectVal();

	void Bind(PVal& v_a);

	void Remove(PVal& v_a) const;

	virtual bool InitialPropagate() = 0;

	virtual bool CheckConsistencyAfterAssignment(vector<PVar*>& x_evt) = 0;

	virtual bool CheckConsistencyAfterRefutation(vector<PVar*>& x_evt) = 0;

	virtual ~PSolver();

	void show();
protected:
	vector<int> level_V_dense_;
	vector<int> level_V_sparse_;
	string& propagator_type_;
	string& var_type_;
	string& heu_type_;
	vector<PVar*> Y_evt;
};
}
