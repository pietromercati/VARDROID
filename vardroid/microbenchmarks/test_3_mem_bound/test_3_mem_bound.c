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

#define SELECT_BUBBLE   1
#define BUBBLE_LENGTH   3
#define READ_JIFFIES    4


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


#define ODROID_L1_SIZE 	32 	//in kilobytes
#define ODROID_L2_SIZE 	2048  	// in kilobytes
#define ODROID_RAM_SIZE 2  // in gigabytes
// ---------------- MAIN FUNCTION ----------------- //

int main(int argc, char ** argv){
	pid_t pidM ; 	
	int target_cpu = 0;
	int i;
	int j;
	int k;
	int *x;
	int a = 0;
	cpu_set_t mask;
	long unsigned int c2, c1;
	double time_spent;
	int *pt;
	int vec_size;
	int exp_size;
	char *p1;
	char *p2;
	char *p3;
	long unsigned int jiffies_start;
	long unsigned int jiffies_end;
	int fd;
	int num_iterations;
	int t;

	if(argc < 2){
                printf("please provide number of iterations\n");
                exit(0);
        }
        num_iterations = strtol(argv[1], &p1, 10);


	// write vector size and experiment size
	vec_size = ODROID_L2_SIZE*1024*10;
	exp_size = 10;
	

	//allocate a big vector
	pt = (int*)malloc(vec_size*sizeof(int));	
	x = (int*)malloc(exp_size*sizeof(int));   // vector of indeces build to avoid prefetching
	
	//initialize vector
	for (i = 0 ; i < vec_size ; i++ ){
		pt[i] = i;
	}

	
	// initialize vector in indeces
	for ( i = 0 ; i < exp_size ; i ++ ){ 
		// note: we need to make sure that the vector is larger than the L1 cache 
		// then we should read elements far enough from each other so to be sure 
		// that they are in L2 memory (e.g. not prefetched)
//		printf("debug %u %u \n", i, (i*ODROID_L1_SIZE*(1024/4) )%vec_size);
		x[i] = (i*ODROID_L2_SIZE*(1024) )%vec_size;   // note: everything is converted to #int
	}

	// open driver, get jiffies and close driver
        fd = open("/dev/vardroid_interface",O_RDWR);
        if( fd == -1) {
                printf("Monitor driver open error...\n");
                exit(0);
        }
        ioctl(fd, READ_JIFFIES, &jiffies_start);
        close(fd);


	pidM = fork();

        if (pidM != 0) {
                        //printf("debug : pidM = %u\n", pidM);
					
		for (k = 0 ; k < num_iterations ; k ++){
			c1 = clock();

		       	// execute some code
			for (t = 0 ; t < 1000000 ; t++){
				for (i = 0 ; i < exp_size ; i ++){
					//a = pt[i]; //use this to check if there is time difference between the two versions
					a = pt[x[i]];
				}
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
	
	// open driver, get jiffies and close driver
        fd = open("/dev/vardroid_interface",O_RDWR);
        if( fd == -1) {
                printf("Monitor driver open error...\n");
                exit(0);
        }

        ioctl(fd, READ_JIFFIES, &jiffies_end);
        close(fd);


	free(pt);
	free(x);




	return 0;
} 











