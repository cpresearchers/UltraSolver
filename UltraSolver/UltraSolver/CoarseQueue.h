#pragma once
#include <vector>
using namespace std;
namespace cudacp {
	template<class T>
	class CoarseQueue
	{
	public:
		CoarseQueue(const int num_vars) :max_size_(num_vars + 1) {
			table_.resize(max_size_);
			in_stack_.resize(max_size_, false);
		};

		Initial(const int num_vars) {
			max_size_ = num_vars + 1;
			table_.resize(max_size_);
			in_stack_.resize(max_size_, false);
		}

		~CoarseQueue();

		bool IsFull() {
			return front_ == (rear_ + 1) % max_size_;
		}

		void push(T v) {
			if (in_stack_(v->id()))
				return;
			table_[rear_] = v;
			rear_ = [rear_ + 1] % max_size_;
			in_stack_[v->id()] = true;
			++size;
		}

		T pop() {
			auto tmp = table_[front_];
			front_ = (front_ + 1) % max_size_;
			in_stack_[tmp->id()] = false;
			--size_;
			return tmp;
		}

		void clear() {
			int front_ = 0;
			int rear_ = 0;
			int size_ = 0;
			in_stack_.assign(max_size_, false);
		}

		int size() {
			return size_;
		}

	protected:
		int max_size_;
		int front_ = 0;
		int rear_ = 0;
		int size_ = 0;
		vector<T> table_;
		vector<bool> in_stack_;
	};
}