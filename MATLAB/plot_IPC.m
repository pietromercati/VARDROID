

pre_process_data

num_monitored_cpus = length(monitored_cpus);

jiffies_difference     = zeros(NUM_CORES, max(size1) );
instruction_difference = zeros(NUM_CORES, max(size1) );
cycles_difference      = zeros(NUM_CORES, max(size1) );
IPC                    = zeros(NUM_CORES, max(size1) );

%---------------------------------------------------------------------------------

% Plot IPC

for k = 1 : num_monitored_cpus
  i = monitored_cpus(k);
  jiffies_difference(i,1:size1(i)-1)    = monitor_stats_data_cpu(2:size1(i),2,i) - ...
                                          monitor_stats_data_cpu(1:size1(i)-1,2,i) ;
  instruction_difference(i,1:size1(i)-1)= monitor_stats_data_cpu(2:size1(i),4,i) - ...
                                          monitor_stats_data_cpu(1:size1(i)-1,4,i) ;                                        
  cycles_difference(i,1:size1(i)-1)     = monitor_stats_data_cpu(2:size1(i),3,i) - ...
                                          monitor_stats_data_cpu(1:size1(i)-1,3,i) ;
  
  for j = 1 : size1(i) 
    thr = 2^30 ; %threshold to understand if there was register overflow
    if jiffies_difference(i,j) < 0
      monitor_stats_data_cpu(j+1:size1(i),2,i) = monitor_stats_data_cpu(j+1:size1(i),2,i) + 2^32 ;
    end
    
%    if cycles_difference(i,j) < 0
%        cycles_difference(i,j) = cycles_difference(i,j) + 2^32 ;
%    end
%    
%    if instruction_difference(i,j) < 0 
%        instruction_difference(i,j) = instruction_difference(i,j) + 2^32 ;
%    end
  
  end
                                    
  IPC (i,1:size1(i)-1) = instruction_difference(i,1:size1(i)-1) ./ cycles_difference(i,1:size1(i)-1) ;                 
end

% LITTLE
figure
title ('Cluster LITTLE')
xlabel('time [jiffies]');
ylabel('IPC');
hold on
for k = 1 : num_monitored_cpus
  i = monitored_cpus(k);                              %quantity to plot%
  if i > 4 
    break;
  end
  stairs(monitor_stats_data_cpu(1:size1(i),2,i),   IPC(i, 1:size1(i))  , colors(i));
  % resize plot
  x1 = min(monitor_stats_data_cpu(1,2,monitored_cpus));
  x2 = max(max(monitor_stats_data_cpu(:,2,monitored_cpus)));
  y1 = 0 ;
  y2 = 3 ;
  xlim([x1 , x2]);
  ylim([y1 , y2]);
end
hold off
legend('core0','core1','core2','core3');


% big
figure
title('Cluster big')
xlabel('time [jiffies]');
ylabel('IPC');
hold on
for k = 1 : num_monitored_cpus
  i = monitored_cpus(k);                              %quantity to plot%
  if i >= 4 
     
    stairs(monitor_stats_data_cpu(1:size1(i),2,i),  IPC(i, 1:size1(i))  , colors(i));
    
    % resize plot
    x1 = min(monitor_stats_data_cpu(1,2,monitored_cpus));
    x2 = max(max(monitor_stats_data_cpu(:,2,monitored_cpus)));
    y1 = 0 ;
    y2 = 3 ;
    xlim([x1 , x2]);
    ylim([y1 , y2]);
  end
end
hold off
legend('core4','core5','core6','core7');



%--------------------------------------------------------------------------------------------------------------------

