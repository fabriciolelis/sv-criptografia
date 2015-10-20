/*
 * win.cpp

 *
 *  Created on: Sep 26, 2012
 *      Author: fabricio
 */


#include "win.h"
#include <math.h>

void win_init(){
	int k;
	countWin = 0;
	for(k=0; k<WINDOW_SIZE; k++) {
		windowIn[k]=0.0;
		windowOut[k]=0.0;
	}
}

void setAvail(){
	availWin = 1; //true;
}

void win_resetAvail(){
	availWin = 0; //false;
}

void superPosition(){
	int k;
	for(k=0; k<(WINDOW_SIZE/2); k++){
		windowIn[k]=windowIn[k+WINDOW_SIZE/2];
	}
}

int calcWIN(double sampleIn){
	int k;

	windowIn[countWin] = sampleIn;
	countWin++;
	if(countWin==WINDOW_SIZE){
		for(k=0; k<WINDOW_SIZE; k++){
			windowOut[k]=(double)(0.54 - 0.46*cos(2*PI*k/(WINDOW_SIZE-1)))*windowIn[k];
		}
		setAvail();
		superPosition();
		countWin=WINDOW_SIZE/2;
	}
	return availWin;
}
