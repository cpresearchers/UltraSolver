#pragma once
#include "Var.h"
namespace cudacp
{
	using namespace std;
	class Propagator
	{
	public:
		Propagator(const int id, vector<Var*>& scope) :id(id), arity(scope.size()), scope(scope) {};
		virtual ~Propagator() = default;
		int id;
		int arity;
		int bind_count = 0;
		vector<Var*> scope;
		//vector<Var*> scope;
		int level = 0;
		int num_assigned = 0;
		virtual bool propagate(vector<Var*>& Xevt) = 0;

		virtual void NewLevel() = 0;
		virtual void BackLevel() = 0;
	};
}

