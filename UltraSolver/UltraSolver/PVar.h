#pragma once
#include "PSearchHelper.h"
namespace cp {
using namespace std;
class PVar {
public:
	//PVar(string& name, const int id, const int num_vars, vector<int>& values,
	//	 shared_ptr<PSearchHelper>&& helper) :
	//	helper(helper),
	//	name_(name),
	//	id_(id),
	//	num_vars_(num_vars),
	//	values_(values),
	//	capacity_(values.size()) {};
	//int Id() const { return id_; }
	//int Capacity() const { return capacity_; }
	////vector<int> last_remove_values;
	////vector<int> valid_values;

	//virtual int NewLevel() = 0;
	//virtual int BackLevel() = 0;
	//virtual int Size() = 0;
	//virtual void Bind(const int a) = 0;
	//inline bool IsBind() const { return bind_level_ != Constants::kINTMAX; };
	////inline bool IsBindOrLastPast() const { return bind_level_ <= level_; };
	//inline bool IsLastPast() const { return bind_level_ == level_; };
	//inline bool NeedFilterDomain() const { return level_ <= bind_level_; };
	//virtual void Remove(const int a) = 0;
	//virtual bool IsEmpty() = 0;
	//virtual void Restrict() = 0;
	//virtual void Mark(const int a) = 0;
	//virtual bool FullMark() = 0;
	//int BindLevel() const { return bind_level_; }
	//virtual bool Contains(const int a) = 0;
	//virtual int MinValue() = 0;
	//virtual int MaxValue() = 0;
	//virtual int NextValue(const int a) = 0;
	////vector<int>& values
	////virtual void GetLastRemoveValues(const int last) = 0;
	////virtual void GetValidValues() = 0;
	//virtual void GetLastRemoveValues(const u64 last, vector<int>& values) = 0;
	//virtual void GetValidValues(vector<int>& values) = 0;
	//virtual u64 SimpleMask() = 0;
	//virtual u64 SubmitMask(const u64 mask) = 0;
	//virtual u64 SubmitMaskAndGet(const u64 mask) = 0;
	//virtual u64 GetAndSubmitMask(const u64 mask) = 0;
	//const shared_ptr<PSearchHelper> helper;

protected:
	/*string name_;
	int id_;
	int num_vars_;
	vector<int> values_;
	int level_ = 0;
	int capacity_ = 0;
	int bind_level_ = Constants::kINTMAX;*/
};
}
