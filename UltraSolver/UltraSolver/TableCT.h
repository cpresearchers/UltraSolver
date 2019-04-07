#pragma once
#include <memory>
#include "RSBitSet.h"
#include "Propagator.h"
#include "Var.h"
#include "SearchHelper.h"
namespace cp {
using namespace std;
class TableCT : public Propagator {
public:
	vector<vector<int>> tuples;

	TableCT(const int id, const int arity, const int num_vars, vector<Var*> scope, vector<vector<int>>& tuples,
			shared_ptr<SearchHelper>&& helper);

	//检查变量
	bool InitGAC();

	bool UpdateTable();

	bool FilterDomains(vector<Var*>& y);

	bool propagate(vector<Var*>& x_evt) override;

	void NewLevel() override;

	void BackLevel() override;
private:
	int num_bit_;

	unique_ptr<RSBitSet> curr_table_;
	vector<vector<vector<u64>>> supports_;
	vector<vector<int>> residues_;
	vector<int> Ssup_;
	vector<int> Sval_;
	vector<int> last_size_;
	vector<int> old_size_;

};
}

