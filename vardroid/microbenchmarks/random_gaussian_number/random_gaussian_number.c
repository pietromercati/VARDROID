// --------------- VARDROID PROJECT - BUBBLE SELECTOR -----------------
/*
 * 
 * Author: Andrea Bartolini
 *
 * Modified by : Pietro Mercati
 * email : pimercat@eng.ucsd.edu
 * 
 * If using this code for research purposes, include 
 * references to the following publications
 * 
 * 1) P.Mercati, A. Bartolini, F. Paterna, T. Rosing and L. Benini; A Linux-governor based 
 *    Dynamic Reliability Manager for android mobile devices. DATE 2014.
 * 2) P.Mercati, A. Bartolini, F. Paterna, L. Benini and T. Rosing; An On-line Reliability 
 *    Emulation Framework. EUC 2014
 * 
	This file is part of DRM.
        DRM is free software: you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation, either version 3 of the License, or
        (at your option) any later version.

        DRM is distributed in the hope that it will be useful,
        but WITHOUT ANY WARRANTY; without even the implied warranty of
        MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
        GNU General Public License for more details.

        You should have received a copy of the GNU General Public License
        along with DRM.  If not, see <http://www.gnu.org/licenses/>.


*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sched.h>
#include <unistd.h>
#include <string.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <sys/wait.h> //for wait()

#include <math.h>


double randn (double mu, double sigma){
	  double U1, U2, W, mult;
	  static double X1, X2;
	  static int call = 0;

	  if (call == 1)
	    {
	      call = !call;
	      return (mu + sigma * (double) X2);
	    }

	  do
	    {
	      U1 = -1 + ((double) rand () / RAND_MAX) * 2;
	      U2 = -1 + ((double) rand () / RAND_MAX) * 2;
	      W = pow (U1, 2) + pow (U2, 2);
	    }
	  while (W >= 1 || W == 0);

	  mult = sqrt ((-2 * log (W)) / W);
	  X1 = U1 * mult;
	  X2 = U2 * mult;

	  call = !call;

	  return (mu + sigma * (double) X1);
	}



// ---------------- MAIN FUNCTION ----------------- //

int main(int argc, char ** argv){
	char *p1;
	char *p2;
	char *p3;
	double num;
	double mu;
	double sigma;
	int i;
	int iter;

	if(argc < 4){
		printf("please provide mu, sigma and number of iterations\n");
		exit(0);
	}

	mu 		= strtol(argv[1], &p1, 10);
	sigma 		= strtol(argv[2], &p2, 10);
	iter		= strtol(argv[3], &p3, 10);

	for (i = 0 ; i < iter ; i++){
		num = randn(mu, sigma);
		printf("%d\n",(int)num);
	}
	return 0;
} 











