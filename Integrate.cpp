/*
Programmer: Connor Fricke (cd.fricke23@gmail.com)
File: Integrate.h
Latest Revision: 19-Feb-2024
Synopsis: Implementation File for Integration functions
*/

#include "Integrate.h"

double integrate::trapezoidal(double (*func)(double), const double a, const double b, const int n)
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