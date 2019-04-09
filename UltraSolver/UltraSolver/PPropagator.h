#pragma once
#include "PSearchHelper.h"
//#include "PVar.h"

namespace cp {
using namespace std;
class PPropagator {
public:
	PPropagator(const int id, const int num_vars, vector<PVar*>& scope, shared_ptr<PSearchHelper>&& helper) :
		arity(scope.size()),
		num_vars(num_vars),
		scope(scope),
		helper(std::move(helper)),
		id_(id) {};
	~PPropagator() {};
	virtual void operator()() = 0;
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

protected:
	vector<int> values_;
	int id_;

};
}
