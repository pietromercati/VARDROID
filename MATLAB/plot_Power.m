close all

pre_process_data

num_P_sensor = 4;

% monitor_stats_data_cpu(1:size1(1),2,1)

min_jiffy = min(monitor_stats_data_cpu(1,2,monitored_cpus));
max_jiffy = max(max(monitor_stats_data_cpu(:,2,monitored_cpus)));
sizeP = max_jiffy - min_jiffy;

jiffy_axes = min_jiffy+1: 1 : max_jiffy ;

% avoid problems related to very large numbers
jiffy_axes = jiffy_axes - min_jiffy;

power = zeros(num_P_sensor, sizeP);

for k = 1:monitored_cpus
  i = monitored_cpus(k);
  for j = 1 : size1(i)
     power(monitor_stats_data_cpu(j,10,i)+1,monitor_stats_data_cpu(j,2,i)-min_jiffy+1) = monitor_stats_data_cpu(j,11,i);
  end 
end


% smooth power trace (e.g put all zero values equal to the previous non zero)  
last_non_zero_pow = 0;
for i = 1:num_P_sensor
for j = 1:sizeP 
  if (power(i,j)~=0)
    last_non_zero_pow = power(i,j);
  end

  if power(i,j) == 0
    power(i,j) = last_non_zero_pow;
  end

end  
end

power_average = zeros(1,num_P_sensor);

for i = 1:num_P_sensor
   power_average(i) = sum(power(i,:)) / length(power(i,:)) ;
end

figure
title('Power measures');
xlabel('Time [jiffies]');
ylabel('Power [W]')
hold on
for i = 1: num_P_sensor
  stairs(jiffy_axes, power(i,1:length(jiffy_axes))/1000000, colors(i));
end
hold off
legend('arm','kfg','g3d','mem')

figure
labels ={'arm','kfg','g3d','mem'};
bar(power_average/1000000);
set(gca, 'XTick', 1:num_P_sensor, 'XTickLabel', labels);
ylabel('Average Power [W]')
title('Power measures');





