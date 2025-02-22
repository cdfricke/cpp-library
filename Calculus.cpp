/*
Programmer: Connor Fricke (cd.fricke23@gmail.com)
File: Calculus.cpp
Latest Revision: 20-Feb-2024
Synopsis: Implementation File for calculus routines
*/

#include "Calculus.h"

using namespace calculus;

double integral::trapezoidal(double (*func)(double), const double a, const double b, const int n)
{
    double h = ((b - a)/double(n - 1)); // point spacing
    double sum= 0.0;

    // midpoint contributions
    for (int i=2; i<n; i++) 
    { 
        double x = a + h * double(i-1);
        sum += h * func(x);
    }
    // endpoint contributions
    sum += (h/2.0) * (func(a) + func(b));

    return (sum);   
}

double integral::monteCarlo(double (*func)(double), const double a, const double b, const int iterations)
{
    /* 
    https://cameron-mcelfresh.medium.com/monte-carlo-integration-313b37157852
    */

	double sum = 0;
	double randNum, f;

    for (int i = 0; i < iterations - 1; i++)
	{
		// generate random number within bounds
		randNum = a + (double(std::rand())/RAND_MAX) * (b - a);
        // sample function
		f = func(randNum);
		sum += f;
	}

	return (b - a) * sum / iterations;
}

double integral::simpsons(double (*func)(double), const double a, const double b, const int n)
{
    double h = ((b - a) / double(n - 1));
    double sum = 0.;

    for (int i = 2; i < n; i += 2) // loop for odd points
    {
        double x = a + h * double(i-1);
        sum += (4./3.)*h * func(x);
    }
    for (int i = 3; i < n; i += 2) // loop for even points
    {
        double x = a + h * double(i-1);
        sum += (2./3.)*h * func(x);
    }
    // endpoint contributions
    sum += (h/3.) * (func(a) + func(b));
    return (sum);
}