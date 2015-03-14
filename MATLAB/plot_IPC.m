

pre_process_data

num_monitored_cpus = length(monitored_cpus);

jiffies_difference     = zeros(NUM_CORES, max(size1) );
instruction_difference = zeros(NUM_CORES, max(size1) );
cycles_difference      = zeros(NUM_CORES, max(size1) );
IPC                    = zeros(NUM_CORES, max(size1) );
freq_MHZ               = zeros(NUM_CORES, max(size1) );

%adjust jiffies

jiffy_axes = monitor_stats_data_cpu(1:size_ref,2,index_ref) ;
min_jiffy = min(jiffy_axes);

% avoid problems related to very large numbers
jiffy_axes = jiffy_axes - min_jiffy;


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
    
    if cycles_difference(i,j) < 0
        cycles_difference(i,j) = cycles_difference(i,j) + 2^32 ;
    end
    
    if instruction_difference(i,j) < 0 
        instruction_difference(i,j) = instruction_difference(i,j) + 2^32 ;
    end
  
  end
                                    
  IPC (i,1:size1(i)-1)      = instruction_difference(i,1:size1(i)-1) ./ cycles_difference(i,1:size1(i)-1) ;                 
  freq_MHZ (i,1:size1(i)-1) = ( cycles_difference(i,1:size1(i)-1)  ./  jiffies_difference(i,1:size1(i)-1) ) / 10000;
  %brutally force to 0 stuff that spoils the plot
  for k = 1:size1(i)
    if IPC(i,k) < 0
      IPC(i,k) = 0;
    end
  end
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
  stairs(jiffy_axes(1:size1(i)),   IPC(i, 1:size1(i))  , colors(i));
  % resize plot
  x1 = min(monitor_stats_data_cpu(1,2,monitored_cpus));
  x2 = max(max(monitor_stats_data_cpu(:,2,monitored_cpus)));
  y1 = 0 ;
  y2 = 3 ;
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
  if i > 4 
     
    stairs(jiffy_axes(1:size1(i)),  IPC(i, 1:size1(i))  , colors(i));
    
    % resize plot
    x1 = min(monitor_stats_data_cpu(1,2,monitored_cpus));
    x2 = max(max(monitor_stats_data_cpu(:,2,monitored_cpus)));
    y1 = 0 ;
    y2 = 3 ;
    
  end
end
hold off
legend('core4','core5','core6','core7');



%--------------------------------------------------------------------------------------------------------------------

