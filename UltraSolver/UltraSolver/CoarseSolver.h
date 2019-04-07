#pragma once
#include "Solver.h"
#include <memory>
#include "CoarseQueue.h"
#include "RSBitSet.h"
#include "Var.h"
#include "TableCT.h"

namespace cp {
class CoarseSolver final :
	public Solver {
public:
	CoarseSolver(HModel& hm, string& propagator_type, string& var_type, string& heu_type);

	~CoarseSolver(){}

	bool InitialPropagate() override;

	bool CheckConsistencyAfterAssignment(vector<Var*>& x_evt) override;

	bool CheckConsistencyAfterRefutation(vector<Var*>& x_evt) override;

	bool propagate(vector<Var*>& x_evt);

	void insert(Var* x);


protected:

	unique_ptr<CoarseQueue<Var*>> Q_;
	//CoarseQueue<Var*> q;
	vector<Var*> Y_evt_;
};
}


