#!/bin/bash


sh switch_on_all_cores.sh
sh select_governor_all_cores.sh performance
sh switch_on_one_little_one_big.sh
echo 0 > /sys/devices/system/cpu/cpu4/online 

# activate monitor and save PID
echo "activate monitor"
./monitor_userspace &
monitor_pid=$!
echo $monitor_pid


# run benchmark
echo "executing test"
./VARDROID/test_migr > time_migr1 &
echo $! > pid1
echo "end test"

echo 1 > /sys/devices/system/cpu/cpu4/online

sleep 100


# kill monitor
kill -9 $monitor_pid

killall test_migr


