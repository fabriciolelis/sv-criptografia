/*
 * dm.c
 *
 *  Created on: Sep 26, 2012
 *      Author: fabricio
 */

#include "dm.h"



double recDistance(double distance){

	static double mean_distance_current = 0.0;
	static double sum_distances = 0.0;
	static int count_window = 0;

	sum_distances += distance;
	++count_window;
	mean_distance_current=(sum_distances/count_window);

	return mean_distance_current;
}


void take_decision(double mean_distance, char *codebookName){

	if(mean_distance < FINAL_LOWER){
		printf("You are accepted!!\n");
	}
	else if(mean_distance > FINAL_UPPER){
		printf("You are rejected!!\n");
	}
	else{
		printf("You are unknow! Please, repeat your vocal password!\n");
	}
}

