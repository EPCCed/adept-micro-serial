/* Copyright (c) 2015 The University of Edinburgh. */

/* 
* This software was developed as part of the                       
* EC FP7 funded project Adept (Project ID: 610490)                 
* www.adept-project.eu                                            
*/

/* Licensed under the Apache License, Version 2.0 (the "License"); */
/* you may not use this file except in compliance with the License. */
/* You may obtain a copy of the License at */

/*     http://www.apache.org/licenses/LICENSE-2.0 */

/* Unless required by applicable law or agreed to in writing, software */
/* distributed under the License is distributed on an "AS IS" BASIS, */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. */
/* See the License for the specific language governing permissions and */
/* limitations under the License. */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <limits.h>

#include "utils.h"

/*
 *
 * Core of the Pi approximation calculation
 *
 */
double piapprox(int index, unsigned int n){

	double approx;
	double nn = (double) n;
	double idx = (double) index;
	approx = 1.0 / (1.0 + (idx-0.5)/nn * (idx-0.5)/nn);
	return approx;
}


/*
 *
 * Recursive core of the Pi approximation calculation
 *
 */
double recur_piapprox(int index, unsigned int n){

	double result;
	double approx;
	double nn = (double) n;
	double idx = (double) index;

	if (idx == 1) result = 1.0 / (1.0 + (idx-0.5)/nn * (idx-0.5)/nn);
	else{
		approx = 1.0 / (1.0 + (idx-0.5)/nn * (idx-0.5)/nn);
		result = approx + recur_piapprox(idx-1, nn);
	}
	return result;
}


/*
 *
 * Calculate the overhead of function calls.
 *
 * Compute an approximate value of Pi - first by doing
 * the calculation directly inside a for loop, and
 * then by using a function to do the calculation. The
 * difference in time and power usage between the two
 * methods is the overhead added by the function calls.
 *
 */
int function_calls(unsigned int N){

	double pi, exactpi;
	int i;
	double idx, nn;

	struct timespec start, end;

	pi = 0.0;
	nn = (double) N;

	/* warm-up */
	for (i=1; i<=1000; i++){
		idx = (double) i;
		pi = pi + 1.0 / (1.0 + (idx-0.5)/(1000.0) * (idx-0.5)/(1000.0) );
	}

	clock_gettime(CLOCK, &start);

	/* loop to calculate approximate value of pi directly */
	for (i=N; i>=1; i--){
		idx = (double) i;
		pi = pi + 1.0 / (1.0 + (idx-0.5)/nn * (idx-0.5)/nn);
	}

	clock_gettime(CLOCK, &end);
	elapsed_time_hr(start, end, "No Function Calls");

	pi = pi * 4.0 / nn;
	exactpi = 4.0 * atan(1.0);

	/* verify result */
	printf("pi = %f, %% error = %f\n", pi, fabs(100.0*(pi-exactpi)/exactpi));

	/* allow things to settle down */
	sleep(5);

	pi=0.0;

	/* warm-up */
	for (i=1; i<=1000; i++){
		pi = pi + piapprox(i, 1000);
	}

	clock_gettime(CLOCK, &start);

	/* loop to calculate approximate value of pi using a function */
	for (i=N; i>=1; i--){
		pi = pi + piapprox(i, N);
	}

	clock_gettime(CLOCK, &end);
	elapsed_time_hr(start, end, "Function Calls");

	pi = pi * 4.0 / nn;
	exactpi = 4.0 * atan(1.0);

	/* verify result */
	printf("pi = %f, %% error = %f\n", pi, fabs(100.0*(pi-exactpi)/exactpi));

	return 0;

}

int function_calls_recursive(unsigned int N){

	double pi, exactpi;
	int i = 0, j = 0, rep = N;
	double idx, nn;

	struct timespec start, end;

	if(N > 10000) {
		rep = N/10000;
		N = 10000;
	}

	nn = (double) N;


	/* warm-up */
	for (i=1; i<=1000; i++){
		idx = (double) i;
		pi = pi + 1.0 / (1.0 + (idx-0.5)/1000.0 * (idx-0.5)/1000.0);
	}

	clock_gettime(CLOCK, &start);

	for(j=0;j<rep;j++){
		pi = 0.0;
		/* loop to calculate approximate value of pi directly */
		for (i=N; i>=1; i--){
			idx = (double) i;
			pi = pi + 1.0 / (1.0 + (idx-0.5)/nn * (idx-0.5)/nn);
		}
	}

	clock_gettime(CLOCK, &end);
	elapsed_time_hr(start, end, "No Function Calls");

	pi = pi * 4.0 / nn;
	exactpi = 4.0 * atan(1.0);

	/* verify result */
	printf("pi = %f, %% error = %f\n", pi, fabs(100.0*(pi-exactpi)/exactpi));

	/* allow things to settle down */
	sleep(5);

	pi=0.0;

	/* warm-up */
	for (i=1; i<=1000; i++){
		pi = pi + piapprox(i, 1000);
	}

	clock_gettime(CLOCK, &start);

	for(j=0;j<rep;j++){
		pi = 0.0;
		/* loop to calculate approximate vaule of pi using a function */
		for (i=N; i>=1; i--){
			pi = pi + piapprox(i, N);
		}
	}
	clock_gettime(CLOCK, &end);
	elapsed_time_hr(start, end, "Function Calls");

	pi = pi * 4.0 / nn;
	exactpi = 4.0 * atan(1.0);

	/* verify result */
	printf("pi = %f, %% error = %f\n", pi, fabs(100.0*(pi-exactpi)/exactpi));

	/* allow things to settle down */
	sleep(5);

	/* warm-up */
	i = 1000;

	pi = recur_piapprox(i, N);

	clock_gettime(CLOCK, &start);

	/* loop to calculate approximate value of pi using a function */
	i = N;
	for(j=0;j<rep;j++){
		pi = 0.0;
		pi = recur_piapprox(i, N);
	}

	clock_gettime(CLOCK, &end);
	elapsed_time_hr(start, end, "Recursive Function Calls");

	pi = pi * 4.0 / nn;
	exactpi = 4.0 * atan(1.0);

	/* verify result */
	printf("pi = %f, %% error = %f\n", pi, fabs(100.0*(pi-exactpi)/exactpi));

	return 0;
}
