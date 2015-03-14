#!/bin/bash

echo "REMEMBER: PROVIDE THE NUMBER OF ITERATION AND NUMBER OF RANDOM SAMPLES AS ARGUMENT TO THIS SCRIPT !"

sh switch_on_all_cores.sh
echo "Setting performance governor to all cores"
sh select_governor_all_cores.sh performance
sleep 2
echo "Verify status"
sh show_status_odroid.sh
sleep 2

# define directory and test names
test_dir='VARDROID'

num_random_samples=$1
num_iterations=$2

./random_gaussian_number 600 110 $num_random_samples > SCHED_random

test_name=${test_name_list[0]}

rm ./SCHED_power*
rm ./MICROBENCHMARKS_RESULTS/SCHED_power_big/*


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
	./${test_dir}/vardroid_SCHED_big $sched_ID $sleep_length &


	for i in `seq 0 $(($num_iterations-1))`
	do
		cat /sys/bus/i2c/drivers/INA231/4-0041/sensor_W >> SCHED_power_big_${sched_ID}_${sleep_length}
		sleep 1
	done

	
	killall vardroid_SCHED_big
	

done < SCHED_random
	
mv SCHED_power_big* ./MICROBENCHMARKS_RESULTS/SCHED_power_big/
mv SCHED_random ./MICROBENCHMARKS_RESULTS/SCHED_power_big/

echo "Turning all cores back on"
sh switch_on_all_cores.sh

echo "Setting interactive governor back to all cores"
sh select_governor_all_cores.sh interactive

echo "Verify status"
sh show_status_odroid.sh




