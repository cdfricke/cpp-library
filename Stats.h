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

    #define UNIFORM 0
    #define GAUSSIAN 1

    namespace stats {
        // 1D and 2D SUM()
        double sum(std::vector<double> arr);
        std::vector<double> sum(std::vector<std::vector<double>> mat, const int axis = 1);
        
        // 1D and 2D MEAN()
        double mean(std::vector<double> arr);
        std::vector<double> mean(std::vector<std::vector<double>> mat, const int axis = 1);

        // 1D and 2D STDEV()
        double stdev(std::vector<double> arr);
        std::vector<double> stdev(std::vector<std::vector<double>> mat, const int axis = 1);

        /* DISTRIBUTION GENERATORS */
        std::vector<double> realDistribution(const int dist, const int num_samples, const double min, const double max);
        std::vector<int> intDistribution(const int dist, const int num_samples, const int min, const int max);
    }



#endif