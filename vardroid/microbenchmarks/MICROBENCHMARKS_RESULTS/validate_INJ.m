clear all
close all

inp = importdata('INJ_random_validate_test_1_cpu_bound_10000');

outp = importdata('execution_time_validate_INJ_test_1_cpu_bound_1');

for i = 1:length(outp)
if outp(i) > 0.4
outp(i) = 0.3;
end
if outp(i) < 0.2
outp(i) = 0.2;
end
end


figure
subplot(1,2,1)
hist(inp,50)
title('Vardroid INJ')
ylabel('# of occurances')
xlabel('Bubble length')
subplot(1,2,2)
hist(outp,50)
title('Cpu-bound microbenchmark')
ylabel('# of occurances')
xlabel('Execution time [sec]')


%-----------------------------------------------


clear all

inp = importdata('SCHED_random_validate_test_1_cpu_bound_1000');

outp = importdata('execution_time_validate_SCHED_test_1_long_1');

for i = 1:length(outp)
if outp(i) > 1.25
outp(i) = 1.2;
end
if outp(i) < 0.9
outp(i) = 1;
end
end


figure
subplot(1,2,1)
hist(inp,20)
title('Vardroid SCHED Idle')
ylabel('# of occurances')
xlabel('Nanosleep duration [msec]')
subplot(1,2,2)  
hist(outp,20)
title('Cpu-bound long microbenchmark')
ylabel('# of occurances')
xlabel('Execution time [sec]')

%-------------------------------------------------------

clear all

inp = importdata('PERT_random_validate_test_1_cpu_bound_10000');


outp = importdata('execution_time_validate_PERT_test_1_cpu_bound');

for i = 1:length(outp)
if outp(i) > 0.4
outp(i) = 0.3;
end
if outp(i) < 0.15
outp(i) = 0.2;
end
end

figure
subplot(1,2,1)
hist(inp,50)
title('Vardroid PERT')
ylabel('# of occurances')
xlabel('PERT input variation')
subplot(1,2,2)
hist(outp,50)
title('Cpu-bound microbenchmark')
ylabel('# of occurances')
xlabel('Execution time [sec]')