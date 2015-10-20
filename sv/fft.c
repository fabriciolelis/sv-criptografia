/*
 * fft.cpp
 *
 *  Created on: Sep 26, 2012
 *      Author: fabricio
 */

#include "fft.h"


void fft_init(){

	int k;
	for(k=0; k<NUMBER_FFT_POINTS; k++){
		Xr[k] = 0.0;
		Xi[k] = 0.0;
	}

}

void fft_receiveWin(const double windowOut[]){
	int i;
	for(i=0; i < WINDOW_SIZE; i++){
		Xr[i]= windowOut[i];
	}

}

void mixInput(){

	// Embaralhamento da sequencia de entrada

	double Tr, Ti, Ui; // var auxiliares para o calculo da FFT
	int n3; // var aux para o calculo da FFT
	int j = 0;
	int k,i;

	Tr = Ti = Ui = 0.0;
	n3 = NUMBER_FFT_POINTS / 2;


	// comeco do "for"
	for(i=0; i<NUMBER_FFT_POINTS-1; i++){
		if(i < j){
			Tr = Xr[j];
			Xr[j] = Xr[i];
			Xr[i] = Tr;
			Ti = Xi[j];      // para seq.complexas
			Xi[j] = Xi[i];
			Xi[i] = Ti;
		}
		int k = n3;
		while(k <= j)
		{
			j -= k;
			k /= 2;
		}
		j += k;
	}     // fim do "for" (e do embaralhamento)
}

//-------------------------------
// Calculo da FFT
//-------------------------------
void calcFFT(double *fMagnitude){

	int k,i,j;
	int ip, n1, n2; // var aux para o calculo da FFT
	double Tr, Ti, Ur, Ui, Wr, Wi, b; // var auxiliares para o calculo da FFT

	for(k=1; k<=FFT_SIZE; k++){ // "for 1"
		n1 = 1;
		for(i=1; i<=k; i++) // equivale a fazer 2**k
			n1 *= 2;
		n2 = n1 / 2;
		Ur = 1.0;
		Ui = 0.0;
		b = PI / n2;                  // M_PI=3.1415... (definido em <math.h>)
		Wr = cos(b);
		Wi = -sin(b);

		for(j=0; j<n2; j++){ // "for 2"
			for(i=j; i<NUMBER_FFT_POINTS; i+=n1){ // "for 3"
				ip = i + n2;
				Tr = Xr[ip] * Ur - Xi[ip] * Ui;
				Ti = Xr[ip] * Ui + Xi[ip] * Ur;
				Xr[ip] = Xr[i] - Tr;
				Xi[ip] = Xi[i] - Ti;
				Xr[i] =  Xr[i] + Tr;
				Xi[i]  = Xi[i] + Ti;
			}     // fim "for 3"
			Tr = Ur;
			Ur = Ur * Wr - Ui * Wi;
			Ui = Tr * Wi + Ui * Wr;
		}        // fim "for 2"
	}           // fim "for 1"

	for(i=0; i<NUMBER_FFT_POINTS; i++){
		fMagnitude[i] = sqrt(pow(Xr[i],2) + pow(Xi[i],2));
	}
}
