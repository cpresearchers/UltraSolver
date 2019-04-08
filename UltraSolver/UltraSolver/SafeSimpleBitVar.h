//#pragma once
//#include "PSearchHelper.h"
//
//namespace cp {
//using namespace std;
//class SafeSimpleBitVar :public PVar {
//public:
//	SafeSimpleBitVar(string& name, const int id, const int num_vars, vector<int>& values, shared_ptr<PSearchHelper>&& helper);
//	int NewLevel() override;
//	int BackLevel() override;
//	int Size() override;
//	void Bind(const int a) override;
//	void Remove(const int a) override;
//	bool IsEmpty() override;
//	void Restrict() override;
//	void Mark(const int a) override;
//	bool FullMark() override;
//	bool Contains(const int a) override;
//	int MinValue() override;
//	int MaxValue() override;
//	int NextValue(const int a) override;
//	void GetLastRemoveValues(const u64 last, vector<int>& values) override;
//	void GetValidValues(vector<int>& values) override;
//	u64 SimpleMask() override;
//	u64 SubmitMask(const u64 mask) override;
//	u64 SubmitMaskAndGet(const u64 mask) override;
//	u64 GetAndSubmitMask(const u64 mask) override;
//protected:
//	int limit_;
//	int num_level_;
//	vector<int> size_level_;
//	atomic<u64> bit_mark_;
//	vector<atomic<u64>> bit_doms_;
//};
//}
