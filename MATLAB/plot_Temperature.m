
pre_process_data

% Plot Temperature

figure
xlabel('time [jiffies]');
ylabel('Temperature [^oC]');
hold on
temp_index1 = 5;
temp_index2 = 9;
for i = temp_index1 : temp_index2
  plot(monitor_stats_data_cpu(1:size1(1),2,1), monitor_stats_data_cpu(1:size1(1),i,1)/1000 , colors(i - temp_index1 + 1));
  % resize plot
  x1 = min(monitor_stats_data_cpu(1,2,monitored_cpus));
  x2 = max(max(monitor_stats_data_cpu(:,2,monitored_cpus)));
  xlim([x1 , x2]);
end
hold off
legend('sensor0','sensor1','sensor2','sensor3','sensor4');