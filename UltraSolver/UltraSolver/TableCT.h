#pragma once
#include <memory>
#include "RSBitSet.h"
#include "Propagator.h"
#include <iostream>
#include "Var.h"
#include "SparseSetVar.h"
#include "SearchHelper.h"
namespace cp {
using namespace std;
class TableCT : public Propagator {
public:
	vector<vector<int>> tuples;

	TableCT(const int id, const int arity, const int num_vars, vector<Var*> scope, vector<vector<int>>& tuples, SearchHelper& helper) :
		Propagator(id, num_vars, scope, helper), tuples(tuples) {
		curr_table_.reset(new RSBitSet(tuples.size(), num_vars));
		num_bit_ = int(ceil(double(tuples.size()) / double(Constants::BITSIZE)));
		supports_.resize(arity);
		residues_.resize(arity);

		for (size_t i = 0; i < arity; ++i) {
			auto size = scope[i]->Size();
			supports_[i].resize(scope[i]->Size(), vector<u64>(num_bit_));
			residues_[i].resize(scope[i]->Size(), -1);
		}

		for (size_t i = 0; i < tuples.size(); ++i) {
			const auto idx2D = Constants::GetIndex2D(i);
			auto& t = tuples[i];

			for (size_t j = 0; j < t.size(); ++j) {
				supports_[j][t[j]][idx2D.x] |= Constants::MASK1[idx2D.y];
			}
		}

		Ssup_.reserve(arity);
		Sval_.resize(arity);
		last_size_.resize(arity, -1);
		old_size_.resize(arity, 0x3f3f3f3f);
	}

	//检查变量
	bool InitGAC() {
		bool changed = false;
		Ssup_.clear();
		Sval_.clear();

		for (size_t i = 0; i < arity; ++i) {
			auto& v = scope[i];

			if (last_size_[i] != v->Size()) {
				Sval_.push_back(i);
				last_size_[i] = v->Size();
				changed = true;
			}

			// 未赋值或刚赋值 
			if (v->NeedFilterDomain()) {
				Ssup_.push_back(i);
			}
		}

		return changed;
	}

	bool UpdateTable() {
		const size_t num_sval = Sval_.size();
		for (size_t i = 0; i < num_sval; ++i) {
			const int vv = Sval_[i];
			auto v = scope[vv];
			curr_table_->ClearMask();

			// !!此处delta重写了一次
			if ((old_size_[vv] - v->Size()) < v->Size()) {
				// delta更新
				v->GetLastRemoveValues(old_size_[vv], values_);
				for (auto a : values_) {
					curr_table_->AddToMask(supports_[vv][a]);
				}
				curr_table_->ReverseMask();
			}
			else {
				// 重头重新
				v->GetValidValues(values_);
				for (auto a : values_) {
					curr_table_->AddToMask(supports_[vv][a]);
				}
			}

			bool changed = curr_table_->IntersectWithMask();
			//传播失败
			if (curr_table_->IsEmpty()) {
				return false;
			}
		}

		return true;
	}

	bool FilterDomains(vector<Var*> & y) {
		y.clear();
		const size_t num_ssup = Ssup_.size();
		for (size_t i = 0; i < num_ssup; ++i) {
			bool deleted = false;
			const int vv = Ssup_[i];
			auto v = scope[vv];
			v->GetValidValues(values_);

			for (auto a : values_) {
				int index = residues_[vv][a];
				if (index == -1 || !curr_table_->CheckValid(index, supports_[vv][a][index])) {
					index = curr_table_->IntersectIndex(supports_[vv][a]);
					if (index != -1) { //重新找到支持
						residues_[vv][a] = index;
					}
					else {
						deleted = true;
						//无法找到支持, 删除(v, a)
						//	cout << "name: " << Id() << ", delete: " << v->Id() << "," << a << endl;
						v->Remove(a);
					}
				}
			}

			if (deleted) {
				last_size_[vv] = v->Size();
				old_size_[vv] = v->Size();

				y.push_back(v);

				if (v->IsEmpty()) {
					return false;
				}
			}
		}

		return true;
	}

	bool propagate(vector<Var*> & x_evt) override {
		//L32~L33
		InitGAC();
		if (!UpdateTable()) {
			return false;
		}
		return FilterDomains(x_evt);
	}

	void NewLevel() override {
		level++;
		curr_table_->NewLevel();
	}

	void BackLevel() override {
		curr_table_->DeleteLevel();
		level--;
		for (auto i = 0; i < arity; ++i) {
			last_size_[i] = scope[i]->Size();
			old_size_[i] = last_size_[i];
		}
	}
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

