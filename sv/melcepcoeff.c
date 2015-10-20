/*
 * melcepcoeff.cpp
 *
 *  Created on: Sep 26, 2012
 *      Author: fabricio
 */

#include "melcepcoeff.h"


void calcMelCoeff(const double *earMag, FILE *melCepCoeffs){

	double melCoffs[MEL_VECT_SIZE] = {0.0};
	double mfccDCT[NUM_MEL_COEFFS][NUM_FILTERS] = {0.0};
	double scalefac = (double) (1.0 / sqrt ((double)NUM_FILTERS/2.0));
	int i,j;

	for(j=0; j < NUM_MEL_COEFFS; j++){
		for(i=0; i < NUM_FILTERS; ++i){
			mfccDCT[j][i] = scalefac * cos(j*(2*i + 1)* PI/2/NUM_FILTERS);
			if(j==0){
				mfccDCT[j][i] *= (double) (sqrt(2.0)/2.0);
			}
		}
	}
	double sum = 0.0;
	for(i=0; i < NUM_MEL_COEFFS; ++i){
		sum = 0.0;
		for(j=0; j < NUM_FILTERS; ++j){
			sum += (mfccDCT[i][j] * earMag[j]);
		}
		melCoffs[i] = sum;
	}
	for(i=0; i < MEL_VECT_SIZE; i++){
		melCoffs[i]= melCoffs[i+1];
		fwrite(&melCoffs[i], sizeof(double),1, melCepCoeffs);
	}
}
