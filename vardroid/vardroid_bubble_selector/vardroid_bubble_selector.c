    
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

#include <errno.h>   // for errno
#include <limits.h>  // for INT_MAX
#include <stdlib.h>  // for strtol


#define DEBUG // enables printf for debugging

// Macros for ioctl
#define SELECT_BUBBLE 		1
#define SELECT_BUBBLE_LENGTH 	3

// ---------------- MAIN FUNCTION ----------------- //

int main(int argc, char ** argv){
	int a, b;
	int fd;	
	int bubble;	
	int bubble_length;
	char *p1;
	char *p2;

	// get the bubble from user
	if (argc < 3){
		printf("Please select a Vardroid bubble and bubble length\n");
		exit(1);
	}
	bubble = strtol(argv[1], &p1, 10);
	bubble_length = strtol(argv[2], &p2, 10);

	printf("select bubble %u with length %u\n", bubble, bubble_length);
	// open vardroid driver
	fd = open("/dev/vardroid_interface",O_RDWR);
	if( fd == -1) {
                printf("Monitor driver open error...\n");
                exit(0);
        }

	// select bubble
	a = ioctl(fd, SELECT_BUBBLE, &bubble);
	close (fd);

	// select bubble length
	fd = open("/dev/vardroid_interface",O_RDWR);
	if( fd == -1) {
                printf("Monitor driver open error...\n");
                exit(0);
        }

	b = ioctl(fd, SELECT_BUBBLE_LENGTH, &bubble_length);
	
	close(fd);
	
	printf("a = %u, b = %u\n",a,b);
	// exit 
	return 0;
} 











