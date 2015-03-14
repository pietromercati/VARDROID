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

// Sched for affinity macros
#define  __USE_GNU

#define DEBUG // enables printf for debugging

// Macros for ioctl
#define SELECT_BUBBLE 	1
#define BUBBLE_LENGTH	3
#define READ_JIFFIES	4

// stuff for set thread affinity
#define CPU_SETSIZE 1024
#define __NCPUBITS  (8 * sizeof (unsigned long))
typedef struct
{
   unsigned long __bits[CPU_SETSIZE / __NCPUBITS];
} cpu_set_t;

#define CPU_SET(cpu, cpusetp) \
  ((cpusetp)->__bits[(cpu)/__NCPUBITS] |= (1UL << ((cpu) % __NCPUBITS)))
#define CPU_ZERO(cpusetp) \
  memset((cpusetp), 0, sizeof(cpu_set_t))

void setCurrentThreadAffinityMask(cpu_set_t mask)
{
    int err, syscallres;
    pid_t pid = gettid();
    syscallres = syscall(__NR_sched_setaffinity, pid, sizeof(mask), &mask);
    if (syscallres)
    {
        err = errno;
        printf("Error in the syscall setaffinity: mask=%d=0x%x err=%d=0x%x", mask, mask, err, err);
    }
}

#define NANO_SECOND_MULTIPLIER  1000000  // 1 millisecond = 1,000,000 Nanoseconds

#define LENGTH_SCHED            1000000   // 50000000 is ~1 sec exec time with 2 little cores runnign at maw freq


// ---------------- MAIN FUNCTION ----------------- //

int main(int argc, char ** argv){
	int i;
	int j;
	unsigned int a = 0;
	unsigned int b;
	double d;
	double g;
	long unsigned int c4, c3, c2, c1;
	double time_spent_low,time_spent_medium, time_spent_high, time_spent_tot;


        struct timespec sleepValue = {0};
	int sleep_duration = 1;
        sleepValue.tv_nsec = sleep_duration*NANO_SECOND_MULTIPLIER;


		c1 = clock();

		// low utilization
		printf("Starting low cycle......\n");

                for ( i = 1; i < 5000 ; i++ ){
                        nanosleep(&sleepValue, NULL);
                        for (j = 1; j < 100 ; j ++){
                                a = a + 1;
                        }
                }

		c2 = clock();

		// low utilization
		printf("Starting medium cycle......\n");
	
		for ( i = 1; i < 1000 ; i++ ){
                	nanosleep(&sleepValue, NULL);
			for (j = 1; j < 500000 ; j ++){
				a = a + 1; 
			}
		}
	

		c3 = clock();

		// high utilization 
		
		printf("Starting high cycle......\n");
		for (j = 0 ; j < 100000000 ; j ++){
			d = g*d;
			g = d*d;
		}

		c4 = clock();

		time_spent_low = (double)(c2 - c1) / CLOCKS_PER_SEC;	        	
		time_spent_medium = (double)(c3 - c2) / CLOCKS_PER_SEC;	        	
		time_spent_high = (double)(c4 - c3) / CLOCKS_PER_SEC;
		time_spent_tot = (double)(c4 - c1) / CLOCKS_PER_SEC;	        	
		printf("%f %f %f %f\n", time_spent_low, time_spent_medium, time_spent_high, time_spent_tot );
	return 0;
} 











