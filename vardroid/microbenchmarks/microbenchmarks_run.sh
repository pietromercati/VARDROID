#!/bin/bash

echo "REMEMBER: PROVIDE THE NUMBER OF ITERATION AS ARGUMENT TO THIS SCRIPT !"

# PRELIMINARY
echo "Setting performance governor to all cores"
sh select_governor_all_cores.sh performance
sleep 1
echo "Turning off all cores except for core0"
sh switch_on_two_cores.sh
sleep 1
echo "Verify status"
sh show_status_odroid.sh
sleep 1

test_dir='VARDROID'
echo $test_dir


#--------------------------------------------------

# PART 1 : test of INJ (e.g. bubbles)
# output : 
# 	- one set of monitor files per each microbenchmark run
# 	- one value of average execution time per each microbenchmark run



# define directory and test names
test_name_list=('test_1_cpu_bound' 'test_2_L2_bound' 'test_3_mem_bound')
num_test_name=3

bubble_length_list=(1 50000 100000 150000)
num_bubble_lengths=4 #this in general has to be equal to the length of bubble_length_list

bubble_ID_list=(0 1 2)
num_bubble_ID=3

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

			sleep 2
	
			# activate monitor and save PID
			echo "activate monitor"
			./monitor_userspace &
			monitor_pid=$!
			echo $monitor_pid

			sleep 1		

			# run benchmark
			echo "executing test"
			./${test_dir}/${test_name} $num_iterations
			echo "end test"

			sleep 1

			# kill monitor
			kill -9 $monitor_pid

			mkdir results_microbenchmark_INJ_${test_name}_${bubble_ID}_${bubble_length}
			
			cp MONITOR_STATS/monitor_stats_data_cpu_* ./results_microbenchmark_INJ_${test_name}_${bubble_ID}_${bubble_length}

		done
		sleep 1 
	done
	sleep 1
done

echo "Restoring normal bubble"
./$test_dir/vardroid_bubble_selector 0 1

mv results_microbenchmark_INJ_* ./MICROBENCHMARKS_RESULTS

#----------------------------------------------------

# PART 2 : test of SCHED


#----------------------------------------------------

# PART 3 : test of PERT

PERT_variability_list=(0 5 10 15 20)

# for each test (microbenchmark) : (test_name_list)
for k in `seq 0 $((num_test_name-1))`;
do
	test_name=${test_name_list[$k]}
	echo $test_name	

	for i in `seq 0 $((num_PERT_variability-1))`;
	do
		PERT_variability=${PERT_variability_list[$i]}

		# invoke vardroid_PERT
		./${test_dir}/vardroid_PERT $PERT_variability		

		sleep 2
	
		# activate monitor and save PID
		echo "activate monitor"
		./monitor_userspace &
		monitor_pid=$!
		echo $monitor_pid
		sleep 1	

		# run benchmark
		echo "executing test"
		./${test_dir}/${test_name} $num_iterations
		echo "end test"

		sleep 1
		# kill monitor
		kill -9 $monitor_pid

		mkdir results_microbenchmark_PERT_${test_name}_${PERT_variability}
			
		cp MONITOR_STATS/monitor_stats_data_cpu_* ./results_microbenchmark_PERT_${test_name}_${PERT_variability}


	done

done

#----------------------------------------------------

echo "Turning all cores back on"
sh switch_on_all_cores.sh

echo "Setting interactive governor back to all cores"
sh select_governor_all_cores.sh interactive

echo "Verify status"
sh show_status_odroid.sh




