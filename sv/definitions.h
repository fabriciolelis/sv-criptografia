/*
 * definitions.h
 *
 *  Created on: Sep 26, 2012
 *      Author: fabricio
 */

#ifndef definitions
#define definitions

#define WINDOW_SIZE 256 //trocar para 220 ???
#define SUB_WINDOW_SIZE 128 //trocar para 110 ???
#define LOWER_ENERGY 0.001577
#define HIGHER_ENERGY 0.007760
#define START_TIME 25
#define END_TIME 31
#define PI 3.14159265358979323846  //3.14159265358979
#define CBOOK_VECT_SIZE 64
#define SAMPLE_MAX 32768 //Corresponding to 8000h for normalisation
#define WAV_HEADER 22 // Corresponding a Wave file header
#define FFT_SIZE 8
#define NUMBER_FFT_POINTS 256
#define SAMPLE_RATE 11025
#define MEL_VECT_SIZE 12
#define NUM_MEL_COEFFS 13
#define LIN_FILTERS 13
#define LOG_FILTERS 23
#define NUM_FILTERS (LIN_FILTERS + LOG_FILTERS)
#define MIN_FREQ 133.33
#define STEP_LIN_FILTER 66.67
#define STEP_LOG_FILTER 1.0711703
#define TWOPI 6.28318530717958647692
#define FINAL_UPPER 0.09
#define FINAL_LOWER 0.082
#define MIN_DISTANCE_INITIAL 100000000.0

#endif
