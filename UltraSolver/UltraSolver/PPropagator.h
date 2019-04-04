#pragma once
#include "typedef.h"
#include <vector>
#include "Var.h"
#include "PSearchHelper.h"

namespace cp {
using namespace std;
class PPropagator {
public:
	PPropagator(const int id, const int num_vars, vector<PVar*>& scope, PSearchHelper& helper) :arity(scope.size()), num_vars(num_vars), scope(scope), helper(&helper), id_(id) {};
	virtual ~PPropagator() = default;
	inline int Id() const { return id_; };
	int arity;
	int bind_count = 0;
	int level = 0;
	int num_assigned = 0;
	int num_vars = 0;
	vector<PVar*> scope;
	PSearchHelper* helper;
	virtual void operator()() = 0;
	virtual bool propagate(vector<Var*>& x_evt) = 0;
	virtual void NewLevel() = 0;
	virtual void BackLevel() = 0;

	void GetRemovedValues(u64 last_mask, u64 mask) {
		values_.clear();
		// ȡmask
		const auto b = (last_mask & (~mask));
		for (int i = CLZ64(b), end = CTZ64(b); i <= end; ++i) {
			//if (BIT_CHECK(b, i)) {
			if (!!(b & Constants::MASK1[i])) {
				values_.push_back(i);
			}
		}
	}

	void GetValidValues(u64 mask) {
		values_.clear();
		// ȡmask
		const auto b = ~mask;
		for (int i = CLZ64(b), end = CTZ64(b); i <= end; ++i) {
			//if (BIT_CHECK(b, i)) {
			if (!!(b & Constants::MASK1[i])) {
				values_.push_back(i);
			}
		}
	}

protected:
	// last remove values and valid values
	vector<int> values_;
	int id_;
};
}
