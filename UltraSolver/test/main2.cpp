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

#include <taskflow/threadpool/threadpool.hpp>
#include <chrono>
#include <cmath>
#include <random>
#include <numeric>
#include <climits>
#include <iomanip>
#include <taskflow/taskflow.hpp>
using namespace std;
class A {
public:
	string xixi;
	A() {};
	A(string& xi) :xixi(xi) {}
	void operator()() const { cout << xixi; }
	void show() { cout << xixi; }
};
int main() {
	string a1 = "xixi\n";
	string b1 = "haha\n";
	string c1 = "hehe\n";
	string d1 = "hoho\n";
	A a(a1);
	A b(b1);

	shared_ptr<A> c = make_shared <A>(c1);
	shared_ptr<A> d = make_shared <A>(d1);
	//a();
	//b();
	tf::Taskflow taskflow(1);
	auto aa = taskflow.emplace(a);
	auto bb = taskflow.emplace(b);
	taskflow.wait_for_all();
	tf::Taskflow taskflow2(1);
	taskflow2.emplace(std::move(a));
	taskflow2.emplace(std::ref(*c));
	taskflow2.wait_for_all();
	//aa = taskflow.emplace(a);
	//bb = taskflow.emplace(b);
	//bb = taskflow.emplace(b);
	//aa = taskflow.emplace(a);
	//bb = taskflow.emplace(b);
	//bb = taskflow.emplace(b);
	//aa = taskflow.emplace(a);
	//bb = taskflow.emplace(b);
	////taskflow.wait_for_all();
	//bb = taskflow.emplace(b);	aa = taskflow.emplace(a);
	//bb = taskflow.emplace(b);
	//bb = taskflow.emplace(b);	aa = taskflow.emplace(a);
	//bb = taskflow.emplace(b);
	//bb = taskflow.emplace(b);
	//taskflow.wait_for_all();

	//tf::WorkStealingThreadpool<shared_ptr<A>> pool(3);
	//pool
	return 0;
}