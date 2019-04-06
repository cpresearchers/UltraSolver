#pragma once
#include "PSolver.h"

namespace cp {
using namespace std;
class PCoarseSolver final :
	public PSolver {
public:
	PCoarseSolver(HModel& hm, string& propagator_type, string& var_type, string& heu_type, const int parallelism);

	~PCoarseSolver() {}

	bool InitialPropagate() override;

	bool CheckConsistencyAfterAssignment(vector<Var*>& x_evt) override;

	bool CheckConsistencyAfterRefutation(vector<Var*>& x_evt) override;

	bool propagate(vector<Var*>& x_evt);

	//void insert(Var* x);
protected:

	//unique_ptr<CoarseQueue<Var*>> Q_;
	vector<Var*> Y_evt_;

};
}

