#pragma once
#include "Solver.h"
#include <memory>
#include "CoarseQueue.h"
#include "RSBitSet.h"
#include "SparseSetVar.h"
#include "TableCT.h"

namespace cudacp {
	class CoarseSolver :
		public Solver
	{
	public:
		CoarseSolver(HModel& hm, string& propagator_type, string& var_type, string& heu_type) :
			Solver(hm, propagator_type, var_type, heu_type) {
			Q_.reset(new CoarseQueue<Var*>(num_vars));
			Y_evt_.reserve(m->max_arity());
			level_V_dense_.resize(num_vars, 0);
			level_V_sparse_.resize(num_vars, 0);

			vars.reserve(num_vars);
			tabs.reserve(num_tabs);
			if (var_type_ == "SparseSet") {
				for (size_t i = 0; i < num_vars; i++)
				{
					auto xv = m->vars[i];
					vars.push_back(new SparseSetVar(xv->name, xv->id, num_vars, xv->vals, helper));
				}
			}

			if (propagator_type_ == "CT") {
				for (size_t i = 0; i < num_tabs; ++i)
				{
					auto xc = m->tabs[i];
					vector<Var*>scope(xc->scope.size());

					for (size_t i = 0; i < scope.size(); i++)
					{
						scope[i] = vars[xc->scope[i]->id];
					}

					tabs.emplace_back(new TableCT(xc->id, xc->scope.size(), num_vars, scope, xc->tuples, helper));

					for (auto v : scope) {
						helper.subscription[v->id()].push_back(tabs[i]);
					}
				}
			}
		};

		~CoarseSolver() {};

		bool InitialPropagate() override {
			Y_evt_.clear();
			// 随便传一个为空的集合
			for (auto x : vars) {
				Y_evt_.push_back(x);
			}
			return propagate(Y_evt_);
		}

		bool CheckConsistencyAfterAssignment(vector<Var*>& x_evt) override {
			return propagate(x_evt);
		}

		bool CheckConsistencyAfterRefutation(vector<Var*>& x_evt) override {
			return propagate(x_evt);
		}

		bool propagate(vector<Var*>& x_evt) {
			Q_->clear();
			for (auto x : x_evt) {
				insert(x);
			}

			while (!Q_->IsEmpty())
			{
				auto v = Q_->pop();
				for (auto c : helper.subscription[v->id()]) {
					if (helper.var_stamp[v->id()] > helper.tab_stamp[v->id()]) {
						Y_evt_.clear();
						const auto consistent = c->propagate(Y_evt_);
						if (!consistent) {
							return false;
						}
						else
						{
							for (auto y : Y_evt_) {
								insert(y);
							}
						}

						helper.global_stamp++;
						helper.tab_stamp[c->id] = helper.global_stamp;
					}
				}
			}
			return true;
		}

		void Remove(Val& v_a) override {
			//约束的已实例化变量个数减1
			for (auto c : helper.subscription[v_a.v->id()]) {
				c->bind_count--;
			}
			v_a.v->Remove(v_a.a);
		}

		void Bind(Val& v_a) {
			//在稀疏集上交换变量
			auto minvi = level_V_sparse_[v_a.v->id()];
			auto a = level_V_dense_[helper.level - 1];
			level_V_dense_[helper.level - 1] = level_V_dense_[minvi];

			level_V_sparse_[a] = minvi;
			level_V_sparse_[level_V_dense_[minvi]] = helper.level - 1;

			level_V_dense_[minvi] = a;

			for (auto c : helper.subscription[v_a.v->id()]) {
				c->bind_count++;
			}

			v_a.v->Bind(v_a.a);
		}
		void insert(Var* x) {
			Q_->push(x);
			++helper.global_stamp;
			helper.var_stamp[x->id()] = helper.global_stamp;
		}


		//修改levelvdense
		Val SelectVal() override {
			auto mindmdd = DBL_MAX;
			Var* minv = nullptr;

			for (size_t i = helper.level; i < num_vars; i++)
			{
				auto vid = level_V_dense_[i];
				auto  v = vars[vid];
				double ddeg = 0.0;
				for (auto c : helper.subscription[vid]) {
					if (c->bind_count + 1 < c->arity) {
						ddeg++;
					}
				}

				if (!ddeg) {
					return Val{ v, v->MinValue() };
				}

				auto dmdd = v->Size() / ddeg;
				if (dmdd < mindmdd) {
					minv = v;
					mindmdd = dmdd;
				}
			}

			return  Val{ minv, minv->MinValue() };
			//var i = level
			//	while (i < num_vars) {
			//		val vid = levelvdense(i)
			//			val v = vars(vid)
			//			var ddeg : Double = 0L

			//			for (c < -subscription(vid)) {
			//				if (c.assignedCount + 1 < c.arity) {
			//					ddeg += 1
			//				}
			//			}

			//		if (ddeg == 0) {
			//			return new Val(v, v.minValue)
			//				//        return new Val(v, v.dense(0))
			//		}

			//		val sizeD : Double = v.size.toDouble
			//			val dmdd = sizeD / ddeg

			//			if (dmdd < mindmdd) {
			//				minv = v
			//					mindmdd = dmdd
			//			}
			//		i += 1
			//	}

		}

	protected:
		vector<int> level_V_dense_;
		vector<int> level_V_sparse_;
		unique_ptr<CoarseQueue<Var*>> Q_;
		vector<Var*> Y_evt_;
	};
}


