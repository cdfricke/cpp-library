/*
Programmer: Connor Fricke (cd.fricke23@gmail.com)
File: Calculus.h
Latest Revision: 19-Feb-2024
Synopsis: Header File for Integration function library
*/

#ifndef CALCULUS_H
#define CALCULUS_H

    namespace calculus::integral {

        /* Adapted from Physics 5810 with Prof. Ralf Bundschuh and Prof. Dick Furnstahl */
        double trapezoidal(double (*func)(double), const double a, const double b, const int n);
        
        /* Adapted from adapted from Cameron McElfresh */
        /* https://cameron-mcelfresh.medium.com/monte-carlo-integration-313b37157852 */
        double monteCarlo(double (*func)(double), const double a, const double b, const int n);

        /* Adapted from Physics 5810 with Prof. Ralf Bundschuh and Prof. Dick Furnstahl */
        double simpsons(double (*func)(double), const double a, const double b, const int n);
        
    }

    namespace calculus::derivative {

        
    }

#endif