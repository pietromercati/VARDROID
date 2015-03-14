





clear all
close all




sched_rand = importdata('SCHED_random');

for i = 1 : length(sched_rand)
file_name = strcat('SCHED_power_1_', num2str(sched_rand(i)));
p = importdata(file_name);
power(i,:) = p(1:10);
end

for i = 1 : length(sched_rand)
file_name = strcat('execution_time_SCHED_power_1_', num2str(sched_rand(i)));
x = importdata(file_name);
exec_time(i,:) = x(1:5);
end


for i = 1 : length(power(:,1))

avg_power(i) = sum(power(i,:)/length(power(i,:)));

end


for i = 1 : length(exec_time(:,1))
avg_exec_time(i) = sum(exec_time(i,:)/length(exec_time(i,:)));
end





figure
subplot(1,3,1)
hist(sched_rand,15)
title('Vardroid SCHED Power')
ylabel('# of occurances')
xlabel('Nanosleep duration [msec]')
subplot(1,3,2)  
hist(avg_power,15)
title('Power variability')
ylabel('# of occurances')
xlabel('Power [W]')
subplot(1,3,3)  
hist(avg_exec_time,15)
title('Cpu-bound microbenchmark')
ylabel('# of occurances')
xlabel('Execution time [sec]')
