#!/bin/bash

echo "REMEMBER: PROVIDE THE NUMBER OF RANDOM SAMPLES AS ARGUMENT TO THIS SCRIPT !"

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

bubble_ID_list=(1)
num_bubble_ID=1

num_iterations=1

num_random_samples=$1

./random_gaussian_number 70 10 $num_random_samples > SCHED_random

test_name=${test_name_list[0]}

rm execution_time_validate_SCHED_${test_name}
	
echo "sched ID"
sched_ID=1
echo $sched_ID


while read line
do
	name=$line
	echo "Text read from file - $name"

	sleep_length=${name}
	echo "sleep length"
	echo $sleep_length
	./${test_dir}/vardroid_SCHED $sched_ID $sleep_length &
	
	# run benchmark
	echo "executing test"
	./${test_dir}/${test_name} $num_iterations >> execution_time_validate_SCHED_${test_name}_${sched_ID}
	echo "end test"

	killall vardroid_SCHED

done < SCHED_random


mv execution_time_validate_SCHED_${test_name}_${sched_ID} ./MICROBENCHMARKS_RESULTS/
cp SCHED_random ./MICROBENCHMARKS_RESULTS/SCHED_random_validate_${test_name}_${num_random_samples}


echo "Turning all cores back on"
sh switch_on_all_cores.sh

echo "Setting interactive governor back to all cores"
sh select_governor_all_cores.sh interactive

echo "Verify status"
sh show_status_odroid.sh




