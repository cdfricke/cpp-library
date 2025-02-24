/*
Programmer: Connor Fricke (cd.fricke23@gmail.com)
File: Stats.cpp
Latest Revision: 22-Feb-2024
Synopsis: Implementation File for statistics routines
*/

#include "Stats.h"

namespace stats
{
    // 1D and 2D SUM()
    double sum(std::vector<double> arr)
    {
        double sum = 0.0;
        for (double x : arr) {
            sum += x;
        }
        return sum;
    }
    std::vector<double> sum(std::vector<std::vector<double>> mat, const int axis)
    {
        int rows = mat.size();
        int cols = mat[0].size();

        if (axis == 0) {
            std::vector<double> sums(cols, 0.0);
            for (int j = 0; j < cols; j++) {
                for (int i = 0; i < rows; i++) {
                    sums[j] += mat[i][j];
                }
            }
            return sums;
        }
        else if (axis == 1) {
            std::vector<double> sums(rows, 0.0);
            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    sums[i] += mat[i][j];
                }
            }
            return sums;
        }
        else return {};
    }

    // 1D and 2D MEAN()
    double mean(std::vector<double> arr) {
        return sum(arr) / arr.size();
    }
    std::vector<double> mean(std::vector<std::vector<double>> mat, const int axis) {
        int rows = mat.size();
        int cols = mat[0].size();

        std::vector<double> means = sum(mat, axis);
        for (double& x : means) {
            if (axis == 0) {x /= rows;}
            else if (axis == 1) {x /= cols;}
        }
        return means;
    }

    // 1D and 2D STDEV()
    double stdev(std::vector<double> arr) {
        /* TODO : IMPLEMENT */
        return 0.0;
    }
    std::vector<double> stdev(std::vector<std::vector<double>> mat, const int axis) {
        /* TODO : IMPLEMENT */
        return {};
    }

    

    /* DISTRIBUTION GENERATORS */
    std::vector<double> realDistribution(const int dist, const int num_samples, const double min, const double max) {
        std::vector<double> result(num_samples, 0.0);

        std::random_device rd;
        std::mt19937 generator(rd());

        if (dist == UNIFORM) {
            std::uniform_real_distribution distribution(min, max);
            for (int i = 0; i < num_samples; i++) {
                result[i] = distribution(generator);
            }
        }
        else if (dist == GAUSSIAN) {
            std::normal_distribution<double> distribution(min, max);
            for (int i = 0; i < num_samples; i++) {
                result[i] = distribution(generator);
            }
        }
        return result;
    }
}
