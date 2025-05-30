/*
Programmer: Connor Fricke (cd.fricke23@gmail.com)
File: DSP.h
Latest Revision: 19-Feb-2024
Synopsis: Header and implementation file for DSP functions
*/

#ifndef DSP_H
#define DSP_H

    #include <random>
    #include <complex>
    #include <vector>
    #include <cmath>

    // DECLARATIONS
    namespace dsp {

        struct SignalComponent
        {
            double coeff=1.0;
            double freq=1.0;
            double phase=0.0;
        };

        typedef std::complex<double> dcomp;

        /*
        getRandomFloat(const double, const double):
            Simply returns a single random number generated with the Mersenne Twister (mt19937)
            over a uniform distribution between the upper and lower bounds parameters.
            @@ parameters:
                const double lower: lower bound to uniform distribution
                const double upper: upper bound to uniform distribution
        */
        double getRandomFloat(const double lower, const double upper);

        /*
        generateSignal(vector<double>& signal, const vector<SignalComponent>&):
            creates a signal with a Fourier sine series expansion method from frequency and coefficients 
            stored by the vector parameter. Discretizes output signal over x_values vector parameter.
            @@ parameters:
                const vector<double>& x_values: discretization vector, i.e. calculate the signal value at these x_values
                const vector<SignalComponent>& components: sine wave signal components, with their frequency and coefficient stored in a struct
            @@ return:
                vector<double> signal: output of function, a signal generated based on signal components
        */
        std::vector<double> generateSignal(const std::vector<double> &x_values, const std::vector<SignalComponent> &components);

        /*
        bool noAliasing(const vector<SignalComponent>& signal, const double SAMPLING_RATE):
            prevents aliasing by returning true if the highest frequency component of the signal is less than the NYQUIST rate
        */
        bool noAliasing(const std::vector<SignalComponent>& signal, const double SAMPLING_RATE);

        /*
        generateTiming(const double SAMPLING_RATE, const int N)
            generates a vector of time values for samples based on a sampling rate and signal duration 
            in terms of number of samples (N)
        */
        std::vector<double> generateTiming(const double SAMPLING_RATE, const int N);

        /*
        vector<double> decimateSignal(const vector<double> signal, const int DECIMATION_FACTOR)
            takes every n-th element of the input vector and adds it to the returned vector of resulting floor(len(signal)/n)
        */
        std::vector<double> decimateSignal(const std::vector<double> signal, const int DECIMATION_FACTOR);

        /*
        double aliasesTo(const double SIGNAL_FREQ, const double SAMPLING_RATE)
            returns the expected aliased frequency of a signal given a sampling rate lower than the signal frequency.
        */
        double aliasesTo(const double SIGNAL_FREQ, const double SAMPLING_RATE);

        /*
        DFT(const vector<double>&, const vector<double>&, vector<dcomp> out):
            Takes the discrete Fourier Transform of a signal for discrete k values.
            @@ parameters:
                const vector<double>& x: input signal to be transformed
                const vector<int>& k_range: represents the discrete values of k for the DFT to be calculated over. must range from -N/2 to N/2
            @@ return:
                vector<complex<double>>& output: resulting transformed signal. each value is a complex number with real and imaginary components.
        */
        std::vector<dcomp> DFT(const std::vector<double> &x, const std::vector<int> &k_range);

        /*
        lowpassFIR(const vector<double>&, vector<double>&, const double):
            Takes in an input vector and output vector as parameters. Applies an FIR Filter
            to the input vector characterized by the difference equation:
                y[n] = a*x[n] + (1-a)*x[n-1]
            @@ parameters:
                const vector<double>& input: represents an unfiltered signal
                const double alpha: defines both taps (coefficients) of the FIR filter via {a, 1-a}.
            @@ return:
                vector<double> output: resulting filtered signal
        */
        std::vector<double> lowpassFIR(const std::vector<double> &input, const double alpha);

        /*
        movingAvgIIR(const vector<double>&, vector<double>&, const double) :
            Takes in an input vector and output vector as parameters. Applies an IIR Filter
            to the input vector characterized by the difference equation:
                y[n] = a*x[n] + (1-a)*y[n-1]
            This filter corresponds to a lowpass filter with two taps defined by the third parameter value.
            It is well known as an exponential averaging filter.
            @@ parameters:
                const vector<double>& input: represents an unfiltered signal
                const double alpha: defines both taps (coefficients) of the IIR filter via {a, 1-a}
            @@ return:
                vector<double>& output: resulting filtered signal
        */
        std::vector<double> movingAvgIIR(const std::vector<double>& input, const double alpha);

        /* 
        goertzel_IIR(const vector<double>&, const int):
            Implements the IIR portion of the Goertzel algorithm. Takes in an input sequence x[n] and 
            first created an intermediate sequence s[n] to then be passed to the FIR portion of the cascade
                s[n] = x[n] + 2cos()s[n-1] - s[n-2]
            The result of this IIR filter is passed to the FIR portion in this function, and the result
            (a complex number whose magnitude is the discrete DFT value for the particular k) is returned.
            This implementation is most similar to a hardware (FPGA) implementation of the Goertzel filter.
            @@ parameters:
                const vector<double>& input: input signal to be passed through the filter
                const int k: used in the difference equation
            @@ return:
                complex<double> out: result of Goertzel filtering on input signal
        */
        dcomp goertzelIIR(const std::vector<double> &input, const int k);
    }

    // DEFINITIONS
    namespace dsp {

        extern const double PI = 2 * asin(1);

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
                    currentValue += c.coeff * sin((2.0 * PI * c.freq * t) + c.phase);
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
            while (abs(aliasedFreq) > nyquist)
            {
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

        std::vector<double> lowpassFIR(const std::vector<double> &input, const double alpha)
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

        std::vector<double> movingAvgIIR(const std::vector<double> &input, const double alpha)
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

        dcomp goertzelIIR(const std::vector<double> &x, const int k)
        {
            std::vector<double> s;
            int N = x.size();
            double COS = cos(2 * PI * double(k) / double(N));
            double SIN = sin(2 * PI * double(k) / double(N));

            double drs0 = 0; // s[n]
            double drs1 = 0; // s[n-1]
            double drs2 = 0; // s[n-2]

            // run IIR filter up through x[N-1] term
            for (int n = 0; n < N; n++)
            {
                drs0 = x[n] + (2 * COS * drs1) - drs2;
                s.push_back(drs0); // s[n] = drs0
                drs2 = drs1;
                drs1 = drs0;
            }

            // s[N] = 2cos()s[N-1] - s[N-2] assuming x[N] = 0
            double s_N = 2 * COS * s[N - 1] - s[N - 2];

            // y[N] = s[N] - cos()s[N-1] + isin()s[N-1]
            double real = s_N - (COS * s[N - 1]);
            double imag = SIN * s[N - 1];
            return dcomp(real, imag);
        }
    }

#endif