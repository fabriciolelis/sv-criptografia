/*
 * filterbank.cpp
 *
 *  Created on: Sep 26, 2012
 *      Author: fabricio
 */


#include "filterbank.h"

void filters_init(){
	int i,k;
	for(i=0; i<NUM_FILTERS; i++){
		for(k=0; k<NUMBER_FFT_POINTS; k++){
			mffccFw[i][k] = 0.0;
		}
	}
}
void createFilterBank(){

	double center[NUM_FILTERS] = {0.0};
	double lower[NUM_FILTERS] = {0.0};
	double upper[NUM_FILTERS] = {0.0};
	double triangleHeigth[NUM_FILTERS] = {0.0};
	double fftFreq[NUMBER_FFT_POINTS] = {0.0};
	double freqs[NUM_FILTERS+2] = {0.0};
	int i,k;

	// bordas dos filtros lineares
	for(i=0; i < LIN_FILTERS; ++i){
		freqs[i] = MIN_FREQ + (STEP_LIN_FILTER * i);
	}

	//bordas dos filtros logarítmicos
	double first_log = freqs[LIN_FILTERS-1];
	for(i=1; i <= (LOG_FILTERS+2); ++i){
		freqs[LIN_FILTERS-1+i] = first_log * pow(STEP_LOG_FILTER,(double)i);
	}

	//contrução das informações do filtros triangulares
	for(i= 0; i < NUM_FILTERS; ++i){
		lower[i] = freqs[i];
	}
	for(i = 1; i <= NUM_FILTERS; ++i) {
		center[i-1] = freqs[i];
	}
	for (i = 2; i <= NUM_FILTERS+1; ++i){
		upper[i-2] = freqs[i];
	}
	for(i=0; i < NUM_FILTERS; ++i){
		triangleHeigth[i] = (double) (2.0 / (upper[i] - lower[i]));
	}

	//Frequências da FFT
	for(i=0; i < NUMBER_FFT_POINTS; ++i){
		fftFreq[i] = (double) i /(double) NUMBER_FFT_POINTS * (double) SAMPLE_RATE;
	}

	//Ponderação dos filtros
	for(k = 0; k < NUM_FILTERS; ++k){
		for(i = 0; i < NUMBER_FFT_POINTS; ++i){
			if((fftFreq[i] > lower[k]) && (fftFreq[i] <= center[k])){
				mffccFw[k][i] = triangleHeigth[k] * ((fftFreq[i]-lower[k]) / (center[k]- lower[k]));
			}
			if ((fftFreq[i] > center[k]) && (fftFreq[i] <= upper[k])){
				mffccFw[k][i] = triangleHeigth[k] * ((upper[k] - fftFreq[i]) / (upper[k] - center[k]));

			}
		}
	}
}

void applyFilters(const double *fMagnitude, double *earMag){

	int i,k;
	double sum = 0.0;
	// aplicação dos filtros (filtros ponderados x espectro da FFT) Mel Spectrum
	for(i=0; i<NUM_FILTERS; ++i){
		sum = 0.0;
		for(k=0; k<NUMBER_FFT_POINTS; k++){
			sum += (mffccFw[i][k] * fMagnitude[k]);
		}
		if(sum != 0.0){
			earMag[i] = log10(sum);
		}
		else earMag[i] = 0.0;
	}
}


