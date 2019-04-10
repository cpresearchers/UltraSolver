#pragma once
#include <taskflow/threadpool/threadpool.hpp>
#include <taskflow/taskflow.hpp>
#include <chrono>
#include <random>
#include <functional>

using Closure = std::function<void()>;
using namespace std;

tf::Taskflow taskflow(5);
tf::Taskflow tfw;

using Closure = std::function<void()>;
tf::WorkStealingThreadpool<Closure > pool(5);


void xixixi() {
	cout << "xixixi" << endl;
}
class A {
public:
	string ss;
	A() { cout << "kaka1" << endl; }
	A(const int id) {
		stringstream s;
		s << "thread_";
		s << id;
		ss = s.str();
		cout << "construct: " << ss << endl;
	}
	virtual void operator()() {
		this_thread::sleep_for(chrono::milliseconds(1000));
		cout << ss << endl;
	};
	void show() const { cout << ss; }
};

int main() {
	const int num_task = 100;
	vector<shared_ptr<A>> a(num_task);

	std::promise<void> prom;
	auto fut = prom.get_future();
	std::atomic<int64_t> inner_counter(0);
	bool flag = false;
	//atomic_flag lock = ATOMIC_FLAG_INIT;
	vector<Closure> tasks(num_task);

	for (auto i = 0; i < num_task; i++) {
		a[i] = make_shared<A>(i);
	}

	for (auto i = 0; i < num_task; i++) {

		tasks[i] = [&prom, &inner_counter, &flag, &a, i]() {

			cout << "task_" << i << endl;
			a[i]->show();

			if (!(--inner_counter) && flag) {
				prom.set_value();
			}
		};
	}

	cout << "---" << endl;
	//lock.clear();
	for (auto i = 0; i < num_task; i += 2) {
		++inner_counter;
		cout << "push: " << i << endl;
		pool.emplace(tasks[i]);

	}
	//cout << inner_counter << endl;

	//while (inner_counter != num_task);
	//flag = true;
	//lock.test_and_set();
	flag = true;
	fut.get();

	cout << "------------------finished----------------------" << endl;
	//this_thread::sleep_for(chrono::seconds(2));
	return 0;
}

