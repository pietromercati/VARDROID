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
        //printf("debug funct pid = %u\n", pid);
    syscallres = syscall(__NR_sched_setaffinity, pid, sizeof(mask), &mask);
    if (syscallres)
    {
        err = errno;

    }
}

static inline unsigned int get_cyclecount (void)
{
  unsigned int value;
  // Read CCNT Register
  asm volatile ("MRC p15, 0, %0, c9, c13, 0\t\n": "=r"(value));  
  return value;
}

static inline void init_perfcounters (int32_t do_reset, int32_t enable_divider)
{
  // in general enable all counters (including cycle counter)
  int32_t value = 1;

  // peform reset:  
  if (do_reset)
  {
    value |= 2;     // reset all counters to zero.
    value |= 4;     // reset cycle counter to zero.
  } 

  if (enable_divider)
    value |= 8;     // enable "by 64" divider for CCNT.

  value |= 16;

  // program the performance-counter control-register:
  asm volatile ("MCR p15, 0, %0, c9, c12, 0\t\n" :: "r"(value));  

  // enable all counters:  
  asm volatile ("MCR p15, 0, %0, c9, c12, 1\t\n" :: "r"(0x8000000f));  

  // clear overflows:
  asm volatile ("MCR p15, 0, %0, c9, c12, 3\t\n" :: "r"(0x8000000f));
}


// ---------------- MAIN FUNCTION ----------------- //

int main(int argc, char ** argv){
	pid_t pidM ; 	
	int target_cpu = 0;
	int i;
	int j;
	int k;
	int a = 0;
	cpu_set_t mask;
	long unsigned int c2, c1;
	double time_spent;
	// fork a process

	//init_perfcounters (1, 0); 
		

		pidM = fork();

                if (pidM != 0) {
                        //printf("debug : pidM = %u\n", pidM);
					
			for (k = 0 ; k < 100 ; k ++){
				c1 = clock();

		        	// execute some code
				for (i = 0 ; i < 100 ; i ++){
					for (j = 0 ; j < 1000000 ; j ++)
					//asm volatile ("add r3, r3");
					a = a + 1;
				}
				c2 = clock();
			
				time_spent = (double)(c2 - c1) / CLOCKS_PER_SEC;	        	
				printf("Execution Time = %f\n", time_spent );
			}
                        
                }
                else{
                        CPU_ZERO(&mask);
                        CPU_SET(target_cpu, &mask);
                        setCurrentThreadAffinityMask(mask);
                }
	
	return 0;
} 











