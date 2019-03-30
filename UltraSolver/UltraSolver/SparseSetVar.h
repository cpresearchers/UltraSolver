#pragma once
#include "Var.h"
#include <string>
#include "SearchHelper.h"

namespace cudacp
{
	using namespace std;
	class SparseSetVar : public Var
	{
	public:
		vector<int> dense;
		vector<int> sparse;
		vector<int> size_level;

		int mark = 0;

		vector<int> last_remove_values;
		vector<int> valid_values;

		SearchHelper* helper{};

		SparseSetVar(string& name, const int id, const int num_vars, vector<int>& vals, SearchHelper& helper);

		int NewLevel() override;
		int BackLevel() override;
		int Size() override;
		void Bind(const int a) override;
		void Remove(const int a) override;
		bool IsEmpty() override;
		void Restrict() override;
		void Mark(const int a) override;
		bool FullMark() override;
		int BindLevel() override;
		bool Contains(const int a) override;
		int MinValue() override;
		int MaxValue() override;
		int NextValue(const int a) override;
		void GetLastRemoveValues(const int last) override;
		void GetValidValues() override;
		void swap(const int i, const int j);
	};
}
