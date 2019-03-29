#pragma once
#include <vector>
#include "typedef.h"
#include <string>

using namespace std;
namespace cudacp
{
	class SearchHelper;

	class Var
	{
	public:
		virtual ~Var() = default;

		Var(string& name, const int id, const int num_vars, vector<int>& vals,
			SearchHelper& helper) :name_(name), id_(id), num_vars_(num_vars), vals_(vals), helper(&helper), capacity_(vals.size()) {};
		int id() const { return id_; }
		int capacity() const { return capacity_; }
		vector<int> last_remove_values;
		vector<int> valid_values;

		virtual int NewLevel() = 0;
		virtual int BackLevel() = 0;
		virtual int Size() = 0;
		virtual void Bind(const int a) = 0;
		bool IsBind() const { return bind_level != Constants::kINTMAX; };
		bool IsBindOrLastPast() const { return bind_level < Constants::kINTMAX; };
		virtual void Remove(const int a) = 0;
		virtual bool IsEmpty() = 0;
		virtual void Restrict() = 0;
		virtual void Mark(const int a) = 0;
		virtual bool FullMark() = 0;
		virtual int BindLevel() = 0;
		virtual bool Contains(const int a) = 0;
		virtual int MinValue() = 0;
		virtual int MaxValue() = 0;
		virtual int NextValue(const int a) = 0;
		virtual void GetLastRemoveValues(const int last) = 0;
		virtual void GetValidValues() = 0;
		SearchHelper* helper;
	protected:
		string name_;
		int id_;
		int num_vars_;
		vector<int> vals_;
		int level_ = 0;
		int capacity_ = 0;
		int bind_level = Constants::kINTMAX;

	};




	class PVar :virtual Var
	{
	public:
		u64 SimpleMask();
		bool SubmitMask();
		u64 SubmitMaskAndGet();
		u64 GetAndSubmitMask();
	};

}