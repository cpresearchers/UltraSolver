#include "SafeSimpleBitVar.h"
namespace cp {
SafeSimpleBitVar::SafeSimpleBitVar(string& name, const int id, const int num_vars, vector<int>& values,
								   SearchHelper& helper) :
	Var(name, id, num_vars, values, helper),
	limit(capacity_),
	num_level(capacity_ + 3),
	size_level(vector<int>(num_level, -1)),
	bit_doms(vector<atomic<u64>>(num_level)) {
	size_level[0] = capacity_;
	bit_doms[0] = Constants::kALLONELONG << (Constants::BITSIZE - limit);
}

inline int SafeSimpleBitVar::NewLevel() {
	const auto pre_level = level_++;
	bit_doms[level_] = bit_doms[pre_level].load();
	size_level[pre_level] = cur_size.load();
	//level_size(pre_level) = cur_size.get()
	size_level[level_]--;
	return level_;
}

inline int SafeSimpleBitVar::BackLevel() {
	if (bind_level_ == level_)
		bind_level_ = Constants::kUNBINDLEVEL;

	size_level[level_--] = -1;
	// 当前层size置0，这一层不要了
	// 回到上一层，当前size等于当前层的上一层的size
	// 当前size = 当前层的上一层
	// !!cur_size应该全部去掉
	//cur_size.store(size_level[level_]);
	return level_;
}

int SafeSimpleBitVar::Size() {
	return BitCount64(bit_doms[level_].load());
}

void SafeSimpleBitVar::Bind(const int a) {
	bit_doms[level_].store(Constants::MASK1[a]);
	//cur_size--;
	bind_level_ = level_;
}

void SafeSimpleBitVar::Remove(const int a) {
	BIT_CLEAR(bit_doms[level_], a);
	//bit_doms[level_] &= Constants::MASK0[a];
}

bool SafeSimpleBitVar::IsEmpty() {
	return bit_doms[level_] == 0;
}

void SafeSimpleBitVar::Restrict() {
	bit_doms[level_] &= bit_mark.load();
}

void SafeSimpleBitVar::Mark(const int a) {
	BIT_SET(bit_mark, a);
}

bool SafeSimpleBitVar::FullMark() {
	return !(bit_doms[level_] ^ bit_mark);
}

bool SafeSimpleBitVar::Contains(const int a) {
	return BIT_CHECK(bit_doms[level_], a);
}

int SafeSimpleBitVar::MinValue() {
	return CLZ64(bit_doms[level_]);
}

int SafeSimpleBitVar::MaxValue() {
	return CTZ64(bit_doms[level_].load());
}

int SafeSimpleBitVar::NextValue(const int a) {
	return a + CLZ64((bit_doms[level_].load() << a) << 1);
}

void SafeSimpleBitVar::GetLastRemoveValues(const u64 last, const u64 mask, vector<int> & values) {
	values.clear();
	// 取mask
	const auto b = (mask & (~bit_doms[level_]));
	for (int i = CLZ64(b), end = CTZ64(b); i <= end; ++i) {
		values.push_back(i);
	}
}

void SafeSimpleBitVar::GetValidValues(vector<int> & values) {
	const auto b = ~bit_doms[level_].load();
	for (int i = CLZ64(b), end = CTZ64(b); i <= end; ++i) {
		values.push_back(i);
	}
}

u64 SafeSimpleBitVar::SimpleMask() {
	return bit_doms[level_].load();
}
}

