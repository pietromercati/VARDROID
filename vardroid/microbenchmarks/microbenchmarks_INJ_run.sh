#!/bin/bash

echo "REMEMBER: PROVIDE THE NUMBER OF ITERATION AS ARGUMENT TO THIS SCRIPT !"

#--------------------------------------------------

# PART 1 : test of INJ (e.g. bubbles)
# output : 
# 	- one set of monitor files per each microbenchmark run
# 	- one value of average execution time per each microbenchmark run

sh switch_on_all_cores
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

bubble_length_list=(0 50000 100000 150000)
num_bubble_lengths=4 #this in general has to be equal to the length of bubble_length_list

bubble_ID_list=(1)
num_bubble_ID=1

num_iterations=$1


# for each test (microbenchmark) : (test_name_list)
for k in `seq 0 $((num_test_name-1))`;
do
	test_name=${test_name_list[$k]}
	echo $test_name	

	# for each configuration (e.g. bubble) : (bubble_ID_list)
	for j in `seq 0 $((num_bubble_ID-1))`;
	do
	
		echo "bubble ID"
		bubble_ID=${bubble_ID_list[$j]}
		echo $bubble_ID
		# for each bubble length :  (bubble_length_list)
		for i  in `seq 0 $((num_bubble_lengths-1))`;
        	do
               
			bubble_length=${bubble_length_list[$i]}
			echo "bubble length"
			echo $bubble_length
			./${test_dir}/vardroid_bubble_selector $bubble_ID $bubble_length 

	
			# activate monitor and save PID
			#echo "activate monitor"
			#./monitor_userspace &
			#monitor_pid=$!
			#echo $monitor_pid


			# run benchmark
			echo "executing test"
			./${test_dir}/${test_name} $num_iterations > execution_time_INJ_${test_name}_${bubble_ID}_${bubble_length}
			echo "end test"


			# kill monitor
			#kill -9 $monitor_pid

			#mkdir ./MICROBENCHMARKS_RESULTS/results_microbenchmark_INJ_${test_name}_${bubble_ID}_${bubble_length}
			
			#cp MONITOR_STATS/monitor_stats_data_cpu_* ./MICROBENCHMARKS_RESULTS/results_microbenchmark_INJ_${test_name}_${bubble_ID}_${bubble_length}

		done
	done
done

echo "Restoring normal bubble"
./$test_dir/vardroid_bubble_selector 0 1

mv execution_time_INJ_* ./MICROBENCHMARKS_RESULTS


echo "Turning all cores back on"
sh switch_on_all_cores.sh

echo "Setting interactive governor back to all cores"
sh select_governor_all_cores.sh interactive

echo "Verify status"
sh show_status_odroid.sh




