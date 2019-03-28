#pragma once
#include "Var.h"
namespace cudacp
{
	using namespace std;
	class Propagator
	{
	public:
		virtual ~Propagator() = default;
		int id;
		int arity;
		//vector<Var*> scope;
		int level = 0;
		int num_assigned = 0;
		//virtual bool propagate(vector<Var*>& Xevt) = 0;

		virtual void NewLevel();
		virtual void BackLevel();
	};
}

