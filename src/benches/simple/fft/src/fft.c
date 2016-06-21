/*****************************************************************************
 * fft: Fast Fourier Transform.
 *****************************************************************************
 * Source: http://web.mit.edu/~emin/Desktop/ref_to_emin/www.old/source_code/fft/index.html
 * Author: Emin Martinian
 *
 * INPUT: float input[16], float output[16]
 * OUTPUT: none
 * EFFECTS:  Places the 16 point fft of input in output in a strange
 * order using 10 real multiplies and 79 real adds.
 * Re{F[0]}= out0
 * Im{F[0]}= 0
 * Re{F[1]}= out8
 * Im{F[1]}= out12
 * Re{F[2]}= out4
 * Im{F[2]}= -out6
 * Re{F[3]}= out11
 * Im{F[3]}= -out15
 * Re{F[4]}= out2
 * Im{F[4]}= -out3
 * Re{F[5]}= out10
 * Im{F[5]}= out14
 * Re{F[6]}= out5
 * Im{F[6]}= -out7
 * Re{F[7]}= out9
 * Im{F[7]}= -out13
 * Re{F[8]}= out1
 * Im{F[8]}=0
 *
 * F[9] through F[15] can be found by using the formula
 * Re{F[n]}=Re{F[(16-n)mod16]} and Im{F[n]}= -Im{F[(16-n)mod16]}
 * Note using temporary variables to store intermediate computations
 * in the butterflies might speed things up.  When the current version
 * needs to compute a=a+b, and b=a-b, I do a=a+b followed by b=a-b-b.
 *
 * So practically everything is done in place, but the number of adds
 * can be reduced by doinc c=a+b followed by b=a-b.
 * The algorithm behind this program is to find F[2k] and F[4k+1]
 * seperately.  To find F[2k] we take the 8 point Real FFT of x[n]+x[n+8]
 * for n from 0 to 7.  To find F[4k+1] we take the 4 point Complex FFT of
 * exp(-2*pi*j*n/16)*{x[n] - x[n+8] + j(x[n+12]-x[n+4])} for n from 0 to 3.
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define SIN_2PI_16   0.38268343236508978
#define SIN_4PI_16   0.707106781186547460
#define SIN_6PI_16   0.923879532511286740
#define C_P_S_2PI_16 1.30656296487637660
#define C_M_S_2PI_16 0.54119610014619690
#define C_P_S_6PI_16 1.3065629648763766
#define C_M_S_6PI_16 -0.54119610014619690

float output[16];

void __attribute__ ((noinline)) R16SRFFT(float *input)
{
    float temp;
    float out0, out1, out2, out3, out4, out5, out6, out7, out8;
    float out9, out10, out11, out12, out13, out14, out15;

    // output[0 through 7] is the data that we take the 8 point real FFT of
    float in_0  = input[0];
    float in_1  = input[1];
    float in_2  = input[2];
    float in_3  = input[3];
    float in_4  = input[4];
    float in_5  = input[5];
    float in_6  = input[6];
    float in_7  = input[7];
    float in_8  = input[8];
    float in_9  = input[9];
    float in_10 = input[10];
    float in_11 = input[11];
    float in_12 = input[12];
    float in_13 = input[13];
    float in_14 = input[14];
    float in_15 = input[15];

    out0 = in_0 + in_8;
    out1 = in_1 + in_9;
    out2 = in_2 + in_10;
    out3 = in_3 + in_11;
    out4 = in_4 + in_12;
    out5 = in_5 + in_13;
    out6 = in_6 + in_14;
    out7 = in_7 + in_15;

    // inputs 8,9,10,11 are the Real part of the 4 point Complex FFT inputs
    out8  = in_0 - in_8;
    out9  = in_1 - in_9;
    out10 = in_2 - in_10;
    out11 = in_3 - in_11;

    // outputs 12,13,14,15 are the Imaginary pars of the 4 point Complex FFT inputs
    out12 = in_12 - in_4;
    out13 = in_13 - in_5;
    out14 = in_14 - in_6;
    out15 = in_15 - in_7;

    /* First we do the "twiddle factor" multiplies for the 4 point CFFT */
    /* Note that we use the following handy trick for doing a complex */
    /* multiply:  (e + jf) = (a + jb) * (c + jd) */
    /*            e = (a - b) * d + a * (c - d)   and    f = (a - b) * d + b * (c + d) */

    /* C_M_S_2PI/16=cos(2pi/16)-sin(2pi/16) when replaced by macroexpansion */
    /* C_P_S_2PI/16=cos(2pi/16)+sin(2pi/16) when replaced by macroexpansion */
    /* (SIN_2PI_16)=sin(2pi/16) when replaced by macroexpansion */
    temp  = (out13 - out9) * SIN_2PI_16;
    out9  = out9 * C_P_S_2PI_16 + temp;
    out13 = out13 * C_M_S_2PI_16 + temp;

    out14 *= SIN_4PI_16;
    out10 *= SIN_4PI_16;
    out14 =  out14 - out10;
    out10 =  out14 + out10 + out10;

    temp  = (out15 - out11) * SIN_6PI_16;
    out11 = out11 * C_P_S_6PI_16 + temp;
    out15 = out15 * C_M_S_6PI_16 + temp;

    /* The following are the first set of two point butterfiles */
    /* for the 4 point CFFT */

    out8 += out10;
    out10 = out8 - out10 - out10;

    out12 += out14;
    out14 = out12 - out14 - out14;

    out9 += out11;
    out11 = out9 - out11 - out11;

    out13 += out15;
    out15 = out13 - out15 - out15;

    // The followin are the final set of two point butterflies
    output[1] = out8 + out9;
    output[7] = out8 - out9;

    output[9]  = out12 + out13;
    output[15] = out13 - out12;

    output[5]  = out10 + out15;  // implicit multiplies by
    output[13] = out14 - out11;  // a twiddle factor of -j
    output[3]  = out10 - out15;  // implicit multiplies by
    output[11] = -out14 - out11; // a twiddle factor of -j


    /* What follows is the 8-point FFT of points output[0-7] */
    /* This 8-point FFT is basically a Decimation in Frequency FFT */
    /* where we take advantage of the fact that the initial data is real*/

    // First set of 2-point butterflies
    out0 = out0 + out4;
    out4 = out0 - out4 - out4;
    out1 = out1 + out5;
    out5 = out1 - out5 - out5;
    out2+= out6;
    out6 = out2 - out6 - out6;
    out3+= out7;
    out7 = out3 - out7 - out7;

    // Computations to find X[0], X[4], X[6]
    output[0] = out0 + out2;
    output[4] = out0 - out2;
    out1 += out3;
    output[12] = out3 + out3 - out1;

    output[0] += out1;  // Real Part of X[0]
    output[8] = output[0] - out1 - out1; // Real Part of X[4]
    /* out2 = Real Part of X[6] */
    /* out3 = Imag Part of X[6] */

    // Computations to find X[5], X[7]
    out5 *= SIN_4PI_16;
    out7 *= SIN_4PI_16;
    out5 = out5 - out7;
    out7 = out5 + out7 + out7;

    output[14] =  out6 - out7; /* Imag Part of X[5] */
    output[2]  =  out5 + out4; /* Real Part of X[7] */
    output[6]  =  out4 - out5; /*Real Part of X[5] */
    output[10] = -out7 - out6; /* Imag Part of X[7] */

}

