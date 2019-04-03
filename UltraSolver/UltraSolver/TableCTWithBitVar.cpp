#include "TableCTWithBitVar.h"
#include <iostream>

namespace cp {
TableCTWithBitVar::TableCTWithBitVar(const int id, const int arity, const int num_vars, vector<Var*> scope, vector<vector<int>>& tuples,
									 SearchHelper& helper) :
	Propagator(id, num_vars, scope, helper), tuples(tuples) {
	curr_table_.reset(new RSBitSet(tuples.size(), num_vars));
	num_bit_ = int(ceil(double(tuples.size()) / double(Constants::BITSIZE)));
	supports_.resize(arity);
	residues_.resize(arity);

	pscope_.resize(arity);
	for (auto i = 0; i < arity; ++i) {
		pscope_[i] = dynamic_cast<PVar*>(scope[i]);
	}

	for (auto i = 0; i < arity; ++i) {
		auto size = pscope_[i]->Size();
		supports_[i].resize(scope[i]->Size(), vector<u64>(num_bit_));
		residues_[i].resize(scope[i]->Size(), -1);
	}

	for (auto i = 0; i < tuples.size(); ++i) {
		const auto idx2D = Constants::GetIndex2D(i);
		auto& t = tuples[i];

		for (auto j = 0; j < t.size(); ++j) {
			supports_[j][t[j]][idx2D.x] |= Constants::MASK1[idx2D.y];
		}
	}

	Ssup_.reserve(arity);
	Sval_.resize(arity);
	last_mask_.resize(arity, 0);
	old_mask_.resize(arity, Constants::kALLONELONG);
}

bool TableCTWithBitVar::InitGAC() {
	bool changed = false;
	Ssup_.clear();
	Sval_.clear();

	for (auto i = 0; arity > i; ++i) {
		auto& v = pscope_[i];
		const auto mask = v->SimpleMask();

		if (last_mask_[i] != mask) {
			Sval_.push_back(i);
			changed = true;
		}

		last_mask_[i] = mask;

		// 未赋值或刚赋值 
		if (v->NeedFilterDomain()) {
			Ssup_.push_back(i);
		}
	}

	return changed;
}

bool TableCTWithBitVar::UpdateTable() {
	const auto num_sval = Sval_.size();
	for (auto i = 0; i < num_sval; ++i) {
		const int vv = Sval_[i];
		auto v = pscope_[vv];
		curr_table_->ClearMask();

		const auto last_removed_mask = (~last_mask_[vv]) & old_mask_[vv];
		//const auto num_valid = BitCount64(last_mask_[vv]);
		const auto num_remove = BitCount64(last_removed_mask);

		//if (num_remove >= num_valid || first_prop_) {

		if (num_remove >= v->Size() || first_prop_) {
			// 重头重新
			v->GetValidValues(values_);
			for (auto a : values_) {
				curr_table_->AddToMask(supports_[vv][a]);
			}
		}
		else {
			// delta更新
			v->GetLastRemoveValues(old_mask_[vv], values_);
			for (auto a : values_) {
				curr_table_->AddToMask(supports_[vv][a]);
			}
			curr_table_->ReverseMask();
		}

		bool changed = curr_table_->IntersectWithMask();
		//传播失败
		if (curr_table_->IsEmpty()) {
			return false;
		}
	}

	first_prop_ = false;
	return true;
}

bool TableCTWithBitVar::FilterDomains(vector<Var*> & y) {
	y.clear();
	const size_t num_ssup = Ssup_.size();
	for (size_t i = 0; i < num_ssup; ++i) {
		bool deleted = false;
		const int vv = Ssup_[i];
		auto v = pscope_[vv];
		v->GetValidValues(values_);

		for (auto a : values_) {
			int index = residues_[vv][a];
			if (index == -1 || !curr_table_->CheckValid(index, supports_[vv][a][index])) {
				index = curr_table_->IntersectIndex(supports_[vv][a]);
				if (index != -1) {
					//重新找到支持
					residues_[vv][a] = index;
				}
				else {
					deleted = true;
					//无法找到支持, 删除(v, a)
					cout << "name: " << Id() << ", delete: " << v->Id() << "," << a << endl;
					v->Remove(a);
					//BIT_CLEAR(last_mask_[vv], a);
				}
			}
		}

		if (deleted) {
			//v->SubmitMaskAndGet(last_mask_[vv]);
			last_mask_[vv] = v->SimpleMask();
			old_mask_[vv] = last_mask_[vv];
			y.push_back(v);

			if (v->IsEmpty()) {
				return false;
			}
		}
	}

	return true;
}

bool TableCTWithBitVar::propagate(vector<Var*> & x_evt) {
	//L32~L33
	InitGAC();
	if (!UpdateTable()) {
		return false;
	}
	return FilterDomains(x_evt);
}

void TableCTWithBitVar::NewLevel() {
	level++;
	curr_table_->NewLevel();
}

void TableCTWithBitVar::BackLevel() {
	curr_table_->DeleteLevel();
	level--;
	for (auto i = 0; i < arity; ++i) {
		last_mask_[i] = pscope_[i]->SimpleMask();
		old_mask_[i] = last_mask_[i];
	}
}

}
