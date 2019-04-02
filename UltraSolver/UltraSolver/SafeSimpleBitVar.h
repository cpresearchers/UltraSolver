#pragma once
#include "Var.h"
#include <atomic>

namespace cp {

/**
 * \brief 用于论域小于64的变量
 */
class SafeSimpleBitVar :public PVar {
public:
	SafeSimpleBitVar(string& name, const int id, const int num_vars, vector<int>& values, SearchHelper& helper);
	int NewLevel() override;
	int BackLevel() override;
	int Size() override;
	void Bind(const int a) override;
	void Remove(const int a) override;
	bool IsEmpty() override;
	void Restrict() override;
	void Mark(const int a) override;
	bool FullMark() override;
	bool Contains(const int a) override;
	int MinValue() override;
	int MaxValue() override;
	int NextValue(const int a) override;
	void GetLastRemoveValues(const u64 last, const u64 mask, vector<int>& values) override;
	void GetValidValues(vector<int>& values) override;
	u64 SimpleMask() override;
	bool SubmitMask() override;
	u64 SubmitMaskAndGet() override;
	u64 GetAndSubmitMask() override;
	~SafeSimpleBitVar() override;
	//SafeSimpleBitVar(string& name, const int id, const int num_vars, vector<int>& values, SearchHelper& helper);
	//~SafeSimpleBitVar() = default;;
	//int NewLevel() override;
	//int BackLevel() override;
	//int Size() override;
	//void Bind(const int a) override;
	//void Remove(const int a) override;
	//bool IsEmpty() override;
	//void Restrict() override;
	//void Mark(const int a) override;
	//bool FullMark() override;
	//bool Contains(const int a) override;
	//int MinValue() override;
	//int MaxValue() override;
	//int NextValue(const int a) override;
	//void GetLastRemoveValues(const u64 last, const u64 mask, vector<int>& values) override;
	//void GetValidValues(vector<int>& values) override;

protected:
	int limit{};
	int num_level{};
	vector<int> size_level;
	atomic<u64> bit_mark;
	vector<atomic<u64>> bit_doms;
	atomic<int> cur_size;
};



}
