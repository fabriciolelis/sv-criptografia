/*
 * vd.cpp
 *
 *  Created on: Sep 27, 2012
 *      Author: fabricio
 */

#include "vd.h"

//After calculating energy we should call for init method.
void vd_init(){
	int i;
	countVD=0;
	countMax=0;
	countReader=0;
	countSample=0;
	for (i=0; i<SUB_WINDOW_SIZE; i++) subWindowIn[i]=0.0;
}

//When either start or end is found, we should call this method to change the search.
void changeSearch(){
	startEnd = !startEnd;
}

//When starting the algorithm we should reset the search in order to begin by searching the start of voice
void resetSearch(){
	startEnd = 0;//false;
}

//When the start of voice is found we should call this method.
void setStart(){
	startFound = 1;//true;
}

//When the end of voice is found we should call this method.
void setEnd(){
	endFound = 1;//true;
}

/*Method used to reset the flag that indicates when we find the start of voice. It is normally called
    when all the available samples are read*/
void resetStart(){
	startFound = 0;//false;
}

//Method used to reset the flag that indicates when we find the end of voice.
void resetEnd(){
	endFound = 0;//false;
}

//After reading all the available sample contained into startSample, this method should be called.
void sampStart(){
	int i,k;
	countStart=0;
	for (i=0; i<START_TIME; i++){
		for(k=0; k<SUB_WINDOW_SIZE; k++){
			startSample[i][k]=0.0;
		}
	}
}

//After reading all the available sample contained into endSample, this method should be called.
void sampEnd(){
	int i,k;
	countEnd=0;
	for (i=0; i<END_TIME; i++){
		for(k=0; k<SUB_WINDOW_SIZE; k++){
			endSample[i][k]=0.0;
		}
	}
}

//Avail is a flag to be set to true when samples are available.
void availSample(){
	avail = 1; // true;
}

//This method sets avail flag to false.
void vd_resetAvail(){
	avail = 0; //false;
}

int vd_checkSample(){
	if(countReader==countMax){
		vd_resetAvail();
	}

	return avail;
	//check origin of data to decide whether samples are still availlable or not.
}

double vd_getSample() {

	double sampleRead;

	if(startFound){
		sampleRead=startSample[countReader][countSample];
	}

	else {
		sampleRead=endSample[countReader][countSample];
	}

	countSample++;
	if(countSample == SUB_WINDOW_SIZE) {
		countSample = 0;
		countReader++;
	}

	return sampleRead;//return samples once their availability is confirmed.
}

/*Main method of VD class. It calculates energy of subwindows and makes decision on whether the start/end
	is found or not*/
int calcVD(double sampleIn){
	int k,j;
	double energy = 0.0;

	subWindowIn[countVD]=sampleIn;
	countVD++;

	if(countVD==SUB_WINDOW_SIZE){
		for(k=0; k<SUB_WINDOW_SIZE; k++){
			energy+=subWindowIn[k]*subWindowIn[k];
		}

		if (!startEnd){ //Searching for the start
			if(energy > HIGHER_ENERGY){
				countStart++;
				//Stocking the subwindow once its energy is higher than the higher energy threshold
				for(j=0; j<SUB_WINDOW_SIZE; j++){
					startSample[countStart-1][j]=subWindowIn[j];
				}
				vd_init();

				if(countStart>=START_TIME) {
					setStart();
					changeSearch();
					availSample();
					countMax = START_TIME;
				}
			}
			else {
				vd_init();
				sampStart();
			}
		}

		else {
			if(energy>LOWER_ENERGY){  //Sinaliza amostras prontas e zera countEnd
				countEnd++;
				for(k=0; k<SUB_WINDOW_SIZE; k++){
					endSample[countEnd-1][k]=subWindowIn[k];
				}
				availSample();
				countMax = countEnd;
			}
			else{
				countEnd++;
				for(k=0; k<SUB_WINDOW_SIZE; k++){
					endSample[countEnd-1][k]=subWindowIn[k];
				}
				vd_init();
				if (countEnd>=END_TIME){
					setEnd();
					changeSearch();
				}
			}

		}
	}
	return avail;
}
