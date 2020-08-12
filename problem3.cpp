#include <iostream>
#include <cmath>
#include <omp.h>

//READ: http://www.inf.ufsc.br/~bosco.sobral/ensino/ine5645/OpenMP_Dynamic_Scheduling.pdf

int main()
{
	const double pi = 3.14159265358979323846;
	double t = omp_get_wtime();
	double sum = 1337; //note how the reduction clause REinitializes this to ZERO (for operator+)
	int N = 100000000;
	double dx = 1.0 / N;

#pragma omp parallel for schedule(static) reduction(+:sum)  //parallelization with a single line of code, neat no?
	for (int i = 0; i < N; i++)
	{
		double x = (i + 0.5) / N;
		sum += (4 / (1 + x * x));
	}
	sum *= dx;

	//std::cout << "nthreads: " << nthreads << std::endl;
	printf("calculated: %1.18f\nactual____: %1.18f\ndifference: %1.18f\n", sum, pi, sum - pi);
	printf("Time elapsed: %f", omp_get_wtime() - t);
}

/*
N = 100000000

SINGLE THREADED

calculated: 3.141592653590426387
actual____: 3.141592653589793116
difference: 0.000000000000633271
Time elapsed: 3.959210

MULTI THREADED (static scheduling) //assigns iterations BEFORE runtime

calculated: 3.141592653589828643
actual____: 3.141592653589793116
difference: 0.000000000000035527
Time elapsed: 0.514650

MULTI THREADED (dynamic scheduling) //pointless overhead here because each iteration of the loop takes constant time
									//assigns iterations AT RUNTIME to each thread as it finishes execution
calculated: 3.141592653589734496    //useful for when some iterations take longer
actual____: 3.141592653589793116    //you don't want to assign all the longest iterations to one thread at the start and then have all other threads waiting for it
difference: -0.000000000000058620	//to finish before joining
Time elapsed: 8.152584

*/