/*
 * vd.h
 *
 *  Created on: Sep 27, 2012
 *      Author: fabricio
 */

#include "definitions.h"

#ifndef VD_H_
#define VD_H_

short int countVD, countMax, countReader, countSample, countStart, countEnd;
int startFound, endFound;
int startEnd, avail;
double subWindowIn[SUB_WINDOW_SIZE];
double startSample[START_TIME][SUB_WINDOW_SIZE];
double endSample[END_TIME][SUB_WINDOW_SIZE];

void vd_init();
void changeSearch();
void resetSearch();
void setStart();
void setEnd();
void resetStart();
void resetEnd();
void sampStart();
void sampEnd();
void availSample();
void vd_resetAvail();
int vd_checkSample();
double vd_getSample();
int calcVD(double sampleIn);
void delStartSample();
void delEndSample();


#endif /* VD_H_ */
