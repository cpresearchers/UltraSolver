#pragma once
#include "Val.h"
#include <iostream>
using namespace std;
template<typename T>
class AssignedStack
{
public:
	void push(const T va)
	{
		++index_;
		table_[index_] = va;
		in_stack_(va.v->id) = va.a;
	}

	T pop()
	{
		if (index_ == -1)
		{
			cout << "stack empty" << endl;
			return T{ nullptr,-1 };
		}

		T x = table_[index_];
		table_[index_] = nullptr;
		--index_;
		--in_stack_[x.v->id];
		return x;
	}

	bool full() const
	{
		return (index_ + 1) == num_vars;
	}

	bool empty() const
	{
		return index_ == -1;
	}

	void show()
	{
		for (auto s : in_stack_)
		{
			cout << s << " ";
		}
		cout << endl;
	}

protected:
	vector<T> table_;
	vector<int> in_stack_;
	int index_ = -1;
	int num_vars = 0;
};
