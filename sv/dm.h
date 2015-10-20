/*
 * dm.h
 *
 *  Created on: Sep 26, 2012
 *      Author: fabricio
 */

#ifndef DM_H_
#define DM_H_

#include "definitions.h"
#include <stdio.h>

double recDistance(double distance);
void take_decision(double mean_distance, char *codebookName);

#endif /* DM_H_ */
