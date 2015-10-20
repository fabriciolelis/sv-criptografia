
#include "pe.h"



double calc_pe(const double sample_in){

	static double  past_sample = 0.0;

	double sample_pe = (double)((double)sample_in - (double)0.9375*past_sample);
	if(sample_pe <= -1.0){
		sample_pe = -0.99999;
	}
	else if(sample_pe >= 1.0){
		sample_pe = 0.99999;
	}

	past_sample = sample_in;

	return sample_pe;


}
