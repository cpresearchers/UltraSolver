#pragma once
#include "PSolver.h"

namespace cp {
class PCoarseSolver :public PSolver {
public:
	PCoarseSolver(HModel& m, string& propagator_type, string& var_type, string& heu_type, const int parallelism);
	bool InitialPropagate() override;
	bool CheckConsistencyAfterAssignment(vector<PVar*>& x_evt) override;
	bool CheckConsistencyAfterRefutation(vector<PVar*>& x_evt) override;
};
}

