#include "SimpleBitVar.h"

namespace cp {
SimpleBitVar::SimpleBitVar(string& name, const int id, const int num_vars, vector<int>& values,
						   shared_ptr<SearchHelper>&& helper) :
	Var(name, id, num_vars, values, std::move(helper)),
	limit_(capacity_),
	num_level_(capacity_ + 3),
	size_level_(vector<int>(num_level_, -1)),
	bit_doms_(vector<atomic<u64>>(num_level_)) {
	size_level_[0] = capacity_;
	bit_doms_[0] = Constants::kALLONELONG << (Constants::BITSIZE - limit_);
}

inline int SimpleBitVar::NewLevel() {
	const auto prev_level = level_++;
	bit_doms_[level_] = bit_doms_[prev_level].load();
	size_level_[level_] = size_level_[prev_level];
	return level_;
}

inline int SimpleBitVar::BackLevel() {
	if (bind_level_ == level_)
		bind_level_ = Constants::kUNBINDLEVEL;

	size_level_[level_--] = -1;
	return level_;
}

int SimpleBitVar::Size() {
	return BitCount64(bit_doms_[level_].load());
}

void SimpleBitVar::Bind(const int a) {
	bit_doms_[level_].store(Constants::MASK1[a]);
	//curr_size_--;
	bind_level_ = level_;
}

void SimpleBitVar::Remove(const int a) {
	//BIT_CLEAR(bit_doms_[level_], a);
	bit_doms_[level_] &= Constants::MASK0[a];
}

bool SimpleBitVar::IsEmpty() {
	return bit_doms_[level_] == 0;
}

void SimpleBitVar::Restrict() {
	bit_doms_[level_] &= bit_mark_.load();
}

void SimpleBitVar::Mark(const int a) {
	bit_mark_ |= Constants::MASK1[a];
	//BIT_SET(bit_mark_, a);
}

bool SimpleBitVar::FullMark() {
	return !(bit_doms_[level_].load() ^ bit_mark_.load());
}

bool SimpleBitVar::Contains(const int a) {
	return !(!(bit_doms_[level_].load() & Constants::MASK1[a]));
	//return BIT_CHECK(bit_doms_[level_], a);
}

int SimpleBitVar::MinValue() {
	return CLZ64(bit_doms_[level_].load());
}

int SimpleBitVar::MaxValue() {
	return CTZ64(bit_doms_[level_].load());
}

int SimpleBitVar::NextValue(const int a) {
	return a + CLZ64((bit_doms_[level_].load() << a) << 1);
}

void SimpleBitVar::GetLastRemoveValues(const u64 last, vector<int> & values) {
	values.clear();
	// 取mask
	const auto b = (last & (~bit_doms_[level_]));
	for (int i = CLZ64(b), end = CTZ64(b); i <= end; ++i) {
		//if (BIT_CHECK(b, i)) {
		if (!!(b & Constants::MASK1[i])) {
			values.push_back(i);
		}
	}
}

void SimpleBitVar::GetValidValues(vector<int> & values) {
	values.clear();
	const auto b = bit_doms_[level_].load();
	for (int i = CLZ64(b), end = CTZ64(b); i <= end; ++i) {
		if (Contains(i)) {
			values.push_back(i);
		}
	}
}

u64 SimpleBitVar::SimpleMask() {
	return bit_doms_[level_].load();
}

u64 SimpleBitVar::SubmitMask(const u64 mask) {
	return bit_doms_[level_] &= mask;
}

u64 SimpleBitVar::SubmitMaskAndGet(const u64 mask) {
	return bit_doms_[level_] &= mask;
}

u64 SimpleBitVar::GetAndSubmitMask(const u64 mask) {
	return bit_doms_[level_].fetch_and(mask);
}
}

