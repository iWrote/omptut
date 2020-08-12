#include <iostream>
#include <omp.h>

//promote accumulator to an array and sum it up at the end 


int main()
{
	double t = omp_get_wtime();
	int N = 100000000;
	double dx = 1.0 / N;
	double I = 0;
	const double pi = 3.14159265358979323846;
	double g[24]; //12 is the max number of threads I have? No. Can ask for 24 threads if I like; they'll get scheduled by OS.
	int nthreads = 1; //to store the number of threads I actually get
	omp_set_num_threads(4);
#pragma omp parallel
	{

		int id = omp_get_thread_num();
		int nthrds = omp_get_num_threads(); //this could be LESS THAN 15! I don't set num_threads; I request for a certain num of threads
		double sum = 0;
		if (id == 0) nthreads = nthrds; // I need to know this AFTER threads have joined for sum. And can't risk multiple threads writing together
		for (int i = id; i < N; i+=nthrds)
		{
			double x = (i + 0.5) / N;
			sum += 4 / (1 + x * x);// *dx --  just do it l8r;
		}

		g[id] = sum;
	}

	for (int i = 0; i < nthreads; i++)
	{
		I += g[i] * dx;
	}
	std::cout << "nthreads: " << nthreads << std::endl;
	printf("calculated: %1.18f\nactual____: %1.18f\ndifference: %1.18f\n", I, pi, I-pi);
	printf("Time elapsed: %f", omp_get_wtime() - t);
}


/*
nthreads: 1
calculated: 3.141592653590426387
actual____: 3.141592653589793116
difference: 0.000000000000633271
Time elapsed: 1.326220

nthreads: 2
calculated: 3.141592653590022266
actual____: 3.141592653589793116
difference: 0.000000000000229150
Time elapsed: 0.674603

nthreads: 4
calculated: 3.141592653590216777
actual____: 3.141592653589793116
difference: 0.000000000000423661
Time elapsed: 0.339127

nthreads: 12
calculated: 3.141592653589739825
actual____: 3.141592653589793116
difference: -0.000000000000053291
Time elapsed: 0.182399

nthreads: 24
calculated: 3.141592653589754036
actual____: 3.141592653589793116
difference: -0.000000000000039080
Time elapsed: 0.208672

*/