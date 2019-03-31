#pragma once
#include "Var.h"

namespace cp
{
	class Val
	{
	public:
		Var* v;
		int a;
	};

	class PVal
	{
	public:
		PVar* v;
		int a;
	};

	ostream& operator<<(ostream& os, Val v_a) {
		os << "(" << v_a.v->id() << ", " << v_a.a << ") ";
		return os;
	}

	//ostream& operator<<(ostream& os, PVal v_a) {
	//	os << "(" << v_a.v->id() << ", " << v_a.a << ") ";
	//	return os;
	//}

}
