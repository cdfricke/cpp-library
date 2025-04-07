/*
Programmer: Connor Fricke (cd.fricke23@gmail.com)
File: Stats.h
Latest Revision: 22-Feb-2024
Synopsis: Header File for statistics routines
*/

#ifndef STATS_H
#define STATS_H

    #include <cstdlib>
    #include <vector>
    #include <random>

    namespace stats {
        const short int UNIFORM = 0;
        const short int GAUSSIAN = 1;

        // SUM()
        double sum(std::vector<double> arr);
        
        // 1D and 2D MEAN()
        double mean(std::vector<double> arr);

        // 1D and 2D STDEV()
        double stdev(std::vector<double> arr);

        /* DISTRIBUTION GENERATORS */
        std::vector<double> realDistribution(const int dist, const int num_samples, const double min, const double max);
    }



#endif