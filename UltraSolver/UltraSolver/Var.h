#pragma once
#include <utility>
#include <vector>
#include "typedef.h"
#include <string>
#include <memory>
#include <iostream>
using namespace std;
namespace cp {
class SearchHelper;

class Var {
public:
	virtual ~Var() {};
	Var(string& name, const int id, const int num_vars, vector<int>& vals,
		shared_ptr<SearchHelper>&& helper) :
		helper(helper),
		name_(name),
		id_(id),
		num_vars_(num_vars),
		vals_(vals),
		capacity_(vals.size()) {}

	int Id() const { return id_; }
	int Capacity() const { return capacity_; }

	virtual int NewLevel() = 0;
	virtual int BackLevel() = 0;
	virtual int Size() = 0;
	virtual void Bind(const int a) = 0;
	inline bool IsBind() const { return bind_level_ != Constants::kINTMAX; };
	//inline bool IsBindOrLastPast() const { return bind_level_ <= level_; };
	inline bool IsLastPast() const { return bind_level_ == level_; };
	inline bool NeedFilterDomain() const { return level_ <= bind_level_; };
	virtual void Remove(const int a) = 0;
	virtual bool IsEmpty() = 0;
	virtual void Restrict() = 0;
	virtual void Mark(const int a) = 0;
	virtual bool FullMark() = 0;
	int BindLevel() const { return bind_level_; }
	virtual bool Contains(const int a) = 0;
	virtual int MinValue() = 0;
	virtual int MaxValue() = 0;
	virtual int NextValue(const int a) = 0;
	//vector<int>& values
	//virtual void GetLastRemoveValues(const int last) = 0;
	//virtual void GetValidValues() = 0;
	virtual void GetLastRemoveValues(const u64 last, vector<int>& values) = 0;
	virtual void GetValidValues(vector<int>& values) = 0;
	virtual u64 SimpleMask() = 0;
	virtual u64 SubmitMask(const u64 mask) = 0;
	virtual u64 SubmitMaskAndGet(const u64 mask) = 0;
	virtual u64 GetAndSubmitMask(const u64 mask) = 0;
	//SearchHelper* helper;
	const shared_ptr<SearchHelper> helper;
protected:
	string name_;
	int id_;
	int num_vars_;
	vector<int> vals_;
	int level_ = 0;
	int capacity_ = 0;
	int bind_level_ = Constants::kINTMAX;

};
//
//class PVar {
//public:
//	PVar(string& name, const int id, const int num_vars, vector<int>& vals,
//		 SearchHelper& helper) :helper(&helper), name_(name), id_(id), num_vars_(num_vars), vals_(vals), capacity_(vals.size()) {};
//	int Id() const { return id_; }
//	int Capacity() const { return capacity_; }
//	//vector<int> last_remove_values;
//	//vector<int> valid_values;
//
//	virtual int NewLevel() = 0;
//	virtual int BackLevel() = 0;
//	virtual int Size() = 0;
//	virtual void Bind(const int a) = 0;
//	inline bool IsBind() const { return bind_level_ != Constants::kINTMAX; };
//	//inline bool IsBindOrLastPast() const { return bind_level_ <= level_; };
//	inline bool IsLastPast() const { return bind_level_ == level_; };
//	inline bool NeedFilterDomain() const { return level_ <= bind_level_; };
//	virtual void Remove(const int a) = 0;
//	virtual bool IsEmpty() = 0;
//	virtual void Restrict() = 0;
//	virtual void Mark(const int a) = 0;
//	virtual bool FullMark() = 0;
//	int BindLevel() const { return bind_level_; }
//	virtual bool Contains(const int a) = 0;
//	virtual int MinValue() = 0;
//	virtual int MaxValue() = 0;
//	virtual int NextValue(const int a) = 0;
//	//vector<int>& values
//	//virtual void GetLastRemoveValues(const int last) = 0;
//	//virtual void GetValidValues() = 0;
//	virtual void GetLastRemoveValues(const u64 last, vector<int>& values) = 0;
//	virtual void GetValidValues(vector<int>& values) = 0;
//	virtual u64 SimpleMask() = 0;
//	virtual u64 SubmitMask(const u64 mask) = 0;
//	virtual u64 SubmitMaskAndGet(const u64 mask) = 0;
//	virtual u64 GetAndSubmitMask(const u64 mask) = 0;
//	SearchHelper* helper;
//	//PVar(string& name, const int id, const int num_vars, vector<int>& vals,
//	//	 SearchHelper& helper) :Var(name, id, num_vars, vals, helper) {};
//	//virtual u64 SimpleMask() = 0;
//	//virtual u64 SubmitMask(const u64 mask) = 0;
//	//virtual u64 SubmitMaskAndGet(const u64 mask) = 0;
//	//virtual u64 GetAndSubmitMask(const u64 mask) = 0;
//	//protected:
//	//	string name_;
//	//	int level_ = 0;
//	//	int capacity_ = 0;
//	//	int bind_level_ = Constants::kINTMAX;
//
//protected:
//	string name_;
//	int id_;
//	int num_vars_;
//	vector<int> vals_;
//	int level_ = 0;
//	int capacity_ = 0;
//	int bind_level_ = Constants::kINTMAX;
//};

class SparseSetVar : public Var {
public:
	vector<int> dense;
	vector<int> sparse;
	vector<int> size_level;
	int mark = 0;

	SparseSetVar(string& name, const int id, const int num_vars, vector<int>& vals, shared_ptr<SearchHelper>&& helper);

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
	void GetLastRemoveValues(const u64 last, vector<int>& values) override;
	void GetValidValues(vector<int>& values) override;
	inline void swap(const int i, const int j);
	u64 SimpleMask() override;
	u64 SubmitMask(const u64 mask) override;
	u64 SubmitMaskAndGet(const u64 mask) override;
	u64 GetAndSubmitMask(const u64 mask) override;
};


///**
// * \brief 用于论域小于64的变量
// */
//class SimpleBitVar :public Var {
//public:
//	SimpleBitVar(string& name, const int id, const int num_vars, vector<int>& vals, SearchHelper& helper);
//	~SimpleBitVar() override;
//	int NewLevel() override;
//	int BackLevel() override;
//	int Size() override;
//	void Bind(const int a) override;
//	void Remove(const int a) override;
//	bool IsEmpty() override;
//	void Restrict() override;
//	void Mark(const int a) override;
//	bool FullMark() override;
//	int BindLevel() override;
//	bool Contains(const int a) override;
//	int MinValue() override;
//	int MaxValue() override;
//	int NextValue(const int a) override;
//	void GetLastRemoveValues(const int last, vector<int>& values) override;
//	void GetValidValues(vector<int>& values) override;
//};
}