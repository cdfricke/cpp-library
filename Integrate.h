/*
Programmer: Connor Fricke (cd.fricke23@gmail.com)
File: Integrate.h
Latest Revision: 19-Feb-2024
Synopsis: Header File for Integration function library
*/

#ifndef INTEGRATE_H
#define INTEGRATE_H

    namespace integrate {

        double trapezoidal(double (*func)(double), const double a, const double b, const int n);
        
    }

#endif