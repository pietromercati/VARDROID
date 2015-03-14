        
// --------------- VARDROID PROJECT - PERT ----------------
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

#include <errno.h>   // for errno
#include <limits.h>  // for INT_MAX
#include <stdlib.h>  // for strtol


// Macros for ioctl
#define SELECT_BUBBLE 		1
#define SELECT_BUBBLE_LENGTH 	3
#define READ_JIFFIES		4

#define ODROIDXU3
//#define DEBUG


#ifdef ODROIDXU3
#define NUM_CORES		8
#define NUM_CORE_L		4
#define NUM_F_L			5
#define NUM_F_b			9
#endif


// kfc frequency table (LITTLE cluster)
int freq_L[] = {1400000,1300000,1200000,1100000,1000000};

// cpu frequency table (big cluster)
int freq_b[] = {2000000, 1900000, 1800000, 1700000, 1600000, 1500000, 1400000, 1300000, 1200000};


// /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq


// ---------------- MAIN FUNCTION ----------------- //

int main(int argc, char ** argv){
	char cmd[100];
	int var;
	char *p;
	int freq_tmp;
	int freq_diff;
	int freq_max[NUM_CORES];
	int i;
	int j;
	char str[10];
	char i_str[5];	


	if (argc < 2){
		printf("please provide the performance degradation due to variability in %\n");
		exit(0);
	}

	var = strtol(argv[1],&p,10);
	
	if (var < 0){
		var = 0;
	}

	//for each core: assign a maximum frequency
	for (i = 0 ; i < NUM_CORES ; i++ ){


		if (i < NUM_CORE_L){
			freq_tmp  = freq_L[0] - 0.01*(freq_L[0]*var);
                        freq_max[i] = freq_L[NUM_F_L-1];
			for(j = 0 ; j < NUM_F_L ; j++ ){
				freq_diff = freq_L[j] - freq_tmp;
				if (freq_diff <= 0){
					freq_max[i] = freq_L[j];
					break;
				}
			} 	
		}else{
			freq_tmp  = freq_b[0] - 0.01*(freq_b[0]*var);
                        freq_max[i] = freq_b[NUM_F_b-1];
			for(j = 0 ; j < NUM_F_b ; j++ ){
				freq_diff = freq_b[j] - freq_tmp;
				if (freq_diff <= 0){
					freq_max[i] = freq_b[j];
					break;
				}
			}
		}
	}

#ifdef DEBUG	
	for (i = 0 ; i < NUM_CORES ; i++ ){
		printf("freq_max[%d] = %d, freq_tmp = %d, freq_diff = %d\n", i, freq_max[i], freq_tmp, freq_diff);
	}
#endif



	for (i = 0 ; i < NUM_CORES ; i++ ){
		
		sprintf(str, "%d", freq_max[i]);
		sprintf(i_str, "%d", i);
		
		// compose the command
		strcpy(cmd, "echo ");
		strcat(cmd, str);
		strcat(cmd, " > ");
		strcat(cmd, "/sys/devices/system/cpu/cpu");
		strcat(cmd, i_str);
		strcat(cmd,"/cpufreq/scaling_max_freq");	

		#ifdef DEBUG
		printf("CMD = %s\n", cmd);		
		#endif

		// execute the command
		system(cmd);

	}
	return 0;
} 











