/*
 * fft.h
 *
 *  Created on: Sep 26, 2012
 *      Author: fabricio
 */


#ifndef FFT_H_
#define FFT_H_

#include "definitions.h"
#include <math.h>

double Xr[NUMBER_FFT_POINTS],Xi[NUMBER_FFT_POINTS];     // partes real e imaginaria da FFT


void fft_init();
void fft_receiveWin(const double windowOut[]);
void mixInput();
void calcFFT(double * fMagnitude);

#endif /* FFT_H_ */
