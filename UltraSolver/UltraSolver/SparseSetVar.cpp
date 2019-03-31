#include "SparseSetVar.h"
namespace cp {
SparseSetVar::SparseSetVar(string& name, const int id, const int num_vars, vector<int>& vals,
						   SearchHelper& helper) :
	Var(name, id, num_vars, vals, helper), mark(0) {
	dense = vals;
	sparse = vals;
	size_level.resize(num_vars_, -1);
	size_level[0] = vals.size();
	last_remove_values.reserve(capacity_);
	valid_values.reserve(capacity_);
}

inline int SparseSetVar::NewLevel() {
	size_level[level_] = size_level[level_++];
	return level_;
}

inline int SparseSetVar::BackLevel() {
	if (bind_level == level_)
		bind_level = Constants::kUNBINDLEVEL;

	size_level[level_--] = -1;
	return level_;
}

inline int SparseSetVar::Size() {
	return size_level[level_];
}

void SparseSetVar::Bind(const int a) {
	bind_level = level_;
	if (sparse[a] >= size_level[level_]) {
		size_level[level_] = 0;
		cout << "value already removed" << endl;
	}
	else {
		swap(sparse[a], 0);
		size_level[level_] = 1;
	}
}

inline void SparseSetVar::swap(const int i, const int j) {
	//const auto tmp = dense[i];
	//dense[i] = dense[j];
	//dense[j] = tmp;
	dense[i] = dense[i] ^ dense[j];
	dense[j] = dense[i] ^ dense[j];
	dense[i] = dense[i] ^ dense[j];
	sparse[dense[i]] = i;
	sparse[dense[j]] = j;
}

void SparseSetVar::Remove(const int a) {
	if (sparse[a] < size_level[level_]) {
		swap(sparse[a], size_level[level_] - 1);
		size_level[level_] --;
	}
}

bool SparseSetVar::IsEmpty() {
	return !size_level[level_];
}

void SparseSetVar::Restrict() {
	size_level[level_] = mark;
}

void SparseSetVar::Mark(const int a) {
	if (sparse[a] < size_level[level_] && sparse[a] >= mark) {
		swap(sparse[a], mark);
		mark++;
	}
}

bool SparseSetVar::FullMark() {
	return mark == size_level[level_];
}

int SparseSetVar::BindLevel() {
	return bind_level;
}

bool SparseSetVar::Contains(const int a) {
	return sparse[a] < size_level[level_];
}

int SparseSetVar::MinValue() {
	for (int a = 0; a < capacity_; ++a) {
		if (Contains(a)) {
			return a;
		}
	}

	return Constants::INDEXOVERFLOW;
}

int SparseSetVar::MaxValue() {

	for (int a = capacity_; a >= 0; --a) {
		if (Contains(a)) {
			return a;
		}
	}

	return Constants::INDEXOVERFLOW;
}

int SparseSetVar::NextValue(const int a) {
	for (int b = a + 1; b < capacity_; ++b) {
		if (Contains(b)) {
			return b;
		}
	}

	return Constants::INDEXOVERFLOW;
}

void SparseSetVar::GetLastRemoveValues(const int last) {
	last_remove_values.clear();
	auto a = Size();
	if (last <= a)
		return;


	while (a < last) {
		last_remove_values.push_back(dense[a]);
		++a;
	}

	sort(last_remove_values.begin(), last_remove_values.end());
}

void SparseSetVar::GetValidValues() {
	valid_values.clear();
	for (size_t a = 0; a < Size(); a++) {
		valid_values.push_back(dense[a]);
	}

	sort(valid_values.begin(), valid_values.end());
}
}
