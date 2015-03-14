#!/bin/bash

echo "REMEMBER: PROVIDE THE NUMBER OF ITERATION AND NUMBER OF RANDOM SAMPLES AS ARGUMENT TO THIS SCRIPT !"

sh switch_on_all_cores.sh
echo "Setting performance governor to all cores"
sh select_governor_all_cores.sh performance
sleep 2
echo "Turning off all cores except for core0"
sh switch_on_little.sh
sleep 2
echo "Verify status"
sh show_status_odroid.sh
sleep 2

# define directory and test names
test_dir='VARDROID'

num_random_samples=$1
num_iterations=$2

./random_gaussian_number 500 50 $num_random_samples > SCHED_random

test_name=${test_name_list[0]}

rm ./SCHED_power*
rm ./MICORBENCHMARKS_RESULTS/SCHED_power*


echo "sched ID"
sched_ID=1
echo $sched_ID

while read line
do
	name=$line
	echo "Text read from file - $name"

	# activate monitor and save PID 
	echo "activate monitor" 
	./monitor_userspace & 
	monitor_pid=$! 
	echo $monitor_pid


	sleep_length=${name}
	echo "sleep length"
	echo $sleep_length
	./${test_dir}/vardroid_SCHED $sched_ID $sleep_length &


	for i in `seq 0 $(($num_iterations-1))`
	do
		cat /sys/bus/i2c/drivers/INA231/4-0045/sensor_W >> SCHED_power_${sched_ID}_${sleep_length}
		sleep 1
	done

	
	killall vardroid_SCHED
	
	killall monitor_userspace

	mkdir ./MICROBENCHMARKS_RESULTS/results_microbenchmark_SCHED_power_${sched_ID}_${sleep_length}
	
	cp MONITOR_STATS/monitor_stats_data_cpu_* ./MICROBENCHMARKS_RESULTS/results_microbenchmark_SCHED_power_${sched_ID}_${sleep_length}


done < SCHED_random
	
mv SCHED_power_* ./MICROBENCHMARKS_RESULTS/


echo "Turning all cores back on"
sh switch_on_all_cores.sh

echo "Setting interactive governor back to all cores"
sh select_governor_all_cores.sh interactive

echo "Verify status"
sh show_status_odroid.sh




