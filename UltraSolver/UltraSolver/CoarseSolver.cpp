#include "CoarseSolver.h"


namespace cp {

	CoarseSolver::CoarseSolver(HModel& hm, string& propagator_type, string& var_type, string& heu_type):
		Solver(hm, propagator_type, var_type, heu_type)
	{
		Q_.reset(new CoarseQueue<Var*>(num_vars));
		Y_evt_.reserve(m->max_arity());

		vars.reserve(num_vars);
		tabs.reserve(num_tabs);
		if (var_type_ == "SparseSet")
		{
			for (size_t i = 0; i < num_vars; i++)
			{
				auto xv = m->vars[i];
				vars.push_back(new SparseSetVar(xv->name, xv->id, num_vars, xv->vals, helper));
			}
		}

		if (propagator_type_ == "CT")
		{
			for (size_t i = 0; i < num_tabs; ++i)
			{
				auto xc = m->tabs[i];
				vector<Var*> scope(xc->scope.size());

				for (size_t i = 0; i < scope.size(); i++)
				{
					scope[i] = vars[xc->scope[i]->id];
				}

				tabs.emplace_back(new TableCT(xc->id, xc->scope.size(), num_vars, scope, xc->tuples, helper));

				for (auto v : scope)
				{
					helper.subscription[v->Id()].push_back(tabs[i]);
				}
			}
		}
	}

	CoarseSolver::~CoarseSolver()
	{
	}

	bool CoarseSolver::InitialPropagate()
	{
		Y_evt_.clear();
		// 随便传一个为空的集合
		for (auto x : vars)
		{
			Y_evt_.push_back(x);
		}
		return propagate(Y_evt_);
	}

	bool CoarseSolver::CheckConsistencyAfterAssignment(vector<Var*>& x_evt)
	{
		return propagate(x_evt);
	}

	bool CoarseSolver::CheckConsistencyAfterRefutation(vector<Var*>& x_evt)
	{
		return propagate(x_evt);
	}

	bool CoarseSolver::propagate(vector<Var*>& x_evt)
	{
		Q_->clear();
		for (auto x : x_evt)
		{
			insert(x);
		}

		while (!Q_->IsEmpty())
		{
			const auto v = Q_->pop();
			for (auto c : helper.subscription[v->Id()])
			{
				if (helper.var_stamp[v->Id()] > helper.tab_stamp[c->Id()])
				{
					Y_evt_.clear();
					const auto consistent = c->propagate(Y_evt_);
					if (!consistent)
					{
						return false;
					}
					else
					{
						for (auto y : Y_evt_)
						{
							insert(y);
						}
					}
					helper.global_stamp++;
					helper.tab_stamp[c->Id()] = helper.global_stamp;
				}
			}
		}
		return true;
	}

	void CoarseSolver::insert(Var* x)
	{
		Q_->push(x);
		++helper.global_stamp;
		helper.var_stamp[x->Id()] = helper.global_stamp;
	}
}
