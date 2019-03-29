#pragma once
#include "Solver.h"
namespace cudacp {
	class CoarseSolver :
		public Solver
	{
	public:
		CoarseSolver(HModel& m, string& propagator_type, string& var_type, string& heu_type);
		~CoarseSolver();

	protected:
		vector<int> level_V_dense_;
		vector<int> level_V_sparse_;

	};
}


