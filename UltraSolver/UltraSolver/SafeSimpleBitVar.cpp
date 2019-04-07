#include "SafeSimpleBitVar.h"
namespace cp {
SafeSimpleBitVar::SafeSimpleBitVar(string& name, const int id, const int num_vars, vector<int>& values,
								   PSearchHelper& helper) :
	PVar(name, id, num_vars, values, helper),
	limit_(capacity_),
	num_level_(capacity_ + 3),
	size_level_(vector<int>(num_level_, -1)),
	bit_doms_(vector<atomic<u64>>(num_level_)) {
	size_level_[0] = capacity_;
	bit_doms_[0] = Constants::kALLONELONG << (Constants::BITSIZE - limit_);
}


inline int SafeSimpleBitVar::NewLevel() {
	const auto prev_level = level_++;
	bit_doms_[level_] = bit_doms_[prev_level].load();
	size_level_[level_] = size_level_[prev_level];
	return level_;
}

inline int SafeSimpleBitVar::BackLevel() {
	if (bind_level_ == level_)
		bind_level_ = Constants::kUNBINDLEVEL;

	size_level_[level_--] = -1;
	return level_;
}

int SafeSimpleBitVar::Size() {
	return BitCount64(bit_doms_[level_].load());
}

void SafeSimpleBitVar::Bind(const int a) {
	bit_doms_[level_].store(Constants::MASK1[a]);
	//curr_size_--;
	bind_level_ = level_;
}

void SafeSimpleBitVar::Remove(const int a) {
	BIT_CLEAR(bit_doms_[level_], a);
	//bit_doms_[level_] &= Constants::MASK0[a];
}

bool SafeSimpleBitVar::IsEmpty() {
	return bit_doms_[level_] == 0;
}

void SafeSimpleBitVar::Restrict() {
	bit_doms_[level_] &= bit_mark_.load();
}

void SafeSimpleBitVar::Mark(const int a) {
	BIT_SET(bit_mark_, a);
}

bool SafeSimpleBitVar::FullMark() {
	return !(bit_doms_[level_] ^ bit_mark_);
}

bool SafeSimpleBitVar::Contains(const int a) {
	return BIT_CHECK(bit_doms_[level_], a);
}

int SafeSimpleBitVar::MinValue() {
	return CLZ64(bit_doms_[level_]);
}

int SafeSimpleBitVar::MaxValue() {
	return CTZ64(bit_doms_[level_].load());
}

int SafeSimpleBitVar::NextValue(const int a) {
	return a + CLZ64((bit_doms_[level_].load() << a) << 1);
}

void SafeSimpleBitVar::GetLastRemoveValues(const u64 last, vector<int> & values) {
	values.clear();
	// 取mask
	const auto b = (last & (~bit_doms_[level_]));
	for (int i = CLZ64(b), end = CTZ64(b); i <= end; ++i) {
		values.push_back(i);
	}
}

void SafeSimpleBitVar::GetValidValues(vector<int> & values) {
	const auto b = ~bit_doms_[level_].load();
	for (int i = CLZ64(b), end = CTZ64(b); i <= end; ++i) {
		values.push_back(i);
	}
}

u64 SafeSimpleBitVar::SimpleMask() {
	return bit_doms_[level_].load();
}

u64 SafeSimpleBitVar::SubmitMask(const u64 mask) {
	return bit_doms_[level_] &= mask;
}

u64 SafeSimpleBitVar::SubmitMaskAndGet(const u64 mask) {
	return bit_doms_[level_] &= mask;
}

u64 SafeSimpleBitVar::GetAndSubmitMask(const u64 mask) {
	return bit_doms_[level_].fetch_and(mask);
}
}

