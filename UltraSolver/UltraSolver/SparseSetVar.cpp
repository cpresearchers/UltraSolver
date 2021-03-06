﻿#include "Var.h"
#include <iostream>
#include <algorithm>

namespace cp {
using namespace std;
//SparseSetVar::SparseSetVar(string& name, const int id, const int num_vars, vector<int>& vals,
//						   SearchHelper& helper) :
//	Var(name, id, num_vars, vals, helper), mark(0) {
//	dense = vals;
//	sparse = vals;
//	size_level.resize(num_vars_ + 1, -1);
//	size_level[0] = vals.size();
//}

SparseSetVar::SparseSetVar(string& name, const int id, const int num_vars, vector<int>& vals,
						   shared_ptr<SearchHelper>&& helper) :
	Var(name, id, num_vars, vals, std::move(helper)), mark(0) {
	dense = vals;
	sparse = vals;
	size_level.resize(num_vars_ + 1, -1);
	size_level[0] = vals.size();
}

inline int SparseSetVar::NewLevel() {
	//level_++;
	size_level[level_] = size_level[level_++];
	return level_;
}

inline int SparseSetVar::BackLevel() {
	if (bind_level_ == level_)
		bind_level_ = Constants::kUNBINDLEVEL;

	size_level[level_--] = -1;
	//level_--;
	return level_;
}

inline int SparseSetVar::Size() {
	return size_level[level_];
}

void SparseSetVar::Bind(const int a) {
	bind_level_ = level_;
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
	//std::swap(dense[i], dense[j]);
	const auto tmp = dense[i];
	dense[i] = dense[j];
	dense[j] = tmp;

	sparse[dense[i]] = i;
	sparse[dense[j]] = j;
}

u64 SparseSetVar::SimpleMask() {
	return 0;
}

u64 SparseSetVar::SubmitMask(const u64 mask) {
	return 0;
}

u64 SparseSetVar::SubmitMaskAndGet(const u64 mask) {
	return 0;
}

u64 SparseSetVar::GetAndSubmitMask(const u64 mask) {
	return 0;
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

void SparseSetVar::GetLastRemoveValues(const u64 last, vector<int> & values) {
	values.clear();
	if (last <= Size())
		return;

	for (auto a = Size(); a < last; a++) {
		values.push_back(dense[a]);
	}
	sort(values.begin(), values.end());
}

void SparseSetVar::GetValidValues(vector<int> & values) {
	values.clear();
	for (auto a = 0; a < Size(); a++) {
		values.push_back(dense[a]);
	}

	sort(values.begin(), values.end());
}
}
