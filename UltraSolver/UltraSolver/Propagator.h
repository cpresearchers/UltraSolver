#pragma once
#include "Var.h"
namespace cp {
using namespace std;
class Propagator {
public:
	Propagator(const int id, const int num_vars, vector<Var*>& scope, SearchHelper& helper) :arity(scope.size()), num_vars(num_vars), scope(scope), helper(&helper), id_(id) {};
	virtual ~Propagator() = default;
	inline int Id() const { return id_; };
	int arity;
	int bind_count = 0;
	int level = 0;
	int num_assigned = 0;
	int num_vars = 0;
	vector<Var*> scope;
	SearchHelper* helper;
	virtual bool propagate(vector<Var*>& x_evt) = 0;
	virtual void NewLevel() = 0;
	virtual void BackLevel() = 0;

protected:
	// last remove values and valid values
	vector<int> values_;
	int id_;
};
}

