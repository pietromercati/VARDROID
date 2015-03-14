

% INJ -------------------------------------------------

clear all
close all

bubble_length_list = [0 50000 100000 150000];
num_bubble_length = length(bubble_length_list);
bubble_ID_list = [1];
num_bubble_ID = length(bubble_ID_list);
test_name_list=char('test_1_cpu_bound' ,'test_2_L2_bound' ,'test_3_mem_bound')
num_length_list= length(test_name_list(:,1));


for i = 1 : num_length_list
for j = 1 : num_bubble_ID
for k = 1 : num_bubble_length

  file_name = strcat('execution_time_INJ_',test_name_list(i,:), '_' , num2str(bubble_ID_list(j)) , '_' , num2str(bubble_length_list(k))  ) 
  exec_time(i,j,k,:) = importdata(file_name);
  
end
end
end 

for i = 1 : num_length_list
for j = 1 : num_bubble_ID
for k = 1 : num_bubble_length

avg_exec_time(i,j,k) = sum(exec_time(i,j,k,:)) / length(exec_time(i,j,k,:)); 

end
end
end


colors = ["r","k","b","g"];

for i = 1 : num_bubble_ID
figure
hold on
for j = 1 : num_length_list

plot(bubble_length_list,avg_exec_time(j,i,:)/avg_exec_time(j,1,1),colors(j));

end
hold off
title('VarDroid INJ')
ylabel('Norm. execution time')
xlabel('Bubble length')
legend('cpu-bound' ,'L2-bound' ,'mem-bound');
end

% SCHED -------------------------------------------------

clear all

bubble_length_list = [50 100 150];
num_bubble_length = length(bubble_length_list);
bubble_ID_list = [1];
num_bubble_ID = length(bubble_ID_list);
test_name_list=char('test_1_cpu_bound' ,'test_2_L2_bound' ,'test_3_mem_bound')
num_length_list= length(test_name_list(:,1));


for i = 1 : num_length_list
for j = 1 : num_bubble_ID
for k = 1 : num_bubble_length

  file_name = strcat('execution_time_SCHED_',test_name_list(i,:), '_' , num2str(bubble_ID_list(j)) , '_' , num2str(bubble_length_list(k))  ) 
  exec_time(i,j,k,:) = importdata(file_name);
  
end
end
end 

for i = 1 : num_length_list
for j = 1 : num_bubble_ID
for k = 1 : num_bubble_length

avg_exec_time(i,j,k) = sum(exec_time(i,j,k,:)) / length(exec_time(i,j,k,:)); 

end
end
end


colors = ["r","k","b","g"];

for i = 1 : num_bubble_ID
figure
hold on
for j = 1 : num_length_list

plot(bubble_length_list,avg_exec_time(j,i,:)/avg_exec_time(j,1,1),colors(j));

end
hold off
title('VarDroid SCHED')
ylabel('Norm. execution time')
xlabel('Nanosleep duration [msec]')
legend('cpu-bound' ,'L2-bound' ,'mem-bound');
end


% PERT -------------------------------------------------
% SCHED -------------------------------------------------

clear all

bubble_length_list = [0 10 20];
num_bubble_length = length(bubble_length_list);
bubble_ID_list = [1];
num_bubble_ID = length(bubble_ID_list);
test_name_list=char('test_1_cpu_bound' ,'test_2_L2_bound' ,'test_3_mem_bound')
num_length_list= length(test_name_list(:,1));


for i = 1 : num_length_list
for j = 1 : num_bubble_ID
for k = 1 : num_bubble_length

  file_name = strcat('execution_time_PERT_',test_name_list(i,:), '_'  , num2str(bubble_length_list(k))  ) 
  exec_time(i,j,k,:) = importdata(file_name);
  
end
end
end 

for i = 1 : num_length_list
for j = 1 : num_bubble_ID
for k = 1 : num_bubble_length

avg_exec_time(i,j,k) = sum(exec_time(i,j,k,:)) / length(exec_time(i,j,k,:)); 

end
end
end


colors = ["r","k","b","g"];

for i = 1 : num_bubble_ID
figure
hold on
for j = 1 : num_length_list

plot(bubble_length_list,avg_exec_time(j,i,:)/avg_exec_time(j,1,1),colors(j));

end
hold off
title('VarDroid PERT')
ylabel('Norm. execution time')
xlabel('PERT input variation')
legend('cpu-bound' ,'L2-bound' ,'mem-bound');
end

