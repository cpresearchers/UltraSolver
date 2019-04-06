#include "TablePCT.h"

namespace cp {
//TablePCT::TablePCT() {}
//
//
//TablePCT::~TablePCT() {}

TablePCT::TablePCT(const int id, const int arity, const int num_vars, vector<PVar*> scope,
				   vector<vector<int>>& tuples, PSearchHelper& helper) :
	PPropagator(id, num_vars, scope, helper), tuples(tuples) {
	curr_table_.reset(new RSBitSet(tuples.size(), num_vars));
	num_bit_ = int(ceil(double(tuples.size()) / double(Constants::BITSIZE)));
	supports_.resize(arity);
	residues_.resize(arity);

	for (auto i = 0; i < arity; ++i) {
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


bool TablePCT::InitGAC() {
	bool changed = false;
	Ssup_.clear();
	Sval_.clear();

	for (auto i = 0; arity > i; ++i) {
		auto& v = scope[i];
		const auto mask = v->SimpleMask();

		if (last_mask_[i] != mask) {
			Sval_.push_back(i);
			last_mask_[i] = mask;
			changed = true;
		}

		// δ��ֵ��ո�ֵ 
		if (v->NeedFilterDomain()) {
			Ssup_.push_back(i);
		}
	}

	return changed;
}

bool TablePCT::UpdateTable() {
	for (int vv : Sval_) {
		if (!helper->is_consistent) {
			return false;
		}
		auto v = scope[vv];
		curr_table_->ClearMask();

		const auto last_removed_mask = (~last_mask_[vv]) & old_mask_[vv];
		//const auto num_valid = BitCount64(last_mask_[vv]);
		const auto num_remove = BitCount64(last_removed_mask);

		//if (num_remove >= num_valid || first_prop_) {

		if (num_remove >= v->Size() || first_prop_) {
			// ��ͷ����
			v->GetValidValues(values_);
			for (auto a : values_) {
				curr_table_->AddToMask(supports_[vv][a]);
			}
		}
		else {
			// delta����
			v->GetLastRemoveValues(old_mask_[vv], values_);
			for (auto a : values_) {
				curr_table_->AddToMask(supports_[vv][a]);
			}
			curr_table_->ReverseMask();
		}

		bool changed = curr_table_->IntersectWithMask();
		//����ʧ��
		if (curr_table_->IsEmpty()) {
			helper->is_consistent = false;
			return false;
		}
	}

	first_prop_ = false;
	return true;
}

bool TablePCT::FilterDomains(vector<Var*> & y) {
	y.clear();
	for (int vv : Ssup_) {
		if (!helper->is_consistent) {
			return false;
		}
		bool deleted = false;
		auto v = scope[vv];
		v->GetValidValues(values_);

		for (auto a : values_) {
			int index = residues_[vv][a];
			if (index == -1 || !curr_table_->CheckValid(index, supports_[vv][a][index])) {
				index = curr_table_->IntersectIndex(supports_[vv][a]);
				if (index != -1) {
					//�����ҵ�֧��
					residues_[vv][a] = index;
				}
				else {
					deleted = true;
					//�޷��ҵ�֧��, ɾ��(v, a)
					//cout << "name: " << Id() << ", delete: " << v->Id() << "," << a << endl;
					//v->Remove(a);
					last_mask_[vv] &= Constants::MASK0[a];
					//BIT_CLEAR(last_mask_[vv], a);
				}
			}
		}

		if (deleted) {
			helper->var_stamp[v->Id()] = helper->global_stamp + 1;
			auto mask = v->SubmitMaskAndGet(last_mask_[vv]);
			//v->SubmitMaskAndGet(last_mask_[vv]);
			//last_mask_[vv] = v->SimpleMask();
			//old_mask_[vv] = last_mask_[vv];
			//y.push_back(v);

			if (v->IsEmpty()) {
				helper->is_consistent = false;
				return false;
			}

			old_mask_[vv] = last_mask_[vv];
		}
	}

	return true;
}

bool TablePCT::propagate(vector<Var*> & x_evt) {
	//L32~L33
	InitGAC();
	if (!UpdateTable()) {
		return false;
	}
	return FilterDomains(x_evt);
}

void TablePCT::NewLevel() {
	level++;
	curr_table_->NewLevel();
}

void TablePCT::BackLevel() {
	curr_table_->DeleteLevel();
	level--;
	for (auto i = 0; i < arity; ++i) {
		last_mask_[i] = scope[i]->SimpleMask();
		old_mask_[i] = last_mask_[i];
	}
}

void TablePCT::operator()() const {}
}

