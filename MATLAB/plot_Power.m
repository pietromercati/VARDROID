close all

pre_process_data

num_P_sensor = 4;

% monitor_stats_data_cpu(1:size1(1),2,1)

min_jiffy = min(monitor_stats_data_cpu(1,2,monitored_cpus));
max_jiffy = max(max(monitor_stats_data_cpu(:,2,monitored_cpus)));
sizeP = max_jiffy - min_jiffy;

jiffy_axes = min_jiffy+1 : 1 : max_jiffy ;
power = zeros(num_P_sensor, sizeP);

for k = 1:monitored_cpus
  i = monitored_cpus(k);
  for j = 1 : size1(i)
     power(monitor_stats_data_cpu(j,10,i)+1,monitor_stats_data_cpu(j,2,i)-min_jiffy+1) = monitor_stats_data_cpu(j,11,i);
  end 
end



figure
title('Power measures');
xlabel('Time [jiffies]');
ylabel('Power [W]')
hold on
for i = 1: num_P_sensor
  stairs(jiffy_axes, power(i,:)/1000000, colors(i));
end
hold off
legend('arm','kfg','g3d','mem')

