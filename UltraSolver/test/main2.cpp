//#include <taskflow/taskflow.hpp>  // Cpp-Taskflow is header-only
//#include "../UltraSolver/AssignedStack.h"
//
//int main() {
//
//	//tf::Taskflow taskflow;
//	//auto [A, B, C, D] = taskflow.emplace(
//	//	[]() { std::cout << "TaskA\n"; },               //  task dependency graph
//	//	[]() { std::cout << "TaskB\n"; },               // 
//	//	[]() { std::cout << "TaskC\n"; },               //          +---+          
//	//	[]() { std::cout << "TaskD\n"; }                //    +---->| B |-----+   
//	//);                                                 //    |     +---+     |
//	//												   //  +---+           +-v-+ 
//	//A.precede(B);  // A runs before B                  //  | A |           | D | 
//	//A.precede(C);  // A runs before C                  //  +---+           +-^-+ 
//	//B.precede(D);  // B runs before D                  //    |     +---+     |    
//	//C.precede(D);  // C runs before D                  //    +---->| C |-----+    
//	//												   //          +---+          
//	//taskflow.wait_for_all();  // block until finish
//
//	/*auto b = 0;
//	auto c = b++;
//	std::cout << b << ", " << c << std::endl;
//	using namespace std;
//
//	unsigned long mask = 0x1000;
//	unsigned long index;
//	unsigned char isNonzero;
//
//	cout << "Enter a positive integer as the mask: " << flush;
//	cin >> mask;
//	isNonzero = _BitScanForward(&index, mask);
//	if (isNonzero) {
//		cout << "Mask: " << mask << " Index: " << index << endl;
//	}
//	else {
//		cout << "No set bits found.  Mask is zero." << endl;
//	}*/
//
//	//cp::u64 c = 0xf4da;
//	cp::u64 c = 0;
//	bitset<64> b(c);
//	cout << b.to_string() << endl;
//	cout << BitCount64(c) << endl;
//	cout << CLZ64(c) << endl;
//	cout << CTZ64(c) << endl;
//	//cout << BitCount64(c) << endl;
//	//cout << BitCount64(c) << endl;
//	return 0;
//}
#pragma once
#include <taskflow/threadpool/threadpool.hpp>
#include <taskflow/taskflow.hpp>
#include <chrono>
#include <cmath>
#include <random>
#include <numeric>
#include <climits>
#include <iomanip>
#include <functional>

using Closure = std::function<void()>;
using namespace std;

tf::Taskflow taskflow(5);
void xixixi() {
	cout << "xixixi" << endl;
}
class A {
public:
	string xixi;
	A() { cout << "kaka1" << endl; };
	A(string& xi) :xixi(xi) { cout << "kaka2 " << xi << endl; }
	//void operator()() const { cout << xixi; }
	void operator()(tf::SubflowBuilder& subflow) const {
		this_thread::sleep_for(chrono::milliseconds(1000));
		const string  aa = xixi + "inner";
		cout << xixi << endl;
		this_thread::sleep_for(chrono::milliseconds(1000));
		subflow.emplace([aa]() {
			this_thread::sleep_for(chrono::milliseconds(1000));
			const string  bb = aa + "inner";
			cout << bb << endl;
						});
	}
	void show() const { cout << xixi; }
};
int main() {
	string a1 = "xixi";
	string b1 = "haha";
	string c1 = "hehe";
	string d1 = "hoho";
	A a(a1);
	A b(b1);

	shared_ptr<A> c = make_shared <A>(c1);
	shared_ptr<A> d = make_shared <A>(d1);
	//taskflow.silent_dispatch(a);
	//cout << "--" << endl;
	//taskflow.silent_dispatch(b);
	//a();
	//b();
	//a();
	//cout << "---" << endl;
	//tf::Taskflow taskflow(2);
	//auto f = [a] {a(); };
	//f();
	//cout << "---" << endl;
	////taskflow.emplace(f());
	////taskflow.emplace([b](A & x) {x(); });
	//auto bb = taskflow.emplace(b);
	//taskflow.emplace(std::move(*c));
	//taskflow.wait_for_all();
	////tf::Taskflow taskflow2(1);
	////taskflow2.emplace(std::move(a));

	////taskflow2.wait_for_all();
	//taskflow.emplace(a);
	cout << "--" << endl;
	taskflow.emplace(a);
	taskflow.emplace(b);
	taskflow.wait_for_all();
	////bb = taskflow.emplace(b);	aa = taskflow.emplace(a);
	////bb = taskflow.emplace(b);
	////bb = taskflow.emplace(b);	aa = taskflow.emplace(a);
	////bb = taskflow.emplace(b);
	////bb = taskflow.emplace(b);
	////taskflow.wait_for_all();

	//tf::WorkStealingThreadpool<Closure> pool(std::thread::hardware_concurrency());
	//pool.emplace(a);
	//tf::WorkStealingThreadpool<A> tp(std::thread::hardware_concurrency());
	//pool
	return 0;
}
