#include "CoarseSolver.h"


namespace cudacp {
	CoarseSolver::CoarseSolver(HModel& m, string& propagator_type, string& var_type, string& heu_type) :
		Solver(m, propagator_type, var_type, heu_type) {

		if (propagator_type_ == "CT") {}
	}


	CoarseSolver::~CoarseSolver()
	{
	}

}
