#pragma once
#include "Var.h"
namespace cp {
using namespace std;
class Propagator {
public:
	Propagator(const int id, const int num_vars, vector<Var*>& scope, SearchHelper& helper) :id(id), arity(scope.size()), num_vars(num_vars), scope(scope), helper(&helper) {};
	virtual ~Propagator() = default;
	int id;
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

};
}

