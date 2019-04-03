#include <taskflow/taskflow.hpp>  // Cpp-Taskflow is header-only
#include "../UltraSolver/AssignedStack.h"

int main() {

	//tf::Taskflow taskflow;
	//auto [A, B, C, D] = taskflow.emplace(
	//	[]() { std::cout << "TaskA\n"; },               //  task dependency graph
	//	[]() { std::cout << "TaskB\n"; },               // 
	//	[]() { std::cout << "TaskC\n"; },               //          +---+          
	//	[]() { std::cout << "TaskD\n"; }                //    +---->| B |-----+   
	//);                                                 //    |     +---+     |
	//												   //  +---+           +-v-+ 
	//A.precede(B);  // A runs before B                  //  | A |           | D | 
	//A.precede(C);  // A runs before C                  //  +---+           +-^-+ 
	//B.precede(D);  // B runs before D                  //    |     +---+     |    
	//C.precede(D);  // C runs before D                  //    +---->| C |-----+    
	//												   //          +---+          
	//taskflow.wait_for_all();  // block until finish

	/*auto b = 0;
	auto c = b++;
	std::cout << b << ", " << c << std::endl;
	using namespace std;

	unsigned long mask = 0x1000;
	unsigned long index;
	unsigned char isNonzero;

	cout << "Enter a positive integer as the mask: " << flush;
	cin >> mask;
	isNonzero = _BitScanForward(&index, mask);
	if (isNonzero) {
		cout << "Mask: " << mask << " Index: " << index << endl;
	}
	else {
		cout << "No set bits found.  Mask is zero." << endl;
	}*/

	//cp::u64 c = 0xf4da;
	cp::u64 c = 0;
	bitset<64> b(c);
	cout << b.to_string() << endl;
	cout << BitCount64(c) << endl;
	cout << CLZ64(c) << endl;
	cout << CTZ64(c) << endl;
	//cout << BitCount64(c) << endl;
	//cout << BitCount64(c) << endl;
	return 0;
}
