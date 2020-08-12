/*

//https://docs.microsoft.com/en-us/visualstudio/debugger/get-started-debugging-multithreaded-apps?view=vs-2019
**  CORRECTED VERSION

//https://github.com/lat/esc/blob/master/exercises/openmp/mandel.c
link has question aka code with deliberate errors
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

# define NPOINTS 1000
# define MAXITER 1000


struct d_complex {

public:

    d_complex()
    {}
    double r;
    double i;
};

void testpoint(d_complex c);


d_complex c;


int numoutside = 0;

int main() {
    int i; //note how this master thread stack variable is handled when loop iterations are distributed 
    double area, error, eps = 1.0e-5;
    c.r = 3; c.i = 3;
    

    //note how default(none) forces me to be explicit about data env (private/shared/blah) 
    //good debugging practice

#pragma omp parallel for default(none) private(c) shared(eps) //each thread gets its own copy of c
    for (i = 0; i < NPOINTS; i++) {
        for (int j = 0; j < NPOINTS; j++) { //note each thread needs its own copy of j
            c.r = -2.0 + 2.5 * (double)(i) / (double)(NPOINTS) + eps;
            c.i = 1.125 * (double)(j) / (double)(NPOINTS) + eps;
            testpoint(c);
        }
    }


    area = 2.0 * 2.5 * 1.125 * (double)(NPOINTS * NPOINTS - numoutside) / (double)(NPOINTS * NPOINTS);
    error = area / (double)NPOINTS;

    printf("Area of Mandlebrot set = %12.8f +/- %12.8f\n", area, error);
    std::cout << numoutside;
}

void testpoint(d_complex c) {

   

    d_complex z;
    int iter;
    double temp;
    z = c;

    for (iter = 0; iter < MAXITER; iter++) {
        temp = (z.r * z.r) - (z.i * z.i) + c.r;
        z.i = z.r * z.i * 2 + c.i;
        z.r = temp;
        if ((z.r * z.r + z.i * z.i) > 4.0) {
#pragma omp atomic //okay 'cause the majority of points are inside, this sync is not too expensive 
            numoutside++;            
            break;
        }
    }

}