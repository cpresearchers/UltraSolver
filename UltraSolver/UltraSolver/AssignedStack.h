#pragma once
#include "Var.h"
namespace cp {
using namespace std;

class Val {
public:
	Var* v;
	int a;
};

//class PVal {
//public:
//	PVar* v;
//	int a;
//};

inline ostream& operator<<(ostream& os, Val& va) {
	os << va.v->Id() << ", " << va.a << " ";
	return os;
}

//inline ostream& operator<<(ostream& os, PVal& va) {
//	os << va.v->Id() << ", " << va.a << " ";
//	return os;
//}

template<class T>
class AssignedStack {
public:
	void Initial(const int num_vars) {
		num_vars_ = num_vars;
		table_.resize(num_vars, { nullptr,-1 });
		in_stack_.resize(num_vars, -1);
	}

	void push(const T va) {
		//++index_;
		table_[++index_] = va;
		in_stack_[va.v->Id()] = va.a;
	}

	T pop() {
		if (index_ == -1) {
			cout << "stack empty" << endl;
			return T{ nullptr,-1 };
		}

		T x = table_[index_--];
		////table_[index_] = nullptr;
		//--index_;
		in_stack_[x.v->Id()] = -1;
		return x;
	}

	inline bool full() const {
		return (index_ + 1) == num_vars_;
	}

	inline bool empty() const {
		return index_ == -1;
	}

	void show() {
		for (auto s : in_stack_) {
			cout << s << " ";
		}
		cout << endl;
	}

	inline int GetValue(const int i) const {
		return in_stack_[i];
	}

	inline int Size() const {
		return index_ + 1;
	}

protected:
	vector<T> table_;
	vector<int> in_stack_;
	int index_ = -1;
	int num_vars_ = 0;
};

template<class T>
ostream& operator<<(ostream & os, AssignedStack<T> & I) {
	for (int i = 0; i < I.Size(); ++i)
		os << I.GetValue(i) << " ";
	return os;
}
}
