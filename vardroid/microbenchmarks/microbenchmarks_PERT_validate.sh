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

num_iterations=1

num_random_samples=$1

./random_gaussian_number 10 5 $num_random_samples > PERT_random


test_name=${test_name_list[0]}

rm execution_time_validate_PERT_${test_name}


while read line
do
	name=$line
	echo "Text read from file - $name"

	pert_var=${name}
	echo "pert_var"
	echo $pert_var
	./${test_dir}/vardroid_PERT $pert_var 

	# run benchmark
	echo "executing test"
	./${test_dir}/${test_name} $num_iterations >> execution_time_validate_PERT_${test_name}
	echo "end test"


done < PERT_random


mv execution_time_validate_PERT_${test_name} ./MICROBENCHMARKS_RESULTS/

echo "Turning all cores back on"
sh switch_on_all_cores.sh

echo "Setting interactive governor back to all cores"
sh select_governor_all_cores.sh interactive

echo "Verify status"
sh show_status_odroid.sh




