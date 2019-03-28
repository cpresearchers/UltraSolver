#pragma once
#include <vector>
#include "typedef.h"

using namespace std;
namespace cudacp
{
	class Var
	{
		int id() const { return id_; }
		vector<int> last_remove_values;
		vector<int> valid_values;

		virtual int NewLevel() = 0;
		virtual int BackLevel() = 0;

		virtual int Size() = 0;
		virtual void Bind(const int a) = 0;
		virtual void IsBind() = 0;
		virtual void Remove(const int a) = 0;
		virtual bool IsEmpty() = 0;
		virtual bool Restrict() = 0;
		virtual void Mark() = 0;
		virtual bool FullMark() = 0;
		virtual int BindLevel() = 0;
		virtual bool Contains(const int a) = 0;
		virtual int MinValue() = 0;
		virtual int MaxValue() = 0;
		virtual int NextValue() = 0;
		virtual void GetLastRemoveValues(const int last);
		virtual void GetValidValues();
	private:
		int level_ = 0;
		int id_;
		int capacity = 0;
	};
}