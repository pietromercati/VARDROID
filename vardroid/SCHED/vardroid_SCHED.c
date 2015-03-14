       
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
#include <time.h>

// Macros for ioctl
#define SELECT_BUBBLE 		1
#define SELECT_BUBBLE_LENGTH 	3
#define READ_JIFFIES		4

#define ODROIDXU3
//#define DEBUG

//#define IDLE


#ifdef ODROIDXU3
#endif

// #define DEBUG_TIME

#define NANO_SECOND_MULTIPLIER  1000000  // 1 millisecond = 1,000,000 Nanoseconds

#define LENGTH_SCHED		1000000   // 50000000 is ~1 sec exec time with 2 little cores runnign at maw freq


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

void long_nop();
/*
#define NANO_SECOND_MULTIPLIER  1000000  // 1 millisecond = 1,000,000 Nanoseconds
const long INTERVAL_MS = 500 * NANO_SECOND_MULTIPLIER;
Hence my code would look like this:

timespec sleepValue = {0};

sleepValue.tv_nsec = INTERVAL_MS;
nanosleep(&sleepValue, NULL);

*/

// sleep sched
void execute_idle_sched(int sleep_duration){
	int i;
	long unsigned int c1;
	long unsigned int c2;
	long unsigned int c3;
	struct timespec sleepValue = {0};
	double time_spent12;
	double time_spent13;
	double time_spent23;

	sleepValue.tv_nsec = sleep_duration*NANO_SECOND_MULTIPLIER;

	while(1){
		// sleep for a while

		#ifdef DEBUG_TIME
		c1 = clock();
		#endif

		nanosleep(&sleepValue, NULL);
		#ifdef DEBUG_TIME
		c2 = clock();
		#endif
		

		for(i = 0 ; i < LENGTH_SCHED ; i++){
			asm volatile ("mov r0, r0");
			asm volatile ("mov r0, r0");
			asm volatile ("mov r0, r0");
			asm volatile ("mov r0, r0");
			asm volatile ("mov r0, r0");
			asm volatile ("mov r0, r0");
			asm volatile ("mov r0, r0");
			asm volatile ("mov r0, r0");
			asm volatile ("mov r0, r0");
			asm volatile ("mov r0, r0");
			asm volatile ("mov r0, r0");
			asm volatile ("mov r0, r0");
			asm volatile ("mov r0, r0");
			asm volatile ("mov r0, r0");
			asm volatile ("mov r0, r0");
			asm volatile ("mov r0, r0");
			asm volatile ("mov r0, r0");
			asm volatile ("mov r0, r0");
			asm volatile ("mov r0, r0");
			asm volatile ("mov r0, r0");
		}
		

		#ifdef DEBUG_TIME
		c3 = clock();
		
		time_spent12 = (double)(c2 - c1) / CLOCKS_PER_SEC;
		time_spent13 = (double)(c3 - c1) / CLOCKS_PER_SEC;
		time_spent23 = (double)(c3 - c2) / CLOCKS_PER_SEC;
                printf("Execution Time : 12 = %f , 13 = %f , 23 =%f\n", time_spent12,  time_spent13, time_spent23);
		#endif
	}

}


// power sched
void execute_power_sched(int sleep_duration){
	int i;
	long unsigned int c1;
	long unsigned int c2;
	long unsigned int c3;
	struct timespec sleepValue = {0};
	double time_spent12;
	double time_spent13;
	double time_spent23;

	sleepValue.tv_nsec = sleep_duration*NANO_SECOND_MULTIPLIER;

	while(1){

		// sleep for a while
		#ifdef DEBUG_TIME
		c1 = clock();
		#endif

		nanosleep(&sleepValue, NULL);
		#ifdef DEBUG_TIME
		c2 = clock();
		#endif
		
		for(i = 0 ; i < LENGTH_SCHED ; i++){
	                asm volatile ("mul r8, r7");
	                asm volatile ("mul r6, r5");
	                asm volatile ("mul r8, r7");
	                asm volatile ("mul r6, r5");
	                asm volatile ("mul r4, r3");
			asm volatile ("mul r8, r7");
	                asm volatile ("mul r6, r5");
	                asm volatile ("mul r4, r3");
			asm volatile ("mul r8, r7");
	                asm volatile ("mul r6, r5");
	                asm volatile ("mul r4, r3");
			asm volatile ("mul r8, r7");
	                asm volatile ("mul r6, r5");
	                asm volatile ("mul r4, r3");
			asm volatile ("mul r4, r3");
		}
		#ifdef DEBUG_TIME
		c3 = clock();
		
		time_spent12 = (double)(c2 - c1) / CLOCKS_PER_SEC;
		time_spent13 = (double)(c3 - c1) / CLOCKS_PER_SEC;
		time_spent23 = (double)(c3 - c2) / CLOCKS_PER_SEC;
                printf("Execution Time : 12 = %f , 13 = %f , 23 =%f\n", time_spent12,  time_spent13, time_spent23);
		#endif	
	}
}



void child_proc(int sched_type, int sleep_duration){
			switch(sched_type) {
				case 0: // empty sched
					break;
				case 1: // idle sched
					execute_idle_sched(sleep_duration);
					break;
				case 2:	// power sched
					execute_power_sched(sleep_duration);
					break;
				default:
					printf("Insert a valid number (0=empty, 1=idle, 2=power)");
					exit(0);
					break;
			}
}
// ---------------- MAIN FUNCTION ----------------- //

int main(int argc, char ** argv){
	char *p1;
	char *p2;
	int sched_type;
	int sleep_duration;
	int i;
	int target_cpu;	
	int num_active_cores = 4;
	int list_index_active_cores[]={0,1,2,3};
	//int list_index_active_cores[]={4,5,6,7};
	
	pid_t pidM[2];
	pid_t pid;
	cpu_set_t mask;
	struct WORK_DESCRIPTION {
    		int childpid;
		int index;
		} work[2];


	if (argc<2){
		printf("provide the sched type (0=empty, 1=idle, 2=power) and the sleep duration (in millisec)");
		exit(0);	
	}
	// get arguments : sched type (idle or power) sleep duration
	sched_type 	= strtol(argv[1], &p1, 10);
	sleep_duration 	= strtol(argv[2], &p2, 10);


	for (i = 0 ; i < num_active_cores ; i++){

		target_cpu = list_index_active_cores[i];

		// sched
		pid = fork();
		if (pid == 0){
			CPU_ZERO(&mask);
			CPU_SET(target_cpu, &mask);
			setCurrentThreadAffinityMask(mask);
			//printf("This is process %u  running on core %u\n", getpid(), target_cpu);
			child_proc(sched_type, sleep_duration);	
		} //end of son process
		else{
			continue;
		}

	}

	wait(NULL);

	return 0;
} 





