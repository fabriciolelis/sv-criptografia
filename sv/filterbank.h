/*
 * filterbank.h
 *
 *  Created on: Sep 26, 2012
 *      Author: fabricio
 */

#ifndef FILTERBANK_H_
#define FILTERBANK_H_
#include "definitions.h"
#include <math.h>


double mffccFw[NUMBER_FFT_POINTS][NUMBER_FFT_POINTS];


void filters_init();
void createFilterBank();
void applyFilters(const double *fMagnitude, double *earMag);

#endif /* FILTERBANK_H_ */
