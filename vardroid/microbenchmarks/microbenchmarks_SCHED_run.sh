#!/bin/bash

echo "REMEMBER: PROVIDE THE NUMBER OF ITERATION AS ARGUMENT TO THIS SCRIPT !"

# PRELIMINARY
sh switch_on_all_cores.sh
echo "Setting performance governor to all cores"
sh select_governor_all_cores.sh performance
echo "Turning off all cores except for core0 and core1"
sh switch_on_two_cores.sh
echo "Verify status"
sh show_status_odroid.sh

test_dir='VARDROID'
echo $test_dir


#--------------------------------------------------

# PART 2 : test of SCHED 
# output : 
# 	- one set of monitor files per each microbenchmark run
# 	- one value of average execution time per each microbenchmark run



# define directory and test names
test_name_list=('test_1_cpu_bound' 'test_2_L2_bound' 'test_3_mem_bound')
num_test_name=3

sleep_length_list=(200 150 100 50)
num_sleep_lengths=4 #this in general has to be equal to the length of bubble_length_list

sched_ID_list=(1 2)
num_sched_ID=2

num_iterations=$1


# for each test (microbenchmark) : (test_name_list)
for k in `seq 0 $((num_test_name-1))`;
do
	test_name=${test_name_list[$k]}
	echo $test_name	

	# for each configuration (e.g. sched program) : (sched_ID_list)
	for j in `seq 0 $((num_sched_ID-1))`;
	do
	
		echo "sched ID"
		sched_ID=${sched_ID_list[$j]}
		echo $sched_ID
		# for each sleep length :  (sleep_length_list)

		for i  in `seq 0 $((num_sleep_lengths-1))`;
        	do
			sleep_length=${sleep_length_list[$i]}
			echo "sleep length"
			echo $sleep_length

			# insert vardroid_SCHED here
			echo "launch sched"
			./VARDROID/vardroid_SCHED $sched_ID $sleep_length &
			sched_pid=$!
			echo $sched_pid

	
			# activate monitor and save PID
			#echo "activate monitor"
			#./monitor_userspace &
			#monitor_pid=$!
			#echo $monitor_pid


			# run benchmark
			echo "executing test"
			./${test_dir}/${test_name} $num_iterations > execution_time_SCHED_${test_name}_${sched_ID}_${sleep_length}
			echo "end test"


			# kill monitor
			#kill -9 $monitor_pid

			#mkdir ./MICROBENCHMARKS_RESULTS/results_microbenchmark_SCHED_${test_name}_${sched_ID}_${sleep_length}
			
			#cp MONITOR_STATS/monitor_stats_data_cpu_* ./MICROBENCHMARKS_RESULTS/results_microbenchmark_SCHED_${test_name}_${sched_ID}_${sleep_length}

			killall vardroid_SCHED
		done
	done
done


mv execution_time_SCHED_* ./MICROBENCHMARKS_RESULTS

#----------------------------------------------------

# PART 2 : test of SCHED

#----------------------------------------------------

echo "Turning all cores back on"
sh switch_on_all_cores.sh

echo "Setting interactive governor back to all cores"
sh select_governor_all_cores.sh interactive

echo "Verify status"
sh show_status_odroid.sh




