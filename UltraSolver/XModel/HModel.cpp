//
// Created by leezear on 18-4-6.
//

#include "HModel.h"
#include <utility>

namespace cudacp {
////////////////////////////////////////////////////////////////////
HVar::HVar(const int id, const int uid, string name, const int min_val, const int max_val) :
	id(id), uid(uid), name(std::move(name)), std_max(max_val - min_val), is_std(min_val == 0) {
	int j = 0;
	const int size = max_val - min_val + 1;
	vals.resize(size);
	anti_map.resize(size);
	for (int i = min_val; i <= max_val; ++i) {
		val_map[i] = j;
		vals[j] = j;
		anti_map[j] = i;
		++j;
	}
}

HVar::HVar(const int id, const int uid, const string name, vector<int> &v) :
	id(id), uid(uid), name(name), std_max(v.size() - 1) {
	vals.resize(v.size());
	anti_map = v;
	for (int i = 0; i < vals.size(); ++i) {
		val_map[v[i]] = i;
		vals[i] = i;
	}
}

void HVar::show() {
	cout << id << "| " << name << ": ";
	for (int i = 0; i < vals.size(); ++i) {
		cout << vals[i] << "[" << anti_map[i] << "] ";
	}
	cout << endl;
}

HVar::~HVar() {}

////////////////////////////////////////////////////////////////////
HTab::HTab(const int id, const bool sem, vector<vector<int>> &ts, vector<HVar *> &scp, const bool STD) :
	id(id), scope(scp), semantics(sem), isSTD(STD) {
	unsigned long all_size = 1;
	for (auto i : scp)
		all_size *= i->vals.size();
	unsigned long sup_size;

	if (!sem)
		sup_size = all_size - ts.size();
	else
		sup_size = ts.size();

	//非标准表
	if (!STD) {
		vector<int> ori_t_(scope.size());
		vector<int> std_t_(scope.size());
		tmp_t_.resize(scope.size());
		tuples.resize(sup_size, vector<int>(scope.size()));

		if (sem) {
			for (int i = 0; i < sup_size; i++) {
				GetSTDTuple(ts[i], std_t_);
				//tuples[i] = std_t_;
				tuples[i].assign(std_t_.begin(), std_t_.end());
			}
		}
		else {
			int i = 0, j = 0;
			GetTuple(i++, ori_t_, std_t_);
			for (auto& t : ts) {
				while (ori_t_ != t) {
					tuples[j++].assign(std_t_.begin(), std_t_.end());
					GetTuple(i++, ori_t_, std_t_);
				}
				GetTuple(i++, ori_t_, std_t_);
			}
			while (j < sup_size) {
				tuples[j++].assign(std_t_.begin(), std_t_.end());
				GetTuple(i++, ori_t_, std_t_);
			}
		}
	}
	else {
		tuples = std::move(ts);
	}

	semantics = true;
	isSTD = true;
}

HTab::HTab(const int id, const bool sem, vector<vector<int>> &ts, vector<HVar *> &scp, const bool transform, const bool AsPre) :
	id(id), scope(scp), isSTD(AsPre), semantics(sem) {
	//没转过
	if (!AsPre) {
		vector<int> ori_t_(scope.size());
		vector<int> std_t_(scope.size());

		//是否全部为标准论域
		bool standard = true;
		for (auto v : scp) {
			if (!v->is_std) {
				standard = false;
				break;
			}
		}
		//需转正表
		if (transform) {
			//转正表
			//语义转正
			unsigned long all_size = 1;
			for (auto i : scp)
				all_size *= i->vals.size();
			unsigned long sup_size;

			if (!semantics)
				sup_size = all_size - ts.size();
			else
				sup_size = ts.size();

			//原表为负表
			if (!semantics) {
				tuples.resize(sup_size, vector<int>(scope.size()));
				int i = 0, j = 0;
				GetTuple(i++, ori_t_, std_t_);

				for (auto& t : ts) {
					while (ori_t_ != t) {
						tuples[j++].assign(std_t_.begin(), std_t_.end());
						GetTuple(i++, ori_t_, std_t_);
					}
					GetTuple(i++, ori_t_, std_t_);
				}

				while (j < sup_size) {
					tuples[j++].assign(std_t_.begin(), std_t_.end());
					GetTuple(i++, ori_t_, std_t_);
				}
			}
			else {
				//原表为正表
				if (!standard) {
					//非标准表转标准表
					for (auto & t : ts) {
						ToSTDTuple(t);
					}
				}
				//标准论域
				tuples = ts;
			}
			semantics = true;
			isSTD = true;
		}
		else {
			//不转需正表，保持原样，但需标准化
			if (!standard) {
				//非标准表转标准表
				for (auto & t : ts) {
					ToSTDTuple(t);
				}
			}
			tuples = ts;
		}
	}
	else {
		tuples = ts;
	}

}

HTab::HTab(HTab *t, vector<HVar *> &scp) :
	scope(scp), semantics(t->semantics) {
	id = t->id + 1;
	isSTD = t->isSTD;
	tuples = t->tuples;
}

//void HTab::GetSTDTuple(vector<int>& tuple) {
//	for (int i = 0; i < tuple.size(); ++i)
//		tuple[i] = scope[i]->val_map[tuple[i]];
//}
void HTab::GetSTDTuple(vector<int> &src_tuple, vector<int> &std_tuple) {
	for (int i = 0; i < src_tuple.size(); ++i)
		std_tuple[i] = scope[i]->val_map[src_tuple[i]];
}

void HTab::ToSTDTuple(vector<int>& t) {
	for (int i = 0; i < t.size(); ++i)
		t[i] = scope[i]->val_map[t[i]];
}

void HTab::GetORITuple(vector<int> &std_tuple, vector<int> &ori_tuple) {
	for (int i = 0; i < std_tuple.size(); ++i)
		ori_tuple[i] = scope[i]->anti_map[std_tuple[i]];
}

int HTab::GetAllSize() const {
	int size = 1;
	for (auto v : scope)
		size *= v->vals.size();
	return size;
}

bool HTab::SAT(vector<int> &t) {
	//return find(tuples.begin(), tuples.end(), t) != tuples.end();
	return binary_search(tuples.begin(), tuples.end(), t);
}

bool HTab::SAT_STD(vector<int> &t) {
	GetORITuple(t, tmp_t_);
	return binary_search(tuples.begin(), tuples.end(), tmp_t_);
}

void HTab::GetTuple(int idx, vector<int> &src_t, vector<int> &std_t) {
	for (int i = (scope.size() - 1); i >= 0; --i) {
		HVar *v = scope[i];
		std_t[i] = idx % v->vals.size();
		src_t[i] = v->anti_map[std_t[i]];
		idx /= v->vals.size();
	}
}

ostream &operator<<(ostream &os, const vector<HVar *> &a) {
	for (auto v : a)
		os << v->id << "[" << v->name << "] ";
	return os;
}

void HTab::show() {
	const string sem = semantics ? "supports" : "conflicts";
	cout << "id: " << id << " semantics: " << sem << " size: " << tuples.size()
		<< " arity:" << scope.size() << " scope = {" << scope << endl;
	for (auto t : tuples) {
		cout << "( ";
		for (auto i : t)
			cout << i << " ";
		cout << ")";
	}
	cout << endl;
}
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
HModel::HModel() {
	// TODO Auto-generated constructor stub

}

HModel::HModel(bool transform) :need_positive_(transform) {}

int HModel::AddVar(const string& name, const int min_val, const int max_val) {
	const int id = vars.size();
	const int uid = generate_var_uid();
	HVar *var = new HVar(id, uid, name, min_val, max_val);
	str_var_map_[name] = var;
	int_var_map_[uid] = var;
	vars.push_back(var);
	mds_ = max(mds_, int(var->vals.size()));
	return id;
}

int HModel::AddVar(const string& name, vector<int> &v) {
	const int id = vars.size();
	const int uid = generate_var_uid();
	HVar *var = new HVar(id, uid, name, v);
	str_var_map_[name] = var;
	int_var_map_[uid] = var;
	vars.push_back(var);
	mds_ = max(mds_, int(var->vals.size()));
	return id;
}

int HModel::AddTab(const bool sem, vector<vector<int>> &ts, vector<HVar *> &scp, const bool STD) {
	const int id = tabs.size();
	HTab *t = new HTab(id, sem, ts, scp, need_positive_, STD);
	tabs.push_back(t);
	mas_ = max(mas_, int(t->scope.size()));
	//subscript(t);
	return id;
}

int HModel::AddTab(const bool sem, vector<vector<int>> &ts, vector<string> &scp) {
	vector<HVar *> scope;
	get_scope(scp, scope);
	return AddTab(sem, ts, scope);
}

int HModel::AddTabAsPrevious(HTab *t, vector<string> &scp) {
	vector<HVar *> scope;
	get_scope(scp, scope);
	HTab *nt = new HTab(t, scope);
	tabs.push_back(nt);
	mas_ = max(mas_, int(nt->scope.size()));
	//subscript(t);
	return tabs.size() - 1;
}

void HModel::show() {
	cout << "--------------Variables--------------" << endl;
	cout << "size: " << vars.size() << "\tmax domain size :" << mds_ << endl;
	for (auto v : vars)
		v->show();
	cout << "-------------Constraints-------------" << endl;
	cout << "size: " << tabs.size() << "\tmax arity size :" << mas_ << endl;
	for (auto t : tabs)
		t->show();
}

bool HModel::is_positive() const {
	return need_positive_;
}

//void HModel::subscript(HTab *t) {
//	for (auto v : t->scope)
//		subscriptions[v].push_back(t);
//	neighbor(t);
//}

//void HModel::neighbor(HTab *t) {
//	if (neighborhoods.empty()) 
//		neighborhoods.reserve(vars.size(), vector<vector<int>>(vars.size()));

//		//neighborhoods.resize(int(vars.size()), vector<vector<int>>(vars.size()));


//	for (auto x : t->scope)
//		for (auto y : t->scope)
//			if (x != y)
//				neighborhoods[x->id][y->id].push_back(t->id);
//}

void HModel::get_scope(vector<string> &scp_str, vector<HVar *> &scp) {
	scp.resize(scp_str.size());
	for (int i = 0; i < scp_str.size(); ++i)
		scp[i] = str_var_map_[scp_str[i]];
}

int HModel::get_var_id(const int id) const {
	return var_uid_ - MAX_VALUE - 1;
}

int HModel::generate_exp_uid() {
	return ++exp_id_;
}

int HModel::generate_var_uid() {
	return ++var_uid_;
}

void HModel::GetSTDTuple(vector<int> &src_tuple, vector<int> &std_tuple, vector<HVar *> &scp) {
	for (int i = 0; i < src_tuple.size(); ++i)
		std_tuple[i] = scp[i]->val_map[src_tuple[i]];
}

void HModel::GetORITuple(vector<int> &std_tuple, vector<int> &ori_tuple, vector<HVar *> &scp) {
	for (int i = 0; i < std_tuple.size(); ++i)
		ori_tuple[i] = scp[i]->anti_map[std_tuple[i]];
}


void HModel::get_ori_tuple_by_index(int idx, std::vector<int> &t, const vector<HVar *> scp) {
	for (int i = scp.size() - 1; i >= 0; --i) {
		const int size = scp[i]->vals.size();
		t[i] = idx % size;
		idx /= size;
	}
}

//void HModel::result(const int op, vector<int> &result, const int len) {
//	vector<int> a(len);
//	for (int i = len - 1; i >= 0; --i) {
//		a[i] = result.back();
//		result.pop_back();
//		//cout << result.size() << endl;
//	}
//	result.push_back(Funcs::int_expr_map[op](a));
//}


HModel::~HModel() {
	for (auto i : vars)
		delete i;
	vars.clear();

	for (auto i : tabs)
		delete i;
	tabs.clear();
}
}

