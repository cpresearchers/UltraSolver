#pragma once
#include <vector>
#include <memory>
#include "RSBitSet.h"
#include "PPropagator.h"

namespace cp {
using namespace std;
class TablePCT : public PPropagator {
public:
	vector<std::vector<int>> tuples;

	TablePCT(const int id, const int arity, const int num_vars, vector<PVar*> scope, vector<vector<int>>& tuples,
					  PSearchHelper& helper);

	//������
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
	vector<u64> last_mask_;
	vector<u64> old_mask_;
	bool first_prop_ = true;
};
}

