/*  Mel Cepstrais Verification Software
 *   Author: Fabricio Lelis

 */


// Including libraries.
#include "definitions.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "vd.h"
#include "pe.h"
#include "win.h"
#include "fft.h"
#include "filterbank.h"
#include "melcepcoeff.h"
#include "pm.h"
#include "dm.h"



int main(int argc, char *argv[]){

	FILE *wavInput, *codeBook;
	FILE *melCepCoeffs;

	//Declaring variables of main
	short int sample=0;
	double sampleVD=0.0;
	double samplePE=0.0;
	double sampleCode=0.0;
	double distance=0.0;
	double meanDistance = 0.0;
	long int fileSize, filePosition=0;
	char *codebookName = malloc(256);
	double readCoeff[MEL_VECT_SIZE];
	double sampleNorm=0.0;
	char codebook_teste[100] = "cbook2VERIF_";
	double fMagnitude[NUMBER_FFT_POINTS] = {0.0};
	double earMag[NUM_FILTERS] = {0.0};
	double userVect[MEL_VECT_SIZE] = {0.0};
	double userCodebook[CBOOK_VECT_SIZE][MEL_VECT_SIZE] = {0.0};
	int i,k;


	//Opening wav file

	for(i=0; i<argc; i++){
		if(!strcmp(argv[i],"-wav")){
			if((wavInput = fopen( argv[i+1] ,"rb")) == NULL){
				printf("Wav file not found\n");
				exit(1);
			}
		}
		if(!strcmp(argv[i],"-codebook")){
			if((codebookName = argv[i+1])==NULL){
				printf("Argument [-codebook] not found\n");
				exit(1);
			}
		}
	}

	if((melCepCoeffs = fopen("coeffMEL.dat", "wb"))==NULL){
		printf("Could not open file (coeffMEL.dat)\n");
		exit(1);
	}

	sample=0;

	resetSearch();
	vd_init();
	sampStart();
	sampEnd();
	resetStart();
	resetEnd();
	vd_resetAvail();
	win_init();
	win_resetAvail();
	fft_init();
	filters_init();

	rewind(wavInput);
	fseek(wavInput,WAV_HEADER*sizeof(short int),0);


	while(!feof(wavInput)){
		fread(&sample, sizeof(short int), 1, wavInput);
		sampleNorm = ((double)sample)/SAMPLE_MAX;

		if(calcVD(sampleNorm)){
			while(vd_checkSample()){
				sampleVD = vd_getSample();
				samplePE = calc_pe(sampleVD);
				if(calcWIN(samplePE)){
					fft_receiveWin(windowOut);
					mixInput();
					calcFFT(fMagnitude);
					createFilterBank();
					applyFilters(fMagnitude, earMag);
					calcMelCoeff(earMag, melCepCoeffs);
					win_resetAvail();
					fft_init();
					filters_init();
				}
			}
			vd_resetAvail();
			vd_init();
			sampStart();
			sampEnd();
			resetStart();
			resetEnd();
		}

	}

	fclose(melCepCoeffs);

	if((codeBook = fopen(codebookName, "r"))==NULL){
		printf("Could not open codebook file\n");
		exit(1);
	}

	if((melCepCoeffs = fopen("coeffMEL.dat", "rb"))==NULL){
		printf("Could not open file (coeffMEL.dat)\n");
		exit(1);
	}

	fseek(melCepCoeffs, 0, SEEK_END);
	fileSize = ftell(melCepCoeffs)/sizeof(double);
	rewind(melCepCoeffs);


	for(k=0; k<CBOOK_VECT_SIZE; k++){
		for(i=0; i<MEL_VECT_SIZE; i++){
			fscanf(codeBook, "%lf\n", &sampleCode);
			userCodebook[k][i] = sampleCode;
		}
	}
	while(filePosition != fileSize){

		filePosition += fread(readCoeff,sizeof(double),MEL_VECT_SIZE,melCepCoeffs);
		for(i=0; i<MEL_VECT_SIZE; i++){
			userVect[i]=readCoeff[i];
		}
		distance = calcPM(userVect, userCodebook);
		meanDistance = recDistance(distance);
	}

	take_decision(meanDistance, codebookName);

	fclose(codeBook);
	fclose(wavInput);
	fclose(melCepCoeffs);
	return 0;

}
