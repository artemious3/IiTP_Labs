#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include <sched.h>
#include <thread>

/*
 *  VARIANT : 10
 */

using namespace std;

// @return : <result, num of iterations, time in ns>
std::tuple<double, int, int> calcX(double x, double eps, double target){

	double x_mul = (x-1.0)/(x+1.0);
	double accum = (x-1.0)/(x+1.0);
	std::chrono::high_resolution_clock clk;

	int i = 1;
	auto beg = clk.now();
	while(abs(accum - target) > eps){
		double koef = 2*i+1;

		double tmp;
		asm (

			"fld1          \n\t"
			"fldl %[x]     \n\t"
			"faddp         \n\t"
			"fst %%st(1)   \n\t"
			"fmulp         \n\t"
			"fstl %[tmp]   \n\t"       // tmp <- (x+1)*(x+1)

			"fld1          \n\t"
			"fldl %[x]     \n\t"
			"fsubp         \n\t"
			"fst %%st(1)   \n\t"
			"fmulp         \n\t"
			"fdivl %[tmp]  \n\t"

			"fldl %[x_mul]  \n\t"
			"fmulp          \n\t"
			"fstl %[x_mul]  \n\t"


			"fldl %[koef]   \n\t"
			"fdivrp          \n\t"


			"fldl %[accum] \n\t"
			"faddp         \n\t"
			"fstl %[accum] \n\t"


			: [tmp]"+m"(tmp), [x_mul]"+m"(x_mul), [accum]"+m"(accum)
			: [x]"m"(x), [koef]"m"(koef)
			   );

		i++;
	}
	auto end = clk.now();
	int time = std::chrono::duration_cast<std::chrono::nanoseconds>(end-beg).count();
	return std::make_tuple(accum, i, time);
}

double calcY(double x){
	return 0.5*std::log(x);
}



void run_single_core(){
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(2, &mask);
	sched_setaffinity(0, sizeof(mask), &mask);
	double a, b, h, eps;

	a = 1.0001;
	b = 40000.1;

	h = 1.0;
	eps = 0.00001;

	std::ofstream ofs("results.txt");
	for (double x = a; x <= b; x+= h){
		double target = calcY(x);
		auto [res, iter, time] = calcX(x, eps, target);
		ofs << iter << ' '  << time << ' ' << res << '\n';
		// cout << "f(" << x << ") = " <<  res << '\n';
	}
}


void run_multi_core(){

	double a, b, h, eps;

	a = 1.0001;
	b = 40000.1;

	h = 1.0;
	eps = 0.00001;


	std::vector<thread> vec;
	vec.reserve(16);
	for(int i = 0; i < 16; ++i){

		double beg = a + i*h;
		vec.push_back(std::thread { [=]{
			cpu_set_t mask;
			CPU_ZERO(&mask);
			CPU_SET(i, &mask);
			sched_setaffinity(0, sizeof(mask), &mask);

			std::ofstream ofs("NOMCres" + std::to_string(i) + ".txt");
			for (double x = beg; x <= b; x+= 16*h){
				double target = calcY(x);
				auto [res, iter, time] = calcX(x, eps, target);
				ofs << iter << ' '  << time << ' ' << res << '\n';
			}
		}});
	}


	for(auto& t : vec ){
		t.join();
	}
}


int main(){
	run_multi_core();

}
