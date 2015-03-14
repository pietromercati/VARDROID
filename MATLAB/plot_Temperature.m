
pre_process_data

% Plot Temperature


jiffy_axes = monitor_stats_data_cpu(1:size_ref,2,index_ref) ;
min_jiffy = min(jiffy_axes);

% avoid problems related to very large numbers
jiffy_axes = jiffy_axes - min_jiffy;


figure
xlabel('time [jiffies]');
ylabel('Temperature [^oC]');
hold on
temp_index1 = 5;
temp_index2 = 9;
for i = temp_index1 : temp_index2
  plot(jiffy_axes, monitor_stats_data_cpu(1:size_ref,i,index_ref)/1000 , colors(i - temp_index1 + 1));
  % resize plot
  
end
hold off
legend('sensor0','sensor1','sensor2','sensor3','sensor4');