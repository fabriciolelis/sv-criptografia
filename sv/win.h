/*
 * win.h
 *
 *  Created on: Sep 26, 2012
 *      Author: fabricio
 */


#include "definitions.h"
#include <math.h>
#ifndef WIN_H_
#define WIN_H_

//variables
double windowIn[WINDOW_SIZE];
double windowOut[WINDOW_SIZE];
short int countWin;
int availWin;


//fuctions
void win_init();
void setAvail();
void win_resetAvail();
void superPosition();
int calcWIN(double sampleIn);


#endif /* WIN_H_ */
