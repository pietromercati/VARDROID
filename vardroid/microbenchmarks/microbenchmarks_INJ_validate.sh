#!/bin/bash

echo "REMEMBER: PROVIDE THE NUMBER OF RANDOM SAMPLES AS ARGUMENT TO THIS SCRIPT !"

#--------------------------------------------------

# PART 1 : test of INJ (e.g. bubbles)
# output : 
# 	- one set of monitor files per each microbenchmark run
# 	- one value of average execution time per each microbenchmark run

sh switch_on_all_cores.sh
echo "Setting performance governor to all cores"
sh select_governor_all_cores.sh performance
sleep 2
echo "Turning off all cores except for core0"
sh switch_on_two_cores.sh
sleep 2
echo "Verify status"
sh show_status_odroid.sh
sleep 2

# define directory and test names
test_dir='VARDROID'
echo $test_dir
test_name_list=('test_1_cpu_bound' 'test_2_L2_bound' 'test_3_mem_bound')
num_test_name=3

bubble_ID=1

num_iterations=1

num_random_samples=$1

# use ./random_gaussian_number 100000 10000 10 > INJ_random_bubble to generate a file with random bubble durations
./random_gaussian_number 100000 10000 $num_random_samples > INJ_random


test_name=${test_name_list[0]}

rm execution_time_validate_INJ_${test_name}_${bubble_ID}

#activate monitor and save PID
#echo "activate monitor"
#./monitor_userspace &
#monitor_pid=$!
#echo $monitor_pid

while read line
do
    	name=$line
	echo "Text read from file - $name"

	bubble_length=${name}
	echo "bubble length"
	echo $bubble_length
	./${test_dir}/vardroid_bubble_selector $bubble_ID $bubble_length 


	# run benchmark
	echo "executing test"
	./${test_dir}/${test_name} $num_iterations >> execution_time_validate_INJ_${test_name}_${bubble_ID}
	echo "end test"


done < INJ_random

#kill -9 $monitor_pid
#mkdir results_microbenchmark_validate_INJ_${test_name}_${bubble_ID}
#cp MONITOR_STATS/monitor_stats_data_cpu_* ./results_microbenchmark_validate_INJ_${test_name}_${bubble_ID}

echo "Restoring normal bubble"
./$test_dir/vardroid_bubble_selector 0 1

mv results_microbenchmark_validate_INJ_* ./MICROBENCHMARKS_RESULTS
cp  INJ_random ./MICROBENCHMARKS_RESULTS/INJ_random_validate_${test_name}_${num_random_samples}


echo "Turning all cores back on"
sh switch_on_all_cores.sh
echo "Setting interactive governor back to all cores"
sh select_governor_all_cores.sh interactive
echo "Verify status"
sh show_status_odroid.sh




