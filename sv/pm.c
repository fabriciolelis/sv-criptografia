/*
 * pm.cpp
 *
 *  Created on: Sep 26, 2012
 *      Author: fabricio
 */

#include "pm.h"


double calcPM(const double *userVect, double user_codebook[][12]){
	int i,k;
	double distance=0.0;
	double minDistance = MIN_DISTANCE_INITIAL;

	for(k=0; k<CBOOK_VECT_SIZE; k++){
		double eucDistance=0.0;
		for(i=0; i<MEL_VECT_SIZE; i++){
			eucDistance += (userVect[i] - user_codebook[k][i])*(userVect[i] - user_codebook[k][i]);
		}
		if(eucDistance <= minDistance){
			minDistance = eucDistance;
		}
	}
	distance = (minDistance)/MEL_VECT_SIZE;

	return distance;
}
