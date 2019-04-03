#pragma once
#include <vector>
#include "Propagator.h"
#include <memory>
#include "RSBitSet.h"

namespace cp {
using namespace std;
class TableCTWithBitVar : public Propagator {
public:
	vector<std::vector<int>> tuples;

	TableCTWithBitVar(const int id, const int arity, const int num_vars, vector<Var*> scope, vector<vector<int>>& tuples,
					  SearchHelper& helper);

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
	vector<PVar*> pscope ;
	vector<int> Ssup_;
	vector<int> Sval_;
	vector<u64> last_mask_;
	vector<u64> old_mask_;
	bool first_prop_ = true;
};
}

