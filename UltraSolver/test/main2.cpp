////#include <taskflow/taskflow.hpp>  // Cpp-Taskflow is header-only
////#include "../UltraSolver/AssignedStack.h"
////
////int main() {
////
////	//tf::Taskflow taskflow;
////	//auto [A, B, C, D] = taskflow.emplace(
////	//	[]() { std::cout << "TaskA\n"; },               //  task dependency graph
////	//	[]() { std::cout << "TaskB\n"; },               // 
////	//	[]() { std::cout << "TaskC\n"; },               //          +---+          
////	//	[]() { std::cout << "TaskD\n"; }                //    +---->| B |-----+   
////	//);                                                 //    |     +---+     |
////	//												   //  +---+           +-v-+ 
////	//A.precede(B);  // A runs before B                  //  | A |           | D | 
////	//A.precede(C);  // A runs before C                  //  +---+           +-^-+ 
////	//B.precede(D);  // B runs before D                  //    |     +---+     |    
////	//C.precede(D);  // C runs before D                  //    +---->| C |-----+    
////	//												   //          +---+          
////	//taskflow.wait_for_all();  // block until finish
////
////	/*auto b = 0;
////	auto c = b++;
////	std::cout << b << ", " << c << std::endl;
////	using namespace std;
////
////	unsigned long mask = 0x1000;
////	unsigned long index;
////	unsigned char isNonzero;
////
////	cout << "Enter a positive integer as the mask: " << flush;
////	cin >> mask;
////	isNonzero = _BitScanForward(&index, mask);
////	if (isNonzero) {
////		cout << "Mask: " << mask << " Index: " << index << endl;
////	}
////	else {
////		cout << "No set bits found.  Mask is zero." << endl;
////	}*/
////
////	//cp::u64 c = 0xf4da;
////	cp::u64 c = 0;
////	bitset<64> b(c);
////	cout << b.to_string() << endl;
////	cout << BitCount64(c) << endl;
////	cout << CLZ64(c) << endl;
////	cout << CTZ64(c) << endl;
////	//cout << BitCount64(c) << endl;
////	//cout << BitCount64(c) << endl;
////	return 0;
////}
//#pragma once
//#include <taskflow/threadpool/threadpool.hpp>
//#include <taskflow/taskflow.hpp>
//#include <chrono>
//#include <cmath>
//#include <random>
//#include <numeric>
//#include <climits>
//#include <iomanip>
//#include <functional>
//
//using Closure = std::function<void()>;
//using namespace std;
//
//tf::Taskflow taskflow(5);
//tf::Taskflow tfw;
//
//using Closure = std::function<void()>;
//tf::WorkStealingThreadpool<Closure > pool(5);
//
//
//void xixixi() {
//	cout << "xixixi" << endl;
//}
//class A {
//public:
//	string ss;
//	A() { cout << "kaka1" << endl; }
//	A(const int id) {
//		stringstream s;
//		s << "thread_";
//		s << id;
//		ss = s.str();
//		cout << "construct: " << ss << endl;
//	}
//	virtual void operator()() {
//		this_thread::sleep_for(chrono::milliseconds(1000));
//		cout << ss << endl;
//	};
//	//virtual void operator()() = 0;
//	//virtual void operator()(tf::SubflowBuilder& subflow) = 0;
//	//virtual void operator()() =;
//	//void operator()(tf::SubflowBuilder& subflow) const {
//	//	this_thread::sleep_for(chrono::milliseconds(1000));
//	//	const string  aa = ss + "inner";
//	//	cout << ss << endl;
//	//	subflow.emplace([aa]() {
//	//		this_thread::sleep_for(chrono::milliseconds(1000));
//	//		const string  bb = aa + "inner";
//	//		cout << bb << endl;
//	//					});
//	//}
//	void show() const { cout << ss; }
//};
//
////class B final : public A {
////public:
////	//void operator()() override {
////	//	cout << "overwrite A" << endl;
////	//}
////
////
////	B(string& cs) :A(cs) {
////		cout << "overwrite constructor" << endl;
////	}
////
////	//void operator()(tf::SubflowBuilder& subflow) override {
////	//	this_thread::sleep_for(chrono::milliseconds(1000));
////	//	cout << ss << endl;
////	//	auto aa = ss;
////	//	subflow.emplace([aa]() {
////	//		//aa = aa + "sub";
////	//		this_thread::sleep_for(chrono::milliseconds(1000));
////	//		cout << "sub" << aa << endl;
////	//					}
////	//	);
////
////	//	cout << "after sub " << ss << endl;
////	//}
////};
//
//void fafa(A&& a) {
//	a.show();
//}
//
//void tata(A* a) {
//	(*a)();
//}
//
//void fafa(shared_ptr<A>&& a) {
//	a->show();
//}
//
////void tata(A* a, tf::SubflowBuilder& subflow) {
////	(*a)(subflow);
////}
//
////void coco(A* a) {
////	tfw.emplace([a](auto & subflow) {tata(a, subflow); });
////	//tfw.emplace(*(dynamic_cast<B*>(a)));
////}
//
////void cocob(B* a) {
////	tfw.emplace(*a);
////}
//
//
//
//int main() {
//	//string a1 = "xixi";
//	//string b1 = "haha";
//	//string c1 = "hehe";
//	//string d1 = "hoho";
//	//string e1 = "koko";
//
//	//A a(a1);
//	//A b(b1);
//	//shared_ptr<A> c = make_shared <A>(c1);
//	//shared_ptr<A> d = make_shared <A>(d1);
//	//A* e = new A(e1);
//
//	//taskflow.silent_dispatch(a);
//	//cout << "--" << endl;
//	//taskflow.silent_dispatch(b);
//
//	////a();
//	////b();
//	////a();
//	////cout << "---" << endl;
//	////tf::Taskflow taskflow(2);
//	////auto f = [a] {a(); };
//	////f();
//	////cout << "---" << endl;
//	//////taskflow.emplace(f());
//	//////taskflow.emplace([b](A & x) {x(); });
//	////auto bb = taskflow.emplace(b);
//	////taskflow.emplace(std::move(*c));
//	////taskflow.wait_for_all();
//	//////tf::Taskflow taskflow2(1);
//	//////taskflow2.emplace(std::move(a));
//
//	//////taskflow2.wait_for_all();
//	////taskflow.emplace(a);
//	//cout << "--" << endl;
//	//taskflow.emplace(a);
//	//taskflow.emplace(b);
//	//coco(e);
//	//tfw.wait_for_all();
//	//taskflow.wait_for_all();
//	////bb = taskflow.emplace(b);	aa = taskflow.emplace(a);
//	////bb = taskflow.emplace(b);
//	////bb = taskflow.emplace(b);	aa = taskflow.emplace(a);
//	////bb = taskflow.emplace(b);
//	////bb = taskflow.emplace(b);
//	////taskflow.wait_for_all();
//
//	//tf::WorkStealingThreadpool<Closure> pool(std::thread::hardware_concurrency());
//	//pool.emplace(a);
//	//tf::WorkStealingThreadpool<A> tp(std::thread::hardware_concurrency());
//	//pool
//	//delete e;
//
//	//vector<std::function<void>> pro(5);
//	//taskflow.emplace(pro[1]);
//
//
//	//B a(a1);
//	//B b(b1);
//	////tata(&a);
//	//coco(&a);
//	//coco(&b);
//	////tfw.emplace(a);
//	//tfw.wait_for_all();
//	const int num_task = 100;
//	vector<shared_ptr<A>> a(num_task);
//	std::promise<void> prom;
//	auto fut = prom.get_future();
//	std::atomic<int64_t> inner_counter(0);
//	std::atomic<int64_t> outer_counter(0);
//
//	vector<Closure> tasks(num_task);
//	for (auto i = 0; i < num_task; i++) {
//		a[i] = make_shared<A>(i);
//		//tasks[i] = [a, i]() {fafa(a[i]); };
//	}
//
//	for (auto i = 0; i < num_task; i++) {
//		tasks[i] = [&num_task, &prom, &inner_counter, &outer_counter, &a, i]() {
//
//			//if (outer_counter == inner_counter) {
//			//	prom.set_value();
//			//}
//
//			if (++outer_counter == num_task) {
//				prom.set_value();
//			}
//
//			cout << "task_" << i << endl;
//			a[i]->show();
//			//++outer_counter;
//
//			//prom.set_value();
//		};
//		//tasks[i] = [a, i]() {fafa(a[i]); };
//
//		//tasks[i] = fafa;
//	}
//
//	cout << "---" << endl;
//
//	for (auto i = 0; i < num_task; ++i) {
//		cout << "push: " << i << endl;
//		//pool.emplace([&]() {
//		//	cout << "task_" << i << endl;
//		//	a[i]->show();
//		//	prom.set_value();
//		//			 });
//		++inner_counter;
//		pool.emplace(tasks[i]);
//	}
//
//	fut.get();
//
//
//	cout << "------------------finished----------------------" << endl;
//	//this_thread::sleep_for(chrono::seconds(2));
//	return 0;
//}
//
