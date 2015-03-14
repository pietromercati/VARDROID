





clear all
close all




sched_rand = importdata('SCHED_random');

for i = 1 : length(sched_rand)
file_name = strcat('SCHED_power_1_', num2str(sched_rand(i)));
p = importdata(file_name);
power(i,:) = p(1:10);
end



for i = 1 : length(power(:,1))

avg_power(i) = sum(power(i,:)/length(power(i,:)));

end







figure
subplot(1,2,1)
hist(sched_rand,15)
title('Vardroid SCHED Power')
ylabel('# of occurances')
xlabel('Nanosleep duration [msec]')
subplot(1,2,2)  
hist(avg_power,15)
title('Power variability - LITTLE cluster')
ylabel('# of occurances')
xlabel('Power [W]')