void computation(float input[16])
{
}

int main(int argc, char *argv[])
{
    float data[16];
    float zero=0;

    // get intput
    if(argc != 17) {
        printf("16 point vector should be supplied!\n");
        return 1;
    } else {
        int i;
        for (i = 0; i < 16; i ++) {
            data[i] = atof(argv[i + 1]);
        }
    }

    long i = 0;
    for (; i < 50000000; i++) {
        // for perf tests
        R16SRFFT(data);
    }

    // print results
    printf("\nresult is:\n");
    printf("k,\t\tReal Part\t\tImaginary Part\n");
    printf(" 0\t\t%.9f\t\t%.9f\n", output[0], zero);
    printf(" 1\t\t%.9f\t\t%.9f\n", output[1], output[9]);
    printf(" 2\t\t%.9f\t\t%.9f\n", output[2], output[10]);
    printf(" 3\t\t%.9f\t\t%.9f\n", output[3], output[11]);
    printf(" 4\t\t%.9f\t\t%.9f\n", output[4], output[12]);
    printf(" 5\t\t%.9f\t\t%.9f\n", output[5], output[13]);
    printf(" 6\t\t%.9f\t\t%.9f\n", output[6], output[14]);
    printf(" 7\t\t%.9f\t\t%.9f\n", output[7], output[15]);
    printf(" 8\t\t%.9f\t\t%.9f\n", output[8], zero);
    printf(" 9\t\t%.9f\t\t%.9f\n", output[7], -output[15]);
    printf("10\t\t%.9f\t\t%.9f\n", output[6], -output[14]);
    printf("11\t\t%.9f\t\t%.9f\n", output[5], -output[13]);
    printf("12\t\t%.9f\t\t%.9f\n", output[4], -output[12]);
    printf("13\t\t%.9f\t\t%.9f\n", output[3], -output[11]);
    printf("14\t\t%.9f\t\t%.9f\n", output[2], -output[9]);
    printf("15\t\t%.9f\t\t%.9f\n", output[1], -output[8]);
    return 0;
}
