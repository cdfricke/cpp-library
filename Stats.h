/*
Programmer: Connor Fricke (cd.fricke23@gmail.com)
File: Stats.h
Latest Revision: 22-Feb-2024
Synopsis: Header and implementation file for statistics routines
*/

#ifndef STATS_H
#define STATS_H

    #include <cstdlib>
    #include <vector>
    #include <random>

    // DECLARATIONS
    namespace stats {
        const short int UNIFORM = 0;
        const short int GAUSSIAN = 1;

        // SUM()
        template <typename T>
        T sum(std::vector<T> arr);
        
        // MEAN()
        template <typename T>
        double mean(std::vector<T> arr);

        // STDEV()
        template <typename T>
        double stdev(std::vector<T> arr);

        // REALDISTRIBUTION
        std::vector<double> realDistribution(const int dist, const int num_samples, const double min, const double max);
    }

    // DEFINITIONS
    namespace stats {

        // SUM()
        template <typename T>
        T sum(std::vector<T> arr)
        {
            T sum = 0;
            for (T &x : arr)
            {
                sum += x;
            }
            return sum;
        }

        // MEAN()
        template <typename T>
        double mean(std::vector<T> arr)
        {
            return sum(arr) / double(arr.size());
        }

        // STDEV()
        template <typename T>
        double stdev(std::vector<T> arr)
        {
            double avg = mean(arr);
            double sum_sqr_res = 0.0; // sum of squared residuals
            for (T &x : arr)
            {
                sum_sqr_res += (x - avg) * (x - avg);
            }
            return sqrt(sum_sqr_res);
        }

        // REALDISTRIBUTION()
        std::vector<double> realDistribution(const int dist, const int num_samples, const double min, const double max)
        {
            std::vector<double> result(num_samples, 0.0);

            std::random_device rd;
            std::mt19937 generator(rd());

            if (dist == UNIFORM)
            {
                std::uniform_real_distribution distribution(min, max);
                for (int i = 0; i < num_samples; i++)
                {
                    result[i] = distribution(generator);
                }
            }
            else if (dist == GAUSSIAN)
            {
                std::normal_distribution<double> distribution(min, max);
                for (int i = 0; i < num_samples; i++)
                {
                    result[i] = distribution(generator);
                }
            }
            return result;
        }
    }

#endif