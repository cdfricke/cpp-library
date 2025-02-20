/*
Programmer: Connor Fricke (cd.fricke23@gmail.com)
File: DSP.cpp
Latest Revision: 4-Dec-2024
Synopsis: Implementation File for DSP function library
*/

#include "DSP.h"

extern const double PI = 2 * asin(1);

namespace DSP {
    double getRandomFloat(const double lower, const double upper)
    {
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_real_distribution<double> distribution(lower, upper);

        return distribution(generator);
    }

    std::vector<double> generateSignal(const std::vector<double> &t_values, const std::vector<SignalComponent> &components)
    {
        std::vector<double> output;
        for (double t : t_values)
        {
            double currentValue = 0.0;
            
            for (SignalComponent c : components)
            {
                // cos takes radians argument so we multiply by 2PI because freq is units of Hz
                currentValue += c.coeff * sin( (2.0 * PI * c.freq * t) + c.phase);
            }
            
            output.push_back(currentValue);
        }
        return output;
    }

    bool noAliasing(const std::vector<SignalComponent> &signal, const double SAMPLING_RATE)
    {
        double maxFreq = signal[0].freq;

        for (SignalComponent c : signal)
        {
            if (c.freq > maxFreq)
                maxFreq = c.freq;
        }
        return (maxFreq < (SAMPLING_RATE / 2.0));
    }

    std::vector<double> generateTiming(const double SAMPLING_RATE, const int N)
    {
        std::vector<double> sampleTimes;
        double t = 0.0;
        for (int i = 0; i < N; i++)
        {
            sampleTimes.push_back(t);
            t += (1.0 / SAMPLING_RATE);
        }
        return sampleTimes;
    }

    std::vector<double> decimateSignal(const std::vector<double> signal, const int DECIMATION_FACTOR)
    {
        std::vector<double> decimated;
        for (int i = 0; i < signal.size(); i += DECIMATION_FACTOR)
        {
            decimated.push_back(signal[i]);
        }
        return decimated;
    }

    double aliasesTo(const double SIGNAL_FREQ, const double SAMPLING_RATE) 
    {
        double nyquist = SAMPLING_RATE / 2.0;
        double aliasedFreq = SIGNAL_FREQ;
        while (abs(aliasedFreq) > nyquist) {
            aliasedFreq -= SAMPLING_RATE;
        } 
        return abs(aliasedFreq);
    }

    std::vector<dcomp> DFT(const std::vector<double> &x, const std::vector<int> &k_range)
    {
        std::vector<dcomp> output;
        dcomp I = -1;
        I = sqrt(I);

        // X(k) = sum_{n=0}^{N-1}{x[n]*W_N^{kn}}
        // W_N = exp(-I*2*PI/N)
        const int N = x.size();
        for (double k : k_range)
        {
            dcomp X = 0.0;
            for (int i = 0; i < N; i++)
            {
                X += x[i] * exp(-I * 2.0 * PI * k * double(i) / double(N));
            }
            output.push_back(X);
        }
        return output;
    }

    std::vector<double> LOWPASS_FIR(const std::vector<double> &input, const double alpha)
    {
        std::vector<double> output;
        double delay0 = 0;
        for (double in : input) 
        {
            double out = (alpha * in) + ((1.0 - alpha) * delay0);
            output.push_back(out);
            delay0 = in;
        }
        return output;
    }

    std::vector<double> AVERAGER_IIR(const std::vector<double> &input, const double alpha)
    {
        std::vector<double> output;
        double delay0 = 0;
        for (double in : input)
        {
            // DIFFERENCE EQUATION:
            // y[n] = a*x[n] + (1-a)*y[n-1]
            double out = (alpha * in) + ((1.0 - alpha) * delay0);
            delay0 = out;
            output.push_back(out);
        }
        return output;
    }

    dcomp goertzel_1(const std::vector<double> &input, const int k)
    {
        dcomp I = -1;
        I = sqrt(I);
        int N = input.size();

        dcomp delay0 = 0;
        dcomp out;
        for (double in : input)
        {
            // convert real number input to complex value with only real component
            dcomp c_in = std::complex<double>(in, 0.0);
            // DIFFERENCE EQUATION:
            // y[n] = x[n] + exp(i * 2 * PI * k / N) * y[n - 1]
            out = c_in + exp(I * 2.0 * PI * double(k) / double(N))*delay0;
            delay0 = out;
        }
        return out;
    }

    dcomp goertzel_2(const std::vector<double> &input, const int k)
    {
        int N = input.size();

        // IIR delay registers for re and im calculations
        double dro0_re = 0;     // Re{y[n-1]}
        double dro1_re = 0;     // Re{y[n-2]}

        double dro0_im = 0;     // Im{y[n-1]}
        double dro1_im = 0;     // Im{y[n-2]}

        // FIR delay register
        double dri = 0;         // x[n-1]

        // compute constant coefficients
        const double phase = 2.0 * PI * double(k) / double(N);
        const double COS = cos(phase);
        const double SIN = sin(phase);
        double out_Re=0, out_Im=0;

        int n = 0;
        for (double in : input)
        {
            // DIFFERENCE EQUATION:
            // Re{y[n]} = x[n] - cos(2PIk/N)*x[n-1] + 2cos(2PIk/N)*Re{y[n-1]} - Re{y[n-2]}
            // Im{y[n]} = sin(2PIk/N)*x[n-1] + 2cos(2PIk/N)*Im{y[n-1]} - Im{y[n-2]}
            out_Re = ( in ) - ( COS*dri ) + ( 2*COS*dro0_re ) - ( dro1_re );
            out_Im = ( SIN * dri ) + ( 2*COS*dro0_im ) - ( dro1_im );

            // update delay regs
            dri = in;           // x[n] -> x[n-1]

            dro1_re = dro0_re;  // Re{y[n-1]} -> Re{y[n-2]}
            dro0_re = out_Re;   // Re{y[n]} -> Re{y[n-1]}
            
            dro1_im = dro0_im;  // Im{y[n-1]} -> Im{y[n-2]}
            dro0_im = out_Im;   // Im{y[n]} -> Im{y[n-1]}
        }

        return dcomp(out_Re, out_Im);
    }

    dcomp goertzel_IIR(const std::vector<double> &x, const int k)
    {
        std::vector<double> s;
        int N = x.size();
        double COS = cos(2*PI*double(k)/double(N));
        double SIN = sin(2*PI*double(k)/double(N));
        
        double drs0 = 0;   // s[n]
        double drs1 = 0;    // s[n-1]
        double drs2 = 0;    // s[n-2]

        // run IIR filter up through x[N-1] term
        for (int n = 0; n < N; n++) {
            drs0 = x[n] + (2*COS*drs1) - drs2;
            s.push_back(drs0);  // s[n] = drs0
            drs2 = drs1;
            drs1 = drs0;
        }

        // s[N] = 2cos()s[N-1] - s[N-2] assuming x[N] = 0
        double s_N = 2*COS*s[N-1] - s[N-2];

        // y[N] = s[N] - cos()s[N-1] + isin()s[N-1]
        double real = s_N - (COS*s[N-1]);
        double imag = SIN*s[N-1];
        return dcomp(real, imag);
    }
}